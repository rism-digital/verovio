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
#include "glyph.h"

namespace vrv {

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
    ///@}

    /** Status checker */
    bool Ok() const { return (m_loadedFonts.size() > 1); }

    /**
     * Return the name of the text font (Times or Liberation)
     */
    void UseLiberationTextFont(bool useLiberation) { m_useLiberation = useLiberation; }
    bool UseLiberationTextFont() const { return m_useLiberation; }
    std::string GetTextFont() const { return ((m_useLiberation) ? "Liberation" : "Times"); }

    /**
     * Font initialization
     */
    ///@{
    /** Init the SMufL music and text fonts */
    bool InitFonts();
    /**  Set the font to be used and loads it if necessary */
    bool SetFont(const std::string &fontName);
    /** Add custom (external) fonts */
    bool AddCustom(const std::vector<std::string> &extraFonts);
    /** Load all music fonts available in the resource directory */
    bool LoadAll();
    /** Set the fallback font (Leipzig or Bravura) when some glyphs are missing in the current font */
    void SetFallbackFont(const std::string &fontName);
    /** Get the fallback font name */
    std::string GetFallbackFont() const { return m_fallbackFontName; }

    /** Select a particular font */
    bool SetCurrentFont(const std::string &fontName, bool allowLoading = false);
    std::string GetCurrentFont() const { return m_currentFontName; }
    bool IsFontLoaded(const std::string &fontName) const { return m_loadedFonts.find(fontName) != m_loadedFonts.end(); }
    ///@}

    /**
     * Retrieving glyphs
     */
    ///@{
    /** Returns the glyph (if exists) for a glyph code in the current SMuFL font */
    const Glyph *GetGlyph(char32_t smuflCode) const;
    /** Returns the glyph (if exists) for a glyph name in the current SMuFL font */
    const Glyph *GetGlyph(const std::string &smuflName) const;
    /** Returns the glyph (if exists) for a glyph name in the current SMuFL font */
    char32_t GetGlyphCode(const std::string &smuflName) const;
    ///@}

    /**
     * Check if the text has any charachter that needs the smufl fallback font
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
    /** Returns the glyph (if exists) for the text font (bounding box and ASCII only) */
    const Glyph *GetTextGlyph(char32_t code) const;
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

    /** Init the text font (bounding boxes and ASCII only) */
    bool InitTextFont(const std::string &fontName, const StyleAttributes &style);

    const GlyphTable &GetCurrentGlyphTable() const { return m_loadedFonts.at(m_currentFontName).GetGlyphTable(); };
    const GlyphTable &GetFallbackGlyphTable() const { return m_loadedFonts.at(m_fallbackFontName).GetGlyphTable(); };

    bool m_useLiberation;
    std::string m_path;
    std::string m_defaultFontName;
    std::string m_fallbackFontName;
    std::map<std::string, LoadedFont> m_loadedFonts;
    std::string m_currentFontName;

    /** A text font used for bounding box calculations */
    GlyphTextMap m_textFont;
    mutable StyleAttributes m_currentStyle;
    /**
     * A map of glyph name / code
     */
    GlyphNameTable m_glyphNameTable;

    /** Cache of the last glyph that was looked up in loaded fonts */
    mutable std::optional<std::pair<char32_t, const Glyph *>> m_cachedGlyph;

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
