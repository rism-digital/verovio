/////////////////////////////////////////////////////////////////////////////
// Name:        fontstore.cpp
// Purpose:     Runtime OpenType font registration and lazy glyph access
/////////////////////////////////////////////////////////////////////////////

#include "fontstore.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <mutex>
#include <optional>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

#include "filereader.h"
#include "vrv.h"

#include "hb-ot.h"
#include "hb.h"
#include "jsonxx.h"
#include "woff2/decode.h"

namespace vrv {

namespace {

    constexpr size_t MAX_FONT_INPUT = 32U * 1024U * 1024U;
    constexpr size_t MAX_DECODED_FONT = 64U * 1024U * 1024U;
    constexpr unsigned int MAX_TABLES = 256;

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

    size_t Align4(size_t value)
    {
        return (value + 3U) & ~size_t(3U);
    }

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

    void AddToHash(uint64_t &hash, const unsigned char *data, size_t length)
    {
        for (size_t i = 0; i < length; ++i) {
            hash ^= data[i];
            hash *= 1099511628211ULL;
        }
    }

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
            // WOFF2 reconstructs TrueType outlines and loca offsets into a
            // semantically equivalent, but byte-different, canonical SFNT. These
            // container-dependent tables cannot participate in cross-format ID.
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

    bool IsSfnt(const unsigned char *data, size_t length)
    {
        if (length < 12) return false;
        const bool signature = !std::memcmp(data, "OTTO", 4) || !std::memcmp(data, "true", 4)
            || !std::memcmp(data, "typ1", 4)
            || ((data[0] == 0x00) && (data[1] == 0x01) && (data[2] == 0x00) && (data[3] == 0x00));
        if (!signature || !std::memcmp(data, "ttcf", 4)) return false;
        const unsigned int tables = ReadU16(data + 4);
        if (!tables || (tables > MAX_TABLES) || (12U + tables * 16U > length)) return false;

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
            for (const auto &tag : unsupportedTables) {
                if (!std::memcmp(entry, tag.data(), tag.size())) return false;
            }
            const uint32_t numericTag = ReadU32(entry);
            if (std::ranges::find(tags, numericTag) != tags.end()) return false;
            tags.push_back(numericTag);
            const uint32_t offset = ReadU32(entry + 8);
            const uint32_t tableLength = ReadU32(entry + 12);
            if ((offset > length) || (tableLength > length - offset)) return false;
            if (tableLength) ranges.emplace_back(offset, tableLength);
        }
        std::ranges::sort(ranges);
        for (size_t i = 1; i < ranges.size(); ++i) {
            const uint64_t previousEnd = static_cast<uint64_t>(ranges[i - 1].first) + ranges[i - 1].second;
            if (previousEnd > ranges[i].first) return false;
        }
        return true;
    }

