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

#include "attcomparison.h"
#include "bboxdevicecontext.h"
#include "devicecontext.h"
#include "doc.h"
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

    Beam *parentBeam = NULL;
    Chord *startParentChord = NULL;
    Chord *endParentChord = NULL;
    Note *startNote = NULL;
    Note *endNote = NULL;
    Chord *startChord = NULL;
    Chord *endChord = NULL;

    curvature_CURVEDIR drawingCurveDir = curvature_CURVEDIR_above;
    data_STEMDIRECTION startStemDir = STEMDIRECTION_NONE;
    data_STEMDIRECTION endStemDir = STEMDIRECTION_NONE;
    data_STEMDIRECTION stemDir = STEMDIRECTION_NONE;
    bool isGraceToNoteSlur = false;
    int y1 = staff->GetDrawingY();
    int y2 = staff->GetDrawingY();

    /************** parent layers **************/

    LayerElement *start = dynamic_cast<LayerElement *>(slur->GetStart());
    LayerElement *end = dynamic_cast<LayerElement *>(slur->GetEnd());

    if (!start || !end) {
        // no start and end, obviously nothing to do...
        return;
    }

    if (start->Is(TIMESTAMP_ATTR) || end->Is(TIMESTAMP_ATTR)) {
        // for now ignore slur using tstamps
        return;
    }

    if (start->Is(NOTE)) {
        startNote = dynamic_cast<Note *>(start);
        assert(startNote);
        startParentChord = startNote->IsChordTone();
        startStemDir = startNote->GetDrawingStemDir();
    }
    else if (start->Is(CHORD)) {
        startChord = dynamic_cast<Chord *>(start);
        assert(startChord);
        startStemDir = startChord->GetDrawingStemDir();
    }
    if (end->Is(NOTE)) {
        endNote = dynamic_cast<Note *>(end);
        assert(endNote);
        endParentChord = endNote->IsChordTone();
        endStemDir = endNote->GetDrawingStemDir();
    }
    else if (end->Is(CHORD)) {
        endChord = dynamic_cast<Chord *>(end);
        assert(endChord);
        endStemDir = endChord->GetDrawingStemDir();
    }

    if (startNote && endNote && startNote->IsGraceNote() && !endNote->IsGraceNote()) {
        isGraceToNoteSlur = true;
    }

    // For now, with timestamps, get the first layer. We should eventually look at the @layerident (not implemented)
    // if (start->Is(TIMESTAMP_ATTR))
    //    layer1 = dynamic_cast<Layer *>(staff->FindChildByType(LAYER));
    // else
    Layer *layer1 = dynamic_cast<Layer *>(start->GetFirstParent(LAYER));

    // idem
    // if (end->Is(TIMESTAMP_ATTR))
    //    layer2 = dynamic_cast<Layer *>(staff->FindChildByType(LAYER));
    // else
    Layer *layer2 = dynamic_cast<Layer *>(end->GetFirstParent(LAYER));

    assert(layer1 && layer2);

    if (layer1->GetN() != layer2->GetN()) {
        LogWarning("Slurs between different layers may not be fully supported.");
    }
    else if (!start->Is(TIMESTAMP_ATTR) && !end->Is(TIMESTAMP_ATTR) && (spanningType == SPANNING_START_END)) {
        System *system = dynamic_cast<System *>(staff->GetFirstParent(SYSTEM));
        assert(system);
        // If we have a start to end situation, then store the curvedir in the slur for mixed drawing stem dir
        // situations
        if (system->HasMixedDrawingStemDir(start, end)) {
            slur->SetDrawingCurvedir(curvature_CURVEDIR_above);
        }
    }

    /************** calculate the radius for adjusting the x position **************/

    int startRadius = 0;
    if (!start->Is(TIMESTAMP_ATTR)) {
        startRadius = start->GetDrawingRadius(m_doc);
    }

    int endRadius = 0;
    if (!end->Is(TIMESTAMP_ATTR)) {
        endRadius = end->GetDrawingRadius(m_doc);
    }

    /************** note stem dir **************/

    if (spanningType == SPANNING_START_END) {
        stemDir = startStemDir;
    }
    // This is the case when the tie is split over two system of two pages.
    // In this case, we are now drawing its beginning to the end of the measure (i.e., the last aligner)
    else if (spanningType == SPANNING_START) {
        stemDir = startStemDir;
    }
    // Now this is the case when the tie is split but we are drawing the end of it
    else if (spanningType == SPANNING_END) {
        stemDir = endStemDir;
    }
    // Finally, slur accross an entire system; use the staff position and up (see below)
    else {
        stemDir = STEMDIRECTION_down;
    }

    /************** direction **************/

    data_STEMDIRECTION layerStemDir;

    // first should be the tie @curvedir
    if (slur->HasCurvedir()) {
        drawingCurveDir
            = (slur->GetCurvedir() == curvature_CURVEDIR_above) ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
    }
    // grace notes - always below unless we have a drawing stem direction on the layer
    else if (isGraceToNoteSlur && (layer1->GetDrawingStemDir(startNote) == STEMDIRECTION_NONE)) {
        drawingCurveDir = curvature_CURVEDIR_below;
    }
    // the normal case
    else if (slur->HasDrawingCurvedir()) {
        drawingCurveDir = slur->GetDrawingCurvedir();
    }
    // then layer direction trumps note direction
    else if (layer1 && ((layerStemDir = layer1->GetDrawingStemDir(start)) != STEMDIRECTION_NONE)) {
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
            drawingCurveDir = (stemDir != STEMDIRECTION_up) ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
        }
    }
    else if (stemDir == STEMDIRECTION_up) {
        drawingCurveDir = curvature_CURVEDIR_below;
    }
    else if (stemDir == STEMDIRECTION_NONE) {
        // no information from the note stem directions, look at the position in the notes
        int center = staff->GetDrawingY() - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * 2;
        drawingCurveDir = (start->GetDrawingY() > center) ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
    }

    /************** adjusting y position **************/

    bool isShortSlur = false;
    if (x2 - x1 < 1 * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize)) isShortSlur = true;

    int yChordMax, yChordMin;
    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START)) {
        // first get the min max of the chord (if any)
        if (startParentChord) {
            startParentChord->GetYExtremes(yChordMax, yChordMin);
        }
        else if (startChord) {
            startChord->GetYExtremes(yChordMax, yChordMin);
        }
        // slur is up
        if (drawingCurveDir == curvature_CURVEDIR_above) {
            // P(^)
            if (startStemDir == STEMDIRECTION_down) y1 = start->GetDrawingTop(m_doc, staff->m_drawingStaffSize);
            //  d(^)d
            else if (isShortSlur) {
                y1 = start->GetDrawingTop(m_doc, staff->m_drawingStaffSize);
            }
            // same but in beam - adjust the x too
            else if ((parentBeam = start->IsInBeam()) && !parentBeam->IsLastInBeam(start)) {
                y1 = start->GetDrawingTop(m_doc, staff->m_drawingStaffSize);
                x1 += startRadius - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            }
            // d(^)
            else {
                // put it on the side, move it left, but not if we have a @stamp
                if (!start->Is(TIMESTAMP_ATTR)) x1 += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 4 / 2;
                if (startChord || startParentChord)
                    y1 = yChordMin + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 3;
                else
                    y1 = start->GetDrawingY() + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 3;
            }
        }
        // slur is down
        else {
            // d(_)
            if (startStemDir == STEMDIRECTION_up) y1 = start->GetDrawingBottom(m_doc, staff->m_drawingStaffSize);
            // P(_)P
            else if (isShortSlur) {
                y1 = start->GetDrawingBottom(m_doc, staff->m_drawingStaffSize);
            }
            // same but in beam
            else if ((parentBeam = start->IsInBeam()) && !parentBeam->IsLastInBeam(start)) {
                y1 = start->GetDrawingBottom(m_doc, staff->m_drawingStaffSize);
                x1 -= startRadius - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            }
            // P(_)
            else {
                // put it on the side, but no need to move it left
                if (startChord || startParentChord)
                    y1 = yChordMin - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 3;
                else
                    y1 = start->GetDrawingY() - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 3;
            }
        }
    }
    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_END)) {
        // get the min max of the chord if any
        if (endParentChord) {
            endParentChord->GetYExtremes(yChordMax, yChordMin);
        }
        else if (endChord) {
            endChord->GetYExtremes(yChordMax, yChordMin);
        }
        // get the stem direction of the end
        // slur is up
        if (drawingCurveDir == curvature_CURVEDIR_above) {
            // (^)P
            if (endStemDir == STEMDIRECTION_down) y2 = end->GetDrawingTop(m_doc, staff->m_drawingStaffSize);
            // d(^)d
            else if (isShortSlur) {
                y2 = end->GetDrawingTop(m_doc, staff->m_drawingStaffSize);
            }
            // same but in beam - adjust the x too
            else if ((parentBeam = end->IsInBeam()) && !parentBeam->IsFirstInBeam(end)) {
                y2 = end->GetDrawingTop(m_doc, staff->m_drawingStaffSize);
                x2 += endRadius - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            }
            // (^)d
            else {
                // put it on the side, no need to move it right
                if (endChord || endParentChord)
                    y2 = yChordMin + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 3;
                else
                    y2 = end->GetDrawingY() + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 3;
            }
        }
        else {
            if (isGraceToNoteSlur) {
                if (endNote) {
                    y2 = endNote->GetDrawingY();
                    x2 -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2;
                    isShortSlur = true;
                }
                else {
                    y2 = y1;
                }
            }
            // (_)d
            else if (endStemDir == STEMDIRECTION_up)
                y2 = end->GetDrawingBottom(m_doc, staff->m_drawingStaffSize);
            // P(_)P
            else if (isShortSlur) {
                y2 = end->GetDrawingBottom(m_doc, staff->m_drawingStaffSize);
            }
            // same but in beam
            else if ((parentBeam = end->IsInBeam()) && !parentBeam->IsFirstInBeam(end)) {
                y2 = end->GetDrawingBottom(m_doc, staff->m_drawingStaffSize);
                //
                x2 -= endRadius - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            }
            // (_)P
            else {
                // put it on the side, move it right, but not if we have a @stamp2
                if (!end->Is(TIMESTAMP_ATTR)) x2 -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2;
                if (endChord || endParentChord)
                    y2 = yChordMin - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 3;
                else
                    y2 = end->GetDrawingY() - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 3;
            }
        }
    }

    // Positions not attached to a note
    if (spanningType == SPANNING_START) {
        if (drawingCurveDir == curvature_CURVEDIR_above)
            y2 = std::max(staff->GetDrawingY(), y1);
        else
            y2 = std::min(staff->GetDrawingY() - m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize), y1);
    }
    if (end->Is(TIMESTAMP_ATTR)) {
        if (drawingCurveDir == curvature_CURVEDIR_above)
            y2 = std::max(staff->GetDrawingY(), y1);
        else
            y2 = std::min(staff->GetDrawingY() - m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize), y1);
    }
    if (spanningType == SPANNING_END) {
        if (drawingCurveDir == curvature_CURVEDIR_above)
            y1 = std::max(staff->GetDrawingY(), y2);
        else
            y1 = std::min(staff->GetDrawingY() - m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize), y2);
    }
    if (start->Is(TIMESTAMP_ATTR)) {
        if (drawingCurveDir == curvature_CURVEDIR_above)
            y1 = std::max(staff->GetDrawingY(), y2);
        else
            y1 = std::min(staff->GetDrawingY() - m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize), y2);
    }
    // slur accross an entire system; use the staff position
    else if (spanningType == SPANNING_MIDDLE) {
        // To be adjusted
        if (drawingCurveDir == curvature_CURVEDIR_above)
            y1 = staff->GetDrawingY();
        else
            y1 = staff->GetDrawingY() - m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize);
        y2 = y1;
    }

    /************** y position **************/

    if (drawingCurveDir == curvature_CURVEDIR_above) {
        y1 += 1 * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        y2 += 1 * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    }
    else {
        y1 -= 1 * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        y2 -= 1 * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    }

    Point points[4];
    points[0] = Point(x1, y1);
    points[3] = Point(x2, y2);

    float angle = AdjustSlur(slur, staff, layer1->GetN(), drawingCurveDir, points);

    int thickness = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * m_options->m_slurThickness.GetValue();

    assert(slur->GetCurrentFloatingPositioner());
    slur->GetCurrentFloatingPositioner()->UpdateCurvePosition(points, angle, thickness, drawingCurveDir);

    /************** articulation **************/

    // First get all artic children
    AttComparison matchType(ARTIC);
    ArrayOfObjects artics;
    ArrayOfObjects::iterator articIter;

    // the normal case or start
    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START)) {
        start->FindAllChildByComparison(&artics, &matchType);
        // Then the @n of each first staffDef
        for (articIter = artics.begin(); articIter != artics.end(); ++articIter) {
            Artic *artic = dynamic_cast<Artic *>(*articIter);
            assert(artic);
            ArticPart *outsidePart = artic->GetOutsidePart();
            if (outsidePart) {
                if ((outsidePart->GetPlace().GetBasic() == STAFFREL_basic_above)
                    && (drawingCurveDir == curvature_CURVEDIR_above)) {
                    outsidePart->AddSlurPositioner(slur->GetCurrentFloatingPositioner(), true);
                }
                else if ((outsidePart->GetPlace().GetBasic() == STAFFREL_basic_below)
                    && (drawingCurveDir == curvature_CURVEDIR_below)) {
                    outsidePart->AddSlurPositioner(slur->GetCurrentFloatingPositioner(), true);
                }
            }
        }
    }
    // normal case or end
    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_END)) {
        end->FindAllChildByComparison(&artics, &matchType);
        // Then the @n of each first staffDef
        for (articIter = artics.begin(); articIter != artics.end(); ++articIter) {
            Artic *artic = dynamic_cast<Artic *>(*articIter);
            assert(artic);
            ArticPart *outsidePart = artic->GetOutsidePart();
            if (outsidePart) {
                if ((outsidePart->GetPlace().GetBasic() == STAFFREL_basic_above)
                    && (drawingCurveDir == curvature_CURVEDIR_above)) {
                    outsidePart->AddSlurPositioner(slur->GetCurrentFloatingPositioner(), false);
                }
                else if ((outsidePart->GetPlace().GetBasic() == STAFFREL_basic_below)
                    && (drawingCurveDir == curvature_CURVEDIR_below)) {
                    outsidePart->AddSlurPositioner(slur->GetCurrentFloatingPositioner(), false);
                }
            }
        }
    }

    if (graphic)
        dc->ResumeGraphic(graphic, graphic->GetUuid());
    else
        dc->StartGraphic(slur, "spanning-slur", "");

    DrawThickBezierCurve(dc, points, thickness, staff->m_drawingStaffSize, angle);

    /* drawing debug points */
    /*
    int i;
    for (i = 0; i <= 10; ++i) {
        Point p = BoundingBox::CalcDeCasteljau(points, (double)i / 10.0);
        DrawDot(dc, p.x, p.y, staff->m_drawingStaffSize);
    }
    */

    if (graphic)
        dc->EndResumedGraphic(graphic, this);
    else
        dc->EndGraphic(slur, this);
}

