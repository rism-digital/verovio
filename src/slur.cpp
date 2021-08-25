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
        this->GetControlPoints(bezier, curveDir, true);

        // The slur is being adjusted
        adjusted = true;
        bezier.Rotate(slurAngle, bezier.p1);
        bezier.UpdateControlPointParameters(curveDir);

        // Use the adjusted control points for adjusting the position
        this->AdjustSlurPosition(doc, curve, bezier);
        // Recalculate the control points and update the bezier curve
        this->GetControlPoints(bezier, curveDir, true);
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
    int height = abs(bezierCurve.c1.y - bezierCurve.p1.y) * sqrt(doc->GetOptions()->m_slurCurveFactor.GetValue()) / 3.0;

    height = std::min(height, 2 * doc->GetDrawingOctaveSize(staffSize));
    height = std::min<int>(height, bezierCurve.GetLeftControlPointOffset() * cos(angle) / 3.0);

    bezierCurve.SetControlHeight(height);
}

void Slur::AdjustSlurPosition(Doc *doc, FloatingCurvePositioner *curve, BezierCurve &bezierCurve)
{
    const int margin = doc->GetDrawingUnit(100);
    Point points[4];
    points[0] = bezierCurve.p1;
    points[1] = bezierCurve.c1;
    points[2] = bezierCurve.c2;
    points[3] = bezierCurve.p2;
    curve->UpdateCurveParams(points, curve->GetAngle(), curve->GetThickness(), curve->GetDir());

    int endPointShiftLeft = 0;
    int endPointShiftRight = 0;
    std::tie(endPointShiftLeft, endPointShiftRight) = this->CalcEndShift(curve, bezierCurve, margin);
    const int sign = (curve->GetDir() == curvature_CURVEDIR_above) ? 1 : -1;
    bezierCurve.p1.y += sign * endPointShiftLeft;
    bezierCurve.p2.y += sign * endPointShiftRight;
    if (bezierCurve.p1.x != bezierCurve.p2.x) {
        double lambda = double(bezierCurve.c1.x - bezierCurve.p1.x) / double(bezierCurve.p2.x - bezierCurve.p1.x);
        bezierCurve.c1.y += sign * ((1.0 - lambda) * endPointShiftLeft + lambda * endPointShiftRight);
        lambda = double(bezierCurve.c2.x - bezierCurve.p1.x) / double(bezierCurve.p2.x - bezierCurve.p1.x);
        bezierCurve.c2.y += sign * ((1.0 - lambda) * endPointShiftLeft + lambda * endPointShiftRight);
    }
    bezierCurve.UpdateControlPointParameters(curve->GetDir());
    points[0] = bezierCurve.p1;
    points[1] = bezierCurve.c1;
    points[2] = bezierCurve.c2;
    points[3] = bezierCurve.p2;
    curve->UpdateCurveParams(points, curve->GetAngle(), curve->GetThickness(), curve->GetDir());

    bool ok = false;
    int CPOffsetLeft = 0;
    int CPOffsetRight = 0;
    std::tie(ok, CPOffsetLeft, CPOffsetRight) = this->CalcCPOffset(curve, bezierCurve);
    if (ok) {
        bezierCurve.SetLeftControlPointOffset(CPOffsetLeft);
        bezierCurve.SetRightControlPointOffset(CPOffsetRight);
        GetControlPoints(bezierCurve, curve->GetDir(), true);
        points[0] = bezierCurve.p1;
        points[1] = bezierCurve.c1;
        points[2] = bezierCurve.c2;
        points[3] = bezierCurve.p2;
        curve->UpdateCurveParams(points, curve->GetAngle(), curve->GetThickness(), curve->GetDir());
    }

    int CPShiftLeft = 0;
    int CPShiftRight = 0;
    std::tie(CPShiftLeft, CPShiftRight) = this->CalcCPVerticalShift(curve, bezierCurve, margin);
    bezierCurve.SetLeftControlHeight(bezierCurve.GetLeftControlHeight() + CPShiftLeft);
    bezierCurve.SetRightControlHeight(bezierCurve.GetRightControlHeight() + CPShiftRight);
    GetControlPoints(bezierCurve, curve->GetDir(), true);
    points[0] = bezierCurve.p1;
    points[1] = bezierCurve.c1;
    points[2] = bezierCurve.c2;
    points[3] = bezierCurve.p2;
    curve->UpdateCurveParams(points, curve->GetAngle(), curve->GetThickness(), curve->GetDir());
}

std::pair<int, int> Slur::CalcEndShift(FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, int margin)
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

            if (distanceRatio < 0.15) {
                if (distanceRatio > 0.05) {
                    intersection *= pow(1.5 - 10.0 * distanceRatio, 2.0);
                }
                shiftLeft = std::max(shiftLeft, intersection);
            }
            else if (distanceRatio > 0.85) {
                if (distanceRatio < 0.95) {
                    intersection *= pow(10.0 * distanceRatio - 8.5, 2.0);
                }
                shiftRight = std::max(shiftRight, intersection);
            }
        }
    }
    return { shiftLeft, shiftRight };
}

