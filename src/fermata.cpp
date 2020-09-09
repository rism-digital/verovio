/////////////////////////////////////////////////////////////////////////////
// Name:        fermata.cpp
// Author:      Klaus Rettinghaus
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "fermata.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "smufl.h"
#include "verticalaligner.h"

namespace vrv {

//----------------------------------------------------------------------------
// Fermata
//----------------------------------------------------------------------------

Fermata::Fermata()
    : ControlElement("fermata-"), TimePointInterface(), AttColor(), AttExtSym(), AttFermataVis(), AttPlacement()
{
    RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_EXTSYM);
    RegisterAttClass(ATT_FERMATAVIS);
    RegisterAttClass(ATT_PLACEMENT);

    Reset();
}

Fermata::~Fermata() {}

void Fermata::Reset()
{
    ControlElement::Reset();
    TimePointInterface::Reset();
    ResetColor();
    ResetExtSym();
    ResetFermataVis();
    ResetPlacement();
}

void Fermata::ConvertFromAnalyticalMarkup(
    AttFermataPresent *fermataPresent, const std::string &uuid, ConvertMarkupAnalyticalParams *params)
{
    this->SetPlace(Att::StaffrelBasicToStaffrel(fermataPresent->GetFermata()));
    if (params->m_permanent) {
        fermataPresent->ResetFermataPresent();
    }
    else {
        this->IsAttribute(true);
    }
    this->SetStartid("#" + uuid);
    params->m_controlEvents.push_back(this);
}

wchar_t Fermata::GetFermataGlyph() const
{
    // If there is glyph.num, prioritize it, otherwise check other attributes
    if (HasGlyphNum()) {
        wchar_t code = GetGlyphNum();
        if (NULL != Resources::GetGlyph(code)) return code;
    }

    // check for shape
    if (GetShape() == fermataVis_SHAPE_angular) {
        if (GetForm() == fermataVis_FORM_inv || (GetPlace() == STAFFREL_below && !(GetForm() == fermataVis_FORM_norm)))
            return SMUFL_E4C5_fermataShortBelow;
        return SMUFL_E4C4_fermataShortAbove;
    }
    else if (GetShape() == fermataVis_SHAPE_square) {
        if (GetForm() == fermataVis_FORM_inv || (GetPlace() == STAFFREL_below && !(GetForm() == fermataVis_FORM_norm)))
            return SMUFL_E4C7_fermataLongBelow;
        return SMUFL_E4C6_fermataLongAbove;
    }
    else if (GetForm() == fermataVis_FORM_inv || (GetPlace() == STAFFREL_below && !(GetForm() == fermataVis_FORM_norm)))
        return SMUFL_E4C1_fermataBelow;

    // If no other attributes match, return default one (fermataAbove)
    return SMUFL_E4C0_fermataAbove;
}

//----------------------------------------------------------------------------
// Fermata functor methods
//----------------------------------------------------------------------------

} // namespace vrv