float View::AdjustSlur(Slur *slur, Staff *staff, int layerN, curvature_CURVEDIR curveDir, Point points[4])
{
    // For readability makes them p1 and p2
    Point *p1 = &points[0];
    Point *p2 = &points[3];

    /************** height **************/

    // the 'height' of the bezier
    int height;
    if (slur->HasBulge()) {
        height = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * slur->GetBulge();
    }
    else {
        int dist = abs(p2->x - p1->x);
        height = std::max(int(m_options->m_slurMinHeight.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize)),
            dist / m_options->m_slurHeightFactor.GetValue());
        height = std::min(
            int(m_options->m_slurMaxHeight.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize)), height);
    }

    // the height of the control points
    height = height * 4 / 3;

    /************** content **************/

    System *system = dynamic_cast<System *>(staff->GetFirstParent(SYSTEM));
    assert(system);
    FindTimeSpanningLayerElementsParams findTimeSpanningLayerElementsParams;
    findTimeSpanningLayerElementsParams.m_minPos = p1->x;
    findTimeSpanningLayerElementsParams.m_maxPos = p2->x;
    ArrayOfComparisons filters;
    // Create ad comparison object for each type / @n
    // For now we only look at one layer (assumed layer1 == layer2)
    AttNIntegerComparison matchStaff(STAFF, staff->GetN());
    AttNIntegerComparison matchLayer(LAYER, layerN);
    filters.push_back(&matchStaff);
    filters.push_back(&matchLayer);

    Functor timeSpanningLayerElements(&Object::FindTimeSpanningLayerElements);
    system->Process(&timeSpanningLayerElements, &findTimeSpanningLayerElementsParams, NULL, &filters);
    // if (spanningContent.size() > 12) LogDebug("### %d %s", spanningContent.size(), slur->GetUuid().c_str());

    ArrayOfLayerElementPointPairs spanningContentPoints;
    std::vector<LayerElement *>::iterator it;
    for (it = findTimeSpanningLayerElementsParams.m_spanningContent.begin();
         it != findTimeSpanningLayerElementsParams.m_spanningContent.end(); ++it) {
        // We skip the start or end of the slur
        if ((*it == slur->GetStart()) || (*it == slur->GetEnd())) continue;

        Note *note = NULL;
        // We keep only notes and chords for now
        if (!(*it)->Is(NOTE) && !(*it)->Is(CHORD)) continue;
        // Also skip notes that are part of a chords since we already have the chord
        if ((note = dynamic_cast<Note *>(*it)) && note->IsChordTone()) continue;
        Point p;
        spanningContentPoints.push_back(std::make_pair((*it), p));
    }

    /************** angle **************/

    float slurAngle = GetAdjustedSlurAngle(p1, p2, curveDir, (spanningContentPoints.size() > 0));

    Point rotatedP2 = BoundingBox::CalcPositionAfterRotation(*p2, -slurAngle, *p1);
    // LogDebug("P1 %d %d, P2 %d %d, Angle %f, Pres %d %d", x1, y1, x2, y2, slurAnge, rotadedP2.x, rotatedP2.y);

    /************** control points **************/

    Point rotatedC1, rotatedC2;
    GetControlPoints(p1, &rotatedP2, &rotatedC1, &rotatedC2, curveDir, height, staff->m_drawingStaffSize);

    GetSpanningPointPositions(&spanningContentPoints, *p1, slurAngle, curveDir, staff->m_drawingStaffSize);

    if (!spanningContentPoints.empty()) {

        // Adjust the curvatur (control points are move)
        int adjustedHeight = AdjustSlurCurve(slur, &spanningContentPoints, p1, &rotatedP2, &rotatedC1, &rotatedC2,
            curveDir, slurAngle, staff->m_drawingStaffSize, true);

        // The adjustedHeight value is 0 if everything fits within the slur
        // If not we need to move its position
        if (adjustedHeight != 0) {
            // Use the adjusted control points for adjusting the position (p1, p2 and angle will be updated)
            AdjustSlurPosition(
                slur, &spanningContentPoints, p1, &rotatedP2, &rotatedC1, &rotatedC2, curveDir, &slurAngle, false);
            // Re-calculate the control points with the new height
            GetControlPoints(
                p1, &rotatedP2, &rotatedC1, &rotatedC2, curveDir, adjustedHeight, staff->m_drawingStaffSize);
        }

        // If we still have spanning points then move the slur but now by forcing both sides to be move
        if (!spanningContentPoints.empty()) {
            // First re-calcuate the spanning point positions
            GetSpanningPointPositions(&spanningContentPoints, *p1, slurAngle, curveDir, staff->m_drawingStaffSize);

            // Move it and force both sides to move
            AdjustSlurPosition(
                slur, &spanningContentPoints, p1, &rotatedP2, &rotatedC1, &rotatedC2, curveDir, &slurAngle, true);
            GetControlPoints(
                p1, &rotatedP2, &rotatedC1, &rotatedC2, curveDir, adjustedHeight, staff->m_drawingStaffSize);
        }
    }

    points[1] = BoundingBox::CalcPositionAfterRotation(rotatedC1, slurAngle, *p1);
    points[2] = BoundingBox::CalcPositionAfterRotation(rotatedC2, slurAngle, *p1);
    points[3] = BoundingBox::CalcPositionAfterRotation(rotatedP2, slurAngle, *p1);

    return slurAngle;
}

