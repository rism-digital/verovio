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

#include "chord.h"
#include "doc.h"
#include "functorparams.h"
#include "layer.h"
#include "layerelement.h"
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

bool Slur::AdjustSlur(Doc *doc, FloatingCurvePositioner *curve, Staff *staff)
{
    assert(doc);
    assert(curve);
    assert(staff);

    float slurAngle = curve->GetAngle();
    curvature_CURVEDIR curveDir = curve->GetDir();
    Point points[4];
    curve->GetPoints(points);
    const ArrayOfCurveSpannedElements *spannedElements = curve->GetSpannedElements();

    BezierCurve bezier(points[0], points[1], points[2], points[3]);
    bezier.Rotate(-slurAngle, points[0]);
    bezier.CalculateControlPointOffset(doc);

    bool adjusted = false;
    if (!spannedElements->empty()) {
        // Bound the height of the control points
        this->AdjustControlPointHeight(doc, bezier, curve->GetAngle(), staff->m_drawingStaffSize);
        bezier.UpdateControlPoints(curveDir);

        // The slur is being adjusted
        adjusted = true;
        bezier.Rotate(slurAngle, bezier.p1);
        bezier.UpdateControlPointParams(curveDir);

        // Use the adjusted control points for adjusting the position
        this->AdjustSlurPosition(doc, curve, bezier);
        // Recalculate the control points and update the bezier curve
        bezier.UpdateControlPoints(curveDir);
    }

    if (adjusted) {
        points[0] = bezier.p1;
        points[1] = bezier.c1;
        points[2] = bezier.c2;
        points[3] = bezier.p2;
        curve->UpdateCurveParams(points, slurAngle, curve->GetThickness(), curveDir);
        // Since we are going to redraw-it reset its bounding box
        curve->BoundingBox::ResetBoundingBox();
    }

    return adjusted;
}

void Slur::AdjustControlPointHeight(Doc *doc, BezierCurve &bezierCurve, float angle, int staffSize)
{
    int height
        = std::abs(bezierCurve.c1.y - bezierCurve.p1.y) * sqrt(doc->GetOptions()->m_slurCurveFactor.GetValue()) / 3.0;

    height = std::min(height, 2 * doc->GetDrawingOctaveSize(staffSize));
    height = std::min<int>(height, bezierCurve.GetLeftControlPointOffset() * cos(angle) / 3.0);

    bezierCurve.SetControlHeight(height);
}

void Slur::AdjustSlurPosition(Doc *doc, FloatingCurvePositioner *curve, BezierCurve &bezierCurve)
{
    const int margin = doc->GetDrawingUnit(100);
    bezierCurve.CopyPointsTo(curve);

    // STEP 1: Calculate the vertical adjustment of end points. This shifts the slur vertically.
    // Only collisions near the endpoints are taken into account.
    int endPointShiftLeft = 0;
    int endPointShiftRight = 0;
    std::tie(endPointShiftLeft, endPointShiftRight) = this->CalcEndPointShift(curve, bezierCurve, margin);
    if ((endPointShiftLeft != 0) || (endPointShiftRight != 0)) {
        const int sign = (curve->GetDir() == curvature_CURVEDIR_above) ? 1 : -1;
        bezierCurve.p1.y += sign * endPointShiftLeft;
        bezierCurve.p2.y += sign * endPointShiftRight;
        if (bezierCurve.p1.x != bezierCurve.p2.x) {
            double lambda = double(bezierCurve.c1.x - bezierCurve.p1.x) / double(bezierCurve.p2.x - bezierCurve.p1.x);
            bezierCurve.c1.y += sign * ((1.0 - lambda) * endPointShiftLeft + lambda * endPointShiftRight);
            lambda = double(bezierCurve.c2.x - bezierCurve.p1.x) / double(bezierCurve.p2.x - bezierCurve.p1.x);
            bezierCurve.c2.y += sign * ((1.0 - lambda) * endPointShiftLeft + lambda * endPointShiftRight);
        }
        bezierCurve.UpdateControlPointParams(curve->GetDir());
        bezierCurve.CopyPointsTo(curve);
    }

    // STEP 2: Calculate the horizontal offset of the control points.
    // The idea is to shift control points to the outside if there is an obstacle in the vicinity of the corresponding
    // endpoint. For C1 we consider the largest angle <)BP1P2 where B is a colliding left bounding box corner and choose
    // C1 in this direction. Similar for C2.
    bool ok = false;
    int controlPointOffsetLeft = 0;
    int controlPointOffsetRight = 0;
    std::tie(ok, controlPointOffsetLeft, controlPointOffsetRight) = this->CalcControlPointOffset(curve, bezierCurve);
    if (ok) {
        bezierCurve.SetLeftControlPointOffset(controlPointOffsetLeft);
        bezierCurve.SetRightControlPointOffset(controlPointOffsetRight);
        bezierCurve.UpdateControlPoints(curve->GetDir());
        bezierCurve.CopyPointsTo(curve);
    }

    // STEP 3: Calculate the vertical shift of the control points.
    // For each colliding bounding box we formulate a constraint ax + by >= c
    // where x, y denote the vertical adjustments of the control points and c is the size of the collision.
    // The coefficients a, b are calculated from the Bezier curve equation.
    // After collecting all constraints we calculate a solution.
    int controlPointShiftLeft = 0;
    int controlPointShiftRight = 0;
    std::tie(controlPointShiftLeft, controlPointShiftRight)
        = this->CalcControlPointVerticalShift(curve, bezierCurve, margin);
    bezierCurve.SetLeftControlHeight(bezierCurve.GetLeftControlHeight() + controlPointShiftLeft);
    bezierCurve.SetRightControlHeight(bezierCurve.GetRightControlHeight() + controlPointShiftRight);
    bezierCurve.UpdateControlPoints(curve->GetDir());
    bezierCurve.CopyPointsTo(curve);
}

