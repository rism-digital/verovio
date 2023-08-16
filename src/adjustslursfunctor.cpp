/////////////////////////////////////////////////////////////////////////////
// Name:        adjustslursfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustslursfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "floatingobject.h"
#include "slur.h"
#include "staff.h"
#include "system.h"
#include "verticalaligner.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustSlursFunctor
//----------------------------------------------------------------------------

AdjustSlursFunctor::AdjustSlursFunctor(Doc *doc) : DocFunctor(doc)
{
    m_crossStaffSlurs = false;
    this->ResetCurrent();
}

void AdjustSlursFunctor::ResetCurrent()
{
    m_currentCurve = NULL;
    m_currentSlur = NULL;
}

FunctorCode AdjustSlursFunctor::VisitStaffAlignment(StaffAlignment *staffAlignment)
{
    Staff *staff = staffAlignment->GetStaff();
    if (!staff) return FUNCTOR_CONTINUE;
    const int unit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);

    // Adjust each slur such that spanned elements are avoided
    ArrayOfFloatingCurvePositioners positioners;
    for (FloatingPositioner *positioner : staffAlignment->GetFloatingPositioners()) {
        assert(positioner->GetObject());
        if (!positioner->GetObject()->Is({ PHRASE, SLUR })) continue;
        Slur *slur = vrv_cast<Slur *>(positioner->GetObject());
        assert(slur);
        m_currentSlur = slur;

        assert(positioner->Is(FLOATING_CURVE_POSITIONER));
        FloatingCurvePositioner *curve = vrv_cast<FloatingCurvePositioner *>(positioner);
        assert(curve);
        m_currentCurve = curve;

        // Skip if no content bounding box is available
        if (!curve->HasContentBB()) continue;
        positioners.push_back(curve);

        this->AdjustSlur(unit);

        if (curve->IsCrossStaff()) {
            m_crossStaffSlurs = true;
        }
    }

    this->ResetCurrent();

    // Detection of inner slurs
    std::map<FloatingCurvePositioner *, ArrayOfFloatingCurvePositioners> innerCurveMap;
    for (int i = 0; i < (int)positioners.size(); ++i) {
        Slur *firstSlur = vrv_cast<Slur *>(positioners[i]->GetObject());
        ArrayOfFloatingCurvePositioners innerCurves;
        for (int j = 0; j < (int)positioners.size(); ++j) {
            if (i == j) continue;
            Slur *secondSlur = vrv_cast<Slur *>(positioners[j]->GetObject());
            // Check if second slur is inner slur of first
            if (positioners[j]->GetSpanningType() == SPANNING_START_END) {
                if (firstSlur->HasInnerSlur(secondSlur)) {
                    innerCurves.push_back(positioners[j]);
                    continue;
                }
            }
            // Adjust positioning of slurs with common start/end
            Point points1[4], points2[4];
            positioners[i]->GetPoints(points1);
            positioners[j]->GetPoints(points2);
            if ((firstSlur->GetEnd() == secondSlur->GetStart())
                && BoundingBox::ArePointsClose(points1[3], points2[0], unit)) {
                positioners[i]->MoveBackHorizontal(-unit / 2);
                positioners[j]->MoveFrontHorizontal(unit / 2);
            }
            if ((firstSlur->GetStart() == secondSlur->GetStart())
                && BoundingBox::ArePointsClose(points1[0], points2[0], unit) && (points1[3].x > points2[3].x)) {
                int diff = points2[0].y - points1[0].y;
                diff += ((positioners[i]->GetDir() == curvature_CURVEDIR_below) ? -unit : unit);
                positioners[i]->MoveFrontVertical(diff);
            }
            if ((firstSlur->GetEnd() == secondSlur->GetEnd())
                && BoundingBox::ArePointsClose(points1[3], points2[3], unit) && (points1[0].x < points2[0].x)) {
                int diff = points2[3].y - points1[3].y;
                diff += ((positioners[i]->GetDir() == curvature_CURVEDIR_below) ? -unit : unit);
                positioners[i]->MoveBackVertical(diff);
            }
        }
        if (!innerCurves.empty()) {
            innerCurveMap[positioners[i]] = innerCurves;
        }
    }

    // Adjust outer slurs w.r.t. inner slurs
    for (const auto &mapEntry : innerCurveMap) {
        Slur *slur = vrv_cast<Slur *>(mapEntry.first->GetObject());
        assert(slur);
        m_currentCurve = mapEntry.first;
        m_currentSlur = slur;

        this->AdjustOuterSlur(mapEntry.second, unit);
    }

    this->ResetCurrent();

    return FUNCTOR_SIBLINGS;
}

FunctorCode AdjustSlursFunctor::VisitSystem(System *system)
{
    system->m_systemAligner.Process(*this);

    return FUNCTOR_SIBLINGS;
}

