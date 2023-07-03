/////////////////////////////////////////////////////////////////////////////
// Name:        adjusttupletsyfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjusttupletsyfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "elementpart.h"
#include "floatingobject.h"
#include "ftrem.h"
#include "staff.h"
#include "stem.h"
#include "tuplet.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustTupletsYFunctor
//----------------------------------------------------------------------------

AdjustTupletsYFunctor::AdjustTupletsYFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode AdjustTupletsYFunctor::VisitTuplet(Tuplet *tuplet)
{
    // Nothing to do if there is no number
    if (!tuplet->HasNum()) {
        return FUNCTOR_SIBLINGS;
    }

    // Nothing to do if the bracket and the num are not visible
    if ((tuplet->GetBracketVisible() == BOOLEAN_false) && (tuplet->GetNumVisible() == BOOLEAN_false)) {
        return FUNCTOR_SIBLINGS;
    }

    if (!tuplet->GetDrawingLeft() || !tuplet->GetDrawingRight()) {
        return FUNCTOR_SIBLINGS;
    }

    Staff *staff = tuplet->GetAncestorStaff();

    assert(tuplet->GetDrawingBracketPos() != STAFFREL_basic_NONE);

    Staff *relevantStaff = tuplet->m_crossStaff ? tuplet->m_crossStaff : staff;

    this->AdjustTupletBracketY(tuplet, relevantStaff);

    this->AdjustTupletNumY(tuplet, relevantStaff);

    return FUNCTOR_SIBLINGS;
}

void AdjustTupletsYFunctor::AdjustTupletBracketY(Tuplet *tuplet, const Staff *staff) const
{
    TupletBracket *tupletBracket = vrv_cast<TupletBracket *>(tuplet->GetFirst(TUPLET_BRACKET));
    if (!tupletBracket || (tuplet->GetBracketVisible() == BOOLEAN_false)) return;

    // if bracket is used for beam elements - process that part separately
    Beam *beam = tuplet->GetBracketAlignedBeam();
    if (beam) return this->AdjustTupletBracketBeamY(tuplet, tupletBracket, beam, staff);

    const int staffSize = staff->m_drawingStaffSize;
    const data_STAFFREL_basic bracketPos = tuplet->GetDrawingBracketPos();

    // Default position is above or below the staff
    const int staffBoundary = (bracketPos == STAFFREL_basic_above) ? 0 : -m_doc->GetDrawingStaffSize(staffSize);
    const int bracketMidX = (tupletBracket->GetDrawingXLeft() + tupletBracket->GetDrawingXRight()) / 2;
    const Point referencePos(bracketMidX, staff->GetDrawingY() + staffBoundary);

    // Check for overlap with content
    ListOfObjects descendants;
    ClassIdsComparison comparison({ ARTIC, ACCID, DOT, FLAG, NOTE, REST, STEM });
    tuplet->FindAllDescendantsByComparison(&descendants, &comparison);

    std::list<Point> obstacles;
    for (Object *descendant : descendants) {
        if (!descendant->HasSelfBB()) continue;
        if (vrv_cast<LayerElement *>(descendant)->m_crossStaff) continue;
        const int obstacleY
            = (bracketPos == STAFFREL_basic_above) ? descendant->GetSelfTop() : descendant->GetSelfBottom();
        obstacles.push_back({ descendant->GetDrawingX(), obstacleY });
    }

    // Calculate the horizontal bracket first
    const int unit = m_doc->GetDrawingUnit(staffSize);
    const int sign = (bracketPos == STAFFREL_basic_above) ? 1 : -1;
    const int horizontalBracketShift = this->CalcBracketShift(referencePos, 0.0, sign, obstacles);
    int optimalTilt = 0;
    int optimalShift = horizontalBracketShift;

    if (!m_doc->GetOptions()->m_tupletAngledOnBeams.GetValue()) {
        // Now try different angles and possibly find a better position
        const int bracketWidth = tupletBracket->GetDrawingXRight() - tupletBracket->GetDrawingXLeft();
        const MelodicDirection direction = tuplet->GetMelodicDirection();
        for (int tilt : { -4, -2, 2, 4 }) {
            if (bracketWidth == 0) continue;
            // Drop if angle does not fit to the melodic direction
            if ((direction == MelodicDirection::Up) && (tilt < 0)) continue;
            if ((direction == MelodicDirection::Down) && (tilt > 0)) continue;
            // Calculate the shift for the angle
            const double slope = tilt * unit / double(bracketWidth);
            const int shift = this->CalcBracketShift(referencePos, slope, sign, obstacles);
            // Drop angled brackets that would go into the staff
            if (shift < abs(tilt) * unit / 2) continue;
            // Drop angled brackets where the midpoint is moved only slightly closer to the staff
            if (shift > horizontalBracketShift - abs(tilt) * unit / 4) continue;
            // Update the optimal tilt
            if (shift < optimalShift) {
                optimalShift = shift;
                optimalTilt = tilt;
            }
        }
    }

    const int verticalMargin = 2 * unit;
    tupletBracket->SetDrawingYRel(staffBoundary + sign * (optimalShift + verticalMargin));
    tupletBracket->SetDrawingYRelLeft(-optimalTilt * unit / 2);
    tupletBracket->SetDrawingYRelRight(optimalTilt * unit / 2);
}

