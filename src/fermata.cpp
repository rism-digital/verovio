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

static const ClassRegistrar<Fermata> s_factory("fermata", FERMATA);

Fermata::Fermata()
    : ControlElement("fermata-"), TimePointInterface(), AttColor(), AttExtSym(), AttFermataVis(), AttPlacementRelStaff()
{
    RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_ENCLOSINGCHARS);
    RegisterAttClass(ATT_EXTSYM);
    RegisterAttClass(ATT_FERMATAVIS);
    RegisterAttClass(ATT_PLACEMENTRELSTAFF);

    Reset();
}

Fermata::~Fermata() {}

void Fermata::Reset()
{
    ControlElement::Reset();
    TimePointInterface::Reset();
    ResetColor();
    ResetEnclosingChars();
    ResetExtSym();
    ResetFermataVis();
    ResetPlacementRelStaff();
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

wchar_t Fermata::GetEnclosingGlyph(bool beforeFermata) const
{
    wchar_t glyph = 0;
    if (this->HasEnclose()) {
        switch (this->GetEnclose()) {
            case ENCLOSURE_brack:
                glyph = beforeFermata ? SMUFL_E26C_accidentalBracketLeft : SMUFL_E26D_accidentalBracketRight;
                break;
            case ENCLOSURE_paren:
                glyph = beforeFermata ? SMUFL_E26A_accidentalParensLeft : SMUFL_E26B_accidentalParensRight;
                break;
            default: break;
        }
    }
    return glyph;
}

//----------------------------------------------------------------------------
// Static methods
//----------------------------------------------------------------------------

data_VERTICALALIGNMENT Fermata::GetVerticalAlignment(wchar_t code)
{
    switch (code) {
        case SMUFL_E4C0_fermataAbove:
        case SMUFL_E4C2_fermataVeryShortAbove:
        case SMUFL_E4C4_fermataShortAbove:
        case SMUFL_E4C6_fermataLongAbove:
        case SMUFL_E4C8_fermataVeryLongAbove: return VERTICALALIGNMENT_top;

        case SMUFL_E4C1_fermataBelow:
        case SMUFL_E4C3_fermataVeryShortBelow:
        case SMUFL_E4C5_fermataShortBelow:
        case SMUFL_E4C7_fermataLongBelow:
        case SMUFL_E4C9_fermataVeryLongBelow: return VERTICALALIGNMENT_bottom;

        default: return VERTICALALIGNMENT_middle;
    }
}

//----------------------------------------------------------------------------
// Fermata functor methods
//----------------------------------------------------------------------------

} // namespace vrv
