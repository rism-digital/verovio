/////////////////////////////////////////////////////////////////////////////
// Name:        view_slur.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <iostream>
#include <sstream>

//----------------------------------------------------------------------------

#include "bboxdevicecontext.h"
#include "comparison.h"
#include "devicecontext.h"
#include "doc.h"
#include "ftrem.h"
#include "functorparams.h"
#include "layer.h"
#include "layerelement.h"
#include "measure.h"
#include "note.h"
#include "options.h"
#include "slur.h"
#include "staff.h"
#include "system.h"
#include "timeinterface.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// View - Slur
//----------------------------------------------------------------------------

void View::DrawSlur(DeviceContext *dc, Slur *slur, int x1, int x2, Staff *staff, char spanningType, Object *graphic)
{
    assert(dc);
    assert(slur);
    assert(staff);

    FloatingPositioner *positioner = slur->GetCurrentFloatingPositioner();
    assert(positioner && positioner->Is(FLOATING_CURVE_POSITIONER));
    FloatingCurvePositioner *curve = vrv_cast<FloatingCurvePositioner *>(positioner);
    assert(curve);

    if ((this->GetSlurHandling() == SlurHandling::Initialize) && dc->Is(BBOX_DEVICE_CONTEXT)
        && (curve->GetDir() == curvature_CURVEDIR_NONE || curve->IsCrossStaff())) {
        this->DrawSlurInitial(curve, slur, x1, x2, staff, spanningType);
    }

    Point points[4];
    curve->GetPoints(points);

    if (graphic)
        dc->ResumeGraphic(graphic, graphic->GetUuid());
    else
        dc->StartGraphic(slur, "", slur->GetUuid(), false);

    int penStyle = AxSOLID;
    switch (slur->GetLform()) {
        case LINEFORM_dashed: penStyle = AxSHORT_DASH; break;
        case LINEFORM_dotted: penStyle = AxDOT; break;
        case LINEFORM_wavy:
        // TODO: Implement wavy slur.
        default: break;
    }
    const int penWidth
        = m_doc->GetOptions()->m_slurEndpointThickness.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    const double thicknessCoefficient
        = BoundingBox::GetBezierThicknessCoefficient(points, curve->GetThickness(), penWidth);
    this->DrawThickBezierCurve(
        dc, points, thicknessCoefficient * curve->GetThickness(), staff->m_drawingStaffSize, penWidth, penStyle);

    /*
    int i;
    for (i = 0; i <= 10; ++i) {
        Point p = BoundingBox::CalcDeCasteljau(points, (double)i / 10.0);
        this->DrawDot(dc, p.x, p.y, staff->m_drawingStaffSize);
    }
    */

    if (graphic)
        dc->EndResumedGraphic(graphic, this);
    else
        dc->EndGraphic(slur, this);
}

