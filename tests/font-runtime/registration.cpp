/////////////////////////////////////////////////////////////////////////////
// Name:        registration.cpp
// Author:      Simon Waloschek
// Created:     2026
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

// Tests of the runtime font registration and rendering

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

//----------------------------------------------------------------------------

#include "toolkit.h"
#include "vrv.h"

namespace {

// Font selection is verified through the glyphs referenced in the SVG, which requires text rendered as paths
void RenderTextAsPaths(vrv::Toolkit &toolkit)
{
    toolkit.SetOptions("{\"svgTextAsPaths\":true}");
}

bool Expect(bool condition, const std::string &message)
{
    if (condition) return true;
    std::cerr << message << '\n';
    return false;
}

std::optional<int> FindRectAttribute(const std::string &svg, const std::string &id, const std::string &attribute)
{
    const size_t group = svg.find("id=\"" + id + "\"");
    if (group == std::string::npos) return std::nullopt;
    const size_t rect = svg.find("<rect", group);
    if (rect == std::string::npos) return std::nullopt;
    const size_t rectEnd = svg.find('>', rect);
    const std::string needle = attribute + "=\"";
    const size_t value = svg.find(needle, rect);
    if ((value == std::string::npos) || (value > rectEnd)) return std::nullopt;
    const size_t begin = value + needle.size();
    const size_t end = svg.find('"', begin);
    if ((end == std::string::npos) || (end > rectEnd)) return std::nullopt;
    return std::stoi(svg.substr(begin, end - begin));
}

void WriteU16(std::vector<unsigned char> &bytes, size_t offset, uint16_t value)
{
    bytes[offset] = static_cast<unsigned char>(value >> 8);
    bytes[offset + 1] = static_cast<unsigned char>(value);
}

void WriteU32(std::vector<unsigned char> &bytes, size_t offset, uint32_t value)
{
    bytes[offset] = static_cast<unsigned char>(value >> 24);
    bytes[offset + 1] = static_cast<unsigned char>(value >> 16);
    bytes[offset + 2] = static_cast<unsigned char>(value >> 8);
    bytes[offset + 3] = static_cast<unsigned char>(value);
}

} // namespace

std::vector<unsigned char> ReadFile(const std::string &path)
{
    std::ifstream input(path, std::ios::binary);
    return { std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>() };
}

std::string JsonEscape(const std::string &value)
{
    std::string escaped;
    for (const char character : value) {
        if ((character == '\\') || (character == '"')) escaped.push_back('\\');
        escaped.push_back(character);
    }
    return escaped;
}

void ReplaceAll(std::string &value, const std::string &from, const std::string &to)
{
    size_t offset = 0;
    while ((offset = value.find(from, offset)) != std::string::npos) {
        value.replace(offset, from.size(), to);
        offset += to.size();
    }
}

size_t CountOccurrences(const std::string &value, const std::string &needle)
{
    size_t count = 0;
    size_t offset = 0;
    while ((offset = value.find(needle, offset)) != std::string::npos) {
        ++count;
        offset += needle.size();
    }
    return count;
}

// Remove the per-rendering postfix from the glyph ids and references
std::string NormalizeFontReference(std::string value)
{
    for (const std::string attribute : { "id=\"", "href=\"#" }) {
        size_t offset = 0;
        while ((offset = value.find(attribute, offset)) != std::string::npos) {
            offset += attribute.size();
            const size_t end = value.find('"', offset);
            if (end == std::string::npos) break;
            const size_t suffix = value.rfind('-', end);
            if ((suffix != std::string::npos) && (suffix > offset)) value.erase(suffix, end - suffix);
        }
    }
    return value;
}

// Check if the SVG contains the outline of the glyph, which identifies the font it comes from
bool HasGlyphOutline(
    const std::string &svg, const vrv::FontStore &store, const std::optional<vrv::FontStore::GlyphMetrics> &glyph)
{
    if (!glyph) return false;
    const std::optional<std::string> outline = store.GetGlyphOutline(glyph->m_face, glyph->m_glyphId);
    return outline && (svg.find("d=\"" + *outline + "\"") != std::string::npos);
}

std::string ExtractSvgGeometry(const std::string &svg)
{
    std::string geometry;
    size_t offset = 0;
    while (true) {
        const size_t path = svg.find("<path ", offset);
        const size_t use = svg.find("<use ", offset);
        const size_t begin = std::min(path, use);
        if (begin == std::string::npos) break;
        const size_t end = svg.find("/>", begin);
        if (end == std::string::npos) break;
        geometry += NormalizeFontReference(svg.substr(begin, end - begin + 2));
        geometry.push_back('\n');
        offset = end + 2;
    }
    return geometry;
}

int main(int argc, char **argv)
{
    if (argc != 17) return 2;

    vrv::Toolkit toolkit(false);
    RenderTextAsPaths(toolkit);
    bool ok = true;
    const std::string textFamily = toolkit.RegisterTextFontFile(argv[1]);
    ok &= Expect(textFamily == "Tinos", "Tinos family was not registered (returned '" + textFamily + "')");
    const std::string duplicateFamily = toolkit.RegisterTextFontFile(argv[1]);
    ok &= Expect(
        duplicateFamily == "Tinos", "duplicate registration was not idempotent (returned '" + duplicateFamily + "')");
    ok &= Expect(toolkit.RegisterMusicFontFile(argv[2], argv[3]) == "Bravura", "Bravura was not registered");
    ok &= Expect(toolkit.RegisterTextFontFile("missing-font.ttf").empty(), "missing file was accepted");

    vrv::FontStore localizedStyleStore;
    const std::vector<unsigned char> localizedRegular = ReadFile(argv[11]);
    const std::vector<unsigned char> localizedBold = ReadFile(argv[15]);
    ok &= Expect(localizedStyleStore.RegisterTextFont(localizedRegular.data(), localizedRegular.size())
            == "Verovio Test Ligature",
        "localized style regular-face setup failed");
    const std::optional<vrv::FontStore::ShapedRun> localizedRegularRun
        = localizedStyleStore.ShapeText("Verovio Test Ligature", U"ffi", vrv::FontStore::Weight::Normal);
    const std::optional<vrv::FontStore::ShapedRun> localizedSyntheticItalicRun = localizedStyleStore.ShapeText(
        "Verovio Test Ligature", U"ffi", vrv::FontStore::Weight::Normal, vrv::FontStore::Style::Italic);
    ok &= Expect(localizedRegularRun && localizedSyntheticItalicRun && !localizedRegularRun->m_glyphs.empty()
            && !localizedSyntheticItalicRun->m_glyphs.empty()
            && (localizedRegularRun->m_glyphs.front().m_face != localizedSyntheticItalicRun->m_glyphs.front().m_face),
        "missing italic face did not synthesize a distinct style within the requested family");
    if (localizedRegularRun && localizedSyntheticItalicRun && !localizedRegularRun->m_glyphs.empty()
        && !localizedSyntheticItalicRun->m_glyphs.empty()) {
        const std::optional<std::string> regularOutline = localizedStyleStore.GetGlyphOutline(
            localizedRegularRun->m_glyphs.front().m_face, localizedRegularRun->m_glyphs.front().m_glyphId);
        const std::optional<std::string> syntheticItalicOutline
            = localizedStyleStore.GetGlyphOutline(localizedSyntheticItalicRun->m_glyphs.front().m_face,
                localizedSyntheticItalicRun->m_glyphs.front().m_glyphId);
        ok &= Expect(regularOutline && syntheticItalicOutline && (*regularOutline != *syntheticItalicOutline),
            "synthetic italic face did not slant the emitted glyph outline");
    }
    ok &= Expect(
        localizedStyleStore.RegisterTextFont(localizedBold.data(), localizedBold.size()) == "Verovio Test Ligature",
        "localized bold subfamily was not classified from OpenType style metadata");
    const std::optional<vrv::FontStore::ShapedRun> localizedBoldRun
        = localizedStyleStore.ShapeText("Verovio Test Ligature", U"ffi", vrv::FontStore::Weight::Bold);
    ok &= Expect(localizedRegularRun && localizedBoldRun && !localizedRegularRun->m_glyphs.empty()
            && !localizedBoldRun->m_glyphs.empty()
            && (localizedRegularRun->m_glyphs.front().m_face != localizedBoldRun->m_glyphs.front().m_face),
        "localized regular and bold faces did not resolve to distinct font identities");
    vrv::Toolkit localizedStyleRendering(false);
    RenderTextAsPaths(localizedStyleRendering);
    ok &= Expect(localizedStyleRendering.SetResourcePath(argv[9])
            && (localizedStyleRendering.RegisterTextFontFile(argv[11]) == "Verovio Test Ligature")
            && (localizedStyleRendering.RegisterTextFontFile(argv[15]) == "Verovio Test Ligature"),
        "localized style rendering setup failed");
    const std::string localizedStyleMei = R"mei(<?xml version="1.0" encoding="UTF-8"?>
<mei xmlns="http://www.music-encoding.org/ns/mei" meiversion="5.1"><music><body><mdiv><score>
<scoreDef><staffGrp><staffDef n="1" lines="5" clef.shape="G" clef.line="2"/></staffGrp></scoreDef>
<section><measure n="1"><staff n="1"><layer n="1"><note xml:id="style-note" pname="c" oct="4" dur="1"/>
</layer></staff><dir startid="#style-note"><rend fontname="Verovio Test Ligature" fontweight="bold" fontstyle="normal">ffi</rend></dir>
<dir startid="#style-note"><rend fontname="Verovio Test Ligature" fontstyle="italic">ffi</rend></dir>
</measure></section></score></mdiv></body></music></mei>)mei";
    ok &= Expect(localizedStyleRendering.LoadData(localizedStyleMei), "localized bold render MEI could not be loaded");
    const std::string localizedStyleSvg = localizedStyleRendering.RenderToSVG(1);
    std::ostringstream localizedBoldPrefix;
    if (localizedBoldRun && !localizedBoldRun->m_glyphs.empty()) {
        localizedBoldPrefix << "text-" << std::uppercase << std::hex
                            << localizedBoldRun->m_glyphs.front().m_face.m_value << "-";
    }
    std::ostringstream localizedSyntheticItalicPrefix;
    if (localizedSyntheticItalicRun && !localizedSyntheticItalicRun->m_glyphs.empty()) {
        localizedSyntheticItalicPrefix << "text-" << std::uppercase << std::hex
                                       << localizedSyntheticItalicRun->m_glyphs.front().m_face.m_value << "-";
    }
    ok &= Expect(!localizedBoldPrefix.str().empty()
            && (localizedStyleSvg.find(localizedBoldPrefix.str()) != std::string::npos)
            && (localizedStyleSvg.find("font-weight=\"bold\"") != std::string::npos),
        "MEI rend with bold weight did not render with the localized bold face");
    ok &= Expect(!localizedSyntheticItalicPrefix.str().empty()
            && (localizedStyleSvg.find(localizedSyntheticItalicPrefix.str()) != std::string::npos)
            && (localizedStyleSvg.find("font-style=\"italic\"") != std::string::npos),
        "MEI rend with an unavailable italic face did not stay within the requested family");

    // Without CSS, the default text styles (here the italic of dir) have to be applied to the paths
    vrv::Toolkit defaultStyleRendering(false);
    RenderTextAsPaths(defaultStyleRendering);
    ok &= Expect(defaultStyleRendering.SetResourcePath(argv[9])
            && (defaultStyleRendering.RegisterTextFontFile(argv[11]) == "Verovio Test Ligature"),
        "default style rendering setup failed");
    std::string defaultStyleMei = localizedStyleMei;
    ReplaceAll(defaultStyleMei, " fontweight=\"bold\"", "");
    ReplaceAll(defaultStyleMei, " fontstyle=\"normal\"", "");
    ReplaceAll(defaultStyleMei, " fontstyle=\"italic\"", "");
    ok &= Expect(defaultStyleRendering.LoadData(defaultStyleMei), "default style MEI could not be loaded");
    const std::string defaultStyleSvg = defaultStyleRendering.RenderToSVG(1);
    ok &= Expect(!localizedSyntheticItalicPrefix.str().empty()
            && (defaultStyleSvg.find(localizedSyntheticItalicPrefix.str()) != std::string::npos),
        "dir text rendered as paths did not use the italic default style");

    vrv::Toolkit scoreTextStyleRendering(false);
    RenderTextAsPaths(scoreTextStyleRendering);
    ok &= Expect(scoreTextStyleRendering.SetResourcePath(argv[9])
            && (scoreTextStyleRendering.RegisterTextFontFile(argv[11]) == "Verovio Test Ligature"),
        "score text-style rendering setup failed");
    const std::string scoreTextStyleMei = R"mei(<?xml version="1.0" encoding="UTF-8"?>
