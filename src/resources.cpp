/////////////////////////////////////////////////////////////////////////////
// Name:        resources.cpp
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "resources.h"

//----------------------------------------------------------------------------

#include <cmath>

//----------------------------------------------------------------------------

#include "devicecontextbase.h"
#include "smufl.h"
#include "vrv.h"
#include "vrvdef.h"

#define BRAVURA "Bravura"
#define LEIPZIG "Leipzig"
#define TINOS "Tinos"

namespace vrv {

//----------------------------------------------------------------------------
// Static members with some default values
//----------------------------------------------------------------------------

thread_local std::string Resources::s_defaultPath = VRV_RESOURCE_DIR;

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

namespace {

    /** The SMuFL names of the glyphs supported by Verovio, by name and by code */
    struct GlyphNames {
        Resources::GlyphNameTable m_codes;
        std::unordered_map<char32_t, std::string> m_names;
    };

    const GlyphNames &GetGlyphNames()
    {
        static const GlyphNames glyphNames = [] {
            static constexpr std::pair<char32_t, const char *> entries[] = {
#include "smufl_names.inc"
            };
            GlyphNames names;
            for (const std::pair<char32_t, const char *> &entry : entries) {
                names.m_codes.emplace(entry.second, entry.first);
                names.m_names.emplace(entry.first, entry.second);
            }
            return names;
        }();
        return glyphNames;
    }

} // namespace

Resources::Resources()
{
    m_path = s_defaultPath;
    m_textFontName = TINOS;
}

bool Resources::Ok() const
{
    return m_fontStore.HasFace(FontStore::Kind::Music, LEIPZIG) && m_fontStore.HasFace(FontStore::Kind::Music, BRAVURA)
        && m_fontStore.HasFace(FontStore::Kind::Text, TINOS);
}

bool Resources::InitFonts()
{
    m_cachedGlyph.reset();
    m_runtimeGlyphs.clear();

    const std::string fontPath = m_path + "/fonts/";
    // Leipzig is the default music font and Bravura the final fallback, with the most complete SMuFL coverage
    for (const std::string family : { LEIPZIG, BRAVURA }) {
        const std::vector<unsigned char> font = FontStore::ReadFile(fontPath + family + ".woff2");
        const std::vector<unsigned char> metadata = FontStore::ReadFile(fontPath + family + "_metadata.json");
        if (font.empty() || metadata.empty()
            || (m_fontStore.RegisterMusicFont(font.data(), font.size(), std::string(metadata.begin(), metadata.end()))
                != family)) {
            LogError("%s runtime font could not be loaded.", family.c_str());
            return false;
        }
        m_musicFontSubsets[family] = FontStore::ReadFile(fontPath + family + "_subset.woff2");
    }
    for (const std::string style : { "Regular", "Italic", "Bold", "BoldItalic" }) {
        const std::vector<unsigned char> font = FontStore::ReadFile(fontPath + TINOS + "-" + style + ".woff2");
        if (font.empty() || (m_fontStore.RegisterTextFont(font.data(), font.size()) != TINOS)) {
            LogError("%s %s runtime font could not be loaded.", TINOS, style.c_str());
            return false;
        }
    }
    m_fontStore.PinBundledData();

    m_currentFontName = LEIPZIG;
    m_fallbackFontName = BRAVURA;
    m_textFontName = TINOS;

    return true;
}

bool Resources::IsFontLoaded(const std::string &fontName) const
{
    return m_fontStore.HasFace(FontStore::Kind::Music, fontName);
}

bool Resources::SetFallbackFont(const std::string &fontName)
{
    m_cachedGlyph.reset();

    if (!this->IsFontLoaded(fontName)) {
        LogError("Music font '%s' is not registered.", fontName.c_str());
        return false;
    }
    m_fallbackFontName = fontName;
    return true;
}

bool Resources::SetTextFont(const std::string &fontName)
{
    if (!m_fontStore.HasFace(FontStore::Kind::Text, fontName)) {
        LogError("Text font '%s' is not registered.", fontName.c_str());
        return false;
    }
    m_textFontName = fontName;
    return true;
}

bool Resources::SetCurrentFont(const std::string &fontName)
{
    m_cachedGlyph.reset();

    if (!this->IsFontLoaded(fontName)) return false;
    m_currentFontName = fontName;
    return true;
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
    // Look for the glyph in the font, the fallback font, and Bravura, which has the most complete coverage
    for (const std::string &family : { fontName, m_fallbackFontName, std::string(BRAVURA) }) {
        const std::optional<FontStore::GlyphMetrics> metrics
            = m_fontStore.GetGlyphMetrics(FontStore::Kind::Music, family, smuflCode);
        if (!metrics) continue;
        Glyph *glyph = const_cast<Glyph *>(
            this->GetRuntimeGlyph(metrics->m_face, metrics->m_glyphId, StringFormat("%04X", smuflCode)));
        if (!glyph) return NULL;
        const std::unordered_map<char32_t, std::string> &glyphNames = GetGlyphNames().m_names;
        if (const auto name = glyphNames.find(smuflCode); name != glyphNames.end()) {
            for (const FontStore::GlyphAnchor &anchor : m_fontStore.GetMusicGlyphAnchors(family, name->second)) {
                glyph->SetAnchor(anchor.m_name, anchor.m_x, anchor.m_y);
            }
        }
        return glyph;
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
    const GlyphNameTable &glyphCodes = GetGlyphNames().m_codes;
    if (const auto code = glyphCodes.find(smuflName); code != glyphCodes.end()) {
        return code->second;
    }
    return 0;
}

bool Resources::IsSmuflFallbackNeeded(const std::u32string &text) const
{
    for (char32_t c : text) {
        if (!this->FontHasGlyphAvailable(m_currentFontName, c)) return true;
    }
    return false;
}

bool Resources::FontHasGlyphAvailable(const std::string &fontName, char32_t smuflCode) const
{
    return m_fontStore.GetGlyphMetrics(FontStore::Kind::Music, fontName, smuflCode).has_value();
}

std::optional<FontStore::FontFile> Resources::GetMusicFontForEmbedding(
    const std::string &fontName, std::string &bundledFile) const
{
    bundledFile.clear();
    const std::map<std::string, std::vector<unsigned char>>::const_iterator subset = m_musicFontSubsets.find(fontName);
    if ((subset != m_musicFontSubsets.end()) && !subset->second.empty()) {
        bundledFile = fontName + "_subset.woff2";
        return FontStore::FontFile{ FontStore::Weight::Normal, FontStore::Style::Normal, "woff2", "font/woff2",
            subset->second };
    }
    // Other fonts are embedded entirely
    const std::vector<FontStore::FontFile> files = m_fontStore.GetFontFiles(FontStore::Kind::Music, fontName);
    if (files.empty()) return std::nullopt;
    return files.front();
}

std::string Resources::GetTextFontMetricEquivalent() const
{
    return (m_textFontName == TINOS) ? "Times" : "";
}

std::optional<FontStore::ShapedRun> Resources::ShapeText(const FontInfo &font, const std::u32string &text) const
{
    const std::string family = font.GetFaceName().empty() ? m_textFontName : font.GetFaceName();
    const FaceStyle faceStyle = Resources::GetFaceStyle(font);
    std::optional<FontStore::ShapedRun> run
        = m_fontStore.ShapeText(family, text, faceStyle.first, faceStyle.second, m_currentFontName, m_fallbackFontName);
    if (!run && (family != TINOS)) {
        run = m_fontStore.ShapeText(
            TINOS, text, faceStyle.first, faceStyle.second, m_currentFontName, m_fallbackFontName);
    }
    return run;
}

int Resources::GetTextAdvance(const FontInfo &font, const FontStore::ShapedRun &run) const
{
    double advance = 0.0;
    for (const FontStore::GlyphPlacement &placement : run.m_glyphs) {
        advance += static_cast<double>(placement.m_advanceX) * font.GetPointSize() / placement.m_unitsPerEm;
    }
    return static_cast<int>(std::ceil(advance)) + run.GetClusterGapCount() * font.GetLetterSpacing();
}

const Glyph *Resources::GetTextGlyph(char32_t code, const FontInfo &font) const
{
    const std::string family = font.GetFaceName().empty() ? m_textFontName : font.GetFaceName();
    const FaceStyle faceStyle = Resources::GetFaceStyle(font);
    std::optional<FontStore::GlyphMetrics> metrics
        = m_fontStore.GetGlyphMetrics(FontStore::Kind::Text, family, code, faceStyle.first, faceStyle.second);
    if (!metrics && (family != TINOS)) {
        metrics = m_fontStore.GetGlyphMetrics(FontStore::Kind::Text, TINOS, code, faceStyle.first, faceStyle.second);
    }
    if (!metrics) return NULL;
    return this->GetRuntimeGlyph(metrics->m_face, metrics->m_glyphId);
}

const Glyph *Resources::GetRuntimeGlyph(FontStore::FaceIdentity face, int glyphId, const std::string &code) const
{
    // The glyphs are cached with their code, since the same glyph can be used as SMuFL glyph and in shaped text
    std::map<std::pair<int, std::string>, Glyph> &glyphs = m_runtimeGlyphs[face.m_value];
    const std::pair<int, std::string> key(glyphId, code);
    if (const auto existing = glyphs.find(key); existing != glyphs.end()) return &existing->second;

    const std::optional<FontStore::GlyphMetrics> metrics = m_fontStore.GetGlyphMetrics(face, glyphId);
    if (!metrics) return NULL;
    Glyph glyph(metrics->m_unitsPerEm);
    // SMuFL glyphs are identified by their code, glyphs of shaped text by their face and glyph ID
    glyph.SetCodeStr(code.empty() ? StringFormat("text-%llX-%d", (unsigned long long)face.m_value, glyphId) : code);
    glyph.SetHorizAdvX(metrics->m_advanceX);
    glyph.SetBoundingBox(
        metrics->m_xBearing, metrics->m_yBearing + metrics->m_height, metrics->m_width, -metrics->m_height);
    glyph.SetFace(face.m_value, glyphId);
    return &glyphs.emplace(key, std::move(glyph)).first->second;
}

Resources::FaceStyle Resources::GetFaceStyle(const FontInfo &font)
{
    const FontStore::Weight weight
        = (font.GetWeight() == FONTWEIGHT_bold) ? FontStore::Weight::Bold : FontStore::Weight::Normal;
    const FontStore::Style style = ((font.GetStyle() == FONTSTYLE_italic) || (font.GetStyle() == FONTSTYLE_oblique))
        ? FontStore::Style::Italic
        : FontStore::Style::Normal;
    return { weight, style };
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

} // namespace vrv
