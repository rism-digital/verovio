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
#include "layer.h"
#include "layerelement.h"
#include "staff.h"
#include "verticalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Slur
//----------------------------------------------------------------------------

Slur::Slur() : ControlElement("slur-"), TimeSpanningInterface(), AttColor(), AttCurvature(), AttCurveRend()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_CURVATURE);
    RegisterAttClass(ATT_CURVEREND);

    Reset();
}

Slur::Slur(const std::string &classid)
    : ControlElement(classid), TimeSpanningInterface(), AttColor(), AttCurvature(), AttCurveRend()
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

    Point p1 = points[0];
    Point rotatedC1 = BoundingBox::CalcPositionAfterRotation(points[1], -slurAngle, p1);
    Point rotatedC2 = BoundingBox::CalcPositionAfterRotation(points[2], -slurAngle, p1);
    Point rotatedP2 = BoundingBox::CalcPositionAfterRotation(points[3], -slurAngle, p1);

    GetSpannedPointPositions(doc, spannedElements, p1, slurAngle, curveDir, staff->m_drawingStaffSize);

    bool adjusted = false;
    if (!spannedElements->empty()) {

        // Adjust the curvatur (control points are move)
        int adjustedHeight = AdjustSlurCurve(doc, spannedElements, p1, rotatedP2, rotatedC1, rotatedC2, curveDir,
            slurAngle, staff->m_drawingStaffSize, true);

        adjusted = true;

        rotatedC1 = BoundingBox::CalcPositionAfterRotation(rotatedC1, slurAngle, p1);
        rotatedC2 = BoundingBox::CalcPositionAfterRotation(rotatedC2, slurAngle, p1);
        rotatedP2 = BoundingBox::CalcPositionAfterRotation(rotatedP2, slurAngle, p1);

        // The adjustedHeight value is 0 if everything fits within the slur
        // If not we need to move its position
        if (adjustedHeight != 0) {
            // The slur is being adjusted
            adjusted = true;
            // Use the adjusted control points for adjusting the position (p1, p2 and angle will be updated)
            AdjustSlurPosition(
                doc, curve, spannedElements, p1, rotatedP2, rotatedC1, rotatedC2, curveDir, slurAngle, false);
            // Re-calculate the control points with the new height
            GetControlPoints(
                doc, p1, rotatedP2, rotatedC1, rotatedC2, curveDir, adjustedHeight, staff->m_drawingStaffSize);

            points[0] = p1;
            points[1] = rotatedC1;
            points[2] = rotatedC2;
            points[3] = rotatedP2;
            curve->UpdateCurveParams(points, slurAngle, curve->GetThickness(), curveDir);
        }

        // If we still have spanning points then move the slur but now by forcing both sides to be move
        if (!spannedElements->empty()) {

            // First re-calcuate the spanning point positions
            GetSpannedPointPositions(doc, spannedElements, p1, slurAngle, curveDir, staff->m_drawingStaffSize);

            // Move it and force both sides to move
            AdjustSlurPosition(
                doc, curve, spannedElements, p1, rotatedP2, rotatedC1, rotatedC2, curveDir, slurAngle, true);
            GetControlPoints(
                doc, p1, rotatedP2, rotatedC1, rotatedC2, curveDir, adjustedHeight, staff->m_drawingStaffSize);
        }
    }

    if (adjusted) {
        points[0] = p1;
        points[1] = rotatedC1; // BoundingBox::CalcPositionAfterRotation(rotatedC1, slurAngle, *p1);
        points[2] = rotatedC2; // BoundingBox::CalcPositionAfterRotation(rotatedC2, slurAngle, *p1);
        points[3] = rotatedP2; // BoundingBox::CalcPositionAfterRotation(rotatedP2, slurAngle, *p1);
        curve->UpdateCurveParams(points, slurAngle, curve->GetThickness(), curveDir);
        // Since we are going to redraw-it reset its bounding box
        curve->BoundingBox::ResetBoundingBox();
    }

    return adjusted;
}

