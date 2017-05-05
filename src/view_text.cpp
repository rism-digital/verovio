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
#include <sstream>
#include <math.h>

//----------------------------------------------------------------------------

#include "devicecontext.h"
#include "doc.h"
#include "fb.h"
#include "style.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// View - TextElement
//----------------------------------------------------------------------------

void View::DrawF(DeviceContext *dc, F *figure, int x, int y)
{
    assert(dc);
    assert(figure);
    
    bool setX = false, setY = false;
    
    //dc->DrawText(UTF16to8(figure->GetText()), figure->GetText());
    
    dc->StartText(ToDeviceContextX(x), ToDeviceContextY(y), LEFT);
    DrawTextChildren(dc, figure, x, y, setX, setY);
    dc->EndText();

}

void View::DrawHarmString(DeviceContext *dc, int x, int y, std::wstring s)
{
    assert(dc);
    
    std::size_t prev_pos = 0, pos;
    while ((pos = s.find_first_of(L"\u266D\u266E\u266F", prev_pos)) != std::wstring::npos) {
        // If pos if > than the previous, it is the substring to exctrace
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
                smufl_accid.push_back(0xE260);
            } else if (accid == L"\u266E") { // MUSIC FLAT SIGN
                smufl_accid.push_back(0xE261);
            } else if (accid == L"\u266F") { // MUSIC SHARP SIGN
                smufl_accid.push_back(0xE262);
            } else {
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
    
void View::DrawTextElement(DeviceContext *dc, TextElement *element, int x, int y, bool &setX, bool &setY)
{
    assert(dc);
    assert(element);

    if (element->Is(REND)) {
        Rend *rend = dynamic_cast<Rend *>(element);
        assert(rend);
        DrawRend(dc, rend, x, y, setX, setY);
    }
    else if (element->Is(TEXT)) {
        Text *text = dynamic_cast<Text *>(element);
        assert(text);
        DrawText(dc, text, x, y, setX, setY);
    }
}

void View::DrawRend(DeviceContext *dc, Rend *rend, int x, int y, bool &setX, bool &setY)
{
    assert(dc);
    assert(rend);

    dc->StartTextGraphic(rend, "", rend->GetUuid());

    FontInfo rendFont;
    bool customFont = false;
    if (rend->HasFontsize()) {
        customFont = true;
        rendFont.SetPointSize(rend->GetFontsize());
    }
    if (customFont) dc->SetFont(&rendFont);

    DrawTextChildren(dc, rend, x, y, setX, setY);

    if (customFont) dc->ResetFont();

    dc->EndTextGraphic(rend, this);
}

void View::DrawText(DeviceContext *dc, Text *text, int x, int y, bool &setX, bool &setY)
{
    assert(dc);
    assert(text);

    // special case where we want to replace the '_' with a lyric connector
    // '_' are produce with the SibMEI plugin
    if (text->GetFirstParent(SYL)) {
        DrawLyricString(dc, x, y, text->GetText());
    }
    else if (text->GetFirstParent(HARM)) {
        DrawHarmString(dc, x, y, text->GetText());
    }
    else {
        dc->DrawText(UTF16to8(text->GetText()), text->GetText());
    }
}

} // namespace vrv