    std::optional<std::vector<unsigned char>> DecodeWoff1(const unsigned char *data, size_t length)
    {
        if ((length < 44) || std::memcmp(data, "wOFF", 4)) return std::nullopt;
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

    std::optional<std::vector<unsigned char>> DecodeWoff2(const unsigned char *data, size_t length)
    {
        if ((length < 48) || std::memcmp(data, "wOF2", 4)) return std::nullopt;
        const size_t outputSize = woff2::ComputeWOFF2FinalSize(data, length);
        if (!outputSize || (outputSize > MAX_DECODED_FONT)) return std::nullopt;
        std::vector<unsigned char> output(outputSize);
        if (!woff2::ConvertWOFF2ToTTF(output.data(), output.size(), data, length)) return std::nullopt;
        return output;
    }

    bool IsCompressedFont(const unsigned char *data, size_t length)
    {
        return (length >= 4) && (!std::memcmp(data, "wOFF", 4) || !std::memcmp(data, "wOF2", 4));
    }

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

    std::string Number(float value)
    {
        if (std::abs(value - std::round(value)) < 0.0001F) return std::to_string(static_cast<int>(std::round(value)));
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(3) << value;
        std::string result = stream.str();
        while (!result.empty() && result.back() == '0') result.pop_back();
        if (!result.empty() && result.back() == '.') result.pop_back();
        return result;
    }

    struct PathBuilder {
        std::string path;

        void Add(char operation, std::initializer_list<float> values)
        {
            if (!path.empty()) path.push_back(' ');
            path.push_back(operation);
            for (float value : values) {
                path.push_back(' ');
                path += Number(value);
            }
        }
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
        if (!builder->path.empty()) builder->path += " Z";
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

    struct FaceKey {
        FontStore::Kind kind;
        std::string family;
        FontStore::Weight weight;
        FontStore::Style style;

        bool operator==(const FaceKey &) const = default;
    };

    struct FaceKeyHash {
        size_t operator()(const FaceKey &key) const
        {
            size_t hash = std::hash<std::string>()(key.family);
            hash ^= static_cast<size_t>(key.kind) << 1;
            hash ^= static_cast<size_t>(key.weight) << 2;
            hash ^= static_cast<size_t>(key.style) << 3;
            return hash;
        }
    };

    struct FamilyKey {
        FontStore::Kind kind;
        std::string family;

        bool operator==(const FamilyKey &) const = default;
    };

    struct FamilyKeyHash {
        size_t operator()(const FamilyKey &key) const
        {
            return std::hash<std::string>()(key.family) ^ (static_cast<size_t>(key.kind) << 1);
        }
    };

    struct ShapeKey {
        const void *face = NULL;
        std::u32string text;
        bool operator==(const ShapeKey &) const = default;
    };

    struct ShapeKeyHash {
        size_t operator()(const ShapeKey &key) const
        {
            size_t hash = std::hash<const void *>()(key.face);
            for (char32_t character : key.text) {
                hash ^= static_cast<size_t>(character) + 0x9e3779b9U + (hash << 6) + (hash >> 2);
            }
            return hash;
        }
    };

    struct FaceData {
        explicit FaceData(std::vector<unsigned char> input, uint64_t inputHash)
            : bytes(std::move(input)), byteHash(inputHash)
        {
            blob = hb_blob_create(reinterpret_cast<const char *>(bytes.data()), static_cast<unsigned int>(bytes.size()),
                HB_MEMORY_MODE_READONLY, NULL, NULL);
            face = hb_face_create(blob, 0);
            font = hb_font_create(face);
            hb_ot_font_set_funcs(font);
            unitsPerEm = static_cast<int>(hb_face_get_upem(face));
            identity = HashFontIdentity(face);
        }

        ~FaceData()
        {
            hb_font_destroy(font);
            hb_face_destroy(face);
            hb_blob_destroy(blob);
        }

        std::vector<unsigned char> bytes;
        uint64_t byteHash;
        uint64_t identity;
        hb_blob_t *blob = NULL;
        hb_face_t *face = NULL;
        hb_font_t *font = NULL;
        int unitsPerEm = 0;
        mutable std::mutex cacheMutex;
        mutable std::unordered_map<uint32_t, FontStore::GlyphMetrics> metrics;
        mutable std::unordered_map<uint32_t, std::string> outlines;
    };

    std::mutex g_sharedFacesMutex;
    std::unordered_multimap<uint64_t, std::weak_ptr<FaceData>> g_sharedFaces;

    std::shared_ptr<FaceData> FindOrCreateFace(const unsigned char *data, size_t length, uint64_t hash)
    {
        std::lock_guard<std::mutex> lock(g_sharedFacesMutex);
        const auto range = g_sharedFaces.equal_range(hash);
        for (auto iterator = range.first; iterator != range.second; ++iterator) {
            if (std::shared_ptr<FaceData> face = iterator->second.lock()) {
                if ((face->bytes.size() == length) && !std::memcmp(face->bytes.data(), data, length)) return face;
            }
        }
        std::vector<unsigned char> bytes(data, data + length);
        std::shared_ptr<FaceData> face = std::make_shared<FaceData>(std::move(bytes), hash);
        g_sharedFaces.emplace(hash, face);
        return face;
    }

    struct DecodedInput {
        std::vector<unsigned char> source;
        std::shared_ptr<FaceData> face;
    };

    using MusicAnchorMap = std::unordered_map<std::string, std::vector<FontStore::GlyphAnchor>>;

    struct ParsedMusicMetadata {
        std::string source;
        std::string family;
        std::shared_ptr<const MusicAnchorMap> anchors;
    };

    std::mutex g_musicMetadataMutex;
    std::unordered_multimap<uint64_t, std::weak_ptr<const ParsedMusicMetadata>> g_musicMetadata;
    std::vector<std::shared_ptr<DecodedInput>> g_pinnedDecodedInputs;
    std::vector<std::shared_ptr<const ParsedMusicMetadata>> g_pinnedMusicMetadata;

    std::shared_ptr<const ParsedMusicMetadata> FindOrParseMusicMetadata(const std::string &metadata)
    {
        const uint64_t hash = HashBytes(reinterpret_cast<const unsigned char *>(metadata.data()), metadata.size());
        std::lock_guard<std::mutex> lock(g_musicMetadataMutex);
        const auto range = g_musicMetadata.equal_range(hash);
        for (auto iterator = range.first; iterator != range.second; ++iterator) {
            if (const std::shared_ptr<const ParsedMusicMetadata> parsed = iterator->second.lock()) {
                if (parsed->source == metadata) return parsed;
            }
        }

        jsonxx::Object json;
        if (!json.parse(metadata)) return NULL;
        std::string family;
        if (json.has<jsonxx::String>("fontName")) family = json.get<jsonxx::String>("fontName");
        MusicAnchorMap anchors;
        if (json.has<jsonxx::Object>("glyphsWithAnchors")) {
            const jsonxx::Object &glyphs = json.get<jsonxx::Object>("glyphsWithAnchors");
            for (const auto &[glyphName, glyphValue] : glyphs.kv_map()) {
                if (!glyphValue->is<jsonxx::Object>()) continue;
                const jsonxx::Object &glyphAnchors = glyphValue->get<jsonxx::Object>();
                for (const auto &[anchorName, anchorValue] : glyphAnchors.kv_map()) {
                    if (!anchorValue->is<jsonxx::Array>()) continue;
                    const jsonxx::Array &coordinates = anchorValue->get<jsonxx::Array>();
                    if ((coordinates.size() != 2) || !coordinates.has<jsonxx::Number>(0)
                        || !coordinates.has<jsonxx::Number>(1)) {
                        continue;
                    }
                    anchors[glyphName].push_back({ anchorName, static_cast<double>(coordinates.get<jsonxx::Number>(0)),
                        static_cast<double>(coordinates.get<jsonxx::Number>(1)) });
                }
            }
        }
        auto parsed = std::make_shared<ParsedMusicMetadata>(ParsedMusicMetadata{
            metadata, std::move(family), std::make_shared<const MusicAnchorMap>(std::move(anchors)) });
        g_musicMetadata.emplace(hash, parsed);
        return parsed;
    }

    std::mutex g_decodedInputsMutex;
    std::unordered_multimap<uint64_t, std::weak_ptr<DecodedInput>> g_decodedInputs;

    std::shared_ptr<DecodedInput> FindOrDecodeInput(
        const unsigned char *data, size_t length, uint64_t sourceHash, bool &decodedNow)
    {
        decodedNow = false;
        std::lock_guard<std::mutex> lock(g_decodedInputsMutex);
        const auto range = g_decodedInputs.equal_range(sourceHash);
        for (auto iterator = range.first; iterator != range.second; ++iterator) {
            if (const std::shared_ptr<DecodedInput> decoded = iterator->second.lock()) {
                if ((decoded->source.size() == length) && !std::memcmp(decoded->source.data(), data, length)) {
                    return decoded;
                }
            }
        }

        std::optional<std::vector<unsigned char>> sfnt
            = !std::memcmp(data, "wOFF", 4) ? DecodeWoff1(data, length) : DecodeWoff2(data, length);
        if (!sfnt || !IsSfnt(sfnt->data(), sfnt->size())) return NULL;
        const uint64_t hash = HashBytes(sfnt->data(), sfnt->size());
        auto decoded = std::make_shared<DecodedInput>(DecodedInput{
            std::vector<unsigned char>(data, data + length), FindOrCreateFace(sfnt->data(), sfnt->size(), hash) });
        g_decodedInputs.emplace(sourceHash, decoded);
        decodedNow = true;
        return decoded;
    }

    bool FacesEquivalent(const std::shared_ptr<FaceData> &left, const std::shared_ptr<FaceData> &right)
    {
        if (left == right) return true;
        if ((left->bytes.size() == right->bytes.size())
            && !std::memcmp(left->bytes.data(), right->bytes.data(), left->bytes.size())) {
            return true;
        }
        const unsigned int glyphCount = hb_face_get_glyph_count(left->face);
        if ((left->unitsPerEm != right->unitsPerEm) || (glyphCount != hb_face_get_glyph_count(right->face)))
            return false;
        for (unsigned int glyphId = 0; glyphId < glyphCount; ++glyphId) {
            if (hb_font_get_glyph_h_advance(left->font, glyphId) != hb_font_get_glyph_h_advance(right->font, glyphId)) {
                return false;
            }
            hb_glyph_extents_t leftExtents{};
            hb_glyph_extents_t rightExtents{};
            const bool hasLeftExtents = hb_font_get_glyph_extents(left->font, glyphId, &leftExtents);
            const bool hasRightExtents = hb_font_get_glyph_extents(right->font, glyphId, &rightExtents);
            if ((hasLeftExtents != hasRightExtents)
                || (hasLeftExtents
                    && ((leftExtents.x_bearing != rightExtents.x_bearing)
                        || (leftExtents.y_bearing != rightExtents.y_bearing)
                        || (leftExtents.width != rightExtents.width) || (leftExtents.height != rightExtents.height)))) {
                return false;
            }
            PathBuilder leftPath;
            PathBuilder rightPath;
            const bool hasLeftPath = hb_font_draw_glyph_or_fail(left->font, glyphId, GetDrawFunctions(), &leftPath);
            const bool hasRightPath = hb_font_draw_glyph_or_fail(right->font, glyphId, GetDrawFunctions(), &rightPath);
            if ((hasLeftPath != hasRightPath) || (leftPath.path != rightPath.path)) return false;
        }
        return true;
    }

} // namespace

class FontStore::Impl {
public:
    std::string Register(
        Kind kind, const unsigned char *data, size_t length, const std::string &metadata, const std::string &alias)
    {
        if (!data || !length || (length > MAX_FONT_INPUT)) return {};
        if (!alias.empty()
            && ((alias.find('=') != std::string::npos)
                || std::ranges::all_of(alias, [](unsigned char character) { return std::isspace(character); }))) {
            LogError("Font alias '%s' is invalid.", alias.c_str());
            return {};
        }
        std::shared_ptr<FaceData> face;
        if (IsCompressedFont(data, length)) {
            const uint64_t sourceHash = HashBytes(data, length);
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                const auto range = m_decodedInputs.equal_range(sourceHash);
                for (auto iterator = range.first; iterator != range.second; ++iterator) {
                    if ((iterator->second->source.size() == length)
                        && !std::memcmp(iterator->second->source.data(), data, length)) {
                        face = iterator->second->face;
                        break;
                    }
                }
            }
            if (!face) {
                bool decodedNow = false;
                const std::shared_ptr<DecodedInput> decoded = FindOrDecodeInput(data, length, sourceHash, decodedNow);
                if (!decoded) return {};
                face = decoded->face;
                std::lock_guard<std::mutex> lock(m_mutex);
                m_decodedInputs.emplace(sourceHash, decoded);
                if (decodedNow) ++m_counters.decodedFonts;
            }
        }
        else {
            if (!IsSfnt(data, length)) return {};
            const uint64_t hash = HashBytes(data, length);
            face = FindOrCreateFace(data, length, hash);
        }
        if ((hb_face_count(face->blob) != 1) || !hb_face_get_glyph_count(face->face) || !face->unitsPerEm) return {};

        std::string family = ReadName(face->face, HB_OT_NAME_ID_TYPOGRAPHIC_FAMILY);
        if (family.empty()) family = ReadName(face->face, HB_OT_NAME_ID_FONT_FAMILY);
        std::shared_ptr<const ParsedMusicMetadata> musicMetadata;
        if (kind == Kind::Music) {
            musicMetadata = FindOrParseMusicMetadata(metadata);
            if (!musicMetadata) return {};
            if (!musicMetadata->family.empty()) family = musicMetadata->family;
        }
        if (family.empty()) return {};

        std::string subfamily = ReadName(face->face, HB_OT_NAME_ID_TYPOGRAPHIC_SUBFAMILY);
        if (subfamily.empty()) subfamily = ReadName(face->face, HB_OT_NAME_ID_FONT_SUBFAMILY);
        const auto [weight, style] = ReadFaceStyle(face->face, subfamily);
        const FaceKey key{ kind, family, weight, style };

        std::lock_guard<std::mutex> lock(m_mutex);
        const FamilyKey familyKey{ kind, family };
        const auto canonicalAlias = m_aliases.find(familyKey);
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
            const auto existingAlias = m_aliases.find(aliasKey);
            if ((existingAlias != m_aliases.end()) && (existingAlias->second != family)) {
                LogError(
                    "Font alias '%s' is already registered for '%s'.", alias.c_str(), existingAlias->second.c_str());
                return {};
            }
            addAlias = (existingAlias == m_aliases.end());
        }
        const auto existing = m_faces.find(key);
        if (existing != m_faces.end()) {
            if ((existing->second->identity != face->identity) || !FacesEquivalent(existing->second, face)) return {};
            if (addAlias) {
                m_aliases.emplace(std::move(aliasKey), family);
                ++m_generation;
            }
            return family;
        }
        m_faces.emplace(key, std::move(face));
        m_families.emplace(familyKey);
        if (kind == Kind::Music) {
            m_musicAnchors[key] = musicMetadata->anchors;
            m_parsedMusicMetadata.push_back(std::move(musicMetadata));
        }
        if (addAlias) m_aliases.emplace(std::move(aliasKey), family);
        m_shapeCache.clear();
        ++m_generation;
        return family;
    }

