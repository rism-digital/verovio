/////////////////////////////////////////////////////////////////////////////
// Name:        fontstore.cpp
// Author:      Simon Waloschek
// Created:     2026
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "fontstore.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <optional>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

//----------------------------------------------------------------------------

#include "filereader.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "hb-ot.h"
#include "hb.h"
#include "jsonxx.h"
#include "woff2/decode.h"

namespace vrv {

namespace {

    /** The maximum size of a registered font file */
    constexpr int MAX_FONT_INPUT = 32 * 1024 * 1024;
    /** The maximum size of a decompressed WOFF or WOFF2 font */
    constexpr int MAX_DECODED_FONT = 64 * 1024 * 1024;
    /** The maximum number of tables in a font */
    constexpr int MAX_TABLES = 256;

    //----------------------------------------------------------------------------
    // Binary helpers
    //----------------------------------------------------------------------------

    /**
     * @name Read and write big-endian values
     */
    ///@{
    uint16_t ReadU16(const unsigned char *data)
    {
        return (static_cast<uint16_t>(data[0]) << 8) | data[1];
    }

    uint32_t ReadU32(const unsigned char *data)
    {
        return (static_cast<uint32_t>(data[0]) << 24) | (static_cast<uint32_t>(data[1]) << 16)
            | (static_cast<uint32_t>(data[2]) << 8) | data[3];
    }

    void WriteU16(unsigned char *data, uint16_t value)
    {
        data[0] = static_cast<unsigned char>(value >> 8);
        data[1] = static_cast<unsigned char>(value);
    }

    void WriteU32(unsigned char *data, uint32_t value)
    {
        data[0] = static_cast<unsigned char>(value >> 24);
        data[1] = static_cast<unsigned char>(value >> 16);
        data[2] = static_cast<unsigned char>(value >> 8);
        data[3] = static_cast<unsigned char>(value);
    }
    ///@}

    /** Round up to the next multiple of 4, as required for SFNT table offsets */
    size_t Align4(size_t value)
    {
        return (value + 3U) & ~size_t(3U);
    }

    /** Return a non-zero 64-bit hash of the data */
    uint64_t HashBytes(const unsigned char *data, size_t length)
    {
        auto mix = [](uint64_t value) {
            value ^= value >> 33;
            value *= 0xff51afd7ed558ccdULL;
            value ^= value >> 33;
            value *= 0xc4ceb9fe1a85ec53ULL;
            return value ^ (value >> 33);
        };
        const size_t originalLength = length;
        uint64_t hash = 0x9e3779b97f4a7c15ULL ^ originalLength;
        while (length >= sizeof(uint64_t)) {
            uint64_t word = 0;
            std::memcpy(&word, data, sizeof(word));
            hash ^= mix(word + 0x9e3779b97f4a7c15ULL);
            hash = (hash << 27) | (hash >> 37);
            hash = hash * 5 + 0x52dce729;
            data += sizeof(word);
            length -= sizeof(word);
        }
        uint64_t tail = 0;
        std::memcpy(&tail, data, length);
        const uint64_t result = mix(hash ^ tail);
        return result ? result : 1;
    }

    /** Add the data to an FNV-1a hash */
    void AddToHash(uint64_t &hash, const unsigned char *data, size_t length)
    {
        for (size_t i = 0; i < length; ++i) {
            hash ^= data[i];
            hash *= 1099511628211ULL;
        }
    }

    /** Return a hash of the font tables that is the same for an SFNT font and its WOFF or WOFF2 version */
    uint64_t HashFontIdentity(hb_face_t *face)
    {
        unsigned int count = hb_face_get_table_tags(face, 0, NULL, NULL);
        std::vector<hb_tag_t> tags(count);
        hb_face_get_table_tags(face, 0, &count, tags.data());
        tags.resize(count);
        std::ranges::sort(tags);
        uint64_t hash = 1469598103934665603ULL;
        const uint32_t unitsPerEm = hb_face_get_upem(face);
        const uint32_t glyphCount = hb_face_get_glyph_count(face);
        const unsigned char faceData[8]
            = { static_cast<unsigned char>(unitsPerEm >> 24), static_cast<unsigned char>(unitsPerEm >> 16),
                  static_cast<unsigned char>(unitsPerEm >> 8), static_cast<unsigned char>(unitsPerEm),
                  static_cast<unsigned char>(glyphCount >> 24), static_cast<unsigned char>(glyphCount >> 16),
                  static_cast<unsigned char>(glyphCount >> 8), static_cast<unsigned char>(glyphCount) };
        AddToHash(hash, faceData, sizeof(faceData));
        for (hb_tag_t tag : tags) {
            // WOFF2 reconstructs TrueType outlines and loca offsets into a semantically equivalent, but byte-different,
            // canonical SFNT. These container-dependent tables cannot participate in the cross-format identity.
            if ((tag == HB_TAG('D', 'S', 'I', 'G')) || (tag == HB_TAG('h', 'e', 'a', 'd'))
                || (tag == HB_TAG('g', 'l', 'y', 'f')) || (tag == HB_TAG('l', 'o', 'c', 'a'))) {
                continue;
            }
            unsigned char tagBytes[4] = { static_cast<unsigned char>(tag >> 24), static_cast<unsigned char>(tag >> 16),
                static_cast<unsigned char>(tag >> 8), static_cast<unsigned char>(tag) };
            AddToHash(hash, tagBytes, sizeof(tagBytes));
            hb_blob_t *table = hb_face_reference_table(face, tag);
            unsigned int length = 0;
            const char *bytes = hb_blob_get_data(table, &length);
            AddToHash(hash, reinterpret_cast<const unsigned char *>(bytes), length);
            hb_blob_destroy(table);
        }
        return hash ? hash : 1;
    }

    /** Return the identity of a face synthesized from a base face */
    uint64_t HashSyntheticIdentity(uint64_t baseIdentity, bool bold, bool italic)
    {
        unsigned char data[9] = { static_cast<unsigned char>(baseIdentity >> 56),
            static_cast<unsigned char>(baseIdentity >> 48), static_cast<unsigned char>(baseIdentity >> 40),
            static_cast<unsigned char>(baseIdentity >> 32), static_cast<unsigned char>(baseIdentity >> 24),
            static_cast<unsigned char>(baseIdentity >> 16), static_cast<unsigned char>(baseIdentity >> 8),
            static_cast<unsigned char>(baseIdentity), static_cast<unsigned char>((bold ? 2 : 0) | (italic ? 1 : 0)) };
        return HashBytes(data, sizeof(data));
    }

    //----------------------------------------------------------------------------
    // Font decoding
    //----------------------------------------------------------------------------

