/////////////////////////////////////////////////////////////////////////////
// Name:        fermata.cpp
// Author:      Klaus Rettinghaus
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "fermata.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "functor.h"
#include "resources.h"
#include "smufl.h"
#include "verticalaligner.h"

namespace vrv {

//----------------------------------------------------------------------------
// Fermata
//----------------------------------------------------------------------------

static const ClassRegistrar<Fermata> s_factory("fermata", FERMATA);

Fermata::Fermata()
    : ControlElement(FERMATA)
    , TimePointInterface()
    , AttExtSymAuth()
    , AttExtSymNames()
    , AttFermataVis()
    , AttPlacementRelStaff()
{
    this->RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    this->RegisterAttClass(ATT_ENCLOSINGCHARS);
    this->RegisterAttClass(ATT_EXTSYMAUTH);
    this->RegisterAttClass(ATT_EXTSYMNAMES);
    this->RegisterAttClass(ATT_FERMATAVIS);
    this->RegisterAttClass(ATT_PLACEMENTRELSTAFF);

    this->Reset();
}

Fermata::~Fermata() {}

void Fermata::Reset()
{
    ControlElement::Reset();
    AltSymInterface::Reset();
    TimePointInterface::Reset();
    this->ResetEnclosingChars();
    this->ResetExtSymAuth();
    this->ResetExtSymNames();
    this->ResetFermataVis();
    this->ResetPlacementRelStaff();
}

char32_t Fermata::GetFermataGlyph() const
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

    // check for shape
    if (this->GetShape() == fermataVis_SHAPE_angular) {
        if (this->GetForm() == fermataVis_FORM_inv
            || (this->GetPlace() == STAFFREL_below && !(this->GetForm() == fermataVis_FORM_norm)))
            return SMUFL_E4C5_fermataShortBelow;
        return SMUFL_E4C4_fermataShortAbove;
    }
    else if (this->GetShape() == fermataVis_SHAPE_square) {
        if (this->GetForm() == fermataVis_FORM_inv
            || (this->GetPlace() == STAFFREL_below && !(this->GetForm() == fermataVis_FORM_norm)))
            return SMUFL_E4C7_fermataLongBelow;
        return SMUFL_E4C6_fermataLongAbove;
    }
    else if (this->GetForm() == fermataVis_FORM_inv
        || (this->GetPlace() == STAFFREL_below && !(this->GetForm() == fermataVis_FORM_norm)))
        return SMUFL_E4C1_fermataBelow;

    // If no other attributes match, return default one (fermataAbove)
    return SMUFL_E4C0_fermataAbove;
}

std::pair<char32_t, char32_t> Fermata::GetEnclosingGlyphs() const
{
    std::pair<char32_t, char32_t> glyphs(0, 0);
    if (this->HasEnclose()) {
        switch (this->GetEnclose()) {
            case ENCLOSURE_brack:
                glyphs = { SMUFL_E26C_accidentalBracketLeft, SMUFL_E26D_accidentalBracketRight };
                break;
            case ENCLOSURE_paren: glyphs = { SMUFL_E26A_accidentalParensLeft, SMUFL_E26B_accidentalParensRight }; break;
            default: break;
        }
    }
    return glyphs;
}

//----------------------------------------------------------------------------
// Static methods
//----------------------------------------------------------------------------

data_VERTICALALIGNMENT Fermata::GetVerticalAlignment(char32_t code)
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

FunctorCode Fermata::Accept(Functor &functor)
{
    return functor.VisitFermata(this);
}

FunctorCode Fermata::Accept(ConstFunctor &functor) const
{
    return functor.VisitFermata(this);
}

FunctorCode Fermata::AcceptEnd(Functor &functor)
{
    return functor.VisitFermataEnd(this);
}

FunctorCode Fermata::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitFermataEnd(this);
}

} // namespace vrv
