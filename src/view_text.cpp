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
    
void View::DrawTextElement( DeviceContext *dc, TextElement *element, int x, int y, bool &setX, bool &setY )
{
    assert( dc );
    assert( element );
    
    if (element->Is() == REND) {
        Rend *rend = dynamic_cast<Rend*>(element);
        assert(rend);
        DrawRend( dc, rend, x, y, setX, setY );
    }
    else if (element->Is() == TEXT) {
        Text *text = dynamic_cast<Text*>(element);
        assert(text);
        DrawText( dc, text, x, y, setX, setY );
    }
}
    
void View::DrawRend( DeviceContext *dc, Rend *rend, int x, int y, bool &setX, bool &setY )
{
    assert( dc );
    assert( rend );
    
    dc->StartTextGraphic( rend, "", rend->GetUuid());
    
    DrawTextChildren( dc, rend, x, y, setX, setY);
    
    dc->EndTextGraphic( rend, this );
    
}

void View::DrawText( DeviceContext *dc, Text *text, int x, int y, bool &setX, bool &setY )
{
    assert( dc );
    assert( text );
    
    dc->DrawText( UTF16to8( text->GetText().c_str() ), text->GetText() );
}

} // namespace vrv
    
