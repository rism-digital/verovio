/////////////////////////////////////////////////////////////////////////////
// Name:        fontstore.h
// Purpose:     Runtime OpenType font registration and lazy glyph access
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FONTSTORE_H__
#define __VRV_FONTSTORE_H__

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace vrv {

class FontStore {
public:
    enum class Kind { Music, Text };
    enum class Weight { Normal, Bold };
    enum class Style { Normal, Italic };

    struct FaceIdentity {
        uint64_t value = 0;
        bool operator==(const FaceIdentity &) const = default;
        explicit operator bool() const { return value != 0; }
    };

    struct GlyphMetrics {
        FaceIdentity face;
        uint32_t glyphId = 0;
        int unitsPerEm = 0;
        int advanceX = 0;
        int xBearing = 0;
        int yBearing = 0;
        int width = 0;
        int height = 0;
    };

    struct GlyphAnchor {
        std::string name;
        double x = 0.0;
        double y = 0.0;
    };

    struct GlyphPlacement {
        FaceIdentity face;
        int unitsPerEm = 0;
        uint32_t glyphId = 0;
        uint32_t cluster = 0;
        int advanceX = 0;
        int advanceY = 0;
        int offsetX = 0;
        int offsetY = 0;
        bool operator==(const GlyphPlacement &) const = default;
    };

    struct ShapedRun {
        FaceIdentity face;
        int unitsPerEm = 0;
        std::vector<GlyphPlacement> glyphs;
        bool operator==(const ShapedRun &) const = default;
    };

    struct Counters {
        size_t decodedFonts = 0;
        size_t extractedMetrics = 0;
        size_t extractedOutlines = 0;
        size_t shapedRuns = 0;
    };

    FontStore();
    ~FontStore();
    FontStore(FontStore &&) noexcept;
    FontStore &operator=(FontStore &&) noexcept;
    FontStore(const FontStore &) = delete;
    FontStore &operator=(const FontStore &) = delete;

    std::string RegisterTextFont(const unsigned char *data, size_t length);
    std::string RegisterTextFont(const unsigned char *data, size_t length, const std::string &alias);
    std::string RegisterMusicFont(const unsigned char *data, size_t length, const std::string &smuflMetadataJson);
    std::string RegisterMusicFont(
        const unsigned char *data, size_t length, const std::string &smuflMetadataJson, const std::string &alias);

    bool HasFace(
        Kind kind, const std::string &family, Weight weight = Weight::Normal, Style style = Style::Normal) const;
    std::optional<GlyphMetrics> GetGlyphMetrics(Kind kind, const std::string &family, char32_t codepoint,
        Weight weight = Weight::Normal, Style style = Style::Normal) const;
    std::optional<GlyphMetrics> GetGlyphMetrics(FaceIdentity face, uint32_t glyphId) const;
    std::optional<std::string> GetGlyphOutline(Kind kind, const std::string &family, uint32_t glyphId,
        Weight weight = Weight::Normal, Style style = Style::Normal) const;
    std::optional<std::string> GetGlyphOutline(FaceIdentity face, uint32_t glyphId) const;
    std::vector<GlyphAnchor> GetMusicGlyphAnchors(const std::string &family, const std::string &glyphName) const;
    std::optional<ShapedRun> ShapeText(const std::string &family, const std::u32string &text,
        Weight weight = Weight::Normal, Style style = Style::Normal) const;

    uint64_t GetGeneration() const;
    /** Pin the currently registered bundled faces in the bounded process cache. */
    void PinBundledData();
    Counters GetCounters() const;

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace vrv

#endif
