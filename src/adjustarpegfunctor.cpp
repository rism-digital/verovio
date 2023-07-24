/////////////////////////////////////////////////////////////////////////////
// Name:        adjustarpegfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustarpegfunctor.h"

//----------------------------------------------------------------------------

#include "arpeg.h"
#include "doc.h"
#include "staff.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustArpegFunctor
//----------------------------------------------------------------------------

AdjustArpegFunctor::AdjustArpegFunctor(Doc *doc) : DocFunctor(doc)
{
    m_measureAligner = NULL;
}

FunctorCode AdjustArpegFunctor::VisitAlignment(Alignment *alignment)
{
    const AlignmentType alignmentType = alignment->GetType();

    // An array of Alignment / arpeg / staffN / bool (for indicating if we have reached the alignment yet)
    ArrayOfAlignmentArpegTuples::iterator iter = m_alignmentArpegTuples.begin();

    while (iter != m_alignmentArpegTuples.end()) {
        // We are reaching the alignment to which an arpeg points to (i.e, the topNote one)
        if (std::get<0>(*iter) == alignment) {
            std::get<3>(*iter) = true;
            ++iter;
            continue;
        }
        // We have not reached the alignment of the arpeg, just continue (backwards)
        else if (!std::get<3>(*iter)) {
            ++iter;
            continue;
        }
        // We are now in an alignment preceeding an arpeg - check for overlap
        int minLeft, maxRight;
        alignment->GetLeftRight(std::get<2>(*iter), minLeft, maxRight);

        // Nothing for the staff we are looking at, we also need to check with barlines
        if (maxRight == VRV_UNSET) {
            alignment->GetLeftRight(-1, minLeft, maxRight);
        }

        // Make sure that there is no overlap with right barline of the previous measure
        if ((maxRight == VRV_UNSET) && (alignmentType == ALIGNMENT_MEASURE_LEFT_BARLINE)) {
            Measure *measure = vrv_cast<Measure *>(alignment->GetFirstAncestor(MEASURE));
            auto parent = measure->GetParent();
            Measure *previous = vrv_cast<Measure *>(parent->GetPrevious(measure, MEASURE));
            if (previous) {
                Alignment *barLineAlignment = previous->m_measureAligner.GetRightBarLineAlignment();
                barLineAlignment->GetLeftRight(-1, minLeft, maxRight);
                if (maxRight != VRV_UNSET) {
                    const int previousWidth = previous->GetWidth();
                    minLeft -= previousWidth;
                    maxRight -= previousWidth;
                }
            }
        }

        // Make sure that there is no overlap with grace notes (since they are handled separately by graceAligner)
        if (alignmentType == ALIGNMENT_GRACENOTE) {
            int graceAlignerId = m_doc->GetOptions()->m_graceRhythmAlign.GetValue() ? 0 : std::get<2>(*iter);
            if (alignment->HasGraceAligner(graceAlignerId)) {
                GraceAligner *graceAligner = alignment->GetGraceAligner(graceAlignerId);
                maxRight = graceAligner->GetGraceGroupRight(std::get<2>(*iter));
                const int overlap = maxRight - std::get<1>(*iter)->GetCurrentFloatingPositioner()->GetSelfLeft();
                if (overlap > 0) {
                    const int drawingUnit = m_doc->GetDrawingUnit(100);
                    alignment->SetXRel(alignment->GetXRel() - drawingUnit / 6);
                }
            }
        }

        // Nothing, just continue
        if (maxRight == VRV_UNSET) {
            ++iter;
            continue;
        }

        const int overlap = maxRight - std::get<1>(*iter)->GetCurrentFloatingPositioner()->GetSelfLeft();
        const int drawingUnit = m_doc->GetDrawingUnit(100);
        // HARDCODED
        int adjust = overlap + drawingUnit / 2 * 3;
        // LogDebug("maxRight %d, %d %d", maxRight, std::get<2>(*iter), overlap);
        if (adjust > 0) {
            ArrayOfAdjustmentTuples boundaries{ std::make_tuple(alignment, std::get<0>(*iter), adjust) };
            m_measureAligner->AdjustProportionally(boundaries);
            // After adjusting, make sure that arpeggio does not overlap with elements from the previous alignment
            if (alignmentType == ALIGNMENT_CLEF) {
                auto [currentMin, currentMax] = alignment->GetAlignmentTopBottom();
                Note *topNote = NULL;
                Note *bottomNote = NULL;
                std::get<1>(*iter)->GetDrawingTopBottomNotes(topNote, bottomNote);
                if (topNote && bottomNote) {
                    const int arpegMax = topNote->GetDrawingY() + drawingUnit / 2;
                    const int arpegMin = bottomNote->GetDrawingY() - drawingUnit / 2;
                    // Make sure that there is vertical overlap, otherwise do not shift arpeggio
                    if (((currentMin < arpegMin) && (currentMax > arpegMin))
                        || ((currentMax > arpegMax) && (currentMin < arpegMax))) {
                        std::get<0>(*iter)->SetXRel(std::get<0>(*iter)->GetXRel() + overlap + drawingUnit / 2);
                    }
                }
            }
        }

        // We can remove it from the list
        iter = m_alignmentArpegTuples.erase(iter);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustArpegFunctor::VisitArpeg(Arpeg *arpeg)
{
    Note *topNote = NULL;
    Note *bottomNote = NULL;

    arpeg->GetDrawingTopBottomNotes(topNote, bottomNote);

    // Nothing to do
    if (!topNote || !bottomNote) return FUNCTOR_CONTINUE;

    // We should have call DrawArpeg before
    assert(arpeg->GetCurrentFloatingPositioner());

    Staff *topStaff = topNote->GetAncestorStaff();
    Staff *bottomStaff = bottomNote->GetAncestorStaff();

    Staff *crossStaff = arpeg->GetCrossStaff();
    const int staffN = (crossStaff != NULL) ? crossStaff->GetN() : topStaff->GetN();

    int minTopLeft, maxTopRight;
    topNote->GetAlignment()->GetLeftRight(staffN, minTopLeft, maxTopRight);

    m_alignmentArpegTuples.push_back(std::make_tuple(topNote->GetAlignment(), arpeg, topStaff->GetN(), false));

    if (topStaff != bottomStaff) {
        int minBottomLeft, maxBottomRight;
        topNote->GetAlignment()->GetLeftRight(bottomStaff->GetN(), minBottomLeft, maxBottomRight);
        minTopLeft = std::min(minTopLeft, minBottomLeft);

        m_alignmentArpegTuples.push_back(std::make_tuple(topNote->GetAlignment(), arpeg, bottomStaff->GetN(), false));
    }

    if (minTopLeft != -VRV_UNSET) {
        int dist = topNote->GetDrawingX() - minTopLeft;
        // HARDCODED
        double unitFactor = 1.0;
        if ((arpeg->GetEnclose() == ENCLOSURE_brack) || (arpeg->GetEnclose() == ENCLOSURE_box)) unitFactor += 0.75;
        if (arpeg->GetArrow() == BOOLEAN_true) unitFactor += 0.33;
        dist += unitFactor * m_doc->GetDrawingUnit(topStaff->m_drawingStaffSize);
        arpeg->SetDrawingXRel(-dist);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustArpegFunctor::VisitMeasureEnd(Measure *measure)
{
    if (!m_alignmentArpegTuples.empty()) {
        m_measureAligner = &measure->m_measureAligner;
        // Process backwards on measure aligner, then reset to previous direction
        const bool previousDirection = this->SetDirection(BACKWARD);
        m_measureAligner->Process(*this);
        this->SetDirection(previousDirection);
        m_alignmentArpegTuples.clear();
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