    /** Check that the data is a single, well-formed SFNT font without unsupported tables */
    bool IsSfnt(const unsigned char *data, size_t length)
    {
        if (length < 12) return false;
        const bool signature = !std::memcmp(data, "OTTO", 4) || !std::memcmp(data, "true", 4)
            || !std::memcmp(data, "typ1", 4)
            || ((data[0] == 0x00) && (data[1] == 0x01) && (data[2] == 0x00) && (data[3] == 0x00));
        if (!signature || !std::memcmp(data, "ttcf", 4)) {
            return false;
        }
        const unsigned int tables = ReadU16(data + 4);
        if (!tables || (tables > MAX_TABLES) || (12U + tables * 16U > length)) {
            return false;
        }

        static constexpr std::array<std::array<char, 4>, 18> unsupportedTables
            = { { { 'f', 'v', 'a', 'r' }, { 'C', 'F', 'F', '2' }, { 'C', 'O', 'L', 'R' }, { 'C', 'P', 'A', 'L' },
                { 'C', 'B', 'D', 'T' }, { 'C', 'B', 'L', 'C' }, { 's', 'b', 'i', 'x' }, { 'S', 'V', 'G', ' ' },
                { 'm', 'o', 'r', 'x' }, { 'm', 'o', 'r', 't' }, { 'k', 'e', 'r', 'x' }, { 'a', 'n', 'k', 'r' },
                { 't', 'r', 'a', 'k' }, { 'f', 'e', 'a', 't' }, { 'l', 'c', 'a', 'r' }, { 'o', 'p', 'b', 'd' },
                { 'b', 's', 'l', 'n' }, { 'j', 'u', 's', 't' } } };
        std::vector<std::pair<uint32_t, uint32_t>> ranges;
        ranges.reserve(tables);
        std::vector<uint32_t> tags;
        tags.reserve(tables);
        for (unsigned int i = 0; i < tables; ++i) {
            const unsigned char *entry = data + 12U + i * 16U;
            for (const std::array<char, 4> &tag : unsupportedTables) {
                if (!std::memcmp(entry, tag.data(), tag.size())) return false;
            }
            const uint32_t numericTag = ReadU32(entry);
            if (std::ranges::find(tags, numericTag) != tags.end()) return false;
            tags.push_back(numericTag);
            const uint32_t offset = ReadU32(entry + 8);
            const uint32_t tableLength = ReadU32(entry + 12);
            if ((offset > length) || (tableLength > length - offset)) {
                return false;
            }
            if (tableLength) ranges.emplace_back(offset, tableLength);
        }
        std::ranges::sort(ranges);
        for (int i = 1; i < static_cast<int>(ranges.size()); ++i) {
            const uint64_t previousEnd = static_cast<uint64_t>(ranges[i - 1].first) + ranges[i - 1].second;
            if (previousEnd > ranges[i].first) return false;
        }
        return true;
    }

    /** Decode WOFF data into an SFNT font */
    std::optional<std::vector<unsigned char>> DecodeWoff1(const unsigned char *data, size_t length)
    {
        if ((length < 44) || std::memcmp(data, "wOFF", 4)) {
            return std::nullopt;
        }
        const uint32_t declaredLength = ReadU32(data + 8);
        const uint16_t tableCount = ReadU16(data + 12);
        const uint16_t reserved = ReadU16(data + 14);
        const uint32_t outputSize = ReadU32(data + 16);
        if ((declaredLength != length) || reserved || !tableCount || (tableCount > MAX_TABLES)
            || (outputSize < 12U + tableCount * 16U) || (outputSize > MAX_DECODED_FONT)
            || (44U + tableCount * 20U > length)) {
            return std::nullopt;
        }

        std::vector<unsigned char> output(outputSize, 0);
        std::memcpy(output.data(), data + 4, 4);
        WriteU16(output.data() + 4, tableCount);
        uint16_t power = 1;
        uint16_t selector = 0;
        while ((power << 1) <= tableCount) {
            power <<= 1;
            ++selector;
        }
        WriteU16(output.data() + 6, power * 16);
        WriteU16(output.data() + 8, selector);
        WriteU16(output.data() + 10, tableCount * 16 - power * 16);

        size_t outputOffset = 12U + tableCount * 16U;
        for (uint16_t i = 0; i < tableCount; ++i) {
            const unsigned char *entry = data + 44U + i * 20U;
            const uint32_t inputOffset = ReadU32(entry + 4);
            const uint32_t compressedLength = ReadU32(entry + 8);
            const uint32_t originalLength = ReadU32(entry + 12);
            if (!originalLength || (compressedLength > originalLength) || (inputOffset > length)
                || (compressedLength > length - inputOffset) || (outputOffset > output.size())
                || (originalLength > output.size() - outputOffset)) {
                return std::nullopt;
            }

            unsigned char *record = output.data() + 12U + i * 16U;
            std::memcpy(record, entry, 4);
            std::memcpy(record + 4, entry + 16, 4);
            WriteU32(record + 8, static_cast<uint32_t>(outputOffset));
            WriteU32(record + 12, originalLength);
            if (compressedLength == originalLength) {
                std::memcpy(output.data() + outputOffset, data + inputOffset, originalLength);
            }
            else if (!InflateZlib(data + inputOffset, compressedLength, output.data() + outputOffset, originalLength)) {
                return std::nullopt;
            }
            outputOffset = Align4(outputOffset + originalLength);
            if (outputOffset > output.size()) return std::nullopt;
        }
        if (outputOffset != output.size()) return std::nullopt;
        return output;
    }

    /** Decode WOFF2 data into an SFNT font */
    std::optional<std::vector<unsigned char>> DecodeWoff2(const unsigned char *data, size_t length)
    {
        if ((length < 48) || std::memcmp(data, "wOF2", 4)) {
            return std::nullopt;
        }
        const size_t outputSize = woff2::ComputeWOFF2FinalSize(data, length);
        if (!outputSize || (outputSize > MAX_DECODED_FONT)) {
            return std::nullopt;
        }
        std::vector<unsigned char> output(outputSize);
        if (!woff2::ConvertWOFF2ToTTF(output.data(), output.size(), data, length)) return std::nullopt;
        return output;
    }

    bool IsCompressedFont(const unsigned char *data, size_t length)
    {
        return (length >= 4) && (!std::memcmp(data, "wOFF", 4) || !std::memcmp(data, "wOF2", 4));
    }

    //----------------------------------------------------------------------------
    // Font tables
    //----------------------------------------------------------------------------

    /** Read the first entry of a name in the font name table */
    std::string ReadName(hb_face_t *face, hb_ot_name_id_t id)
    {
        hb_language_t language = HB_LANGUAGE_INVALID;
        unsigned int entryCount = 0;
        const hb_ot_name_entry_t *entries = hb_ot_name_list_names(face, &entryCount);
        for (unsigned int i = 0; i < entryCount; ++i) {
            if (entries[i].name_id == id) {
                language = entries[i].language;
                break;
            }
        }
        if (language == HB_LANGUAGE_INVALID) return {};
        unsigned int capacity = 0;
        const unsigned int length = hb_ot_name_get_utf8(face, id, language, &capacity, NULL);
        if (!length) return {};
        std::string value(length + 1, '\0');
        capacity = static_cast<unsigned int>(value.size());
        hb_ot_name_get_utf8(face, id, language, &capacity, value.data());
        value.resize(capacity);
        while (!value.empty() && (value.back() == '\0')) value.pop_back();
        return value;
    }

    std::string Lower(std::string value)
    {
        std::transform(value.begin(), value.end(), value.begin(),
            [](unsigned char character) { return static_cast<char>(std::tolower(character)); });
        return value;
    }

