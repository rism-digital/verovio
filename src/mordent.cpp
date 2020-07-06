/////////////////////////////////////////////////////////////////////////////
// Name:        mordent.cpp
// Author:      Klaus Rettinghaus
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "mordent.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "horizontalaligner.h"
#include "smufl.h"

namespace vrv {

//----------------------------------------------------------------------------
// Mordent
//----------------------------------------------------------------------------

Mordent::Mordent()
    : ControlElement("mordent-"), TimePointInterface(), AttColor(), AttOrnamentAccid(), AttPlacement(), AttMordentLog()
{
    RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_ORNAMENTACCID);
    RegisterAttClass(ATT_PLACEMENT);
    RegisterAttClass(ATT_MORDENTLOG);
    RegisterAttClass(ATT_EXTSYM);

    Reset();
}

Mordent::~Mordent() {}

void Mordent::Reset()
{
    ControlElement::Reset();
    TimePointInterface::Reset();
    ResetColor();
    ResetOrnamentAccid();
    ResetPlacement();
    ResetMordentLog();
    ResetExtSym();
}

wchar_t Mordent::GetMordentGlyph() const 
{
    if (!HasGlyphName()) {
        if (GetLong() == BOOLEAN_true) {
            // TODO: change FALSE condition glyph, it's supposed to be inverted long mordent/tremblement
            return GetForm() == mordentLog_FORM_upper ? SMUFL_E56E_ornamentTremblement : SMUFL_E56E_ornamentTremblement;
        }
        return GetForm() == mordentLog_FORM_upper ? SMUFL_E56C_ornamentMordent : SMUFL_E56D_ornamentMordentInverted;
    }

    // TODO: handle glyph.name value and return glyphs based on it
    return SMUFL_E56D_ornamentMordentInverted;
}

//----------------------------------------------------------------------------
// Mordent functor methods
//----------------------------------------------------------------------------

} // namespace vrv
