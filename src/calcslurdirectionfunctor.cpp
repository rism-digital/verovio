/////////////////////////////////////////////////////////////////////////////
// Name:        calcslurdirectionfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "calcslurdirectionfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "slur.h"
#include "staff.h"
#include "system.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// CalcSlurDirectionFunctor
//----------------------------------------------------------------------------

CalcSlurDirectionFunctor::CalcSlurDirectionFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode CalcSlurDirectionFunctor::VisitLayerElement(LayerElement *layerElement)
{
    return FUNCTOR_SIBLINGS;
}

FunctorCode CalcSlurDirectionFunctor::VisitSlur(Slur *slur)
{
    // If curve direction is prescribed as above or below, use it
    if (slur->HasCurvedir() && (slur->GetCurvedir() != curvature_CURVEDIR_mixed)) {
        slur->SetDrawingCurveDir(
            (slur->GetCurvedir() == curvature_CURVEDIR_above) ? SlurCurveDirection::Above : SlurCurveDirection::Below);
    }
    if (slur->HasDrawingCurveDir()) return FUNCTOR_CONTINUE;

    // Retrieve boundary
    LayerElement *start = slur->GetStart();
    LayerElement *end = slur->GetEnd();
    if (!start || !end) {
        slur->SetDrawingCurveDir(SlurCurveDirection::Above);
        return FUNCTOR_CONTINUE;
    }

    // If curve direction is prescribed as mixed, use it if boundary lies in different staves
    if (slur->GetCurvedir() == curvature_CURVEDIR_mixed) {
        if (slur->HasBulge()) {
            LogWarning("Mixed curve direction is ignored for slurs with prescribed bulge.");
        }
        else {
            const int startStaffN = start->GetAncestorStaff(RESOLVE_CROSS_STAFF)->GetN();
            const int endStaffN = end->GetAncestorStaff(RESOLVE_CROSS_STAFF)->GetN();
            if (startStaffN < endStaffN) {
                slur->SetDrawingCurveDir(SlurCurveDirection::BelowAbove);
                return FUNCTOR_CONTINUE;
            }
            else if (startStaffN > endStaffN) {
                slur->SetDrawingCurveDir(SlurCurveDirection::AboveBelow);
                return FUNCTOR_CONTINUE;
            }
            else {
                LogWarning("Mixed curve direction is ignored for slurs starting and ending on the same staff.");
            }
        }
    }

    // Retrieve staves and system
    std::vector<Staff *> staffList = slur->GetTstampStaves(slur->GetStartMeasure(), slur);
    if (staffList.empty()) {
        slur->SetDrawingCurveDir(SlurCurveDirection::Above);
        return FUNCTOR_CONTINUE;
    }
    Staff *staff = staffList.at(0);
    System *system = vrv_cast<System *>(staff->GetFirstAncestor(SYSTEM));
    assert(system);

    const bool isCrossStaff = (slur->GetBoundaryCrossStaff() != NULL);
    const bool isGraceToNoteSlur
        = !start->Is(TIMESTAMP_ATTR) && !end->Is(TIMESTAMP_ATTR) && start->IsGraceNote() && !end->IsGraceNote();

    if (!start->Is(TIMESTAMP_ATTR) && !end->Is(TIMESTAMP_ATTR) && !isGraceToNoteSlur
        && system->HasMixedDrawingStemDir(start, end)) {
        // Handle mixed stem direction
        if (isCrossStaff && (system->GetPreferredCurveDirection(start, end, slur) == curvature_CURVEDIR_below)) {
            slur->SetDrawingCurveDir(SlurCurveDirection::Below);
        }
        else {
            slur->SetDrawingCurveDir(SlurCurveDirection::Above);
        }
    }
    else {
        // Handle uniform stem direction, time stamp boundaries and grace note slurs
        StemmedDrawingInterface *startStemDrawInterface = start->GetStemmedDrawingInterface();
        data_STEMDIRECTION startStemDir = STEMDIRECTION_NONE;
        if (startStemDrawInterface) {
            startStemDir = startStemDrawInterface->GetDrawingStemDir();
        }

        const int center = staff->GetDrawingY() - m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize) / 2;
        const bool isAboveStaffCenter = (start->GetDrawingY() > center);
        if (slur->GetPreferredCurveDirection(startStemDir, isAboveStaffCenter, isGraceToNoteSlur)
            == curvature_CURVEDIR_below) {
            slur->SetDrawingCurveDir(SlurCurveDirection::Below);
        }
        else {
            slur->SetDrawingCurveDir(SlurCurveDirection::Above);
        }
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