void View::DrawSlurInitial(FloatingCurvePositioner *curve, Slur *slur, int x1, int x2, Staff *staff, char spanningType)
{
    LayerElement *start = slur->GetStart();
    LayerElement *end = slur->GetEnd();

    if (!start || !end) return;

    const curvature_CURVEDIR drawingCurveDir = slur->CalcDrawingCurveDir(spanningType);

    /************** adjusting y position **************/

    int y1 = staff->GetDrawingY();
    int y2 = staff->GetDrawingY();
    std::pair<Point, Point> adjustedPoints
        = slur->AdjustCoordinates(m_doc, staff, std::make_pair(Point(x1, y1), Point(x2, y2)), spanningType);

    /************** y position **************/

    int sign = (drawingCurveDir == curvature_CURVEDIR_above) ? 1 : -1;
    if (drawingCurveDir == curvature_CURVEDIR_mixed) {
        sign = slur->HasEndpointAboveStart() ? 1 : -1;
    }
    adjustedPoints.first.y += 1.25 * sign * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);

    if (drawingCurveDir == curvature_CURVEDIR_mixed) {
        sign = slur->HasEndpointAboveEnd() ? 1 : -1;
    }
    adjustedPoints.second.y += 1.25 * sign * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);

    Point points[4];
    points[0] = adjustedPoints.first;
    points[3] = adjustedPoints.second;

    this->CalcInitialSlur(curve, slur, staff, drawingCurveDir, points);
    int thickness = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * m_options->m_slurMidpointThickness.GetValue();

    curve->UpdateCurveParams(points, thickness, drawingCurveDir);

    /************** articulation **************/

    // First get all artic children
    ClassIdComparison matchType(ARTIC);
    ListOfObjects artics;

    // the normal case or start
    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START)) {
        start->FindAllDescendantsByComparison(&artics, &matchType);
        // Then the @n of each first staffDef
        for (auto &object : artics) {
            Artic *artic = vrv_cast<Artic *>(object);
            assert(artic);
            if (artic->IsOutsideArtic()) {
                if ((artic->GetPlace() == STAFFREL_above) && (drawingCurveDir == curvature_CURVEDIR_above)) {
                    artic->AddSlurPositioner(curve, true);
                }
                else if ((artic->GetPlace() == STAFFREL_below) && (drawingCurveDir == curvature_CURVEDIR_below)) {
                    artic->AddSlurPositioner(curve, true);
                }
            }
        }
    }
    // normal case or end
    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_END)) {
        end->FindAllDescendantsByComparison(&artics, &matchType);
        // Then the @n of each first staffDef
        for (auto &object : artics) {
            Artic *artic = vrv_cast<Artic *>(object);
            assert(artic);
            if (artic->IsOutsideArtic()) {
                if ((artic->GetPlace() == STAFFREL_above) && (drawingCurveDir == curvature_CURVEDIR_above)) {
                    artic->AddSlurPositioner(curve, false);
                }
                else if ((artic->GetPlace() == STAFFREL_below) && (drawingCurveDir == curvature_CURVEDIR_below)) {
                    artic->AddSlurPositioner(curve, false);
                }
            }
        }
    }
}

void View::CalcInitialSlur(
    FloatingCurvePositioner *curve, Slur *slur, Staff *staff, curvature_CURVEDIR curveDir, Point points[4])
{
    // For now we pick C1 = P1 and C2 = P2
    BezierCurve bezier(points[0], points[0], points[3], points[3]);
    slur->InitBezierControlSides(bezier, curveDir);

    /************** content **************/

    const SpannedElements spannedElements = slur->CollectSpannedElements(staff, bezier.p1.x, bezier.p2.x);
    slur->AddSpannedElements(curve, spannedElements, staff, bezier.p1.x, bezier.p2.x);

    /************** angle **************/

    const bool dontAdjustAngle = curve->IsCrossStaff() || slur->GetStart()->IsGraceNote();
    const float nonAdjustedAngle
        = (bezier.p2 == bezier.p1) ? 0 : atan2(bezier.p2.y - bezier.p1.y, bezier.p2.x - bezier.p1.x);
    const float slurAngle
        = dontAdjustAngle ? nonAdjustedAngle : slur->GetAdjustedSlurAngle(m_doc, bezier.p1, bezier.p2, curveDir);
    if (curveDir != curvature_CURVEDIR_mixed) {
        bezier.p2 = BoundingBox::CalcPositionAfterRotation(bezier.p2, -slurAngle, bezier.p1);
    }

    /************** control points **************/

    if (slur->HasBulge()) {
        bezier.CalcInitialControlPointParams();
    }
    else {
        bezier.CalcInitialControlPointParams(m_doc, slurAngle, staff->m_drawingStaffSize);
    }
    bezier.UpdateControlPoints();
    if (curveDir != curvature_CURVEDIR_mixed) {
        bezier.Rotate(slurAngle, bezier.p1);
    }

    points[0] = bezier.p1;
    points[1] = bezier.c1;
    points[2] = bezier.c2;
    points[3] = bezier.p2;
}

} // namespace vrv
