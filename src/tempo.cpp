/////////////////////////////////////////////////////////////////////////////
// Name:        tempo.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tempo.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "controlelement.h"
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
    RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    RegisterAttClass(ATT_LANG);
    RegisterAttClass(ATT_MIDITEMPO);
    RegisterAttClass(ATT_MMTEMPO);

    Reset();
}

Tempo::~Tempo() {}

void Tempo::Reset()
{
    ControlElement::Reset();
    TextDirInterface::Reset();
    TimePointInterface::Reset();
    ResetLang();
    ResetMidiTempo();
    ResetMmTempo();
}

bool Tempo::IsSupportedChild(Object *child)
{
    if (child->Is({ REND, TEXT })) {
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

int Tempo::GetDrawingXRelativeToStaff(int staffN)
{
    int m_relativeX = 0;
    if (m_drawingXRels.find(staffN) != m_drawingXRels.end()) {
        m_relativeX = m_drawingXRels.at(staffN);
    }

    return GetStart()->GetDrawingX() + m_relativeX;
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

    Measure *measure = vrv_cast<Measure *>(GetFirstAncestor(MEASURE));
    MeasureAlignerTypeComparison alignmentComparison(ALIGNMENT_SCOREDEF_METERSIG);
    Alignment *pos
        = dynamic_cast<Alignment *>(measure->m_measureAligner.FindDescendantByComparison(&alignmentComparison, 1));

    for (auto positioner : positioners) {
        int left, right;
        int start = GetStart()->GetDrawingX();
        const int staffN = positioner->GetAlignment()->GetStaff()->GetN();
        if (!HasStartid() && (GetTstamp() <= 1) && pos) {
            left = measure->GetDrawingX() + pos->GetXRel();
        }
        else {
            Alignment *align = GetStart()->GetAlignment();
            align->GetLeftRight(staffN, left, right);
        }

        if (std::abs(left) != std::abs(VRV_UNSET)) {
            m_drawingXRels[staffN] = left - start;
        }
    }

    return FUNCTOR_CONTINUE;
}

int Tempo::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    ControlElement::ResetDrawing(functorParams);

    m_drawingXRels.clear();

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
