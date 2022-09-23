/////////////////////////////////////////////////////////////////////////////
// Name:        resources.cpp
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "resources.h"

//----------------------------------------------------------------------------

#include <string>

//----------------------------------------------------------------------------

#include "smufl.h"
#include "vrvdef.h"

//----------------------------------------------------------------------------

#include "pugixml.hpp"

namespace vrv {

//----------------------------------------------------------------------------
// Static members with some default values
//----------------------------------------------------------------------------

thread_local std::string Resources::s_defaultPath = "/usr/local/share/verovio";
const Resources::StyleAttributes Resources::k_defaultStyle{ data_FONTWEIGHT::FONTWEIGHT_normal,
    data_FONTSTYLE::FONTSTYLE_normal };

//----------------------------------------------------------------------------
// Resources
//----------------------------------------------------------------------------

Resources::Resources()
{
    m_path = s_defaultPath;
    m_currentStyle = k_defaultStyle;
}

bool Resources::InitFonts()
{
    // We will need to rethink this for adding the option to add custom fonts
    // Font Bravura first since it is expected to have always all symbols
    if (!LoadFont("Bravura")) LogError("Bravura font could not be loaded.");
    // The Leipzig as the default font
    if (!LoadFont("Leipzig")) LogError("Leipzig font could not be loaded.");

    if (m_fontGlyphTable.size() < SMUFL_COUNT) {
        LogError("Expected %d default SMuFL glyphs but could load only %d.", SMUFL_COUNT, m_fontGlyphTable.size());
        return false;
    }

    struct TextFontInfo_type {
        const StyleAttributes m_style;
        const std::string m_fileName;
        bool m_isMandatory;
    };

    static const TextFontInfo_type textFontInfos[]
        = { { k_defaultStyle, "Times", true }, { { FONTWEIGHT_bold, FONTSTYLE_normal }, "Times-bold", false },
              { { FONTWEIGHT_bold, FONTSTYLE_italic }, "Times-bold-italic", false },
              { { FONTWEIGHT_normal, FONTSTYLE_italic }, "Times-italic", false } };

    for (const auto &textFontInfo : textFontInfos) {
        if (!InitTextFont(textFontInfo.m_fileName, textFontInfo.m_style) && textFontInfo.m_isMandatory) {
            LogError("Text font could not be initialized.");
            return false;
        }
    }

    m_currentStyle = k_defaultStyle;

    return true;
}

bool Resources::SetFont(const std::string &fontName)
{
    return LoadFont(fontName);
}

const Glyph *Resources::GetGlyph(char32_t smuflCode) const
{
    return m_fontGlyphTable.count(smuflCode) ? &m_fontGlyphTable.at(smuflCode) : NULL;
}

const Glyph *Resources::GetGlyph(const std::string &smuflName) const
{
    return m_glyphNameTable.count(smuflName) ? &m_fontGlyphTable.at(m_glyphNameTable.at(smuflName)) : NULL;
}

char32_t Resources::GetGlyphCode(const std::string &smuflName) const
{
    return m_glyphNameTable.count(smuflName) ? m_glyphNameTable.at(smuflName) : 0;
}

void Resources::SelectTextFont(data_FONTWEIGHT fontWeight, data_FONTSTYLE fontStyle) const
{
    if (fontWeight == FONTWEIGHT_NONE) {
        fontWeight = FONTWEIGHT_normal;
    }

    if (fontStyle == FONTSTYLE_NONE) {
        fontStyle = FONTSTYLE_normal;
    }

    m_currentStyle = { fontWeight, fontStyle };
    if (m_textFont.count(m_currentStyle) == 0) {
        LogWarning("Text font for style (%d, %d) is not loaded. Use default", fontWeight, fontStyle);
        m_currentStyle = k_defaultStyle;
    }
}

const Glyph *Resources::GetTextGlyph(char32_t code) const
{
    const StyleAttributes style = (m_textFont.count(m_currentStyle) != 0) ? m_currentStyle : k_defaultStyle;
    if (m_textFont.count(style) == 0) return NULL;

    const GlyphTable &currentTable = m_textFont.at(style);
    if (currentTable.count(code) == 0) {
        return NULL;
    }

    return &currentTable.at(code);
}

char32_t Resources::GetSmuflGlyphForUnicodeChar(const char32_t unicodeChar)
{
    char32_t smuflChar = unicodeChar;
    switch (unicodeChar) {
        case UNICODE_DAL_SEGNO: smuflChar = SMUFL_E045_dalSegno; break;
        case UNICODE_DA_CAPO: smuflChar = SMUFL_E046_daCapo; break;
        case UNICODE_SEGNO: smuflChar = SMUFL_E047_segno; break;
        case UNICODE_CODA: smuflChar = SMUFL_E048_coda; break;
        default: break;
    }
    return smuflChar;
}

bool Resources::LoadFont(const std::string &fontName)
{
    pugi::xml_document doc;
    const std::string filename = Resources::GetPath() + "/" + fontName + ".xml";
    pugi::xml_parse_result parseResult = doc.load_file(filename.c_str());
    if (!parseResult) {
        // File not found, default bounding boxes will be used
        LogError("Failed to load font and glyph bounding boxes");
        return false;
    }
    pugi::xml_node root = doc.first_child();
    if (!root.attribute("units-per-em")) {
        LogError("No units-per-em attribute in bouding box file");
        return false;
    }

    const int unitsPerEm = atoi(root.attribute("units-per-em").value());

    for (pugi::xml_node current = root.child("g"); current; current = current.next_sibling("g")) {
        pugi::xml_attribute c_attribute = current.attribute("c");
        pugi::xml_attribute n_attribute = current.attribute("n");
        if (!c_attribute || !n_attribute) continue;

        Glyph glyph;
        glyph.SetUnitsPerEm(unitsPerEm * 10);
        glyph.SetCodeStr(c_attribute.value());
        float x = 0.0, y = 0.0, width = 0.0, height = 0.0;
        if (current.attribute("x")) x = current.attribute("x").as_float();
        if (current.attribute("y")) y = current.attribute("y").as_float();
        if (current.attribute("w")) width = current.attribute("w").as_float();
        if (current.attribute("h")) height = current.attribute("h").as_float();
        glyph.SetBoundingBox(x, y, width, height);
        glyph.SetPath(Resources::GetPath() + "/" + fontName + "/" + c_attribute.value() + ".xml");
        if (current.attribute("h-a-x")) glyph.SetHorizAdvX(current.attribute("h-a-x").as_float());

        // load anchors
        pugi::xml_node anchor;
        for (anchor = current.child("a"); anchor; anchor = anchor.next_sibling("a")) {
            if (anchor.attribute("n")) {
                std::string name = std::string(anchor.attribute("n").value());
                // No check for possible x and y missing attributes - not very safe.

                glyph.SetAnchor(name, anchor.attribute("x").as_float(), anchor.attribute("y").as_float());
            }
        }

        const char32_t smuflCode = (char32_t)strtol(c_attribute.value(), NULL, 16);
        m_fontGlyphTable[smuflCode] = glyph;
        m_glyphNameTable[n_attribute.value()] = smuflCode;
    }

    m_fontName = fontName;
    return true;
}

bool Resources::InitTextFont(const std::string &fontName, const StyleAttributes &style)
{
    // For the text font, we load the bounding boxes only
    pugi::xml_document doc;
    // For now, we have only Times bounding boxes for ASCII chars
    // For any other char, we currently use 'o' bounding box
    std::string filename = GetPath() + "/text/" + fontName + ".xml";
    pugi::xml_parse_result result = doc.load_file(filename.c_str());
    if (!result) {
        // File not found, default bounding boxes will be used
        LogMessage("Cannot load bounding boxes for text font '%s'", filename.c_str());
        return false;
    }
    pugi::xml_node root = doc.first_child();
    if (!root.attribute("units-per-em")) {
        LogWarning("No units-per-em attribute in bouding box file");
        return false;
    }
    const int unitsPerEm = root.attribute("units-per-em").as_int();
    pugi::xml_node current;
    if (m_textFont.count(style) == 0) {
        m_textFont[style] = {};
    }
    GlyphTable &currentTable = m_textFont.at(style);
    for (current = root.child("g"); current; current = current.next_sibling("g")) {
        if (current.attribute("c")) {
            char32_t code = (char32_t)strtol(current.attribute("c").value(), NULL, 16);
            // We create a glyph with only the units per em which is the only info we need for
            // the bounding boxes; path and codeStr will remain [unset]
            Glyph glyph(unitsPerEm);
            float x = 0.0, y = 0.0, width = 0.0, height = 0.0;
            // Not check for missing values...
            if (current.attribute("x")) x = current.attribute("x").as_float();
            if (current.attribute("y")) y = current.attribute("y").as_float();
            if (current.attribute("w")) width = current.attribute("w").as_float();
            if (current.attribute("h")) height = current.attribute("h").as_float();
            glyph.SetBoundingBox(x, y, width, height);

            if (current.attribute("h-a-x")) glyph.SetHorizAdvX(current.attribute("h-a-x").as_float());
            if (currentTable.count(code) > 0) {
                LogDebug("Redefining %d with %s", code, fontName.c_str());
            }
            currentTable[code] = glyph;
        }
    }
    return true;
}

} // namespace vrv
