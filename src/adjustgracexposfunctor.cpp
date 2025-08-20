/////////////////////////////////////////////////////////////////////////////
// Name:        adjustgracexposfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustgracexposfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "score.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustGraceXPosFunctor
//----------------------------------------------------------------------------

AdjustGraceXPosFunctor::AdjustGraceXPosFunctor(Doc *doc, const std::vector<int> &staffNs) : DocFunctor(doc)
{
    m_staffNs = staffNs;
    m_graceMaxPos = 0;
    m_graceUpcomingMaxPos = -VRV_UNSET;
    m_graceCumulatedXShift = 0;
    m_isGraceAlignment = false;
    m_rightDefaultAlignment = NULL;
}

FunctorCode AdjustGraceXPosFunctor::VisitAlignment(Alignment *alignment)
{
    // We are in a Measure aligner - redirect to the GraceAligner when it is a ALIGNMENT_GRACENOTE
    if (!m_isGraceAlignment) {
        // Do not process AlignmentReference children if no GraceAligner
        if (alignment->GetGraceAligners().empty()) {
            // We store the default alignment before we hit the grace alignment
            if (alignment->GetType() == ALIGNMENT_DEFAULT) m_rightDefaultAlignment = alignment;
            return FUNCTOR_SIBLINGS;
        }
        assert(alignment->GetType() == ALIGNMENT_GRACENOTE);

        // Change the flag for indicating that the alignment is child of a GraceAligner
        m_isGraceAlignment = true;

        // Get the parent measure aligner
        MeasureAligner *measureAligner = vrv_cast<MeasureAligner *>(alignment->GetFirstAncestor(MEASURE_ALIGNER));
        assert(measureAligner);

        const bool previousDirection = this->SetDirection(BACKWARD);
        Filters filters;
        Filters *previousFilters = this->SetFilters(&filters);

        for (int &n : m_staffNs) {
            const int graceAlignerId = m_doc->GetOptions()->m_graceRhythmAlign.GetValue() ? 0 : n;

            std::vector<ClassId> exclude;
            if (alignment->HasGraceAligner(graceAlignerId) && m_rightDefaultAlignment) {
                GraceAligner *graceAligner = alignment->GetGraceAligner(graceAlignerId);
                // last alignment of GraceAligner is rightmost one, so get it
                Alignment *alignment = vrv_cast<Alignment *>(graceAligner->GetLast(ALIGNMENT));
                // if there is no overlap with accidentals, exclude them when getting left-right margins of alignment
                if (alignment && !alignment->HasAccidVerticalOverlap(m_rightDefaultAlignment, graceAlignerId)) {
                    exclude.push_back(ACCID);
                }
            }

            // Rescue value, used at the end of a measure without a barline
            int graceMaxPos = alignment->GetXRel() - m_doc->GetDrawingUnit(100);
            // If we have a rightDefault, then this is (quite likely) the next note / chord
            // Get its minimum left and make it the max right position of the grace group
            if (m_rightDefaultAlignment) {
                int minLeft, maxRight;
                m_rightDefaultAlignment->GetLeftRight(n, minLeft, maxRight, exclude);
                if (minLeft != -VRV_UNSET)
                    graceMaxPos = minLeft - m_doc->GetLeftMargin(NOTE) * m_doc->GetDrawingUnit(75);
            }
            // This happens when grace notes are at the end of a measure before a barline
            else {
                int minLeft, maxRight;
                assert(measureAligner->GetRightBarLineAlignment());
                // staffN -1 is barline
                measureAligner->GetRightBarLineAlignment()->GetLeftRight(
                    BARLINE_REFERENCES, minLeft, maxRight, exclude);
                if (minLeft != -VRV_UNSET)
                    graceMaxPos = minLeft - m_doc->GetLeftMargin(NOTE) * m_doc->GetDrawingUnit(75);
            }

            m_graceMaxPos = graceMaxPos;
            m_graceUpcomingMaxPos = -VRV_UNSET;
            m_graceCumulatedXShift = VRV_UNSET;
            filters.Clear();
            // Create ad comparison object for each type / @n
            AttNIntegerComparison matchStaff(ALIGNMENT_REFERENCE, n);
            filters.Add(&matchStaff);

            if (alignment->HasGraceAligner(graceAlignerId)) {
                // Process backwards with filters on grace aligner
                alignment->GetGraceAligner(graceAlignerId)->Process(*this);

                // There were no grace notes for the staff
                if (m_graceCumulatedXShift == VRV_UNSET) continue;

                // Now we need to adjust the space for the grace note group
                measureAligner->AdjustGraceNoteSpacing(m_doc, alignment, n);
            }
        }

        this->SetDirection(previousDirection);
        this->SetFilters(previousFilters);

        // Change the flag back
        m_isGraceAlignment = false;

        return FUNCTOR_CONTINUE;
    }

    if (m_graceCumulatedXShift != VRV_UNSET) {
        // This is happening when aligning the grace aligner itself
        alignment->SetXRel(alignment->GetXRel() + m_graceCumulatedXShift);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustGraceXPosFunctor::VisitAlignmentEnd(Alignment *alignment)
{
    if (m_graceUpcomingMaxPos != -VRV_UNSET) {
        m_graceMaxPos = m_graceUpcomingMaxPos;
        // We reset it for the next aligner
        m_graceUpcomingMaxPos = -VRV_UNSET;
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustGraceXPosFunctor::VisitAlignmentReference(AlignmentReference *alignmentReference)
{
    // Because we are processing grace notes alignment backward (see VisitAlignment) we need
    // to process the children (LayerElement) "by hand" in FORWARD manner
    // (filters can be NULL because filtering was already applied in the parent)
    const bool previousDirection = this->SetDirection(FORWARD);
    Filters *previousFilters = this->SetFilters(NULL);

    for (auto child : alignmentReference->GetChildren()) {
        child->Process(*this);
    }

    this->SetDirection(previousDirection);
    this->SetFilters(previousFilters);

    return FUNCTOR_SIBLINGS;
}

FunctorCode AdjustGraceXPosFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    if (m_graceCumulatedXShift == VRV_UNSET) m_graceCumulatedXShift = 0;

    // LogDebug("********* Aligning %s", layerElement->GetClassName().c_str());

    // With non grace alignment we do not need to do this
    layerElement->ResetCachedDrawingX();

    if (!layerElement->HasGraceAlignment()) return FUNCTOR_SIBLINGS;

    if (!layerElement->HasSelfBB() || layerElement->HasEmptyBB()) {
        // if nothing was drawn, do not take it into account
        return FUNCTOR_SIBLINGS;
    }

    int selfRight = layerElement->GetSelfRight();
    int offset = selfRight - m_graceMaxPos;
    if (offset > 0) {
        layerElement->GetGraceAlignment()->SetXRel(layerElement->GetGraceAlignment()->GetXRel() - offset);
        // Also move the accumulated x shift and the minimum position for the next alignment accordingly
        m_graceCumulatedXShift += (-offset);
        m_graceUpcomingMaxPos += (-offset);
    }

    int selfLeft = layerElement->GetSelfLeft()
        - m_doc->GetLeftMargin(layerElement) * m_doc->GetDrawingUnit(m_doc->GetCueSize(100));

    m_graceUpcomingMaxPos = std::min(selfLeft, m_graceUpcomingMaxPos);

    auto it = std::find_if(m_measureTieEndpoints.cbegin(), m_measureTieEndpoints.cend(),
        [layerElement](const std::pair<LayerElement *, LayerElement *> &pair) { return pair.first == layerElement; });
    if (it != m_measureTieEndpoints.end() && m_rightDefaultAlignment) {
        const int unit = m_doc->GetDrawingUnit(100);
        const int minTieLength = m_doc->GetOptions()->m_tieMinLength.GetValue() * unit;
        const int diff = m_rightDefaultAlignment->GetXRel() - layerElement->GetSelfRight();

        if (diff < (minTieLength + unit)) m_graceMaxPos -= (unit + minTieLength - diff);
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode AdjustGraceXPosFunctor::VisitMeasure(Measure *measure)
{
    measure->m_measureAligner.PushAlignmentsRight();
    m_rightDefaultAlignment = NULL;

    // We process it backward because we want to get the rightDefaultAlignment
    const bool previousDirection = this->SetDirection(BACKWARD);
    measure->m_measureAligner.Process(*this);

    // We need to process the staves in the reverse order
    std::vector<int> staffNs = m_staffNs;
    std::vector<int> staffNsReversed;
    staffNsReversed.resize(staffNs.size());
    std::reverse_copy(staffNs.begin(), staffNs.end(), staffNsReversed.begin());

    measure->m_measureAligner.PushAlignmentsRight();
    m_rightDefaultAlignment = NULL;

    m_staffNs = staffNsReversed;
    m_measureTieEndpoints = measure->GetInternalTieEndpoints();
    measure->m_measureAligner.Process(*this);
    this->SetDirection(previousDirection);

    // Put params back
    m_staffNs = staffNs;

    return FUNCTOR_SIBLINGS;
}

FunctorCode AdjustGraceXPosFunctor::VisitScore(Score *score)
{
    assert(score->GetScoreDef());

    m_staffNs = score->GetScoreDef()->GetStaffNs();

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
