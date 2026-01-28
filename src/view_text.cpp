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

#include "bboxdevicecontext.h"
#include "devicecontext.h"
#include "div.h"
#include "doc.h"
#include "dynam.h"
#include "f.h"
#include "fb.h"
#include "fig.h"
#include "graphic.h"
#include "lb.h"
#include "num.h"
#include "options.h"
#include "page.h"
#include "pgfoot.h"
#include "pghead.h"
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
        System *currentSystem = vrv_cast<System *>(f->GetFirstAncestor(SYSTEM));
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
    assert(dc->HasFont());

    std::u32string convertedStr = str;
    // If the current font is a music font, we want to convert Music Unicode glyph to SMuFL
    if (dc->GetFont()->GetSmuflFont()) {
        for (int i = 0; i < (int)str.size(); ++i) {
            convertedStr[i] = Resources::GetSmuflGlyphForUnicodeChar(str.at(i));
        }
    }
    this->DrawTextString(dc, convertedStr, params);
}

void View::DrawDynamString(DeviceContext *dc, const std::u32string &str, TextDrawingParams &params, Rend *rend)
{
    assert(dc);
    assert(dc->HasFont());

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
                vrvTxt.SetFaceName(m_doc->GetResources().GetCurrentFont());
                bool isFallbackNeeded = (m_doc->GetResources()).IsSmuflFallbackNeeded(smuflStr);
                vrvTxt.SetSmuflWithFallback(isFallbackNeeded);
                vrvTxt.SetStyle(FONTSTYLE_normal);
                vrvTxt.SetLetterSpacing(90);
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
    assert(dc->HasFont());

    int toDcX = this->ToDeviceContextX(params.m_x);
    int toDcY = this->ToDeviceContextY(params.m_y);

    size_t prevPos = 0, pos;
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
            vrvTxt.SetFaceName(m_doc->GetResources().GetCurrentFont());
            bool isFallbackNeeded = (m_doc->GetResources()).IsSmuflFallbackNeeded(smuflAccid);
            vrvTxt.SetSmuflWithFallback(isFallbackNeeded);
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

    // Disable x for what is coming next as child of <f>
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
    assert(dc->HasFont());

    bool wroteText = false;
    std::u32string syl = U"";
    std::u32string lyricStr = str;

    const int dcX = (params) ? this->ToDeviceContextX(params->m_x) : VRV_UNSET;
    const int dcY = (params) ? this->ToDeviceContextY(params->m_y) : VRV_UNSET;
    const int width = (params) ? params->m_width : VRV_UNSET;
    const int height = (params) ? params->m_height : VRV_UNSET;

    if (m_doc->GetOptions()->m_lyricElision.GetValue() == ELISION_unicode) {
        std::replace(lyricStr.begin(), lyricStr.end(), U'_', UNICODE_UNDERTIE);
        dc->DrawText(UTF32to8(lyricStr), lyricStr, dcX, dcY, width, height);
    }
    else {
        while (lyricStr.compare(syl) != 0) {
            wroteText = true;
            auto index = lyricStr.find_first_of(U"_");
            syl = lyricStr.substr(0, index);
            dc->DrawText(UTF32to8(syl), syl, dcX, dcY, width, height);

            // no _
            if (index == std::string::npos) break;

            FontInfo vrvTxt;
            vrvTxt.SetPointSize(dc->GetFont()->GetPointSize() * m_doc->GetMusicToLyricFontSizeRatio());
            vrvTxt.SetFaceName(m_doc->GetResources().GetCurrentFont());
            std::u32string elision;
            elision.push_back(m_doc->GetOptions()->m_lyricElision.GetValue());
            bool isFallbackNeeded = (m_doc->GetResources()).IsSmuflFallbackNeeded(elision);
            vrvTxt.SetSmuflWithFallback(isFallbackNeeded);
            dc->SetFont(&vrvTxt);
            dc->DrawText(UTF32to8(elision), elision, dcX, dcY, width, height);
            dc->ResetFont();

            // next syllable
            syl = U"";
            lyricStr = lyricStr.substr(index + 1, lyricStr.length());
        }
    }

    // This should only be called in facsimile mode where a zone is specified but there is
    // no text. This draws the bounds of the zone but leaves the space blank.
    if (!wroteText && params) {
        dc->DrawText("", U"", this->ToDeviceContextX(params->m_x), this->ToDeviceContextY(params->m_y), params->m_width,
            params->m_height);
    }
}