std::tuple<bool, int, int> Slur::CalcCPOffset(FloatingCurvePositioner *curve, const BezierCurve &bezierCurve)
{
    if (bezierCurve.p1.x >= bezierCurve.p2.x) return { false, 0, 0 };

    double leftSlopeMax = abs(BoundingBox::CalcSlope(bezierCurve.p1, bezierCurve.c1));
    double rightSlopeMax = abs(BoundingBox::CalcSlope(bezierCurve.p2, bezierCurve.c2));
    const ArrayOfCurveSpannedElements *spannedElements = curve->GetSpannedElements();
    for (auto spannedElement : *spannedElements) {

        if (spannedElement->m_discarded) {
            continue;
        }

        const int bbY = (curve->GetDir() == curvature_CURVEDIR_above) ? spannedElement->m_boundingBox->GetSelfTop()
                                                                      : spannedElement->m_boundingBox->GetSelfBottom();
        const Point pLeft = { spannedElement->m_boundingBox->GetSelfLeft(), bbY };
        const Point pRight = { spannedElement->m_boundingBox->GetSelfRight(), bbY };

        double slope = 2.0 * BoundingBox::CalcSlope(bezierCurve.p1, pLeft);
        if ((slope > 0.0) && (curve->GetDir() == curvature_CURVEDIR_above)) {
            leftSlopeMax = std::max(leftSlopeMax, slope);
        }
        if ((slope < 0.0) && (curve->GetDir() == curvature_CURVEDIR_below)) {
            leftSlopeMax = std::max(leftSlopeMax, abs(slope));
        }
        slope = BoundingBox::CalcSlope(bezierCurve.p2, pRight);
        if ((slope < 0.0) && (curve->GetDir() == curvature_CURVEDIR_above)) {
            rightSlopeMax = std::max(rightSlopeMax, abs(slope));
        }
        if ((slope > 0.0) && (curve->GetDir() == curvature_CURVEDIR_below)) {
            rightSlopeMax = std::max(rightSlopeMax, slope);
        }
    }

    if ((leftSlopeMax == 0.0) || (rightSlopeMax == 0.0)) return { false, 0, 0 };

    const int leftOffset = abs(bezierCurve.GetLeftControlHeight()) / leftSlopeMax;
    const int rightOffset = abs(bezierCurve.GetRightControlHeight()) / rightSlopeMax;
    return { true, leftOffset, rightOffset };
}

std::pair<int, int> Slur::CalcCPVerticalShift(
    FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, int margin)
{
    std::list<CPInequality> constraints;

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

            const int xLeft = std::max(bezierCurve.p1.x, spannedElement->m_boundingBox->GetSelfLeft());
            float distanceRatio = float(xLeft - bezierCurve.p1.x) / float(dist);
            if (abs(0.5 - distanceRatio) < 0.45) {
                const double t = BoundingBox::CalcBezierParamAtPosition(points, xLeft);
                constraints.push_back(
                    { 3.0 * pow(1.0 - t, 2.0) * t, 3.0 * (1.0 - t) * pow(t, 2.0), double(intersection) });
            }

            const int xRight = std::min(bezierCurve.p2.x, spannedElement->m_boundingBox->GetSelfRight());
            distanceRatio = float(xRight - bezierCurve.p1.x) / float(dist);
            if (abs(0.5 - distanceRatio) < 0.45) {
                const double t = BoundingBox::CalcBezierParamAtPosition(points, xRight);
                constraints.push_back(
                    { 3.0 * pow(1.0 - t, 2.0) * t, 3.0 * (1.0 - t) * pow(t, 2.0), double(intersection) });
            }
        }
    }
    return this->SolveCPConstraints(constraints);
}

std::pair<int, int> Slur::SolveCPConstraints(const std::list<CPInequality> &constraints)
{
    if (constraints.empty()) return { 0, 0 };

    double weightSum = 0.0;
    double weightedAngleSum = 0.0;
    for (const CPInequality &constraint : constraints) {
        // Use distance of line to origin as weight
        const double weight = constraint.c / std::hypot(constraint.a, constraint.b);
        weightedAngleSum += weight * atan(constraint.b / constraint.a);
        weightSum += weight;
    }
    const double slope = tan(weightedAngleSum / weightSum);

    // Solve slope * x = c/b - a/b * x for each constraint
    double xMax = 0.0;
    for (const CPInequality &constraint : constraints) {
        const double x = constraint.c / (constraint.a + slope * constraint.b);
        xMax = std::max(xMax, x);
    }
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

void Slur::GetControlPoints(BezierCurve &bezier, curvature_CURVEDIR curveDir, bool ignoreAngle)
{
    const float slurAngle = (bezier.p2 == bezier.p1) ? 0 : atan2(bezier.p2.y - bezier.p1.y, bezier.p2.x - bezier.p1.x);
    if ((slurAngle != 0.0) && !ignoreAngle) {
        bezier.p2 = BoundingBox::CalcPositionAfterRotation(bezier.p2, -slurAngle, bezier.p1);
        // It should not be the case but we do need to avoid recursive calls whatever the effect in the resutls
        if (bezier.p2.y != bezier.p1.y) bezier.p2.y = bezier.p1.y;
        GetControlPoints(bezier, curveDir);
        bezier.Rotate(slurAngle, bezier.p1);
        return;
    }

    bezier.c1.x = bezier.p1.x + bezier.GetLeftControlPointOffset();
    bezier.c2.x = bezier.p2.x - bezier.GetRightControlPointOffset();

    if (curveDir == curvature_CURVEDIR_above) {
        bezier.c1.y = bezier.p1.y + bezier.GetLeftControlHeight();
        bezier.c2.y = bezier.p2.y + bezier.GetRightControlHeight();
    }
    else {
        bezier.c1.y = bezier.p1.y - bezier.GetLeftControlHeight();
        bezier.c2.y = bezier.p2.y - bezier.GetRightControlHeight();
    }
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
