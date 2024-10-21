/////////////////////////////////////////////////////////////////////////////
// Name:        calcslurdirectionfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "calcslurdirectionfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "layer.h"
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
        else if (start->Is(TIMESTAMP_ATTR) || end->Is(TIMESTAMP_ATTR)) {
            LogWarning("Mixed curve direction is ignored for slurs with tstamp boundary.");
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
        if (this->GetPreferredCurveDirection(slur, startStemDir, isAboveStaffCenter, isGraceToNoteSlur)
            == curvature_CURVEDIR_below) {
            slur->SetDrawingCurveDir(SlurCurveDirection::Below);
        }
        else {
            slur->SetDrawingCurveDir(SlurCurveDirection::Above);
        }
    }

    return FUNCTOR_CONTINUE;
}

curvature_CURVEDIR CalcSlurDirectionFunctor::GetGraceCurveDirection(const Slur *slur) const
{
    // Start on the notehead side
    const LayerElement *start = slur->GetStart();
    const StemmedDrawingInterface *startStemDrawInterface = start->GetStemmedDrawingInterface();
    const bool isStemDown
        = startStemDrawInterface && (startStemDrawInterface->GetDrawingStemDir() == STEMDIRECTION_down);
    return isStemDown ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
}

curvature_CURVEDIR CalcSlurDirectionFunctor::GetPreferredCurveDirection(
    const Slur *slur, data_STEMDIRECTION noteStemDir, bool isAboveStaffCenter, bool isGraceToNoteSlur) const
{
    const Note *startNote = NULL;
    const Chord *startParentChord = NULL;
    if (slur->GetStart()->Is(NOTE)) {
        startNote = vrv_cast<const Note *>(slur->GetStart());
        assert(startNote);
        startParentChord = startNote->IsChordTone();
    }

    const Layer *layer = NULL;
    const LayerElement *layerElement = NULL;
    std::tie(layer, layerElement) = slur->GetBoundaryLayer();
    data_STEMDIRECTION layerStemDir = STEMDIRECTION_NONE;

    curvature_CURVEDIR drawingCurveDir = curvature_CURVEDIR_above;
    // first should be the slur @curvedir
    if (slur->HasCurvedir()) {
        drawingCurveDir
            = (slur->GetCurvedir() == curvature_CURVEDIR_above) ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
    }
    // grace note slurs in case we have no drawing stem direction on the layer
    else if (isGraceToNoteSlur && layer && layerElement
        && (layer->GetDrawingStemDir(layerElement) == STEMDIRECTION_NONE)) {
        drawingCurveDir = this->GetGraceCurveDirection(slur);
    }
    // otherwise layer direction trumps note direction
    else if (layer && layerElement && ((layerStemDir = layer->GetDrawingStemDir(layerElement)) != STEMDIRECTION_NONE)) {
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
        drawingCurveDir = isAboveStaffCenter ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
    }

    return drawingCurveDir;
}

} // namespace vrv