<mei xmlns="http://www.music-encoding.org/ns/mei" meiversion="6.0-dev"><music><body><mdiv><score>
<scoreDef text.fam="Verovio Test Ligature" text.style="normal"><staffGrp><staffDef n="1" lines="5" clef.shape="G"
clef.line="2"/></staffGrp></scoreDef><section><measure n="1"><staff n="1"><layer n="1">
<note xml:id="score-text-note" pname="c" oct="4" dur="1"/></layer></staff>
<harm startid="#score-text-note">ffi</harm></measure></section></score></mdiv></body></music></mei>)mei";
    ok &= Expect(scoreTextStyleRendering.LoadData(scoreTextStyleMei), "score text-style MEI could not be loaded");
    const std::string scoreTextStyleSvg = scoreTextStyleRendering.RenderToSVG(1);
    std::ostringstream localizedRegularPrefix;
    if (localizedRegularRun && !localizedRegularRun->m_glyphs.empty()) {
        localizedRegularPrefix << "text-" << std::uppercase << std::hex
                               << localizedRegularRun->m_glyphs.front().m_face.m_value << "-";
    }
    ok &= Expect(!localizedRegularPrefix.str().empty()
            && (scoreTextStyleSvg.find(localizedRegularPrefix.str()) != std::string::npos),
        "scoreDef text.fam did not select the registered face for harmony text");

    vrv::Toolkit scoreTextDirRendering(false);
    RenderTextAsPaths(scoreTextDirRendering);
    ok &= Expect(scoreTextDirRendering.SetResourcePath(argv[9])
            && (scoreTextDirRendering.RegisterTextFontFile(argv[11]) == "Verovio Test Ligature"),
        "score text-style direction rendering setup failed");
    std::string scoreTextDirMei = scoreTextStyleMei;
    ReplaceAll(scoreTextDirMei, "<harm startid=\"#score-text-note\">ffi</harm>",
        "<dir startid=\"#score-text-note\">ffi</dir>");
    ok &= Expect(scoreTextDirRendering.LoadData(scoreTextDirMei), "score text-style direction MEI could not be loaded");
    const std::string scoreTextDirSvg = scoreTextDirRendering.RenderToSVG(1);
    ok &= Expect(!localizedRegularPrefix.str().empty()
            && (scoreTextDirSvg.find(localizedRegularPrefix.str()) != std::string::npos),
        "scoreDef text.fam did not select the registered face for direction text");

    vrv::Toolkit scoreTextHeaderRendering(false);
    RenderTextAsPaths(scoreTextHeaderRendering);
    ok &= Expect(scoreTextHeaderRendering.SetResourcePath(argv[9])
            && (scoreTextHeaderRendering.RegisterTextFontFile(argv[11]) == "Verovio Test Ligature"),
        "score text-style page-header rendering setup failed");
    std::string scoreTextHeaderMei = scoreTextStyleMei;
    ReplaceAll(scoreTextHeaderMei, "<staffGrp>", "<pgHead func=\"first\"><rend>ffi</rend></pgHead><staffGrp>");
    ReplaceAll(scoreTextHeaderMei, "<harm startid=\"#score-text-note\">ffi</harm>", "");
    ok &= Expect(scoreTextHeaderRendering.LoadData(scoreTextHeaderMei), "score text-style page-header MEI failed");
    const std::string scoreTextHeaderSvg = scoreTextHeaderRendering.RenderToSVG(1);
    ok &= Expect(!localizedRegularPrefix.str().empty()
            && (scoreTextHeaderSvg.find(localizedRegularPrefix.str()) != std::string::npos),
        "scoreDef text.fam did not select the registered face for page-header text");

    vrv::Toolkit textEnclosureRendering(false);
    RenderTextAsPaths(textEnclosureRendering);
    ok &= Expect(textEnclosureRendering.SetResourcePath(argv[9]), "text-enclosure rendering setup failed");
    const std::string textEnclosureMei = R"mei(<?xml version="1.0" encoding="UTF-8"?>