std::pair<int, int> Slur::CalcEndPointShift(FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, int margin)
{
    int shiftLeft = 0;
    int shiftRight = 0;

    const int dist = std::abs(bezierCurve.p2.x - bezierCurve.p1.x);

    const ArrayOfCurveSpannedElements *spannedElements = curve->GetSpannedElements();
    for (auto spannedElement : *spannedElements) {

        if (spannedElement->m_discarded) {
            continue;
        }

        bool discard = false;
        int intersection = curve->CalcAdjustment(spannedElement->m_boundingBox, discard, margin);

        if (discard) {
            spannedElement->m_discarded = true;
            continue;
        }

        if ((intersection > 0) && (dist > 0)) {
            const int xLeft = std::max(bezierCurve.p1.x, spannedElement->m_boundingBox->GetSelfLeft());
            const int xRight = std::min(bezierCurve.p2.x, spannedElement->m_boundingBox->GetSelfRight());
            const int xMiddle = (xLeft + xRight) / 2;
            const float distanceRatio = float(xMiddle - bezierCurve.p1.x) / float(dist);

            // Filter collisions near the endpoints
            // Collisions with 0.15 <= distanceRatio <= 0.85 do not contribute to shifts
            // They are compensated later by shifting the control points
            if (distanceRatio < 0.15) {
                if (distanceRatio > 0.05) {
                    // For 0.05 <= distanceRatio <= 0.15 collisions only partially contribute to shifts
                    // We muliply with a function that interpolates between 1 and 0
                    intersection *= pow(1.5 - 10.0 * distanceRatio, 2.0);
                }
                shiftLeft = std::max(shiftLeft, intersection);
            }
            else if (distanceRatio > 0.85) {
                if (distanceRatio < 0.95) {
                    // For 0.85 <= distanceRatio <= 0.95 collisions only partially contribute to shifts
                    // We muliply with a function that interpolates between 0 and 1
                    intersection *= pow(10.0 * distanceRatio - 8.5, 2.0);
                }
                shiftRight = std::max(shiftRight, intersection);
            }
        }
    }
    return { shiftLeft, shiftRight };
}

std::tuple<bool, int, int> Slur::CalcControlPointOffset(FloatingCurvePositioner *curve, const BezierCurve &bezierCurve)
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

        // This function takes the abs value and increases the slope a bit
        auto adjustSlope = [](double slope) {
            double value = std::abs(slope);
            if (value < 2.5) {
                value = tan(atan(value) + M_PI / 18.0); // rotate by 10 degree towards the y axis
            }
            else {
                value *= 2.0;
            }
            return value;
        };

        // Prefer the (increased) slope of P1-B1, if larger
        // B1 is the upper left bounding box corner of a colliding obstacle
        double slope = BoundingBox::CalcSlope(bezierCurve.p1, pLeft);
        if ((slope > 0.0) && (curve->GetDir() == curvature_CURVEDIR_above)) {
            leftSlopeMax = std::max(leftSlopeMax, adjustSlope(slope));
        }
        if ((slope < 0.0) && (curve->GetDir() == curvature_CURVEDIR_below)) {
            leftSlopeMax = std::max(leftSlopeMax, adjustSlope(slope));
        }

        // Prefer the (increased) slope of P2-B2, if larger
        // B2 is the upper right bounding box corner of a colliding obstacle
        slope = BoundingBox::CalcSlope(bezierCurve.p2, pRight);
        if ((slope < 0.0) && (curve->GetDir() == curvature_CURVEDIR_above)) {
            rightSlopeMax = std::max(rightSlopeMax, adjustSlope(slope));
        }
        if ((slope > 0.0) && (curve->GetDir() == curvature_CURVEDIR_below)) {
            rightSlopeMax = std::max(rightSlopeMax, adjustSlope(slope));
        }
    }

    if ((leftSlopeMax == 0.0) || (rightSlopeMax == 0.0)) return { false, 0, 0 };

    const int leftOffset = std::abs(bezierCurve.GetLeftControlHeight()) / leftSlopeMax;
    const int rightOffset = std::abs(bezierCurve.GetRightControlHeight()) / rightSlopeMax;
    return { true, leftOffset, rightOffset };
}

