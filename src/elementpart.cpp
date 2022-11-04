/////////////////////////////////////////////////////////////////////////////
// Name:        elementpart.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "elementpart.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "btrem.h"
#include "chord.h"
#include "doc.h"
#include "functorparams.h"
#include "layer.h"
#include "note.h"
#include "smufl.h"
#include "staff.h"
#include "tuplet.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Dots
//----------------------------------------------------------------------------

Dots::Dots() : LayerElement(DOTS, "dots-"), AttAugmentDots()
{
    this->RegisterAttClass(ATT_AUGMENTDOTS);

    this->Reset();
}

Dots::~Dots() {}

void Dots::Reset()
{
    LayerElement::Reset();
    this->ResetAugmentDots();

    m_isAdjusted = false;
    m_flagShift = 0;
    m_dotLocsByStaff.clear();
}

std::set<int> Dots::GetDotLocsForStaff(const Staff *staff) const
{
    if (m_dotLocsByStaff.find(staff) != m_dotLocsByStaff.end()) {
        return m_dotLocsByStaff.at(staff);
    }
    return {};
}

std::set<int> &Dots::ModifyDotLocsForStaff(const Staff *staff)
{
    return m_dotLocsByStaff[staff];
}

//----------------------------------------------------------------------------
// Flag
//----------------------------------------------------------------------------

Flag::Flag() : LayerElement(FLAG, "flag-")
{

    this->Reset();
}

Flag::~Flag() {}

void Flag::Reset()
{
    LayerElement::Reset();

    m_drawingNbFlags = 0;
}

char32_t Flag::GetFlagGlyph(data_STEMDIRECTION stemDir) const
{
    if (stemDir == STEMDIRECTION_up) {
        switch (m_drawingNbFlags) {
            case 1: return SMUFL_E240_flag8thUp;
            case 2: return SMUFL_E242_flag16thUp;
            case 3: return SMUFL_E244_flag32ndUp;
            case 4: return SMUFL_E246_flag64thUp;
            case 5: return SMUFL_E248_flag128thUp;
            case 6: return SMUFL_E24A_flag256thUp;
            case 7: return SMUFL_E24C_flag512thUp;
            case 8: return SMUFL_E24E_flag1024thUp;
            default: return 0;
        }
    }
    else {
        switch (m_drawingNbFlags) {
            case 1: return SMUFL_E241_flag8thDown;
            case 2: return SMUFL_E243_flag16thDown;
            case 3: return SMUFL_E245_flag32ndDown;
            case 4: return SMUFL_E247_flag64thDown;
            case 5: return SMUFL_E249_flag128thDown;
            case 6: return SMUFL_E24B_flag256thDown;
            case 7: return SMUFL_E24D_flag512thDown;
            case 8: return SMUFL_E24F_flag1024thDown;
            default: return 0;
        }
    }
}

Point Flag::GetStemUpSE(const Doc *doc, int staffSize, bool graceSize) const
{
    const char32_t code = this->GetFlagGlyph(STEMDIRECTION_up);

    return Point(0, doc->GetGlyphTop(code, staffSize, graceSize));
}

Point Flag::GetStemDownNW(const Doc *doc, int staffSize, bool graceSize) const
{
    const char32_t code = this->GetFlagGlyph(STEMDIRECTION_down);

    return Point(0, doc->GetGlyphBottom(code, staffSize, graceSize));
}

//----------------------------------------------------------------------------
// TupletBracket
//----------------------------------------------------------------------------

TupletBracket::TupletBracket() : LayerElement(TUPLET_BRACKET, "bracket-"), AttTupletVis()
{
    this->RegisterAttClass(ATT_TUPLETVIS);

    this->Reset();
}

TupletBracket::~TupletBracket() {}

void TupletBracket::Reset()
{
    LayerElement::Reset();
    this->ResetTupletVis();

    m_drawingXRelLeft = 0;
    m_drawingXRelRight = 0;
    m_drawingYRelLeft = 0;
    m_drawingYRelRight = 0;
    m_alignedNum = NULL;
}

int TupletBracket::GetDrawingXLeft() const
{
    const Tuplet *tuplet = vrv_cast<const Tuplet *>(this->GetFirstAncestor(TUPLET));
    assert(tuplet && tuplet->GetDrawingLeft());

    return tuplet->GetDrawingLeft()->GetDrawingX() + m_drawingXRelLeft;
}

int TupletBracket::GetDrawingXRight() const
{
    const Tuplet *tuplet = vrv_cast<const Tuplet *>(this->GetFirstAncestor(TUPLET));
    assert(tuplet && tuplet->GetDrawingRight());

    return tuplet->GetDrawingRight()->GetDrawingX() + m_drawingXRelRight;
}