<mei xmlns="http://www.music-encoding.org/ns/mei" meiversion="5.0"><music><body><mdiv><score>
<scoreDef text.style="normal"><staffGrp><staffDef n="1" lines="5"/></staffGrp></scoreDef>
<section><measure><staff n="1"><layer n="1">
<note xml:id="box-note-1" pname="c" oct="4" dur="4"/>
<note xml:id="box-note-2" pname="d" oct="4" dur="4"/>
</layer></staff>
<dir xml:id="box-intro" startid="#box-note-1"><rend rend="box">Intro</rend></dir>
<dir xml:id="box-strophen" startid="#box-note-2"><rend rend="box">Strophen</rend></dir>
</measure></section></score></mdiv></body></music></mei>)mei";
    ok &= Expect(textEnclosureRendering.LoadData(textEnclosureMei), "text-enclosure MEI could not be loaded");
    const std::string textEnclosureSvg = textEnclosureRendering.RenderToSVG(1);
    const std::optional<int> introBoxHeight = FindRectAttribute(textEnclosureSvg, "box-intro", "height");
    const std::optional<int> strophenBoxHeight = FindRectAttribute(textEnclosureSvg, "box-strophen", "height");
    ok &= Expect(introBoxHeight && strophenBoxHeight && (std::abs(*introBoxHeight - *strophenBoxHeight) <= 4),
        "boxed text without a descender did not reserve the font descender height (Intro "
            + std::to_string(introBoxHeight.value_or(-1)) + ", Strophen "
            + std::to_string(strophenBoxHeight.value_or(-1)) + ")");

    vrv::Toolkit inheritedRendRendering(false);
    RenderTextAsPaths(inheritedRendRendering);
    ok &= Expect(inheritedRendRendering.SetResourcePath(argv[9])
            && (inheritedRendRendering.RegisterTextFontFile(argv[11]) == "Verovio Test Ligature")
            && (inheritedRendRendering.RegisterTextFontFile(argv[15]) == "Verovio Test Ligature"),
        "inherited rend rendering setup failed");
    std::string inheritedRendMei = scoreTextStyleMei;
    ReplaceAll(inheritedRendMei, ">ffi</harm>", ">ffi<rend fontweight=\"bold\">ffi</rend></harm>");
    ok &= Expect(inheritedRendRendering.LoadData(inheritedRendMei), "inherited rend MEI could not be loaded");
    const std::string inheritedRendSvg = inheritedRendRendering.RenderToSVG(1);
    ok &= Expect(
        !localizedBoldPrefix.str().empty() && (inheritedRendSvg.find(localizedBoldPrefix.str()) != std::string::npos),
        "nested rend style did not retain its inherited runtime family");

    vrv::Toolkit scoreLyricStyleRendering(false);
    RenderTextAsPaths(scoreLyricStyleRendering);
    ok &= Expect(scoreLyricStyleRendering.SetResourcePath(argv[9])
            && (scoreLyricStyleRendering.RegisterTextFontFile(argv[11]) == "Verovio Test Ligature"),
        "score lyric-style rendering setup failed");
    const std::string scoreLyricStyleMei = R"mei(<?xml version="1.0" encoding="UTF-8"?>
<mei xmlns="http://www.music-encoding.org/ns/mei" meiversion="6.0-dev"><music><body><mdiv><score>
<scoreDef lyric.fam="Verovio Test Ligature"><staffGrp><staffDef n="1" lines="5" clef.shape="G"
clef.line="2"/></staffGrp></scoreDef><section><measure n="1"><staff n="1"><layer n="1">
<note xml:id="score-lyric-note" pname="c" oct="4" dur="1"><verse n="1"><syl>ffi</syl></verse></note>
</layer></staff></measure></section></score></mdiv></body></music></mei>)mei";
    ok &= Expect(scoreLyricStyleRendering.LoadData(scoreLyricStyleMei), "score lyric-style MEI could not be loaded");
    const std::string scoreLyricStyleSvg = scoreLyricStyleRendering.RenderToSVG(1);
    ok &= Expect(!localizedRegularPrefix.str().empty()
            && (scoreLyricStyleSvg.find(localizedRegularPrefix.str()) != std::string::npos),
        "scoreDef lyric.fam did not select the registered face for lyric text");

    vrv::Toolkit rendFamilyRendering(false);
    RenderTextAsPaths(rendFamilyRendering);
    ok &= Expect(rendFamilyRendering.SetResourcePath(argv[9])
            && (rendFamilyRendering.RegisterTextFontFile(argv[11]) == "Verovio Test Ligature"),
        "rend font-family rendering setup failed");
    const std::string rendFamilyMei = R"mei(<?xml version="1.0" encoding="UTF-8"?>
<mei xmlns="http://www.music-encoding.org/ns/mei" meiversion="6.0-dev"><music><body><mdiv><score>
<scoreDef><staffGrp><staffDef n="1" lines="5" clef.shape="G" clef.line="2"/></staffGrp></scoreDef>
<section><measure n="1"><staff n="1"><layer n="1"><note xml:id="rend-family-note" pname="c"
oct="4" dur="1"/></layer></staff><dir startid="#rend-family-note"><rend
fontfam="Verovio Test Ligature" fontstyle="normal">ffi</rend></dir></measure></section></score></mdiv></body></music></mei>)mei";
    ok &= Expect(rendFamilyRendering.LoadData(rendFamilyMei), "rend font-family MEI could not be loaded");
    const std::string rendFamilySvg = rendFamilyRendering.RenderToSVG(1);
    ok &= Expect(!localizedRegularPrefix.str().empty()
            && (rendFamilySvg.find(localizedRegularPrefix.str()) != std::string::npos),
        "rend fontfam did not select the registered face");

    vrv::Toolkit verseFontRendering(false);
    RenderTextAsPaths(verseFontRendering);
    ok &= Expect(verseFontRendering.SetResourcePath(argv[9])
            && (verseFontRendering.RegisterTextFontFile(argv[11]) == "Verovio Test Ligature"),
        "verse font rendering setup failed");
    const std::string verseFontMei = R"mei(<?xml version="1.0" encoding="UTF-8"?>
