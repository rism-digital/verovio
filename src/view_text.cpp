/////////////////////////////////////////////////////////////////////////////
// Name:        view_text.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <iostream>
#include <math.h>

//----------------------------------------------------------------------------

#include "devicecontext.h"
#include "doc.h"
#include "fb.h"
#include "fig.h"
#include "lb.h"
#include "num.h"
#include "options.h"
#include "rend.h"
#include "smufl.h"
#include "svg.h"
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

    dc->StartTextGraphic(f, "", f->GetUuid());

    DrawTextChildren(dc, f, params);

    dc->EndTextGraphic(f, this);
}

void View::DrawHarmString(DeviceContext *dc, int x, int y, std::wstring s)
{
    assert(dc);

    int toDcX = ToDeviceContextX(x);
    int toDcY = ToDeviceContextY(y);

    std::size_t prevPos = 0, pos;
    while ((pos = s.find_first_of(L"\u266D\u266E\u266F", prevPos)) != std::wstring::npos) {
        // If pos is > than the previous, it is the substring to extract
        if (pos > prevPos) {
            std::wstring substr = s.substr(prevPos, pos - prevPos);
            dc->DrawText(UTF16to8(substr), substr, toDcX, toDcY);
            // Once we have rendered the some text to not pass x / y anymore
            toDcX = VRV_UNSET;
            toDcY = VRV_UNSET;
        }

        // if it is the same or we still have space, it is the accidental
        if (pos == prevPos || pos < s.length()) {
            // Then the accidental
            std::wstring accid = s.substr(pos, 1);
            std::wstring smuflAccid;
            if (accid == L"\u266D") { // MUSIC FLAT SIGN
                smuflAccid.push_back(SMUFL_E260_accidentalFlat);
            }
            else if (accid == L"\u266E") { // MUSIC NATURAL SIGN
                smuflAccid.push_back(SMUFL_E261_accidentalNatural);
            }
            else if (accid == L"\u266F") { // MUSIC SHARP SIGN
                smuflAccid.push_back(SMUFL_E262_accidentalSharp);
            }
            else {
                smuflAccid.push_back(0xE26D);
            }

            FontInfo vrvTxt;
            vrvTxt.SetFaceName("VerovioText");
            dc->SetFont(&vrvTxt);
            // Once we have rendered the some text to not pass x / y anymore
            dc->DrawText(UTF16to8(smuflAccid), smuflAccid, toDcX, toDcY);
            dc->ResetFont();
            toDcX = VRV_UNSET;
            toDcY = VRV_UNSET;
        }
        // Skip the accidental and continue
        prevPos = pos + 1;
    }
    // Print the remainder of the string, or the full string if no accid
    if (prevPos < s.length()) {
        std::wstring substr = s.substr(prevPos, std::wstring::npos);
        dc->DrawText(UTF16to8(substr), substr, toDcX, toDcY);
    }
}

void View::DrawTextElement(DeviceContext *dc, TextElement *element, TextDrawingParams &params)
{
    assert(dc);
    assert(element);

    if (element->Is(FIGURE)) {
        F *f = dynamic_cast<F *>(element);
        assert(f);
        DrawF(dc, f, params);
    }
    else if (element->Is(LB)) {
        Lb *lb = dynamic_cast<Lb *>(element);
        assert(lb);
        DrawLb(dc, lb, params);
    }
    else if (element->Is(NUM)) {
        Num *num = dynamic_cast<Num *>(element);
        assert(num);
        DrawNum(dc, num, params);
    }
    else if (element->Is(REND)) {
        Rend *rend = dynamic_cast<Rend *>(element);
        assert(rend);
        DrawRend(dc, rend, params);
    }
    else if (element->Is(TEXT)) {
        Text *text = dynamic_cast<Text *>(element);
        assert(text);
        DrawText(dc, text, params);
    }
    else {
        assert(false);
    }
}

void View::DrawLb(DeviceContext *dc, Lb *lb, TextDrawingParams &params)
{
    assert(dc);
    assert(lb);

    dc->StartTextGraphic(lb, "", lb->GetUuid());

    FontInfo *currentFont = dc->GetFont();
    int descender = -m_doc->GetTextGlyphDescender(L'q', currentFont, false);
    int height = m_doc->GetTextGlyphHeight(L'I', currentFont, false);

    params.m_y -= ((descender + height) * 1.1);
    params.m_newLine = true;
    params.m_laidOut = true;

    dc->EndTextGraphic(lb, this);
}

void View::DrawNum(DeviceContext *dc, Num *num, TextDrawingParams &params)
{
    assert(dc);
    assert(num);

    dc->StartTextGraphic(num, "", num->GetUuid());

    Text *currentText = num->GetCurrentText();
    if (currentText && (currentText->GetText().length() > 0)) {
        DrawText(dc, num->GetCurrentText(), params);
    }
    else {
        DrawTextChildren(dc, num, params);
    }

    dc->EndTextGraphic(num, this);
}