    std::optional<uint16_t> ReadTableU16(hb_face_t *face, hb_tag_t tag, unsigned int offset)
    {
        hb_blob_t *table = hb_face_reference_table(face, tag);
        unsigned int length = 0;
        const char *data = hb_blob_get_data(table, &length);
        const std::optional<uint16_t> value = (data && (offset + sizeof(uint16_t) <= length))
            ? std::optional<uint16_t>(ReadU16(reinterpret_cast<const unsigned char *>(data + offset)))
            : std::nullopt;
        hb_blob_destroy(table);
        return value;
    }

    /** Read the weight and style of a face from the OS/2 and head tables, or from the subfamily name otherwise */
    std::pair<FontStore::Weight, FontStore::Style> ReadFaceStyle(hb_face_t *face, const std::string &subfamily)
    {
        const std::optional<uint16_t> weightClass = ReadTableU16(face, HB_TAG('O', 'S', '/', '2'), 4);
        const std::optional<uint16_t> selection = ReadTableU16(face, HB_TAG('O', 'S', '/', '2'), 62);
        const std::optional<uint16_t> macStyle = ReadTableU16(face, HB_TAG('h', 'e', 'a', 'd'), 44);
        const std::string normalizedSubfamily = Lower(subfamily);

        const bool hasBoldMetadata = weightClass || selection || macStyle;
        const bool hasItalicMetadata = selection || macStyle;
        const bool bold = hasBoldMetadata ? (weightClass && (*weightClass >= 600))
                || (selection && (*selection & (1U << 5))) || (macStyle && (*macStyle & (1U << 0)))
                                          : (normalizedSubfamily.find("bold") != std::string::npos);
        const bool italic = hasItalicMetadata
            ? (selection && (*selection & ((1U << 0) | (1U << 9)))) || (macStyle && (*macStyle & (1U << 1)))
            : (normalizedSubfamily.find("italic") != std::string::npos)
                || (normalizedSubfamily.find("oblique") != std::string::npos);
        return { bold ? FontStore::Weight::Bold : FontStore::Weight::Normal,
            italic ? FontStore::Style::Italic : FontStore::Style::Normal };
    }

    //----------------------------------------------------------------------------
    // Glyph outlines
    //----------------------------------------------------------------------------

    /** Format a number with at most three decimals and without trailing zeros */
    std::string Number(float value)
    {
        if (std::abs(value - std::round(value)) < 0.0001F) return std::to_string(static_cast<int>(std::round(value)));
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(3) << value;
        std::string result = stream.str();
        while (!result.empty() && (result.back() == '0')) result.pop_back();
        if (!result.empty() && (result.back() == '.')) {
            result.pop_back();
        }
        return result;
    }

    /** Build an SVG path from the HarfBuzz draw callbacks */
    struct PathBuilder {
        void Add(char operation, std::initializer_list<float> values)
        {
            if (!m_path.empty()) m_path.push_back(' ');
            m_path.push_back(operation);
            for (float value : values) {
                m_path.push_back(' ');
                m_path += Number(value);
            }
        }

        std::string m_path;
    };

    void MoveTo(hb_draw_funcs_t *, void *drawData, hb_draw_state_t *, float x, float y, void *)
    {
        static_cast<PathBuilder *>(drawData)->Add('M', { x, y });
    }

    void LineTo(hb_draw_funcs_t *, void *drawData, hb_draw_state_t *, float x, float y, void *)
    {
        static_cast<PathBuilder *>(drawData)->Add('L', { x, y });
    }

    void QuadraticTo(
        hb_draw_funcs_t *, void *drawData, hb_draw_state_t *, float controlX, float controlY, float x, float y, void *)
    {
        static_cast<PathBuilder *>(drawData)->Add('Q', { controlX, controlY, x, y });
    }

    void CubicTo(hb_draw_funcs_t *, void *drawData, hb_draw_state_t *, float control1X, float control1Y,
        float control2X, float control2Y, float x, float y, void *)
    {
        static_cast<PathBuilder *>(drawData)->Add('C', { control1X, control1Y, control2X, control2Y, x, y });
    }

    void ClosePath(hb_draw_funcs_t *, void *drawData, hb_draw_state_t *, void *)
    {
        PathBuilder *builder = static_cast<PathBuilder *>(drawData);
        if (!builder->m_path.empty()) builder->m_path += " Z";
    }

    hb_draw_funcs_t *GetDrawFunctions()
    {
        static hb_draw_funcs_t *functions = [] {
            hb_draw_funcs_t *value = hb_draw_funcs_create();
            hb_draw_funcs_set_move_to_func(value, MoveTo, NULL, NULL);
            hb_draw_funcs_set_line_to_func(value, LineTo, NULL, NULL);
            hb_draw_funcs_set_quadratic_to_func(value, QuadraticTo, NULL, NULL);
            hb_draw_funcs_set_cubic_to_func(value, CubicTo, NULL, NULL);
            hb_draw_funcs_set_close_path_func(value, ClosePath, NULL, NULL);
            hb_draw_funcs_make_immutable(value);
            return value;
        }();
        return functions;
    }

    //----------------------------------------------------------------------------
    // Keys
    //----------------------------------------------------------------------------

    struct FaceKey {
        bool operator==(const FaceKey &) const = default;

        FontStore::Kind m_kind;
        std::string m_family;
        FontStore::Weight m_weight;
        FontStore::Style m_style;
    };

    struct FaceKeyHash {
        size_t operator()(const FaceKey &key) const
        {
            size_t hash = std::hash<std::string>()(key.m_family);
            hash ^= static_cast<size_t>(key.m_kind) << 1;
            hash ^= static_cast<size_t>(key.m_weight) << 2;
            hash ^= static_cast<size_t>(key.m_style) << 3;
            return hash;
        }
    };

    struct FamilyKey {
        bool operator==(const FamilyKey &) const = default;

        FontStore::Kind m_kind;
        std::string m_family;
    };

    struct FamilyKeyHash {
        size_t operator()(const FamilyKey &key) const
        {
            return std::hash<std::string>()(key.m_family) ^ (static_cast<size_t>(key.m_kind) << 1);
        }
    };

    /** The key of a shaped run, which depends on the text and on all the faces used for shaping it */
    struct ShapeKey {
        bool operator==(const ShapeKey &) const = default;

        const void *m_face = NULL;
        const void *m_textFallback = NULL;
        const void *m_musicFace = NULL;
        const void *m_musicFallback = NULL;
        const void *m_bravura = NULL;
        std::u32string m_text;
    };

    struct ShapeKeyHash {
        size_t operator()(const ShapeKey &key) const
        {
            size_t hash = std::hash<const void *>()(key.m_face);
            const std::array<const void *, 4> fallbacks
                = { key.m_textFallback, key.m_musicFace, key.m_musicFallback, key.m_bravura };
            for (const void *fallback : fallbacks) {
                hash ^= std::hash<const void *>()(fallback) + 0x9e3779b9U + (hash << 6) + (hash >> 2);
            }
            for (char32_t character : key.m_text) {
                hash ^= static_cast<size_t>(character) + 0x9e3779b9U + (hash << 6) + (hash >> 2);
            }
            return hash;
        }
    };

    //----------------------------------------------------------------------------
    // FaceData
    //----------------------------------------------------------------------------