<mei xmlns="http://www.music-encoding.org/ns/mei" meiversion="6.0-dev"><music><body><mdiv><score>
<scoreDef><staffGrp><staffDef n="1" lines="5" clef.shape="G" clef.line="2"/></staffGrp></scoreDef>
<section><measure n="1"><staff n="1"><layer n="1"><note xml:id="verse-font-note" pname="c"
oct="4" dur="1"><verse n="1" fontname="Verovio Test Ligature"><syl>ffi</syl></verse></note>
</layer></staff></measure></section></score></mdiv></body></music></mei>)mei";
    ok &= Expect(verseFontRendering.LoadData(verseFontMei), "verse font MEI could not be loaded");
    const std::string verseFontSvg = verseFontRendering.RenderToSVG(1);
    ok &= Expect(
        !localizedRegularPrefix.str().empty() && (verseFontSvg.find(localizedRegularPrefix.str()) != std::string::npos),
        "verse fontname was not inherited by its syllable");

    vrv::Toolkit syllableFamilyRendering(false);
    RenderTextAsPaths(syllableFamilyRendering);
    ok &= Expect(syllableFamilyRendering.SetResourcePath(argv[9])
            && (syllableFamilyRendering.RegisterTextFontFile(argv[11]) == "Verovio Test Ligature"),
        "syllable font-family rendering setup failed");
    std::string syllableFamilyMei = verseFontMei;
    ReplaceAll(syllableFamilyMei, " fontname=\"Verovio Test Ligature\"", "");
    ReplaceAll(syllableFamilyMei, "<syl>", "<syl fontfam=\"Verovio Test Ligature\">");
    ok &= Expect(syllableFamilyRendering.LoadData(syllableFamilyMei), "syllable font-family MEI could not be loaded");
    const std::string syllableFamilySvg = syllableFamilyRendering.RenderToSVG(1);
    ok &= Expect(!localizedRegularPrefix.str().empty()
            && (syllableFamilySvg.find(localizedRegularPrefix.str()) != std::string::npos),
        "syl fontfam did not select the registered face");

    const std::vector<unsigned char> woff = ReadFile(argv[4]);
    const std::vector<unsigned char> ttf = ReadFile(argv[1]);
    const std::vector<unsigned char> bravura = ReadFile(argv[2]);
    const std::vector<unsigned char> metadataBytes = ReadFile(argv[3]);
    const std::string metadata(metadataBytes.begin(), metadataBytes.end());
    vrv::FontStore aliasStore;
    ok &= Expect(aliasStore.RegisterTextFont(ttf.data(), ttf.size(), "QS") == "Tinos",
        "text font registration with an alias did not return the canonical family");
    ok &= Expect(aliasStore.HasFace(vrv::FontStore::Kind::Text, "QS"),
        "registered text font alias did not resolve to its family");
    ok &= Expect(!aliasStore.HasFace(vrv::FontStore::Kind::Text, "qs"),
        "text font aliases unexpectedly became case-insensitive");
    const uint64_t aliasGeneration = aliasStore.GetGeneration();
    const std::optional<vrv::FontStore::GlyphMetrics> aliasMetrics
        = aliasStore.GetGlyphMetrics(vrv::FontStore::Kind::Text, "QS", U'A');
    const std::optional<vrv::FontStore::GlyphMetrics> canonicalAliasMetrics
        = aliasStore.GetGlyphMetrics(vrv::FontStore::Kind::Text, "Tinos", U'A');
    ok &= Expect(aliasMetrics && canonicalAliasMetrics && (aliasMetrics->m_face == canonicalAliasMetrics->m_face)
            && (aliasMetrics->m_glyphId == canonicalAliasMetrics->m_glyphId),
        "text alias did not share canonical metrics and identity");
    ok &= Expect(aliasStore.RegisterTextFont(ttf.data(), ttf.size(), "QS") == "Tinos"
            && (aliasStore.GetGeneration() == aliasGeneration),
        "identical text alias registration was not idempotent");
    const vrv::FontStore::Counters aliasCounters = aliasStore.GetCounters();
    ok &= Expect(aliasStore.RegisterTextFont(ttf.data(), ttf.size(), "Short Tinos") == "Tinos"
            && (aliasStore.GetGeneration() == aliasGeneration + 1),
        "a second alias for an existing family was not registered");
    ok &= Expect(aliasStore.GetCounters().m_decodedFonts == aliasCounters.m_decodedFonts
            && aliasStore.GetCounters().m_extractedOutlines == aliasCounters.m_extractedOutlines,
        "adding an alias decoded the font or extracted outlines again");
    const std::optional<vrv::FontStore::ShapedRun> canonicalAliasShape = aliasStore.ShapeText("Tinos", U"AV");
    const std::optional<vrv::FontStore::ShapedRun> aliasShape = aliasStore.ShapeText("QS", U"AV");
    ok &= Expect(aliasShape == canonicalAliasShape && (aliasStore.GetCounters().m_shapedRuns == 1),
        "text alias duplicated canonical shaping work");
    if (aliasMetrics) {
        const std::optional<std::string> aliasOutline
            = aliasStore.GetGlyphOutline(vrv::FontStore::Kind::Text, "QS", aliasMetrics->m_glyphId);
        const std::optional<std::string> canonicalAliasOutline
            = aliasStore.GetGlyphOutline(vrv::FontStore::Kind::Text, "Tinos", aliasMetrics->m_glyphId);
        ok &= Expect(aliasOutline == canonicalAliasOutline && (aliasStore.GetCounters().m_extractedOutlines == 1),
            "text alias duplicated canonical outline extraction");
    }
    const uint64_t generationBeforeInvalidAlias = aliasStore.GetGeneration();
    ok &= Expect(aliasStore.RegisterTextFont(ttf.data(), ttf.size(), "bad=alias").empty()
            && aliasStore.RegisterTextFont(ttf.data(), ttf.size(), "   ").empty()
            && (aliasStore.GetGeneration() == generationBeforeInvalidAlias),
        "invalid aliases changed the font registry");
    vrv::FontStore musicStore;
    ok &= Expect(musicStore.RegisterMusicFont(bravura.data(), bravura.size(), metadata, "QS") == "Bravura",
        "FontStore Bravura registration failed");
    ok &= Expect(musicStore.GetCounters().m_extractedOutlines == 0, "Bravura registration eagerly extracted outlines");
    const std::optional<vrv::FontStore::GlyphMetrics> gClef
        = musicStore.GetGlyphMetrics(vrv::FontStore::Kind::Music, "Bravura", U'\uE050');
    ok &= Expect(gClef && (gClef->m_glyphId == 74) && (gClef->m_unitsPerEm == 1000) && (gClef->m_advanceX == 671)
            && (gClef->m_xBearing == 0) && (gClef->m_yBearing == 1098) && (gClef->m_width == 671)
            && (gClef->m_height == -1756),
        "Bravura gClef metrics differ from the trusted fixture");
    if (gClef) {
        const std::optional<std::string> outline = musicStore.GetGlyphOutline(gClef->m_face, gClef->m_glyphId);
        ok &= Expect(outline && !outline->empty(), "Bravura CFF gClef outline was unavailable");
        ok &= Expect(musicStore.GetCounters().m_extractedOutlines == 1, "Bravura outline was not extracted lazily");
    }
    ok &= Expect(musicStore.GetGlyphMetrics(vrv::FontStore::Kind::Music, "Bravura", U'\uE0A2').has_value(),
        "Bravura whole-note glyph metrics were unavailable");
    const std::vector<vrv::FontStore::GlyphAnchor> noteheadAnchors
        = musicStore.GetMusicGlyphAnchors("Bravura", "noteheadBlack");
    ok &= Expect(std::any_of(noteheadAnchors.begin(), noteheadAnchors.end(),
                     [](const vrv::FontStore::GlyphAnchor &anchor) {
                         return (anchor.m_name == "stemUpSE") && (anchor.m_x == 1.18) && (anchor.m_y == 0.168);
                     }),
        "Bravura SMuFL anchors were not loaded from metadata");
    const uint64_t musicDuplicateGeneration = musicStore.GetGeneration();
    const std::string changedMetadata
        = R"({"fontName":"Bravura","glyphsWithAnchors":{"noteheadBlack":{"stemUpSE":[9,9]}}})";
    ok &= Expect(musicStore.RegisterMusicFont(bravura.data(), bravura.size(), changedMetadata, "QS") == "Bravura"
            && (musicStore.GetGeneration() == musicDuplicateGeneration),
        "identical music face and alias registration was not idempotent");
    const std::vector<vrv::FontStore::GlyphAnchor> anchorsAfterDuplicate
        = musicStore.GetMusicGlyphAnchors("QS", "noteheadBlack");
    ok &= Expect(anchorsAfterDuplicate.size() == noteheadAnchors.size()
            && std::any_of(anchorsAfterDuplicate.begin(), anchorsAfterDuplicate.end(),
                [](const vrv::FontStore::GlyphAnchor &anchor) {
                    return (anchor.m_name == "stemUpSE") && (anchor.m_x == 1.18) && (anchor.m_y == 0.168);
                }),
        "duplicate music registration silently replaced face-owned anchors");
    const std::vector<vrv::FontStore::GlyphAnchor> aliasedNoteheadAnchors
        = musicStore.GetMusicGlyphAnchors("QS", "noteheadBlack");
    ok &= Expect(aliasedNoteheadAnchors.size() == noteheadAnchors.size(), "music alias did not resolve SMuFL anchors");
    ok &= Expect(
        musicStore.HasFace(vrv::FontStore::Kind::Music, "QS") && aliasStore.HasFace(vrv::FontStore::Kind::Text, "QS"),
        "the same alias was not allowed in separate music and text namespaces");

    vrv::FontStore store;
    ok &= Expect(store.RegisterTextFont(woff.data(), woff.size()) == "Tinos", "FontStore WOFF1 registration failed");
    ok &= Expect(store.GetCounters().m_decodedFonts == 1, "WOFF1 was not decoded exactly once");
    ok &= Expect(store.GetCounters().m_extractedOutlines == 0, "registration eagerly extracted outlines");
    ok &= Expect(store.RegisterTextFont(woff.data(), woff.size()) == "Tinos", "FontStore WOFF1 duplicate failed");
    ok &= Expect(store.GetCounters().m_decodedFonts == 1, "duplicate WOFF1 was decoded again");
    const uint64_t compressedAliasGeneration = store.GetGeneration();
    ok &= Expect(store.RegisterTextFont(woff.data(), woff.size(), "Compressed Tinos") == "Tinos"
            && (store.GetGeneration() == compressedAliasGeneration + 1) && (store.GetCounters().m_decodedFonts == 1),
        "adding an alias to a compressed face decoded it again");
    ok &= Expect(store.RegisterTextFont(woff.data(), 20).empty(), "truncated WOFF1 was accepted");
    vrv::FontStore sharedDecodeStore;
    ok &= Expect(
        sharedDecodeStore.RegisterTextFont(woff.data(), woff.size()) == "Tinos", "shared WOFF1 registration failed");
    ok &= Expect(sharedDecodeStore.GetCounters().m_decodedFonts == 0,
        "WOFF1 was physically decompressed again in another FontStore");

    vrv::Toolkit woffToolkit(false);
    RenderTextAsPaths(woffToolkit);
    ok &= Expect(woffToolkit.RegisterTextFontFile(argv[4]) == "Tinos", "WOFF1 face was not registered");
    ok &= Expect(woffToolkit.RegisterTextFontFile(argv[4]) == "Tinos", "WOFF1 duplicate was not idempotent");
    vrv::Toolkit woff2Toolkit(false);
    RenderTextAsPaths(woff2Toolkit);
    ok &= Expect(woff2Toolkit.RegisterTextFontFile(argv[5]) == "Tinos", "WOFF2 face was not registered");

    std::vector<unsigned char> malformedSfnt = ttf;
    WriteU32(malformedSfnt, 20, 0xFFFFFFF0U);
    ok &= Expect(store.RegisterTextFont(malformedSfnt.data(), malformedSfnt.size()).empty(),
        "SFNT with an out-of-range table offset was accepted");
    std::vector<unsigned char> excessiveTables(12, 0);
    excessiveTables[0] = 0;
    excessiveTables[1] = 1;
    WriteU16(excessiveTables, 4, 257);
    ok &= Expect(store.RegisterTextFont(excessiveTables.data(), excessiveTables.size()).empty(),
        "SFNT with excessive table count was accepted");
    const std::vector<unsigned char> collection = { 't', 't', 'c', 'f', 0, 1, 0, 0, 0, 0, 0, 1 };
    ok &= Expect(store.RegisterTextFont(collection.data(), collection.size()).empty(), "font collection was accepted");
    std::vector<unsigned char> woffBomb = woff;
    WriteU32(woffBomb, 16, 64U * 1024U * 1024U + 4U);
    ok &= Expect(store.RegisterTextFont(woffBomb.data(), woffBomb.size()).empty(),
        "WOFF1 with an excessive decoded size was accepted");
    const std::vector<unsigned char> oversized(32U * 1024U * 1024U + 1U, 0);
    ok &= Expect(
        store.RegisterTextFont(oversized.data(), oversized.size()).empty(), "font input over 32 MiB was accepted");
    ok &= Expect(musicStore.RegisterMusicFont(bravura.data(), bravura.size(), {}).empty(),
        "music font without SMuFL metadata was accepted");

    const std::optional<vrv::FontStore::GlyphMetrics> metrics
        = store.GetGlyphMetrics(vrv::FontStore::Kind::Text, "Tinos", U'A');
    ok &= Expect(metrics.has_value(), "Tinos A metrics were unavailable");
    if (metrics) {
        ok &= Expect(metrics->m_glyphId == 36, "Tinos A glyph ID differs from the trusted fixture");
        ok &= Expect(metrics->m_unitsPerEm == 2048, "Tinos units-per-em differs from the trusted fixture");
        ok &= Expect(metrics->m_advanceX == 1479, "Tinos A advance differs from the trusted fixture");
        ok &= Expect((metrics->m_xBearing == 20) && (metrics->m_yBearing == 1352) && (metrics->m_width == 1444)
                && (metrics->m_height == -1352),
            "Tinos A extents differ from the trusted fixture");
        const std::optional<std::string> outline
            = store.GetGlyphOutline(vrv::FontStore::Kind::Text, "Tinos", metrics->m_glyphId);
        ok &= Expect(outline && !outline->empty(), "Tinos A outline was unavailable");
        const std::optional<std::string> outlineAgain
            = store.GetGlyphOutline(vrv::FontStore::Kind::Text, "Tinos", metrics->m_glyphId);
        ok &= Expect(outlineAgain == outline, "cached Tinos A outline changed");
        ok &= Expect(store.GetCounters().m_extractedOutlines == 1, "Tinos A outline was not extracted exactly once");
    }

    const std::optional<vrv::FontStore::ShapedRun> shaped = store.ShapeText("Tinos", U"AV");
    ok &= Expect(shaped && (shaped->m_glyphs.size() == 2), "Tinos AV shaping failed");
    if (shaped && (shaped->m_glyphs.size() == 2)) {
        ok &= Expect((shaped->m_glyphs[0].m_glyphId == 36) && (shaped->m_glyphs[0].m_advanceX == 1215)
                && (shaped->m_glyphs[1].m_advanceX == 1479),
            "Tinos AV kerning differs from the trusted fixture");
    }
    ok &= Expect(store.ShapeText("Tinos", U"AV") == shaped, "cached Tinos AV shaping changed");
    ok &= Expect(store.GetCounters().m_shapedRuns == 1, "Tinos AV was shaped more than once");

    vrv::FontStore formatStore;
    ok &= Expect(formatStore.RegisterTextFont(ttf.data(), ttf.size()) == "Tinos", "TTF format identity setup failed");
    const std::optional<vrv::FontStore::GlyphMetrics> ttfIdentity
        = formatStore.GetGlyphMetrics(vrv::FontStore::Kind::Text, "Tinos", U'A');
    ok &= Expect(formatStore.RegisterTextFont(woff.data(), woff.size()) == "Tinos",
        "equivalent TTF/WOFF duplicate registration was rejected");
    vrv::FontStore woffFormatStore;
    ok &= Expect(woffFormatStore.RegisterTextFont(woff.data(), woff.size()) == "Tinos", "WOFF format setup failed");
    const std::optional<vrv::FontStore::GlyphMetrics> woffIdentity
        = woffFormatStore.GetGlyphMetrics(vrv::FontStore::Kind::Text, "Tinos", U'A');
    const std::vector<unsigned char> woff2 = ReadFile(argv[5]);
    std::vector<unsigned char> woff2Bomb = woff2;
    WriteU32(woff2Bomb, 16, 64U * 1024U * 1024U + 4U);
    ok &= Expect(formatStore.RegisterTextFont(woff2Bomb.data(), woff2Bomb.size()).empty(),
        "WOFF2 with an excessive decoded size was accepted");
    ok &= Expect(formatStore.RegisterTextFont(woff2.data(), woff2.size()) == "Tinos",
        "equivalent TTF/WOFF2 duplicate registration was rejected");
    vrv::FontStore woff2FormatStore;
    ok &= Expect(woff2FormatStore.RegisterTextFont(woff2.data(), woff2.size()) == "Tinos", "WOFF2 format setup failed");
    const std::optional<vrv::FontStore::GlyphMetrics> woff2Identity
        = woff2FormatStore.GetGlyphMetrics(vrv::FontStore::Kind::Text, "Tinos", U'A');
    ok &= Expect(ttfIdentity && woffIdentity && woff2Identity && (ttfIdentity->m_face == woffIdentity->m_face)
            && (ttfIdentity->m_face == woff2Identity->m_face),
        "TTF/WOFF1/WOFF2 did not produce one font identity");

    const std::vector<unsigned char> italic = ReadFile(argv[6]);
    const std::vector<unsigned char> bold = ReadFile(argv[7]);
    const std::vector<unsigned char> boldItalic = ReadFile(argv[8]);
    ok &= Expect(store.RegisterTextFont(italic.data(), italic.size()) == "Tinos", "Tinos Italic was not registered");
    ok &= Expect(store.RegisterTextFont(bold.data(), bold.size()) == "Tinos", "Tinos Bold was not registered");
    ok &= Expect(store.RegisterTextFont(boldItalic.data(), boldItalic.size()) == "Tinos",
        "Tinos Bold Italic was not registered");
    ok &= Expect(
        store.HasFace(vrv::FontStore::Kind::Text, "Tinos", vrv::FontStore::Weight::Bold, vrv::FontStore::Style::Italic),
        "Tinos Bold Italic face resolution failed");
    ok &= Expect(aliasStore.RegisterTextFont(italic.data(), italic.size()) == "Tinos"
            && aliasStore.RegisterTextFont(bold.data(), bold.size()) == "Tinos"
            && aliasStore.RegisterTextFont(boldItalic.data(), boldItalic.size()) == "Tinos"
            && aliasStore.HasFace(
                vrv::FontStore::Kind::Text, "QS", vrv::FontStore::Weight::Bold, vrv::FontStore::Style::Italic),
        "text alias did not resolve later registered family styles");
    for (const std::pair<vrv::FontStore::Weight, vrv::FontStore::Style> &weightStyle :
        { std::pair{ vrv::FontStore::Weight::Normal, vrv::FontStore::Style::Normal },
            std::pair{ vrv::FontStore::Weight::Normal, vrv::FontStore::Style::Italic },
            std::pair{ vrv::FontStore::Weight::Bold, vrv::FontStore::Style::Normal },
            std::pair{ vrv::FontStore::Weight::Bold, vrv::FontStore::Style::Italic } }) {
        const vrv::FontStore::Weight weight = weightStyle.first;
        const vrv::FontStore::Style style = weightStyle.second;
        const std::optional<vrv::FontStore::ShapedRun> aliasStyle = aliasStore.ShapeText("QS", U"A", weight, style);
        const std::optional<vrv::FontStore::ShapedRun> canonicalStyle
            = aliasStore.ShapeText("Tinos", U"A", weight, style);
        ok &= Expect(aliasStyle && canonicalStyle && !aliasStyle->m_glyphs.empty() && !canonicalStyle->m_glyphs.empty()
                && (aliasStyle->m_glyphs.front().m_face == canonicalStyle->m_glyphs.front().m_face),
            "family-wide alias did not select the canonical text weight/style face");
    }

    const std::vector<unsigned char> customHyphenFont = ReadFile(argv[16]);
    vrv::FontStore hyphenStore;
    ok &= Expect(hyphenStore.RegisterTextFont(ttf.data(), ttf.size()) == "Tinos"
            && hyphenStore.RegisterTextFont(customHyphenFont.data(), customHyphenFont.size()) == "Verovio Test Hyphen"
            && hyphenStore.RegisterTextFont(localizedRegular.data(), localizedRegular.size())
                == "Verovio Test Ligature",
        "lyric hyphen expected-font setup failed");

    vrv::Toolkit lyricHyphenRendering(false);
    RenderTextAsPaths(lyricHyphenRendering);
    ok &= Expect(lyricHyphenRendering.SetOptions("{\"breaks\":\"encoded\",\"lyricSize\":8.0}")
            && lyricHyphenRendering.SetResourcePath(argv[9])
            && (lyricHyphenRendering.RegisterTextFontFile(argv[16], "VH") == "Verovio Test Hyphen")
            && (lyricHyphenRendering.RegisterTextFontFile(argv[11], "NoHyphen") == "Verovio Test Ligature"),
        "custom lyric hyphen render resources could not be loaded");
    const std::string lyricHyphenMei = R"mei(<?xml version="1.0" encoding="UTF-8"?>
