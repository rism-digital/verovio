/////////////////////////////////////////////////////////////////////////////
// Name:        fontstore.h
// Author:      Simon Waloschek
// Created:     2026
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FONTSTORE_H__
#define __VRV_FONTSTORE_H__

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace vrv {

//----------------------------------------------------------------------------
// FontStore
//----------------------------------------------------------------------------

/**
 * This class registers OpenType fonts at runtime and gives access to their glyph metrics, outlines, and anchors,
 * and to shaped text. Fonts are decoded once and their glyph data is extracted lazily.
 */
class FontStore {
public:
    enum class Kind { Music, Text };
    enum class Weight { Normal, Bold };
    enum class Style { Normal, Italic };

    /** The identity of a face, which is the same for identical font data */
    struct FaceIdentity {
        uint64_t m_value = 0;
        bool operator==(const FaceIdentity &) const = default;
        explicit operator bool() const { return m_value != 0; }
    };

    /** The metrics of a glyph, in font units */
    struct GlyphMetrics {
        FaceIdentity m_face;
        int m_glyphId = 0;
        int m_unitsPerEm = 0;
        int m_advanceX = 0;
        int m_xBearing = 0;
        int m_yBearing = 0;
        int m_width = 0;
        int m_height = 0;
    };

    /** A SMuFL anchor of a glyph, in staff spaces */
    struct GlyphAnchor {
        std::string m_name;
        double m_x = 0.0;
        double m_y = 0.0;
    };

    /** The placement of a glyph in a shaped run, in font units */
    struct GlyphPlacement {
        FaceIdentity m_face;
        int m_unitsPerEm = 0;
        int m_glyphId = 0;
        int m_cluster = 0;
        int m_advanceX = 0;
        int m_advanceY = 0;
        int m_offsetX = 0;
        int m_offsetY = 0;
        bool operator==(const GlyphPlacement &) const = default;
    };

    /** A shaped text run */
    struct ShapedRun {
        FaceIdentity m_face;
        int m_unitsPerEm = 0;
        std::vector<GlyphPlacement> m_glyphs;
        bool operator==(const ShapedRun &) const = default;

        /** Return the number of gaps between clusters, where letter spacing applies */
        int GetClusterGapCount() const;
    };

    /** A registered font file, kept in the format it was registered with (e.g., WOFF2 is not decompressed) */
    struct FontFile {
        Weight m_weight = Weight::Normal;
        Style m_style = Style::Normal;
        /** The CSS @font-face format: "woff2", "woff", "opentype", or "truetype" */
        std::string m_format;
        std::string m_mimeType;
        std::vector<unsigned char> m_data;
    };

    /** The amount of work done by the store, for checking that the glyph data is extracted lazily */
    struct Counters {
        int m_decodedFonts = 0;
        int m_extractedMetrics = 0;
        int m_extractedOutlines = 0;
        int m_shapedRuns = 0;
    };

    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    FontStore();
    ~FontStore();
    FontStore(FontStore &&) noexcept;
    FontStore &operator=(FontStore &&) noexcept;
    FontStore(const FontStore &) = delete;
    FontStore &operator=(const FontStore &) = delete;
    ///@}

    /**
     * @name Register a font from OTF, TTF, WOFF, or WOFF2 data.
     * Return the canonical family name, or an empty string if the font could not be registered.
     * The alias is an additional family name for the font.
     */
    ///@{
    std::string RegisterTextFont(const unsigned char *data, int length, const std::string &alias = "");
    std::string RegisterMusicFont(
        const unsigned char *data, int length, const std::string &smuflMetadataJson, const std::string &alias = "");
    ///@}

    /**
     * @name Getters for the registered faces
     */
    ///@{
    bool HasFace(
        Kind kind, const std::string &family, Weight weight = Weight::Normal, Style style = Style::Normal) const;
    std::optional<GlyphMetrics> GetGlyphMetrics(Kind kind, const std::string &family, char32_t codepoint,
        Weight weight = Weight::Normal, Style style = Style::Normal) const;
    std::optional<GlyphMetrics> GetGlyphMetrics(FaceIdentity face, int glyphId) const;
    std::optional<std::string> GetGlyphOutline(Kind kind, const std::string &family, int glyphId,
        Weight weight = Weight::Normal, Style style = Style::Normal) const;
    std::optional<std::string> GetGlyphOutline(FaceIdentity face, int glyphId) const;
    std::vector<GlyphAnchor> GetMusicGlyphAnchors(const std::string &family, const std::string &glyphName) const;
    /** Return the files of all faces registered for a family (or alias), without synthesized faces */
    std::vector<FontFile> GetFontFiles(Kind kind, const std::string &family) const;
    ///@}

    /**
     * Shape a text with a text family.
     * Clusters missing in the text family are taken from the music families and from Tinos.
     */
    std::optional<ShapedRun> ShapeText(const std::string &family, const std::u32string &text,
        Weight weight = Weight::Normal, Style style = Style::Normal, const std::string &musicFamily = "",
        const std::string &musicFallbackFamily = "") const;

    /** Return a value changed with each registration */
    uint64_t GetGeneration() const;

    /** Pin the currently registered bundled faces in the process cache */
    void PinBundledData();

    /** Return the amount of work done by the store */
    Counters GetCounters() const;

    /** Read a font or metadata file, which is empty if it cannot be read or is too large to be registered */
    static std::vector<unsigned char> ReadFile(const std::string &filename);

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace vrv

#endif // __VRV_FONTSTORE_H__