    std::shared_ptr<FaceData> Find(Kind kind, const std::string &family, Weight weight, Style style) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        const auto iterator = m_faces.find({ kind, ResolveFamilyLocked(kind, family), weight, style });
        return (iterator == m_faces.end()) ? NULL : iterator->second;
    }

    const std::string &ResolveFamilyLocked(Kind kind, const std::string &family) const
    {
        const auto alias = m_aliases.find({ kind, family });
        return (alias == m_aliases.end()) ? family : alias->second;
    }

    std::shared_ptr<FaceData> Find(FaceIdentity identity) const
    {
        if (!identity) return NULL;
        std::lock_guard<std::mutex> lock(m_mutex);
        for (const auto &[key, face] : m_faces) {
            if (face->identity == identity.value) return face;
        }
        return NULL;
    }

    mutable std::mutex m_mutex;
    std::unordered_map<FaceKey, std::shared_ptr<FaceData>, FaceKeyHash> m_faces;
    std::unordered_map<FaceKey, std::shared_ptr<const MusicAnchorMap>, FaceKeyHash> m_musicAnchors;
    std::unordered_set<FamilyKey, FamilyKeyHash> m_families;
    std::unordered_map<FamilyKey, std::string, FamilyKeyHash> m_aliases;
    std::unordered_multimap<uint64_t, std::shared_ptr<DecodedInput>> m_decodedInputs;
    mutable std::unordered_map<ShapeKey, ShapedRun, ShapeKeyHash> m_shapeCache;
    std::vector<std::shared_ptr<const ParsedMusicMetadata>> m_parsedMusicMetadata;
    uint64_t m_generation = 0;
    mutable Counters m_counters;
    mutable bool m_warnedMissingText = false;
    mutable bool m_warnedRtl = false;

    void PinBundledData()
    {
        std::scoped_lock lock(m_mutex, g_decodedInputsMutex, g_musicMetadataMutex);
        if (!g_pinnedDecodedInputs.empty()) return;
        g_pinnedDecodedInputs.reserve(m_decodedInputs.size());
        for (const auto &[hash, decoded] : m_decodedInputs) g_pinnedDecodedInputs.push_back(decoded);
        g_pinnedMusicMetadata = m_parsedMusicMetadata;
    }
};