float View::GetAdjustedSlurAngle(Point *p1, Point *p2, curvature_CURVEDIR curveDir, bool withPoints)
{
    float slurAngle = atan2(p2->y - p1->y, p2->x - p1->x);
    float maxSlope = (float)m_options->m_slurMaxSlope.GetValue() * M_PI / 180.0;

    // For slurs without spanning points allow for double angle
    // This normally looks better with slurs with two notes and high ambitus
    if (!withPoints) maxSlope *= 2.0;

    // the slope of the slur is high and needs to be corrected
    if (fabs(slurAngle) > maxSlope) {
        int side = (p2->x - p1->x) * sin(maxSlope) / sin(M_PI / 2 - maxSlope);
        if (p2->y > p1->y) {
            if (curveDir == curvature_CURVEDIR_above)
                p1->y = p2->y - side;
            else
                p2->y = p1->y + side;
            slurAngle = maxSlope;
        }
        else {
            if (curveDir == curvature_CURVEDIR_above)
                p2->y = p1->y - side;
            else
                p1->y = p2->y + side;
            slurAngle = -maxSlope;
        }
    }

    return slurAngle;
}

void View::GetControlPoints(
    Point *p1, Point *p2, Point *c1, Point *c2, curvature_CURVEDIR curveDir, int height, int staffSize)
{
    // Set the x position of the control points
    int cPos
        = std::min((p2->x - p1->x) / m_options->m_slurControlPoints.GetValue(), m_doc->GetDrawingStaffSize(staffSize));
    c1->x = p1->x + cPos;
    c2->x = p2->x - cPos;

    if (curveDir == curvature_CURVEDIR_above) {
        c1->y = p1->y + height;
        c2->y = p2->y + height;
    }
    else {
        c1->y = p1->y - height;
        c2->y = p2->y - height;
    }
}