void AdjustSlursFunctor::AdjustSlur(int unit) const
{
    assert(m_currentCurve);
    assert(m_currentSlur);

    Point points[4];
    m_currentCurve->GetPoints(points);
    BezierCurve bezier(points[0], points[1], points[2], points[3]);
    m_currentSlur->InitBezierControlSides(bezier, m_currentCurve->GetDir());
    bezier.UpdateControlPointParams();

    const int margin = m_doc->GetOptions()->m_slurMargin.GetValue() * unit;
    const double flexibility = m_doc->GetOptions()->m_slurEndpointFlexibility.GetValue();
    const double symmetry = m_doc->GetOptions()->m_slurSymmetry.GetValue();

    // STEP 1: Filter spanned elements and discard certain bounding boxes even though they collide
    this->FilterSpannedElements(bezier, margin);

    // STEP 2: Detect collisions near the endpoints and switch to secondary endpoints if necessary
    NearEndCollision nearEndCollision = this->DetectCollisionsNearEnd(bezier, margin);
    m_currentSlur->CalcInitialCurve(m_doc, m_currentCurve, &nearEndCollision);
    if (nearEndCollision.endPointsAdjusted) {
        m_currentCurve->GetPoints(points);
        bezier.p1 = points[0];
        bezier.c1 = points[1];
        bezier.c2 = points[2];
        bezier.p2 = points[3];
        bezier.UpdateControlPointParams();
        m_currentSlur->CalcSpannedElements(m_currentCurve);
        this->FilterSpannedElements(bezier, margin);
    }
    else {
        m_currentCurve->UpdatePoints(bezier);
    }

    // STEP 3: Calculate the vertical adjustment of endpoints. This shifts the slur vertically.
    // Only collisions near the endpoints are taken into account.
    int endPointShiftLeft = 0;
    int endPointShiftRight = 0;
    std::tie(endPointShiftLeft, endPointShiftRight) = this->CalcEndPointShift(bezier, flexibility, margin);
    this->ApplyEndPointShift(bezier, endPointShiftLeft, endPointShiftRight);

    // Special handling if bulge is prescribed from here on
    if (m_currentSlur->HasBulge()) {
        this->AdjustSlurFromBulge(bezier, unit);
        return;
    }

    // STEP 4: Calculate the horizontal offset of the control points.
    // The idea is to shift control points to the outside if there is an obstacle in the vicinity of the corresponding
    // endpoint. For C1 we consider the largest angle <)BP1P2 where B is a colliding left bounding box corner and choose
    // C1 in this direction. Similar for C2.
    if (this->AllowControlOffsetAdjustment(bezier, symmetry, unit)) {
        bool ok = false;
        int controlPointOffsetLeft = 0;
        int controlPointOffsetRight = 0;
        std::tie(ok, controlPointOffsetLeft, controlPointOffsetRight) = this->CalcControlPointOffset(bezier, margin);
        if (ok) {
            bezier.SetLeftControlOffset(controlPointOffsetLeft);
            bezier.SetRightControlOffset(controlPointOffsetRight);
            bezier.UpdateControlPoints();
            m_currentCurve->UpdatePoints(bezier);
        }
    }

    // STEP 5: Calculate the vertical shift of the control points.
    // For each colliding bounding box we formulate a constraint ax + by >= c
    // where x, y denote the vertical adjustments of the control points and c is the size of the collision.
    // The coefficients a, b are calculated from the Bezier curve equation.
    // After collecting all constraints we calculate a solution.
    const ControlPointAdjustment adjustment = this->CalcControlPointVerticalShift(bezier, symmetry, margin);
    const int leftSign = (bezier.IsLeftControlAbove() == adjustment.moveUpwards) ? 1 : -1;
    bezier.SetLeftControlHeight(bezier.GetLeftControlHeight() + leftSign * adjustment.leftShift);
    const int rightSign = (bezier.IsRightControlAbove() == adjustment.moveUpwards) ? 1 : -1;
    bezier.SetRightControlHeight(bezier.GetRightControlHeight() + rightSign * adjustment.rightShift);
    bezier.UpdateControlPoints();
    m_currentCurve->UpdatePoints(bezier);
    m_currentCurve->SetRequestedStaffSpace(adjustment.requestedStaffSpace);

    // STEP 6: Adjust the slur shape
    // Through the control point adjustments in step 4 and 5 it can happen that the slur looses its desired shape.
    // We correct the shape if the slur is too flat or not convex.
    if (m_currentCurve->GetDir() != curvature_CURVEDIR_mixed) {
        this->AdjustSlurShape(bezier, m_currentCurve->GetDir(), unit);
        m_currentCurve->UpdatePoints(bezier);
    }

    // Since we are going to redraw it, reset its bounding box
    m_currentCurve->ResetBoundingBox();
}

void AdjustSlursFunctor::AdjustOuterSlur(const ArrayOfFloatingCurvePositioners &innerCurves, int unit) const
{
    assert(m_currentCurve);
    assert(m_currentSlur);

    Point points[4];
    m_currentCurve->GetPoints(points);
    BezierCurve bezier(points[0], points[1], points[2], points[3]);
    m_currentSlur->InitBezierControlSides(bezier, m_currentCurve->GetDir());
    bezier.UpdateControlPointParams();

    const int margin = m_doc->GetOptions()->m_slurMargin.GetValue() * unit;
    const double flexibility = m_doc->GetOptions()->m_slurEndpointFlexibility.GetValue();
    const double symmetry = m_doc->GetOptions()->m_slurSymmetry.GetValue();

    // STEP 1: Calculate the vertical adjustment of endpoints. This shifts the slur vertically.
    // Only collisions near the endpoints are taken into account.
    int endPointShiftLeft = 0;
    int endPointShiftRight = 0;
    std::tie(endPointShiftLeft, endPointShiftRight) = this->CalcEndPointShift(bezier, innerCurves, flexibility, margin);
    this->ApplyEndPointShift(bezier, endPointShiftLeft, endPointShiftRight);

    // STEP 2: Calculate the vertical shift of the control points.
    // For each inner slur we formulate some constraints ax + by >= c
    // where x, y denote the vertical adjustments of the control points and c is the size of the collision.
    // The coefficients a, b are calculated from the Bezier curve equation.
    // After collecting all constraints we calculate a solution.
    const ControlPointAdjustment adjustment = this->CalcControlPointShift(bezier, innerCurves, symmetry, margin);
    bezier.SetLeftControlHeight(bezier.GetLeftControlHeight() + adjustment.leftShift);
    bezier.SetRightControlHeight(bezier.GetRightControlHeight() + adjustment.rightShift);
    bezier.UpdateControlPoints();
    m_currentCurve->UpdatePoints(bezier);

    // STEP 3: Adjust the slur shape
    // We correct the shape if the slur is too flat or not convex.
    if (m_currentCurve->GetDir() != curvature_CURVEDIR_mixed) {
        this->AdjustSlurShape(bezier, m_currentCurve->GetDir(), unit);
        m_currentCurve->UpdatePoints(bezier);
    }

    // Since we are going to redraw it, reset its bounding box
    m_currentCurve->ResetBoundingBox();
}

