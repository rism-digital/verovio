/////////////////////////////////////////////////////////////////////////////
// Name:        mordent.cpp
// Author:      Klaus Rettinghaus
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "mordent.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "horizontalaligner.h"
#include "resources.h"
#include "smufl.h"

namespace vrv {

//----------------------------------------------------------------------------
// Mordent
//----------------------------------------------------------------------------

static const ClassRegistrar<Mordent> s_factory("mordent", MORDENT);

Mordent::Mordent()
    : ControlElement(MORDENT, "mordent-")
    , TimePointInterface()
    , AttColor()
    , AttExtSym()
    , AttOrnamentAccid()
    , AttPlacementRelStaff()
    , AttMordentLog()
{
    this->RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_EXTSYM);
    this->RegisterAttClass(ATT_ORNAMENTACCID);
    this->RegisterAttClass(ATT_PLACEMENTRELSTAFF);
    this->RegisterAttClass(ATT_MORDENTLOG);

    this->Reset();
}

Mordent::~Mordent() {}

void Mordent::Reset()
{
    ControlElement::Reset();
    TimePointInterface::Reset();
    this->ResetColor();
    this->ResetExtSym();
    this->ResetOrnamentAccid();
    this->ResetPlacementRelStaff();
    this->ResetMordentLog();
}

char32_t Mordent::GetMordentGlyph() const
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

    // Handle glyph based on other attributes
    if (this->GetLong() == BOOLEAN_true) {
        return this->GetForm() == mordentLog_FORM_upper ? SMUFL_E56E_ornamentTremblement
                                                        : SMUFL_E5BD_ornamentPrecompTrillWithMordent;
    }
    return this->GetForm() == mordentLog_FORM_upper ? SMUFL_E56C_ornamentShortTrill : SMUFL_E56D_ornamentMordent;
}

//----------------------------------------------------------------------------
// Mordent functor methods
//----------------------------------------------------------------------------

} // namespace vrv
