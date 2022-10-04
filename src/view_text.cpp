/////////////////////////////////////////////////////////////////////////////
// Name:        view_text.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <iostream>
#include <math.h>
#include <sstream>

//----------------------------------------------------------------------------

#include "devicecontext.h"
#include "doc.h"
#include "dynam.h"
#include "f.h"
#include "fb.h"
#include "fig.h"
#include "lb.h"
#include "num.h"
#include "options.h"
#include "rend.h"
#include "smufl.h"
#include "staff.h"
#include "svg.h"
#include "symbol.h"
#include "system.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// View - TextElement
//----------------------------------------------------------------------------

void View::DrawF(DeviceContext *dc, F *f, TextDrawingParams &params)
{
    assert(dc);
    assert(f);

    dc->StartTextGraphic(f, "", f->GetID());

    this->DrawTextChildren(dc, f, params);

    if (f->GetStart() && f->GetEnd()) {
        System *currentSystem = dynamic_cast<System *>(f->GetFirstAncestor(SYSTEM));
        // Postpone the drawing of the end of the system; this will call DrawFConnector
        if (currentSystem) {
            currentSystem->AddToDrawingList(f);
        }
    }

    dc->EndTextGraphic(f, this);
}

void View::DrawTextString(DeviceContext *dc, const std::u32string &str, TextDrawingParams &params)
{
    assert(dc);

    dc->DrawText(UTF32to8(str), str);
}

void View::DrawDirString(DeviceContext *dc, const std::u32string &str, TextDrawingParams &params)
{
    assert(dc);
    assert(dc->GetFont());

    std::u32string convertedStr = str;
    // If the current font is a music font, we want to convert Music Unicode glyph to SMuFL
    if (dc->GetFont()->GetSmuflFont()) {
        for (int i = 0; i < (int)str.size(); i++) {
            convertedStr[i] = Resources::GetSmuflGlyphForUnicodeChar(str.at(i));
        }
    }
    this->DrawTextString(dc, convertedStr, params);
}

void View::DrawDynamString(DeviceContext *dc, const std::u32string &str, TextDrawingParams &params, Rend *rend)
{
    assert(dc);

    const bool singleGlyphs = m_doc->GetOptions()->m_dynamSingleGlyphs.GetValue();

    if (rend && rend->HasFontfam()) {
        this->DrawTextString(dc, str, params);
        return;
    }

    if (params.m_textEnclose != ENCLOSURE_NONE) {
        std::u32string open;
        switch (params.m_textEnclose) {
            case ENCLOSURE_paren: open.push_back(L'('); break;
            case ENCLOSURE_brack: open.push_back(L'['); break;
            default: break;
        }
        this->DrawTextString(dc, open, params);
    }

    ArrayOfStringDynamTypePairs tokens;
    if (Dynam::GetSymbolsInStr(str, tokens)) {
        int first = true;
        for (auto &token : tokens) {
            if (!first) {
                // this->DrawTextString(dc, U" ", params);
            }
            first = false;

            if (token.second) {
                std::u32string smuflStr = Dynam::GetSymbolStr(token.first, singleGlyphs);
                FontInfo vrvTxt;
                vrvTxt.SetPointSize(dc->GetFont()->GetPointSize() * m_doc->GetMusicToLyricFontSizeRatio());
                vrvTxt.SetFaceName(m_doc->GetOptions()->m_font.GetValue());
                vrvTxt.SetSmuflFont(true);
                vrvTxt.SetStyle(FONTSTYLE_normal);
                dc->SetFont(&vrvTxt);
                this->DrawTextString(dc, smuflStr, params);
                dc->ResetFont();
            }
            else {
                this->DrawTextString(dc, token.first, params);
            }
        }
    }
    else {
        this->DrawTextString(dc, str, params);
    }

    if (params.m_textEnclose != ENCLOSURE_NONE) {
        std::u32string close;
        switch (params.m_textEnclose) {
            case ENCLOSURE_paren: close.push_back(L')'); break;
            case ENCLOSURE_brack: close.push_back(L']'); break;
            default: break;
        }
        this->DrawTextString(dc, close, params);
    }
}

