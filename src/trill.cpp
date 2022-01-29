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
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_EXTENDER);
    this->RegisterAttClass(ATT_EXTSYM);
    this->RegisterAttClass(ATT_LINEREND);
    this->RegisterAttClass(ATT_NNUMBERLIKE);
    this->RegisterAttClass(ATT_ORNAMENTACCID);
    this->RegisterAttClass(ATT_PLACEMENTRELSTAFF);

    this->Reset();
}

Trill::~Trill() {}

void Trill::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    this->ResetColor();
    this->ResetExtender();
    this->ResetExtSym();
    this->ResetLineRend();
    this->ResetNNumberLike();
    this->ResetOrnamentAccid();
    this->ResetPlacementRelStaff();
}

wchar_t Trill::GetTrillGlyph() const
{
    // If there is glyph.num, return glyph based on it
    if (this->HasGlyphNum()) {
        wchar_t code = this->GetGlyphNum();
        if (NULL != Resources::GetGlyph(code)) return code;
    }
    // If there is glyph.name (second priority)
    else if (this->HasGlyphName()) {
        wchar_t code = Resources::GetGlyphCode(this->GetGlyphName());
        if (NULL != Resources::GetGlyph(code)) return code;
    }

    // return default glyph for trill
    return SMUFL_E566_ornamentTrill;
}

//----------------------------------------------------------------------------
// Trill functor methods
//----------------------------------------------------------------------------

} // namespace vrv
