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
    : ControlElement("mordent-")
    , TimePointInterface()
    , AttColor()
    , AttExtSym()
    , AttOrnamentAccid()
    , AttPlacement()
    , AttMordentLog()
{
    RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_EXTSYM);
    RegisterAttClass(ATT_ORNAMENTACCID);
    RegisterAttClass(ATT_PLACEMENT);
    RegisterAttClass(ATT_MORDENTLOG);

    Reset();
}

Mordent::~Mordent() {}

void Mordent::Reset()
{
    ControlElement::Reset();
    TimePointInterface::Reset();
    ResetColor();
    ResetExtSym();
    ResetOrnamentAccid();
    ResetPlacement();
    ResetMordentLog();
}

wchar_t Mordent::GetMordentGlyph() const
{
    // If there is glyph.num, prioritize it
    if (HasGlyphNum()) {
        wchar_t code = GetGlyphNum();
        if (NULL != Resources::GetGlyph(code)) return code;
    }
    // If there is glyph.name (second priority)
    else if (HasGlyphName()) {
        wchar_t code = Resources::GetGlyphCode(GetGlyphName());
        if (NULL != Resources::GetGlyph(code)) return code;
    }

    // Handle glyph based on other attributes
    if (GetLong() == BOOLEAN_true) {
        return GetForm() == mordentLog_FORM_upper ? SMUFL_E56E_ornamentTremblement
                                                  : SMUFL_E5BD_ornamentPrecompTrillWithMordent;
    }
    return GetForm() == mordentLog_FORM_upper ? SMUFL_E56C_ornamentShortTrill : SMUFL_E56D_ornamentMordent;
}

//----------------------------------------------------------------------------
// Mordent functor methods
//----------------------------------------------------------------------------

} // namespace vrv