void AdjustSlursFunctor::FilterSpannedElements(const BezierCurve &bezierCurve, int margin) const
{
    if (bezierCurve.p1.x >= bezierCurve.p2.x) return;

    const int dist = bezierCurve.p2.x - bezierCurve.p1.x;

    const ArrayOfCurveSpannedElements *spannedElements = m_currentCurve->GetSpannedElements();

    for (CurveSpannedElement *spannedElement : *spannedElements) {

        if (spannedElement->m_discarded) {
            continue;
        }

        bool discard = false;
        const int intersection = m_currentCurve->CalcDirectionalAdjustment(
            spannedElement->m_boundingBox, spannedElement->m_isBelow, discard, margin);
        const int xMiddle
            = (spannedElement->m_boundingBox->GetSelfLeft() + spannedElement->m_boundingBox->GetSelfRight()) / 2.0;
        const float distanceRatio = float(xMiddle - bezierCurve.p1.x) / float(dist);

        // Check if obstacles completely lie on the other side of the slur
        const int elementHeight
            = std::abs(spannedElement->m_boundingBox->GetSelfTop() - spannedElement->m_boundingBox->GetSelfBottom());
        if (intersection > elementHeight + 4 * margin) {
            // Ignore elements in a different layer near the endpoints
            const LayerElement *layerElement = dynamic_cast<const LayerElement *>(spannedElement->m_boundingBox);
            if (distanceRatio < 0.05) {
                spannedElement->m_discarded = layerElement
                    ? (layerElement->GetOriginalLayerN() != m_currentSlur->GetStart()->GetOriginalLayerN())
                    : true;
            }
            else if (distanceRatio > 0.95) {
                spannedElement->m_discarded = layerElement
                    ? (layerElement->GetOriginalLayerN() != m_currentSlur->GetEnd()->GetOriginalLayerN())
                    : true;
            }
            // Ignore tuplet numbers
            if (layerElement && layerElement->Is(TUPLET_NUM)) {
                spannedElement->m_discarded = true;
            }
        }
    }
}

NearEndCollision AdjustSlursFunctor::DetectCollisionsNearEnd(const BezierCurve &bezierCurve, int margin) const
{
    NearEndCollision nearEndCollision({ 0.0, 0.0, false });
    if (bezierCurve.p1.x >= bezierCurve.p2.x) return nearEndCollision;

    const ArrayOfCurveSpannedElements *spannedElements = m_currentCurve->GetSpannedElements();
    for (CurveSpannedElement *spannedElement : *spannedElements) {
        if (spannedElement->m_discarded) {
            continue;
        }

        bool discard = false;
        int intersectionLeft, intersectionRight;
        std::tie(intersectionLeft, intersectionRight) = m_currentCurve->CalcDirectionalLeftRightAdjustment(
            spannedElement->m_boundingBox, spannedElement->m_isBelow, discard, margin);

        if ((intersectionLeft > 0) || (intersectionRight > 0)) {
            Point points[4];
            points[0] = bezierCurve.p1;
            points[1] = bezierCurve.c1;
            points[2] = bezierCurve.c2;
            points[3] = bezierCurve.p2;

            // Adjust the collision metrics
            const int xLeft = std::max(bezierCurve.p1.x, spannedElement->m_boundingBox->GetSelfLeft());
            Point pLeft(xLeft, BoundingBox::CalcBezierAtPosition(points, xLeft));
            double distStart = std::max(BoundingBox::CalcDistance(bezierCurve.p1, pLeft), 1.0);
            double distEnd = std::max(BoundingBox::CalcDistance(bezierCurve.p2, pLeft), 1.0);
            nearEndCollision.metricAtStart = std::max(intersectionLeft / distStart, nearEndCollision.metricAtStart);
            nearEndCollision.metricAtEnd = std::max(intersectionLeft / distEnd, nearEndCollision.metricAtEnd);

            const int xRight = std::min(bezierCurve.p2.x, spannedElement->m_boundingBox->GetSelfRight());
            Point pRight(xRight, BoundingBox::CalcBezierAtPosition(points, xRight));
            distStart = std::max(BoundingBox::CalcDistance(bezierCurve.p1, pRight), 1.0);
            distEnd = std::max(BoundingBox::CalcDistance(bezierCurve.p2, pRight), 1.0);
            nearEndCollision.metricAtStart = std::max(intersectionRight / distStart, nearEndCollision.metricAtStart);
            nearEndCollision.metricAtEnd = std::max(intersectionRight / distEnd, nearEndCollision.metricAtEnd);
        }
    }

    return nearEndCollision;
}

std::pair<int, int> AdjustSlursFunctor::CalcEndPointShift(
    const BezierCurve &bezierCurve, double flexibility, int margin) const
{
    if (bezierCurve.p1.x >= bezierCurve.p2.x) return { 0, 0 };

    int shiftLeft = 0;
    int shiftRight = 0;

    const int dist = bezierCurve.p2.x - bezierCurve.p1.x;

    const ArrayOfCurveSpannedElements *spannedElements = m_currentCurve->GetSpannedElements();
    for (CurveSpannedElement *spannedElement : *spannedElements) {

        if (spannedElement->m_discarded) {
            continue;
        }

        bool discard = false;
        int intersectionLeft, intersectionRight;
        std::tie(intersectionLeft, intersectionRight) = m_currentCurve->CalcDirectionalLeftRightAdjustment(
            spannedElement->m_boundingBox, spannedElement->m_isBelow, discard, margin);

        if (discard) {
            spannedElement->m_discarded = true;
            continue;
        }

        if ((intersectionLeft > 0) || (intersectionRight > 0)) {
            // Now apply the intersections on the left and right hand side of the bounding box
            const int xLeft = std::max(bezierCurve.p1.x, spannedElement->m_boundingBox->GetSelfLeft());
            const float distanceRatioLeft = float(xLeft - bezierCurve.p1.x) / float(dist);
            this->ShiftEndPoints(shiftLeft, shiftRight, distanceRatioLeft, intersectionLeft, flexibility,
                spannedElement->m_isBelow, m_currentCurve->m_spanningType);

            const int xRight = std::min(bezierCurve.p2.x, spannedElement->m_boundingBox->GetSelfRight());
            const float distanceRatioRight = float(xRight - bezierCurve.p1.x) / float(dist);
            this->ShiftEndPoints(shiftLeft, shiftRight, distanceRatioRight, intersectionRight, flexibility,
                spannedElement->m_isBelow, m_currentCurve->m_spanningType);
        }
    }

    return { shiftLeft, shiftRight };
}