void View::DrawLb(DeviceContext *dc, Lb *lb, TextDrawingParams &params)
{
    assert(dc);
    assert(dc->HasFont());
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

    Svg *svg = vrv_cast<Svg *>(fig->FindDescendantByType(SVG));
    if (svg) {
        params.m_x = fig->GetDrawingX();
        params.m_y = fig->GetDrawingY();
        this->DrawSvg(dc, svg, params, 100, false);
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
            dc->MoveTextTo(this->ToDeviceContextX(params.m_x), this->ToDeviceContextY(params.m_y), params.m_alignment);
        }
    }

    FontInfo rendFont;
    bool customFont = false;
    if (rend->HasFontname()) {
        rendFont.SetFaceName(rend->GetFontname().c_str());
        customFont = true;
    }
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
        customFont = true;
        // Also pass it to the children
        params.m_pointSize = rendFont.GetPointSize();
    }
    if (rend->HasGlyphAuth() && rend->GetGlyphAuth() == "smufl") {
        // Because we do not have the string at this stage we rely only on the selected font
        // This means fallback will not work for missing glyphs within <rend>
        rendFont.SetSmuflWithFallback(false);
        rendFont.SetFaceName(m_doc->GetResources().GetCurrentFont());
        int pointSize = (rendFont.GetPointSize() != 0) ? rendFont.GetPointSize() : params.m_pointSize;
        rendFont.SetPointSize(pointSize * m_doc->GetMusicToLyricFontSizeRatio());
        customFont = true;
    }
    if (rend->HasFontstyle()) {
        rendFont.SetStyle(rend->GetFontstyle());
        customFont = true;
    }
    if (rend->HasFontweight()) {
        rendFont.SetWeight(rend->GetFontweight());
        customFont = true;
    }
    if (rend->HasLetterspacing()) {
        rendFont.SetLetterSpacing(rend->GetLetterspacing() * m_doc->GetDrawingUnit(100));
        customFont = true;
    }

    if (customFont) dc->SetFont(&rendFont);

    int yShift = 0;
    if ((rend->GetRend() == TEXTRENDITION_sup) || (rend->GetRend() == TEXTRENDITION_sub)) {
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

    // Do not render enclosings if the content is empty
    if (rend->HasEnclosure()) {
        params.m_enclosedRend.push_back(rend);
        params.m_x = rend->GetContentRight() + m_doc->GetDrawingUnit(100);
        params.m_explicitPosition = true;
        params.m_enclose = rend->GetRend();
    }

    if (customFont) {
        dc->ResetFont();
        // Reset the point size not to have it cummulated
        assert(dc->HasFont());
        params.m_pointSize = dc->GetFont()->GetPointSize();
        // Possilbe corner case: maybe we also need to reset text enclosure here?
    }

    dc->EndTextGraphic(rend, this);
}