<mei xmlns="http://www.music-encoding.org/ns/mei" meiversion="5.1"><music><body><mdiv><score>
<scoreDef><staffGrp><staffDef n="1" lines="5" clef.shape="G" clef.line="2"/></staffGrp></scoreDef>
<section><measure n="1"><staff n="1"><layer n="1">
<note xml:id="hyphen-note-1" pname="c" oct="4" dur="4"><verse n="1"><syl xml:id="hyphen-start"
con="d" wordpos="i" fontname="VH">hy</syl></verse><verse n="2"><syl xml:id="fallback-start"
con="d" wordpos="i" fontname="NoHyphen">ffi</syl></verse></note>
</layer></staff></measure><sb/><measure n="2"><staff n="1"><layer n="1">
<note xml:id="hyphen-note-2" pname="d" oct="4" dur="4"><verse n="1"><syl wordpos="t"
fontname="VH">phen</syl></verse><verse n="2"><syl wordpos="t" fontname="NoHyphen">ffi</syl></verse></note>
</layer></staff></measure></section></score></mdiv></body></music></mei>)mei";
    ok &= Expect(lyricHyphenRendering.LoadData(lyricHyphenMei), "lyric hyphen render MEI could not be loaded");
    const std::string lyricHyphenSvg = lyricHyphenRendering.RenderToSVG(1);
    const std::optional<vrv::FontStore::ShapedRun> customHyphen = hyphenStore.ShapeText("Verovio Test Hyphen", U"-");
    const std::optional<vrv::FontStore::ShapedRun> fallbackHyphen
        = hyphenStore.ShapeText("Verovio Test Ligature", U"-");
    auto glyphPrefix = [](const std::optional<vrv::FontStore::ShapedRun> &run) {
        std::ostringstream prefix;
        if (run && !run->m_glyphs.empty()) {
            prefix << "text-" << std::uppercase << std::hex << run->m_glyphs.front().m_face.m_value << "-" << std::dec
                   << run->m_glyphs.front().m_glyphId << "-";
        }
        return prefix.str();
    };
    const std::string customHyphenPrefix = glyphPrefix(customHyphen);
    const std::string fallbackHyphenPrefix = glyphPrefix(fallbackHyphen);
    ok &= Expect(!customHyphenPrefix.empty() && !fallbackHyphenPrefix.empty()
            && (customHyphenPrefix != fallbackHyphenPrefix)
            && (CountOccurrences(lyricHyphenSvg, customHyphenPrefix) >= 3)
            && (CountOccurrences(lyricHyphenSvg, fallbackHyphenPrefix) >= 3),
        "same-system or system-spanning lyric connectors did not use the selected font and fallback hyphen glyphs");
    ok &= Expect((lyricHyphenSvg.find("class=\"syl id-hyphen-start spanning\"") != std::string::npos)
            && (lyricHyphenSvg.find("class=\"syl id-fallback-start spanning\"") != std::string::npos)
            && (lyricHyphenSvg.find("<rect") == std::string::npos),
        "system-spanning lyric hyphens used rectangle geometry");

    vrv::Toolkit scoreLyricConnectorRendering(false);
    RenderTextAsPaths(scoreLyricConnectorRendering);
    ok &= Expect(scoreLyricConnectorRendering.SetOptions("{\"breaks\":\"encoded\",\"lyricSize\":8.0}")
            && scoreLyricConnectorRendering.SetResourcePath(argv[9])
            && (scoreLyricConnectorRendering.RegisterTextFontFile(argv[16]) == "Verovio Test Hyphen"),
        "score lyric connector rendering setup failed");
    std::string scoreLyricConnectorMei = lyricHyphenMei;
    ReplaceAll(scoreLyricConnectorMei, "<scoreDef>", "<scoreDef lyric.fam=\"Verovio Test Hyphen\">");
    ReplaceAll(scoreLyricConnectorMei, " fontname=\"VH\"", "");
    ReplaceAll(scoreLyricConnectorMei, " fontname=\"NoHyphen\"", "");
    ok &= Expect(
        scoreLyricConnectorRendering.LoadData(scoreLyricConnectorMei), "score lyric connector MEI could not be loaded");
    const std::string scoreLyricConnectorSvg = scoreLyricConnectorRendering.RenderToSVG(1);
    ok &= Expect(!customHyphenPrefix.empty() && (CountOccurrences(scoreLyricConnectorSvg, customHyphenPrefix) >= 6),
        "scoreDef lyric.fam did not select the registered face for syllables and their connectors");
    ok &= Expect(lyricHyphenRendering.RenderToSVG(1) == lyricHyphenSvg, "repeated lyric hyphen render changed SVG");

    const std::vector<unsigned char> ligatureFont = ReadFile(argv[11]);
    ok &= Expect(store.RegisterTextFont(ligatureFont.data(), ligatureFont.size()) == "Verovio Test Ligature",
        "ffi test font was not registered");
    const std::vector<unsigned char> conflictingLigatureFont = ReadFile(argv[14]);
    ok &= Expect(store.RegisterTextFont(conflictingLigatureFont.data(), conflictingLigatureFont.size()).empty(),
        "conflicting face with the same family/style was accepted");
    vrv::FontStore aliasConflictStore;
    ok &= Expect(aliasConflictStore.RegisterTextFont(ttf.data(), ttf.size(), "QS") == "Tinos",
        "alias conflict test setup failed");
    const uint64_t generationBeforeAliasConflict = aliasConflictStore.GetGeneration();
    ok &= Expect(aliasConflictStore.RegisterTextFont(ligatureFont.data(), ligatureFont.size(), "QS").empty()
            && !aliasConflictStore.HasFace(vrv::FontStore::Kind::Text, "Verovio Test Ligature")
            && (aliasConflictStore.GetGeneration() == generationBeforeAliasConflict),
        "alias reassignment was not rejected transactionally");
    ok &= Expect(aliasConflictStore.RegisterTextFont(ligatureFont.data(), ligatureFont.size(), "Tinos").empty()
            && !aliasConflictStore.HasFace(vrv::FontStore::Kind::Text, "Verovio Test Ligature"),
        "alias collision with a canonical family was accepted");
    vrv::FontStore canonicalConflictStore;
    ok &= Expect(canonicalConflictStore.RegisterTextFont(ttf.data(), ttf.size(), "Verovio Test Ligature") == "Tinos",
        "canonical collision test setup failed");
    const uint64_t canonicalConflictGeneration = canonicalConflictStore.GetGeneration();
    const std::optional<vrv::FontStore::GlyphMetrics> canonicalConflictAlias
        = canonicalConflictStore.GetGlyphMetrics(vrv::FontStore::Kind::Text, "Verovio Test Ligature", U'A');
    ok &= Expect(canonicalConflictStore.RegisterTextFont(ligatureFont.data(), ligatureFont.size()).empty()
            && (canonicalConflictStore.GetGeneration() == canonicalConflictGeneration) && canonicalConflictAlias
            && (canonicalConflictStore.GetGlyphMetrics(vrv::FontStore::Kind::Text, "Verovio Test Ligature", U'A')
                    ->m_face
                == canonicalConflictAlias->m_face),
        "canonical family collision with an existing alias was accepted");
    const std::optional<vrv::FontStore::ShapedRun> ligature = store.ShapeText("Verovio Test Ligature", U"ffi");
    ok &= Expect(ligature && (ligature->m_glyphs.size() == 1), "ffi ligature was not applied");
    const std::optional<vrv::FontStore::ShapedRun> combining = store.ShapeText("Tinos", U"A\u0301");
    ok &= Expect(combining
            && std::none_of(combining->m_glyphs.begin(), combining->m_glyphs.end(),
                [](const vrv::FontStore::GlyphPlacement &glyph) { return glyph.m_glyphId == 0; }),
        "Tinos combining mark shaping produced .notdef");
    const std::optional<vrv::FontStore::ShapedRun> greek = store.ShapeText("Tinos", U"Καλημέρα");
    ok &= Expect(greek
            && std::none_of(greek->m_glyphs.begin(), greek->m_glyphs.end(),
                [](const vrv::FontStore::GlyphPlacement &glyph) { return glyph.m_glyphId == 0; }),
        "Tinos Greek shaping produced .notdef");
    const std::optional<vrv::FontStore::ShapedRun> fallbackGreek
        = store.ShapeText("Verovio Test Ligature", U"Καλημέρα");
    ok &= Expect(fallbackGreek && greek && !fallbackGreek->m_glyphs.empty()
            && (fallbackGreek->m_glyphs.front().m_face == greek->m_glyphs.front().m_face),
        "missing custom-font clusters did not fall back to Tinos");
    const std::optional<vrv::FontStore::ShapedRun> missingCluster
        = store.ShapeText("Verovio Test Ligature", U"\U0001F9CC");
    ok &= Expect(missingCluster
            && std::ranges::any_of(missingCluster->m_glyphs,
                [](const vrv::FontStore::GlyphPlacement &glyph) { return glyph.m_glyphId == 0; }),
        "cluster missing from both requested font and Tinos did not use .notdef");

    vrv::Toolkit rendering(false);
    RenderTextAsPaths(rendering);
    ok &= Expect(rendering.SetResourcePath(argv[9]), "bundled runtime fonts could not be initialized");
    ok &= Expect(rendering.LoadFile(argv[10]), "text-heavy runtime font fixture could not be loaded");
    const std::string svg = rendering.RenderToSVG(1);
    ok &= Expect(!svg.empty() && (svg.find("<text") == std::string::npos) && (svg.find("<tspan") == std::string::npos)
            && (svg.find("@font-face") == std::string::npos) && (svg.find("font-family") == std::string::npos),
        "runtime SVG contains visible text or an embedded font");
    ok &= Expect((svg.find("text-") != std::string::npos) && (svg.find("<use") != std::string::npos),
        "runtime SVG did not emit deduplicated text glyph paths");
    ok &= Expect(rendering.RenderToSVG(1) == svg, "identical repeated SVG render did not use stable output");
    const std::string svgWithDeclaration = rendering.RenderToSVG(1, true);
    ok &= Expect(svgWithDeclaration.starts_with("<?xml") && (svgWithDeclaration != svg),
        "SVG cache did not distinguish XML declaration mode");
    ok &= Expect(rendering.SetOptions("{\"scale\":110}"), "SVG cache option-invalidation setup failed");
    const std::string scaledSvg = rendering.RenderToSVG(1);
    ok &= Expect(!scaledSvg.empty() && (scaledSvg != svg), "SVG cache was not invalidated after an option change");

    vrv::Toolkit textRendering(false);
    ok &= Expect(textRendering.SetResourcePath(argv[9]) && textRendering.LoadFile(argv[10]),
        "text-mode runtime font fixture could not be loaded");
    const std::string textSvg = textRendering.RenderToSVG(1);
    ok &= Expect((textSvg.find("<text") != std::string::npos) && (textSvg.find("<tspan") != std::string::npos)
            && (textSvg.find("#text-") == std::string::npos),
        "default SVG did not output text as SVG text");
    ok &= Expect(textSvg.find("font-family=\"Times, Tinos, serif\"") != std::string::npos,
        "default SVG did not declare the text font family");
    const std::string resourcePath = argv[9];
    const std::vector<unsigned char> leipzig = ReadFile(resourcePath + "/fonts/Leipzig.woff2");
    const std::vector<unsigned char> leipzigMetadata = ReadFile(resourcePath + "/fonts/Leipzig_metadata.json");
    vrv::FontStore leipzigStore;
    ok &= Expect(leipzigStore.RegisterMusicFont(
                     leipzig.data(), leipzig.size(), std::string(leipzigMetadata.begin(), leipzigMetadata.end()))
            == "Leipzig",
        "bundled Leipzig was not registered");
    const std::optional<vrv::FontStore::GlyphMetrics> leipzigClef
        = leipzigStore.GetGlyphMetrics(vrv::FontStore::Kind::Music, "Leipzig", U'\uE050');
    ok &= Expect(HasGlyphOutline(textSvg, leipzigStore, leipzigClef), "default SVG did not use Leipzig as music font");
    ok &= Expect(
        textSvg.find("href=\"#E050-") != std::string::npos, "music glyph ids did not keep the SMuFL code as prefix");
    ok &= Expect(textSvg.find("@font-face {font-family: 'Tinos'") == std::string::npos,
        "text font was embedded without svgEmbedTextFont");
    ok &= Expect(textRendering.SetOptions("{\"svgEmbedTextFont\":true}"), "text font embedding option was rejected");
    const std::string embeddedSvg = textRendering.RenderToSVG(1);
    ok &= Expect(
        embeddedSvg.find("@font-face {font-family: 'Tinos'; src: url(data:font/woff2;base64,") != std::string::npos,
        "svgEmbedTextFont did not embed the text font as base64");
    ok &= Expect(embeddedSvg.find("format('woff2'); font-weight: bold; font-style: italic;}") != std::string::npos,
        "svgEmbedTextFont did not embed all the faces of the text font");
    ok &= Expect(embeddedSvg.find("font-family=\"Tinos, serif\"") != std::string::npos,
        "the embedded text font was not preferred over its metric equivalent");
    ok &= Expect(
        embeddedSvg.find("font-family=\"Tinos\"") == std::string::npos, "the text font was repeated within the SVG");

    vrv::Toolkit musicTextRendering(false);
    const std::string musicTextMei = R"mei(<?xml version="1.0" encoding="UTF-8"?>