FontStore::FontStore() : m_impl(std::make_unique<Impl>()) {}
FontStore::~FontStore() = default;
FontStore::FontStore(FontStore &&) noexcept = default;
FontStore &FontStore::operator=(FontStore &&) noexcept = default;

void FontStore::PinBundledData()
{
    m_impl->PinBundledData();
}

std::string FontStore::RegisterTextFont(const unsigned char *data, size_t length)
{
    return this->RegisterTextFont(data, length, {});
}

std::string FontStore::RegisterTextFont(const unsigned char *data, size_t length, const std::string &alias)
{
    return m_impl->Register(Kind::Text, data, length, {}, alias);
}

std::string FontStore::RegisterMusicFont(const unsigned char *data, size_t length, const std::string &smuflMetadataJson)
{
    return this->RegisterMusicFont(data, length, smuflMetadataJson, {});
}

std::string FontStore::RegisterMusicFont(
    const unsigned char *data, size_t length, const std::string &smuflMetadataJson, const std::string &alias)
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
    if (!hb_font_get_nominal_glyph(face->font, codepoint, &glyphId)) return std::nullopt;

    std::lock_guard<std::mutex> lock(face->cacheMutex);
    const auto existing = face->metrics.find(glyphId);
    if (existing != face->metrics.end()) return existing->second;

    hb_glyph_extents_t extents{};
    if (!hb_font_get_glyph_extents(face->font, glyphId, &extents)) return std::nullopt;
    GlyphMetrics metrics{ { face->identity }, glyphId, face->unitsPerEm,
        hb_font_get_glyph_h_advance(face->font, glyphId), extents.x_bearing, extents.y_bearing, extents.width,
        extents.height };
    face->metrics.emplace(glyphId, metrics);
    {
        std::lock_guard<std::mutex> counterLock(m_impl->m_mutex);
        ++m_impl->m_counters.extractedMetrics;
    }
    return metrics;
}