    /**
     * The HarfBuzz objects of a decoded SFNT face, with the glyph data extracted from it.
     * A synthesized face shares the data of its source face.
     */
    struct FaceData {
        explicit FaceData(std::vector<unsigned char> input, uint64_t inputHash)
            : m_bytes(std::move(input)), m_byteHash(inputHash)
        {
            m_blob = hb_blob_create(reinterpret_cast<const char *>(m_bytes.data()),
                static_cast<unsigned int>(m_bytes.size()), HB_MEMORY_MODE_READONLY, NULL, NULL);
            m_face = hb_face_create(m_blob, 0);
            m_font = hb_font_create(m_face);
            hb_ot_font_set_funcs(m_font);
            m_unitsPerEm = static_cast<int>(hb_face_get_upem(m_face));
            m_identity = HashFontIdentity(m_face);
        }

        FaceData(const std::shared_ptr<FaceData> &source, bool bold, bool italic)
            : m_byteHash(source->m_byteHash)
            , m_identity(HashSyntheticIdentity(source->m_identity, bold, italic))
            , m_blob(hb_blob_reference(source->m_blob))
            , m_face(hb_face_reference(source->m_face))
            , m_font(hb_font_create_sub_font(source->m_font))
            , m_unitsPerEm(source->m_unitsPerEm)
        {
            if (bold) hb_font_set_synthetic_bold(m_font, 0.03F, 0.03F, false);
            if (italic) hb_font_set_synthetic_slant(m_font, 0.2F);
        }

        ~FaceData()
        {
            hb_font_destroy(m_font);
            hb_face_destroy(m_face);
            hb_blob_destroy(m_blob);
        }

        /** Return the number of glyphs in the face */
        int GetGlyphCount() const { return static_cast<int>(hb_face_get_glyph_count(m_face)); }

        std::vector<unsigned char> m_bytes;
        uint64_t m_byteHash;
        uint64_t m_identity;
        hb_blob_t *m_blob = NULL;
        hb_face_t *m_face = NULL;
        hb_font_t *m_font = NULL;
        int m_unitsPerEm = 0;
        mutable std::mutex m_cacheMutex;
        mutable std::unordered_map<int, FontStore::GlyphMetrics> m_metrics;
        mutable std::unordered_map<int, std::string> m_outlines;
    };

    using SharedFaceMap = std::unordered_multimap<uint64_t, std::weak_ptr<FaceData>>;

    /** The faces shared by all stores, by hash of their data */
    std::mutex s_sharedFacesMutex;
    SharedFaceMap s_sharedFaces;

    struct SyntheticFaceKey {
        bool operator==(const SyntheticFaceKey &) const = default;

        uint64_t m_identity;
        bool m_bold;
        bool m_italic;
    };

    struct SyntheticFaceKeyHash {
        size_t operator()(const SyntheticFaceKey &key) const
        {
            return static_cast<size_t>(HashSyntheticIdentity(key.m_identity, key.m_bold, key.m_italic));
        }
    };

    using SharedSyntheticFaceMap = std::unordered_map<SyntheticFaceKey, std::weak_ptr<FaceData>, SyntheticFaceKeyHash>;

    /** The synthesized faces shared by all stores, also guarded by s_sharedFacesMutex */
    SharedSyntheticFaceMap s_sharedSyntheticFaces;

    std::shared_ptr<FaceData> FindOrCreateFace(const unsigned char *data, size_t length, uint64_t hash)
    {
        std::lock_guard<std::mutex> lock(s_sharedFacesMutex);
        const std::pair<SharedFaceMap::iterator, SharedFaceMap::iterator> range = s_sharedFaces.equal_range(hash);
        for (SharedFaceMap::iterator iter = range.first; iter != range.second; ++iter) {
            if (std::shared_ptr<FaceData> face = iter->second.lock()) {
                if ((face->m_bytes.size() == length) && !std::memcmp(face->m_bytes.data(), data, length)) {
                    return face;
                }
            }
        }
        std::vector<unsigned char> bytes(data, data + length);
        std::shared_ptr<FaceData> face = std::make_shared<FaceData>(std::move(bytes), hash);
        s_sharedFaces.emplace(hash, face);
        return face;
    }

    std::shared_ptr<FaceData> FindOrCreateSyntheticFace(const std::shared_ptr<FaceData> &source, bool bold, bool italic)
    {
        const SyntheticFaceKey key{ source->m_identity, bold, italic };
        std::lock_guard<std::mutex> lock(s_sharedFacesMutex);
        const SharedSyntheticFaceMap::const_iterator existing = s_sharedSyntheticFaces.find(key);
        if (existing != s_sharedSyntheticFaces.end()) {
            if (std::shared_ptr<FaceData> face = existing->second.lock()) return face;
        }
        std::shared_ptr<FaceData> face = std::make_shared<FaceData>(source, bold, italic);
        s_sharedSyntheticFaces[key] = face;
        return face;
    }

    //----------------------------------------------------------------------------
    // Decoded inputs and music metadata
    //----------------------------------------------------------------------------

    /** A WOFF or WOFF2 input with its decoded face */
    struct DecodedInput {
        std::vector<unsigned char> m_source;
        std::shared_ptr<FaceData> m_face;
    };

    using MusicAnchorMap = std::unordered_map<std::string, std::vector<FontStore::GlyphAnchor>>;

    struct ParsedMusicMetadata {
        std::string m_source;
        std::string m_family;
        std::shared_ptr<const MusicAnchorMap> m_anchors;
    };

    using SharedMusicMetadataMap = std::unordered_multimap<uint64_t, std::weak_ptr<const ParsedMusicMetadata>>;
    using SharedDecodedInputMap = std::unordered_multimap<uint64_t, std::weak_ptr<DecodedInput>>;

    /** The parsed SMuFL metadata shared by all stores, by hash of the JSON */
    std::mutex s_musicMetadataMutex;
    SharedMusicMetadataMap s_musicMetadata;
    /** The decoded inputs shared by all stores, by hash of the WOFF or WOFF2 data */
    std::mutex s_decodedInputsMutex;
    SharedDecodedInputMap s_decodedInputs;
    /** The bundled data kept alive for the lifetime of the process */
    std::vector<std::shared_ptr<DecodedInput>> s_pinnedDecodedInputs;
    std::vector<std::shared_ptr<const ParsedMusicMetadata>> s_pinnedMusicMetadata;