void AdjustSlursFunctor::ApplyEndPointShift(
    BezierCurve &bezierCurve, int endPointShiftLeft, int endPointShiftRight) const
{
    if ((endPointShiftLeft != 0) || (endPointShiftRight != 0)) {
        const int signLeft = bezierCurve.IsLeftControlAbove() ? 1 : -1;
        const int signRight = bezierCurve.IsRightControlAbove() ? 1 : -1;
        bezierCurve.p1.y += signLeft * endPointShiftLeft;
        bezierCurve.p2.y += signRight * endPointShiftRight;
        if (bezierCurve.p1.x != bezierCurve.p2.x) {
            double lambda1, lambda2;
            std::tie(lambda1, lambda2) = bezierCurve.EstimateCurveParamForControlPoints();
            bezierCurve.c1.y
                += signLeft * (1.0 - lambda1) * endPointShiftLeft + signRight * lambda1 * endPointShiftRight;
            bezierCurve.c2.y
                += signLeft * (1.0 - lambda2) * endPointShiftLeft + signRight * lambda2 * endPointShiftRight;
        }
        bezierCurve.UpdateControlPointParams();
        m_currentCurve->UpdatePoints(bezierCurve);
    }
}

void AdjustSlursFunctor::AdjustSlurFromBulge(BezierCurve &bezierCurve, int unit) const
{
    if (bezierCurve.p1.x >= bezierCurve.p2.x) return;

    data_BULGE bulge = m_currentSlur->GetBulge();

    // Filter admissible values
    bulge.erase(std::remove_if(bulge.begin(), bulge.end(),
                    [](const data_BULGE::value_type &entry) {
                        return ((entry.first <= 0.0) || (entry.second <= 0.0) || (entry.second >= 100.0));
                    }),
        bulge.end());

    // Get the minimal and maximal lambda
    double lambdaMin = 0.66;
    double lambdaMax = 0.33;
    for (const data_BULGE::value_type &bulgeEntry : bulge) {
        const double lambda = bulgeEntry.second / 100.0;
        lambdaMin = std::min(lambda, lambdaMin);
        lambdaMax = std::max(lambda, lambdaMax);
    }

    // Horizontal control point adjustment
    lambdaMin /= 2.0;
    lambdaMax = 1.0 - (1.0 - lambdaMax) / 2.0;
    const double xMin = (1.0 - lambdaMin) * bezierCurve.p1.x + lambdaMin * bezierCurve.p2.x;
    const double xMax = (1.0 - lambdaMax) * bezierCurve.p1.x + lambdaMax * bezierCurve.p2.x;
    bezierCurve.SetLeftControlOffset(xMin - bezierCurve.p1.x);
    bezierCurve.SetRightControlOffset(bezierCurve.p2.x - xMax);
    bezierCurve.UpdateControlPoints();
    m_currentCurve->UpdatePoints(bezierCurve);

    // Generate a control point constraint for each bulge entry
    std::list<ControlPointConstraint> constraints;
    Point points[4];
    points[0] = bezierCurve.p1;
    points[1] = bezierCurve.c1;
    points[2] = bezierCurve.c2;
    points[3] = bezierCurve.p2;

    for (const data_BULGE::value_type &bulgeEntry : bulge) {
        const double lambda = bulgeEntry.second / 100.0;
        const double x = (1.0 - lambda) * bezierCurve.p1.x + lambda * bezierCurve.p2.x;
        const double t = BoundingBox::CalcBezierParamAtPosition(points, x);
        constraints.push_back({ 3.0 * pow(1.0 - t, 2.0) * t, 3.0 * (1.0 - t) * pow(t, 2.0), bulgeEntry.first * unit });
    }

    // Solve these constraints and calculate the vertical control point adjustment
    int leftShift = 0;
    int rightShift = 0;
    std::tie(leftShift, rightShift) = this->SolveControlPointConstraints(constraints);
    bezierCurve.SetLeftControlHeight(bezierCurve.GetLeftControlHeight() + leftShift);
    bezierCurve.SetRightControlHeight(bezierCurve.GetRightControlHeight() + rightShift);
    bezierCurve.UpdateControlPoints();
    m_currentCurve->UpdatePoints(bezierCurve);

    // Prevent awkward slur shapes
    this->AdjustSlurShape(bezierCurve, m_currentCurve->GetDir(), unit);
    m_currentCurve->UpdatePoints(bezierCurve);

    // Since we are going to redraw it, reset its bounding box
    m_currentCurve->ResetBoundingBox();
}

bool AdjustSlursFunctor::AllowControlOffsetAdjustment(const BezierCurve &bezierCurve, double symmetry, int unit) const
{
    const double distance = BoundingBox::CalcDistance(bezierCurve.p1, bezierCurve.p2);

    return (distance > symmetry * 40 * unit);
}

