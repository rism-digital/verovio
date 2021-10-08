/////////////////////////////////////////////////////////////////////////////
// Name:        slur.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "slur.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <cmath>
#include <math.h>

//----------------------------------------------------------------------------

#include "beam.h"
#include "chord.h"
#include "doc.h"
#include "ftrem.h"
#include "functorparams.h"
#include "layer.h"
#include "layerelement.h"
#include "note.h"
#include "staff.h"
#include "verticalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Slur
//----------------------------------------------------------------------------

static const ClassRegistrar<Slur> s_factory("slur", SLUR);

Slur::Slur() : ControlElement(SLUR, "slur-"), TimeSpanningInterface(), AttColor(), AttCurvature(), AttCurveRend()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_CURVATURE);
    RegisterAttClass(ATT_CURVEREND);

    Reset();
}

Slur::Slur(ClassId classId)
    : ControlElement(classId, "slur-"), TimeSpanningInterface(), AttColor(), AttCurvature(), AttCurveRend()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_CURVATURE);
    RegisterAttClass(ATT_CURVEREND);

    Reset();
}

Slur::Slur(ClassId classId, const std::string &classIdStr)
    : ControlElement(classId, classIdStr), TimeSpanningInterface(), AttColor(), AttCurvature(), AttCurveRend()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_CURVATURE);
    RegisterAttClass(ATT_CURVEREND);

    Reset();
}

Slur::~Slur() {}

void Slur::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    ResetColor();
    ResetCurvature();
    ResetCurveRend();

    m_drawingCurvedir = curvature_CURVEDIR_NONE;
    // m_isCrossStaff = false;
}

void Slur::AdjustSlur(Doc *doc, FloatingCurvePositioner *curve, Staff *staff)
{
    assert(doc);
    assert(curve);
    assert(staff);

    Point points[4];
    curve->GetPoints(points);
    BezierCurve bezier(points[0], points[1], points[2], points[3]);
    bezier.UpdateControlPointParams(curve->GetDir());

    const int margin = doc->GetOptions()->m_slurMargin.GetValue() * doc->GetDrawingUnit(100);

    // STEP 1: Filter spanned elements and discard certain bounding boxes even though they collide
    this->FilterSpannedElements(curve, bezier, margin);

    // STEP 2: Calculate the vertical adjustment of end points. This shifts the slur vertically.
    // Only collisions near the endpoints are taken into account.
    int endPointShiftLeft = 0;
    int endPointShiftRight = 0;
    std::tie(endPointShiftLeft, endPointShiftRight) = this->CalcEndPointShift(curve, bezier, margin);
    if ((endPointShiftLeft != 0) || (endPointShiftRight != 0)) {
        const int sign = (curve->GetDir() == curvature_CURVEDIR_above) ? 1 : -1;
        bezier.p1.y += sign * endPointShiftLeft;
        bezier.p2.y += sign * endPointShiftRight;
        if (bezier.p1.x != bezier.p2.x) {
            double lambda = double(bezier.c1.x - bezier.p1.x) / double(bezier.p2.x - bezier.p1.x);
            bezier.c1.y += sign * ((1.0 - lambda) * endPointShiftLeft + lambda * endPointShiftRight);
            lambda = double(bezier.c2.x - bezier.p1.x) / double(bezier.p2.x - bezier.p1.x);
            bezier.c2.y += sign * ((1.0 - lambda) * endPointShiftLeft + lambda * endPointShiftRight);
        }
        bezier.UpdateControlPointParams(curve->GetDir());
        curve->UpdatePoints(bezier);
    }

    // STEP 3: Calculate the horizontal offset of the control points.
    // The idea is to shift control points to the outside if there is an obstacle in the vicinity of the corresponding
    // endpoint. For C1 we consider the largest angle <)BP1P2 where B is a colliding left bounding box corner and choose
    // C1 in this direction. Similar for C2.
    bool ok = false;
    int controlPointOffsetLeft = 0;
    int controlPointOffsetRight = 0;
    std::tie(ok, controlPointOffsetLeft, controlPointOffsetRight) = this->CalcControlPointOffset(curve, bezier, margin);
    if (ok) {
        bezier.SetLeftControlPointOffset(controlPointOffsetLeft);
        bezier.SetRightControlPointOffset(controlPointOffsetRight);
        bezier.UpdateControlPoints(curve->GetDir());
        curve->UpdatePoints(bezier);
    }

    // STEP 4: Calculate the vertical shift of the control points.
    // For each colliding bounding box we formulate a constraint ax + by >= c
    // where x, y denote the vertical adjustments of the control points and c is the size of the collision.
    // The coefficients a, b are calculated from the Bezier curve equation.
    // After collecting all constraints we calculate a solution.
    int controlPointShiftLeft = 0;
    int controlPointShiftRight = 0;
    std::tie(controlPointShiftLeft, controlPointShiftRight)
        = this->CalcControlPointVerticalShift(curve, bezier, margin);
    bezier.SetLeftControlHeight(bezier.GetLeftControlHeight() + controlPointShiftLeft);
    bezier.SetRightControlHeight(bezier.GetRightControlHeight() + controlPointShiftRight);
    bezier.UpdateControlPoints(curve->GetDir());
    curve->UpdatePoints(bezier);

    // STEP 5: Adjust the slur shape
    // Through the control point adjustments in step 3 and 4 it can happen that the slur looses its desired shape.
    // We correct the shape if the slur is too flat or not convex.
    this->AdjustSlurShape(bezier, curve->GetDir(), doc->GetDrawingUnit(100));
    curve->UpdatePoints(bezier);

    // Since we are going to redraw it, reset its bounding box
    curve->BoundingBox::ResetBoundingBox();
}

