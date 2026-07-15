/////////////////////////////////////////////////////////////////////////////
// Name:        resources.cpp
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "resources.h"

//----------------------------------------------------------------------------

#include <array>
#include <charconv>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

//----------------------------------------------------------------------------

#include "devicecontextbase.h"
#include "smufl.h"
#include "vrv.h"
#include "vrvdef.h"

//----------------------------------------------------------------------------

#include "pugixml.hpp"

#define BRAVURA "Bravura"
#define LEIPZIG "Leipzig"
#define TINOS "Tinos"

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
    m_textFontName = TINOS;
}

bool Resources::Ok() const
{
    return m_fontStore.HasFace(FontStore::Kind::Music, BRAVURA) && m_fontStore.HasFace(FontStore::Kind::Text, TINOS);
}

namespace {

    std::vector<unsigned char> ReadFontFile(const std::string &filename)
    {
        std::ifstream input(filename, std::ios::binary | std::ios::ate);
        if (!input) return {};
        const std::streamsize size = input.tellg();
        if ((size <= 0) || (size > static_cast<std::streamsize>(32U * 1024U * 1024U))) return {};
        input.seekg(0);
        std::vector<unsigned char> data(static_cast<size_t>(size));
        if (!input.read(reinterpret_cast<char *>(data.data()), size)) return {};
        return data;
    }

    std::string ReadTextFile(const std::string &filename)
    {
        std::ifstream input(filename, std::ios::binary);
        return input ? std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>())
                     : std::string();
    }

    FontStore::Weight ToRuntimeWeight(data_FONTWEIGHT weight)
    {
        return (weight == FONTWEIGHT_bold) ? FontStore::Weight::Bold : FontStore::Weight::Normal;
    }

    FontStore::Style ToRuntimeStyle(data_FONTSTYLE style)
    {
        return ((style == FONTSTYLE_italic) || (style == FONTSTYLE_oblique)) ? FontStore::Style::Italic
                                                                             : FontStore::Style::Normal;
    }

    const Resources::GlyphNameTable &GetBundledGlyphNameTable()
    {
        static const Resources::GlyphNameTable table = [] {
            Resources::GlyphNameTable names;
            static constexpr std::pair<char32_t, const char *> entries[] = {
#include "smufl_names.inc"
            };
            names.reserve(std::size(entries));
            for (const auto &[code, name] : entries) names.emplace(name, code);
            return names;
        }();
        return table;
    }

    const std::unordered_map<char32_t, std::string> &GetBundledGlyphCodeNameTable()
    {
        static const std::unordered_map<char32_t, std::string> table = [] {
            std::unordered_map<char32_t, std::string> names;
            static constexpr std::pair<char32_t, const char *> entries[] = {
#include "smufl_names.inc"
            };
            names.reserve(std::size(entries));
            for (const auto &[code, name] : entries) names.emplace(code, name);
            return names;
        }();
        return table;
    }

    std::string RuntimeTextGlyphCode(uint64_t face, uint32_t glyphId)
    {
        std::array<char, 48> buffer{};
        char *output = buffer.data();
        std::memcpy(output, "text-", 5);
        output += 5;
        const auto faceResult = std::to_chars(output, buffer.data() + buffer.size(), face, 16);
        for (char *character = output; character != faceResult.ptr; ++character) {
            *character = static_cast<char>(std::toupper(static_cast<unsigned char>(*character)));
        }
        output = faceResult.ptr;
        *output++ = '-';
        output = std::to_chars(output, buffer.data() + buffer.size(), glyphId).ptr;
        return { buffer.data(), output };
    }

} // namespace