<mei xmlns="http://www.music-encoding.org/ns/mei" meiversion="5.1"><music><body><mdiv><score>
<scoreDef><staffGrp><staffDef n="1" lines="5" clef.shape="G" clef.line="2"/></staffGrp></scoreDef>
<section><measure n="1"><staff n="1"><layer n="1"><note xml:id="dynam-note" pname="c" oct="4" dur="1"/>
</layer></staff><dynam startid="#dynam-note">mf cresc.</dynam></measure></section></score></mdiv></body></music></mei>)mei";
    ok &= Expect(musicTextRendering.SetResourcePath(argv[9]) && musicTextRendering.LoadData(musicTextMei),
        "music text MEI could not be loaded");
    ok &= Expect(
        musicTextRendering.RenderToSVG(1).find("@font-face {font-family: 'Leipzig'; src: url(data:font/woff2;base64,")
            != std::string::npos,
        "the music font used in text was not embedded");
    ok &= Expect(musicTextRendering.SetOptions("{\"smuflTextFont\":\"linked\"}"), "smuflTextFont was rejected");
    ok &= Expect(
        musicTextRendering.RenderToSVG(1).find("/data/fonts/Leipzig_subset.woff2\") format('woff2')") != std::string::npos,
        "the music font used in text was not linked");

    vrv::Toolkit musicGlyphRendering(false);
    std::string musicGlyphMei = musicTextMei;
    ReplaceAll(musicGlyphMei, "<dynam startid=\"#dynam-note\">mf cresc.</dynam>",
        "<harm startid=\"#dynam-note\">x&#xE050;y</harm>");
    ok &= Expect(musicGlyphRendering.SetResourcePath(argv[9]) && musicGlyphRendering.LoadData(musicGlyphMei),
        "music glyph MEI could not be loaded");
    ok &= Expect(
        musicGlyphRendering.RenderToSVG(1).find("font-family=\"Leipzig\" font-style=\"normal\"") != std::string::npos,
        "the music glyph missing in the text font was not written with the music font");

    // A glyph provided by two music fonts is defined twice, with a counter in the id of the second one
    vrv::Toolkit twoFontsRendering(false);
    const std::string twoFontsMei = R"mei(<?xml version="1.0" encoding="UTF-8"?>