    /** Return the parsed SMuFL metadata, or NULL if the JSON is invalid */
    std::shared_ptr<const ParsedMusicMetadata> FindOrParseMusicMetadata(const std::string &metadata)
    {
        const uint64_t hash = HashBytes(reinterpret_cast<const unsigned char *>(metadata.data()), metadata.size());
        std::lock_guard<std::mutex> lock(s_musicMetadataMutex);
        const std::pair<SharedMusicMetadataMap::iterator, SharedMusicMetadataMap::iterator> range
            = s_musicMetadata.equal_range(hash);
        for (SharedMusicMetadataMap::iterator iter = range.first; iter != range.second; ++iter) {
            if (const std::shared_ptr<const ParsedMusicMetadata> parsed = iter->second.lock()) {
                if (parsed->m_source == metadata) return parsed;
            }
        }

        jsonxx::Object json;
        if (!json.parse(metadata)) return NULL;
        std::string family;
        if (json.has<jsonxx::String>("fontName")) family = json.get<jsonxx::String>("fontName");
        MusicAnchorMap anchors;
        if (json.has<jsonxx::Object>("glyphsWithAnchors")) {
            const jsonxx::Object &glyphs = json.get<jsonxx::Object>("glyphsWithAnchors");
            for (const std::pair<const std::string, jsonxx::Value *> &glyph : glyphs.kv_map()) {
                if (!glyph.second->is<jsonxx::Object>()) continue;
                const jsonxx::Object &glyphAnchors = glyph.second->get<jsonxx::Object>();
                for (const std::pair<const std::string, jsonxx::Value *> &anchor : glyphAnchors.kv_map()) {
                    if (!anchor.second->is<jsonxx::Array>()) continue;
                    const jsonxx::Array &coordinates = anchor.second->get<jsonxx::Array>();
                    if ((coordinates.size() != 2) || !coordinates.has<jsonxx::Number>(0)
                        || !coordinates.has<jsonxx::Number>(1)) {
                        continue;
                    }
                    anchors[glyph.first].push_back(
                        { anchor.first, static_cast<double>(coordinates.get<jsonxx::Number>(0)),
                            static_cast<double>(coordinates.get<jsonxx::Number>(1)) });
                }
            }
        }
        std::shared_ptr<const ParsedMusicMetadata> parsed = std::make_shared<ParsedMusicMetadata>(ParsedMusicMetadata{
            metadata, std::move(family), std::make_shared<const MusicAnchorMap>(std::move(anchors)) });
        s_musicMetadata.emplace(hash, parsed);
        return parsed;
    }

    /** Return the decoded WOFF or WOFF2 input, or NULL if it cannot be decoded */
    std::shared_ptr<DecodedInput> FindOrDecodeInput(
        const unsigned char *data, size_t length, uint64_t sourceHash, bool &decodedNow)
    {
        decodedNow = false;
        std::lock_guard<std::mutex> lock(s_decodedInputsMutex);
        const std::pair<SharedDecodedInputMap::iterator, SharedDecodedInputMap::iterator> range
            = s_decodedInputs.equal_range(sourceHash);
        for (SharedDecodedInputMap::iterator iter = range.first; iter != range.second; ++iter) {
            if (const std::shared_ptr<DecodedInput> decoded = iter->second.lock()) {
                if ((decoded->m_source.size() == length) && !std::memcmp(decoded->m_source.data(), data, length)) {
                    return decoded;
                }
            }
        }

        std::optional<std::vector<unsigned char>> sfnt
            = !std::memcmp(data, "wOFF", 4) ? DecodeWoff1(data, length) : DecodeWoff2(data, length);
        if (!sfnt || !IsSfnt(sfnt->data(), sfnt->size())) {
            return NULL;
        }
        const uint64_t hash = HashBytes(sfnt->data(), sfnt->size());
        std::shared_ptr<DecodedInput> decoded = std::make_shared<DecodedInput>(DecodedInput{
            std::vector<unsigned char>(data, data + length), FindOrCreateFace(sfnt->data(), sfnt->size(), hash) });
        s_decodedInputs.emplace(sourceHash, decoded);
        decodedNow = true;
        return decoded;
    }

    /** Check that two faces have the same data, or the same metrics and outlines for all glyphs */
    bool FacesEquivalent(const std::shared_ptr<FaceData> &left, const std::shared_ptr<FaceData> &right)
    {
        if (left == right) return true;
        if ((left->m_bytes.size() == right->m_bytes.size())
            && !std::memcmp(left->m_bytes.data(), right->m_bytes.data(), left->m_bytes.size())) {
            return true;
        }
        const unsigned int glyphCount = hb_face_get_glyph_count(left->m_face);
        if ((left->m_unitsPerEm != right->m_unitsPerEm) || (glyphCount != hb_face_get_glyph_count(right->m_face))) {
            return false;
        }
        for (hb_codepoint_t glyphId = 0; glyphId < glyphCount; ++glyphId) {
            if (hb_font_get_glyph_h_advance(left->m_font, glyphId)
                != hb_font_get_glyph_h_advance(right->m_font, glyphId)) {
                return false;
            }
            hb_glyph_extents_t leftExtents{};
            hb_glyph_extents_t rightExtents{};
            const bool hasLeftExtents = hb_font_get_glyph_extents(left->m_font, glyphId, &leftExtents);
            const bool hasRightExtents = hb_font_get_glyph_extents(right->m_font, glyphId, &rightExtents);
            if ((hasLeftExtents != hasRightExtents)
                || (hasLeftExtents
                    && ((leftExtents.x_bearing != rightExtents.x_bearing)
                        || (leftExtents.y_bearing != rightExtents.y_bearing)
                        || (leftExtents.width != rightExtents.width) || (leftExtents.height != rightExtents.height)))) {
                return false;
            }
            PathBuilder leftPath;
            PathBuilder rightPath;
            const bool hasLeftPath = hb_font_draw_glyph_or_fail(left->m_font, glyphId, GetDrawFunctions(), &leftPath);
            const bool hasRightPath
                = hb_font_draw_glyph_or_fail(right->m_font, glyphId, GetDrawFunctions(), &rightPath);
            if ((hasLeftPath != hasRightPath) || (leftPath.m_path != rightPath.m_path)) {
                return false;
            }
        }
        return true;
    }

} // namespace

//----------------------------------------------------------------------------
// FontStore::ShapedRun
//----------------------------------------------------------------------------

int FontStore::ShapedRun::GetClusterGapCount() const
{
    int count = 0;
    for (int i = 1; i < static_cast<int>(m_glyphs.size()); ++i) {
        if (m_glyphs.at(i).m_cluster != m_glyphs.at(i - 1).m_cluster) ++count;
    }
    return count;
}

//----------------------------------------------------------------------------
// FontStore::Impl
//----------------------------------------------------------------------------

class FontStore::Impl {
public:
    using FaceMap = std::unordered_map<FaceKey, std::shared_ptr<FaceData>, FaceKeyHash>;
    using MusicAnchorsMap = std::unordered_map<FaceKey, std::shared_ptr<const MusicAnchorMap>, FaceKeyHash>;
    using AliasMap = std::unordered_map<FamilyKey, std::string, FamilyKeyHash>;
    using DecodedInputMap = std::unordered_multimap<uint64_t, std::shared_ptr<DecodedInput>>;
    using ShapeCache = std::unordered_map<ShapeKey, ShapedRun, ShapeKeyHash>;

    /** Register a font and return its canonical family name, or an empty string on failure */
    std::string Register(
        Kind kind, const unsigned char *data, int length, const std::string &metadata, const std::string &alias)
    {
        if (!data || (length <= 0) || (length > MAX_FONT_INPUT)) {
            return {};
        }
        if (!alias.empty()
            && ((alias.find('=') != std::string::npos)
                || std::ranges::all_of(alias, [](unsigned char character) { return std::isspace(character); }))) {
            LogError("Font alias '%s' is invalid.", alias.c_str());
            return {};
        }
        const size_t size = static_cast<size_t>(length);
        std::shared_ptr<FaceData> face;
        if (IsCompressedFont(data, size)) {
            const uint64_t sourceHash = HashBytes(data, size);
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                const std::pair<DecodedInputMap::iterator, DecodedInputMap::iterator> range
                    = m_decodedInputs.equal_range(sourceHash);
                for (DecodedInputMap::iterator iter = range.first; iter != range.second; ++iter) {
                    if ((iter->second->m_source.size() == size)
                        && !std::memcmp(iter->second->m_source.data(), data, size)) {
                        face = iter->second->m_face;
                        break;
                    }
                }
            }
            if (!face) {
                bool decodedNow = false;
                const std::shared_ptr<DecodedInput> decoded = FindOrDecodeInput(data, size, sourceHash, decodedNow);
                if (!decoded) return {};
                face = decoded->m_face;
                std::lock_guard<std::mutex> lock(m_mutex);
                m_decodedInputs.emplace(sourceHash, decoded);
                if (decodedNow) ++m_counters.m_decodedFonts;
            }
        }
        else {
            if (!IsSfnt(data, size)) return {};
            const uint64_t hash = HashBytes(data, size);
            face = FindOrCreateFace(data, size, hash);
        }
        if ((hb_face_count(face->m_blob) != 1) || !face->GetGlyphCount()) {
            return {};
        }