int TupletBracket::GetDrawingYLeft() const
{
    const Tuplet *tuplet = vrv_cast<const Tuplet *>(this->GetFirstAncestor(TUPLET));
    assert(tuplet && tuplet->GetDrawingLeft());

    const Beam *beam = tuplet->GetBracketAlignedBeam();
    if (beam) {
        // Calculate the y point aligning with the beam
        int xLeft = tuplet->GetDrawingLeft()->GetDrawingX() + m_drawingXRelLeft;
        return beam->m_beamSegment.GetStartingY()
            + beam->m_beamSegment.m_beamSlope * (xLeft - beam->m_beamSegment.GetStartingX()) + this->GetDrawingYRel()
            + m_drawingYRelLeft;
    }
    else {
        return this->GetDrawingY();
    }
}

int TupletBracket::GetDrawingYRight() const
{
    const Tuplet *tuplet = vrv_cast<const Tuplet *>(this->GetFirstAncestor(TUPLET));
    assert(tuplet && tuplet->GetDrawingRight());

    const Beam *beam = tuplet->GetBracketAlignedBeam();
    if (beam) {
        // Calculate the y point aligning with the beam
        int xRight = tuplet->GetDrawingRight()->GetDrawingX() + m_drawingXRelRight;
        return beam->m_beamSegment.GetStartingY()
            + beam->m_beamSegment.m_beamSlope * (xRight - beam->m_beamSegment.GetStartingX()) + this->GetDrawingYRel()
            + m_drawingYRelRight;
    }
    else {
        return this->GetDrawingY();
    }
}

//----------------------------------------------------------------------------
// TupletNum
//----------------------------------------------------------------------------

TupletNum::TupletNum() : LayerElement(TUPLET_NUM, "num-"), AttNumberPlacement(), AttTupletVis()
{
    this->RegisterAttClass(ATT_NUMBERPLACEMENT);
    this->RegisterAttClass(ATT_TUPLETVIS);

    this->Reset();
}

TupletNum::~TupletNum() {}

void TupletNum::Reset()
{
    LayerElement::Reset();
    this->ResetNumberPlacement();
    this->ResetTupletVis();

    m_alignedBracket = NULL;
}

int TupletNum::GetDrawingYMid() const
{
    if (m_alignedBracket) {
        int yLeft = m_alignedBracket->GetDrawingYLeft();
        int yRight = m_alignedBracket->GetDrawingYRight();
        return yLeft + ((yRight - yLeft) / 2);
    }
    else {
        return this->GetDrawingY();
    }
}

int TupletNum::GetDrawingXMid(const Doc *doc) const
{
    if (m_alignedBracket) {
        int xLeft = m_alignedBracket->GetDrawingXLeft();
        int xRight = m_alignedBracket->GetDrawingXRight();
        return xLeft + ((xRight - xLeft) / 2);
    }
    else {
        const Tuplet *tuplet = vrv_cast<const Tuplet *>(this->GetFirstAncestor(TUPLET));
        assert(tuplet && tuplet->GetDrawingLeft() && tuplet->GetDrawingRight());
        int xLeft = tuplet->GetDrawingLeft()->GetDrawingX();
        int xRight = tuplet->GetDrawingRight()->GetDrawingX();
        if (doc) {
            xRight += (tuplet->GetDrawingRight()->GetDrawingRadius(doc) * 2);
        }
        if (tuplet->GetNumAlignedBeam()) {
            const Beam *beam = tuplet->GetNumAlignedBeam();
            switch (beam->m_drawingPlace) {
                case BEAMPLACE_above: xLeft += (tuplet->GetDrawingLeft()->GetDrawingRadius(doc)); break;
                case BEAMPLACE_below: xRight -= (tuplet->GetDrawingRight()->GetDrawingRadius(doc)); break;
                default: break;
            }
        }
        return xLeft + ((xRight - xLeft) / 2);
    }
}

void TupletNum::SetAlignedBracket(TupletBracket *alignedBracket)
{
    if (m_alignedBracket) m_alignedBracket->SetAlignedNum(NULL);
    m_alignedBracket = alignedBracket;
    if (m_alignedBracket) m_alignedBracket->SetAlignedNum(this);
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Dots::ResetData(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetData(functorParams);

    m_dotLocsByStaff.clear();
    m_isAdjusted = false;

    return FUNCTOR_CONTINUE;
}

int Dots::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    LayerElement::ResetHorizontalAlignment(functorParams);

    m_dotLocsByStaff.clear();

    return FUNCTOR_CONTINUE;
}

int Flag::ResetData(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetData(functorParams);

    m_drawingNbFlags = 0;

    return FUNCTOR_CONTINUE;
}

int TupletBracket::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetHorizontalAlignment(functorParams);

    m_drawingXRelLeft = 0;
    m_drawingXRelRight = 0;
    m_alignedNum = NULL;

    return FUNCTOR_CONTINUE;
}

int TupletBracket::ResetVerticalAlignment(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetVerticalAlignment(functorParams);

    m_drawingYRelLeft = 0;
    m_drawingYRelRight = 0;

    return FUNCTOR_CONTINUE;
}

int TupletNum::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetHorizontalAlignment(functorParams);

    m_alignedBracket = NULL;

    return FUNCTOR_CONTINUE;
}

int TupletNum::ResetVerticalAlignment(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetVerticalAlignment(functorParams);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
