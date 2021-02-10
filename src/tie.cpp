/////////////////////////////////////////////////////////////////////////////
// Name:        tie.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tie.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "chord.h"
#include "comparison.h"
#include "doc.h"
#include "elementpart.h"
#include "functorparams.h"
#include "layer.h"
#include "note.h"
#include "slur.h"
#include "staff.h"

namespace vrv {

//----------------------------------------------------------------------------
// Tie
//----------------------------------------------------------------------------

Tie::Tie() : ControlElement("tie-"), TimeSpanningInterface(), AttColor(), AttCurvature(), AttCurveRend()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_CURVATURE);
    RegisterAttClass(ATT_CURVEREND);

    Reset();
}

Tie::~Tie() {}

void Tie::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    ResetColor();
    ResetCurvature();
    ResetCurveRend();
}

bool Tie::CalculatePosition(Doc *doc, Staff *staff, int x1, int x2, int spanningType, Point bezier[4])
{
    const int drawingUnit = doc->GetDrawingUnit(staff->m_drawingStaffSize);
    /************** parent layers **************/

    if ((spanningType != SPANNING_START_END) && (spanningType != SPANNING_START) && (spanningType != SPANNING_END)) {
        //  this make no sense 
        LogDebug("Tie across an entire system is not supported");
        return false;
    }

    Note *note1 = dynamic_cast<Note *>(GetStart());
    Note *note2 = dynamic_cast<Note *>(GetEnd());

    if (!note1 && !note2) {
        // no note, obviously nothing to do...
        // this also means that notes with tstamp events are not supported
        return false;
    }

    bool isOuterChordNote = false;
    LayerElement *durElement = NULL;
    Chord *startParentChord = NULL;
    Layer *layer1 = NULL;
    if (note1) {
        durElement = note1;
        layer1 = note1->m_crossStaff ? note1->m_crossLayer : dynamic_cast<Layer *>(note1->GetFirstAncestor(LAYER));
        startParentChord = note1->IsChordTone();
    }
    if (startParentChord) {
        durElement = startParentChord;
        if (startParentChord->m_crossStaff) layer1 = startParentChord->m_crossLayer;
        if ((note1 == startParentChord->GetTopNote()) || (note1 == startParentChord->GetBottomNote()))
            isOuterChordNote = true;
    }

    /************** x positions **************/

    Point startPoint(x1, staff->GetDrawingY());
    Point endPoint(x2, staff->GetDrawingY());
    data_STEMDIRECTION noteStemDir
        = CalculateXPosition(doc, staff, startParentChord, spanningType, isOuterChordNote, startPoint, endPoint);

    /************** direction **************/

    bool isAboveStaffCenter = startPoint.y > (staff->GetDrawingY() - 4 * drawingUnit);
    curvature_CURVEDIR drawingCurveDir = GetPreferredCurveDirection(
        layer1, note1, startParentChord, noteStemDir, isAboveStaffCenter);

    /************** y position **************/

    bool isShortTie = false;
    // shortTie correction cannot be applied for chords
    if (!startParentChord && (endPoint.x - startPoint.x < 6 * drawingUnit)) {
        isShortTie = true;
    }
    if (drawingCurveDir == curvature_CURVEDIR_above) {
        startPoint.y += drawingUnit / 2;
        endPoint.y += drawingUnit / 2;
        if (isShortTie) {
            startPoint.y += drawingUnit;
            endPoint.y += drawingUnit;
        }
    }
    else {
        startPoint.y -= drawingUnit / 2;
        endPoint.y -= drawingUnit / 2;
        if (isShortTie) {
            startPoint.y -= drawingUnit;
            endPoint.y -= drawingUnit;
        }
    }

    /************** bezier points **************/

    // the 'height' of the bezier
    int height = drawingUnit;
    // if the space between the to points is more than two staff height, increase the height
    if (endPoint.x - startPoint.x > 2 * doc->GetDrawingStaffSize(staff->m_drawingStaffSize)) {
        height += drawingUnit;
    }

    // control points
    Point c1, c2;
    // the height of the control points
    height *= 4 / 3;

    c1.x = startPoint.x + (endPoint.x - startPoint.x) / 4; // point at 1/4
    c2.x = startPoint.x + (endPoint.x - startPoint.x) / 4 * 3; // point at 3/4

    if (drawingCurveDir == curvature_CURVEDIR_above) {
        c1.y = startPoint.y + height;
        c2.y = endPoint.y + height;
    }
    else {
        c1.y = startPoint.y - height;
        c2.y = endPoint.y - height;
    }

    //Point bezier[4];
    bezier[0] = startPoint;
    bezier[1] = c1;
    bezier[2] = c2;
    bezier[3] = endPoint;

    assert(GetCurrentFloatingPositioner());
    FloatingPositioner *positioner = GetCurrentFloatingPositioner();
    assert(positioner && positioner->Is(FLOATING_CURVE_POSITIONER));
    FloatingCurvePositioner *curve = vrv_cast<FloatingCurvePositioner *>(positioner);
    assert(curve);

    const int thickness = drawingUnit * doc->GetOptions()->m_tieMidpointThickness.GetValue();
    curve->UpdateCurveParams(bezier, 0.0, thickness, drawingCurveDir);

    if ((!startParentChord || isOuterChordNote) && durElement) {
        UpdateTiePositioning(curve, bezier, durElement, thickness, height, drawingCurveDir);
    }
    
    return true;
}