        std::string family = ReadName(face->m_face, HB_OT_NAME_ID_TYPOGRAPHIC_FAMILY);
        if (family.empty()) family = ReadName(face->m_face, HB_OT_NAME_ID_FONT_FAMILY);
        std::shared_ptr<const ParsedMusicMetadata> musicMetadata;
        if (kind == Kind::Music) {
            musicMetadata = FindOrParseMusicMetadata(metadata);
            if (!musicMetadata) return {};
            if (!musicMetadata->m_family.empty()) family = musicMetadata->m_family;
        }
        if (family.empty()) return {};

        std::string subfamily = ReadName(face->m_face, HB_OT_NAME_ID_TYPOGRAPHIC_SUBFAMILY);
        if (subfamily.empty()) subfamily = ReadName(face->m_face, HB_OT_NAME_ID_FONT_SUBFAMILY);
        const std::pair<Weight, Style> faceStyle = ReadFaceStyle(face->m_face, subfamily);
        const FaceKey key{ kind, family, faceStyle.first, faceStyle.second };

        std::lock_guard<std::mutex> lock(m_mutex);
        const FamilyKey familyKey{ kind, family };
        const AliasMap::const_iterator canonicalAlias = m_aliases.find(familyKey);
        if ((canonicalAlias != m_aliases.end()) && (canonicalAlias->second != family)) {
            LogError("Font family '%s' is already registered as an alias for '%s'.", family.c_str(),
                canonicalAlias->second.c_str());
            return {};
        }
        FamilyKey aliasKey{ kind, alias };
        bool addAlias = false;
        if (!alias.empty() && (alias != family)) {
            if (m_families.contains(aliasKey)) {
                LogError("Font alias '%s' conflicts with an existing canonical family.", alias.c_str());
                return {};
            }
            const AliasMap::const_iterator existingAlias = m_aliases.find(aliasKey);
            if ((existingAlias != m_aliases.end()) && (existingAlias->second != family)) {
                LogError(
                    "Font alias '%s' is already registered for '%s'.", alias.c_str(), existingAlias->second.c_str());
                return {};
            }
            addAlias = (existingAlias == m_aliases.end());
        }
        const FaceMap::const_iterator existing = m_faces.find(key);
        if (existing != m_faces.end()) {
            if ((existing->second->m_identity != face->m_identity) || !FacesEquivalent(existing->second, face)) {
                return {};
            }
            if (addAlias) {
                m_aliases.emplace(std::move(aliasKey), family);
                ++m_generation;
            }
            return family;
        }
        m_faces.emplace(key, std::move(face));
        m_families.emplace(familyKey);
        if (kind == Kind::Music) {
            m_musicAnchors[key] = musicMetadata->m_anchors;
            m_parsedMusicMetadata.push_back(std::move(musicMetadata));
        }
        if (addAlias) m_aliases.emplace(std::move(aliasKey), family);
        m_shapeCache.clear();
        ++m_generation;
        return family;
    }

    /** Find a registered face, or synthesize a bold or italic text face from a registered one */
    std::shared_ptr<FaceData> Find(Kind kind, const std::string &family, Weight weight, Style style) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        const std::string &resolvedFamily = this->ResolveFamilyLocked(kind, family);
        const FaceKey requested{ kind, resolvedFamily, weight, style };
        const FaceMap::const_iterator exact = m_faces.find(requested);
        if (exact != m_faces.end()) return exact->second;
        if (kind != Kind::Text) return NULL;
        const FaceMap::const_iterator synthetic = m_syntheticFaces.find(requested);
        if (synthetic != m_syntheticFaces.end()) return synthetic->second;

        struct Candidate {
            Weight m_weight;
            Style m_style;
            bool m_syntheticBold;
            bool m_syntheticItalic;
        };
        std::array<Candidate, 3> candidates{};
        int candidateCount = 0;
        if (style == Style::Italic) candidates[candidateCount++] = { weight, Style::Normal, false, true };
        if (weight == Weight::Bold) candidates[candidateCount++] = { Weight::Normal, style, true, false };
        if ((weight == Weight::Bold) && (style == Style::Italic)) {
            candidates[candidateCount++] = { Weight::Normal, Style::Normal, true, true };
        }
        for (int i = 0; i < candidateCount; ++i) {
            const Candidate &candidate = candidates[i];
            const FaceMap::const_iterator source
                = m_faces.find({ kind, resolvedFamily, candidate.m_weight, candidate.m_style });
            if (source == m_faces.end()) continue;
            std::shared_ptr<FaceData> syntheticFace
                = FindOrCreateSyntheticFace(source->second, candidate.m_syntheticBold, candidate.m_syntheticItalic);
            m_syntheticFaces.emplace(requested, syntheticFace);
            return syntheticFace;
        }
        return NULL;
    }

    /** Find a registered or synthesized face by identity */
    std::shared_ptr<FaceData> Find(FaceIdentity identity) const
    {
        if (!identity) return NULL;
        std::lock_guard<std::mutex> lock(m_mutex);
        for (const FaceMap::value_type &entry : m_faces) {
            if (entry.second->m_identity == identity.m_value) return entry.second;
        }
        for (const FaceMap::value_type &entry : m_syntheticFaces) {
            if (entry.second->m_identity == identity.m_value) return entry.second;
        }
        return NULL;
    }

    /** Resolve an alias to its canonical family; m_mutex must be locked */
    const std::string &ResolveFamilyLocked(Kind kind, const std::string &family) const
    {
        const AliasMap::const_iterator alias = m_aliases.find({ kind, family });
        return (alias == m_aliases.end()) ? family : alias->second;
    }

    /** Extract the metrics of a valid glyph, or return them from the cache */
    std::optional<GlyphMetrics> ExtractGlyphMetrics(const std::shared_ptr<FaceData> &face, int glyphId) const
    {
        std::lock_guard<std::mutex> lock(face->m_cacheMutex);
        const std::unordered_map<int, GlyphMetrics>::const_iterator existing = face->m_metrics.find(glyphId);
        if (existing != face->m_metrics.end()) return existing->second;

        const hb_codepoint_t codepoint = static_cast<hb_codepoint_t>(glyphId);
        hb_glyph_extents_t extents{};
        if (!hb_font_get_glyph_extents(face->m_font, codepoint, &extents)) return std::nullopt;
        const GlyphMetrics metrics{ { face->m_identity }, glyphId, face->m_unitsPerEm,
            hb_font_get_glyph_h_advance(face->m_font, codepoint), extents.x_bearing, extents.y_bearing, extents.width,
            extents.height };
        face->m_metrics.emplace(glyphId, metrics);
        {
            std::lock_guard<std::mutex> counterLock(m_mutex);
            ++m_counters.m_extractedMetrics;
        }
        return metrics;
    }

    /** Extract the outline of a valid glyph as an SVG path, or return it from the cache */
    std::optional<std::string> ExtractGlyphOutline(const std::shared_ptr<FaceData> &face, int glyphId) const
    {
        std::lock_guard<std::mutex> lock(face->m_cacheMutex);
        const std::unordered_map<int, std::string>::const_iterator existing = face->m_outlines.find(glyphId);
        if (existing != face->m_outlines.end()) return existing->second;

        PathBuilder builder;
        if (!hb_font_draw_glyph_or_fail(
                face->m_font, static_cast<hb_codepoint_t>(glyphId), GetDrawFunctions(), &builder)) {
            return std::nullopt;
        }
        face->m_outlines.emplace(glyphId, builder.m_path);
        {
            std::lock_guard<std::mutex> counterLock(m_mutex);
            ++m_counters.m_extractedOutlines;
        }
        return builder.m_path;
    }

    void PinBundledData()
    {
        std::scoped_lock lock(m_mutex, s_decodedInputsMutex, s_musicMetadataMutex);
        if (!s_pinnedDecodedInputs.empty()) return;
        s_pinnedDecodedInputs.reserve(m_decodedInputs.size());
        for (const DecodedInputMap::value_type &entry : m_decodedInputs) s_pinnedDecodedInputs.push_back(entry.second);
        s_pinnedMusicMetadata = m_parsedMusicMetadata;
    }

    mutable std::mutex m_mutex;
    FaceMap m_faces;
    mutable FaceMap m_syntheticFaces;
    MusicAnchorsMap m_musicAnchors;
    std::unordered_set<FamilyKey, FamilyKeyHash> m_families;
    AliasMap m_aliases;
    DecodedInputMap m_decodedInputs;
    mutable ShapeCache m_shapeCache;
    std::vector<std::shared_ptr<const ParsedMusicMetadata>> m_parsedMusicMetadata;
    uint64_t m_generation = 0;
    mutable Counters m_counters;
    mutable bool m_warnedMissingText = false;
    mutable bool m_warnedRtl = false;
};

