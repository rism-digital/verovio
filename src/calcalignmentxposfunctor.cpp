/////////////////////////////////////////////////////////////////////////////
// Name:        calcalignmentxposfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "calcalignmentxposfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "system.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// CalcAlignmentXPosFunctor
//----------------------------------------------------------------------------

CalcAlignmentXPosFunctor::CalcAlignmentXPosFunctor(Doc *doc) : DocFunctor(doc)
{
    m_previousTime = 0;
    m_previousXRel = 0;
    m_longestActualDur = DURATION_NONE;
    m_estimatedJustificationRatio = 1.0;
    m_lastNonTimestamp = NULL;
    m_measureAligner = NULL;
}

FunctorCode CalcAlignmentXPosFunctor::VisitAlignment(Alignment *alignment)
{
    // Do not set an x pos for anything before the barline (including it)
    if (alignment->GetType() <= ALIGNMENT_MEASURE_LEFT_BARLINE) return FUNCTOR_CONTINUE;

    int intervalXRel = 0;
    Fraction intervalTime = alignment->GetTime() - m_previousTime;

    if (alignment->GetType() > ALIGNMENT_MEASURE_RIGHT_BARLINE) {
        intervalTime = 0;
    }

    // Do not move aligners that are only time-stamps at this stage but add it to the pending list
    if (alignment->HasTimestampOnly()) {
        m_timestamps.push_back(alignment);
        return FUNCTOR_CONTINUE;
    }

    if (intervalTime > 0) {
        intervalXRel = Alignment::HorizontalSpaceForDuration(intervalTime, m_longestActualDur,
            m_doc->GetOptions()->m_spacingLinear.GetValue(), m_doc->GetOptions()->m_spacingNonLinear.GetValue());
        // LogDebug("CalcAlignmentXPos: intervalTime=%.2f intervalXRel=%d", intervalTime, intervalXRel);
    }

    const MapOfIntGraceAligners &graceAligners = alignment->GetGraceAligners();
    for (const auto &[key, value] : graceAligners) {
        value->SetGraceAlignmentXPos(m_doc);
    }

    alignment->SetXRel(m_previousXRel + intervalXRel * DEFINITION_FACTOR * m_estimatedJustificationRatio);
    m_previousTime = alignment->GetTime();
    m_previousXRel = alignment->GetXRel();

    // This is an alignment which is not timestamp only. If we have a list of pending timestamp
    // alignments, then we now need to move them appropriately
    if (!m_timestamps.empty() && m_lastNonTimestamp) {
        int startXRel = m_lastNonTimestamp->GetXRel();
        Fraction startTime = m_lastNonTimestamp->GetTime();
        Fraction endTime = alignment->GetTime();
        // We have timestamp alignments between the left barline and the first beat. We need
        // to use the MeasureAligner::m_initialTstampDur to calculate the time (percentage) position
        if (m_lastNonTimestamp->GetType() == ALIGNMENT_MEASURE_LEFT_BARLINE) {
            startTime = m_measureAligner->GetInitialTstampDur();
        }
        // The duration since the last alignment and the current one
        Fraction duration = endTime - startTime;
        int space = alignment->GetXRel() - m_lastNonTimestamp->GetXRel();
        // For each timestamp alignment, move them proportionally to the space we currently have
        for (auto &tsAlignment : m_timestamps) {
            // Avoid division by zero (nothing to move with the alignment anyway
            if (duration == 0) break;
            double percent = ((tsAlignment->GetTime() - startTime) / duration).ToDouble();
            tsAlignment->SetXRel(startXRel + space * percent);
        }
        m_timestamps.clear();
    }

    // Do not use clef change and gracenote alignment as reference since these are not aligned at this stage
    if (!alignment->IsOfType({ ALIGNMENT_CLEF, ALIGNMENT_GRACENOTE })) m_lastNonTimestamp = alignment;

    return FUNCTOR_CONTINUE;
}

FunctorCode CalcAlignmentXPosFunctor::VisitMeasure(Measure *measure)
{
    // We start a new Measure
    // Reset the previous time position and x_rel to 0;
    m_previousTime = 0;
    // We un-measured music we never have a left barline, so do not add a default space
    m_previousXRel = (measure->IsMeasuredMusic()) ? m_doc->GetDrawingUnit(100) : 0;

    measure->m_measureAligner.Process(*this);

    return FUNCTOR_SIBLINGS;
}

FunctorCode CalcAlignmentXPosFunctor::VisitMeasureAligner(MeasureAligner *measureAligner)
{

    m_lastNonTimestamp = measureAligner->GetLeftBarLineAlignment();
    m_measureAligner = measureAligner;

    return FUNCTOR_CONTINUE;
}

FunctorCode CalcAlignmentXPosFunctor::VisitSystem(System *system)
{
    const double ratio = system->EstimateJustificationRatio(m_doc);
    if ((!system->IsLastOfMdiv() && !system->IsLastOfSelection()) || (ratio < m_estimatedJustificationRatio)) {
        m_estimatedJustificationRatio = ratio;
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