std::optional<FontStore::GlyphMetrics> FontStore::GetGlyphMetrics(FaceIdentity identity, uint32_t glyphId) const
{
    const std::shared_ptr<FaceData> face = m_impl->Find(identity);
    if (!face || (glyphId >= hb_face_get_glyph_count(face->face))) return std::nullopt;

    std::lock_guard<std::mutex> lock(face->cacheMutex);
    const auto existing = face->metrics.find(glyphId);
    if (existing != face->metrics.end()) return existing->second;

    hb_glyph_extents_t extents{};
    if (!hb_font_get_glyph_extents(face->font, glyphId, &extents)) return std::nullopt;
    GlyphMetrics metrics{ { face->identity }, glyphId, face->unitsPerEm,
        hb_font_get_glyph_h_advance(face->font, glyphId), extents.x_bearing, extents.y_bearing, extents.width,
        extents.height };
    face->metrics.emplace(glyphId, metrics);
    {
        std::lock_guard<std::mutex> counterLock(m_impl->m_mutex);
        ++m_impl->m_counters.extractedMetrics;
    }
    return metrics;
}

std::optional<std::string> FontStore::GetGlyphOutline(
    Kind kind, const std::string &family, uint32_t glyphId, Weight weight, Style style) const
{
    const std::shared_ptr<FaceData> face = m_impl->Find(kind, family, weight, style);
    if (!face || (glyphId >= hb_face_get_glyph_count(face->face))) return std::nullopt;
    std::lock_guard<std::mutex> lock(face->cacheMutex);
    const auto existing = face->outlines.find(glyphId);
    if (existing != face->outlines.end()) return existing->second;

    PathBuilder builder;
    if (!hb_font_draw_glyph_or_fail(face->font, glyphId, GetDrawFunctions(), &builder)) return std::nullopt;
    face->outlines.emplace(glyphId, builder.path);
    {
        std::lock_guard<std::mutex> counterLock(m_impl->m_mutex);
        ++m_impl->m_counters.extractedOutlines;
    }
    return builder.path;
}