void Slur::FilterSpannedElements(FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, int margin)
{
    if (bezierCurve.p1.x >= bezierCurve.p2.x) return;

    const int dist = bezierCurve.p2.x - bezierCurve.p1.x;

    const ArrayOfCurveSpannedElements *spannedElements = curve->GetSpannedElements();

    for (auto spannedElement : *spannedElements) {

        if (spannedElement->m_discarded) {
            continue;
        }

        bool discard = false;
        const int intersection = curve->CalcAdjustment(spannedElement->m_boundingBox, discard, margin);
        const int xMiddle
            = (spannedElement->m_boundingBox->GetSelfLeft() + spannedElement->m_boundingBox->GetSelfRight()) / 2.0;
        const float distanceRatio = float(xMiddle - bezierCurve.p1.x) / float(dist);

        // In case of cross-staff, ignore obstacles which completely lie on the other side of the slur near the
        // endpoints
        const int elementHeight
            = std::abs(spannedElement->m_boundingBox->GetSelfTop() - spannedElement->m_boundingBox->GetSelfBottom());
        if (curve->IsCrossStaff() && (intersection > elementHeight + 4 * margin)
            && (std::abs(distanceRatio - 0.5) > 0.45)) {
            spannedElement->m_discarded = true;
        }
    }
}

std::pair<int, int> Slur::CalcEndPointShift(FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, int margin)
{
    if (bezierCurve.p1.x >= bezierCurve.p2.x) return { 0, 0 };

    int shiftLeft = 0;
    int shiftRight = 0;

    const int dist = bezierCurve.p2.x - bezierCurve.p1.x;

    const ArrayOfCurveSpannedElements *spannedElements = curve->GetSpannedElements();
    for (auto spannedElement : *spannedElements) {

        if (spannedElement->m_discarded) {
            continue;
        }

        bool discard = false;
        int intersectionLeft, intersectionRight;
        std::tie(intersectionLeft, intersectionRight)
            = curve->CalcLeftRightAdjustment(spannedElement->m_boundingBox, discard, margin);

        if (discard) {
            spannedElement->m_discarded = true;
            continue;
        }

        if ((intersectionLeft > 0) || (intersectionRight > 0)) {
            // Now apply the intersections on the left and right hand side of the bounding box
            const int xLeft = std::max(bezierCurve.p1.x, spannedElement->m_boundingBox->GetSelfLeft());
            const float distanceRatioLeft = float(xLeft - bezierCurve.p1.x) / float(dist);
            this->ShiftEndPoints(shiftLeft, shiftRight, distanceRatioLeft, intersectionLeft);

            const int xRight = std::min(bezierCurve.p2.x, spannedElement->m_boundingBox->GetSelfRight());
            const float distanceRatioRight = float(xRight - bezierCurve.p1.x) / float(dist);
            this->ShiftEndPoints(shiftLeft, shiftRight, distanceRatioRight, intersectionRight);
        }
    }
    return { shiftLeft, shiftRight };
}

void Slur::ShiftEndPoints(int &shiftLeft, int &shiftRight, double ratio, int intersection) const
{
    // Filter collisions near the endpoints
    // Collisions with 0.15 <= ratio <= 0.85 do not contribute to shifts
    // They are compensated later by shifting the control points
    if (ratio < 0.15) {
        if (ratio > 0.05) {
            // For 0.05 <= ratio <= 0.15 collisions only partially contribute to shifts
            // We multiply with a function that interpolates between 1 and 0
            intersection *= pow(1.5 - 10.0 * ratio, 2.0);
        }
        shiftLeft = std::max(shiftLeft, intersection);
    }
    else if (ratio > 0.85) {
        if (ratio < 0.95) {
            // For 0.85 <= ratio <= 0.95 collisions only partially contribute to shifts
            // We multiply with a function that interpolates between 0 and 1
            intersection *= pow(10.0 * ratio - 8.5, 2.0);
        }
        shiftRight = std::max(shiftRight, intersection);
    }
}