std::tuple<bool, int, int> AdjustSlursFunctor::CalcControlPointOffset(const BezierCurve &bezierCurve, int margin) const
{
    if (bezierCurve.p1.x >= bezierCurve.p2.x) return { false, 0, 0 };

    // Initially we start with the slopes of the lines P1-C1 and P2-C2
    double leftSlopeMax = std::abs(BoundingBox::CalcSlope(bezierCurve.p1, bezierCurve.c1));
    double rightSlopeMax = std::abs(BoundingBox::CalcSlope(bezierCurve.p2, bezierCurve.c2));
    const ArrayOfCurveSpannedElements *spannedElements = m_currentCurve->GetSpannedElements();
    for (CurveSpannedElement *spannedElement : *spannedElements) {

        if (spannedElement->m_discarded) {
            continue;
        }

        const int bbY = spannedElement->m_isBelow ? spannedElement->m_boundingBox->GetSelfTop()
                                                  : spannedElement->m_boundingBox->GetSelfBottom();
        const Point pLeft = { spannedElement->m_boundingBox->GetSelfLeft(), bbY };
        const Point pRight = { spannedElement->m_boundingBox->GetSelfRight(), bbY };

        // Prefer the (increased) slope of P1-B1, if larger
        // B1 is the upper left bounding box corner of a colliding obstacle
        if ((pLeft.x > bezierCurve.p1.x + margin) && (bezierCurve.IsLeftControlAbove() == spannedElement->m_isBelow)) {
            const double slope = BoundingBox::CalcSlope(bezierCurve.p1, pLeft);
            if ((slope > 0.0) && bezierCurve.IsLeftControlAbove()) {
                const double adjustedSlope = this->RotateSlope(slope, 10.0, 2.5, true);
                leftSlopeMax = std::max(leftSlopeMax, adjustedSlope);
            }
            if ((slope < 0.0) && !bezierCurve.IsLeftControlAbove()) {
                const double adjustedSlope = this->RotateSlope(-slope, 10.0, 2.5, true);
                leftSlopeMax = std::max(leftSlopeMax, adjustedSlope);
            }
        }

        // Prefer the (increased) slope of P2-B2, if larger
        // B2 is the upper right bounding box corner of a colliding obstacle
        if ((pRight.x < bezierCurve.p2.x - margin)
            && (bezierCurve.IsRightControlAbove() == spannedElement->m_isBelow)) {
            const double slope = BoundingBox::CalcSlope(bezierCurve.p2, pRight);
            if ((slope < 0.0) && bezierCurve.IsRightControlAbove()) {
                const double adjustedSlope = this->RotateSlope(-slope, 10.0, 2.5, true);
                rightSlopeMax = std::max(rightSlopeMax, adjustedSlope);
            }
            if ((slope > 0.0) && !bezierCurve.IsRightControlAbove()) {
                const double adjustedSlope = this->RotateSlope(slope, 10.0, 2.5, true);
                rightSlopeMax = std::max(rightSlopeMax, adjustedSlope);
            }
        }
    }

    if ((leftSlopeMax == 0.0) || (rightSlopeMax == 0.0)) return { false, 0, 0 };

    // Calculate offset from extremal slope, but use 1/20 of horizontal distance as minimum
    const int minOffset = (bezierCurve.p2.x - bezierCurve.p1.x) / 20;
    int leftOffset = minOffset;
    if (bezierCurve.GetLeftControlOffset() > 0) {
        leftOffset = std::max<int>(leftOffset, std::abs(bezierCurve.GetLeftControlHeight()) / leftSlopeMax);
    }
    int rightOffset = minOffset;
    if (bezierCurve.GetRightControlOffset() > 0) {
        rightOffset = std::max<int>(rightOffset, std::abs(bezierCurve.GetRightControlHeight()) / rightSlopeMax);
    }

    return { true, leftOffset, rightOffset };
}

ControlPointAdjustment AdjustSlursFunctor::CalcControlPointVerticalShift(
    const BezierCurve &bezierCurve, double symmetry, int margin) const
{
    ControlPointAdjustment adjustment{ 0, 0, false, 0 };
    if (bezierCurve.p1.x >= bezierCurve.p2.x) return adjustment;

    std::list<ControlPointConstraint> aboveConstraints;
    std::list<ControlPointConstraint> belowConstraints;
    int maxIntersectionAbove = 0;
    int maxIntersectionBelow = 0;

    const int dist = bezierCurve.p2.x - bezierCurve.p1.x;

    const ArrayOfCurveSpannedElements *spannedElements = m_currentCurve->GetSpannedElements();

    for (CurveSpannedElement *spannedElement : *spannedElements) {

        if (spannedElement->m_discarded) {
            continue;
        }

        bool discard = false;
        int intersectionLeft, intersectionRight;
        std::tie(intersectionLeft, intersectionRight) = m_currentCurve->CalcDirectionalLeftRightAdjustment(
            spannedElement->m_boundingBox, spannedElement->m_isBelow, discard, margin);

        if (discard) {
            spannedElement->m_discarded = true;
            continue;
        }

        std::list<ControlPointConstraint> &constraints
            = spannedElement->m_isBelow ? belowConstraints : aboveConstraints;
        int &maxIntersection = spannedElement->m_isBelow ? maxIntersectionBelow : maxIntersectionAbove;

        if ((intersectionLeft > 0) || (intersectionRight > 0)) {
            Point points[4];
            points[0] = bezierCurve.p1;
            points[1] = bezierCurve.c1;
            points[2] = bezierCurve.c2;
            points[3] = bezierCurve.p2;

            // Add constraint for the left boundary of the colliding bounding box
            const int xLeft = std::max(bezierCurve.p1.x, spannedElement->m_boundingBox->GetSelfLeft());
            float distanceRatio = float(xLeft - bezierCurve.p1.x) / float(dist);
            // Ignore obstacles close to the endpoints, because this would result in very large shifts
            if ((std::abs(0.5 - distanceRatio) < 0.45) && (intersectionLeft > 0)) {
                const double t = BoundingBox::CalcBezierParamAtPosition(points, xLeft);
                constraints.push_back(
                    { 3.0 * pow(1.0 - t, 2.0) * t, 3.0 * (1.0 - t) * pow(t, 2.0), double(intersectionLeft) });
                maxIntersection = std::max(maxIntersection, intersectionLeft);
            }

            // Add constraint for the right boundary of the colliding bounding box
            const int xRight = std::min(bezierCurve.p2.x, spannedElement->m_boundingBox->GetSelfRight());
            distanceRatio = float(xRight - bezierCurve.p1.x) / float(dist);
            // Ignore obstacles close to the endpoints, because this would result in very large shifts
            if ((std::abs(0.5 - distanceRatio) < 0.45) && (intersectionRight > 0)) {
                const double t = BoundingBox::CalcBezierParamAtPosition(points, xRight);
                constraints.push_back(
                    { 3.0 * pow(1.0 - t, 2.0) * t, 3.0 * (1.0 - t) * pow(t, 2.0), double(intersectionRight) });
                maxIntersection = std::max(maxIntersection, intersectionRight);
            }
        }
    }

    // Solve the constraints and calculate the adjustment
    if (maxIntersectionAbove > maxIntersectionBelow) {
        std::tie(adjustment.leftShift, adjustment.rightShift)
            = this->SolveControlPointConstraints(aboveConstraints, symmetry);
        adjustment.moveUpwards = false;
    }
    else {
        std::tie(adjustment.leftShift, adjustment.rightShift)
            = this->SolveControlPointConstraints(belowConstraints, symmetry);
        adjustment.moveUpwards = true;
    }

    // Determine the requested staff space
    if (bezierCurve.IsLeftControlAbove() && !bezierCurve.IsRightControlAbove()) {
        adjustment.requestedStaffSpace = std::max(bezierCurve.p1.y - bezierCurve.p2.y + 6 * margin, 0);
    }
    else if (!bezierCurve.IsLeftControlAbove() && bezierCurve.IsRightControlAbove()) {
        adjustment.requestedStaffSpace = std::max(bezierCurve.p2.y - bezierCurve.p1.y + 6 * margin, 0);
    }
    if ((maxIntersectionAbove > 0) && (maxIntersectionBelow > 0)) {
        adjustment.requestedStaffSpace
            = std::max(adjustment.requestedStaffSpace, maxIntersectionAbove + maxIntersectionBelow);
    }

    return adjustment;
}

