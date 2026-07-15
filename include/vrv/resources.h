/////////////////////////////////////////////////////////////////////////////
// Name:        resources.h
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_RESOURCES_H__
#define __VRV_RESOURCES_H__

#include <optional>
#include <unordered_map>

//----------------------------------------------------------------------------

#include "filereader.h"
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
    using StyleAttributes = std::pair<data_FONTWEIGHT, data_FONTSTYLE>;
    using GlyphTable = std::unordered_map<char32_t, Glyph>;
    using GlyphNameTable = std::unordered_map<std::string, char32_t>;
    using GlyphTextMap = std::map<StyleAttributes, GlyphTable>;

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

    /** Return the default registered text family. */
    void UseLiberationTextFont(bool useLiberation) { m_useLiberation = useLiberation; }
    bool UseLiberationTextFont() const { return m_useLiberation; }
    std::string GetTextFont() const { return m_textFontName; }
    void SetTextFont(const std::string &fontName) { m_textFontName = fontName; }

    /**
     * Font initialization
     */
    ///@{
    /** Register the bundled Bravura and Tinos faces. */
    bool InitFonts();
    /**  Set the font to be used and loads it if necessary */
    bool SetFont(const std::string &fontName);
    /** Add custom (external) fonts */
    bool AddCustom(const std::vector<std::string> &extraFonts);
    /** Load all music fonts available in the resource directory */
    bool LoadAll();
    /** Set a registered music fallback family. Bravura remains the final fallback. */
    void SetFallbackFont(const std::string &fontName);
    /** Get the fallback font name */
    std::string GetFallbackFont() const { return m_fallbackFontName; }

    /** Select a particular font */
    bool SetCurrentFont(const std::string &fontName, bool allowLoading = false);
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
     * Check if the current font is the fallback font
     */
    bool IsCurrentFontFallback() const;

    /**
     * Text fonts
     */
    ///@{
    /** Set current text style*/
    void SelectTextFont(data_FONTWEIGHT fontWeight, data_FONTSTYLE fontStyle) const;
    /** Shape text using the family and style requested by a drawing font. */
    std::optional<FontStore::ShapedRun> ShapeText(const FontInfo &font, const std::u32string &text) const;
    /** Return the scaled advance of a shaped run, including letter spacing. */
    int GetTextAdvance(const FontInfo &font, const FontStore::ShapedRun &run) const;
    /** Returns a glyph from the selected runtime text face. */
    const Glyph *GetTextGlyph(char32_t code) const;
    /** Returns a cached runtime glyph by immutable face identity and glyph ID. */
    const Glyph *GetRuntimeGlyph(
        FontStore::FaceIdentity face, uint32_t glyphId, const std::string &code = std::string()) const;
    /** Returns true if the specified font is loaded and it contains the requested glyph */
    bool FontHasGlyphAvailable(const std::string &fontName, char32_t smuflCode) const;
    ///@}

    /**
     * Get the CSS font string for the corresponding font.
     * Return an empty string if the font has not been loaded.
     */
    std::string GetCSSFontFor(const std::string &fontName) const;

    /**
     * Retrieve the font name either from the filename path or from the zipFile content.
     */
    std::string GetCustomFontname(const std::string &filename, const ZipFileReader &zipFile);

    /**
     * Static method that converts unicode music code points to SMuFL equivalent.
     * Return the parameter char if nothing can be converted.
     */
    static char32_t GetSmuflGlyphForUnicodeChar(const char32_t unicodeChar);

private:
    //----------------------------------------------------------------------------
    // LoadedFont
    //----------------------------------------------------------------------------

    class LoadedFont {

    public:
        LoadedFont(const std::string &name, bool isFallback) : m_name(name), m_isFallback(isFallback) {}
        ~LoadedFont() {}
        const std::string GetName() const { return m_name; };
        const GlyphTable &GetGlyphTable() const { return m_glyphTable; };
        GlyphTable &GetGlyphTableForModification() { return m_glyphTable; };
        bool isFallback() const { return m_isFallback; };

        void SetCSSFont(const std::string &css) { m_css = css; }
        std::string GetCSSFont(const std::string &path) const;

    private:
        std::string m_name;
        /** The loaded SMuFL font */
        GlyphTable m_glyphTable;
        /** If the font needs to fallback when a glyph is not present **/
        const bool m_isFallback;
        /** CSS font for font loaded as zip archive */
        std::string m_css;
    };

    //----------------------------------------------------------------------------

    bool LoadFont(const std::string &fontName, ZipFileReader *zipFile = NULL);

    /** Legacy ZIP-adapter text metric loader. */
    bool InitTextFont(const std::string &fontName, const StyleAttributes &style);

    const GlyphTable &GetCurrentGlyphTable() const { return m_loadedFonts.at(m_currentFontName).GetGlyphTable(); };
    const GlyphTable &GetFallbackGlyphTable() const { return m_loadedFonts.at(m_fallbackFontName).GetGlyphTable(); };

    bool m_useLiberation;
    std::string m_path;
    std::string m_defaultFontName;
    std::string m_fallbackFontName;
    std::map<std::string, LoadedFont> m_loadedFonts;
    std::string m_currentFontName;
    std::string m_textFontName;

    /** Text metrics retained only for the deprecated ZIP adapter. */
    GlyphTextMap m_textFont;
    mutable StyleAttributes m_currentStyle;
    /**
     * A map of glyph name / code
     */
    GlyphNameTable m_glyphNameTable;
    std::unordered_map<char32_t, std::string> m_glyphCodeNameTable;

    /** Cache of the last glyph that was looked up in loaded fonts */
    mutable std::optional<std::pair<char32_t, const Glyph *>> m_cachedGlyph;

    /** Runtime glyph records contain metrics only; outlines remain lazy in FontStore. */
    mutable std::unordered_map<uint64_t, std::unordered_map<uint32_t, Glyph>> m_runtimeGlyphs;

    /** Runtime OpenType faces, metrics, outlines, and shaped text. */
    FontStore m_fontStore;

    //----------------//
    // Static members //
    //----------------//

    /** The default path to the resources directory (e.g., for the svg/ subdirectory with fonts as XML */
    static thread_local std::string s_defaultPath;

    /** The default font style */
    static const StyleAttributes k_defaultStyle;
};

} // namespace vrv

#endif