std::pair<int, int> Slur::CalcControlPointVerticalShift(
    FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, int margin)
{
    std::list<ControlPointConstraint> constraints;

    const int dist = std::abs(bezierCurve.p2.x - bezierCurve.p1.x);

    const ArrayOfCurveSpannedElements *spannedElements = curve->GetSpannedElements();

    // Find max/min value for the spanning elements within the slur
    int extremeY = VRV_UNSET;
    std::for_each(spannedElements->begin(), spannedElements->end(),
        [dir = curve->GetDir(), &extremeY](CurveSpannedElement *element) {
            if (dir == curvature_CURVEDIR_above) {
                const int y = element->m_boundingBox->GetSelfTop();
                extremeY = (extremeY == VRV_UNSET) ? y : std::max(y, extremeY);
            }
            else {
                const int y = element->m_boundingBox->GetSelfBottom();
                extremeY = (extremeY == VRV_UNSET) ? y : std::min(y, extremeY);
            }
        });
    const int leftPointMaxHeight = extremeY - bezierCurve.p1.y;
    const int rightPointMaxHeight = extremeY - bezierCurve.p2.y;

    for (auto spannedElement : *spannedElements) {

        if (spannedElement->m_discarded) {
            continue;
        }

        bool discard = false;
        int intersection = curve->CalcAdjustment(spannedElement->m_boundingBox, discard, margin);

        if (discard) {
            spannedElement->m_discarded = true;
            continue;
        }

        // In case of cross-staff, intersection with the note that is in the staff directly under the start/end point
        // might result in too big curve or strange slurs. If intersection is bigger than maximum height of the
        // cross-staff slur, we should ignore it.
        if (curve->IsCrossStaff()
            && (intersection > std::max(std::abs(rightPointMaxHeight), std::abs(leftPointMaxHeight)))) {
            continue;
        }

        if ((intersection > 0) && (dist > 0)) {
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
                    { 3.0 * pow(1.0 - t, 2.0) * t, 3.0 * (1.0 - t) * pow(t, 2.0), double(intersection) });
            }

            // Add constraint for the right boundary of the colliding bounding box
            const int xRight = std::min(bezierCurve.p2.x, spannedElement->m_boundingBox->GetSelfRight());
            distanceRatio = float(xRight - bezierCurve.p1.x) / float(dist);
            // Ignore obstacles close to the endpoints, because this would result in very large shifts
            if (std::abs(0.5 - distanceRatio) < 0.45) {
                const double t = BoundingBox::CalcBezierParamAtPosition(points, xRight);
                constraints.push_back(
                    { 3.0 * pow(1.0 - t, 2.0) * t, 3.0 * (1.0 - t) * pow(t, 2.0), double(intersection) });
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

float Slur::GetAdjustedSlurAngle(Doc *doc, Point &p1, Point &p2, curvature_CURVEDIR curveDir, bool withPoints)
{
    float slurAngle = (p1 == p2) ? 0 : atan2(p2.y - p1.y, p2.x - p1.x);
    float maxSlope = (float)doc->GetOptions()->m_slurMaxSlope.GetValue() * M_PI / 180.0;

    // For slurs without spanning points allow for double angle
    // This normally looks better with slurs with two notes and high ambitus
    if (!withPoints) maxSlope *= 2.0;

    // the slope of the slur is high and needs to be corrected
    if (fabs(slurAngle) > maxSlope) {
        int side = (p2.x - p1.x) * sin(maxSlope) / sin(M_PI / 2 - maxSlope);
        if (p2.y > p1.y) {
            if (curveDir == curvature_CURVEDIR_above)
                p1.y = p2.y - side;
            else
                p2.y = p1.y + side;
            slurAngle = maxSlope;
        }
        else {
            if (curveDir == curvature_CURVEDIR_above)
                p2.y = p1.y - side;
            else
                p1.y = p2.y + side;
            slurAngle = -maxSlope;
        }
    }

    return slurAngle;
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
                curve->SetDrawingYRel(curve->GetDrawingYRel() + shift);
            }
            if ((curve->GetAlignment() == bottomStaff->GetAlignment())
                && (curve->GetDir() == curvature_CURVEDIR_above)) {
                curve->SetDrawingYRel(curve->GetDrawingYRel() - shift);
            }
        }
    }
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
