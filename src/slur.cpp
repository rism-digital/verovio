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
    bezier.CalculateControlPointOffset(doc, staff->m_drawingStaffSize);

    bool adjusted = false;
    if (!spannedElements->empty()) {
        // Bound the height of the control points
        AdjustControlPointHeight(doc, bezier, staff->m_drawingStaffSize);
        GetControlPoints(bezier, curveDir);

        // The slur is being adjusted
        adjusted = true;
        bezier.Rotate(slurAngle, bezier.p1);
        bool ignoreAngle = false;

        // Use the adjusted control points for adjusting the position
        ignoreAngle = AdjustSlurPosition(doc, curve, bezier, slurAngle, false);
        // Recalculate the control points and update the bezier curve
        GetControlPoints(bezier, curveDir, ignoreAngle);

        points[0] = bezier.p1;
        points[1] = bezier.c1;
        points[2] = bezier.c2;
        points[3] = bezier.p2;
        curve->UpdateCurveParams(points, slurAngle, curve->GetThickness(), curveDir);

        // Check again and force both sides to move
        AdjustSlurPosition(doc, curve, bezier, slurAngle, true);
        GetControlPoints(bezier, curveDir, ignoreAngle);
    }

    if (adjusted) {
        points[0] = bezier.p1;
        points[1] = bezier.c1; // BoundingBox::CalcPositionAfterRotation(rotatedC1, slurAngle, *p1);
        points[2] = bezier.c2; // BoundingBox::CalcPositionAfterRotation(rotatedC2, slurAngle, *p1);
        points[3] = bezier.p2; // BoundingBox::CalcPositionAfterRotation(rotatedP2, slurAngle, *p1);
        curve->UpdateCurveParams(points, slurAngle, curve->GetThickness(), curveDir);
        // Since we are going to redraw-it reset its bounding box
        curve->BoundingBox::ResetBoundingBox();
    }

    return adjusted;
}

void Slur::AdjustControlPointHeight(Doc *doc, BezierCurve &bezierCurve, int staffSize)
{
    const int currentHeight = abs(bezierCurve.c1.y - bezierCurve.p1.y);
    int height = currentHeight * sqrt(doc->GetOptions()->m_slurCurveFactor.GetValue()) / 3.0;

    height = std::min(height, 2 * doc->GetDrawingOctaveSize(staffSize));

    bezierCurve.SetControlHeight(height);
}

