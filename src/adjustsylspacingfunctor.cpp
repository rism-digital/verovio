/////////////////////////////////////////////////////////////////////////////
// Name:        adjustsylspacingfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustsylspacingfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "label.h"
#include "labelabbr.h"
#include "staff.h"
#include "syl.h"
#include "verse.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustSylSpacingFunctor
//----------------------------------------------------------------------------

AdjustSylSpacingFunctor::AdjustSylSpacingFunctor(Doc *doc) : DocFunctor(doc)
{
    m_previousVerse = NULL;
    m_lastSyl = NULL;
    m_previousMeasure = NULL;
    m_currentLabelAbbr = NULL;
    m_freeSpace = 0;
    m_staffSize = 100;
}

FunctorCode AdjustSylSpacingFunctor::VisitMeasureEnd(Measure *measure)
{
    // At the end of the measure - pass it along for overlapping verses
    m_previousMeasure = measure;

    // Adjust the position of the alignment according to what we have collected for this verse
    measure->m_measureAligner.AdjustProportionally(m_overlappingSyl);
    m_overlappingSyl.clear();

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustSylSpacingFunctor::VisitStaff(Staff *staff)
{
    // Set the staff size for this pass
    m_staffSize = staff->m_drawingStaffSize;

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustSylSpacingFunctor::VisitSystem(System *system)
{
    // reset it
    m_overlappingSyl.clear();
    m_previousVerse = NULL;
    m_previousMeasure = NULL;
    m_freeSpace = 0;
    m_staffSize = 100;

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustSylSpacingFunctor::VisitSystemEnd(System *system)
{
    if (!m_previousMeasure) {
        return FUNCTOR_CONTINUE;
    }

    // Here we also need to handle the last syl of the measure - we check the alignment with the right barline
    if (m_previousVerse && m_lastSyl) {
        int overlap = m_lastSyl->GetContentRight() - m_previousMeasure->GetRightBarLine()->GetAlignment()->GetXRel();
        m_previousVerse->AdjustPosition(overlap, m_freeSpace, m_doc);

        // If the previous verse was not in the previous measure (but before), ignore the overlap because it is
        // not likely to go over the whole following measure
        if ((m_previousMeasure == m_previousVerse->GetFirstAncestor(MEASURE)) && (overlap > 0)) {
            m_overlappingSyl.push_back(std::make_tuple(
                m_previousVerse->GetAlignment(), m_previousMeasure->GetRightBarLine()->GetAlignment(), overlap));
        }
    }

    // Adjust the position of the alignment according to what we have collected for this verse
    m_previousMeasure->m_measureAligner.AdjustProportionally(m_overlappingSyl);
    m_overlappingSyl.clear();

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustSylSpacingFunctor::VisitVerse(Verse *verse)
{
    /****** find label / labelAbbr */

    // If we have a <label>, reset the previous abbreviation
    if (verse->FindDescendantByType(LABEL)) {
        m_currentLabelAbbr = NULL;
    }

    bool newLabelAbbr = false;
    verse->SetDrawingLabelAbbr(NULL);
    // Find the labelAbbr (if none previously given)
    if (m_currentLabelAbbr == NULL) {
        m_currentLabelAbbr = vrv_cast<LabelAbbr *>(verse->FindDescendantByType(LABELABBR));
        // Keep indication that this is a new abbreviation and that it should not be displayed on this verse
        newLabelAbbr = true;
    }

    /*******/

    ListOfObjects syls = verse->FindAllDescendantsByType(SYL);

    int shift = m_doc->GetDrawingUnit(m_staffSize);
    Syl::AdjustToLyricSize(m_doc, shift);

    int previousSylShift = 0;

    verse->SetDrawingXRel(-1 * shift);

    ListOfObjects::iterator iter = syls.begin();
    while (iter != syls.end()) {
        if ((*iter)->HasContentHorizontalBB()) {
            Syl *syl = vrv_cast<Syl *>(*iter);
            assert(syl);
            syl->SetDrawingXRel(previousSylShift);
            previousSylShift += syl->GetContentX2() + syl->CalcConnectorSpacing(m_doc, m_staffSize);
            ++iter;
        }
        else {
            iter = syls.erase(iter);
        }
    }

    if (syls.empty()) return FUNCTOR_CONTINUE;

    Syl *firstSyl = vrv_cast<Syl *>(syls.front());
    assert(firstSyl);
    // We keep a pointer to the last syl because we move it (when more than one) and the verse content bounding box is
    // not updated
    Syl *lastSyl = vrv_cast<Syl *>(syls.back());
    assert(lastSyl);

    // Not much to do when we hit the first syllable of the system
    if (m_previousVerse == NULL) {
        m_previousVerse = verse;
        m_lastSyl = lastSyl;

        if (!newLabelAbbr && m_currentLabelAbbr) {
            verse->SetDrawingLabelAbbr(m_currentLabelAbbr);
        }

        // No free space because we never move the first one back
        m_freeSpace = 0;
        m_previousMeasure = NULL;
        return FUNCTOR_CONTINUE;
    }

    int xShift = 0;

    // We have a previous syllable from the previous measure - we need to add the measure width because the measures are
    // not aligned yet
    if (m_previousMeasure) {
        xShift = m_previousMeasure->GetWidth();
    }

    // Use the syl because the content bounding box of the verse might be invalid at this stage
    int overlap = m_lastSyl->GetContentRight() - (firstSyl->GetContentLeft() + xShift);
    overlap += m_lastSyl->CalcConnectorSpacing(m_doc, m_staffSize);

    // Check that we also include the space for the label if the verse has a new label
    Label *label = vrv_cast<Label *>(verse->FindDescendantByType(LABEL));
    if (label) {
        overlap += (label->GetContentX2() - label->GetContentX1()) + m_doc->GetDrawingDoubleUnit(m_staffSize);
    }

    int nextFreeSpace = m_previousVerse->AdjustPosition(overlap, m_freeSpace, m_doc);

    if (overlap > 0) {
        // We are adjusting syl in two different measures - move only the right barline of the first measure
        if (m_previousMeasure) {
            m_overlappingSyl.push_back(std::make_tuple(
                m_previousVerse->GetAlignment(), m_previousMeasure->GetRightBarLine()->GetAlignment(), overlap));
            // Do it now
            m_previousMeasure->m_measureAligner.AdjustProportionally(m_overlappingSyl);
            m_overlappingSyl.clear();
        }
        else {
            // Normal case, both in the same measure
            m_overlappingSyl.push_back(
                std::make_tuple(m_previousVerse->GetAlignment(), verse->GetAlignment(), overlap));
        }
    }

    m_previousVerse = verse;
    m_lastSyl = lastSyl;
    m_freeSpace = nextFreeSpace;
    m_previousMeasure = NULL;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
