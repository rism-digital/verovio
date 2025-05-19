/////////////////////////////////////////////////////////////////////////////
// Name:        adjustxposfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustxposfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "multirest.h"
#include "rest.h"
#include "score.h"
#include "staff.h"
#include "system.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustXPosFunctor
//----------------------------------------------------------------------------

AdjustXPosFunctor::AdjustXPosFunctor(Doc *doc, const std::vector<int> &staffNs) : DocFunctor(doc)
{
    m_minPos = 0;
    m_upcomingMinPos = VRV_UNSET;
    m_cumulatedXShift = 0;
    m_staffN = 0;
    m_staffNs = staffNs;
    m_staffSize = 100;
    m_rightBarLinesOnly = false;
    m_measure = NULL;
}

FunctorCode AdjustXPosFunctor::VisitAlignment(Alignment *alignment)
{
    // LogDebug("Alignment type %d", alignment->GetType());

    alignment->SetXRel(alignment->GetXRel() + m_cumulatedXShift);

    if ((alignment->GetType() == ALIGNMENT_MEASURE_END) && (alignment->GetXRel() < m_minPos)) {
        alignment->SetXRel(m_minPos);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustXPosFunctor::VisitAlignmentEnd(Alignment *alignment)
{
    if (m_upcomingMinPos != VRV_UNSET) {
        m_minPos = m_upcomingMinPos;
        // We reset it for the next aligner
        m_upcomingMinPos = VRV_UNSET;
    }

    // No upcoming bounding boxes, we keep the previous ones (e.g., the alignment has nothing for this staff)
    // Eventually we might want to have a more sophisticated pruning algorithm
    if (m_upcomingBoundingBoxes.empty()) return FUNCTOR_CONTINUE;

    // Handle additional offsets that can happen when we have overlapping dots/flags. This should happen only for
    // default alignments, so other ones should be ignored. If there is at least one bounding box that overlaps with
    // dot/flag from the previous alignment - we need to consider additional offset for those elements. In such case,
    // all current elements should have their XRel adjusted (as they would normally have) and increase minXPosition by
    // the dot/flag offset
    if (m_previousAlignment.m_overlappingBB && m_previousAlignment.m_alignment
        && (m_previousAlignment.m_alignment->GetType() == ALIGNMENT_DEFAULT)) {
        auto it = std::find_if(m_upcomingBoundingBoxes.begin(), m_upcomingBoundingBoxes.end(), [this](BoundingBox *bb) {
            if (m_previousAlignment.m_overlappingBB == bb) return false;
            // check if elements actually overlap
            return (bb->HorizontalSelfOverlap(m_previousAlignment.m_overlappingBB)
                && bb->VerticalSelfOverlap(m_previousAlignment.m_overlappingBB));
        });
        if (it != m_upcomingBoundingBoxes.end()) {
            m_currentAlignment.m_alignment->SetXRel(
                m_currentAlignment.m_alignment->GetXRel() + m_previousAlignment.m_offset);
            m_minPos += m_previousAlignment.m_offset;
            m_cumulatedXShift += m_previousAlignment.m_offset;
        }
    }
    m_previousAlignment = m_currentAlignment;
    // Reset current alignment
    m_currentAlignment.Reset();

    m_boundingBoxes = m_upcomingBoundingBoxes;
    m_upcomingBoundingBoxes.clear();

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustXPosFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    // we should have processed aligned before
    assert(layerElement->GetAlignment());

    if (!layerElement->HasToBeAligned()) {
        // if nothing to do with this type of element
        // this happens for example with Artic where only ArticPart children are aligned
        return FUNCTOR_SIBLINGS;
    }

    // If we have a list of types to exclude and it is one of them, stop it
    if (!m_excludes.empty() && layerElement->Is(m_excludes)) {
        return FUNCTOR_CONTINUE;
    }

    // If we have a list of types to include and it is not one of them, stop it
    if (!m_includes.empty() && !layerElement->Is(m_includes)) {
        return FUNCTOR_CONTINUE;
    }

    // If desired only handle barlines which are right positioned
    if (m_rightBarLinesOnly && layerElement->Is(BARLINE)) {
        if (vrv_cast<BarLine *>(layerElement)->GetPosition() != BarLinePosition::Right) {
            return FUNCTOR_CONTINUE;
        }
    }

    if (layerElement->HasSameasLink()) {
        // nothing to do when the element has a @sameas attribute
        return FUNCTOR_SIBLINGS;
    }

    if ((layerElement->GetAlignment()->GetType() == ALIGNMENT_CLEF) && !m_isNeumeStaff) {
        return FUNCTOR_CONTINUE;
    }

    int offset = 0;
    int selfLeft;
    const int drawingUnit = m_doc->GetDrawingUnit(m_staffSize);
    std::tie(offset, selfLeft) = this->CalculateXPosOffset(layerElement);

    offset = std::min(offset, selfLeft - m_minPos);
    if (offset < 0) {
        layerElement->GetAlignment()->SetXRel(layerElement->GetAlignment()->GetXRel() - offset);
        // Also move the accumulated x shift and the minimum position for the next alignment accordingly
        m_cumulatedXShift += (-offset);
        m_upcomingMinPos += (-offset);
    }

    int selfRight;
    if (!layerElement->HasSelfBB() || layerElement->HasEmptyBB()) {
        selfRight = layerElement->GetAlignment()->GetXRel();
        // Still add the right margin for the barlines but not with non measure music
        if (layerElement->Is(BARLINE) && m_measure->IsMeasuredMusic())
            selfRight += m_doc->GetRightMargin(layerElement) * drawingUnit;
    }
    else {
        selfRight = layerElement->GetSelfRight() + m_doc->GetRightMargin(layerElement) * drawingUnit;
    }

    // In case of dots/flags we need to hold off adjusting the upcoming min position right away - if it happens that
    // these elements do not overlap with other elements we can draw them as is and save space
    AlignmentReference *currentReference
        = layerElement->GetAlignment()->GetReferenceWithElement(layerElement, m_staffN);
    Alignment *nextAlignment = vrv_cast<Alignment *>(
        layerElement->GetAlignment()->GetParent()->GetNext(layerElement->GetAlignment(), ALIGNMENT));
    AlignmentType next = nextAlignment ? nextAlignment->GetType() : ALIGNMENT_DEFAULT;
    if (layerElement->Is({ DOTS, FLAG }) && currentReference->HasMultipleLayer()
        && (next != ALIGNMENT_MEASURE_RIGHT_BARLINE)) {
        const int additionalOffset = selfRight - m_upcomingMinPos;
        if (additionalOffset > m_currentAlignment.m_offset) {
            m_currentAlignment.m_offset = additionalOffset;
            m_currentAlignment.m_overlappingBB = layerElement;
        }
    }
    else if (layerElement->Is(NOTE) && (next == ALIGNMENT_MEASURE_RIGHT_BARLINE)) {
        Note *note = vrv_cast<Note *>(layerElement);
        if (note->HasStemMod() && (note->GetStemMod() < STEMMODIFIER_MAX)
            && (note->GetDrawingStemDir() == STEMDIRECTION_up)) {
            const int adjust = drawingUnit;
            m_cumulatedXShift += adjust;
            m_upcomingMinPos += adjust;
        }
        else {
            m_upcomingMinPos = std::max(selfRight, m_upcomingMinPos);
        }
    }
    else {
        m_upcomingMinPos = std::max(selfRight, m_upcomingMinPos);
    }

    auto it = std::find_if(m_measureTieEndpoints.begin(), m_measureTieEndpoints.end(),
        [layerElement](const std::pair<LayerElement *, LayerElement *> &pair) { return pair.second == layerElement; });
    if (it != m_measureTieEndpoints.end()) {
        const int minTieLength = m_doc->GetOptions()->m_tieMinLength.GetValue() * drawingUnit;
        const int leftXPos = it->first->HasContentBB() ? it->first->GetContentRight() : it->first->GetDrawingX();
        const int rightXPos = it->second->HasContentBB() ? it->second->GetContentLeft() : it->second->GetDrawingX();
        const int currentTieLength = rightXPos - leftXPos - drawingUnit;
        if ((currentTieLength < minTieLength)
            && ((it->first->GetFirstAncestor(CHORD) != NULL) || (layerElement->GetFirstAncestor(CHORD) != NULL)
                || (it->first->FindDescendantByType(FLAG) != NULL))) {
            const int adjust = minTieLength - currentTieLength;
            layerElement->GetAlignment()->SetXRel(layerElement->GetAlignment()->GetXRel() + adjust);
            // Also move the accumulated x shift and the minimum position for the next alignment accordingly
            m_cumulatedXShift += adjust;
            m_upcomingMinPos += adjust;
        }
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode AdjustXPosFunctor::VisitMeasure(Measure *measure)
{
    m_minPos = 0;
    m_upcomingMinPos = VRV_UNSET;
    m_cumulatedXShift = 0;

    m_measure = measure;

    System *system = vrv_cast<System *>(measure->GetFirstAncestor(SYSTEM));
    assert(system);

    const bool hasSystemStartLine = measure->IsFirstInSystem() && system->GetDrawingScoreDef()->HasSystemStartLine();

    Filters filters;
    Filters *previousFilters = this->SetFilters(&filters);

    for (auto staffN : m_staffNs) {
        m_minPos = 0;
        m_upcomingMinPos = VRV_UNSET;
        m_cumulatedXShift = 0;
        m_staffN = staffN;
        m_boundingBoxes.clear();
        m_previousAlignment.Reset();
        m_currentAlignment.Reset();
        StaffAlignment *staffAlignment = system->m_systemAligner.GetStaffAlignmentForStaffN(staffN);
        m_staffSize = (staffAlignment) ? staffAlignment->GetStaffSize() : 100;
        m_isNeumeStaff = (staffAlignment && staffAlignment->GetStaff()) ? staffAlignment->GetStaff()->IsNeume() : false;

        // Prevent collisions of scoredef clefs with thick barlines
        if (hasSystemStartLine) {
            m_upcomingMinPos = m_doc->GetDrawingBarLineWidth(m_staffSize);
        }

        // Create ad comparison object for each type / @n
        std::vector<int> ns;
        // -1 for barline attributes that need to be taken into account each time
        ns.push_back(-1);
        ns.push_back(staffN);
        AttNIntegerAnyComparison matchStaff(ALIGNMENT_REFERENCE, ns);
        CrossAlignmentReferenceComparison matchCrossStaff;
        filters.SetType(Filters::Type::AnyOf);
        filters = { &matchStaff, &matchCrossStaff };

        m_measureTieEndpoints = measure->GetInternalTieEndpoints();
        measure->m_measureAligner.Process(*this);
    }

    this->SetFilters(previousFilters);

    // There is no reason to adjust a minimum width with mensural music
    if (!measure->IsMeasuredMusic()) return FUNCTOR_SIBLINGS;

    int minMeasureWidth = m_doc->GetOptions()->m_unit.GetValue() * m_doc->GetOptions()->m_measureMinWidth.GetValue();
    // First try to see if we have a double measure length element
    MeasureAlignerTypeComparison alignmentComparison(ALIGNMENT_FULLMEASURE2);
    Alignment *fullMeasure2
        = dynamic_cast<Alignment *>(measure->m_measureAligner.FindDescendantByComparison(&alignmentComparison, 1));

    // With a double measure with element (mRpt2, multiRpt)
    if (fullMeasure2 != NULL) {
        minMeasureWidth *= 2;
    }
    // Nothing if the measure has at least one note or @metcon="false"
    else if ((measure->FindDescendantByType(NOTE) != NULL) || (measure->GetMetcon() == BOOLEAN_false)) {
        minMeasureWidth = 0;
    }
    // Adjust min width based on multirest attributes (@num and @width), but only if these values are larger than
    // current min width
    else if (measure->FindDescendantByType(MULTIREST) != NULL) {
        const int unit = m_doc->GetDrawingUnit(m_staffSize);
        MultiRest *multiRest = vrv_cast<MultiRest *>(measure->FindDescendantByType(MULTIREST));
        const int num = multiRest->GetNum();
        if (multiRest->HasWidth() && multiRest->AttWidth::GetWidth().GetType() == MEASUREMENTTYPE_vu) {
            const int fixedWidth = multiRest->AttWidth::GetWidth().GetVu() * (unit + 4);
            if (minMeasureWidth < fixedWidth) minMeasureWidth = fixedWidth;
        }
        else if (num > 10) {
            minMeasureWidth *= log1p(num) / 2;
        }
        Object *layer = multiRest->GetFirstAncestor(LAYER);
        if (layer->GetLast() != multiRest) {
            Object *object = layer->GetNext(multiRest);
            if (object && object->Is(CLEF)) {
                const int clefWidth = object->GetContentRight() - object->GetContentLeft();
                minMeasureWidth += clefWidth + unit;
            }
        }
    }

    int currentMeasureWidth = measure->GetRightBarLineLeft() - measure->GetLeftBarLineRight();
    if (currentMeasureWidth < minMeasureWidth) {
        ArrayOfAdjustmentTuples boundaries{ std::make_tuple(measure->GetLeftBarLine()->GetAlignment(),
            measure->GetRightBarLine()->GetAlignment(), minMeasureWidth - currentMeasureWidth) };
        measure->m_measureAligner.AdjustProportionally(boundaries);
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode AdjustXPosFunctor::VisitScore(Score *score)
{
    assert(score->GetScoreDef());

    m_staffNs = score->GetScoreDef()->GetStaffNs();

    return FUNCTOR_CONTINUE;
}

std::pair<int, int> AdjustXPosFunctor::CalculateXPosOffset(LayerElement *layerElement)
{
    int selfLeft = 0;
    const int drawingUnit = m_doc->GetDrawingUnit(m_staffSize);
    // Nested alignment of bounding boxes is performed only when both the previous alignment and the current one allow
    // it. For example, when one of them is a barline, we do not look how bounding boxes can be nested but instead only
    // look at the horizontal position
    bool performBoundingBoxAlignment
        = (m_previousAlignment.m_alignment && m_previousAlignment.m_alignment->PerformBoundingBoxAlignment()
            && layerElement->GetAlignment()->PerformBoundingBoxAlignment());

    if (!layerElement->HasSelfBB() || layerElement->HasEmptyBB()) {
        // If nothing was drawn, do not take it into account. This should happen for barline position none but also
        // chords in beam. Otherwise the BB should be set to empty with Object::SetEmptyBB()
        selfLeft = layerElement->GetAlignment()->GetXRel();
        return { 0, selfLeft };
    }

    // We add it to the upcoming bounding boxes
    m_upcomingBoundingBoxes.push_back(layerElement);
    m_currentAlignment.m_alignment = layerElement->GetAlignment();

    // only look at the horizontal position
    if (!performBoundingBoxAlignment) {
        selfLeft = layerElement->GetSelfLeft();
        selfLeft -= m_doc->GetLeftMargin(layerElement) * drawingUnit;
        return { 0, selfLeft };
    }

    // Here we look how bounding boxes overlap and adjust the position only when necessary
    selfLeft = layerElement->GetAlignment()->GetXRel();
    // If we want the nesting to be reduced, we can set to:
    // selfLeft = this->GetSelfLeft();
    // This could be made an option (--spacing-limited-nesting)
    const double selfLeftMargin = m_doc->GetLeftMargin(layerElement);
    int overlap = 0;
    for (const auto &boundingBox : m_boundingBoxes) {
        LayerElement *bboxElement = vrv_cast<LayerElement *>(boundingBox);
        assert(bboxElement);
        int margin = (m_doc->GetRightMargin(bboxElement) + selfLeftMargin) * drawingUnit;
        if (bboxElement->Is(NOTE)) {
            Note *note = vrv_cast<Note *>(bboxElement);
            assert(note);
            if (note->HasStemMod() && note->GetStemMod() < STEMMODIFIER_MAX) {
                const int tremWidth = m_doc->GetGlyphWidth(SMUFL_E220_tremolo1, m_staffSize, false);
                margin = std::max(margin, drawingUnit / 3 + tremWidth / 2);
            }
        }
        bool hasOverlap = layerElement->HorizontalContentOverlap(boundingBox, margin);
        if (!hasOverlap) continue;

        // For note to note alignment, make sure there is a standard spacing even if they do not overlap
        // vertically
        if (layerElement->Is(NOTE) && bboxElement->Is(NOTE)) {
            overlap = std::max(overlap, bboxElement->GetSelfRight() - layerElement->GetSelfLeft() + margin);
        }
        else if (layerElement->Is(ACCID) && bboxElement->Is(NOTE)) {
            Staff *staff = layerElement->GetAncestorStaff();
            const int staffTop = staff->GetDrawingY();
            const int staffBottom = staffTop - m_doc->GetDrawingStaffSize(m_staffSize);
            int verticalMargin = 0;
            if ((layerElement->GetContentTop() > staffTop + 2 * drawingUnit) && (bboxElement->GetDrawingY() > staffTop)
                && (bboxElement->GetDrawingY() > layerElement->GetDrawingY())) {
                verticalMargin = bboxElement->GetDrawingY() - layerElement->GetDrawingY();
            }
            else if ((layerElement->GetContentBottom() < staffBottom - 2 * drawingUnit)
                && (bboxElement->GetDrawingY() < staffBottom)
                && (bboxElement->GetDrawingY() < layerElement->GetDrawingY())) {
                verticalMargin = layerElement->GetDrawingY() - bboxElement->GetDrawingY();
            }
            overlap
                = std::max(overlap, boundingBox->HorizontalRightOverlap(layerElement, m_doc, margin, verticalMargin));
        }
        else if (layerElement->Is(ACCID) && bboxElement->Is(REST)) {
            Rest *rest = vrv_cast<Rest *>(bboxElement);
            assert(rest);
            const bool hasExplicitLoc = ((rest->HasOloc() && rest->HasPloc()) || rest->HasLoc());
            if (rest->IsInBeam() && !hasExplicitLoc) {
                overlap = std::max(overlap, bboxElement->GetSelfRight() - layerElement->GetSelfLeft() + margin);
            }
            else {
                overlap = std::max(overlap, boundingBox->HorizontalRightOverlap(layerElement, m_doc, margin));
            }
        }
        else {
            overlap = std::max(overlap, boundingBox->HorizontalRightOverlap(layerElement, m_doc, margin));
        }
        // if there is no overlap between elements, make additional checks for some of the edge cases
        if (!overlap) {
            // if last element of the tuplet is rest, make sure there is sufficient distance between it and next
            // note/chord (for ledger lines)
            if (layerElement->Is({ NOTE, CHORD }) && !layerElement->GetFirstAncestor(TUPLET) && bboxElement->Is(REST)
                && bboxElement->GetFirstAncestor(TUPLET)) {
                Rest *rest = vrv_cast<Rest *>(bboxElement);
                if (rest->GetDur() > DURATION_8) {
                    overlap = 1.5 * (rest->GetDur() - DURATION_8) * drawingUnit;
                }
            }
        }
    }

    return { -overlap, selfLeft };
}

} // namespace vrv