void View::GetSpanningPointPositions(
    ArrayOfLayerElementPointPairs *spanningPoints, Point p1, float angle, curvature_CURVEDIR curveDir, int staffSize)
{
    ArrayOfLayerElementPointPairs::iterator itPoint;
    for (itPoint = spanningPoints->begin(); itPoint != spanningPoints->end(); ++itPoint) {
        Point p;
        if (curveDir == curvature_CURVEDIR_above) {
            p.y = itPoint->first->GetDrawingTop(m_doc, staffSize, true, ARTIC_PART_OUTSIDE);
        }
        else {
            p.y = itPoint->first->GetDrawingBottom(m_doc, staffSize, true, ARTIC_PART_OUTSIDE);
        }
        p.x = itPoint->first->GetDrawingX();
        // Not sure if it is better to add the margin before or after the rotation...
        // if (up) p.y += m_doc->GetDrawingUnit(staffSize) * 2;
        // else p.y -= m_doc->GetDrawingUnit(staffSize) * 2;
        itPoint->second = BoundingBox::CalcPositionAfterRotation(p, -angle, p1);
        // This would add it after
        if (curveDir == curvature_CURVEDIR_above) {
            itPoint->second.y += m_doc->GetDrawingUnit(staffSize) * 2;
        }
        else {
            itPoint->second.y -= m_doc->GetDrawingUnit(staffSize) * 2;
        }
    }
}