int Slur::AdjustSlurCurve(Doc *doc, const ArrayOfCurveSpannedElements *spannedElements, Point &p1, Point &p2, Point &c1,
    Point &c2, curvature_CURVEDIR curveDir, float angle, int staffSize, bool posRatio)
{
    Point bezier[4];
    bezier[0] = p1;
    bezier[1] = c1;
    bezier[2] = c2;
    bezier[3] = p2;

    int dist = abs(p2.x - p1.x);
    int currentHeight = abs(c1.y - p1.y);
    int maxHeight = 0;

    // 0.2 for avoiding / by 0 (below)
    float maxHeightFactor = std::max(0.2f, fabsf(angle));
    maxHeight = dist
        / (maxHeightFactor
            * (doc->GetOptions()->m_slurCurveFactor.GetValue()
                + 5)); // 5 is the minimum - can be increased for limiting curvature

    maxHeight = std::max(maxHeight, currentHeight);
    maxHeight = std::min(maxHeight, doc->GetDrawingOctaveSize(staffSize));

    maxHeight = currentHeight;
    /*
    bool hasReachedMaxHeight = false;

    if (maxHeight > currentHeight) {
        float maxRatio = 1.0;
        float posXRatio = 1.0;
        int posX;
        for (itPoint = spannedElements->begin(); itPoint != spannedElements->end();) {

            if (itPoint->second.second.x < p1->x) {
                // itPoint = spannedElements->erase(itPoint);
                ++itPoint;
                continue;
            }
            if (itPoint->second.second.x > p2->x) {
                // itPoint = spannedElements->erase(itPoint);
                ++itPoint;
                continue;
            }

            y = BoundingBox::CalcBezierAtPosition(bezier, itPoint->second.second.x);

            // Weight the desired height according to the x position if wanted
            posXRatio = 1.0;
            if (posRatio && (dist != 0)) {
                posX = itPoint->second.second.x - p1->x;
                if (posX > dist / 2) posX = p2->x - itPoint->second.second.x;
                if (dist != 0) posXRatio = (float)posX / ((float)dist / 2.0);
            }

            // Keep the maximum desired ratio
            if (curveDir == curvature_CURVEDIR_above) {
                if (y < itPoint->second.second.y) {
                    float ratio = (float)(p1->y - itPoint->second.second.y) / (float)(p1->y - y) * posXRatio;
                    maxRatio = ratio > maxRatio ? ratio : maxRatio;
                    ++itPoint;
                }
                // The point is below, we can drop it
                else {
                    itPoint = spannedElements->erase(itPoint);
                }
            }
            else {
                if (y > itPoint->second.second.y) {
                    float ratio = (float)(p1->y - itPoint->second.second.y) / (float)(p1->y - y) * posXRatio;
                    maxRatio = ratio > maxRatio ? ratio : maxRatio;
                    ++itPoint;
                }
                // the point is above, we can drop it
                else {
                    itPoint = spannedElements->erase(itPoint);
                }
            }
        }

        // We should not adjust it more than the maximum height
        int desiredHeight = currentHeight * maxRatio;
        if (desiredHeight > maxHeight) {
            hasReachedMaxHeight = true;
            maxRatio = (float)maxHeight / (float)currentHeight;
        }

        if (maxRatio > 1.0) {
            if (curveDir == curvature_CURVEDIR_above) {
                c1->y = p1->y + currentHeight * maxRatio;
                c2->y = c1->y;
            }
            else {
                c1->y = p1->y - currentHeight * maxRatio;
                c2->y = c1->y;
            }
        }
    }

    if (hasReachedMaxHeight) return maxHeight;

    // Check if we need further adjustment of the points with the curve
    bezier[1] = *c1;
    bezier[2] = *c2;
    for (itPoint = spannedElements->begin(); itPoint != spannedElements->end();) {
        y = BoundingBox::CalcBezierAtPosition(bezier, itPoint->second.second.x);
        if (curveDir == curvature_CURVEDIR_above) {
            if (y >= itPoint->second.second.y)
                itPoint = spannedElements->erase(itPoint);
            else
                ++itPoint;
        }
        else {
            if (y <= itPoint->second.second.y)
                itPoint = spannedElements->erase(itPoint);
            else
                ++itPoint;
        }
    }
    */

    if (!spannedElements->empty()) return maxHeight;

    return 0;
}