std::tuple<bool, int, int> Slur::CalcControlPointOffset(
    FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, int margin)
{
    if (bezierCurve.p1.x >= bezierCurve.p2.x) return { false, 0, 0 };

    // Initially we start with the slopes of the lines P1-C1 and P2-C2
    double leftSlopeMax = std::abs(BoundingBox::CalcSlope(bezierCurve.p1, bezierCurve.c1));
    double rightSlopeMax = std::abs(BoundingBox::CalcSlope(bezierCurve.p2, bezierCurve.c2));
    const ArrayOfCurveSpannedElements *spannedElements = curve->GetSpannedElements();
    for (auto spannedElement : *spannedElements) {

        if (spannedElement->m_discarded) {
            continue;
        }

        const int bbY = (curve->GetDir() == curvature_CURVEDIR_above) ? spannedElement->m_boundingBox->GetSelfTop()
                                                                      : spannedElement->m_boundingBox->GetSelfBottom();
        const Point pLeft = { spannedElement->m_boundingBox->GetSelfLeft(), bbY };
        const Point pRight = { spannedElement->m_boundingBox->GetSelfRight(), bbY };

        // Prefer the (increased) slope of P1-B1, if larger
        // B1 is the upper left bounding box corner of a colliding obstacle
        if (pLeft.x > bezierCurve.p1.x + margin) {
            const double slope = BoundingBox::CalcSlope(bezierCurve.p1, pLeft);
            if ((slope > 0.0) && (curve->GetDir() == curvature_CURVEDIR_above)) {
                const double adjustedSlope = this->RotateSlope(slope, 10.0, 2.5, true);
                leftSlopeMax = std::max(leftSlopeMax, adjustedSlope);
            }
            if ((slope < 0.0) && (curve->GetDir() == curvature_CURVEDIR_below)) {
                const double adjustedSlope = this->RotateSlope(-slope, 10.0, 2.5, true);
                leftSlopeMax = std::max(leftSlopeMax, adjustedSlope);
            }
        }

        // Prefer the (increased) slope of P2-B2, if larger
        // B2 is the upper right bounding box corner of a colliding obstacle
        if (pRight.x < bezierCurve.p2.x - margin) {
            const double slope = BoundingBox::CalcSlope(bezierCurve.p2, pRight);
            if ((slope < 0.0) && (curve->GetDir() == curvature_CURVEDIR_above)) {
                const double adjustedSlope = this->RotateSlope(-slope, 10.0, 2.5, true);
                rightSlopeMax = std::max(rightSlopeMax, adjustedSlope);
            }
            if ((slope > 0.0) && (curve->GetDir() == curvature_CURVEDIR_below)) {
                const double adjustedSlope = this->RotateSlope(slope, 10.0, 2.5, true);
                rightSlopeMax = std::max(rightSlopeMax, adjustedSlope);
            }
        }
    }

    if ((leftSlopeMax == 0.0) || (rightSlopeMax == 0.0)) return { false, 0, 0 };

    // Calculate offset from extremal slope, but use 1/20 of horizontal distance as minimum
    const int minOffset = (bezierCurve.p2.x - bezierCurve.p1.x) / 20;
    int leftOffset = minOffset;
    if (bezierCurve.GetLeftControlPointOffset() > 0) {
        leftOffset = std::max<int>(leftOffset, std::abs(bezierCurve.GetLeftControlHeight()) / leftSlopeMax);
    }
    int rightOffset = minOffset;
    if (bezierCurve.GetRightControlPointOffset() > 0) {
        rightOffset = std::max<int>(rightOffset, std::abs(bezierCurve.GetRightControlHeight()) / rightSlopeMax);
    }

    return { true, leftOffset, rightOffset };
}

std::pair<int, int> Slur::CalcControlPointVerticalShift(
    FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, int margin)
{
    if (bezierCurve.p1.x >= bezierCurve.p2.x) return { 0, 0 };

    std::list<ControlPointConstraint> constraints;

    const int dist = bezierCurve.p2.x - bezierCurve.p1.x;

    const ArrayOfCurveSpannedElements *spannedElements = curve->GetSpannedElements();

    for (auto spannedElement : *spannedElements) {

        if (spannedElement->m_discarded) {
            continue;
        }

        bool discard = false;
        int intersectionLeft, intersectionRight;
        std::tie(intersectionLeft, intersectionRight)
            = curve->CalcLeftRightAdjustment(spannedElement->m_boundingBox, discard, margin);

        if (discard) {
            spannedElement->m_discarded = true;
            continue;
        }

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
            if (std::abs(0.5 - distanceRatio) < 0.45) {
                const double t = BoundingBox::CalcBezierParamAtPosition(points, xLeft);
                constraints.push_back(
                    { 3.0 * pow(1.0 - t, 2.0) * t, 3.0 * (1.0 - t) * pow(t, 2.0), double(intersectionLeft) });
            }

            // Add constraint for the right boundary of the colliding bounding box
            const int xRight = std::min(bezierCurve.p2.x, spannedElement->m_boundingBox->GetSelfRight());
            distanceRatio = float(xRight - bezierCurve.p1.x) / float(dist);
            // Ignore obstacles close to the endpoints, because this would result in very large shifts
            if (std::abs(0.5 - distanceRatio) < 0.45) {
                const double t = BoundingBox::CalcBezierParamAtPosition(points, xRight);
                constraints.push_back(
                    { 3.0 * pow(1.0 - t, 2.0) * t, 3.0 * (1.0 - t) * pow(t, 2.0), double(intersectionRight) });
            }
        }
    }
    return this->SolveControlPointConstraints(constraints);
}