std::optional<std::string> FontStore::GetGlyphOutline(FaceIdentity identity, uint32_t glyphId) const
{
    const std::shared_ptr<FaceData> face = m_impl->Find(identity);
    if (!face || (glyphId >= hb_face_get_glyph_count(face->face))) return std::nullopt;
    std::lock_guard<std::mutex> lock(face->cacheMutex);
    const auto existing = face->outlines.find(glyphId);
    if (existing != face->outlines.end()) return existing->second;

    PathBuilder builder;
    if (!hb_font_draw_glyph_or_fail(face->font, glyphId, GetDrawFunctions(), &builder)) return std::nullopt;
    face->outlines.emplace(glyphId, builder.path);
    {
        std::lock_guard<std::mutex> counterLock(m_impl->m_mutex);
        ++m_impl->m_counters.extractedOutlines;
    }
    return builder.path;
}

std::vector<FontStore::GlyphAnchor> FontStore::GetMusicGlyphAnchors(
    const std::string &family, const std::string &glyphName) const
{
    std::lock_guard<std::mutex> lock(m_impl->m_mutex);
    const auto face = m_impl->m_musicAnchors.find(
        { Kind::Music, m_impl->ResolveFamilyLocked(Kind::Music, family), Weight::Normal, Style::Normal });
    if (face == m_impl->m_musicAnchors.end()) return {};
    const auto glyph = face->second->find(glyphName);
    return (glyph == face->second->end()) ? std::vector<GlyphAnchor>() : glyph->second;
}