<mei xmlns="http://www.music-encoding.org/ns/mei" meiversion="5.1"><music><body><mdiv><score>
<scoreDef><staffGrp><staffDef n="1" lines="5" clef.shape="G" clef.line="2" meter.count="3" meter.unit="4"/>
</staffGrp></scoreDef><section><measure n="1"><staff n="1"><layer n="1"><note pname="c" oct="5" dur="2" dots="1"/>
</layer></staff></measure><measure n="2"><staff n="1"><layer n="1"><meterSig count="3" unit="4" fontname="VM"/>
<note pname="d" oct="5" dur="2" dots="1"/></layer></staff></measure></section></score></mdiv></body></music></mei>)mei";
    ok &= Expect(twoFontsRendering.SetResourcePath(argv[9])
            && (twoFontsRendering.RegisterMusicFontFile(argv[12], argv[13], "VM") == "Verovio Test Music")
            && twoFontsRendering.LoadData(twoFontsMei),
        "two music fonts MEI could not be loaded");
    const std::string twoFontsSvg = twoFontsRendering.RenderToSVG(1);
    ok &= Expect((twoFontsSvg.find("id=\"E083-") != std::string::npos)
            && (twoFontsSvg.find("id=\"E083-1-") != std::string::npos),
        "a glyph used from two music fonts was not defined with distinct ids");

    const std::vector<unsigned char> testText = ReadFile(argv[11]);
    const std::vector<unsigned char> testMusic = ReadFile(argv[12]);
    const std::vector<unsigned char> testMusicMetadataBytes = ReadFile(argv[13]);
    const std::string testMusicMetadata(testMusicMetadataBytes.begin(), testMusicMetadataBytes.end());
    vrv::FontStore testMusicStore;
    ok &= Expect(
        testMusicStore.RegisterMusicFont(testMusic.data(), testMusic.size(), testMusicMetadata) == "Verovio Test Music",
        "custom test music font was not registered");
    const std::optional<vrv::FontStore::GlyphMetrics> customClef
        = testMusicStore.GetGlyphMetrics(vrv::FontStore::Kind::Music, "Verovio Test Music", U'\uE050');
    ok &= Expect(customClef && gClef && (customClef->m_face != gClef->m_face),
        "custom music and Bravura unexpectedly share font identity "
            + std::to_string(customClef ? customClef->m_face.m_value : 0));
    vrv::Resources fallbackResources;
    fallbackResources.SetPath(argv[9]);
    ok &= Expect(fallbackResources.InitFonts(), "fallback test resources could not be initialized");
    ok &= Expect(fallbackResources.GetFontStore().GetCounters().m_extractedOutlines == 0,
        "bundled font initialization eagerly extracted outlines");
    ok &= Expect(fallbackResources.GetFontStoreForModification().RegisterMusicFont(
                     testMusic.data(), testMusic.size(), testMusicMetadata)
            == "Verovio Test Music",
        "custom music font could not be registered in Resources");
    ok &= Expect(
        fallbackResources.GetGlyph(U'\uE050', "Verovio Test Music") != NULL, "custom music glyph lookup failed");
    ok &= Expect(fallbackResources.SetCurrentFont("Verovio Test Music"), "custom music font could not be selected");
    vrv::FontInfo textWithMusicFallback;
    textWithMusicFallback.SetFaceName("Tinos");
    const std::optional<vrv::FontStore::ShapedRun> activeMusicTextFallback
        = fallbackResources.ShapeText(textWithMusicFallback, U"\uE050");
    ok &= Expect(activeMusicTextFallback && customClef && !activeMusicTextFallback->m_glyphs.empty()
            && (activeMusicTextFallback->m_glyphs.front().m_face == customClef->m_face),
        "text glyph missing from Tinos did not fall back to the active SMuFL font");
    const std::optional<vrv::FontStore::GlyphMetrics> bravuraQuarter
        = fallbackResources.GetFontStore().GetGlyphMetrics(vrv::FontStore::Kind::Music, "Bravura", U'\u2669');
    const std::optional<vrv::FontStore::ShapedRun> bravuraTextFallback
        = fallbackResources.ShapeText(textWithMusicFallback, U"\u2669");
    ok &= Expect(bravuraTextFallback && bravuraQuarter && !bravuraTextFallback->m_glyphs.empty()
            && (bravuraTextFallback->m_glyphs.front().m_face == bravuraQuarter->m_face)
            && (bravuraTextFallback->m_glyphs.front().m_glyphId == bravuraQuarter->m_glyphId),
        "text glyph missing from the active SMuFL font did not fall back to Bravura");
    const std::optional<vrv::FontStore::GlyphMetrics> fallbackBravuraMetrics
        = fallbackResources.GetFontStore().GetGlyphMetrics(vrv::FontStore::Kind::Music, "Bravura", U'\uE0A2');
    ok &= Expect(fallbackBravuraMetrics.has_value(), "Bravura metrics disappeared after custom music registration");
    ok &= Expect(fallbackBravuraMetrics
            && fallbackResources.GetFontStore()
                .GetGlyphMetrics(fallbackBravuraMetrics->m_face, fallbackBravuraMetrics->m_glyphId)
                .has_value(),
        "Bravura identity-based metrics lookup failed after custom registration");
    ok &= Expect(fallbackResources.GetGlyph(U'\uE0A2', "Verovio Test Music") != NULL,
        "missing custom music glyph did not fall back to Bravura");

    vrv::Toolkit customRendering(false);
    RenderTextAsPaths(customRendering);
    ok &= Expect(customRendering.SetResourcePath(argv[9]), "custom-font render resources could not be initialized");
    ok &= Expect(customRendering.RegisterTextFontFile(argv[11], "QS") == "Verovio Test Ligature",
        "custom text font could not be registered on Toolkit");
    ok &= Expect(customRendering.RegisterMusicFontFile(argv[12], argv[13], "VM") == "Verovio Test Music",
        "custom music font could not be registered on Toolkit");

    vrv::Toolkit base64Rendering(false);
    RenderTextAsPaths(base64Rendering);
    ok &= Expect(base64Rendering.SetResourcePath(argv[9]), "base64 alias resources could not be initialized");
    const std::string textBase64 = vrv::Base64Encode(testText.data(), static_cast<unsigned int>(testText.size()));
    const std::string musicBase64 = vrv::Base64Encode(testMusic.data(), static_cast<unsigned int>(testMusic.size()));
    ok &= Expect(base64Rendering.RegisterTextFontBase64(textBase64, "QS64") == "Verovio Test Ligature",
        "base64 text font alias registration failed");
    ok &= Expect(
        base64Rendering.RegisterMusicFontBase64(musicBase64, testMusicMetadata, "VM64") == "Verovio Test Music",
        "base64 music font alias registration failed");
    const std::string customMei = R"mei(<?xml version="1.0" encoding="UTF-8"?>