void AdjustTupletsYFunctor::AdjustTupletNumY(Tuplet *tuplet, const Staff *staff) const
{
    TupletNum *tupletNum = vrv_cast<TupletNum *>(tuplet->GetFirst(TUPLET_NUM));
    if (!tupletNum || (tuplet->GetNumVisible() == BOOLEAN_false)) return;

    // The num is within a bracket
    if (tupletNum->GetAlignedBracket()) {
        // yRel is not used for drawing but we need to adjust it for the bounding box to follow the changes
        tupletNum->SetDrawingYRel(tupletNum->GetAlignedBracket()->GetDrawingYRel());
        return;
    }

    Beam *beam = tuplet->GetNumAlignedBeam();
    tuplet->CalculateTupletNumCrossStaff(tupletNum);
    // Additional checks are required if tuplet is fully cross-staff and is part of the cross-staff beam. If beam is not
    // fully cross-staff and has more elements than tuplet does, we need to adjust tuplet number position accordingly to
    // make sure that there is no overlap.
    bool isPartialBeamTuplet = false;
    if (beam && tuplet->m_crossStaff) {
        const auto coords = beam->m_beamSegment.GetElementCoordRefs();
        ListOfObjects descendants;
        ClassIdsComparison comparison({ CHORD, NOTE, REST });
        tuplet->FindAllDescendantsByComparison(&descendants, &comparison);
        if ((beam->m_beamSegment.m_nbNotesOrChords > static_cast<int>(descendants.size()))
            && std::any_of(coords->begin(), coords->end(),
                [](const auto coord) { return NULL == coord->m_element->m_crossStaff; })) {
            if (!tuplet->HasValidTupletNumPosition(tupletNum->m_crossStaff, beam->m_beamStaff)) {
                tupletNum->m_crossStaff = beam->m_beamStaff;
            }
            isPartialBeamTuplet = true;
        }
    }

    const Staff *tupletNumStaff = tupletNum->m_crossStaff ? tupletNum->m_crossStaff : staff;
    const int staffSize = staff->m_drawingStaffSize;
    const int yReference = tupletNumStaff->GetDrawingY();
    const int doubleUnit = m_doc->GetDrawingDoubleUnit(staffSize);

    // The num is on its own
    const data_STAFFREL_basic numPos = tuplet->GetDrawingNumPos();
    const int numVerticalMargin = (numPos == STAFFREL_basic_above) ? doubleUnit : -doubleUnit;
    const int staffHeight = m_doc->GetDrawingStaffSize(staffSize);
    const int adjustedPosition = (numPos == STAFFREL_basic_above) ? 0 : -staffHeight;
    if (!beam) {
        tupletNum->SetDrawingYRel(adjustedPosition);
    }

    // Calculate relative Y for the tupletNum
    const int margin = 2 * m_doc->GetDrawingUnit(staffSize);
    AdjustTupletNumOverlapFunctor adjustTupletNumOverlap(tupletNum, tupletNumStaff, numPos, tupletNum->GetDrawingY());
    adjustTupletNumOverlap.SetHorizontalMargin(margin);
    tuplet->Process(adjustTupletNumOverlap);
    int yRel = adjustTupletNumOverlap.GetDrawingY() - yReference;

    // If we have a beam, see if we can move it to more appropriate position
    if (beam && (!tuplet->m_crossStaff || isPartialBeamTuplet) && !tuplet->FindDescendantByType(ARTIC)) {
        const int xMid = tupletNum->GetDrawingXMid(m_doc);
        const int yMid = beam->m_beamSegment.GetStartingY()
            + beam->m_beamSegment.m_beamSlope * (xMid - beam->m_beamSegment.GetStartingX());
        const int beamYRel = yMid - yReference + numVerticalMargin;
        if (((numPos == STAFFREL_basic_above) && (beamYRel > 0))
            || ((numPos == STAFFREL_basic_below) && (beamYRel < -staffHeight))) {
            yRel = beamYRel;
        }
    }
    else {
        yRel += numVerticalMargin;
    }

    // If yRel turns out to be too far from the tuplet - try to adjust it accordingly, aligning with the staff
    // top/bottom sides, unless doing so will make tuplet number overlap
    if (((numPos == STAFFREL_basic_below) && (yRel > adjustedPosition))
        || ((numPos == STAFFREL_basic_above) && (yRel < adjustedPosition))) {
        yRel = adjustedPosition;
    }

    FTrem *fTremChild = vrv_cast<FTrem *>(tuplet->FindDescendantByType(FTREM));
    if (fTremChild) {
        const ArrayOfBeamElementCoords *beamElementCoords = fTremChild->GetElementCoords();
        BeamElementCoord *firstElement = beamElementCoords->at(0);
        BeamElementCoord *secondElement = beamElementCoords->at(1);

        const int y1 = firstElement->m_yBeam;
        const int y2 = secondElement->m_yBeam;
        const int currentPosition = tuplet->GetDrawingY() + yRel;
        if ((numPos == STAFFREL_basic_above) && (currentPosition < (y1 + y2) / 2)) {
            yRel += (y1 + y2) / 2 - currentPosition;
        }
        else if ((numPos == STAFFREL_basic_below) && (currentPosition + margin > (y1 + y2) / 2)) {
            yRel += (y1 + y2) / 2 - (currentPosition + margin);
        }
    }

    tupletNum->SetDrawingYRel(yRel);
}

