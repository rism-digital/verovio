/////////////////////////////////////////////////////////////////////////////
// Name:        adjustbeamsfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustbeamsfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "elementpart.h"
#include "ftrem.h"
#include "layer.h"
#include "rest.h"
#include "staff.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustBeamsFunctor
//----------------------------------------------------------------------------

AdjustBeamsFunctor::AdjustBeamsFunctor(Doc *doc) : DocFunctor(doc)
{
    m_outerBeam = NULL;
    m_outerFTrem = NULL;
    m_y1 = 0;
    m_y2 = 0;
    m_x1 = 0;
    m_x2 = 0;
    m_beamSlope = 0.0;
    m_directionBias = 0;
    m_overlapMargin = 0;
    m_isOtherLayer = false;
}

FunctorCode AdjustBeamsFunctor::VisitBeam(Beam *beam)
{
    BeamSegment &beamSegment = beam->m_beamSegment;

    if (beam->IsTabBeam() || beam->HasSameas() || !beam->GetChildCount()
        || beamSegment.m_beamElementCoordRefs.empty()) {
        return FUNCTOR_CONTINUE;
    }

    // should never happen
    if (m_outerFTrem) return FUNCTOR_CONTINUE;

    // process highest-level beam
    if (!m_outerBeam) {
        if (beam->m_drawingPlace == BEAMPLACE_mixed) {
            beamSegment.RequestStaffSpace(m_doc, beam);
        }
        else {
            m_outerBeam = beam;
            m_y1 = beamSegment.m_beamElementCoordRefs.front()->m_yBeam;
            m_y2 = beamSegment.m_beamElementCoordRefs.back()->m_yBeam;
            m_x1 = beamSegment.m_beamElementCoordRefs.front()->m_x;
            m_x2 = beamSegment.m_beamElementCoordRefs.back()->m_x;
            m_beamSlope = beamSegment.m_beamSlope;
            m_directionBias = (beam->m_drawingPlace == BEAMPLACE_above) ? 1 : -1;
            m_overlapMargin = this->CalcLayerOverlap(beam);
        }
        return FUNCTOR_CONTINUE;
    }

    int leftMargin = 0, rightMargin = 0;
    const int beamCount
        = m_outerBeam->GetBeamPartDuration((*beamSegment.m_beamElementCoordRefs.begin())->m_x) - DURATION_8;
    const int currentBeamYLeft = m_y1 + m_beamSlope * ((*beamSegment.m_beamElementCoordRefs.begin())->m_x - m_x1);
    const int currentBeamYRight = m_y1 + m_beamSlope * (beamSegment.m_beamElementCoordRefs.back()->m_x - m_x1);
    leftMargin = (*beamSegment.m_beamElementCoordRefs.begin())->m_yBeam - currentBeamYLeft
        + m_directionBias * (beamCount * m_outerBeam->m_beamWidth + m_outerBeam->m_beamWidthBlack);
    rightMargin = beamSegment.m_beamElementCoordRefs.back()->m_yBeam - currentBeamYRight
        + m_directionBias * (beamCount * m_outerBeam->m_beamWidth + m_outerBeam->m_beamWidthBlack);

    const int overlapMargin = std::max(leftMargin * m_directionBias, rightMargin * m_directionBias);
    if (overlapMargin >= m_overlapMargin) {
        Staff *staff = beam->GetAncestorStaff();
        const int staffOffset = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        m_overlapMargin = (overlapMargin + staffOffset) * m_directionBias;
    }
    return FUNCTOR_SIBLINGS;
}

