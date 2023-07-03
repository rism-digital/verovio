/////////////////////////////////////////////////////////////////////////////
// Name:        adjustyposfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustyposfunctor.h"

//----------------------------------------------------------------------------

#include "beamspan.h"
#include "calcalignmentpitchposfunctor.h"
#include "calcstemfunctor.h"
#include "div.h"
#include "system.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustYPosFunctor
//----------------------------------------------------------------------------

AdjustYPosFunctor::AdjustYPosFunctor(Doc *doc) : DocFunctor(doc)
{
    m_cumulatedShift = 0;
}

FunctorCode AdjustYPosFunctor::VisitDiv(Div *div)
{
    div->AdjustRunningElementYPos();

    return FUNCTOR_SIBLINGS;
}

FunctorCode AdjustYPosFunctor::VisitStaffAlignment(StaffAlignment *staffAlignment)
{
    const int defaultSpacing = staffAlignment->GetMinimumSpacing(m_doc);
    int minSpacing = staffAlignment->CalcMinimumRequiredSpacing(m_doc);
    minSpacing = std::max(staffAlignment->GetRequestedSpacing(), minSpacing);

    if (minSpacing > defaultSpacing) {
        m_cumulatedShift += minSpacing - defaultSpacing;
    }

    staffAlignment->SetYRel(staffAlignment->GetYRel() - m_cumulatedShift);

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustYPosFunctor::VisitSystem(System *system)
{
    // We need to call this explicitly because changing the YRel of the StaffAligner (below in the functor)
    // will not trigger it
    system->ResetCachedDrawingY();

    m_cumulatedShift = 0;

    system->m_systemAligner.Process(*this);

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// AdjustCrossStaffYPosFunctor
//----------------------------------------------------------------------------

AdjustCrossStaffYPosFunctor::AdjustCrossStaffYPosFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode AdjustCrossStaffYPosFunctor::VisitChord(Chord *chord)
{
    if (!chord->HasCrossStaff()) return FUNCTOR_SIBLINGS;

    // For cross staff chords we need to re-calculate the stem because the staff position might have changed
    CalcAlignmentPitchPosFunctor calcAlignmentPitchPos(m_doc);
    chord->Process(calcAlignmentPitchPos);

    CalcStemFunctor calcStem(m_doc);
    chord->Process(calcStem);

    return FUNCTOR_SIBLINGS;
}

FunctorCode AdjustCrossStaffYPosFunctor::VisitSystem(System *system)
{
    ArrayOfObjects *drawingList = system->GetDrawingList();
    for (auto &item : *drawingList) {
        if (item->Is(BEAMSPAN)) {
            // Here we could check that the beamSpan is actually cross-staff. Otherwise doing this is pointless
            BeamSpan *beamSpan = vrv_cast<BeamSpan *>(item);
            assert(beamSpan);
            BeamSpanSegment *segment = beamSpan->GetSegmentForSystem(system);
            if (segment) {
                segment->CalcBeam(segment->GetLayer(), segment->GetStaff(), m_doc, beamSpan, beamSpan->m_drawingPlace);
            }
        }
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
