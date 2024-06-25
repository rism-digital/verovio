/////////////////////////////////////////////////////////////////////////////
// Name:        adjustneumexfunctor.cpp
// Author:      Laurent Pugin
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustneumexfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "layer.h"
#include "score.h"
#include "staff.h"
#include "syl.h"
#include "syllable.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustNeumeXFunctor
//----------------------------------------------------------------------------

AdjustNeumeXFunctor::AdjustNeumeXFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode AdjustNeumeXFunctor::VisitLayer(Layer *layer)
{
    m_minPos = VRV_UNSET;

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustNeumeXFunctor::VisitLayerEnd(Layer *layer)
{
    // Alignment *alignment = m_rightBarline->GetAlignment();
    Measure *measure = vrv_cast<Measure *>(layer->GetFirstAncestor(MEASURE));
    assert(measure);
    Alignment *alignment = measure->m_measureAligner.GetRightAlignment();
    assert(alignment);

    int selfLeft = alignment->GetXRel();
    if (selfLeft < m_minPos) {
        const int adjust = m_minPos - selfLeft;
        alignment->SetXRel(alignment->GetXRel() + adjust);
    }

    m_minPos = VRV_UNSET;

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustNeumeXFunctor::VisitNeume(Neume *neume)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustNeumeXFunctor::VisitStaff(Staff *staff)
{
    if (!staff->IsNeume()) return FUNCTOR_SIBLINGS;

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustNeumeXFunctor::VisitSyl(Syl *syl)
{
    Alignment *alignment = syl->GetAlignment();

    int selfLeft = syl->GetContentLeft();
    if (selfLeft < m_minPos) {
        const int adjust = m_minPos - selfLeft;
        alignment->SetXRel(alignment->GetXRel() + adjust);
    }

    m_minPos = syl->GetContentRight();

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