std::pair<int, int> Slur::SolveControlPointConstraints(const std::list<ControlPointConstraint> &constraints)
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
    const double slope = tan(weightedAngleSum / weightSum);

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

void Slur::AdjustSlurShape(BezierCurve &bezierCurve, curvature_CURVEDIR dir, int unit)
{
    if (bezierCurve.p1.x >= bezierCurve.p2.x) return;

    // *** STEP 1: Ensure MINIMAL HEIGHT ***
    // <)C1P1P2 should be at least 20 degree, but allow smaller angles if the midpoint would be lifted more than 6 MEI
    // units
    // Similar for <)P1P2C2
    const int sign = (dir == curvature_CURVEDIR_above) ? 1 : -1;
    const Point shiftedMidpoint(
        (bezierCurve.p1.x + bezierCurve.p2.x) / 2, (bezierCurve.p1.y + bezierCurve.p2.y) / 2 + sign * 6 * unit);

    const bool ignoreLeft = (bezierCurve.c1.x <= bezierCurve.p1.x);
    const bool ignoreRight = (bezierCurve.c2.x >= bezierCurve.p2.x);
    double slopeLeft = BoundingBox::CalcSlope(bezierCurve.p1, bezierCurve.c1);
    double slopeRight = BoundingBox::CalcSlope(bezierCurve.p2, bezierCurve.c2);
    const double slopeBase = BoundingBox::CalcSlope(bezierCurve.p1, bezierCurve.p2);

    if (dir == curvature_CURVEDIR_above) {
        double minSlopeLeft = this->RotateSlope(slopeBase, 20.0, 1.0, true);
        minSlopeLeft = std::min(minSlopeLeft, BoundingBox::CalcSlope(bezierCurve.p1, shiftedMidpoint));
        slopeLeft = std::max(slopeLeft, minSlopeLeft);
        double minSlopeRight = this->RotateSlope(slopeBase, 20.0, 1.0, false);
        minSlopeRight = std::max(minSlopeRight, BoundingBox::CalcSlope(bezierCurve.p2, shiftedMidpoint));
        slopeRight = std::min(slopeRight, minSlopeRight);
    }
    else if (dir == curvature_CURVEDIR_below) {
        double minSlopeLeft = this->RotateSlope(slopeBase, 20.0, 1.0, false);
        minSlopeLeft = std::max(minSlopeLeft, BoundingBox::CalcSlope(bezierCurve.p1, shiftedMidpoint));
        if (!ignoreLeft) slopeLeft = std::min(slopeLeft, minSlopeLeft);
        double minSlopeRight = this->RotateSlope(slopeBase, 20.0, 1.0, true);
        minSlopeRight = std::min(minSlopeRight, BoundingBox::CalcSlope(bezierCurve.p2, shiftedMidpoint));
        if (!ignoreRight) slopeRight = std::max(slopeRight, minSlopeRight);
    }

    // Update control points
    if (!ignoreLeft) bezierCurve.SetLeftControlHeight(slopeLeft * sign * bezierCurve.GetLeftControlPointOffset());
    if (!ignoreRight) bezierCurve.SetRightControlHeight(slopeRight * -sign * bezierCurve.GetRightControlPointOffset());
    bezierCurve.UpdateControlPoints(dir);

    // *** STEP 2: Ensure CONVEXITY ***
    // <)C1P1C2 and <)C1P2C2 should be at least 3 degree
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
    if (!ignoreLeft) bezierCurve.SetLeftControlHeight(slopeLeft * sign * bezierCurve.GetLeftControlPointOffset());
    if (!ignoreRight) bezierCurve.SetRightControlHeight(slopeRight * -sign * bezierCurve.GetRightControlPointOffset());
    bezierCurve.UpdateControlPoints(dir);
}

double Slur::RotateSlope(double slope, double degrees, double doublingBound, bool upwards) const
{
    assert(degrees >= 0.0);
    assert(doublingBound >= 0.0);

    if (upwards && (slope >= doublingBound)) return slope * 2.0;
    if (!upwards && (slope <= -doublingBound)) return slope * 2.0;
    const int sign = upwards ? 1 : -1;
    return tan(atan(slope) + sign * M_PI * degrees / 180.0);
}

