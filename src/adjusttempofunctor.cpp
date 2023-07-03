/////////////////////////////////////////////////////////////////////////////
// Name:        adjusttempofunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjusttempofunctor.h"

//----------------------------------------------------------------------------

#include "staff.h"
#include "system.h"
#include "tempo.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustTempoFunctor
//----------------------------------------------------------------------------

AdjustTempoFunctor::AdjustTempoFunctor(Doc *doc) : DocFunctor(doc)
{
    m_systemAligner = NULL;
}

FunctorCode AdjustTempoFunctor::VisitSystem(System *system)
{
    m_systemAligner = &system->m_systemAligner;

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustTempoFunctor::VisitTempo(Tempo *tempo)
{
    // Get all the positioners for this object - all of them (all staves) because we can have different staff sizes
    ArrayOfFloatingPositioners positioners;
    m_systemAligner->FindAllPositionerPointingTo(&positioners, tempo);

    if (positioners.empty()) {
        return FUNCTOR_SIBLINGS;
    }

    Measure *measure = vrv_cast<Measure *>(tempo->GetFirstAncestor(MEASURE));
    MeasureAlignerTypeComparison alignmentComparison(ALIGNMENT_SCOREDEF_METERSIG);
    Alignment *pos
        = dynamic_cast<Alignment *>(measure->m_measureAligner.FindDescendantByComparison(&alignmentComparison, 1));

    for (auto positioner : positioners) {
        int left, right;
        int start = tempo->GetStart()->GetDrawingX();
        const int staffN = positioner->GetAlignment()->GetStaff()->GetN();
        if (!tempo->HasStartid() && (tempo->GetTstamp() <= 1) && pos) {
            left = measure->GetDrawingX() + pos->GetXRel();
        }
        else {
            Alignment *align = tempo->GetStart()->GetAlignment();
            align->GetLeftRight(staffN, left, right);
        }

        if (std::abs(left) != std::abs(VRV_UNSET)) {
            tempo->SetDrawingXRelative(staffN, left - start);
        }
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