int View::AdjustSlurCurve(Slur *slur, ArrayOfLayerElementPointPairs *spanningPoints, Point *p1, Point *p2, Point *c1,
    Point *c2, curvature_CURVEDIR curveDir, float angle, int staffSize, bool posRatio)
{
    Point bezier[4];
    bezier[0] = *p1;
    bezier[1] = *c1;
    bezier[2] = *c2;
    bezier[3] = *p2;

    ArrayOfLayerElementPointPairs::iterator itPoint;
    int y;

    int dist = abs(p2->x - p1->x);
    int currentHeight = abs(c1->y - p1->y);
    int maxHeight = 0;

    // 0.2 for avoiding / by 0 (below)
    float maxHeightFactor = std::max(0.2f, fabsf(angle));
    maxHeight = dist
        / (maxHeightFactor
              * (m_options->m_slurCurveFactor.GetValue()
                    + 5)); // 5 is the minimum - can be increased for limiting curvature

    maxHeight = std::max(maxHeight, currentHeight);
    maxHeight = std::min(maxHeight, m_doc->GetDrawingOctaveSize(staffSize));

    bool hasReachedMaxHeight = false;

    if (maxHeight > currentHeight) {
        float maxRatio = 1.0;
        float posXRatio = 1.0;
        int posX;
        for (itPoint = spanningPoints->begin(); itPoint != spanningPoints->end();) {
            y = BoundingBox::CalcBezierAtPosition(bezier, itPoint->second.x);

            // Weight the desired height according to the x position if wanted
            posXRatio = 1.0;
            if (posRatio && (dist != 0)) {
                posX = itPoint->second.x - p1->x;
                if (posX > dist / 2) posX = p2->x - itPoint->second.x;
                if (dist != 0) posXRatio = (float)posX / ((float)dist / 2.0);
            }

            // Keep the maximum desired ratio
            if (curveDir == curvature_CURVEDIR_above) {
                if (y < itPoint->second.y) {
                    float ratio = (float)(p1->y - itPoint->second.y) / (float)(p1->y - y) * posXRatio;
                    maxRatio = ratio > maxRatio ? ratio : maxRatio;
                    ++itPoint;
                }
                // The point is below, we can drop it
                else {
                    itPoint = spanningPoints->erase(itPoint);
                }
            }
            else {
                if (y > itPoint->second.y) {
                    float ratio = (float)(p1->y - itPoint->second.y) / (float)(p1->y - y) * posXRatio;
                    maxRatio = ratio > maxRatio ? ratio : maxRatio;
                    ++itPoint;
                }
                // the point is above, we can drop it
                else {
                    itPoint = spanningPoints->erase(itPoint);
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
    for (itPoint = spanningPoints->begin(); itPoint != spanningPoints->end();) {
        y = BoundingBox::CalcBezierAtPosition(bezier, itPoint->second.x);
        if (curveDir == curvature_CURVEDIR_above) {
            if (y >= itPoint->second.y)
                itPoint = spanningPoints->erase(itPoint);
            else
                ++itPoint;
        }
        else {
            if (y <= itPoint->second.y)
                itPoint = spanningPoints->erase(itPoint);
            else
                ++itPoint;
        }
    }

    if (!spanningPoints->empty()) return maxHeight;

    return 0;
}

void View::AdjustSlurPosition(Slur *slur, ArrayOfLayerElementPointPairs *spanningPoints, Point *p1, Point *p2,
    Point *c1, Point *c2, curvature_CURVEDIR curveDir, float *angle, bool forceBothSides)
{
    Point bezier[4];
    bezier[0] = *p1;
    bezier[1] = *c1;
    bezier[2] = *c2;
    bezier[3] = *p2;

    int maxShiftLeft = 0;
    int maxShiftRight = 0;
    int shift, leftShift, rightShift;

    int dist = abs(p2->x - p1->x);
    float posXRatio = 1.0;

    ArrayOfLayerElementPointPairs::iterator itPoint;
    for (itPoint = spanningPoints->begin(); itPoint != spanningPoints->end();) {
        int y = BoundingBox::CalcBezierAtPosition(bezier, itPoint->second.x);

        // Weight the desired height according to the x position on the other side
        posXRatio = 1.0;
        bool leftPoint = true;
        int posX = itPoint->second.x - p1->x;
        if (posX > dist / 2) {
            posX = p2->x - itPoint->second.x;
            leftPoint = false;
        }
        if (dist != 0) posXRatio = (float)posX / ((float)dist / 2.0);

        shift = 0;
        // Keep the maximum shift on the left and right
        if (curveDir == curvature_CURVEDIR_above) {
            if (y < itPoint->second.y) {
                shift = (itPoint->second.y - p1->y) - (y - p1->y);
            }
        }
        else {
            if (y > itPoint->second.y) {
                shift = (p1->y - itPoint->second.y) - (p1->y - y);
            }
        }
        if (shift > 0) {
            leftShift = (forceBothSides || leftPoint) ? shift : shift * posXRatio;
            rightShift = (forceBothSides || !leftPoint) ? shift : shift * posXRatio;
            maxShiftLeft = leftShift > maxShiftLeft ? leftShift : maxShiftLeft;
            maxShiftRight = rightShift > maxShiftRight ? rightShift : maxShiftRight;
            ++itPoint;
        }
        else {
            // itPoint = spanningPoints->erase(itPoint);
            ++itPoint;
        }
    }

    // Actually nothing to do
    if (spanningPoints->empty()) return;

    // Unrotated the slur
    *p2 = BoundingBox::CalcPositionAfterRotation(*p2, (*angle), *p1);

    if (curveDir == curvature_CURVEDIR_above) {
        p1->y += maxShiftLeft;
        p2->y += maxShiftRight;
    }
    else {
        p1->y -= maxShiftLeft;
        p2->y -= maxShiftRight;
    }

    *angle = GetAdjustedSlurAngle(p1, p2, curveDir, true);
    *p2 = BoundingBox::CalcPositionAfterRotation(*p2, -(*angle), *p1);
}

} // namespace vrv