std::pair<int, int> AdjustSlursFunctor::SolveControlPointConstraints(
    const std::list<ControlPointConstraint> &constraints, double symmetry) const
{
    if (constraints.empty()) return { 0, 0 };

    // Each constraint corresponds to a halfplane in the upper right quadrant.
    // We consider the line through the origin orthogonal to the halfplane's boundary
    // and average the slopes of these orthogonal lines.
    double weightSum = 0.0;
    double weightedAngleSum = 0.0;
    for (const ControlPointConstraint &constraint : constraints) {
        // Use the distance of the halfplane's boundary to the origin as weight
        const double weight = constraint.c / std::hypot(constraint.a, constraint.b);
        weightedAngleSum += weight * atan(constraint.b / constraint.a);
        weightSum += weight;
    }
    // Depending on symmetry we want the angle to be near PI/4
    double angle = weightedAngleSum / weightSum;
    angle = std::max(symmetry * M_PI / 4.0, angle);
    angle = std::min((2.0 - symmetry) * M_PI / 4.0, angle);
    const double slope = tan(angle);

    // Now follow the line with the averaged slope until we have hit all halfplanes.
    // For each constraint we must solve: slope * x = c/b - a/b * x
    double xMax = 0.0;
    for (const ControlPointConstraint &constraint : constraints) {
        const double x = constraint.c / (constraint.a + slope * constraint.b);
        xMax = std::max(xMax, x);
    }

    // The point which hits the last halfplane is the desired solution.
    return { xMax, slope * xMax };
}

void AdjustSlursFunctor::AdjustSlurShape(BezierCurve &bezierCurve, curvature_CURVEDIR dir, int unit) const
{
    if (bezierCurve.p1.x >= bezierCurve.p2.x) return;

    // Normalize the slur via rotation (such that p1-p2 is horizontal)
    const float angle = atan2(bezierCurve.p2.y - bezierCurve.p1.y, bezierCurve.p2.x - bezierCurve.p1.x);
    bezierCurve.Rotate(-angle, bezierCurve.p1);
    bezierCurve.UpdateControlPointParams();

    // *** STEP 1: Ensure MINIMAL HEIGHT ***
    // <)C1P1P2 should be at least 30 degrees, but allow smaller angles if the midpoint would be lifted more than 6 MEI
    // units
    // Similar for <)P1P2C2
    const int sign = (dir == curvature_CURVEDIR_above) ? 1 : -1;
    const Point shiftedMidpoint(
        (bezierCurve.p1.x + bezierCurve.p2.x) / 2, (bezierCurve.p1.y + bezierCurve.p2.y) / 2 + sign * 6 * unit);
    const float minAngle = this->GetMinControlPointAngle(bezierCurve, angle / M_PI * 180.0, unit);
    const bool ignoreLeft = (bezierCurve.c1.x <= bezierCurve.p1.x);
    const bool ignoreRight = (bezierCurve.c2.x >= bezierCurve.p2.x);
    double slopeLeft = BoundingBox::CalcSlope(bezierCurve.p1, bezierCurve.c1);
    double slopeRight = BoundingBox::CalcSlope(bezierCurve.p2, bezierCurve.c2);
    const double slopeBase = BoundingBox::CalcSlope(bezierCurve.p1, bezierCurve.p2);

    if (dir == curvature_CURVEDIR_above) {
        double minSlopeLeft = this->RotateSlope(slopeBase, minAngle, 1.0, true);
        minSlopeLeft = std::min(minSlopeLeft, BoundingBox::CalcSlope(bezierCurve.p1, shiftedMidpoint));
        slopeLeft = std::max(slopeLeft, minSlopeLeft);
        double minSlopeRight = this->RotateSlope(slopeBase, minAngle, 1.0, false);
        minSlopeRight = std::max(minSlopeRight, BoundingBox::CalcSlope(bezierCurve.p2, shiftedMidpoint));
        slopeRight = std::min(slopeRight, minSlopeRight);
    }
    else if (dir == curvature_CURVEDIR_below) {
        double minSlopeLeft = this->RotateSlope(slopeBase, minAngle, 1.0, false);
        minSlopeLeft = std::max(minSlopeLeft, BoundingBox::CalcSlope(bezierCurve.p1, shiftedMidpoint));
        slopeLeft = std::min(slopeLeft, minSlopeLeft);
        double minSlopeRight = this->RotateSlope(slopeBase, minAngle, 1.0, true);
        minSlopeRight = std::min(minSlopeRight, BoundingBox::CalcSlope(bezierCurve.p2, shiftedMidpoint));
        slopeRight = std::max(slopeRight, minSlopeRight);
    }

    // Update control points
    if (!ignoreLeft) bezierCurve.SetLeftControlHeight(slopeLeft * sign * bezierCurve.GetLeftControlOffset());
    if (!ignoreRight) bezierCurve.SetRightControlHeight(slopeRight * -sign * bezierCurve.GetRightControlOffset());
    bezierCurve.UpdateControlPoints();

    // *** STEP 2: Ensure CONVEXITY ***
    // <)C1P1C2 and <)C1P2C2 should be at least 3 degrees
    if (dir == curvature_CURVEDIR_above) {
        const double minSlopeLeft
            = this->RotateSlope(BoundingBox::CalcSlope(bezierCurve.p1, bezierCurve.c2), 3.0, 10.0, true);
        slopeLeft = std::max(slopeLeft, minSlopeLeft);
        const double minSlopeRight
            = this->RotateSlope(BoundingBox::CalcSlope(bezierCurve.p2, bezierCurve.c1), 3.0, 10.0, false);
        slopeRight = std::min(slopeRight, minSlopeRight);
    }
    else if (dir == curvature_CURVEDIR_below) {
        const double minSlopeLeft
            = this->RotateSlope(BoundingBox::CalcSlope(bezierCurve.p1, bezierCurve.c2), 3.0, 10.0, false);
        slopeLeft = std::min(slopeLeft, minSlopeLeft);
        const double minSlopeRight
            = this->RotateSlope(BoundingBox::CalcSlope(bezierCurve.p2, bezierCurve.c1), 3.0, 10.0, true);
        slopeRight = std::max(slopeRight, minSlopeRight);
    }

    // Update control points
    if (!ignoreLeft) bezierCurve.SetLeftControlHeight(slopeLeft * sign * bezierCurve.GetLeftControlOffset());
    if (!ignoreRight) bezierCurve.SetRightControlHeight(slopeRight * -sign * bezierCurve.GetRightControlOffset());
    bezierCurve.UpdateControlPoints();

    // Rotate back
    bezierCurve.Rotate(angle, bezierCurve.p1);

    // Enforce p1.x <= c1.x <= c2.x <= p2.x
    bezierCurve.c1.x = std::max(bezierCurve.p1.x, bezierCurve.c1.x);
    bezierCurve.c2.x = std::max(bezierCurve.c1.x, bezierCurve.c2.x);
    bezierCurve.c2.x = std::min(bezierCurve.p2.x, bezierCurve.c2.x);
    bezierCurve.c1.x = std::min(bezierCurve.c2.x, bezierCurve.c1.x);

    bezierCurve.UpdateControlPointParams();
}

