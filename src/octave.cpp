/////////////////////////////////////////////////////////////////////////////
// Name:        octave.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "octave.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "doc.h"
#include "functor.h"
#include "smufl.h"
#include "verticalaligner.h"

namespace vrv {

//----------------------------------------------------------------------------
// Octave
//----------------------------------------------------------------------------

static const ClassRegistrar<Octave> s_factory("octave", OCTAVE);

Octave::Octave()
    : ControlElement(OCTAVE)
    , TimeSpanningInterface()
    , AttExtender()
    , AttLineRend()
    , AttLineRendBase()
    , AttNNumberLike()
    , AttOctaveDisplacement()
{
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_EXTENDER);
    this->RegisterAttClass(ATT_LINEREND);
    this->RegisterAttClass(ATT_LINERENDBASE);
    this->RegisterAttClass(ATT_NNUMBERLIKE);
    this->RegisterAttClass(ATT_OCTAVEDISPLACEMENT);

    this->Reset();
}

Octave::~Octave() {}

void Octave::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    this->ResetExtender();
    this->ResetLineRend();
    this->ResetLineRendBase();
    this->ResetNNumberLike();
    this->ResetOctaveDisplacement();

    this->ResetDrawingExtenderX();
}

void Octave::ResetDrawingExtenderX()
{
    m_drawingExtenderX.clear();
}

void Octave::SetDrawingExtenderX(int left, int right)
{
    const FloatingPositioner *positioner = this->GetCurrentFloatingPositioner();
    if (positioner) {
        m_drawingExtenderX[positioner] = { left, right };
    }
}

char32_t Octave::GetOctaveGlyph(bool withAltaBassa) const
{
    const data_OCTAVE_DIS dis = this->GetDis();
    char32_t code = SMUFL_E510_ottava;
    if (this->GetDisPlace() == STAFFREL_basic_above) {
        switch (dis) {
            case OCTAVE_DIS_8: {
                code = withAltaBassa ? SMUFL_E511_ottavaAlta : SMUFL_E510_ottava;
                break;
            }
            case OCTAVE_DIS_15: {
                code = withAltaBassa ? SMUFL_E515_quindicesimaAlta : SMUFL_E514_quindicesima;
                break;
            }
            case OCTAVE_DIS_22: {
                code = withAltaBassa ? SMUFL_E518_ventiduesimaAlta : SMUFL_E517_ventiduesima;
                break;
            }
            default: break;
        }
    }
    else {
        switch (dis) {
            case OCTAVE_DIS_8: {
                code = withAltaBassa ? SMUFL_E51C_ottavaBassaVb : SMUFL_E510_ottava;
                break;
            }
            case OCTAVE_DIS_15: {
                code = withAltaBassa ? SMUFL_E51D_quindicesimaBassaMb : SMUFL_E514_quindicesima;
                break;
            }
            case OCTAVE_DIS_22: {
                code = withAltaBassa ? SMUFL_E51E_ventiduesimaBassaMb : SMUFL_E517_ventiduesima;
                break;
            }
            default: break;
        }
    }
    return code;
}

int Octave::GetLineWidth(const Doc *doc, int unit) const
{
    int lineWidth = doc->GetOptions()->m_octaveLineThickness.GetValue() * unit;
    if (this->HasLwidth()) {
        if (this->GetLwidth().GetType() == LINEWIDTHTYPE_lineWidthTerm) {
            switch (this->GetLwidth().GetLineWithTerm()) {
                case LINEWIDTHTERM_narrow: lineWidth *= LINEWIDTHTERM_factor_narrow; break;
                case LINEWIDTHTERM_medium: lineWidth *= LINEWIDTHTERM_factor_medium; break;
                case LINEWIDTHTERM_wide: lineWidth *= LINEWIDTHTERM_factor_wide; break;
                default: break;
            }
        }
        else if (this->GetLwidth().GetType() == LINEWIDTHTYPE_measurementunsigned) {
            if (this->GetLwidth().GetMeasurementunsigned().GetType() == MEASUREMENTTYPE_px) {
                lineWidth = this->GetLwidth().GetMeasurementunsigned().GetPx();
            }
            else {
                lineWidth = this->GetLwidth().GetMeasurementunsigned().GetVu() * unit;
            }
        }
    }
    return lineWidth;
}

std::pair<int, bool> Octave::GetVerticalContentBoundaryRel(const Doc *doc, const FloatingPositioner *positioner,
    const BoundingBox *horizOverlappingBBox, bool contentTop) const
{
    assert(positioner);

    // Check if we use the extender line for refined vertical layout
    bool useExtenderLine = true;
    const StaffAlignment *staffAlignment = positioner->GetAlignment();
    if (m_drawingExtenderX.find(positioner) == m_drawingExtenderX.end()) {
        useExtenderLine = false;
    }
    else {
        const std::pair<int, int> extenderX = m_drawingExtenderX.at(positioner);
        if ((horizOverlappingBBox->GetContentLeft() < extenderX.first)
            || (horizOverlappingBBox->GetContentRight() > extenderX.second)) {
            useExtenderLine = false;
        }
    }

    if (useExtenderLine) {
        // Calculate the content boundary based on the extender line
        const int unit = doc->GetDrawingUnit(staffAlignment->GetStaffSize());
        const int lineWidth = this->GetLineWidth(doc, unit);
        const int contentBoundary = contentTop ? lineWidth : -lineWidth;
        return { contentBoundary, true };
    }
    else {
        // Calculate the content boundary based on the full bounding box (as usual)
        return FloatingObject::GetVerticalContentBoundaryRel(doc, positioner, horizOverlappingBBox, contentTop);
    }
}

//----------------------------------------------------------------------------
// Octave functor methods
//----------------------------------------------------------------------------

FunctorCode Octave::Accept(Functor &functor)
{
    return functor.VisitOctave(this);
}

FunctorCode Octave::Accept(ConstFunctor &functor) const
{
    return functor.VisitOctave(this);
}

FunctorCode Octave::AcceptEnd(Functor &functor)
{
    return functor.VisitOctaveEnd(this);
}

FunctorCode Octave::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitOctaveEnd(this);
}

} // namespace vrv