//----------------------------------------------------------------------------
// FontStore
//----------------------------------------------------------------------------

FontStore::FontStore() : m_impl(std::make_unique<Impl>()) {}

FontStore::~FontStore() = default;

FontStore::FontStore(FontStore &&) noexcept = default;

FontStore &FontStore::operator=(FontStore &&) noexcept = default;

std::string FontStore::RegisterTextFont(const unsigned char *data, int length, const std::string &alias)
{
    return m_impl->Register(Kind::Text, data, length, "", alias);
}

std::string FontStore::RegisterMusicFont(
    const unsigned char *data, int length, const std::string &smuflMetadataJson, const std::string &alias)
{
    return m_impl->Register(Kind::Music, data, length, smuflMetadataJson, alias);
}

bool FontStore::HasFace(Kind kind, const std::string &family, Weight weight, Style style) const
{
    return static_cast<bool>(m_impl->Find(kind, family, weight, style));
}

std::optional<FontStore::GlyphMetrics> FontStore::GetGlyphMetrics(
    Kind kind, const std::string &family, char32_t codepoint, Weight weight, Style style) const
{
    const std::shared_ptr<FaceData> face = m_impl->Find(kind, family, weight, style);
    if (!face) return std::nullopt;
    hb_codepoint_t glyphId = 0;
    if (!hb_font_get_nominal_glyph(face->m_font, codepoint, &glyphId)) return std::nullopt;
    return m_impl->ExtractGlyphMetrics(face, static_cast<int>(glyphId));
}

std::optional<FontStore::GlyphMetrics> FontStore::GetGlyphMetrics(FaceIdentity identity, int glyphId) const
{
    const std::shared_ptr<FaceData> face = m_impl->Find(identity);
    if (!face || (glyphId < 0) || (glyphId >= face->GetGlyphCount())) {
        return std::nullopt;
    }
    return m_impl->ExtractGlyphMetrics(face, glyphId);
}

std::optional<std::string> FontStore::GetGlyphOutline(
    Kind kind, const std::string &family, int glyphId, Weight weight, Style style) const
{
    const std::shared_ptr<FaceData> face = m_impl->Find(kind, family, weight, style);
    if (!face || (glyphId < 0) || (glyphId >= face->GetGlyphCount())) {
        return std::nullopt;
    }
    return m_impl->ExtractGlyphOutline(face, glyphId);
}

std::optional<std::string> FontStore::GetGlyphOutline(FaceIdentity identity, int glyphId) const
{
    const std::shared_ptr<FaceData> face = m_impl->Find(identity);
    if (!face || (glyphId < 0) || (glyphId >= face->GetGlyphCount())) {
        return std::nullopt;
    }
    return m_impl->ExtractGlyphOutline(face, glyphId);
}

std::vector<FontStore::GlyphAnchor> FontStore::GetMusicGlyphAnchors(
    const std::string &family, const std::string &glyphName) const
{
    std::lock_guard<std::mutex> lock(m_impl->m_mutex);
    const Impl::MusicAnchorsMap::const_iterator face = m_impl->m_musicAnchors.find(
        { Kind::Music, m_impl->ResolveFamilyLocked(Kind::Music, family), Weight::Normal, Style::Normal });
    if (face == m_impl->m_musicAnchors.end()) return {};
    const MusicAnchorMap::const_iterator glyph = face->second->find(glyphName);
    return (glyph == face->second->end()) ? std::vector<GlyphAnchor>() : glyph->second;
}

std::vector<FontStore::FontFile> FontStore::GetFontFiles(Kind kind, const std::string &family) const
{
    std::lock_guard<std::mutex> lock(m_impl->m_mutex);
    const std::string &resolvedFamily = m_impl->ResolveFamilyLocked(kind, family);
    std::vector<FontFile> files;
    for (const Impl::FaceMap::value_type &entry : m_impl->m_faces) {
        const FaceKey &key = entry.first;
        const std::shared_ptr<FaceData> &face = entry.second;
        if ((key.m_kind != kind) || (key.m_family != resolvedFamily)) {
            continue;
        }
        FontFile file{ key.m_weight, key.m_style };
        const Impl::DecodedInputMap::const_iterator decoded = std::ranges::find_if(m_impl->m_decodedInputs,
            [&face](const Impl::DecodedInputMap::value_type &input) { return input.second->m_face == face; });
        if (decoded != m_impl->m_decodedInputs.end()) {
            const std::vector<unsigned char> &source = decoded->second->m_source;
            const bool woff2 = !std::memcmp(source.data(), "wOF2", 4);
            file.m_format = woff2 ? "woff2" : "woff";
            file.m_mimeType = woff2 ? "font/woff2" : "font/woff";
            file.m_data = source;
        }
        else {
            const bool openType = !std::memcmp(face->m_bytes.data(), "OTTO", 4);
            file.m_format = openType ? "opentype" : "truetype";
            file.m_mimeType = openType ? "font/otf" : "font/ttf";
            file.m_data = face->m_bytes;
        }
        files.push_back(std::move(file));
    }
    // Keep the output stable regardless of the hash map order
    std::ranges::sort(files, [](const FontFile &left, const FontFile &right) {
        return std::tie(left.m_weight, left.m_style) < std::tie(right.m_weight, right.m_style);
    });
    return files;
}