void View::DrawHarmString(DeviceContext *dc, const std::u32string &str, TextDrawingParams &params)
{
    assert(dc);

    int toDcX = ToDeviceContextX(params.m_x);
    int toDcY = ToDeviceContextY(params.m_y);

    std::size_t prevPos = 0, pos;
    while ((pos = str.find_first_of(VRV_TEXT_HARM, prevPos)) != std::wstring::npos) {
        // If pos is > than the previous, it is the substring to extract
        if (pos > prevPos) {
            std::u32string substr = str.substr(prevPos, pos - prevPos);
            dc->DrawText(UTF32to8(substr), substr, toDcX, toDcY);
            // Once we have rendered the some text to not pass x / y anymore
            toDcX = VRV_UNSET;
            toDcY = VRV_UNSET;
        }

        // if it is the same or we still have space, it is the accidental
        if (pos == prevPos || pos < str.length()) {
            // Then the accidental
            std::u32string accid = str.substr(pos, 1);
            std::u32string smuflAccid;
            if (accid == U"\u266D" || accid == U"\uE260") { // MUSIC or SMUFL FLAT SIGN
                smuflAccid.push_back(SMUFL_EA64_figbassFlat);
            }
            else if (accid == U"\u266E" || accid == U"\uE261") { // MUSIC or SMUFL NATURAL SIGN
                smuflAccid.push_back(SMUFL_EA65_figbassNatural);
            }
            else if (accid == U"\u266F" || accid == U"\uE262") { // MUSIC or SMUFL SHARP SIGN
                smuflAccid.push_back(SMUFL_EA66_figbassSharp);
            }
            else if (accid == U"\uE264") { // SMUFL DOUBLE FLAT SIGN
                smuflAccid.push_back(SMUFL_EA63_figbassDoubleFlat);
            }
            else if (accid == U"\uE263") { // SMUFL DOUBLE SHARP SIGN
                smuflAccid.push_back(SMUFL_EA67_figbassDoubleSharp);
            }
            else {
                smuflAccid += accid;
            }

            FontInfo vrvTxt;
            vrvTxt.SetPointSize(dc->GetFont()->GetPointSize() * m_doc->GetMusicToLyricFontSizeRatio());
            vrvTxt.SetFaceName(m_doc->GetOptions()->m_font.GetValue());
            vrvTxt.SetSmuflFont(true);
            dc->SetFont(&vrvTxt);
            // Once we have rendered the some text to not pass x / y anymore
            dc->DrawText(UTF32to8(smuflAccid), smuflAccid, toDcX, toDcY);
            dc->ResetFont();
            toDcX = VRV_UNSET;
            toDcY = VRV_UNSET;
        }
        // Skip the accidental and continue
        prevPos = pos + 1;
    }
    // Print the remainder of the string, or the full string if no accid
    if (prevPos < str.length()) {
        std::u32string substr = str.substr(prevPos, std::wstring::npos);
        dc->DrawText(UTF32to8(substr), substr, toDcX, toDcY);
    }

    // Disable x for what is comming next as child of <f>
    // The value is reset in DrawFb
    params.m_x = VRV_UNSET;
}

void View::DrawTextElement(DeviceContext *dc, TextElement *element, TextDrawingParams &params)
{
    assert(dc);
    assert(element);

    if (element->Is(FIGURE)) {
        F *f = vrv_cast<F *>(element);
        assert(f);
        this->DrawF(dc, f, params);
    }
    else if (element->Is(LB)) {
        Lb *lb = vrv_cast<Lb *>(element);
        assert(lb);
        this->DrawLb(dc, lb, params);
    }
    else if (element->Is(NUM)) {
        Num *num = vrv_cast<Num *>(element);
        assert(num);
        this->DrawNum(dc, num, params);
    }
    else if (element->Is(REND)) {
        Rend *rend = vrv_cast<Rend *>(element);
        assert(rend);
        this->DrawRend(dc, rend, params);
    }
    else if (element->Is(SYMBOL)) {
        Symbol *symbol = vrv_cast<Symbol *>(element);
        assert(symbol);
        this->DrawSymbol(dc, symbol, params);
    }
    else if (element->Is(TEXT)) {
        Text *text = vrv_cast<Text *>(element);
        assert(text);
        this->DrawText(dc, text, params);
    }
    else {
        assert(false);
    }
}

