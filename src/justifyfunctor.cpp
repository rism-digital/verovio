/////////////////////////////////////////////////////////////////////////////
// Name:        justifyfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "justifyfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "section.h"
#include "system.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// JustifyXFunctor
//----------------------------------------------------------------------------

JustifyXFunctor::JustifyXFunctor(Doc *doc) : DocFunctor(doc)
{
    m_measureXRel = 0;
    m_justifiableRatio = 1.0;
    m_leftBarLineX = 0;
    m_rightBarLineX = 0;
    m_systemFullWidth = 0;
    m_applySectionRestartShift = false;
}

FunctorCode JustifyXFunctor::VisitAlignment(Alignment *alignment)
{
    const AlignmentType alignmentType = alignment->GetType();
    if (alignmentType <= ALIGNMENT_MEASURE_LEFT_BARLINE) {
        // Nothing to do for all left scoreDef elements and the left barline
    }
    else if (alignmentType < ALIGNMENT_MEASURE_RIGHT_BARLINE) {
        // All elements up to the next barline, move them but also take into account the leftBarlineX
        alignment->SetXRel(ceil(((alignment->GetXRel() - m_leftBarLineX) * m_justifiableRatio) + m_leftBarLineX));
    }
    else {
        // Now move the right barline and all right scoreDef elements
        int shift = alignment->GetXRel() - m_rightBarLineX;
        alignment->SetXRel(ceil(((m_rightBarLineX - m_leftBarLineX) * m_justifiableRatio) + m_leftBarLineX + shift));
    }

    // Finally, when reaching the end of the measure, update the measureXRel for the next measure
    if (alignmentType == ALIGNMENT_MEASURE_END) {
        m_measureXRel += alignment->GetXRel();
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode JustifyXFunctor::VisitMeasure(Measure *measure)
{
    if (m_applySectionRestartShift) {
        m_measureXRel += measure->GetSectionRestartShift(m_doc);
        m_applySectionRestartShift = false;
    }

    if (m_measureXRel > 0) {
        measure->SetDrawingXRel(m_measureXRel);
    }
    else {
        m_measureXRel = measure->GetDrawingXRel();
    }

    measure->m_measureAligner.Process(*this);

    return FUNCTOR_SIBLINGS;
}

FunctorCode JustifyXFunctor::VisitMeasureAligner(MeasureAligner *measureAligner)
{
    m_leftBarLineX = measureAligner->GetLeftBarLineAlignment()->GetXRel();
    m_rightBarLineX = measureAligner->GetRightBarLineAlignment()->GetXRel();

    return FUNCTOR_CONTINUE;
}

FunctorCode JustifyXFunctor::VisitScoreDef(ScoreDef *scoreDef)
{
    if (scoreDef->GetDrawingLabelsWidth() > 0) {
        m_measureXRel += scoreDef->GetDrawingLabelsWidth();
        m_applySectionRestartShift = false;
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode JustifyXFunctor::VisitSection(Section *section)
{
    if (section->GetRestart() == BOOLEAN_true) {
        m_applySectionRestartShift = true;
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode JustifyXFunctor::VisitSystem(System *system)
{
    m_measureXRel = 0;
    int margins = system->m_systemLeftMar + system->m_systemRightMar;
    int nonJustifiableWidth = margins
        + (system->m_drawingTotalWidth - system->m_drawingJustifiableWidth); // m_drawingTotalWidth includes the labels
    m_justifiableRatio
        = (double)(m_systemFullWidth - nonJustifiableWidth) / ((double)system->m_drawingJustifiableWidth);

    if (m_justifiableRatio < 0.8) {
        // Arbitrary value for avoiding over-compressed justification
        LogWarning("Justification is highly compressed (ratio smaller than 0.8: %lf)", m_justifiableRatio);
        LogWarning("\tSystem full width: %d", m_systemFullWidth);
        LogWarning("\tNon-justifiable width: %d", nonJustifiableWidth);
        LogWarning("\tDrawing justifiable width: %d", system->m_drawingJustifiableWidth);
    }

    // Check if we are on the last system of an mdiv.
    // Do not justify it if the non-justified width is less than a specified percent.
    if (system->IsLastOfMdiv() || system->IsLastOfSelection()) {
        double minLastJust = m_doc->GetOptions()->m_minLastJustification.GetValue();
        if ((minLastJust > 0.0) && (m_justifiableRatio > (1.0 / minLastJust))) {
            return FUNCTOR_SIBLINGS;
        }
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// JustifyYFunctor
//----------------------------------------------------------------------------

JustifyYFunctor::JustifyYFunctor(Doc *doc) : DocFunctor(doc)
{
    m_cumulatedShift = 0;
    m_relativeShift = 0;
    m_spaceToDistribute = 0;
    m_justificationSum = 0.0;
}

FunctorCode JustifyYFunctor::VisitStaffAlignment(StaffAlignment *staffAlignment)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode JustifyYFunctor::VisitSystem(System *system)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