void View::DrawFig(DeviceContext *dc, Fig *fig, TextDrawingParams &params)
{
    assert(dc);
    assert(fig);

    dc->StartGraphic(fig, "", fig->GetUuid());

    Svg *svg = dynamic_cast<Svg *>(fig->FindChildByType(SVG));
    if (svg) {
        params.m_x = fig->GetDrawingX();
        params.m_y = fig->GetDrawingY();
        DrawSvg(dc, svg, params);
    }

    dc->EndGraphic(fig, this);
}

void View::DrawRend(DeviceContext *dc, Rend *rend, TextDrawingParams &params)
{
    assert(dc);
    assert(rend);

    dc->StartTextGraphic(rend, "", rend->GetUuid());

    FontInfo rendFont;
    bool customFont = false;
    if (rend->HasFontname() || rend->HasFontsize() || rend->HasFontstyle() || rend->HasFontweight()) {
        customFont = true;
        if (rend->HasFontname()) rendFont.SetFaceName(rend->GetFontname().c_str());
        if (rend->HasFontsize()) {
            data_FONTSIZE *fs = rend->GetFontsizeAlternate();
            if (fs->GetType() == FONTSIZE_fontSizeNumeric) {
                rendFont.SetPointSize(fs->GetFontSizeNumeric());
            }
            else if (fs->GetType() == FONTSIZE_term) {
                int percent = 100;
                switch (fs->GetTerm()) {
                    case (FONTSIZETERM_xx_large): percent = 200; break;
                    case (FONTSIZETERM_x_large): percent = 150; break;
                    case (FONTSIZETERM_large): percent = 110; break;
                    case (FONTSIZETERM_larger): percent = 110; break;
                    case (FONTSIZETERM_small): percent = 80; break;
                    case (FONTSIZETERM_smaller): percent = 80; break;
                    case (FONTSIZETERM_x_small): percent = 60; break;
                    case (FONTSIZETERM_xx_small): percent = 50; break;
                    default: break;
                }
                rendFont.SetPointSize(params.m_pointSize * percent / 100);
            }
            else if (fs->GetType() == FONTSIZE_percent) {
                rendFont.SetPointSize(params.m_pointSize * fs->GetPercent() / 100);
            }
        }
        if (rend->HasFontstyle()) rendFont.SetStyle(rend->GetFontstyle());
        if (rend->HasFontweight()) rendFont.SetWeight(rend->GetFontweight());
    }
    if (customFont) dc->SetFont(&rendFont);

    if (params.m_laidOut) {
        if (params.m_alignment == HORIZONTALALIGNMENT_NONE) {
            params.m_alignment = rend->HasHalign() ? rend->GetHalign() : HORIZONTALALIGNMENT_left;
            params.m_x = rend->GetDrawingX();
            params.m_y = rend->GetDrawingY();
            dc->MoveTextTo(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), params.m_alignment);
        }
        else if (params.m_newLine) {
            params.m_newLine = false;
            dc->MoveTextTo(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), HORIZONTALALIGNMENT_NONE);
        }
    }

    DrawTextChildren(dc, rend, params);

    if (customFont) dc->ResetFont();

    dc->EndTextGraphic(rend, this);
}

void View::DrawText(DeviceContext *dc, Text *text, TextDrawingParams &params)
{
    assert(dc);
    assert(text);

    dc->StartTextGraphic(text, "", text->GetUuid());

    // special case where we want to replace the '#' or 'b' with a VerovioText glyphs
    if (text->GetFirstParent(HARM)) {
        DrawHarmString(dc, params.m_x, params.m_y, text->GetText());
    }
    // special case where we want to replace the '_' with a lyric connector
    // '_' are produce with the SibMEI plugin
    else if (text->GetFirstParent(SYL)) {
        DrawLyricString(dc, params.m_x, params.m_y, text->GetText());
    }

    else {
        if (params.m_laidOut && params.m_newLine) {
            dc->DrawText(
                UTF16to8(text->GetText()), text->GetText(), ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y));
            params.m_newLine = false;
        }
        else {
            dc->DrawText(UTF16to8(text->GetText()), text->GetText());
        }
    }

    dc->EndTextGraphic(text, this);
}

void View::DrawSvg(DeviceContext *dc, Svg *svg, TextDrawingParams &params)
{
    assert(dc);
    assert(svg);

    dc->StartGraphic(svg, "", svg->GetUuid());

    dc->DrawSvgShape(
        ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), svg->GetWidth(), svg->GetHeight(), svg->Get());

    dc->EndGraphic(svg, this);
}

} // namespace vrv