void View::DrawLyricString(
    DeviceContext *dc, const std::u32string &str, int staffSize, std::optional<TextDrawingParams> params)
{
    assert(dc);

    bool wroteText = false;
    std::u32string syl = U"";
    std::u32string lyricStr = str;
    while (lyricStr.compare(syl) != 0) {
        wroteText = true;
        auto index = lyricStr.find_first_of(U"_");
        syl = lyricStr.substr(0, index);
        if (params) {
            dc->DrawText(UTF32to8(syl), syl, params->m_x, params->m_y, params->m_width, params->m_height);
        }
        else {
            dc->DrawText(UTF32to8(syl), syl);
        }

        // no _
        if (index == std::string::npos) break;

        FontInfo vrvTxt;
        vrvTxt.SetPointSize(dc->GetFont()->GetPointSize() * m_doc->GetMusicToLyricFontSizeRatio());
        vrvTxt.SetFaceName(m_doc->GetOptions()->m_font.GetValue());
        vrvTxt.SetSmuflFont(true);
        dc->SetFont(&vrvTxt);
        std::u32string elision;
        elision.push_back(SMUFL_E551_lyricsElision);
        if (params) {
            dc->DrawText(UTF32to8(elision), elision, params->m_x, params->m_y, params->m_width, params->m_height);
        }
        else {
            dc->DrawText(UTF32to8(elision), elision);
        }
        dc->ResetFont();

        // next syllable
        syl = U"";
        lyricStr = lyricStr.substr(index + 1, lyricStr.length());
    }

    // This should only be called in facsimile mode where a zone is specified but there is
    // no text. This draws the bounds of the zone but leaves the space blank.
    if (!wroteText && params) {
        dc->DrawText("", U"", params->m_x, params->m_y, params->m_width, params->m_height);
    }
}

void View::DrawLb(DeviceContext *dc, Lb *lb, TextDrawingParams &params)
{
    assert(dc);
    assert(lb);

    dc->StartTextGraphic(lb, "", lb->GetID());

    FontInfo *currentFont = dc->GetFont();

    params.m_y -= m_doc->GetTextLineHeight(currentFont, false);
    params.m_explicitPosition = true;

    dc->EndTextGraphic(lb, this);
}

void View::DrawNum(DeviceContext *dc, Num *num, TextDrawingParams &params)
{
    assert(dc);
    assert(num);

    dc->StartTextGraphic(num, "", num->GetID());

    Text *currentText = num->GetCurrentText();
    if (currentText && (currentText->GetText().length() > 0)) {
        this->DrawText(dc, num->GetCurrentText(), params);
    }
    else {
        this->DrawTextChildren(dc, num, params);
    }

    dc->EndTextGraphic(num, this);
}

void View::DrawFig(DeviceContext *dc, Fig *fig, TextDrawingParams &params)
{
    assert(dc);
    assert(fig);

    dc->StartGraphic(fig, "", fig->GetID());

    Svg *svg = dynamic_cast<Svg *>(fig->FindDescendantByType(SVG));
    if (svg) {
        params.m_x = fig->GetDrawingX();
        params.m_y = fig->GetDrawingY();
        this->DrawSvg(dc, svg, params);
    }

    dc->EndGraphic(fig, this);
}

