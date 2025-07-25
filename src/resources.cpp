/////////////////////////////////////////////////////////////////////////////
// Name:        resources.cpp
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "resources.h"

//----------------------------------------------------------------------------

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

//----------------------------------------------------------------------------

#include "smufl.h"
#include "vrv.h"
#include "vrvdef.h"

//----------------------------------------------------------------------------

#include "pugixml.hpp"

#define BRAVURA "Bravura"
#define LEIPZIG "Leipzig"

namespace vrv {

//----------------------------------------------------------------------------
// Static members with some default values
//----------------------------------------------------------------------------

thread_local std::string Resources::s_defaultPath = VRV_RESOURCE_DIR;
const Resources::StyleAttributes Resources::k_defaultStyle{ data_FONTWEIGHT::FONTWEIGHT_normal,
    data_FONTSTYLE::FONTSTYLE_normal };

//----------------------------------------------------------------------------
// Function defined in toolkitdef.h
//----------------------------------------------------------------------------

void SetDefaultResourcePath(const std::string &path)
{
    Resources::SetDefaultPath(path);
}

//----------------------------------------------------------------------------
// Resources
//----------------------------------------------------------------------------

Resources::Resources()
{
    m_path = s_defaultPath;
    m_currentStyle = k_defaultStyle;
    m_useLiberation = false;
}

bool Resources::InitFonts()
{
    m_cachedGlyph.reset();
    m_loadedFonts.clear();

    // Font Bravura first. As it is expected to have always all symbols we build the code -> name table from it
    if (!LoadFont(BRAVURA)) LogError("Bravura font could not be loaded.");
    // Leipzig is our initial default font
    if (!LoadFont(LEIPZIG)) LogError("Leipzig font could not be loaded.");

    m_defaultFontName = LEIPZIG;
    m_currentFontName = m_defaultFontName;
    m_fallbackFontName = m_defaultFontName;

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
    m_cachedGlyph.reset();

    // add the default font provided in options, if it is not one of the previous
    if (!fontName.empty() && !IsFontLoaded(fontName)) {
        if (!LoadFont(fontName)) {
            LogError("%s font could not be loaded.", fontName.c_str());
            return false;
        }
    }

    m_defaultFontName = IsFontLoaded(fontName) ? fontName : LEIPZIG;
    m_currentFontName = m_defaultFontName;

    return true;
}

bool Resources::AddCustom(const std::vector<std::string> &extraFonts)
{
    bool success = true;
    // options supplied fonts
    for (const std::string &fontFile : extraFonts) {
        ZipFileReader zipFile;
        if (!zipFile.Load(fontFile)) {
            continue;
        }
        std::string fontName = GetCustomFontname(fontFile, zipFile);
        if (fontName.empty() || IsFontLoaded(fontName)) {
            continue;
        }
        success = success && LoadFont(fontName, &zipFile);
        if (!success) {
            LogError("Option supplied font %s could not be loaded.", fontName.c_str());
        }
    }
    return success;
}

bool Resources::LoadAll()
{
    std::string path = Resources::GetPath() + "/";
    return std::ranges::all_of(
        std::filesystem::directory_iterator(path), [this](const std::filesystem::directory_entry &entry) {
            const std::filesystem::path &path = entry.path();
            if (path.has_extension() && path.has_stem() && path.extension() == ".xml") {
                const std::string fontName = path.stem().string();
                if (!this->IsFontLoaded(fontName) && !this->LoadFont(fontName)) {
                    return false;
                }
            }
            return true;
        });
}

void Resources::SetFallbackFont(const std::string &fontName)
{
    m_cachedGlyph.reset();
    m_fallbackFontName = fontName;
}

bool Resources::SetCurrentFont(const std::string &fontName, bool allowLoading)
{
    m_cachedGlyph.reset();

    if (IsFontLoaded(fontName)) {
        m_currentFontName = fontName;
        return true;
    }
    else if (allowLoading && LoadFont(fontName)) {
        m_currentFontName = fontName;
        return true;
    }

    return false;
}

const Glyph *Resources::GetGlyph(char32_t smuflCode) const
{
    if (m_cachedGlyph && m_cachedGlyph->first == smuflCode) {
        return m_cachedGlyph->second;
    }

    const GlyphTable &currentTable = this->GetCurrentGlyphTable();
    if (auto glyphIter = currentTable.find(smuflCode); glyphIter != currentTable.end()) {
        const Glyph *glyph = &glyphIter->second;
        m_cachedGlyph = std::make_pair(glyphIter->first, glyph);
        return glyph;
    }
    else if (!this->IsCurrentFontFallback()) {
        const GlyphTable &fallbackTable = this->GetFallbackGlyphTable();
        if (auto glyphIter = fallbackTable.find(smuflCode); glyphIter != fallbackTable.end()) {
            const Glyph *glyph = &glyphIter->second;
            m_cachedGlyph = std::make_pair(glyphIter->first, glyph);
            return glyph;
        }
    }
    return NULL;
}

const Glyph *Resources::GetGlyph(const std::string &smuflName) const
{
    if (const char32_t code = this->GetGlyphCode(smuflName); code) {
        return this->GetGlyph(code);
    }
    return NULL;
}

char32_t Resources::GetGlyphCode(const std::string &smuflName) const
{
    if (auto glyphNameIter = m_glyphNameTable.find(smuflName); glyphNameIter != m_glyphNameTable.end()) {
        return glyphNameIter->second;
    }
    return 0;
}

bool Resources::IsSmuflFallbackNeeded(const std::u32string &text) const
{
    if (m_loadedFonts.at(m_currentFontName).isFallback()) {
        return false;
    }
    for (char32_t c : text) {
        if (!GetCurrentGlyphTable().contains(c)) return true;
    }
    return false;
}

bool Resources::IsCurrentFontFallback() const
{
    return (m_currentFontName == m_fallbackFontName);
}

bool Resources::FontHasGlyphAvailable(const std::string &fontName, char32_t smuflCode) const
{
    if (!IsFontLoaded(fontName)) {
        return false;
    }

    const GlyphTable &table = m_loadedFonts.at(fontName).GetGlyphTable();
    return (table.find(smuflCode) != table.end());
}

std::string Resources::GetCSSFontFor(const std::string &fontName) const
{
    if (fontName == this->GetTextFont()) {
        const std::string cssFontPath = StringFormat("%s/%s.css", m_path.c_str(), this->GetTextFont().c_str());
        std::ifstream fstream(cssFontPath);
        std::stringstream sstream;
        sstream << fstream.rdbuf();
        return sstream.str();
    }

    if (!IsFontLoaded(fontName)) {
        return "";
    }

    const LoadedFont &font = m_loadedFonts.at(fontName);
    return font.GetCSSFont(m_path);
}

std::string Resources::GetCustomFontname(const std::string &filename, const ZipFileReader &zipFile)
{
#ifdef __EMSCRIPTEN__
    // Extracts the font name from the bounding box XML file
    // For example, OneGlyph/OneGlpyh.xml
    for (auto &s : zipFile.GetFileList()) {
        std::filesystem::path path(s);
        if (!path.has_parent_path() || (path.parent_path() == path.stem())) {
            if (path.has_extension() && (path.extension() == ".xml")) {
                return path.stem();
            }
        }
    }
    LogWarning("The font name could not be extracted from the archive XML file");
    return "";
#else
    std::filesystem::path path(filename);
    return (path.has_stem()) ? path.stem().string() : "";
#endif
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
    if (!m_textFont.contains(m_currentStyle)) {
        LogWarning("Text font for style (%d, %d) is not loaded. Use default", fontWeight, fontStyle);
        m_currentStyle = k_defaultStyle;
    }
}

const Glyph *Resources::GetTextGlyph(char32_t code) const
{
    const StyleAttributes style = m_textFont.contains(m_currentStyle) ? m_currentStyle : k_defaultStyle;
    if (!m_textFont.contains(style)) return NULL;

    const GlyphTable &currentTable = m_textFont.at(style);
    if (!currentTable.contains(code)) {
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

bool Resources::LoadFont(const std::string &fontName, ZipFileReader *zipFile)
{
    pugi::xml_document doc;
    // For zip archive custom font, load the data from the zipFile
    if (zipFile) {
        const std::string filename = fontName + ".xml";
        if (!zipFile->HasFile(filename)) {
            // File not found, default bounding boxes will be used
            LogError("Failed to load the XML file containing glyph bounding boxes");
            return false;
        }
        pugi::xml_parse_result parseResult = doc.load_string(zipFile->ReadTextFile(filename).c_str());
        if (!parseResult) {
            // File not found, default bounding boxes will be used
            LogError("Failed to parse the XML file containing glyph bounding boxes");
            return false;
        }
    }
    // Other wise use the resource directory
    else {
        const std::string filename = Resources::GetPath() + "/" + fontName + ".xml";
        pugi::xml_parse_result parseResult = doc.load_file(filename.c_str());
        if (!parseResult) {
            // File not found, default bounding boxes will be used
            LogError("Failed to load font and glyph bounding boxes");
            return false;
        }
    }
    pugi::xml_node root = doc.first_child();
    if (!root.attribute("units-per-em")) {
        LogError("No units-per-em attribute in bounding box file");
        return false;
    }

    bool buildNameTable = (fontName == BRAVURA) ? true : false;
    bool isFallback = ((fontName == BRAVURA) || (fontName == LEIPZIG)) ? true : false;

    m_loadedFonts.insert(std::pair<std::string, LoadedFont>(fontName, Resources::LoadedFont(fontName, isFallback)));
    LoadedFont &font = m_loadedFonts.at(fontName);

    // For zip archive custom font also store the CSS
    if (zipFile) {
        font.SetCSSFont(zipFile->ReadTextFile(fontName + ".css"));
    }

    GlyphTable &glyphTable = font.GetGlyphTableForModification();

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

        std::string glyphFilename = fontName + "/" + c_attribute.value() + ".xml";
        // Store the XML in the glyph for fonts loaded from zip files
        if (zipFile) {
            glyph.SetXML(zipFile->ReadTextFile(glyphFilename));
        }
        // Otherwise only store the path
        else {
            glyph.SetPath(Resources::GetPath() + "/" + glyphFilename);
        }

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
        glyphTable[smuflCode] = glyph;
        if (buildNameTable) {
            m_glyphNameTable[n_attribute.value()] = smuflCode;
        }
    }

    if (isFallback && glyphTable.size() < SMUFL_COUNT) {
        LogError("Expected %d default SMuFL glyphs but could load only %d.", SMUFL_COUNT, glyphTable.size());
        return false;
    }

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
        LogInfo("Cannot load bounding boxes for text font '%s'", filename.c_str());
        return false;
    }
    pugi::xml_node root = doc.first_child();
    if (!root.attribute("units-per-em")) {
        LogWarning("No units-per-em attribute in bouding box file");
        return false;
    }
    const int unitsPerEm = root.attribute("units-per-em").as_int();
    pugi::xml_node current;
    if (!m_textFont.contains(style)) {
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
            if (currentTable.contains(code)) {
                LogDebug("Redefining %d with %s", code, fontName.c_str());
            }
            currentTable[code] = glyph;
        }
    }
    return true;
}

std::string Resources::LoadedFont::GetCSSFont(const std::string &path) const
{
    if (!m_css.empty()) {
        return m_css;
    }
    else {
        const std::string cssFontPath = StringFormat("%s/%s.css", path.c_str(), m_name.c_str());
        std::ifstream fstream(cssFontPath);
        std::stringstream sstream;
        sstream << fstream.rdbuf();
        return sstream.str();
    }
}

} // namespace vrv