bool Slur::AdjustSlurPosition(
    Doc *doc, FloatingCurvePositioner *curve, BezierCurve &bezierCurve, float &angle, bool forceBothSides)
{
    bool isNotAdjustable = false;
    const int margin = (curve->IsCrossStaff() ? 2 : 1) * doc->GetDrawingUnit(100);
    int maxShiftLeft = 0;
    int maxShiftRight = 0;
    std::tie(maxShiftLeft, maxShiftRight)
        = CalculateAdjustedSlurShift(curve, bezierCurve, margin, forceBothSides, isNotAdjustable);
    if (!maxShiftLeft && !maxShiftRight) return false;

    // If curve is cross staff and shifts are larger than current height of the control points - adjust control point
    // height to make sure that slur bends around the overlapping elements
    if (curve->IsCrossStaff() && !isNotAdjustable) {
        if ((maxShiftLeft > bezierCurve.GetLeftControlHeight())
            || (maxShiftRight > bezierCurve.GetRightControlHeight())) {
            if ((bezierCurve.c1.x < bezierCurve.p1.x) || (bezierCurve.c2.x > bezierCurve.p2.x)) return true;
            bezierCurve.SetLeftControlPointOffset(0.5 * bezierCurve.GetLeftControlPointOffset());
            bezierCurve.SetRightControlPointOffset(0.5 * bezierCurve.GetRightControlPointOffset());
            bezierCurve.SetLeftControlHeight(bezierCurve.GetLeftControlHeight() + 1.2 * maxShiftLeft);
            bezierCurve.SetRightControlHeight(bezierCurve.GetRightControlHeight() + 1.2 * maxShiftRight);
            if ((maxShiftLeft > 0) && (maxShiftRight == 0)) {
                bezierCurve.SetLeftControlHeight(2.0 * bezierCurve.GetLeftControlHeight());
                bezierCurve.SetRightControlPointOffset(2 * bezierCurve.GetRightControlPointOffset());
                bezierCurve.SetRightControlHeight(0.75 * bezierCurve.GetRightControlHeight());
            }
            else if ((maxShiftRight > 0) && (maxShiftLeft == 0)) {
                bezierCurve.SetRightControlHeight(2.0 * bezierCurve.GetRightControlHeight());
                bezierCurve.SetLeftControlPointOffset(2 * bezierCurve.GetLeftControlPointOffset());
                bezierCurve.SetLeftControlHeight(0.75 * bezierCurve.GetLeftControlHeight());
            }
            if (std::abs(double(bezierCurve.p2.y - bezierCurve.p1.y) / double(bezierCurve.p2.x - bezierCurve.p1.x))
                > 2.0) {
                if (((curve->GetDir() == curvature_CURVEDIR_below) && (bezierCurve.p1.y > bezierCurve.p2.y))
                    || ((curve->GetDir() == curvature_CURVEDIR_above) && (bezierCurve.p1.y < bezierCurve.p2.y))) {
                    bezierCurve.SetLeftControlPointOffset(0.5 * bezierCurve.GetLeftControlPointOffset());
                }
                else if (((curve->GetDir() == curvature_CURVEDIR_above) && (bezierCurve.p1.y > bezierCurve.p2.y))
                    || ((curve->GetDir() == curvature_CURVEDIR_below) && (bezierCurve.p1.y < bezierCurve.p2.y))) {
                    bezierCurve.SetRightControlPointOffset(0.5 * bezierCurve.GetRightControlPointOffset());
                }
            }
            return true;
        }
        else {
            Point points[4];
            points[0] = bezierCurve.p1;
            points[1] = bezierCurve.c1;
            points[2] = bezierCurve.c2;
            points[3] = bezierCurve.p2;
            // Approximate bezier extrema and find time at which curve has highest/lowest Y value
            double time = 0.0;
            int yPos = 0;
            std::tie(time, yPos)
                = BoundingBox::ApproximateBezierExtrema(points, (curve->GetDir() == curvature_CURVEDIR_above));

            const double extremaShift = time - 0.5;
            const int relevantPoint = extremaShift < 0 ? bezierCurve.p1.y : bezierCurve.p2.y;
            Object *startMeasure
                = GetStart()->m_crossStaff ? GetStart()->m_crossStaff : GetStart()->GetFirstAncestor(MEASURE);
            Object *endMeasure = GetEnd()->m_crossStaff ? GetEnd()->m_crossStaff : GetEnd()->GetFirstAncestor(MEASURE);
            // We need to adjust curve based whether extrema time is higher/lower that 0.2 from the center (i.e. values
            // between [0.3; 0.7] are ok). For values that are lower than 0.3 we need to shift left control point base
            // to the right, and vice versa for the values above 0.7. This wouldn't exactly work for the slur that are
            // spanning over several measures, so we ignore them here
            if ((std::abs(extremaShift) > 0.2) && (startMeasure == endMeasure)
                && ((std::abs(relevantPoint - yPos) > (std::abs(bezierCurve.p1.y - bezierCurve.p2.y) / 50)))) {
                const int xDist = std::abs(bezierCurve.p1.x - bezierCurve.p2.x);
                if (extremaShift < 0) {
                    bezierCurve.SetLeftControlPointOffset(xDist / 2 - bezierCurve.GetRightControlPointOffset());
                    bezierCurve.SetRightControlPointOffset(0);
                }
                else {
                    bezierCurve.SetRightControlPointOffset(xDist / 2 - bezierCurve.GetLeftControlPointOffset());
                    bezierCurve.SetLeftControlPointOffset(0);
                }
                return true;
            }
            else {
                maxShiftLeft = maxShiftRight = 0.8 * std::min(maxShiftLeft, maxShiftRight);
                bezierCurve.p1.y += (curve->GetDir() == curvature_CURVEDIR_above) ? maxShiftLeft : -maxShiftLeft;
                bezierCurve.p2.y += (curve->GetDir() == curvature_CURVEDIR_above) ? maxShiftRight : -maxShiftRight;
                return false;
            }
        }
    }
    // otherwise it is normal slur - just move position of the start/end points up or down and recalculate angle
    else {
        // if slur is in the state where it cannot be adjusted (e.g. when there is too bid intersection with other
        // elements), then try to adjust one of the ends of the slur. Non-adjustable slur generally end up having one of
        // their ends just hanging over the staff (since we lift both ends of slur), so by doing following adjustment
        // it's possible to make those slurs look slightly better
        if (isNotAdjustable) {
            if (std::abs(maxShiftLeft) > std::abs(maxShiftRight)) {
                maxShiftRight /= 4;
            }
            else if (std::abs(maxShiftLeft) < std::abs(maxShiftRight)) {
                maxShiftLeft /= 4;
            }
        }
        bezierCurve.p1.y += (curve->GetDir() == curvature_CURVEDIR_above) ? maxShiftLeft : -maxShiftLeft;
        bezierCurve.p2.y += (curve->GetDir() == curvature_CURVEDIR_above) ? maxShiftRight : -maxShiftRight;

        angle = GetAdjustedSlurAngle(
            doc, bezierCurve.p1, bezierCurve.p2, curve->GetDir(), !curve->IsCrossStaff() && !isNotAdjustable);
        return false;
    }
}