bool Resources::InitFonts()
{
    m_cachedGlyph.reset();
    m_loadedFonts.clear();
    m_runtimeGlyphs.clear();
    m_glyphNameTable.clear();
    m_glyphCodeNameTable.clear();

    const std::string fontPath = m_path + "/fonts/";
    const std::vector<unsigned char> bravura = ReadFontFile(fontPath + "Bravura.woff2");
    const std::string metadata = ReadTextFile(fontPath + "Bravura_metadata.json");
    if (bravura.empty() || metadata.empty()
        || (m_fontStore.RegisterMusicFont(bravura.data(), bravura.size(), metadata) != BRAVURA)) {
        LogError("Bravura runtime font could not be loaded.");
        return false;
    }

    static const std::array<const char *, 4> tinosFiles
        = { "Tinos-Regular.woff2", "Tinos-Italic.woff2", "Tinos-Bold.woff2", "Tinos-BoldItalic.woff2" };
    for (const char *filename : tinosFiles) {
        const std::vector<unsigned char> face = ReadFontFile(fontPath + filename);
        if (face.empty() || (m_fontStore.RegisterTextFont(face.data(), face.size()) != TINOS)) {
            LogError("Tinos runtime font '%s' could not be loaded.", filename);
            return false;
        }
    }
    m_fontStore.PinBundledData();

    m_defaultFontName = BRAVURA;
    m_currentFontName = m_defaultFontName;
    m_fallbackFontName = BRAVURA;
    m_textFontName = TINOS;

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

    m_defaultFontName = IsFontLoaded(fontName) ? fontName : BRAVURA;
    m_currentFontName = m_defaultFontName;

    return true;
}

