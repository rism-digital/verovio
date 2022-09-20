/////////////////////////////////////////////////////////////////////////////
// Name:        caesura.cpp
// Author:      Klaus Rettinghaus, Thomas Weber
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "caesura.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "doc.h"
#include "smufl.h"
#include "verticalaligner.h"

namespace vrv {

//----------------------------------------------------------------------------
// Caesura
//----------------------------------------------------------------------------

static const ClassRegistrar<Caesura> s_factory("caesura", CAESURA);

Caesura::Caesura()
    : ControlElement(CAESURA, "caesura-"), TimePointInterface(), AttColor(), AttExtSym(), AttPlacementRelStaff()
{
    this->RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_EXTSYM);
    this->RegisterAttClass(ATT_PLACEMENTRELSTAFF);

    this->Reset();
}

Caesura::~Caesura() {}

void Caesura::Reset()
{
    ControlElement::Reset();
    TimePointInterface::Reset();
    this->ResetColor();
    this->ResetPlacementRelStaff();
}

char32_t Caesura::GetCaesuraGlyph() const
{
    const Resources *resources = this->GetDocResources();
    if (!resources) return 0;

    // If there is glyph.num, prioritize it
    if (this->HasGlyphNum()) {
        char32_t code = this->GetGlyphNum();
        if (NULL != resources->GetGlyph(code)) return code;
    }
    // If there is glyph.name (second priority)
    else if (this->HasGlyphName()) {
        char32_t code = resources->GetGlyphCode(this->GetGlyphName());
        if (NULL != resources->GetGlyph(code)) return code;
    }

    // return standard glyph
    return SMUFL_E4D1_caesura;
}

} // namespace vrv