void View::DrawText(DeviceContext *dc, Text *text, TextDrawingParams &params)
{
    assert(dc);
    assert(dc->HasFont());
    assert(text);

    const Resources *resources = dc->GetResources();
    assert(resources);

    dc->StartTextGraphic(text, "", text->GetID());

    resources->SelectTextFont(dc->GetFont()->GetWeight(), dc->GetFont()->GetStyle());

    if (params.m_explicitPosition) {
        dc->MoveTextTo(
            this->ToDeviceContextX(params.m_x), this->ToDeviceContextY(params.m_y), HORIZONTALALIGNMENT_NONE);
        params.m_explicitPosition = false;
    }
    else if (params.m_verticalShift) {
        dc->MoveTextVerticallyTo(this->ToDeviceContextY(params.m_y));
        params.m_verticalShift = false;
    }

    // special case where we want to replace some unicode music points to SMuFL
    if (text->GetFirstAncestor(CPMARK) || text->GetFirstAncestor(DIR) || text->GetFirstAncestor(ORNAM)
        || text->GetFirstAncestor(REPEATMARK)) {
        this->DrawDirString(dc, text->GetText(), params);
    }
    else if (text->GetFirstAncestor(DYNAM)) {
        this->DrawDynamString(dc, text->GetText(), params, vrv_cast<Rend *>(text->GetFirstAncestor(REND)));
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

    resources->SelectTextFont(FONTWEIGHT_NONE, FONTSTYLE_NONE);

    dc->EndTextGraphic(text, this);
}

void View::DrawGraphic(DeviceContext *dc, Graphic *graphic, TextDrawingParams &params, int staffSize, bool dimin)
{
    assert(dc);
    assert(graphic);

    dc->StartGraphic(graphic, "", graphic->GetID(), SYMBOLREF);

    int width = graphic->GetDrawingWidth(m_doc->GetDrawingUnit(staffSize), staffSize);
    int height = graphic->GetDrawingHeight(m_doc->GetDrawingUnit(staffSize), staffSize);

    if (dimin) {
        width = width * m_options->m_graceFactor.GetValue();
        height = height * m_options->m_graceFactor.GetValue();
    }

    dc->DrawGraphicUri(
        this->ToDeviceContextX(params.m_x), this->ToDeviceContextY(params.m_y), width, height, graphic->GetTarget());

    dc->EndGraphic(graphic, this);
}

void View::DrawSvg(DeviceContext *dc, Svg *svg, TextDrawingParams &params, int staffSize, bool dimin)
{
    assert(dc);
    assert(svg);

    dc->StartGraphic(svg, "", svg->GetID());

    int width = svg->GetWidth();
    int height = svg->GetHeight();
    double scale = 1.0;

    if (staffSize != 100) {
        width = width * staffSize / 100;
        height = height * staffSize / 100;
        scale = scale * staffSize / 100;
    }
    if (dimin) {
        width = width * m_options->m_graceFactor.GetValue();
        height = height * m_options->m_graceFactor.GetValue();
        scale = scale * m_options->m_graceFactor.GetValue();
    }

    dc->DrawSvgShape(
        this->ToDeviceContextX(params.m_x), this->ToDeviceContextY(params.m_y), width, height, scale, svg->Get());

    dc->EndGraphic(svg, this);
}

void View::DrawSymbol(DeviceContext *dc, Symbol *symbol, TextDrawingParams &params)
{
    assert(dc);
    assert(symbol);

    dc->StartTextGraphic(symbol, "", symbol->GetID());

    // This can happen after an <lb/>
    if (params.m_explicitPosition) {
        dc->MoveTextTo(
            this->ToDeviceContextX(params.m_x), this->ToDeviceContextY(params.m_y), HORIZONTALALIGNMENT_NONE);
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
        bool isFallbackNeeded = (m_doc->GetResources()).IsSmuflFallbackNeeded(str);
        symbolFont.SetSmuflWithFallback(isFallbackNeeded);
        symbolFont.SetFaceName(m_doc->GetResources().GetCurrentFont());
        int pointSize = (symbolFont.GetPointSize() != 0) ? symbolFont.GetPointSize() : params.m_pointSize;
        symbolFont.SetPointSize(pointSize * m_doc->GetMusicToLyricFontSizeRatio());
    }

    dc->SetFont(&symbolFont);

    this->DrawTextString(dc, str, params);

    dc->ResetFont();

    dc->EndTextGraphic(symbol, this);
}

void View::DrawRunningElements(DeviceContext *dc, Page *page)
{
    assert(dc);
    assert(page);

    if (dc->Is(BBOX_DEVICE_CONTEXT)) {
        BBoxDeviceContext *bBoxDC = vrv_cast<BBoxDeviceContext *>(dc);
        assert(bBoxDC);
        if (!bBoxDC->UpdateVerticalValues()) return;
    }

    RunningElement *header = page->GetHeader();
    if (header) {
        this->DrawTextLayoutElement(dc, header);
    }
    RunningElement *footer = page->GetFooter();
    if (footer) {
        this->DrawTextLayoutElement(dc, footer);
    }
}

void View::DrawTextLayoutElement(DeviceContext *dc, TextLayoutElement *textLayoutElement)
{
    assert(dc);
    assert(textLayoutElement);

    dc->StartGraphic(textLayoutElement, "", textLayoutElement->GetID());

    FontInfo textElementFont;
    if (!dc->UseGlobalStyling()) {
        textElementFont.SetFaceName(m_doc->GetResources().GetTextFont());
    }

    TextDrawingParams params;

    // If we have not timestamp
    params.m_x = textLayoutElement->GetDrawingX();
    params.m_y = textLayoutElement->GetDrawingY();
    params.m_width = textLayoutElement->GetTotalWidth(m_doc);
    params.m_alignment = HORIZONTALALIGNMENT_NONE;
    params.m_laidOut = true;
    params.m_pointSize = m_doc->GetDrawingLyricFont(100)->GetPointSize();

    textElementFont.SetPointSize(params.m_pointSize);

    dc->SetFont(&textElementFont);

    this->DrawRunningChildren(dc, textLayoutElement, params);

    dc->ResetFont();

    dc->EndGraphic(textLayoutElement, this);
}

void View::DrawDiv(DeviceContext *dc, Div *div, System *system)
{
    this->DrawTextLayoutElement(dc, div);
}

} // namespace vrv