std::pair<int, int> Slur::CalculateAdjustedSlurShift(FloatingCurvePositioner *curve, const BezierCurve &bezierCurve,
    int margin, bool forceBothSides, bool &isNotAdjustable)
{
    int maxShiftLeft = 0;
    int maxShiftRight = 0;

    int dist = std::abs(bezierCurve.p2.x - bezierCurve.p1.x);
    float posXRatio = 1.0;

    const ArrayOfCurveSpannedElements *spannedElements = curve->GetSpannedElements();
    // Actually nothing to do
    if (spannedElements->empty()) return { 0, 0 };

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

        if (discard == true) {
            spannedElement->m_discarded = true;
            continue;
        }

        if (intersection == 0) {
            continue;
        }

        // In case of cross-staff, intersection with the note that is in the staff directly under the start/end point
        // might result in too big curve or strange slurs. If intersection is bigger than maximum height of the
        // cross-staff slur, we should ignore it.
        if (curve->IsCrossStaff()
            && (intersection > std::max(std::abs(rightPointMaxHeight), std::abs(leftPointMaxHeight)))) {
            continue;
        }

        int xLeft = std::max(bezierCurve.p1.x, spannedElement->m_boundingBox->GetSelfLeft());
        int xRight = std::min(bezierCurve.p2.x, spannedElement->m_boundingBox->GetSelfRight());
        int xMiddle = xLeft + ((xRight - xLeft) / 2);
        int posX = xMiddle - bezierCurve.p1.x;

        // Weight the desired height according to the x position on the other side
        posXRatio = 1.0;
        bool leftPoint = true;
        if (posX > dist / 2) {
            posX = bezierCurve.p2.x - xMiddle;
            leftPoint = false;
        }
        if (dist != 0) posXRatio = (float)posX / ((float)dist / 2.0);

        if (intersection > 0) {
            int leftShift = (forceBothSides || leftPoint) ? intersection : intersection * posXRatio;
            int rightShift = (forceBothSides || !leftPoint) ? intersection : intersection * posXRatio;
            // Keep the maximum shift on the left and right
            maxShiftLeft = leftShift > maxShiftLeft ? leftShift : maxShiftLeft;
            maxShiftRight = rightShift > maxShiftRight ? rightShift : maxShiftRight;
        }

        // if intersection happens on the start/end of the slur, make sure that there is enough place for proper slur to
        // be drawn. If intersection is too large, cross-staff slurs should not be drawn with adjusted angles to avoid
        // extreme cases
        const float distanceRatio = float(xMiddle - bezierCurve.p1.x) / float(dist);
        if (((distanceRatio < 0.1) && (intersection > leftPointMaxHeight / 2))
            || ((distanceRatio > 0.9) && (intersection > rightPointMaxHeight / 2)))
            isNotAdjustable = true;
    }
    if (curve->GetDir() == curvature_CURVEDIR_above) {
        if (leftPointMaxHeight + margin < 0) maxShiftLeft = 0;
        if (rightPointMaxHeight + margin < 0) maxShiftRight = 0;
    }
    else {
        if (leftPointMaxHeight - margin > 0) maxShiftLeft = 0;
        if (rightPointMaxHeight - margin > 0) maxShiftRight = 0;
    }

    return { maxShiftLeft, maxShiftRight };
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

    FloatingCurvePositioner *curve = vrv_cast<FloatingCurvePositioner *>(this->GetCurrentFloatingPositioner());
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
        Staff *startStaff = NULL;
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
                startStaff = staff;
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
            if ((startStaff == topStaff) && (curve->GetDir() == curvature_CURVEDIR_below)) {
                curve->SetDrawingYRel(curve->GetDrawingYRel() + shift);
            }
            if ((startStaff == bottomStaff) && (curve->GetDir() == curvature_CURVEDIR_above)) {
                curve->SetDrawingYRel(curve->GetDrawingYRel() - shift);
            }
        }
    }
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