data_STEMDIRECTION Tie::CalculateXPosition(Doc *doc, Staff *staff, Chord *startParentChord, int spanningType,
    bool isOuterChordNote, Point &startPoint, Point &endPoint)
{
    Note *startNote = vrv_cast<Note *>(GetStart());
    Note *endNote = vrv_cast<Note *>(GetEnd());

    const int drawingUnit = doc->GetDrawingUnit(staff->m_drawingStaffSize);
    bool isShortTie = false;
    // shortTie correction cannot be applied for chords
    if (!startParentChord && (endPoint.x - startPoint.x < 6 * drawingUnit)) {
        isShortTie = true;
    }

    int r1 = drawingUnit;
    int r2 = r1;
    data_STEMDIRECTION noteStemDir = STEMDIRECTION_NONE;
    // the normal case
    if (spanningType == SPANNING_START_END) {
        if (startNote) {
            startPoint.y = startNote->GetDrawingY();
            endPoint.y = startPoint.y;
            noteStemDir = startNote->GetDrawingStemDir();
        }
        else if (endNote) {
            endPoint.y = endNote->GetDrawingY();
            startPoint.y = endPoint.y;
        }
        // isShort is never true with tstamp1
        if (!isShortTie) {
            if (startNote) r1 = startNote->GetDrawingRadius(doc);
            if (endNote) r2 = endNote->GetDrawingRadius(doc);
            startPoint.x += r1 + drawingUnit / 2;
            endPoint.x -= r2 + drawingUnit / 2;
        }
        if (startParentChord && !isOuterChordNote && startParentChord->HasDots()) {
            Dots *dots = vrv_cast<Dots *>(startParentChord->FindDescendantByType(DOTS));
            startPoint.x = dots->GetDrawingX() + (1 + startParentChord->GetDots()) * drawingUnit;
        }
    }
    // This is the case when the tie is split over two system of two pages.
    // In this case, we are now drawing its beginning to the end of the measure (i.e., the last aligner)
    else if (spanningType == SPANNING_START) {
        if (startNote) {
            startPoint.y = startNote->GetDrawingY();
            endPoint.y = startPoint.y;
            r1 = startNote->GetDrawingRadius(doc);
            noteStemDir = startNote->GetDrawingStemDir();
        }
        if (!isShortTie) {
            startPoint.x += r1 + drawingUnit / 2;
            if (startNote && startNote->GetDots() > 0) {
                startPoint.x += drawingUnit * startNote->GetDots() * 3 / 2;
            }
            else if (startParentChord && (startParentChord->GetDots() > 0)) {
                startPoint.x += 2 * drawingUnit * startParentChord->GetDots();
            }
        }
        if (startParentChord && !isOuterChordNote && startParentChord->HasDots()) {
            Dots *dots = vrv_cast<Dots *>(startParentChord->FindDescendantByType(DOTS));
            startPoint.x = dots->GetDrawingX() + (1 + startParentChord->GetDots()) * drawingUnit;
        }
        endPoint.x -= (drawingUnit + doc->GetDrawingBarLineWidth(staff->m_drawingStaffSize)) / 2;
    }
    // Now this is the case when the tie is split but we are drawing the end of it
    else if (spanningType == SPANNING_END) {
        if (endNote) {
            endPoint.y = endNote->GetDrawingY();
            startPoint.y = endPoint.y;
            r2 = endNote->GetDrawingRadius(doc);
            noteStemDir = endNote->GetDrawingStemDir();
        }
        if (!isShortTie) {
            endPoint.x -= r2 + drawingUnit / 2;
        }
    }

    return noteStemDir;
}