bool Resources::IsFontLoaded(const std::string &fontName) const
{
    return m_loadedFonts.contains(fontName) || m_fontStore.HasFace(FontStore::Kind::Music, fontName);
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

    if (IsFontLoaded(fontName) || m_fontStore.HasFace(FontStore::Kind::Music, fontName)) {
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

    const Glyph *glyph = this->GetGlyph(smuflCode, m_currentFontName);
    if (glyph) m_cachedGlyph = std::make_pair(smuflCode, glyph);
    return glyph;
}

const Glyph *Resources::GetGlyph(char32_t smuflCode, const std::string &fontName) const
{
    const auto getLegacyGlyph = [this, smuflCode](const std::string &family) -> const Glyph * {
        const auto font = m_loadedFonts.find(family);
        if (font == m_loadedFonts.end()) return nullptr;
        const GlyphTable &glyphs = font->second.GetGlyphTable();
        const auto glyph = glyphs.find(smuflCode);
        return (glyph == glyphs.end()) ? nullptr : &glyph->second;
    };

    std::string resolvedFamily = fontName;
    std::optional<FontStore::GlyphMetrics> metrics
        = m_fontStore.GetGlyphMetrics(FontStore::Kind::Music, fontName, smuflCode);
    if (!metrics) {
        if (const Glyph *legacy = getLegacyGlyph(fontName)) return legacy;
    }
    if (!metrics && (m_fallbackFontName != fontName)) {
        resolvedFamily = m_fallbackFontName;
        metrics = m_fontStore.GetGlyphMetrics(FontStore::Kind::Music, m_fallbackFontName, smuflCode);
        if (!metrics) {
            if (const Glyph *legacy = getLegacyGlyph(m_fallbackFontName)) return legacy;
        }
    }
    if (!metrics && (m_fallbackFontName != BRAVURA)) {
        resolvedFamily = BRAVURA;
        metrics = m_fontStore.GetGlyphMetrics(FontStore::Kind::Music, BRAVURA, smuflCode);
    }
    if (!metrics) return nullptr;
    Glyph *glyph
        = const_cast<Glyph *>(this->GetRuntimeGlyph(metrics->face, metrics->glyphId, StringFormat("%04X", smuflCode)));
    const std::string *glyphName = nullptr;
    if (const auto custom = m_glyphCodeNameTable.find(smuflCode); custom != m_glyphCodeNameTable.end()) {
        glyphName = &custom->second;
    }
    else {
        const auto &bundledNames = GetBundledGlyphCodeNameTable();
        if (const auto bundled = bundledNames.find(smuflCode); bundled != bundledNames.end())
            glyphName = &bundled->second;
    }
    if (glyph && glyphName) {
        for (const FontStore::GlyphAnchor &anchor : m_fontStore.GetMusicGlyphAnchors(resolvedFamily, *glyphName)) {
            glyph->SetAnchor(anchor.name, anchor.x, anchor.y);
        }
    }
    return glyph;
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
    const auto &bundledNames = GetBundledGlyphNameTable();
    if (const auto glyphNameIter = bundledNames.find(smuflName); glyphNameIter != bundledNames.end()) {
        return glyphNameIter->second;
    }
    return 0;
}

bool Resources::IsSmuflFallbackNeeded(const std::u32string &text) const
{
    for (char32_t c : text) {
        if (!m_fontStore.GetGlyphMetrics(FontStore::Kind::Music, m_currentFontName, c)) return true;
    }
    return false;
}

bool Resources::IsCurrentFontFallback() const
{
    return (m_currentFontName == m_fallbackFontName);
}

bool Resources::FontHasGlyphAvailable(const std::string &fontName, char32_t smuflCode) const
{
    return m_fontStore.GetGlyphMetrics(FontStore::Kind::Music, fontName, smuflCode).has_value();
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
    // For example, OneGlyph/OneGlyph.xml
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
    if (!m_fontStore.HasFace(
            FontStore::Kind::Text, m_textFontName, ToRuntimeWeight(fontWeight), ToRuntimeStyle(fontStyle))
        && !m_textFont.contains(m_currentStyle)) {
        LogWarning("Text font for style (%d, %d) is not loaded. Use default", fontWeight, fontStyle);
        m_currentStyle = k_defaultStyle;
    }
}

std::optional<FontStore::ShapedRun> Resources::ShapeText(const FontInfo &font, const std::u32string &text) const
{
    const std::string family = font.GetFaceName().empty() ? m_textFontName : font.GetFaceName();
    const FontStore::Weight weight = ToRuntimeWeight(font.GetWeight());
    const FontStore::Style style = ToRuntimeStyle(font.GetStyle());
    std::optional<FontStore::ShapedRun> run = m_fontStore.ShapeText(family, text, weight, style);
    if (!run && (family != TINOS)) run = m_fontStore.ShapeText(TINOS, text, weight, style);
    return run;
}

int Resources::GetTextAdvance(const FontInfo &font, const FontStore::ShapedRun &run) const
{
    double advance = 0.0;
    int clusterGaps = 0;
    uint32_t previousCluster = 0;
    bool first = true;
    for (const FontStore::GlyphPlacement &placement : run.glyphs) {
        if (!first && (placement.cluster != previousCluster)) ++clusterGaps;
        advance += static_cast<double>(placement.advanceX) * font.GetPointSize() / placement.unitsPerEm;
        previousCluster = placement.cluster;
        first = false;
    }
    return static_cast<int>(std::ceil(advance)) + clusterGaps * font.GetLetterSpacing();
}

const Glyph *Resources::GetTextGlyph(char32_t code) const
{
    const FontStore::Weight weight = ToRuntimeWeight(m_currentStyle.first);
    const FontStore::Style runtimeStyle = ToRuntimeStyle(m_currentStyle.second);
    std::optional<FontStore::GlyphMetrics> metrics
        = m_fontStore.GetGlyphMetrics(FontStore::Kind::Text, m_textFontName, code, weight, runtimeStyle);
    if (!metrics && (m_textFontName != TINOS)) {
        metrics = m_fontStore.GetGlyphMetrics(FontStore::Kind::Text, TINOS, code, weight, runtimeStyle);
    }
    if (metrics) return this->GetRuntimeGlyph(metrics->face, metrics->glyphId, StringFormat("%04X", code));

    const StyleAttributes legacyStyle = m_textFont.contains(m_currentStyle) ? m_currentStyle : k_defaultStyle;
    if (!m_textFont.contains(legacyStyle)) return NULL;

    const GlyphTable &currentTable = m_textFont.at(legacyStyle);
    if (!currentTable.contains(code)) {
        return NULL;
    }

    return &currentTable.at(code);
}

const Glyph *Resources::GetRuntimeGlyph(FontStore::FaceIdentity face, uint32_t glyphId, const std::string &code) const
{
    auto &glyphs = m_runtimeGlyphs[face.value];
    const auto existing = glyphs.find(glyphId);
    if (existing != glyphs.end()) return &existing->second;

    const std::optional<FontStore::GlyphMetrics> metrics = m_fontStore.GetGlyphMetrics(face, glyphId);
    if (!metrics) return nullptr;
    Glyph glyph(metrics->unitsPerEm);
    if (code.empty())
        glyph.SetCodeStr(RuntimeTextGlyphCode(face.value, glyphId));
    else
        glyph.SetCodeStr(code.starts_with("text-") ? code : StringFormat("music-%llX-%s", face.value, code.c_str()));
    glyph.SetHorizAdvX(metrics->advanceX);
    glyph.SetBoundingBox(metrics->xBearing, metrics->yBearing + metrics->height, metrics->width, -metrics->height);
    glyph.SetRuntimeGlyph(face.value, glyphId);
    return &glyphs.emplace(glyphId, std::move(glyph)).first->second;
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
            m_glyphCodeNameTable[smuflCode] = n_attribute.value();
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
        LogWarning("No units-per-em attribute in bounding box file");
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