float Slur::GetAdjustedSlurAngle(Doc *doc, Point &p1, Point &p2, curvature_CURVEDIR curveDir)
{
    float slurAngle = (p1 == p2) ? 0 : atan2(p2.y - p1.y, p2.x - p1.x);
    const float maxAngle = (float)doc->GetOptions()->m_slurMaxSlope.GetValue() * M_PI / 180.0;

    // the slope of the slur is high and needs to be corrected
    if (fabs(slurAngle) > maxAngle) {
        int side = (p2.x - p1.x) * tan(maxAngle);
        if (p2.y > p1.y) {
            if (curveDir == curvature_CURVEDIR_above)
                p1.y = p2.y - side;
            else
                p2.y = p1.y + side;
            slurAngle = maxAngle;
        }
        else {
            if (curveDir == curvature_CURVEDIR_above)
                p2.y = p1.y - side;
            else
                p1.y = p2.y + side;
            slurAngle = -maxAngle;
        }
    }

    return slurAngle;
}

curvature_CURVEDIR Slur::GetGraceCurveDirection(Doc *doc)
{
    LayerElement *start = this->GetStart();
    LayerElement *end = this->GetEnd();
    Staff *staff = vrv_cast<Staff *>(start->GetFirstAncestor(STAFF));
    assert(staff);
    const bool overlap = start->VerticalContentOverlap(end);
    const int topDiff = std::abs(start->GetContentTop() - end->GetContentTop());
    const int bottomDiff = std::abs(start->GetContentBottom() - end->GetContentBottom());
    const int unit = doc->GetDrawingUnit(staff->m_drawingStaffSize);
    if (overlap) {
        if (bottomDiff < 1.5 * topDiff) {
            return curvature_CURVEDIR_below;
        }
        else if ((bottomDiff < 3 * topDiff) && (NULL != end->IsInBeam())) {
            return curvature_CURVEDIR_below;
        }
        else {
            return curvature_CURVEDIR_above;
        }
    }
    else {
        StemmedDrawingInterface *startStemDrawInterface = dynamic_cast<StemmedDrawingInterface *>(start);
        StemmedDrawingInterface *endStemDrawInterface = dynamic_cast<StemmedDrawingInterface *>(end);

        data_STEMDIRECTION startStemDir = STEMDIRECTION_NONE;
        if (startStemDrawInterface) {
            startStemDir = startStemDrawInterface->GetDrawingStemDir();
        }
        data_STEMDIRECTION endStemDir = STEMDIRECTION_NONE;
        if (endStemDrawInterface) {
            endStemDir = endStemDrawInterface->GetDrawingStemDir();
        }

        if (end->GetContentBottom() > start->GetContentTop()) {
            if (endStemDir == STEMDIRECTION_up)
                return curvature_CURVEDIR_above;
            else {
                return (bottomDiff < topDiff) ? curvature_CURVEDIR_below : curvature_CURVEDIR_above;
            }
        }
        else if (end->GetContentTop() < start->GetContentBottom()) {
            if (endStemDir == STEMDIRECTION_down)
                return curvature_CURVEDIR_below;
            else {
                return (bottomDiff < topDiff) ? curvature_CURVEDIR_below : curvature_CURVEDIR_above;
            }
        }
    }
    return curvature_CURVEDIR_below;
}

curvature_CURVEDIR Slur::GetPreferredCurveDirection(
    Doc *doc, Layer *layer, LayerElement *layerElement, data_STEMDIRECTION noteStemDir, bool isAboveStaffCenter)
{
    assert(layerElement);
    const bool isGraceToNoteSlur = !this->GetStart()->Is(TIMESTAMP_ATTR) && !this->GetEnd()->Is(TIMESTAMP_ATTR)
        && this->GetStart()->IsGraceNote() && !this->GetEnd()->IsGraceNote();
    Note *startNote = NULL;
    Chord *startParentChord = NULL;
    if (this->GetStart()->Is(NOTE)) {
        startNote = vrv_cast<Note *>(this->GetStart());
        assert(startNote);
        startParentChord = startNote->IsChordTone();
    }

    data_STEMDIRECTION layerStemDir;
    curvature_CURVEDIR drawingCurveDir = curvature_CURVEDIR_above;
    // first should be the slur @curvedir
    if (this->HasCurvedir()) {
        drawingCurveDir
            = (this->GetCurvedir() == curvature_CURVEDIR_above) ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
    }
    // grace notes - always below unless we have a drawing stem direction on the layer
    else if (isGraceToNoteSlur && layer && (layer->GetDrawingStemDir(layerElement) == STEMDIRECTION_NONE)) {
        drawingCurveDir = this->GetGraceCurveDirection(doc);
    }
    // the normal case
    else if (this->HasDrawingCurvedir()) {
        drawingCurveDir = this->GetDrawingCurvedir();
    }
    // then layer direction trumps note direction
    else if (layer && ((layerStemDir = layer->GetDrawingStemDir(layerElement)) != STEMDIRECTION_NONE)) {
        drawingCurveDir = (layerStemDir == STEMDIRECTION_up) ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
    }
    // look if in a chord
    else if (startParentChord) {
        if (startParentChord->PositionInChord(startNote) < 0) {
            drawingCurveDir = curvature_CURVEDIR_below;
        }
        else if (startParentChord->PositionInChord(startNote) > 0) {
            drawingCurveDir = curvature_CURVEDIR_above;
        }
        // away from the stem if odd number (center note)
        else {
            drawingCurveDir = (noteStemDir != STEMDIRECTION_up) ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
        }
    }
    else if (noteStemDir == STEMDIRECTION_up) {
        drawingCurveDir = curvature_CURVEDIR_below;
    }
    else if (noteStemDir == STEMDIRECTION_NONE) {
        // no information from the note stem directions, look at the position in the notes
        drawingCurveDir = (isAboveStaffCenter) ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
    }

    return drawingCurveDir;
}

