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
#include "style.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// View - TextElement
//----------------------------------------------------------------------------

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

    if (rend->HasHalign()) {
        char alignment = LEFT;
        if (rend->GetHalign() == HORIZONTALALIGNMENT_right)
            alignment = RIGHT;
        else if (rend->GetHalign() == HORIZONTALALIGNMENT_center)
            alignment = CENTER;
        // Justified remains unsupported
        dc->SetTextAlignment(alignment);
    }

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
    else {
        dc->DrawText(UTF16to8(text->GetText()), text->GetText());
    }
}

} // namespace vrv
