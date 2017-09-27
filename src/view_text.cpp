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
#include "lb.h"
#include "rend.h"
#include "smufl.h"
#include "style.h"
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

    std::size_t prev_pos = 0, pos;
    while ((pos = s.find_first_of(L"\u266D\u266E\u266F", prev_pos)) != std::wstring::npos) {
        // If pos is > than the previous, it is the substring to extract
        if (pos > prev_pos) {
            std::wstring substr = s.substr(prev_pos, pos - prev_pos);
            dc->DrawText(UTF16to8(substr), substr);
        }

        // if it is the same or we still have space, it is the accidental
        if (pos == prev_pos || pos < s.length()) {
            // Then the accidental
            std::wstring accid = s.substr(pos, 1);
            std::wstring smufl_accid;
            if (accid == L"\u266D") { // MUSIC FLAT SIGN
                smufl_accid.push_back(SMUFL_E260_accidentalFlat);
            }
            else if (accid == L"\u266E") { // MUSIC NATURAL SIGN
                smufl_accid.push_back(SMUFL_E261_accidentalNatural);
            }
            else if (accid == L"\u266F") { // MUSIC SHARP SIGN
                smufl_accid.push_back(SMUFL_E262_accidentalSharp);
            }
            else {
                smufl_accid.push_back(0xE26D);
            }

            FontInfo vrvTxt;
            vrvTxt.SetFaceName("VerovioText");
            dc->SetFont(&vrvTxt);
            dc->DrawText(UTF16to8(smufl_accid), smufl_accid);
            dc->ResetFont();
        }
        // Skip the accidental and continue
        prev_pos = pos + 1;
    }
    // Print the remainder of the string, or the full string if no accid
    if (prev_pos < s.length()) {
        std::wstring substr = s.substr(prev_pos, std::wstring::npos);
        dc->DrawText(UTF16to8(substr), substr);
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
}

void View::DrawLb(DeviceContext *dc, Lb *lb, TextDrawingParams &params)
{
    assert(dc);
    assert(lb);

    dc->StartTextGraphic(lb, "", lb->GetUuid());
    
    FontInfo *currentFont = dc->GetFont();
    int descender = -m_doc->GetTextGlyphDescender(L'q', currentFont, false);
    int height = m_doc->GetTextGlyphHeight(L'I', currentFont, false);
    
    params.m_y -= (descender + height);
    params.m_newLine = true;
    
    dc->EndTextGraphic(lb, this);
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
            if (fs->GetType() == FONTSIZE_fontSizeNumeric) rendFont.SetPointSize(fs->GetFontSizeNumeric());
        }
        if (rend->HasFontstyle()) rendFont.SetStyle(rend->GetFontstyle());
        if (rend->HasFontweight()) rendFont.SetWeight(rend->GetFontweight());
    }
    if (customFont) dc->SetFont(&rendFont);
    
    if (params.m_laidOut) {
        if (rend->HasHalign() && (rend->GetHalign() != params.m_alignment)) {
            params.m_alignment = rend->GetHalign();
            params.m_x = rend->GetDrawingX();
            params.m_y = rend->GetDrawingY();
            if (params.m_width != 0) {
                switch (params.m_alignment) {
                    case (HORIZONTALALIGNMENT_right): params.m_x += params.m_width; break;
                    case (HORIZONTALALIGNMENT_center): params.m_x += (params.m_width / 2); break;
                    default: break;
                }
                
            }
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
            dc->DrawText(UTF16to8(text->GetText()), text->GetText(), ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y));
            params.m_newLine = false;
        }
        else {
            dc->DrawText(UTF16to8(text->GetText()), text->GetText());
        }
    }
    
    dc->EndTextGraphic(text, this);
}

} // namespace vrv