std::pair<Point, Point> Slur::AdjustCoordinates(
    Doc *doc, Staff *staff, std::pair<Point, Point> points, int spanningType, curvature_CURVEDIR drawingCurveDir)
{
    StemmedDrawingInterface *startStemDrawInterface = dynamic_cast<StemmedDrawingInterface *>(this->GetStart());
    StemmedDrawingInterface *endStemDrawInterface = dynamic_cast<StemmedDrawingInterface *>(this->GetEnd());

    data_STEMDIRECTION startStemDir = STEMDIRECTION_NONE;
    int startStemLen = 0;
    if (startStemDrawInterface) {
        startStemDir = startStemDrawInterface->GetDrawingStemDir();
        startStemLen = startStemDrawInterface->GetDrawingStemLen();
    }
    data_STEMDIRECTION endStemDir = STEMDIRECTION_NONE;
    int endStemLen = 0;
    if (endStemDrawInterface) {
        endStemDir = endStemDrawInterface->GetDrawingStemDir();
        endStemLen = endStemDrawInterface->GetDrawingStemLen();
    }

    // Pointers for the start point of the slur
    Note *startNote = NULL;
    LayerElement *start = this->GetStart();
    Chord *startChord = NULL;
    if (start->Is(NOTE)) {
        startNote = vrv_cast<Note *>(start);
        assert(startNote);
        startChord = startNote->IsChordTone();
    }
    else if (start->Is(CHORD)) {
        startChord = vrv_cast<Chord *>(start);
        assert(startChord);
    }

    // Pointers for the end point of the slur
    Note *endNote = NULL;
    LayerElement *end = this->GetEnd();
    Chord *endChord = NULL;
    if (end->Is(NOTE)) {
        endNote = vrv_cast<Note *>(end);
        assert(endNote);
        endChord = endNote->IsChordTone();
    }
    else if (end->Is(CHORD)) {
        endChord = vrv_cast<Chord *>(end);
        assert(endChord);
    }

    const bool isGraceToNoteSlur
        = !start->Is(TIMESTAMP_ATTR) && !end->Is(TIMESTAMP_ATTR) && start->IsGraceNote() && !end->IsGraceNote();

    int x1, x2, y1, y2;
    std::tie(x1, x2, y1, y2) = std::tie(points.first.x, points.second.x, points.first.y, points.second.y);

    bool isShortSlur = false;
    if (x2 - x1 < doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize)) isShortSlur = true;

    /************** calculate the radius for adjusting the x position **************/

    int startRadius = 0;
    if (!start->Is(TIMESTAMP_ATTR)) {
        startRadius = start->GetDrawingRadius(doc);
    }

    int endRadius = 0;
    if (!end->Is(TIMESTAMP_ATTR)) {
        endRadius = end->GetDrawingRadius(doc);
    }

    Beam *parentBeam = NULL;
    FTrem *parentFTrem = NULL;
    int yChordMax = 0, yChordMin = 0;
    const int unit = doc->GetDrawingUnit(staff->m_drawingStaffSize);
    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START)) {
        // first get the min max of the chord (if any)
        if (startChord) {
            startChord->GetYExtremes(yChordMax, yChordMin);
        }
        // slur is up
        if (drawingCurveDir == curvature_CURVEDIR_above) {
            // P(^)
            if (startStemDir == STEMDIRECTION_down || startStemLen == 0)
                y1 = start->GetDrawingTop(doc, staff->m_drawingStaffSize);
            //  d(^)d
            else if (isShortSlur) {
                y1 = start->GetDrawingTop(doc, staff->m_drawingStaffSize);
            }
            // same but in beam - adjust the x too
            else if (((parentBeam = start->IsInBeam()) && !parentBeam->IsLastIn(parentBeam, start))
                || ((parentFTrem = start->IsInFTrem()) && !parentFTrem->IsLastIn(parentFTrem, start))
                || isGraceToNoteSlur) {
                y1 = start->GetDrawingTop(doc, staff->m_drawingStaffSize);
                x1 += startRadius - doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            }
            // d(^)
            else {
                // put it on the side, move it left, but not if we have a @tstamp
                if (!start->Is(TIMESTAMP_ATTR) && (startStemLen != 0)) x1 += unit * 2;
                if (startChord)
                    y1 = yChordMax + unit * 3;
                else
                    y1 = start->GetDrawingY() + unit * 3;
            }
        }
        // slur is down
        else {
            // grace note
            if (isGraceToNoteSlur) {
                y1 = start->GetDrawingBottom(doc, staff->m_drawingStaffSize);
                if (startStemDir != STEMDIRECTION_up) {
                    x1 -= startRadius + doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
                }
                else {
                    y1 += unit / 2;
                }
            }
            // d(_)
            else if (startStemDir == STEMDIRECTION_up || startStemLen == 0)
                y1 = start->GetDrawingBottom(doc, staff->m_drawingStaffSize);
            // P(_)P
            else if (isShortSlur) {
                y1 = start->GetDrawingBottom(doc, staff->m_drawingStaffSize);
            }
            // same but in beam
            else if (((parentBeam = start->IsInBeam()) && !parentBeam->IsLastIn(parentBeam, start))
                || ((parentFTrem = start->IsInFTrem()) && !parentFTrem->IsLastIn(parentFTrem, start))) {
                y1 = start->GetDrawingBottom(doc, staff->m_drawingStaffSize);
                x1 -= startRadius - doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            }
            // P(_)
            else {
                // put it on the side, but no need to move it left
                if (startChord) {
                    y1 = yChordMin - unit * 3;
                }
                else {
                    y1 = start->GetDrawingY() - unit * 3;
                }
            }
        }
    }
    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_END)) {
        // get the min max of the chord if any
        if (endChord) {
            endChord->GetYExtremes(yChordMax, yChordMin);
        }
        // get the stem direction of the end
        // slur is up
        if (drawingCurveDir == curvature_CURVEDIR_above) {
            // (^)P
            if (endStemDir == STEMDIRECTION_down || endStemLen == 0)
                y2 = end->GetDrawingTop(doc, staff->m_drawingStaffSize);
            // d(^)d
            else if (isShortSlur) {
                y2 = end->GetDrawingTop(doc, staff->m_drawingStaffSize);
            }
            // same but in beam - adjust the x too
            else if ((((parentBeam = end->IsInBeam()) && !parentBeam->IsFirstIn(parentBeam, end))
                         || ((parentFTrem = end->IsInFTrem()) && !parentFTrem->IsFirstIn(parentFTrem, end)))
                && !isGraceToNoteSlur) {
                y2 = end->GetDrawingTop(doc, staff->m_drawingStaffSize);
                x2 += endRadius - doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            }
            else if (isGraceToNoteSlur) {
                if (start->IsInBeam()) {
                    x2 += 2 * doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
                }
                else if (parentBeam || parentFTrem) {
                    if (end->GetDrawingY() > y1) {
                        y2 = end->GetDrawingY() + unit * 3;
                    }
                    else {
                        y2 = y1;
                        x2 += 2 * doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
                    }
                }
                else {
                    y2 = end->GetDrawingTop(doc, staff->m_drawingStaffSize);
                    x2 += endRadius - doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
                }
            }
            // (^)d
            else {
                // put it on the side, no need to move it right
                if (endChord) {
                    y2 = yChordMax + unit * 3;
                }
                else {
                    y2 = end->GetDrawingY() + unit * 3;
                }
            }
        }
        else {
            // (_)d
            if (endStemDir == STEMDIRECTION_up || endStemLen == 0) {
                y2 = end->GetDrawingBottom(doc, staff->m_drawingStaffSize);
            }
            // P(_)P
            else if (isShortSlur && !isGraceToNoteSlur) {
                y2 = end->GetDrawingBottom(doc, staff->m_drawingStaffSize);
            }
            // same but in beam
            else if ((((parentBeam = end->IsInBeam()) && !parentBeam->IsFirstIn(parentBeam, end))
                         || ((parentFTrem = end->IsInFTrem()) && !parentFTrem->IsFirstIn(parentFTrem, end)))
                && !isGraceToNoteSlur) {

                y2 = end->GetDrawingBottom(doc, staff->m_drawingStaffSize);
                x2 -= endRadius - doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            }
            else if (isGraceToNoteSlur) {
                if (start->IsInBeam()) {
                    x2 -= endRadius + 2 * doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
                }
                else if (parentBeam || parentFTrem) {
                    if (end->GetDrawingY() < y1) {
                        y2 = end->GetDrawingY();
                        x2 -= endRadius + doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
                    }
                    else {
                        y2 = y1;
                        x2 -= endRadius + doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
                    }
                }
                else {
                    y2 = end->GetDrawingBottom(doc, staff->m_drawingStaffSize);
                    x2 -= endRadius;
                }
            }
            // (_)P
            else {
                // put it on the side, move it right, but not if we have a @tstamp2
                if (!end->Is(TIMESTAMP_ATTR)) x2 -= unit * 2;
                if (endChord) {
                    y2 = yChordMin - unit * 3;
                }
                else {
                    y2 = end->GetDrawingY() - unit * 3;
                }
            }
        }
    }

    // Positions not attached to a note
    if (spanningType == SPANNING_START) {
        if (drawingCurveDir == curvature_CURVEDIR_above) {
            y2 = staff->GetDrawingY();
        }
        else {
            y2 = staff->GetDrawingY() - doc->GetDrawingStaffSize(staff->m_drawingStaffSize);
        }
        // At the end of a system, the slur finishes just short of the last barline
        x2 -= (doc->GetDrawingBarLineWidth(staff->m_drawingStaffSize) + unit) / 2;
    }
    if (end->Is(TIMESTAMP_ATTR)) {
        if (drawingCurveDir == curvature_CURVEDIR_above) {
            y2 = std::max(staff->GetDrawingY(), y1);
        }
        else {
            y2 = std::min(staff->GetDrawingY() - doc->GetDrawingStaffSize(staff->m_drawingStaffSize), y1);
        }
    }
    if (spanningType == SPANNING_END) {
        if (drawingCurveDir == curvature_CURVEDIR_above) {
            y1 = staff->GetDrawingY();
        }
        else {
            y1 = staff->GetDrawingY() - doc->GetDrawingStaffSize(staff->m_drawingStaffSize);
        }
    }
    if (start->Is(TIMESTAMP_ATTR)) {
        if (drawingCurveDir == curvature_CURVEDIR_above) {
            y1 = std::max(staff->GetDrawingY(), y2);
        }
        else {
            y1 = std::min(staff->GetDrawingY() - doc->GetDrawingStaffSize(staff->m_drawingStaffSize), y2);
        }
    }
    // slur across an entire system; use the staff position
    else if (spanningType == SPANNING_MIDDLE) {
        // To be adjusted
        if (drawingCurveDir == curvature_CURVEDIR_above) {
            y1 = staff->GetDrawingY();
        }
        else {
            y1 = staff->GetDrawingY() - doc->GetDrawingStaffSize(staff->m_drawingStaffSize);
        }
        y2 = y1;
    }

    return std::make_pair(Point(x1, y1), Point(x2, y2));
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Slur::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    ControlElement::ResetDrawing(functorParams);

    m_drawingCurvedir = curvature_CURVEDIR_NONE;
    // m_isCrossStaff = false;

    return FUNCTOR_CONTINUE;
}