std::optional<FontStore::ShapedRun> FontStore::ShapeText(
    const std::string &family, const std::u32string &text, Weight weight, Style style) const
{
    const std::shared_ptr<FaceData> face = m_impl->Find(Kind::Text, family, weight, style);
    if (!face) return std::nullopt;
    const std::shared_ptr<FaceData> fallback
        = (family == "Tinos") ? face : m_impl->Find(Kind::Text, "Tinos", weight, style);
    std::lock_guard<std::mutex> cacheLock(m_impl->m_mutex);
    const ShapeKey key{ face.get(), text };
    const auto cached = m_impl->m_shapeCache.find(key);
    if (cached != m_impl->m_shapeCache.end()) return cached->second;

    auto shape = [](const std::shared_ptr<FaceData> &shapedFace, const std::u32string &value) {
        std::vector<GlyphPlacement> glyphs;
        hb_buffer_t *buffer = hb_buffer_create();
        hb_buffer_set_direction(buffer, HB_DIRECTION_LTR);
        hb_buffer_add_utf32(buffer, reinterpret_cast<const uint32_t *>(value.data()), static_cast<int>(value.size()), 0,
            static_cast<int>(value.size()));
        hb_buffer_guess_segment_properties(buffer);
        hb_buffer_set_direction(buffer, HB_DIRECTION_LTR);
        hb_shape(shapedFace->font, buffer, NULL, 0);
        unsigned int length = 0;
        const hb_glyph_info_t *infos = hb_buffer_get_glyph_infos(buffer, &length);
        const hb_glyph_position_t *positions = hb_buffer_get_glyph_positions(buffer, &length);
        glyphs.reserve(length);
        for (unsigned int i = 0; i < length; ++i) {
            glyphs.push_back({ { shapedFace->identity }, shapedFace->unitsPerEm, infos[i].codepoint, infos[i].cluster,
                positions[i].x_advance, positions[i].y_advance, positions[i].x_offset, positions[i].y_offset });
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

    ShapedRun run{ { face->identity }, face->unitsPerEm, shape(face, text) };
    if (fallback && (fallback != face)) {
        for (size_t begin = 0; begin < run.glyphs.size();) {
            size_t end = begin + 1;
            while ((end < run.glyphs.size()) && (run.glyphs[end].cluster == run.glyphs[begin].cluster)) ++end;
            const bool missing = std::any_of(run.glyphs.begin() + begin, run.glyphs.begin() + end,
                [](const GlyphPlacement &glyph) { return glyph.glyphId == 0; });
            if (missing) {
                const size_t textBegin = std::min<size_t>(run.glyphs[begin].cluster, text.size());
                const size_t textEnd
                    = (end < run.glyphs.size()) ? std::min<size_t>(run.glyphs[end].cluster, text.size()) : text.size();
                std::vector<GlyphPlacement> replacements = shape(fallback, text.substr(textBegin, textEnd - textBegin));
                for (GlyphPlacement &replacement : replacements) replacement.cluster += textBegin;
                run.glyphs.erase(run.glyphs.begin() + begin, run.glyphs.begin() + end);
                run.glyphs.insert(run.glyphs.begin() + begin, replacements.begin(), replacements.end());
                end = begin + replacements.size();
            }
            begin = end;
        }
    }
    if (!m_impl->m_warnedMissingText
        && std::ranges::any_of(run.glyphs, [](const GlyphPlacement &glyph) { return glyph.glyphId == 0; })) {
        LogWarning("A text cluster is missing from both the requested font and Tinos; using .notdef.");
        m_impl->m_warnedMissingText = true;
    }
    m_impl->m_shapeCache.emplace(key, run);
    ++m_impl->m_counters.shapedRuns;
    return run;
}

uint64_t FontStore::GetGeneration() const
{
    std::lock_guard<std::mutex> lock(m_impl->m_mutex);
    return m_impl->m_generation;
}

FontStore::Counters FontStore::GetCounters() const
{
    std::lock_guard<std::mutex> lock(m_impl->m_mutex);
    return m_impl->m_counters;
}

} // namespace vrv
