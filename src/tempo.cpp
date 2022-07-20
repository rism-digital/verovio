/////////////////////////////////////////////////////////////////////////////
// Name:        tempo.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tempo.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "editorial.h"
#include "functorparams.h"
#include "measure.h"
#include "staff.h"
#include "system.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Tempo
//----------------------------------------------------------------------------

static const ClassRegistrar<Tempo> s_factory("tempo", TEMPO);

Tempo::Tempo()
    : ControlElement(TEMPO, "tempo-"), TextDirInterface(), TimePointInterface(), AttLang(), AttMidiTempo(), AttMmTempo()
{
    this->RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    this->RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    this->RegisterAttClass(ATT_LANG);
    this->RegisterAttClass(ATT_MIDITEMPO);
    this->RegisterAttClass(ATT_MMTEMPO);

    this->Reset();
}

Tempo::~Tempo() {}

void Tempo::Reset()
{
    ControlElement::Reset();
    TextDirInterface::Reset();
    TimePointInterface::Reset();
    this->ResetLang();
    this->ResetMidiTempo();
    this->ResetMmTempo();
}

bool Tempo::IsSupportedChild(Object *child)
{
    if (child->Is({ LB, REND, TEXT })) {
        assert(dynamic_cast<TextElement *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

int Tempo::GetDrawingXRelativeToStaff(int staffN) const
{
    int m_relativeX = 0;
    if (m_drawingXRels.find(staffN) != m_drawingXRels.end()) {
        m_relativeX = m_drawingXRels.at(staffN);
    }

    return this->GetStart()->GetDrawingX() + m_relativeX;
}

int Tempo::AdjustTempo(FunctorParams *functorParams)
{
    AdjustTempoParams *params = vrv_params_cast<AdjustTempoParams *>(functorParams);
    assert(params);

    // Get all the positioners for this object - all of them (all staves) because we can have different staff sizes
    ArrayOfFloatingPositioners positioners;
    params->m_systemAligner->FindAllPositionerPointingTo(&positioners, this);

    if (positioners.empty()) {
        return FUNCTOR_SIBLINGS;
    }

    Measure *measure = vrv_cast<Measure *>(this->GetFirstAncestor(MEASURE));
    MeasureAlignerTypeComparison alignmentComparison(ALIGNMENT_SCOREDEF_METERSIG);
    Alignment *pos
        = dynamic_cast<Alignment *>(measure->m_measureAligner.FindDescendantByComparison(&alignmentComparison, 1));

    for (auto positioner : positioners) {
        int left, right;
        int start = this->GetStart()->GetDrawingX();
        const int staffN = positioner->GetAlignment()->GetStaff()->GetN();
        if (!this->HasStartid() && (this->GetTstamp() <= 1) && pos) {
            left = measure->GetDrawingX() + pos->GetXRel();
        }
        else {
            Alignment *align = this->GetStart()->GetAlignment();
            align->GetLeftRight(staffN, left, right);
        }

        if (std::abs(left) != std::abs(VRV_UNSET)) {
            m_drawingXRels[staffN] = left - start;
        }
    }

    return FUNCTOR_CONTINUE;
}

int Tempo::ResetData(FunctorParams *functorParams)
{
    // Call parent one too
    ControlElement::ResetData(functorParams);

    m_drawingXRels.clear();

    return FUNCTOR_CONTINUE;
}

int Tempo::InitMaxMeasureDuration(FunctorParams *functorParams)
{
    InitMaxMeasureDurationParams *params = vrv_params_cast<InitMaxMeasureDurationParams *>(functorParams);
    assert(params);

    if (this->HasMidiBpm()) {
        params->m_currentTempo = this->GetMidiBpm();
    }
    else if (this->HasMm()) {
        double mm = this->GetMm();
        int mmUnit = 4;
        if (this->HasMmUnit() && (this->GetMmUnit() > DURATION_breve)) {
            mmUnit = pow(2, (int)this->GetMmUnit() - 2);
        }
        if (this->HasMmDots()) {
            mmUnit = 2 * mmUnit - (mmUnit / pow(2, this->GetMmDots()));
        }
        if (mmUnit > 0) params->m_currentTempo = mm * 4.0 / mmUnit;
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