<mei xmlns="http://www.music-encoding.org/ns/mei" meiversion="5.1"><music><body><mdiv><score>
<scoreDef><staffGrp><staffDef n="1" lines="5" clef.shape="G" clef.line="2"/></staffGrp></scoreDef>
<section><measure n="1"><staff n="1"><layer n="1">
<clef shape="G" line="2" fontname="VM"/>
<meterSig count="3" unit="4" fontname="VM"/>
<note xml:id="custom-note" pname="c" oct="4" dur="1"/>
</layer></staff><dir startid="#custom-note"><rend fontname="QS" fontstyle="normal">ffi</rend></dir>
</measure></section></score></mdiv></body></music></mei>)mei";
    customRendering.ResetXmlIdSeed(0);
    ok &= Expect(customRendering.LoadData(customMei), "mixed per-element custom-font MEI could not be loaded");
    const std::string customSvg = customRendering.RenderToSVG(1);
    std::ostringstream textPrefix;
    if (ligature) textPrefix << "text-" << std::uppercase << std::hex << ligature->m_glyphs[0].m_face.m_value << "-";
    const std::optional<vrv::FontStore::GlyphMetrics> customTimeSig
        = testMusicStore.GetGlyphMetrics(vrv::FontStore::Kind::Music, "Verovio Test Music", U'\uE083');
    ok &= Expect(
        HasGlyphOutline(customSvg, testMusicStore, customTimeSig), "meterSig did not use its per-element music font");
    ok &= Expect(ligature && (customSvg.find(textPrefix.str()) != std::string::npos),
        "nested dir/rend did not use its per-element text font");
    ok &= Expect((customSvg.find("<text") == std::string::npos) && (customSvg.find("<tspan") == std::string::npos),
        "custom-font SVG emitted visible text");
    ok &= Expect(customRendering.RenderToSVG(1) == customSvg, "repeated aliased-font rendering changed the SVG");
    vrv::Toolkit canonicalRendering(false);
    RenderTextAsPaths(canonicalRendering);
    ok &= Expect(canonicalRendering.SetResourcePath(argv[9])
            && (canonicalRendering.RegisterTextFontFile(argv[11]) == "Verovio Test Ligature")
            && (canonicalRendering.RegisterMusicFontFile(argv[12], argv[13]) == "Verovio Test Music"),
        "canonical comparison fonts could not be registered");
    canonicalRendering.ResetXmlIdSeed(0);
    std::string canonicalMei = customMei;
    ReplaceAll(canonicalMei, "fontname=\"VM\"", "fontname=\"Verovio Test Music\"");
    ReplaceAll(canonicalMei, "fontname=\"QS\"", "fontname=\"Verovio Test Ligature\"");
    ok &= Expect(canonicalRendering.LoadData(canonicalMei), "canonical comparison MEI could not be loaded");
    const std::string canonicalSvg = canonicalRendering.RenderToSVG(1);
    const std::string canonicalGeometry = ExtractSvgGeometry(canonicalSvg);
    const std::string aliasGeometry = ExtractSvgGeometry(customSvg);
    ok &= Expect(canonicalGeometry == aliasGeometry,
        "alias and canonical family names produced different SVG geometry or glyph identities");

    vrv::Toolkit optionAliasRendering(false);
    RenderTextAsPaths(optionAliasRendering);
    const std::string aliasOptions = "{\"fontAddTextAs\":[\"QS=" + JsonEscape(argv[11])
        + "\",\"LongQS=" + JsonEscape(argv[11]) + "\"],\"fontAddMusicAs\":[\"VM=" + JsonEscape(argv[12]) + "\"]}";
    ok &= Expect(optionAliasRendering.SetOptions(aliasOptions), "aliased font options were rejected");
    ok &= Expect(
        optionAliasRendering.SetResourcePath(argv[9]), "aliased text/music options or SMuFL metadata discovery failed");
    ok &= Expect(optionAliasRendering.LoadData(customMei), "aliased option rendering MEI could not be loaded");
    const std::string optionAliasSvg = optionAliasRendering.RenderToSVG(1);
    ok &= Expect(HasGlyphOutline(optionAliasSvg, testMusicStore, customTimeSig)
            && optionAliasSvg.find(textPrefix.str()) != std::string::npos,
        "repeatable aliased font options did not affect rendering");

    vrv::Toolkit malformedAliasOptions(false);
    RenderTextAsPaths(malformedAliasOptions);
    ok &= Expect(malformedAliasOptions.SetOptions("{\"fontAddTextAs\":[\"missing-separator\"]}"),
        "malformed alias option setup failed unexpectedly");
    ok &= Expect(
        !malformedAliasOptions.SetResourcePath(argv[9]), "malformed aliased font option did not fail resource setup");

    vrv::Toolkit lateRegistration(false);
    RenderTextAsPaths(lateRegistration);
    ok &= Expect(lateRegistration.SetResourcePath(argv[9]), "late-registration resources could not be initialized");
    ok &= Expect(lateRegistration.RegisterTextFontFile(argv[11]) == "Verovio Test Ligature",
        "late alias registration face setup failed");
    ok &= Expect(lateRegistration.LoadData(customMei), "late-registration MEI could not be loaded");
    const std::string beforeRegistration = lateRegistration.RenderToSVG(1);
    ok &= Expect(lateRegistration.RegisterTextFontFile(argv[11], "QS") == "Verovio Test Ligature",
        "text font could not be registered after document load");
    const std::string afterRegistration = lateRegistration.RenderToSVG(1);
    ok &= Expect(ligature && (beforeRegistration.find(textPrefix.str()) == std::string::npos)
            && (afterRegistration.find(textPrefix.str()) != std::string::npos),
        "registration after document load did not trigger automatic relayout");
    ok &= Expect(
        lateRegistration.RenderToSVG(1) == afterRegistration, "repeated rendering changed the runtime-font SVG");
    return ok ? 0 : 1;
}