void View::DrawRend(DeviceContext *dc, Rend *rend, TextDrawingParams &params)
{
    assert(dc);
    assert(rend);

    dc->StartTextGraphic(rend, "", rend->GetID());

    if (params.m_laidOut) {
        if (params.m_alignment == HORIZONTALALIGNMENT_NONE) {
            params.m_alignment = rend->HasHalign() ? rend->GetHalign() : HORIZONTALALIGNMENT_left;
            params.m_x = rend->GetDrawingX();
            params.m_y = rend->GetDrawingY();
            dc->MoveTextTo(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), params.m_alignment);
        }
    }

    FontInfo rendFont;
    bool customFont = false;
    if (rend->HasFontfam() || rend->HasFontname() || rend->HasFontsize() || rend->HasFontstyle()
        || rend->HasFontweight()) {
        customFont = true;
        if (rend->HasFontname()) rendFont.SetFaceName(rend->GetFontname().c_str());
        if (rend->HasFontsize()) {
            data_FONTSIZE *fs = rend->GetFontsizeAlternate();
            if (fs->GetType() == FONTSIZE_fontSizeNumeric) {
                rendFont.SetPointSize(fs->GetFontSizeNumeric());
            }
            else if (fs->GetType() == FONTSIZE_term) {
                const int percent = fs->GetPercentForTerm();
                rendFont.SetPointSize(params.m_pointSize * percent / 100);
            }
            else if (fs->GetType() == FONTSIZE_percent) {
                rendFont.SetPointSize(params.m_pointSize * fs->GetPercent() / 100);
            }
        }
        if (rend->HasFontfam() && rend->GetFontfam() == "smufl") {
            rendFont.SetSmuflFont(true);
            rendFont.SetFaceName(m_doc->GetOptions()->m_font.GetValue());
            int pointSize = (rendFont.GetPointSize() != 0) ? rendFont.GetPointSize() : params.m_pointSize;
            rendFont.SetPointSize(pointSize * m_doc->GetMusicToLyricFontSizeRatio());
        }
        if (rend->HasFontstyle()) rendFont.SetStyle(rend->GetFontstyle());
        if (rend->HasFontweight()) rendFont.SetWeight(rend->GetFontweight());
    }

    if (customFont) dc->SetFont(&rendFont);

    int yShift = 0;
    if ((rend->GetRend() == TEXTRENDITION_sup) || (rend->GetRend() == TEXTRENDITION_sub)) {
        assert(dc->GetFont());
        int MHeight = m_doc->GetTextGlyphHeight('M', dc->GetFont(), false);
        if (rend->GetRend() == TEXTRENDITION_sup) {
            yShift += m_doc->GetTextGlyphHeight('o', dc->GetFont(), false);
            yShift += (MHeight * SUPER_SCRIPT_POSITION);
        }
        else {
            yShift += MHeight * SUB_SCRIPT_POSITION;
        }
        params.m_y += yShift;
        params.m_verticalShift = true;
        dc->GetFont()->SetSupSubScript(true);
        dc->GetFont()->SetPointSize(dc->GetFont()->GetPointSize() * SUPER_SCRIPT_FACTOR);
    }

    if ((rend->GetRend() == TEXTRENDITION_box) && (params.m_actualWidth != 0)) {
        params.m_x = params.m_actualWidth + m_doc->GetDrawingUnit(100);
        params.m_explicitPosition = true;
    }

    this->DrawTextChildren(dc, rend, params);

    if ((rend->GetRend() == TEXTRENDITION_sup) || (rend->GetRend() == TEXTRENDITION_sub)) {
        params.m_y -= yShift;
        params.m_verticalShift = true;
        dc->GetFont()->SetSupSubScript(false);
        dc->GetFont()->SetPointSize(dc->GetFont()->GetPointSize() / SUPER_SCRIPT_FACTOR);
    }

    if ((rend->GetRend() == TEXTRENDITION_box) || (rend->GetRend() == TEXTRENDITION_circle)) {
        params.m_enclosedRend.push_back(rend);
        params.m_x = rend->GetContentRight() + m_doc->GetDrawingUnit(100);
        params.m_explicitPosition = true;
        params.m_enclose = rend->GetRend();
    }

    if (customFont) dc->ResetFont();

    dc->EndTextGraphic(rend, this);
}