void AdjustTupletsYFunctor::AdjustTupletBracketBeamY(
    Tuplet *tuplet, TupletBracket *bracket, const Beam *beam, const Staff *staff) const
{
    const int staffSize = staff->m_drawingStaffSize;
    const int doubleUnit = m_doc->GetDrawingDoubleUnit(staffSize);
    const data_STAFFREL_basic bracketPos = tuplet->GetDrawingBracketPos();
    const int sign = (bracketPos == STAFFREL_basic_above) ? 1 : -1;
    int bracketVerticalMargin = sign * doubleUnit;

    // Check for possible articulations
    ListOfObjects artics = tuplet->FindAllDescendantsByType(ARTIC);

    int articPadding = 0;
    for (Object *artic : artics) {
        if (!artic->HasSelfBB()) continue;
        if (bracketPos == STAFFREL_basic_above) {
            // Left point when slope is going up and right when going down
            int relevantX = (beam->m_beamSegment.m_beamSlope > 0) ? artic->GetSelfLeft() : artic->GetSelfRight();
            int currentYRel = beam->m_beamSegment.GetStartingY()
                + beam->m_beamSegment.m_beamSlope * (relevantX - beam->m_beamSegment.GetStartingX());
            int articYRel = artic->GetSelfTop();
            articPadding = std::min(currentYRel - articYRel, articPadding);
        }
        else {
            // Right point when slope is going up and left when going down
            int relevantX = (beam->m_beamSegment.m_beamSlope > 0) ? artic->GetSelfRight() : artic->GetSelfLeft();
            int currentYRel = beam->m_beamSegment.GetStartingY()
                + beam->m_beamSegment.m_beamSlope * (relevantX - beam->m_beamSegment.GetStartingX());
            int articYRel = artic->GetSelfBottom();
            articPadding = std::max(currentYRel - articYRel, articPadding);
        }
    }

    // Check for overlap with rest elements. This might happen when tuplet has rest and beam children that are
    // on the same level in encoding - there might be overlap of bracket with rest in that case
    ListOfObjects descendants = tuplet->FindAllDescendantsByType(REST);

    int restAdjust = 0;
    const int bracketRel = bracket->GetDrawingYRel() - articPadding + bracketVerticalMargin;
    const int bracketPosition = (bracket->GetSelfTop() + bracket->GetSelfBottom() + bracketRel) / 2;
    for (Object *descendant : descendants) {
        if (descendant->GetFirstAncestor(BEAM) || !descendant->HasSelfBB()) continue;
        if (bracketPos == STAFFREL_basic_above) {
            if (bracketPosition < descendant->GetSelfTop()) {
                const int verticalShift = descendant->GetSelfTop() - bracketPosition;
                if ((restAdjust == 0) || (restAdjust < verticalShift)) restAdjust = verticalShift;
            }
        }
        else {
            if (bracketPosition > descendant->GetSelfBottom()) {
                const int verticalShift = descendant->GetSelfBottom() - bracketPosition;
                if ((restAdjust == 0) || (restAdjust > verticalShift)) restAdjust = verticalShift;
            }
        }
    }
    if (restAdjust) bracketVerticalMargin += restAdjust;

    const int yReference = staff->GetDrawingY();
    bracket->SetDrawingYRel(bracket->GetDrawingYRel() - articPadding + bracketVerticalMargin);

    // Make sure that there are no overlaps with staff lines
    const int staffMargin = (bracketPos == STAFFREL_basic_above)
        ? yReference + doubleUnit
        : yReference - m_doc->GetDrawingStaffSize(staffSize) - doubleUnit;

    const int leftMargin = sign * (staffMargin - bracket->GetDrawingYLeft());
    const int rightMargin = sign * (staffMargin - bracket->GetDrawingYRight());
    const int maxMargin = std::max(leftMargin, rightMargin);

    if (maxMargin > 0) {
        int bracketAdjust = 0;
        if ((leftMargin > 0) && (rightMargin > 0)) {
            bracketAdjust = std::min(leftMargin, rightMargin);
        }

        if (bracketAdjust > 0) bracket->SetDrawingYRel(bracket->GetDrawingYRel() + sign * bracketAdjust);
        if (leftMargin > 0) bracket->SetDrawingYRelLeft(sign * (leftMargin - bracketAdjust));
        if (rightMargin > 0) bracket->SetDrawingYRelRight(sign * (rightMargin - bracketAdjust));
    }

    if (beam->m_crossStaffContent) {
        if ((bracketPos == STAFFREL_basic_below) && (beam->m_crossStaffContent->GetN() > staff->GetN())) {
            bracket->SetDrawingYRelLeft(bracket->GetDrawingYRelLeft() - doubleUnit / 4);
            bracket->SetDrawingYRelRight(bracket->GetDrawingYRelRight() - doubleUnit / 4);
        }
    }
}