int Slur::AdjustCrossStaffContent(FunctorParams *functorParams)
{
    AdjustCrossStaffContentParams *params = vrv_params_cast<AdjustCrossStaffContentParams *>(functorParams);
    assert(params);

    FloatingPositioner *positioner = this->GetCurrentFloatingPositioner();
    if (!positioner) return FUNCTOR_CONTINUE;

    FloatingCurvePositioner *curve = vrv_cast<FloatingCurvePositioner *>(positioner);
    assert(curve);

    if (curve->IsCrossStaff()) {
        // Construct list of spanned elements including start and end of slur
        std::list<LayerElement *> slurElements = { this->GetStart(), this->GetEnd() };
        const ArrayOfCurveSpannedElements *spannedElements = curve->GetSpannedElements();
        std::for_each(spannedElements->begin(), spannedElements->end(), [&slurElements](CurveSpannedElement *element) {
            LayerElement *layerElement = dynamic_cast<LayerElement *>(element->m_boundingBox);
            if (layerElement) slurElements.push_back(layerElement);
        });

        // Determine top and bottom staff
        Staff *topStaff = NULL;
        Staff *bottomStaff = NULL;
        for (LayerElement *element : slurElements) {
            Layer *layer = NULL;
            Staff *staff = element->GetCrossStaff(layer);
            if (!staff) staff = vrv_cast<Staff *>(element->GetFirstAncestor(STAFF));
            assert(staff);

            if (topStaff && bottomStaff) {
                if (staff->GetN() < topStaff->GetN()) topStaff = staff;
                if (staff->GetN() > bottomStaff->GetN()) bottomStaff = staff;
            }
            else { // first iteration => initialize everything
                topStaff = staff;
                bottomStaff = staff;
            }
        }

        if (topStaff != bottomStaff) {
            // Now calculate the shift due to vertical justification
            auto getShift = [params](Staff *staff) {
                StaffAlignment *alignment = staff->GetAlignment();
                if (params->m_shiftForStaff.find(alignment) != params->m_shiftForStaff.end()) {
                    return params->m_shiftForStaff.at(alignment);
                }
                return 0;
            };

            const int shift = getShift(bottomStaff) - getShift(topStaff);

            // Apply the shift
            if ((curve->GetAlignment() == topStaff->GetAlignment()) && (curve->GetDir() == curvature_CURVEDIR_below)) {
                curve->SetDrawingYRel(curve->GetDrawingYRel() + shift, true);
            }
            if ((curve->GetAlignment() == bottomStaff->GetAlignment())
                && (curve->GetDir() == curvature_CURVEDIR_above)) {
                curve->SetDrawingYRel(curve->GetDrawingYRel() - shift, true);
            }
        }
    }
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
