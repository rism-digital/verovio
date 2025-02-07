/////////////////////////////////////////////////////////////////////////////
// Name:        view_slur.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <iostream>
#include <sstream>

//----------------------------------------------------------------------------

#include "bboxdevicecontext.h"
#include "doc.h"
#include "layer.h"
#include "layerelement.h"
#include "note.h"
#include "options.h"
#include "slur.h"
#include "staff.h"

namespace vrv {

//----------------------------------------------------------------------------
// View - Slur
//----------------------------------------------------------------------------

void View::DrawSlur(DeviceContext *dc, Slur *slur, int x1, int x2, Staff *staff, char spanningType, Object *graphic)
{
    assert(dc);
    assert(slur);
    assert(staff);

    FloatingCurvePositioner *curve = this->CalcInitialSlur(dc, slur, x1, x2, staff, spanningType);
    Point points[4];
    curve->GetPoints(points);

    if (graphic) {
        dc->ResumeGraphic(graphic, graphic->GetID());
    }
    else {
        dc->StartGraphic(slur, "", slur->GetID(), SPANNING);
    }

    PenStyle penStyle = PEN_SOLID;
    switch (slur->GetLform()) {
        case LINEFORM_dashed: penStyle = PEN_SHORT_DASH; break;
        case LINEFORM_dotted: penStyle = PEN_DOT; break;
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

    if (graphic) {
        dc->EndResumedGraphic(graphic, this);
    }
    else {
        dc->EndGraphic(slur, this);
    }
}

FloatingCurvePositioner *View::CalcInitialSlur(
    DeviceContext *dc, Slur *slur, int x1, int x2, Staff *staff, char spanningType)
{
    FloatingPositioner *positioner = slur->GetCurrentFloatingPositioner();
    assert(positioner && positioner->Is(FLOATING_CURVE_POSITIONER));
    FloatingCurvePositioner *curve = vrv_cast<FloatingCurvePositioner *>(positioner);
    assert(curve);

    if ((this->GetSlurHandling() == SlurHandling::Initialize) && dc->Is(BBOX_DEVICE_CONTEXT)
        && (curve->GetDir() == curvature_CURVEDIR_NONE || curve->IsCrossStaff())) {
        // Initial curve calculation
        curve->SetCachedX12({ x1, x2 });
        slur->CalcInitialCurve(m_doc, curve);

        // Register content
        slur->CalcSpannedElements(curve);
        slur->AddPositionerToArticulations(curve);
    }
    return curve;
}

} // namespace vrv
