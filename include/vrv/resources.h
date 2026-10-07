/////////////////////////////////////////////////////////////////////////////
// Name:        resources.h
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_RESOURCES_H__
#define __VRV_RESOURCES_H__

#include <map>
#include <optional>
#include <unordered_map>

//----------------------------------------------------------------------------

#include "fontstore.h"
#include "glyph.h"

namespace vrv {

class FontInfo;

//----------------------------------------------------------------------------
// Resources
//----------------------------------------------------------------------------

/**
 * This class provides resource values.
 * It manages fonts and glyph tables.
 */

class Resources {
public:
    using GlyphNameTable = std::unordered_map<std::string, char32_t>;

    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    Resources();
    virtual ~Resources() = default;
    ///@}

    /**
     * @name Setters and getters
     */
    ///@{
    static std::string GetDefaultPath() { return s_defaultPath; }
    static void SetDefaultPath(const std::string &path) { s_defaultPath = path; }

    std::string GetPath() const { return m_path; }
    void SetPath(const std::string &path) { m_path = path; }
    const FontStore &GetFontStore() const { return m_fontStore; }
    FontStore &GetFontStoreForModification() { return m_fontStore; }
    ///@}

    /** Status checker */
    bool Ok() const;

    /**
     * Font initialization
     */
    ///@{
    /** Register the bundled Leipzig, Bravura, and Tinos faces */
    bool InitFonts();
    /**
     * Return the font file of a music font for embedding in the SVG, which is a subset for the bundled fonts.
     * The file name is given for the bundled fonts, which are also published on the website.
     */
    std::optional<FontStore::FontFile> GetMusicFontForEmbedding(
        const std::string &fontName, std::string &bundledFile) const;
    /** Set the music fallback family, which has to be registered. Bravura remains the final fallback. */
    bool SetFallbackFont(const std::string &fontName);
    /** Get the fallback font name */
    std::string GetFallbackFont() const { return m_fallbackFontName; }

    /** Select a particular font */
    bool SetCurrentFont(const std::string &fontName);
    std::string GetCurrentFont() const { return m_currentFontName; }
    bool IsFontLoaded(const std::string &fontName) const;
    ///@}

    /**
     * Retrieving glyphs
     */
    ///@{
    /** Returns the glyph (if exists) for a glyph code in the current SMuFL font */
    const Glyph *GetGlyph(char32_t smuflCode) const;
    /** Returns a music glyph using an explicit registered family. */
    const Glyph *GetGlyph(char32_t smuflCode, const std::string &fontName) const;
    /** Returns the glyph (if exists) for a glyph name in the current SMuFL font */
    const Glyph *GetGlyph(const std::string &smuflName) const;
    /** Returns the glyph (if exists) for a glyph name in the current SMuFL font */
    char32_t GetGlyphCode(const std::string &smuflName) const;
    ///@}

    /**
     * Check if the text has any character that needs the smufl fallback font
     */
    bool IsSmuflFallbackNeeded(const std::u32string &text) const;

    /**
     * Text fonts
     */
    ///@{
    /** Set the default text family, which has to be registered */
    bool SetTextFont(const std::string &fontName);
    /** Return the default text family */
    std::string GetTextFont() const { return m_textFontName; }
    /** The widespread font family with the same metrics as the text font, if any (e.g., Times for Tinos) */
    std::string GetTextFontMetricEquivalent() const;
    /** Shape text using the family and style requested by a drawing font. */
    std::optional<FontStore::ShapedRun> ShapeText(const FontInfo &font, const std::u32string &text) const;
    /** Return the scaled advance of a shaped run, including letter spacing. */
    int GetTextAdvance(const FontInfo &font, const FontStore::ShapedRun &run) const;
    /** Returns a glyph from the runtime face selected by a drawing font. */
    const Glyph *GetTextGlyph(char32_t code, const FontInfo &font) const;
    /** Returns a cached runtime glyph by immutable face identity and glyph ID. */
    const Glyph *GetRuntimeGlyph(FontStore::FaceIdentity face, int glyphId, const std::string &code = "") const;
    /** Returns true if the specified font is loaded and it contains the requested glyph */
    bool FontHasGlyphAvailable(const std::string &fontName, char32_t smuflCode) const;
    ///@}

    /**
     * Static method that converts unicode music code points to SMuFL equivalent.
     * Return the parameter char if nothing can be converted.
     */
    static char32_t GetSmuflGlyphForUnicodeChar(const char32_t unicodeChar);

private:
    std::string m_path;
    std::string m_fallbackFontName;
    std::string m_currentFontName;
    std::string m_textFontName;

    /** Cache of the last glyph that was looked up in the current font */
    mutable std::optional<std::pair<char32_t, const Glyph *>> m_cachedGlyph;

    /** Runtime glyph records contain metrics only; outlines remain lazy in FontStore. */
    mutable std::unordered_map<uint64_t, std::unordered_map<int, Glyph>> m_runtimeGlyphs;

    /** The subsets of the bundled music fonts with the glyphs supported by Verovio, for embedding */
    std::map<std::string, std::vector<unsigned char>> m_musicFontSubsets;

    /** Runtime OpenType faces, metrics, outlines, and shaped text. */
    FontStore m_fontStore;

    //----------------//
    // Static members //
    //----------------//

    /** The default path to the resources directory (e.g., for the fonts/ subdirectory) */
    static thread_local std::string s_defaultPath;
};

} // namespace vrv

#endif