curvature_CURVEDIR Tie::GetPreferredCurveDirection(Layer *layer, Note *note,
    Chord *startParentChord, data_STEMDIRECTION noteStemDir, bool isAboveStaffCenter)
{
    data_STEMDIRECTION layerStemDir;
    curvature_CURVEDIR drawingCurveDir = curvature_CURVEDIR_above;
    // first should be the tie @curvedir
    if (HasCurvedir()) {
        drawingCurveDir
            = (GetCurvedir() == curvature_CURVEDIR_above) ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
    }
    // then layer direction trumps note direction
    else if (layer && ((layerStemDir = layer->GetDrawingStemDir(note)) != STEMDIRECTION_NONE)) {
        drawingCurveDir = (layerStemDir == STEMDIRECTION_up) ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
    }
    // look if in a chord
    else if (startParentChord) {
        if (startParentChord->PositionInChord(note) < 0) {
            drawingCurveDir = curvature_CURVEDIR_below;
        }
        else if (startParentChord->PositionInChord(note) > 0) {
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

void Tie::UpdateTiePositioning(FloatingCurvePositioner *curve, Point bezier[4], LayerElement *durElement, int thickness,
    int height, curvature_CURVEDIR drawingCurveDir)
{
    ListOfObjects objects;
    ClassIdsComparison cmp({ DOT, DOTS, FLAG });
    durElement->FindAllDescendantByComparison(&objects, &cmp);

    for (auto object : objects) {
        // if we have possible overlap with dots, we need to move tie up/down to avoid it. This happens only for the
        // outter ties, so there should be no issue of inner tie moving up and colliding with other elements
        if (object->Is(DOTS)) {
            bool discard = false;
            int intersection = curve->CalcAdjustment(object, discard);
            if (intersection == 0) continue;
            if (std::abs(intersection) > height) intersection = intersection / std::abs(intersection) * height;

            intersection *= (drawingCurveDir == curvature_CURVEDIR_above) ? 1 : -1;
            for (int i = 0; i < 4; ++i) {
                bezier[i].y += intersection;
            }
        }
        // In case there is overlap with flag, we need to move starting point to the side, to avoid it
        else if (object->Is(FLAG)) {
            bool discard = false;
            int intersection = curve->CalcAdjustment(object, discard, 0);
            if (intersection != 0) {
                bezier[0].x += height;
                bezier[1].x += height / 2;
                bezier[2].x = bezier[3].x - height / 2;
            }
        }

        curve->UpdateCurveParams(bezier, 0.0, thickness, drawingCurveDir);
    }
}
//----------------------------------------------------------------------------
// Tie functor methods
//----------------------------------------------------------------------------

int Tie::ResolveMIDITies(FunctorParams *)
{
    Note *note1 = dynamic_cast<Note *>(this->GetStart());
    Note *note2 = dynamic_cast<Note *>(this->GetEnd());

    if (!note1 || !note2) {
        return FUNCTOR_CONTINUE;
    }

    double sttd2 = note2->GetScoreTimeTiedDuration();
    double std2 = note2->GetScoreTimeDuration();

    if (sttd2 > 0.0) {
        note1->SetScoreTimeTiedDuration(sttd2 + std2);
    }
    else {
        note1->SetScoreTimeTiedDuration(std2);
    }
    note2->SetScoreTimeTiedDuration(-1.0);

    return FUNCTOR_SIBLINGS;
}

int Tie::FindSpannedLayerElements(FunctorParams *functorParams)
{
    FindSpannedLayerElementsParams *params = vrv_params_cast<FindSpannedLayerElementsParams *>(functorParams);
    assert(params);

    FloatingPositioner *positioner = params->m_slur->GetCorrespFloatingPositioner(this);

    if (!positioner) {
        return FUNCTOR_CONTINUE;
    }

    if (positioner->HasContentBB() && (positioner->GetContentRight() > params->m_minPos)
        && (positioner->GetContentLeft() < params->m_maxPos)) {

        params->m_ties.push_back(positioner);
    }

    return FUNCTOR_CONTINUE;

    /*
    if (!this->Is(params->m_classIds)) {
        return FUNCTOR_CONTINUE;
    }

    if (this->HasContentBB() && (this->GetContentRight() > params->m_minPos)
        && (this->GetContentLeft() < params->m_maxPos)) {

        // We skip the start or end of the slur
        if ((this == params->m_interface->GetStart()) || (this == params->m_interface->GetEnd())) {
            return FUNCTOR_CONTINUE;
        }
        if (params->m_interface->GetStart()->HasChild(this) || this->HasChild(params->m_interface->GetStart())) {
            return FUNCTOR_CONTINUE;
        }
        if (params->m_interface->GetEnd()->HasChild(this) || this->HasChild(params->m_interface->GetEnd())) {
            return FUNCTOR_CONTINUE;
        }

        params->m_elements.push_back(this);
    }
    else if (this->GetDrawingX() > params->m_maxPos) {
        return FUNCTOR_STOP;
    }

    return FUNCTOR_CONTINUE;
    */
}

} // namespace vrv