ControlPointAdjustment AdjustSlursFunctor::CalcControlPointShift(const BezierCurve &bezierCurve,
    const ArrayOfFloatingCurvePositioners &innerCurves, double symmetry, int margin) const
{
    ControlPointAdjustment adjustment{ 0, 0, false, 0 };
    if (bezierCurve.p1.x >= bezierCurve.p2.x) return adjustment;

    const int dist = bezierCurve.p2.x - bezierCurve.p1.x;
    const bool isBelow = (m_currentCurve->GetDir() == curvature_CURVEDIR_above);
    const int sign = isBelow ? 1 : -1;
    Point points[4];
    points[0] = bezierCurve.p1;
    points[1] = bezierCurve.c1;
    points[2] = bezierCurve.c2;
    points[3] = bezierCurve.p2;

    std::list<ControlPointConstraint> constraints;
    for (FloatingCurvePositioner *innerCurve : innerCurves) {
        Point innerPoints[4];
        innerCurve->GetPoints(innerPoints);

        // Create five constraints for each inner slur
        for (int step = 0; step <= 4; ++step) {
            const Point innerPoint = BoundingBox::CalcPointAtBezier(innerPoints, 0.25 * step);
            if ((bezierCurve.p1.x <= innerPoint.x) && (innerPoint.x <= bezierCurve.p2.x)) {
                const int y = BoundingBox::CalcBezierAtPosition(points, innerPoint.x);
                const int intersection = (innerPoint.y - y) * sign + margin;
                const float distanceRatio = float(innerPoint.x - bezierCurve.p1.x) / float(dist);

                // Ignore obstacles close to the endpoints, because this would result in very large shifts
                if ((std::abs(0.5 - distanceRatio) < 0.45) && (intersection > 0)) {
                    const double t = BoundingBox::CalcBezierParamAtPosition(points, innerPoint.x);
                    constraints.push_back(
                        { 3.0 * pow(1.0 - t, 2.0) * t, 3.0 * (1.0 - t) * pow(t, 2.0), double(intersection) });
                }
            }
        }
    }

    // Solve the constraints and calculate the adjustment
    std::tie(adjustment.leftShift, adjustment.rightShift) = this->SolveControlPointConstraints(constraints, symmetry);

    return adjustment;
}