FunctorCode AdjustBeamsFunctor::VisitBeamEnd(Beam *beam)
{
    if (beam->IsTabBeam()) return FUNCTOR_CONTINUE;

    if (m_outerBeam != beam) return FUNCTOR_CONTINUE;

    if (beam->m_drawingPlace == BEAMPLACE_mixed) return FUNCTOR_CONTINUE;

    Layer *parentLayer = vrv_cast<Layer *>(beam->GetFirstAncestor(LAYER));
    if (parentLayer) {
        // find elements on the other layers for the duration of the current beam
        auto otherLayersElements = parentLayer->GetLayerElementsForTimeSpanOf(beam, true);
        if (!otherLayersElements.empty()) {
            // call AdjustBeams separately for each element to find possible overlaps
            m_isOtherLayer = true;
            for (const auto element : otherLayersElements) {
                if (!m_outerBeam->HorizontalContentOverlap(element)) continue;
                element->Process(*this, 0);
            }
            m_isOtherLayer = false;
        }
    }

    // set overlap margin for each coord in the beam
    if (m_overlapMargin) {
        std::for_each(beam->m_beamSegment.m_beamElementCoordRefs.begin(),
            beam->m_beamSegment.m_beamElementCoordRefs.end(),
            [overlap = m_overlapMargin](BeamElementCoord *coord) { coord->m_overlapMargin = overlap; });
    }
    m_outerBeam = NULL;
    m_overlapMargin = 0;

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustBeamsFunctor::VisitClef(Clef *clef)
{
    if (!m_outerBeam) return FUNCTOR_SIBLINGS;
    // ignore elements that start before/after the beam
    if (clef->GetDrawingX() < m_x1) return FUNCTOR_CONTINUE;
    if (clef->GetDrawingX() > m_x2) return FUNCTOR_CONTINUE;

    Staff *staff = clef->GetAncestorStaff();
    // find number of beams at current position
    const int beams = m_outerBeam->GetBeamPartDuration(clef) - DURATION_4;
    const int beamWidth = m_outerBeam->m_beamWidth;
    // find beam Y positions that are relevant to current clef
    const int currentBeamYLeft = m_y1 + m_beamSlope * (clef->GetContentLeft() - m_x1);
    const int currentBeamYRight = m_y1 + m_beamSlope * (clef->GetContentRight() - m_x1);
    // get clef code and find its bounds on the staff (anchor point and top/bottom depending on the beam place)
    const char32_t clefCode = clef->GetClefGlyph(staff->m_drawingNotationType);
    if (!clefCode) return FUNCTOR_SIBLINGS;

    const int clefPosition = staff->GetDrawingY()
        - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * (staff->m_drawingLines - clef->GetLine());
    const int clefBounds = clefPosition
        + ((m_directionBias > 0) ? m_doc->GetGlyphTop(clefCode, staff->m_drawingStaffSize, false)
                                 : m_doc->GetGlyphBottom(clefCode, staff->m_drawingStaffSize, false));
    // calculate margins for the clef
    const int leftMargin = m_directionBias * (currentBeamYLeft - clefBounds) - beams * beamWidth;
    const int rightMargin = m_directionBias * (currentBeamYRight - clefBounds) - beams * beamWidth;
    int overlapMargin = std::min(leftMargin, rightMargin);
    if (overlapMargin >= 0) return FUNCTOR_CONTINUE;
    // calculate offset required for the beam
    overlapMargin *= -m_directionBias;
    const int unit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    const int adjust = this->AdjustOverlapToHalfUnit(overlapMargin, unit);
    if (std::abs(adjust) > std::abs(m_overlapMargin)) m_overlapMargin = adjust;

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustBeamsFunctor::VisitFTrem(FTrem *fTrem)
{
    BeamSegment &beamSegment = fTrem->m_beamSegment;

    if (fTrem->HasSameas() || !fTrem->GetChildCount() || beamSegment.m_beamElementCoordRefs.empty()) {
        return FUNCTOR_CONTINUE;
    }

    if (!m_outerBeam && !m_outerFTrem) {
        if (fTrem->m_drawingPlace == BEAMPLACE_mixed) {
            beamSegment.RequestStaffSpace(m_doc, fTrem);
        }
        else {
            m_outerFTrem = fTrem;
            m_y1 = beamSegment.m_beamElementCoordRefs.front()->m_yBeam;
            m_y2 = beamSegment.m_beamElementCoordRefs.back()->m_yBeam;
            m_x1 = beamSegment.m_beamElementCoordRefs.front()->m_x;
            m_x2 = beamSegment.m_beamElementCoordRefs.back()->m_x;
            m_beamSlope = beamSegment.m_beamSlope;
            m_directionBias = (fTrem->m_drawingPlace == BEAMPLACE_above) ? 1 : -1;
            m_overlapMargin = this->CalcLayerOverlap(fTrem);
        }
        return FUNCTOR_CONTINUE;
    }

    const int leftMargin = (*beamSegment.m_beamElementCoordRefs.begin())->m_yBeam - m_y1;
    const int rightMargin = beamSegment.m_beamElementCoordRefs.back()->m_yBeam - m_y2;

    const int overlapMargin = std::max(leftMargin * m_directionBias, rightMargin * m_directionBias);
    if (overlapMargin >= m_overlapMargin) {
        Staff *staff = fTrem->GetAncestorStaff();
        const int staffOffset = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        m_overlapMargin = (overlapMargin + staffOffset) * m_directionBias;
    }
    return FUNCTOR_SIBLINGS;
}

FunctorCode AdjustBeamsFunctor::VisitFTremEnd(FTrem *fTrem)
{
    if (m_outerFTrem != fTrem) return FUNCTOR_CONTINUE;

    if (fTrem->m_drawingPlace == BEAMPLACE_mixed) return FUNCTOR_CONTINUE;

    Layer *parentLayer = vrv_cast<Layer *>(fTrem->GetFirstAncestor(LAYER));
    if (parentLayer) {
        // find elements on the other layers for the duration of the current beam
        auto otherLayersElements = parentLayer->GetLayerElementsForTimeSpanOf(fTrem, true);
        if (!otherLayersElements.empty()) {
            // call AdjustBeams separately for each element to find possible overlaps
            m_isOtherLayer = true;
            for (const auto element : otherLayersElements) {
                if (!m_outerFTrem->HorizontalContentOverlap(element)) continue;
                element->Process(*this, 0);
            }
            m_isOtherLayer = false;
        }
    }

    // set overlap margin for each coord in the beam
    if (m_overlapMargin) {
        std::for_each(fTrem->m_beamSegment.m_beamElementCoordRefs.begin(),
            fTrem->m_beamSegment.m_beamElementCoordRefs.end(),
            [overlap = m_overlapMargin](BeamElementCoord *coord) { coord->m_overlapMargin = overlap; });
    }
    m_outerFTrem = NULL;
    m_overlapMargin = 0;

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustBeamsFunctor::VisitLayerElement(LayerElement *layerElement)
{
    // ignore elements that are not in the outer beam/ftrem or are direct children
    BeamDrawingInterface *outerBeamInterface = this->GetOuterBeamInterface();
    if (!outerBeamInterface) return FUNCTOR_CONTINUE;
    if (!m_isOtherLayer && !layerElement->Is(ACCID) && !layerElement->IsGraceNote()
        && ((layerElement->GetFirstAncestor(BEAM) == m_outerBeam)
            || (layerElement->GetFirstAncestor(FTREM) == m_outerFTrem)))
        return FUNCTOR_CONTINUE;
    // ignore elements that are both on other layer and cross-staff
    if (m_isOtherLayer && layerElement->m_crossStaff) return FUNCTOR_CONTINUE;
    // ignore specific elements, since they should not be influencing beam positioning
    if (layerElement->Is({ BTREM, GRACEGRP, SPACE, TUPLET, TUPLET_BRACKET, TUPLET_NUM })) return FUNCTOR_CONTINUE;
    // ignore elements that start before the beam
    if (layerElement->GetDrawingX() < m_x1) return FUNCTOR_CONTINUE;
    // ignore elements that have @visible attribute set to false
    AttVisibilityComparison isInvisible(layerElement->GetClassId(), BOOLEAN_false);
    if (isInvisible(layerElement)) return FUNCTOR_SIBLINGS;
    // ignore accidentals outside the staff
    if (layerElement->Is(ACCID)) {
        Accid *accid = vrv_cast<Accid *>(layerElement);
        assert(accid);
        if (accid->GetFunc() == accidLog_FUNC_edit) return FUNCTOR_CONTINUE;
        if (accid->HasPlace()) return FUNCTOR_CONTINUE;
    }
    const StemmedDrawingInterface *stemInterface = layerElement->GetStemmedDrawingInterface();
    if (stemInterface
        && (((m_directionBias == 1) && (stemInterface->GetDrawingStemDir() == STEMDIRECTION_up))
            || ((m_directionBias == -1) && (stemInterface->GetDrawingStemDir() == STEMDIRECTION_down)))) {
        return FUNCTOR_CONTINUE;
    }

    Staff *staff = layerElement->GetAncestorStaff();

    // check if top/bottom of the element overlaps with beam coordinates
    int leftMargin = 0, rightMargin = 0;
    const auto [above, below] = outerBeamInterface->GetAdditionalBeamCount();
    int beamCount = std::max(above, below);
    if (m_outerFTrem) --beamCount;
    const int currentBeamYLeft = m_y1 + m_beamSlope * (layerElement->GetContentLeft() - m_x1);
    const int currentBeamYRight = m_y1 + m_beamSlope * (layerElement->GetContentRight() - m_x1);
    if (m_directionBias > 0) {
        leftMargin = layerElement->GetContentTop() - currentBeamYLeft + beamCount * outerBeamInterface->m_beamWidth
            + outerBeamInterface->m_beamWidthBlack;
        rightMargin = layerElement->GetContentTop() - currentBeamYRight + beamCount * outerBeamInterface->m_beamWidth
            + outerBeamInterface->m_beamWidthBlack;
    }
    else {
        leftMargin = layerElement->GetContentBottom() - currentBeamYLeft - beamCount * outerBeamInterface->m_beamWidth
            - outerBeamInterface->m_beamWidthBlack;
        rightMargin = layerElement->GetContentBottom() - currentBeamYRight - beamCount * outerBeamInterface->m_beamWidth
            - outerBeamInterface->m_beamWidthBlack;
    }

    const int overlapMargin = std::max(leftMargin * m_directionBias, rightMargin * m_directionBias);
    if (overlapMargin >= m_directionBias * m_overlapMargin) {
        const int staffOffset = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        m_overlapMargin = (((overlapMargin + staffOffset - 1) / staffOffset + 0.5) * staffOffset) * m_directionBias;
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustBeamsFunctor::VisitRest(Rest *rest)
{
    if (!m_outerBeam) return FUNCTOR_SIBLINGS;

    // Calculate possible overlap for the rest with beams
    const int beams = m_outerBeam->GetBeamPartDuration(rest, false) - DURATION_4;
    const int beamWidth = m_outerBeam->m_beamWidth;
    int overlapMargin = rest->Intersects(m_outerBeam, SELF, beams * beamWidth, true) * m_directionBias;

    // Adjust drawing location for the rest based on the overlap with beams.
    // Adjustment should be an even number, so that the rest is positioned properly
    if (overlapMargin >= 0) return FUNCTOR_CONTINUE;

    Staff *staff = rest->GetAncestorStaff();

    if ((!rest->HasOloc() || !rest->HasPloc()) && !rest->HasLoc()) {
        // constants
        const int unit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        // calculate new and old locations for the rest
        const int locAdjust = (m_directionBias * (overlapMargin - 2 * unit + 1) / unit);
        const int oldLoc = rest->GetDrawingLoc();
        const int newLoc = oldLoc + locAdjust - locAdjust % 2;
        if (staff->GetChildCount(LAYER) == 1) {
            rest->SetDrawingLoc(newLoc);
            rest->SetDrawingYRel(staff->CalcPitchPosYRel(m_doc, newLoc));
            // If there are dots, adjust their location as well
            if (rest->GetDots() > 0) {
                Dots *dots = vrv_cast<Dots *>(rest->FindDescendantByType(DOTS, 1));
                if (dots) {
                    std::set<int> &dotLocs = dots->ModifyDotLocsForStaff(staff);
                    const int dotLoc = (oldLoc % 2) ? oldLoc : oldLoc + 1;
                    if (std::find(dotLocs.cbegin(), dotLocs.cend(), dotLoc) != dotLocs.cend()) {
                        dotLocs.erase(dotLoc);
                        dotLocs.insert(newLoc);
                    }
                }
            }
            return FUNCTOR_CONTINUE;
        }
    }

    overlapMargin *= -m_directionBias;
    const int unit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    const int adjust = this->AdjustOverlapToHalfUnit(overlapMargin, unit);
    if (std::abs(adjust) > std::abs(m_overlapMargin)) m_overlapMargin = adjust;

    return FUNCTOR_CONTINUE;
}

BeamDrawingInterface *AdjustBeamsFunctor::GetOuterBeamInterface() const
{
    if (m_outerBeam) return m_outerBeam;
    if (m_outerFTrem) return m_outerFTrem;
    return NULL;
}

int AdjustBeamsFunctor::CalcLayerOverlap(const LayerElement *beamElement) const
{
    assert(beamElement);

    const Layer *parentLayer = vrv_cast<const Layer *>(beamElement->GetFirstAncestor(LAYER));
    if (!parentLayer) return 0;
    // Check whether there are elements on the other layer in the duration of the current beam
    ListOfConstObjects collidingElementsList = parentLayer->GetLayerElementsForTimeSpanOf(beamElement, true);
    // Ignore any elements part of a stem-sameas beam
    if (beamElement->Is(BEAM)) {
        const Beam *beam = vrv_cast<const Beam *>(beamElement);
        const Beam *stemSameAsBeam = beam->GetStemSameasBeam();
        if (stemSameAsBeam) {
            collidingElementsList.remove_if([stemSameAsBeam](const Object *object) {
                const LayerElement *layerElement = vrv_cast<const LayerElement *>(object);
                return (layerElement->GetAncestorBeam() == stemSameAsBeam);
            });
        }
    }
    // If there are none - stop here, there's nothing to be done
    if (collidingElementsList.empty()) return 0;

    const Staff *staff = beamElement->GetAncestorStaff();
    const int drawingY = beamElement->GetDrawingY();
    const int yMin = std::min(m_y1, m_y2);
    const int yMax = std::max(m_y1, m_y2);
    const int unit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);

    int elementOverlap = 0;
    std::vector<int> elementOverlaps;
    for (const Object *object : collidingElementsList) {
        const LayerElement *layerElement = vrv_cast<const LayerElement *>(object);
        if (!beamElement->HorizontalContentOverlap(object)) continue;
        const int elementBottom = layerElement->GetContentBottom();
        const int elementTop = layerElement->GetContentTop();
        if (m_directionBias > 0) {
            // Ensure that there's actual overlap first
            if (elementBottom > yMax) continue;
            if (drawingY >= elementTop) continue;
            // If there is a mild overlap, then decrease the beam stem length via negative overlap
            if (elementBottom > yMax - 3 * unit) {
                elementOverlap = std::min(elementBottom - yMax, 0);
            }
            else {
                elementOverlap = std::max(elementTop - yMin, 0);
            }
        }
        else {
            // Ensure that there's actual overlap first
            if (elementTop < yMin) continue;
            if (drawingY <= elementBottom) continue;
            // If there is a mild overlap, then decrease the beam stem length via negative overlap
            if (elementTop < yMin + 3 * unit) {
                elementOverlap = std::min(yMin - elementTop, 0);
            }
            else {
                elementOverlap = std::max(yMax - elementBottom, 0);
            }
        }
        elementOverlaps.emplace_back(elementOverlap);
    }
    if (elementOverlaps.empty()) return 0;

    const auto [minOverlap, maxOverlap] = std::minmax_element(elementOverlaps.begin(), elementOverlaps.end());
    int overlap = 0;
    if (*maxOverlap > 0) {
        overlap = *maxOverlap * m_directionBias;
    }
    else if (*minOverlap < 0) {
        overlap = (*minOverlap - unit) * m_directionBias;
    }
    const int adjust = this->AdjustOverlapToHalfUnit(overlap, unit);
    return adjust;
}

int AdjustBeamsFunctor::AdjustOverlapToHalfUnit(int overlap, int unit) const
{
    const int overlapSign = (overlap >= 0) ? 1 : -1;
    const int halfUnit = unit / 2;
    const int halfUnitChangeNumber = (std::abs(overlap) + halfUnit / 2) / halfUnit;
    return halfUnitChangeNumber * halfUnit * overlapSign;
}

} // namespace vrv