int AdjustTupletsYFunctor::CalcBracketShift(
    Point referencePos, double slope, int sign, const std::list<Point> &obstacles) const
{
    int shift = 0;
    for (Point obstacle : obstacles) {
        const double lineShift = obstacle.y - slope * obstacle.x;
        const int dist = slope * referencePos.x + lineShift - referencePos.y;
        shift = std::max(dist * sign, shift);
    }
    return shift;
}

//----------------------------------------------------------------------------
// AdjustTupletNumOverlapFunctor
//----------------------------------------------------------------------------

AdjustTupletNumOverlapFunctor::AdjustTupletNumOverlapFunctor(
    const TupletNum *tupletNum, const Staff *staff, data_STAFFREL_basic drawingNumPos, int drawingY)
{
    m_tupletNum = tupletNum;
    m_drawingNumPos = drawingNumPos;
    m_horizontalMargin = 0;
    m_verticalMargin = 0;
    m_staff = staff;
    m_drawingY = drawingY;
}

FunctorCode AdjustTupletNumOverlapFunctor::VisitLayerElement(const LayerElement *layerElement)
{
    if (!layerElement->Is({ ACCID, ARTIC, CHORD, DOT, FLAG, NOTE, REST, STEM }) || !layerElement->HasSelfBB())
        return FUNCTOR_CONTINUE;

    if (layerElement->Is({ CHORD, NOTE, REST })
        && ((layerElement->m_crossStaff || (layerElement->GetFirstAncestor(STAFF) != m_staff))
            && (layerElement->m_crossStaff != m_staff)))
        return FUNCTOR_SIBLINGS;

    if (!m_tupletNum->HorizontalSelfOverlap(layerElement, m_horizontalMargin)
        && !m_tupletNum->VerticalSelfOverlap(layerElement, m_verticalMargin)) {
        return FUNCTOR_CONTINUE;
    }

    int stemAdjust = 0;
    if (layerElement->Is(STEM)) {
        const Stem *stem = vrv_cast<const Stem *>(layerElement);
        stemAdjust = stem->GetDrawingStemAdjust();
    }
    if (m_drawingNumPos == STAFFREL_basic_above) {
        int dist = layerElement->GetSelfTop();
        if (m_drawingY < dist) m_drawingY = dist + stemAdjust;
    }
    else {
        int dist = layerElement->GetSelfBottom();
        if (m_drawingY > dist) m_drawingY = dist + stemAdjust;
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// AdjustTupletWithSlursFunctor
//----------------------------------------------------------------------------

AdjustTupletWithSlursFunctor::AdjustTupletWithSlursFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode AdjustTupletWithSlursFunctor::VisitTuplet(Tuplet *tuplet)
{
    TupletBracket *tupletBracket = vrv_cast<TupletBracket *>(tuplet->GetFirst(TUPLET_BRACKET));
    if (!tupletBracket || tuplet->GetInnerSlurs().empty()) {
        return FUNCTOR_SIBLINGS;
    }
    TupletNum *tupletNum = vrv_cast<TupletNum *>(tuplet->GetFirst(TUPLET_NUM));

    const Staff *staff = tuplet->GetAncestorStaff(RESOLVE_CROSS_STAFF);
    const int margin = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2;
    const data_STAFFREL_basic bracketPos = tuplet->GetDrawingBracketPos();
    const int sign = (bracketPos == STAFFREL_basic_above) ? 1 : -1;

    const int xLeft = tuplet->GetDrawingLeft()->GetDrawingX() + tupletBracket->GetDrawingXRelLeft();
    const int xRight = tuplet->GetDrawingRight()->GetDrawingX() + tupletBracket->GetDrawingXRelRight();
    const int yLeft = tupletBracket->GetDrawingYLeft();
    const int yRight = tupletBracket->GetDrawingYRight();
    const double tupletSlope = double(yRight - yLeft) / double(xRight - xLeft);
    int tupletShift = 0;

    for (auto curve : tuplet->GetInnerSlurs()) {
        const int shift = tupletBracket->Intersects(curve, CONTENT, margin) * sign;
        if (shift > 0) {
            // The shift is calculated from the entire bounding box of the tuplet bracket.
            // If the bracket is angled and the slur is short, then this might be too coarse.
            // We reduce the shift by the height of the subbox that cannot be hit.
            Point points[4];
            curve->GetPoints(points);
            const int curveXLeft = std::max(points[0].x, xLeft);
            const int curveXRight = std::min(points[3].x, xRight);
            const int curveYLeft = tupletSlope * (curveXLeft - xLeft) + yLeft;
            const int curveYRight = tupletSlope * (curveXRight - xLeft) + yLeft;

            int reduction = 0;
            if (bracketPos == STAFFREL_basic_above) {
                reduction = std::min(curveYLeft, curveYRight) - std::min(yLeft, yRight);
            }
            else {
                reduction = std::max(yLeft, yRight) - std::max(curveYLeft, curveYRight);
            }
            tupletShift = std::max(shift - reduction, tupletShift);
        }
    }

    // Apply the tuplet shift from slurs
    if (tupletShift) {
        tupletBracket->SetDrawingYRel(tupletBracket->GetDrawingYRel() + sign * tupletShift);
        if (tupletNum) tupletNum->SetDrawingYRel(tupletNum->GetDrawingYRel() + sign * tupletShift);
    }

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