void Slur::AdjustSlurPosition(Doc *doc, FloatingCurvePositioner *curve,
    const ArrayOfCurveSpannedElements *spannedElements, Point &p1, Point &p2, Point &c1, Point &c2,
    curvature_CURVEDIR curveDir, float &angle, bool forceBothSides)
{
    Point bezier[4];
    bezier[0] = p1;
    bezier[1] = c1;
    bezier[2] = c2;
    bezier[3] = p2;

    int maxShiftLeft = 0;
    int maxShiftRight = 0;

    int dist = abs(p2.x - p1.x);
    float posXRatio = 1.0;

    int margin = 1 * doc->GetDrawingUnit(100) / 2;

    for (auto &spannedElement : *spannedElements) {

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

        int xLeft = std::max(p1.x, spannedElement->m_boundingBox->GetSelfLeft());
        int xRight = std::min(p2.x, spannedElement->m_boundingBox->GetSelfRight());
        int xMiddle = xLeft + ((xRight - xLeft) / 2);
        int posX = xMiddle - p1.x;

        // Weight the desired height according to the x position on the other side
        posXRatio = 1.0;
        bool leftPoint = true;
        if (posX > dist / 2) {
            posX = p2.x - xMiddle;
            leftPoint = false;
        }
        if (dist != 0) posXRatio = (float)posX / ((float)dist / 2.0);

        // intersection += doc->GetDrawingUnit(100);
        if (intersection > 0) {
            int leftShift = (forceBothSides || leftPoint) ? intersection : intersection * posXRatio;
            int rightShift = (forceBothSides || !leftPoint) ? intersection : intersection * posXRatio;
            // Keep the maximum shift on the left and right
            maxShiftLeft = leftShift > maxShiftLeft ? leftShift : maxShiftLeft;
            maxShiftRight = rightShift > maxShiftRight ? rightShift : maxShiftRight;
        }
    }

    // Actually nothing to do
    if (spannedElements->empty()) return;

    // Unrotated the slur
    //*p2 = BoundingBox::CalcPositionAfterRotation(*p2, (*angle), *p1);

    if (curveDir == curvature_CURVEDIR_above) {
        p1.y += maxShiftLeft;
        p2.y += maxShiftRight;
    }
    else {
        p1.y -= maxShiftLeft;
        p2.y -= maxShiftRight;
    }

    angle = GetAdjustedSlurAngle(doc, p1, p2, curveDir, true);
    //*p2 = BoundingBox::CalcPositionAfterRotation(*p2, -(*angle), *p1);
}

float Slur::GetAdjustedSlurAngle(Doc *doc, Point &p1, Point &p2, curvature_CURVEDIR curveDir, bool withPoints)
{
    float slurAngle = atan2(p2.y - p1.y, p2.x - p1.x);
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

void Slur::GetControlPoints(
    Doc *doc, Point &p1, Point &p2, Point &c1, Point &c2, curvature_CURVEDIR curveDir, int height, int staffSize)
{
    float slurAngle = atan2(p2.y - p1.y, p2.x - p1.x);
    if (slurAngle != 0.0) {
        p2 = BoundingBox::CalcPositionAfterRotation(p2, -slurAngle, p1);
        // It should not be the case but we do need to avoid recursive calls whatever the effect in the resutls
        if (p2.y != p1.y) p2.y = p1.y;
        GetControlPoints(doc, p1, p2, c1, c2, curveDir, height, staffSize);
        p2 = BoundingBox::CalcPositionAfterRotation(p2, slurAngle, p1);
        c1 = BoundingBox::CalcPositionAfterRotation(c1, slurAngle, p1);
        c2 = BoundingBox::CalcPositionAfterRotation(c2, slurAngle, p1);
        return;
    }

    // Set the x position of the control points
    int cPos = std::min(
        (p2.x - p1.x) / doc->GetOptions()->m_slurControlPoints.GetValue(), doc->GetDrawingStaffSize(staffSize));
    c1.x = p1.x + cPos;
    c2.x = p2.x - cPos;

    if (curveDir == curvature_CURVEDIR_above) {
        c1.y = p1.y + height;
        c2.y = p2.y + height;
    }
    else {
        c1.y = p1.y - height;
        c2.y = p2.y - height;
    }
}

void Slur::GetSpannedPointPositions(Doc *doc, const ArrayOfCurveSpannedElements *spannedElements, Point p1, float angle,
    curvature_CURVEDIR curveDir, int staffSize)
{
    /*
    for (auto &spannedElement : *spannedElements) {
        int margin = 1;
        // Not sure if it is better to add the margin before or after the rotation...
        // if (up) p.y += m_doc->GetDrawingUnit(staffSize) * 2;
        // else p.y -= m_doc->GetDrawingUnit(staffSize) * 2;
        itPoint->second.second = BoundingBox::CalcPositionAfterRotation(itPoint->second.first, -angle, p1);
        // This would add it after
        if (curveDir == curvature_CURVEDIR_above) {
            itPoint->second.second.y += doc->GetDrawingUnit(staffSize) * margin;
        }
        else {
            itPoint->second.second.y -= doc->GetDrawingUnit(staffSize) * margin;
        }
    }
    */
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Slur::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    ControlElement::ResetDrawing(functorParams);

    m_drawingCurvedir = curvature_CURVEDIR_NONE;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