void View::DrawText(DeviceContext *dc, Text *text, TextDrawingParams &params)
{
    assert(dc);
    assert(text);

    const Resources *resources = dc->GetResources();
    assert(resources);

    dc->StartTextGraphic(text, "", text->GetID());

    resources->SelectTextFont(dc->GetFont()->GetWeight(), dc->GetFont()->GetStyle());

    if (params.m_explicitPosition) {
        dc->MoveTextTo(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), HORIZONTALALIGNMENT_NONE);
        params.m_explicitPosition = false;
    }
    else if (params.m_verticalShift) {
        dc->MoveTextVerticallyTo(ToDeviceContextY(params.m_y));
        params.m_verticalShift = false;
    }

    // special case where we want to replace some unicode music points to SMuFL
    if (text->GetFirstAncestor(DIR)) {
        this->DrawDirString(dc, text->GetText(), params);
    }
    else if (text->GetFirstAncestor(DYNAM)) {
        this->DrawDynamString(dc, text->GetText(), params, dynamic_cast<Rend *>(text->GetFirstAncestor(REND)));
    }
    // special case where we want to replace the '#' or 'b' with a VerovioText glyphs
    else if (text->GetFirstAncestor(HARM)) {
        this->DrawHarmString(dc, text->GetText(), params);
    }
    // special case where we want to replace the '_' with a lyric connector
    // '_' are produce with the SibMEI plugin
    else if (text->GetFirstAncestor(SYL)) {
        if (params.m_height != VRV_UNSET && params.m_height != 0) {
            this->DrawLyricString(dc, text->GetText(), 100, params);
        }
        else {
            this->DrawLyricString(dc, text->GetText());
        }
    }
    else {
        this->DrawTextString(dc, text->GetText(), params);
    }

    params.m_actualWidth = text->GetContentRight();

    dc->EndTextGraphic(text, this);
}

void View::DrawSvg(DeviceContext *dc, Svg *svg, TextDrawingParams &params)
{
    assert(dc);
    assert(svg);

    dc->StartGraphic(svg, "", svg->GetID());

    dc->DrawSvgShape(
        ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), svg->GetWidth(), svg->GetHeight(), svg->Get());

    dc->EndGraphic(svg, this);
}

void View::DrawSymbol(DeviceContext *dc, Symbol *symbol, TextDrawingParams &params)
{
    assert(dc);
    assert(symbol);

    dc->StartTextGraphic(symbol, "", symbol->GetID());

    // This can happen after an <lb/>
    if (params.m_explicitPosition) {
        dc->MoveTextTo(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), HORIZONTALALIGNMENT_NONE);
        params.m_explicitPosition = false;
    }

    std::u32string str;
    str.push_back(symbol->GetSymbolGlyph());

    FontInfo symbolFont;

    if (symbol->HasFontsize()) {
        data_FONTSIZE *fs = symbol->GetFontsizeAlternate();
        if (fs->GetType() == FONTSIZE_fontSizeNumeric) {
            symbolFont.SetPointSize(fs->GetFontSizeNumeric());
        }
        else if (fs->GetType() == FONTSIZE_term) {
            const int percent = fs->GetPercentForTerm();
            symbolFont.SetPointSize(params.m_pointSize * percent / 100);
        }
        else if (fs->GetType() == FONTSIZE_percent) {
            symbolFont.SetPointSize(params.m_pointSize * fs->GetPercent() / 100);
        }
    }
    if (symbol->HasFontstyle()) {
        symbolFont.SetStyle(symbol->GetFontstyle());
    }
    else {
        // By default explicitly render it as normal
        symbolFont.SetStyle(FONTSTYLE_normal);
    }

    if (symbol->HasGlyphAuth() && symbol->GetGlyphAuth() == "smufl") {
        symbolFont.SetSmuflFont(true);
        symbolFont.SetFaceName(m_doc->GetOptions()->m_font.GetValue());
        int pointSize = (symbolFont.GetPointSize() != 0) ? symbolFont.GetPointSize() : params.m_pointSize;
        symbolFont.SetPointSize(pointSize * m_doc->GetMusicToLyricFontSizeRatio());
    }

    dc->SetFont(&symbolFont);

    this->DrawTextString(dc, str, params);

    dc->ResetFont();

    dc->EndTextGraphic(symbol, this);
}

} // namespace vrv