std::optional<FontStore::ShapedRun> FontStore::ShapeText(const std::string &family, const std::u32string &text,
    Weight weight, Style style, const std::string &musicFamily, const std::string &musicFallbackFamily) const
{
    const std::shared_ptr<FaceData> face = m_impl->Find(Kind::Text, family, weight, style);
    if (!face) return std::nullopt;
    const std::shared_ptr<FaceData> textFallback
        = (family == "Tinos") ? face : m_impl->Find(Kind::Text, "Tinos", weight, style);
    std::shared_ptr<FaceData> musicFace;
    if (!musicFamily.empty()) musicFace = m_impl->Find(Kind::Music, musicFamily, Weight::Normal, Style::Normal);
    std::shared_ptr<FaceData> musicFallback;
    if (!musicFallbackFamily.empty()) {
        musicFallback = m_impl->Find(Kind::Music, musicFallbackFamily, Weight::Normal, Style::Normal);
    }
    std::shared_ptr<FaceData> bravura;
    if ((musicFamily != "Bravura") && (musicFallbackFamily != "Bravura")) {
        bravura = m_impl->Find(Kind::Music, "Bravura", Weight::Normal, Style::Normal);
    }
    std::lock_guard<std::mutex> cacheLock(m_impl->m_mutex);
    const ShapeKey key = { face.get(), textFallback.get(), musicFace.get(), musicFallback.get(), bravura.get(), text };
    const Impl::ShapeCache::const_iterator cached = m_impl->m_shapeCache.find(key);
    if (cached != m_impl->m_shapeCache.end()) return cached->second;

    auto shape = [](const std::shared_ptr<FaceData> &shapedFace, const std::u32string &value) {
        std::vector<GlyphPlacement> glyphs;
        hb_buffer_t *buffer = hb_buffer_create();
        hb_buffer_set_direction(buffer, HB_DIRECTION_LTR);
        hb_buffer_add_utf32(buffer, reinterpret_cast<const uint32_t *>(value.data()), static_cast<int>(value.size()), 0,
            static_cast<int>(value.size()));
        hb_buffer_guess_segment_properties(buffer);
        hb_buffer_set_direction(buffer, HB_DIRECTION_LTR);
        hb_shape(shapedFace->m_font, buffer, NULL, 0);
        unsigned int length = 0;
        const hb_glyph_info_t *infos = hb_buffer_get_glyph_infos(buffer, &length);
        const hb_glyph_position_t *positions = hb_buffer_get_glyph_positions(buffer, &length);
        glyphs.reserve(length);
        for (unsigned int i = 0; i < length; ++i) {
            glyphs.push_back({ { shapedFace->m_identity }, shapedFace->m_unitsPerEm,
                static_cast<int>(infos[i].codepoint), static_cast<int>(infos[i].cluster), positions[i].x_advance,
                positions[i].y_advance, positions[i].x_offset, positions[i].y_offset });
        }
        hb_buffer_destroy(buffer);
        return glyphs;
    };

    for (char32_t character : text) {
        if (((character >= 0x0590) && (character <= 0x08FF)) || ((character >= 0xFB1D) && (character <= 0xFEFC))) {
            if (!m_impl->m_warnedRtl) {
                LogWarning("RTL text is being processed with LTR-only semantics.");
                m_impl->m_warnedRtl = true;
            }
            break;
        }
    }

    ShapedRun run{ { face->m_identity }, face->m_unitsPerEm, shape(face, text) };
    std::vector<GlyphPlacement> &glyphs = run.m_glyphs;
    const int textLength = static_cast<int>(text.size());
    // Reshape with the fallback face the clusters containing a .notdef glyph
    auto replaceMissingClusters = [&](const std::shared_ptr<FaceData> &fallback) {
        if (!fallback || (fallback == face)) {
            return;
        }
        for (int begin = 0; begin < static_cast<int>(glyphs.size());) {
            int end = begin + 1;
            while (
                (end < static_cast<int>(glyphs.size())) && (glyphs.at(end).m_cluster == glyphs.at(begin).m_cluster)) {
                ++end;
            }
            const bool missing = std::any_of(glyphs.begin() + begin, glyphs.begin() + end,
                [](const GlyphPlacement &glyph) { return glyph.m_glyphId == 0; });
            if (missing) {
                const int textBegin = std::min(glyphs.at(begin).m_cluster, textLength);
                const int textEnd = (end < static_cast<int>(glyphs.size()))
                    ? std::min(glyphs.at(end).m_cluster, textLength)
                    : textLength;
                std::vector<GlyphPlacement> replacements = shape(fallback, text.substr(textBegin, textEnd - textBegin));
                for (GlyphPlacement &replacement : replacements) replacement.m_cluster += textBegin;
                glyphs.erase(glyphs.begin() + begin, glyphs.begin() + end);
                glyphs.insert(glyphs.begin() + begin, replacements.begin(), replacements.end());
                end = begin + static_cast<int>(replacements.size());
            }
            begin = end;
        }
    };
    std::vector<std::shared_ptr<FaceData>> fallbacks;
    for (const std::shared_ptr<FaceData> &fallback : { textFallback, musicFace, musicFallback, bravura }) {
        if (fallback && (std::find(fallbacks.begin(), fallbacks.end(), fallback) == fallbacks.end())) {
            fallbacks.push_back(fallback);
        }
    }
    for (const std::shared_ptr<FaceData> &fallback : fallbacks) replaceMissingClusters(fallback);
    if (!m_impl->m_warnedMissingText
        && std::ranges::any_of(glyphs, [](const GlyphPlacement &glyph) { return glyph.m_glyphId == 0; })) {
        LogWarning("A text cluster is missing from the requested text and SMuFL fonts; using .notdef.");
        m_impl->m_warnedMissingText = true;
    }
    m_impl->m_shapeCache.emplace(key, run);
    ++m_impl->m_counters.m_shapedRuns;
    return run;
}

uint64_t FontStore::GetGeneration() const
{
    std::lock_guard<std::mutex> lock(m_impl->m_mutex);
    return m_impl->m_generation;
}

void FontStore::PinBundledData()
{
    m_impl->PinBundledData();
}

FontStore::Counters FontStore::GetCounters() const
{
    std::lock_guard<std::mutex> lock(m_impl->m_mutex);
    return m_impl->m_counters;
}

//----------------------------------------------------------------------------
// Static methods
//----------------------------------------------------------------------------

std::vector<unsigned char> FontStore::ReadFile(const std::string &filename)
{
    std::ifstream fin(filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
    if (!fin.is_open()) return {};
    const std::streamsize fileSize = static_cast<std::streamsize>(fin.tellg());
    if ((fileSize <= 0) || (fileSize > MAX_FONT_INPUT)) {
        return {};
    }
    fin.seekg(0, std::ios::beg);
    std::vector<unsigned char> bytes(static_cast<size_t>(fileSize));
    if (!fin.read(reinterpret_cast<char *>(bytes.data()), fileSize)) return {};
    return bytes;
}

} // namespace vrv
