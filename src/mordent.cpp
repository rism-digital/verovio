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
        // TODO: change FALSE condition glyph, it's supposed to be inverted long mordent/tremblement
        return GetMordentGlyph({ SMUFL_E56E_ornamentTremblement, SMUFL_E56E_ornamentTremblement,
            SMUFL_E56C_ornamentMordent, SMUFL_E56D_ornamentMordentInverted });
    }

    // TODO: handle glyph.name value and return glyphs based on it
    return SMUFL_E56D_ornamentMordentInverted;
}

wchar_t Mordent::GetMordentGlyph(std::vector<wchar_t> glyphs) const
{
    // order of glyphs in the vector is as following:
    // [0] long, [1] long inverted, [2] short, [3] short inverted
    if (GetLong() == BOOLEAN_true) {
        return GetForm() == mordentLog_FORM_upper ? glyphs[0] : glyphs[1];
    }
    return GetForm() == mordentLog_FORM_upper ? glyphs[2] : glyphs[3];
}

//----------------------------------------------------------------------------
// Mordent functor methods
//----------------------------------------------------------------------------

} // namespace vrv
