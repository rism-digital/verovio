/////////////////////////////////////////////////////////////////////////////
// Name:        resources.h
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_RESOURCES_H__
#define __VRV_RESOURCES_H__

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

    /**
     * Font initialization
     */
    ///@{
    /** Init the SMufL music and text fonts */
    bool InitFonts();
    /** Init the text font (bounding boxes and ASCII only) */
    bool InitTextFont(const std::string &fontName, const StyleAttributes &style);
    /** Select a particular font */
    bool SetFont(const std::string &fontName);
    std::string GetCurrentFontName() const { return m_fontName; }
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
     * Text fonts
     */
    ///@{
    /** Set current text style*/
    void SelectTextFont(data_FONTWEIGHT fontWeight, data_FONTSTYLE fontStyle) const;
    /** Returns the glyph (if exists) for the text font (bounding box and ASCII only) */
    const Glyph *GetTextGlyph(char32_t code) const;
    ///@}

    /**
     * Static method that converts unicode music code points to SMuFL equivalent.
     * Return the parameter char if nothing can be converted.
     */
    static char32_t GetSmuflGlyphForUnicodeChar(const char32_t unicodeChar);

private:
    bool LoadFont(const std::string &fontName);

private:
    /** The font name of the font that is currently loaded */
    std::string m_fontName;
    /** The path to the resources directory (e.g., for the svg/ subdirectory with fonts as XML */
    std::string m_path;
    /** The loaded SMuFL font */
    GlyphTable m_fontGlyphTable;
    /** A text font used for bounding box calculations */
    GlyphTextMap m_textFont;
    mutable StyleAttributes m_currentStyle;
    /**
     * A map of glyph name / code
     */
    GlyphNameTable m_glyphNameTable;

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