std::pair<int, int> AdjustSlursFunctor::CalcEndPointShift(const BezierCurve &bezierCurve,
    const ArrayOfFloatingCurvePositioners &innerCurves, double flexibility, int margin) const
{
    if (bezierCurve.p1.x >= bezierCurve.p2.x) return { 0, 0 };

    int shiftLeft = 0;
    int shiftRight = 0;

    const int dist = bezierCurve.p2.x - bezierCurve.p1.x;
    const bool isBelow = (m_currentCurve->GetDir() == curvature_CURVEDIR_above);
    const int sign = isBelow ? 1 : -1;
    Point points[4];
    points[0] = bezierCurve.p1;
    points[1] = bezierCurve.c1;
    points[2] = bezierCurve.c2;
    points[3] = bezierCurve.p2;

    for (FloatingCurvePositioner *innerCurve : innerCurves) {
        Point innerPoints[4];
        innerCurve->GetPoints(innerPoints);

        // Adjustment for start point of inner slur
        const int xInnerStart = innerPoints[0].x;
        if ((bezierCurve.p1.x <= xInnerStart) && (xInnerStart <= bezierCurve.p2.x)) {
            const int yStart = BoundingBox::CalcBezierAtPosition(points, xInnerStart);
            const int intersectionStart = (innerPoints[0].y - yStart) * sign + 1.5 * margin;
            if (intersectionStart > 0) {
                const float distanceRatioStart = float(xInnerStart - bezierCurve.p1.x) / float(dist);
                this->ShiftEndPoints(shiftLeft, shiftRight, distanceRatioStart, intersectionStart, flexibility, isBelow,
                    m_currentCurve->m_spanningType);
            }
        }

        // Adjustment for midpoint of inner slur
        const Point innerMidPoint = BoundingBox::CalcPointAtBezier(innerPoints, 0.5);
        if ((bezierCurve.p1.x <= innerMidPoint.x) && (innerMidPoint.x <= bezierCurve.p2.x)) {
            const int yMid = BoundingBox::CalcBezierAtPosition(points, innerMidPoint.x);
            const int intersectionMid = (innerMidPoint.y - yMid) * sign + 1.5 * margin;
            if (intersectionMid > 0) {
                const float distanceRatioMid = float(innerMidPoint.x - bezierCurve.p1.x) / float(dist);
                this->ShiftEndPoints(shiftLeft, shiftRight, distanceRatioMid, intersectionMid, flexibility, isBelow,
                    m_currentCurve->m_spanningType);
            }
        }

        // Adjustment for end point of inner slur
        const int xInnerEnd = innerPoints[3].x;
        if ((bezierCurve.p1.x <= xInnerEnd) && (xInnerEnd <= bezierCurve.p2.x)) {
            const int yEnd = BoundingBox::CalcBezierAtPosition(points, xInnerEnd);
            const int intersectionEnd = (innerPoints[3].y - yEnd) * sign + 1.5 * margin;
            if (intersectionEnd > 0) {
                const float distanceRatioEnd = float(xInnerEnd - bezierCurve.p1.x) / float(dist);
                this->ShiftEndPoints(shiftLeft, shiftRight, distanceRatioEnd, intersectionEnd, flexibility, isBelow,
                    m_currentCurve->m_spanningType);
            }
        }
    }

    return { shiftLeft, shiftRight };
}

void AdjustSlursFunctor::ShiftEndPoints(int &shiftLeft, int &shiftRight, double ratio, int intersection,
    double flexibility, bool isBelow, char spanningType) const
{
    // Filter collisions near the endpoints
    // Collisions with ratio beyond the partialShiftRadius do not contribute to shifts
    // They are compensated later by shifting the control points
    double fullShiftRadius = 0.0;
    double partialShiftRadius = 0.0;
    std::tie(fullShiftRadius, partialShiftRadius) = this->CalcShiftRadii(true, flexibility, spanningType);

    if ((ratio < partialShiftRadius) && (m_currentSlur->HasEndpointAboveStart() == isBelow)) {
        if (ratio > fullShiftRadius) {
            // Collisions here only partially contribute to shifts
            // We multiply with a function that interpolates between 1 and 0
            intersection *= this->CalcQuadraticInterpolation(partialShiftRadius, fullShiftRadius, ratio);
        }
        shiftLeft = std::max(shiftLeft, intersection);
    }

    std::tie(fullShiftRadius, partialShiftRadius) = this->CalcShiftRadii(false, flexibility, spanningType);

    if ((ratio > 1.0 - partialShiftRadius) && (m_currentSlur->HasEndpointAboveEnd() == isBelow)) {
        if (ratio < 1.0 - fullShiftRadius) {
            // Collisions here only partially contribute to shifts
            // We multiply with a function that interpolates between 0 and 1
            intersection *= this->CalcQuadraticInterpolation(1.0 - partialShiftRadius, 1.0 - fullShiftRadius, ratio);
        }
        shiftRight = std::max(shiftRight, intersection);
    }
}

std::pair<double, double> AdjustSlursFunctor::CalcShiftRadii(
    bool forShiftLeft, double flexibility, char spanningType) const
{
    // Use full flexibility for broken slur endpoints
    if (forShiftLeft) {
        if ((spanningType == SPANNING_MIDDLE) || (spanningType == SPANNING_END)) {
            flexibility = 1.0;
        }
    }
    else {
        if ((spanningType == SPANNING_START) || (spanningType == SPANNING_MIDDLE)) {
            flexibility = 1.0;
        }
    }

    const double fullShiftRadius = 0.05 + flexibility * 0.15;
    const double partialShiftRadius = fullShiftRadius * 3.0;

    return { fullShiftRadius, partialShiftRadius };
}

double AdjustSlursFunctor::CalcQuadraticInterpolation(double zeroAt, double oneAt, double arg) const
{
    assert(zeroAt != oneAt);
    const double a = 1.0 / (oneAt - zeroAt);
    const double b = zeroAt / (zeroAt - oneAt);
    return pow(a * arg + b, 2.0);
}

double AdjustSlursFunctor::RotateSlope(double slope, double degrees, double doublingBound, bool upwards) const
{
    assert(degrees >= 0.0);
    assert(doublingBound >= 0.0);

    if (upwards && (slope >= doublingBound)) return slope * 2.0;
    if (!upwards && (slope <= -doublingBound)) return slope * 2.0;
    const int sign = upwards ? 1 : -1;
    return tan(atan(slope) + sign * M_PI * degrees / 180.0);
}

float AdjustSlursFunctor::GetMinControlPointAngle(const BezierCurve &bezierCurve, float angle, int unit) const
{
    angle = abs(angle);
    const double distance = double(bezierCurve.p2.x - bezierCurve.p1.x) / unit;

    // Increase min angle for short and angled slurs
    double angleIncrement = std::min(angle / 4.0, 15.0); // values in [0.0, 15.0]
    double factor = 1.0 - (distance - 8.0) / 8.0;
    factor = std::min(factor, 1.0);
    factor = std::max(factor, 0.0); // values in [0.0, 1.0]

    // not if control points are horizontally in a degenerated position
    if ((bezierCurve.c1.x < bezierCurve.p1.x) || (2.0 * bezierCurve.c1.x > bezierCurve.p1.x + bezierCurve.p2.x)) {
        angleIncrement = 0.0;
    }
    if ((bezierCurve.c2.x > bezierCurve.p2.x) || (2.0 * bezierCurve.c2.x < bezierCurve.p1.x + bezierCurve.p2.x)) {
        angleIncrement = 0.0;
    }

    return 30.0 + angleIncrement * factor;
}

} // namespace vrv
