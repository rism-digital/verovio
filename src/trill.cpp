/////////////////////////////////////////////////////////////////////////////
// Name:        trill.cpp
// Author:      Klaus Rettinghaus
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "trill.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "smufl.h"
#include "verticalaligner.h"

namespace vrv {

//----------------------------------------------------------------------------
// Trill
//----------------------------------------------------------------------------

static const ClassRegistrar<Trill> s_factory("trill", TRILL);

Trill::Trill()
    : ControlElement(TRILL, "trill-")
    , TimeSpanningInterface()
    , AttColor()
    , AttExtender()
    , AttExtSym()
    , AttLineRend()
    , AttNNumberLike()
    , AttOrnamentAccid()
    , AttPlacementRelStaff()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_EXTENDER);
    RegisterAttClass(ATT_EXTSYM);
    RegisterAttClass(ATT_LINEREND);
    RegisterAttClass(ATT_NNUMBERLIKE);
    RegisterAttClass(ATT_ORNAMENTACCID);
    RegisterAttClass(ATT_PLACEMENTRELSTAFF);

    Reset();
}

Trill::~Trill() {}

void Trill::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    ResetColor();
    ResetExtender();
    ResetExtSym();
    ResetLineRend();
    ResetNNumberLike();
    ResetOrnamentAccid();
    ResetPlacementRelStaff();
}

wchar_t Trill::GetTrillGlyph() const
{
    // If there is glyph.num, return glyph based on it
    if (HasGlyphNum()) {
        wchar_t code = GetGlyphNum();
        if (NULL != Resources::GetGlyph(code)) return code;
    }
    // If there is glyph.name (second priority)
    else if (HasGlyphName()) {
        wchar_t code = Resources::GetGlyphCode(GetGlyphName());
        if (NULL != Resources::GetGlyph(code)) return code;
    }

    // return default glyph for trill
    return SMUFL_E566_ornamentTrill;
}

//----------------------------------------------------------------------------
// Trill functor methods
//----------------------------------------------------------------------------

} // namespace vrv
