/////////////////////////////////////////////////////////////////////////////
// Name:        view_control.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <iostream>
#define _USE_MATH_DEFINES // needed by Windows for math constants like "M_PI"
#include <math.h>
#include <sstream>

//----------------------------------------------------------------------------

#include "attcomparison.h"
#include "bboxdevicecontext.h"
#include "devicecontext.h"
#include "dir.h"
#include "doc.h"
#include "dynam.h"
#include "ending.h"
#include "fermata.h"
#include "functorparams.h"
#include "hairpin.h"
#include "harm.h"
#include "layer.h"
#include "layerelement.h"
#include "measure.h"
#include "note.h"
#include "octave.h"
#include "pedal.h"
#include "slur.h"
#include "smufl.h"
#include "staff.h"
#include "style.h"
#include "syl.h"
#include "system.h"
#include "tempo.h"
#include "text.h"
#include "tie.h"
#include "timeinterface.h"
#include "trill.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// View - FloatingObject - ControlElement
//----------------------------------------------------------------------------

void View::DrawControlElement(DeviceContext *dc, ControlElement *element, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(element);

    // For dir, dynam, and harm, we do not consider the @tstamp2 for rendering
    if (element->HasInterface(INTERFACE_TIME_SPANNING) && (element->Is() != DIR) && (element->Is() != DYNAM)
        && (element->Is() != HARM)) {
        // create placeholder
        dc->StartGraphic(element, "", element->GetUuid());
        dc->EndGraphic(element, this);
        system->AddToDrawingList(element);
    }
    else if (element->Is() == DIR) {
        Dir *dir = dynamic_cast<Dir *>(element);
        assert(dir);
        DrawDir(dc, dir, measure, system);
    }
    else if (element->Is() == DYNAM) {
        Dynam *dynam = dynamic_cast<Dynam *>(element);
        assert(dynam);
        DrawDynam(dc, dynam, measure, system);
    }
    else if (element->Is() == FERMATA) {
        Fermata *fermata = dynamic_cast<Fermata *>(element);
        assert(fermata);
        DrawFermata(dc, fermata, measure, system);
    }
    else if (element->Is() == HARM) {
        Harm *harm = dynamic_cast<Harm *>(element);
        assert(harm);
        DrawHarm(dc, harm, measure, system);
    }
    else if (element->Is() == PEDAL) {
        Pedal *pedal = dynamic_cast<Pedal *>(element);
        assert(pedal);
        DrawPedal(dc, pedal, measure, system);
    }
    else if (element->Is() == TEMPO) {
        Tempo *tempo = dynamic_cast<Tempo *>(element);
        assert(tempo);
        DrawTempo(dc, tempo, measure, system);
    }
    else if (element->Is() == TRILL) {
        Trill *trill = dynamic_cast<Trill *>(element);
        assert(trill);
        DrawTrill(dc, trill, measure, system);
    }
}

void View::DrawTimeSpanningElement(DeviceContext *dc, Object *element, System *system)
{
    assert(dc);
    assert(element);
    assert(system);

    if (dc->Is() == BBOX_DEVICE_CONTEXT) {
        BBoxDeviceContext *bBoxDC = dynamic_cast<BBoxDeviceContext *>(dc);
        assert(bBoxDC);
        if (!bBoxDC->UpdateVerticalValues()) {
            if ((element->Is() == SLUR) || (element->Is() == HAIRPIN) || (element->Is() == OCTAVE)
                || (element->Is() == TIE))
                return;
        }
    }

    TimeSpanningInterface *interface = element->GetTimeSpanningInterface();
    assert(interface);

    if (!interface->HasStartAndEnd()) return;

    // Get the parent system of the first and last note
    System *parentSystem1 = dynamic_cast<System *>(interface->GetStart()->GetFirstParent(SYSTEM));
    System *parentSystem2 = dynamic_cast<System *>(interface->GetEnd()->GetFirstParent(SYSTEM));

    int x1, x2;
    // Staff *staff = NULL;
    Measure *measure = NULL;
    Object *graphic = NULL;
    char spanningType = SPANNING_START_END;

    // The both correspond to the current system, which means no system break in-between (simple case)
    if ((system == parentSystem1) && (system == parentSystem2)) {
        // we use the start measure
        measure = interface->GetStartMeasure();
        if (!Check(measure)) return;
        x1 = interface->GetStart()->GetDrawingX();
        x2 = interface->GetEnd()->GetDrawingX();
        graphic = element;
    }
    // Only the first parent is the same, this means that the element is "open" at the end of the system
    else if (system == parentSystem1) {
        // We need the last measure of the system for x2 - we also use it for getting the staves later
        measure = dynamic_cast<Measure *>(system->FindChildByType(MEASURE, 1, BACKWARD));
        if (!Check(measure)) return;
        x1 = interface->GetStart()->GetDrawingX();
        x2 = measure->GetDrawingX() + measure->GetRightBarLineXRel();
        graphic = element;
        spanningType = SPANNING_START;
    }
    // We are in the system of the last note - draw the element from the beginning of the system
    else if (system == parentSystem2) {
        // We need the first measure of the system for x1 - we also use it for getting the staves later
        measure = dynamic_cast<Measure *>(system->FindChildByType(MEASURE, 1, FORWARD));
        if (!Check(measure)) return;
        // We need the position of the first default in the first measure for x1
        MeasureAlignerTypeComparison alignmentComparison(ALIGNMENT_DEFAULT);
        Alignment *pos
            = dynamic_cast<Alignment *>(measure->m_measureAligner.FindChildByAttComparison(&alignmentComparison, 1));
        x1 = pos ? measure->GetDrawingX() + pos->GetXRel() - 2 * m_doc->GetDrawingDoubleUnit(100)
                 : measure->GetDrawingX();
        x2 = interface->GetEnd()->GetDrawingX();
        spanningType = SPANNING_END;
    }
    // Rare case where neither the first note nor the last note are in the current system - draw the connector
    // throughout the system
    else {
        // We need the first measure of the system for x1 - we also use it for getting the staves later
        measure = dynamic_cast<Measure *>(system->FindChildByType(MEASURE, 1, FORWARD));
        if (!Check(measure)) return;
        // We need the position of the first default in the first measure for x1
        MeasureAlignerTypeComparison alignmentComparison(ALIGNMENT_DEFAULT);
        Alignment *pos
            = dynamic_cast<Alignment *>(measure->m_measureAligner.FindChildByAttComparison(&alignmentComparison, 1));
        x1 = pos ? measure->GetDrawingX() + pos->GetXRel() - 2 * m_doc->GetDrawingDoubleUnit(100)
                 : measure->GetDrawingX();
        // We need the last measure of the system for x2
        Measure *last = dynamic_cast<Measure *>(system->FindChildByType(MEASURE, 1, BACKWARD));
        if (!Check(last)) return;
        x2 = last->GetDrawingX() + last->GetRightBarLineXRel();
        spanningType = SPANNING_MIDDLE;
    }

    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList = interface->GetTstampStaves(measure);
    for (staffIter = staffList.begin(); staffIter != staffList.end(); staffIter++) {

        // TimeSpanning element are not necessary floating elements (e.g., syl) - we have a bounding box only for them
        if (element->IsControlElement())
            system->SetCurrentFloatingPositioner(
                (*staffIter)->GetN(), dynamic_cast<ControlElement *>(element), x1, (*staffIter)->GetDrawingY());

        if (element->Is() == HAIRPIN) {
            // cast to Harprin check in DrawHairpin
            DrawHairpin(dc, dynamic_cast<Hairpin *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is() == OCTAVE) {
            // cast to Slur check in DrawOctave
            DrawOctave(dc, dynamic_cast<Octave *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is() == SLUR) {
            // For slurs we limit support to one value in @staff
            if (staffIter != staffList.begin()) continue;
            // cast to Slur check in DrawSlur
            DrawSlur(dc, dynamic_cast<Slur *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is() == SYL) {
            // cast to Syl check in DrawSylConnector
            DrawSylConnector(dc, dynamic_cast<Syl *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is() == TIE) {
            // For ties we limit support to one value in @staff
            if (staffIter != staffList.begin()) continue;
            // cast to Slur check in DrawTie
            DrawTie(dc, dynamic_cast<Tie *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
    }
}

void View::DrawHairpin(
    DeviceContext *dc, Hairpin *hairpin, int x1, int x2, Staff *staff, char spanningType, Object *graphic)
{
    assert(dc);
    assert(hairpin);
    assert(staff);

    if (!hairpin->HasForm()) {
        // we cannot draw a hairpin that has no form
        return;
    }

    LayerElement *start = NULL;
    LayerElement *end = NULL;

    data_STAFFREL place = hairpin->GetPlace();
    hairpinLog_FORM form = hairpin->GetForm();

    int startY = 0;
    int endY = m_doc->GetDrawingHairpinSize(staff->m_drawingStaffSize, false);

    // We calculate points for cresc by default. Start/End have to be swapped
    if (form == hairpinLog_FORM_dim) BoundingBox::SwapY(&startY, &endY);

    // int y1 = GetHairpinY(hairpin->GetPlace(), staff);
    int y1 = hairpin->GetDrawingY();
    int y2 = y1;

    /************** parent layers **************/

    start = dynamic_cast<LayerElement *>(hairpin->GetStart());
    end = dynamic_cast<LayerElement *>(hairpin->GetEnd());

    if (!start || !end) {
        // no start and end, obviously nothing to do...
        return;
    }

    /* We actually do not need the layer for now

    Layer *layer1 = NULL;
    Layer *layer2 = NULL;

    // For now, with timestamps, get the first layer. We should eventually look at the @layerident (not implemented)
    if (start->Is() == TIMESTAMP_ATTR)
        layer1 = dynamic_cast<Layer *>(staff->FindChildByType(LAYER));
    else
        layer1 = dynamic_cast<Layer *>(start->GetFirstParent(LAYER));

    // idem
    if (end->Is() == TIMESTAMP_ATTR)
        layer2 = dynamic_cast<Layer *>(staff->FindChildByType(LAYER));
    else
        layer2 = dynamic_cast<Layer *>(end->GetFirstParent(LAYER));

    assert(layer1 && layer2);

     */

    /************** start / end opening **************/

    if (form == hairpinLog_FORM_cres) {
        // the normal case
        if (spanningType == SPANNING_START_END) {
            // nothing to adjust
        }
        // In this case, we are drawing the first half a a cresc. Reduce the openning end
        else if (spanningType == SPANNING_START) {
            endY = endY / 2;
        }
        // Now this is the case we are drawing the end of a cresc. Increase the openning start
        else if (spanningType == SPANNING_END) {
            startY = endY / 2;
        }
        // Finally, cres accross the system, increase the start and reduce the end
        else {
            startY = m_doc->GetDrawingHairpinSize(staff->m_drawingStaffSize, false) / 3;
            endY = 2 * startY;
        }
    }
    else {
        // the normal case
        if (spanningType == SPANNING_START_END) {
            // nothing to adjust
        }
        // In this case, we are drawing the first half a a dim. Increase the openning end
        else if (spanningType == SPANNING_START) {
            endY = startY / 2;
        }
        // Now this is the case we are drawing the end of a dim. Reduce the openning start
        else if (spanningType == SPANNING_END) {
            startY = startY / 2;
        }
        // Finally, dim accross the system, reduce the start and increase the end
        else {
            endY = m_doc->GetDrawingHairpinSize(staff->m_drawingStaffSize, false) / 3;
            startY = 2 * endY;
        }
    }

    /************** direction **************/

    /*
    // first should be the tie @curvedir
    if (slur->HasCurvedir()) {
        up = (slur->GetCurvedir() == curvature_CURVEDIR_above) ? true : false;
    }
    // then layer direction trumps note direction
    else if (layer1 && layer1->GetDrawingStemDir() != STEMDIRECTION_NONE) {
        up = layer1->GetDrawingStemDir() == STEMDIRECTION_up ? true : false;
    }
    // look if in a chord
    else if (startParentChord) {
        if (startParentChord->PositionInChord(startNote) < 0) {
            up = false;
        }
        else if (startParentChord->PositionInChord(startNote) > 0) {
            up = true;
        }
        // away from the stem if odd number (center note)
        else {
            up = (stemDir != STEMDIRECTION_up);
        }
    }
    else if (stemDir == STEMDIRECTION_up) {
        up = false;
    }
    else if (stemDir == STEMDIRECTION_NONE) {
        // no information from the note stem directions, look at the position in the notes
        int center = staff->GetDrawingY() - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * 2;
        up = (start->GetDrawingY() > center) ? true : false;
    }
    */

    /************** adjusting y position **************/

    if (place == STAFFREL_above) {
        // y1 += 1 * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        // y2 += 1 * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    }
    else {
        // y1 -= 1 * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        // y2 -= 1 * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    }

    /************** draw it **************/

    y1 -= m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2;
    y2 -= m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2;

    if (graphic)
        dc->ResumeGraphic(graphic, graphic->GetUuid());
    else
        dc->StartGraphic(hairpin, "spanning-hairpin", "");
    // dc->DeactivateGraphic();

    DrawObliquePolygon(
        dc, x1, y1 - startY / 2, x2, y2 - endY / 2, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize));
    DrawObliquePolygon(
        dc, x1, y1 + startY / 2, x2, y2 + endY / 2, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize));

    // dc->ReactivateGraphic();
    if (graphic)
        dc->EndResumedGraphic(graphic, this);
    else
        dc->EndGraphic(hairpin, this);
}

void View::DrawOctave(
    DeviceContext *dc, Octave *octave, int x1, int x2, Staff *staff, char spanningType, Object *graphic)
{
    assert(dc);
    assert(octave);
    assert(staff);

    if (!octave->HasDis() || !octave->HasDisPlace()) {
        // we cannot draw an octave that has no @dis nor a @dis.place
        return;
    }

    LayerElement *start = NULL;
    LayerElement *end = NULL;

    data_OCTAVE_DIS dis = octave->GetDis();
    data_PLACE disPlace = octave->GetDisPlace();

    int y1 = octave->GetDrawingY();
    int y2 = y1;

    /************** parent layers **************/

    start = dynamic_cast<LayerElement *>(octave->GetStart());
    end = dynamic_cast<LayerElement *>(octave->GetEnd());

    if (!start || !end) {
        // no start and end, obviously nothing to do...
        return;
    }

    /********** adjust the end position ***********/

    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_END)) {
        if (octave->HasEndid())
            x2 += (m_doc->GetGlyphWidth(SMUFL_E0A2_noteheadWhole, staff->m_drawingStaffSize, false) / 2);
    }

    /************** draw it **************/

    if (graphic)
        dc->ResumeGraphic(graphic, graphic->GetUuid());
    else
        dc->StartGraphic(octave, "spanning-octave", "");

    int code = SMUFL_E511_ottavaAlta;
    if (disPlace == PLACE_above) {
        switch (dis) {
            // here we could use other glyphs depending on the style
            case OCTAVE_DIS_8: code = SMUFL_E510_ottava; break;
            case OCTAVE_DIS_15: code = SMUFL_E514_quindicesima; break;
            case OCTAVE_DIS_22: code = SMUFL_E517_ventiduesima; break;
            default: break;
        }
    }
    else {
        switch (dis) {
            // ditto
            case OCTAVE_DIS_8: code = SMUFL_E510_ottava; break;
            case OCTAVE_DIS_15: code = SMUFL_E514_quindicesima; break;
            case OCTAVE_DIS_22: code = SMUFL_E517_ventiduesima; break;
            default: break;
        }
    }
    int w, h;
    int lineWidthFactor = 1;
    std::wstring str;
    str.push_back(code);
    if (octave->HasLwidth()) {
        if (octave->GetLwidth() == "wide") {
            lineWidthFactor *= 4;
        }
        else if (octave->GetLwidth() == "medium") {
            lineWidthFactor *= 2;
        }
    }
    int lineWidth = lineWidthFactor * m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
    dc->SetFont(m_doc->GetDrawingSmuflFont(staff->m_drawingStaffSize, false));
    dc->GetSmuflTextExtent(str, &w, &h);
    int yCode = (disPlace == PLACE_above) ? y1 - h : y1;
    DrawSmuflCode(dc, x1 - w, yCode, code, staff->m_drawingStaffSize, false);
    dc->ResetFont();

    y2 += (disPlace == PLACE_above) ? -h : h;
    // adjust is to avoid the figure to touch the line
    x1 += m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);

    if (octave->HasLform()) {
        if (octave->GetLform() == LINEFORM_solid) {
            h *= 0;
        }
    }

    dc->SetPen(m_currentColour, lineWidth, AxSOLID, h / 3);
    dc->SetBrush(m_currentColour, AxSOLID);

    dc->DrawLine(ToDeviceContextX(x1), ToDeviceContextY(y1), ToDeviceContextX(x2), ToDeviceContextY(y1));
    // draw the ending vertical line if not the end of the system
    if (spanningType != SPANNING_START)
        dc->DrawLine(ToDeviceContextX(x2), ToDeviceContextY(y1 + lineWidth / 2), ToDeviceContextX(x2),
            ToDeviceContextY(y2 + lineWidth / 2));

    dc->ResetPen();
    dc->ResetBrush();

    if (graphic)
        dc->EndResumedGraphic(graphic, this);
    else
        dc->EndGraphic(octave, this);
}

void View::DrawSlur(DeviceContext *dc, Slur *slur, int x1, int x2, Staff *staff, char spanningType, Object *graphic)
{
    assert(dc);
    assert(slur);
    assert(staff);

    LayerElement *start = NULL;
    LayerElement *end = NULL;
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
    int y1 = staff->GetDrawingY();
    int y2 = staff->GetDrawingY();

    /************** parent layers **************/

    start = dynamic_cast<LayerElement *>(slur->GetStart());
    end = dynamic_cast<LayerElement *>(slur->GetEnd());

    if (!start || !end) {
        // no start and end, obviously nothing to do...
        return;
    }

    if (start->Is() == NOTE) {
        startNote = dynamic_cast<Note *>(start);
        assert(startNote);
        startParentChord = startNote->IsChordTone();
        startStemDir = startNote->GetDrawingStemDir();
    }
    else if (start->Is() == CHORD) {
        startChord = dynamic_cast<Chord *>(start);
        assert(startChord);
        startStemDir = startChord->GetDrawingStemDir();
    }
    if (end->Is() == NOTE) {
        endNote = dynamic_cast<Note *>(end);
        assert(endNote);
        endParentChord = endNote->IsChordTone();
        endStemDir = endNote->GetDrawingStemDir();
    }
    else if (end->Is() == CHORD) {
        endChord = dynamic_cast<Chord *>(end);
        assert(endChord);
        endStemDir = endChord->GetDrawingStemDir();
    }

    Layer *layer1 = NULL;
    Layer *layer2 = NULL;

    // For now, with timestamps, get the first layer. We should eventually look at the @layerident (not implemented)
    if (start->Is() == TIMESTAMP_ATTR)
        layer1 = dynamic_cast<Layer *>(staff->FindChildByType(LAYER));
    else
        layer1 = dynamic_cast<Layer *>(start->GetFirstParent(LAYER));

    // idem
    if (end->Is() == TIMESTAMP_ATTR)
        layer2 = dynamic_cast<Layer *>(staff->FindChildByType(LAYER));
    else
        layer2 = dynamic_cast<Layer *>(end->GetFirstParent(LAYER));

    assert(layer1 && layer2);

    if (layer1->GetN() != layer2->GetN()) {
        LogWarning("Slurs between different layers may not be fully supported.");
    }

    /************** note stem dir **************/

    // the normal case
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

    // first should be the tie @curvedir
    if (slur->HasCurvedir()) {
        drawingCurveDir
            = (slur->GetCurvedir() == curvature_CURVEDIR_above) ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
    }
    // then layer direction trumps note direction
    else if (layer1 && layer1->GetDrawingStemDir() != STEMDIRECTION_NONE) {
        drawingCurveDir
            = layer1->GetDrawingStemDir() == STEMDIRECTION_up ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
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
    if (x2 - x1 < 3 * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize)) isShortSlur = true;

    int yChordMax, yChordMin;
    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START)) {
        // first get the min max of the chord (if any)
        if (startParentChord) {
            startParentChord->GetYExtremes(&yChordMax, &yChordMin);
        }
        else if (startChord) {
            startChord->GetYExtremes(&yChordMax, &yChordMin);
        }
        // slur is up
        if (drawingCurveDir == curvature_CURVEDIR_above) {
            // P(^)
            if (startStemDir == STEMDIRECTION_down) y1 = start->GetDrawingTop(m_doc, staff->m_drawingStaffSize);
            //  d(^)d
            else if (isShortSlur || ((parentBeam = start->IsInBeam()) && !parentBeam->IsLastInBeam(start))) {
                y1 = start->GetDrawingTop(m_doc, staff->m_drawingStaffSize);
            }
            // d(^)
            else {
                // put it on the side, move it left, but not if we have a @stamp
                if (start->Is() != TIMESTAMP_ATTR) x1 += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 4 / 2;
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
            else if (isShortSlur || ((parentBeam = start->IsInBeam()) && !parentBeam->IsLastInBeam(start))) {
                y1 = start->GetDrawingBottom(m_doc, staff->m_drawingStaffSize);
            }
            // P(_)
            else {
                // put it on the side, but no need to move it left
                y1 = start->GetDrawingY() + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2;
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
            endParentChord->GetYExtremes(&yChordMax, &yChordMin);
        }
        else if (endChord) {
            endChord->GetYExtremes(&yChordMax, &yChordMin);
        }
        // get the stem direction of the end
        // slur is up
        if (drawingCurveDir == curvature_CURVEDIR_above) {
            // (^)P
            if (endStemDir == STEMDIRECTION_down) y2 = end->GetDrawingTop(m_doc, staff->m_drawingStaffSize);
            // d(^)d
            else if (isShortSlur || ((parentBeam = end->IsInBeam()) && !parentBeam->IsFirstInBeam(end))) {
                y2 = end->GetDrawingTop(m_doc, staff->m_drawingStaffSize);
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
            // (_)d
            if (endStemDir == STEMDIRECTION_up) y2 = end->GetDrawingBottom(m_doc, staff->m_drawingStaffSize);
            // P(_)P
            else if (isShortSlur || ((parentBeam = end->IsInBeam()) && !parentBeam->IsFirstInBeam(end))) {
                y2 = end->GetDrawingBottom(m_doc, staff->m_drawingStaffSize);
            }
            // (_)P
            else {
                // put it on the side, move it right, but not if we have a @stamp2
                if (end->Is() != TIMESTAMP_ATTR) x2 -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2;
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
    if (end->Is() == TIMESTAMP_ATTR) {
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
    if (start->Is() == TIMESTAMP_ATTR) {
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

    int thickness = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * m_doc->GetSlurThickness() / PARAM_DENOMINATOR;

    assert(slur->GetCurrentFloatingPositioner());
    slur->GetCurrentFloatingPositioner()->UpdateCurvePosition(points, angle, thickness, drawingCurveDir);

    /************** articulation **************/

    // First get all artic children
    AttComparison matchType(ARTIC);
    ArrayOfObjects artics;
    ArrayOfObjects::iterator articIter;

    // the normal case or start
    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START)) {
        start->FindAllChildByAttComparison(&artics, &matchType);
        // Then the @n of each first staffDef
        for (articIter = artics.begin(); articIter != artics.end(); articIter++) {
            Artic *artic = dynamic_cast<Artic *>(*articIter);
            assert(artic);
            ArticPart *outsidePart = artic->GetOutsidePart();
            if (outsidePart) {
                if ((outsidePart->GetPlace() == STAFFREL_above) && (drawingCurveDir == curvature_CURVEDIR_above)) {
                    outsidePart->AddSlurPositioner(slur->GetCurrentFloatingPositioner(), true);
                }
                else if ((outsidePart->GetPlace() == STAFFREL_below) && (drawingCurveDir == curvature_CURVEDIR_below)) {
                    outsidePart->AddSlurPositioner(slur->GetCurrentFloatingPositioner(), true);
                }
            }
        }
    }
    // normal case or end
    if ((spanningType == SPANNING_START_END) || (SPANNING_END)) {
        end->FindAllChildByAttComparison(&artics, &matchType);
        // Then the @n of each first staffDef
        for (articIter = artics.begin(); articIter != artics.end(); articIter++) {
            Artic *artic = dynamic_cast<Artic *>(*articIter);
            assert(artic);
            ArticPart *outsidePart = artic->GetOutsidePart();
            if (outsidePart) {
                if ((outsidePart->GetPlace() == STAFFREL_above) && (drawingCurveDir == curvature_CURVEDIR_above)) {
                    outsidePart->AddSlurPositioner(slur->GetCurrentFloatingPositioner(), false);
                }
                else if ((outsidePart->GetPlace() == STAFFREL_below) && (drawingCurveDir == curvature_CURVEDIR_below)) {
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

    /*
    int i;
    int dist = (points[3].x - points[0].x) / 10;
    for (i = 0; i < 10; i++) {
        int x = points[0].x + (i * dist);
        int y = BoundingBox::CalcBezierAtPosition(points, x);
        DrawDot(dc, x, y, staff->m_drawingStaffSize);
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

    /************** angle **************/

    float slurAngle = GetAdjustedSlurAngle(p1, p2, curveDir);

    Point rotatedP2 = BoundingBox::CalcPositionAfterRotation(*p2, -slurAngle, *p1);
    // LogDebug("P1 %d %d, P2 %d %d, Angle %f, Pres %d %d", x1, y1, x2, y2, slurAnge, rotadedP2.x, rotatedP2.y);

    /************** height **************/

    // the 'height' of the bezier
    int height;
    if (slur->HasBulge()) {
        height = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * slur->GetBulge();
    }
    else {
        int dist = abs(p2->x - p1->x);
        height
            = std::max(m_doc->GetSlurMinHeight() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / PARAM_DENOMINATOR,
                dist / TEMP_SLUR_HEIGHT_FACTOR);
        height = std::min(
            m_doc->GetSlurMaxHeight() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / PARAM_DENOMINATOR, height);
    }

    // the height of the control points
    height = height * 4 / 3;

    /************** control points **************/

    Point rotatedC1, rotatedC2;
    GetControlPoints(p1, &rotatedP2, &rotatedC1, &rotatedC2, curveDir, height, staff->m_drawingStaffSize);

    /************** content **************/

    System *system = dynamic_cast<System *>(staff->GetFirstParent(SYSTEM));
    assert(system);
    FindTimeSpanningLayerElementsParams findTimeSpanningLayerElementsParams;
    findTimeSpanningLayerElementsParams.m_minPos = p1->x;
    findTimeSpanningLayerElementsParams.m_maxPos = p2->x;
    std::vector<AttComparison *> filters;
    // Create ad comparison object for each type / @n
    // For now we only look at one layer (assumed layer1 == layer2)
    AttCommonNComparison matchStaff(STAFF, staff->GetN());
    AttCommonNComparison matchLayer(LAYER, layerN);
    filters.push_back(&matchStaff);
    filters.push_back(&matchLayer);

    Functor timeSpanningLayerElements(&Object::FindTimeSpanningLayerElements);
    system->Process(&timeSpanningLayerElements, &findTimeSpanningLayerElementsParams, NULL, &filters);
    // if (spanningContent.size() > 12) LogDebug("### %d %s", spanningContent.size(), slur->GetUuid().c_str());

    ArrayOfLayerElementPointPairs spanningContentPoints;
    std::vector<LayerElement *>::iterator it;
    for (it = findTimeSpanningLayerElementsParams.m_spanningContent.begin();
         it != findTimeSpanningLayerElementsParams.m_spanningContent.end(); it++) {
        Note *note = NULL;
        // We keep only notes and chords for now
        if (((*it)->Is() != NOTE) && ((*it)->Is() != CHORD)) continue;
        // Also skip notes that are part of a chords since we already have the chord
        if ((note = dynamic_cast<Note *>(*it)) && note->IsChordTone()) continue;
        Point p;
        spanningContentPoints.push_back(std::make_pair((*it), p));
    }

    GetSpanningPointPositions(&spanningContentPoints, *p1, slurAngle, curveDir, staff->m_drawingStaffSize);

    // We need to keep the original control points
    Point adjustedRotatedC1 = rotatedC1;
    Point adjustedRotatedC2 = rotatedC2;

    if (!spanningContentPoints.empty()) {
        AdjustSlurCurve(
            slur, &spanningContentPoints, p1, &rotatedP2, &adjustedRotatedC1, &adjustedRotatedC2, curveDir, slurAngle);
        // Use the adjusted control points for adjusting the position (p1, p2 and angle will be updated)
        AdjustSlurPosition(slur, &spanningContentPoints, p1, &rotatedP2, &adjustedRotatedC1, &adjustedRotatedC2,
            curveDir, &slurAngle, false);
        // Now readjust the curvature with the new p1 and p2 with the original control points
        GetControlPoints(p1, &rotatedP2, &rotatedC1, &rotatedC2, curveDir, height, staff->m_drawingStaffSize);

        GetSpanningPointPositions(&spanningContentPoints, *p1, slurAngle, curveDir, staff->m_drawingStaffSize);
        int maxHeight = AdjustSlurCurve(
            slur, &spanningContentPoints, p1, &rotatedP2, &rotatedC1, &rotatedC2, curveDir, slurAngle, false);

        if (maxHeight) {
            // Something went wrong since now all spanning points should be gone...
            // LogDebug("### %d notes for %s will need position adjustment", spanningContentPoints.size(),
            // slur->GetUuid().c_str());
            // Use the normal control points for adjusting the position (p1, p2 and angle will be updated)
            // Move it and force both sides to move
            AdjustSlurPosition(
                slur, &spanningContentPoints, p1, &rotatedP2, &rotatedC1, &rotatedC2, curveDir, &slurAngle, true);
            GetControlPoints(p1, &rotatedP2, &rotatedC1, &rotatedC2, curveDir, maxHeight, staff->m_drawingStaffSize);
        }
    }
    else {
        rotatedC1 = adjustedRotatedC1;
        rotatedC2 = adjustedRotatedC2;
    }

    points[1] = BoundingBox::CalcPositionAfterRotation(rotatedC1, slurAngle, *p1);
    points[2] = BoundingBox::CalcPositionAfterRotation(rotatedC2, slurAngle, *p1);
    points[3] = BoundingBox::CalcPositionAfterRotation(rotatedP2, slurAngle, *p1);

    return slurAngle;
}

float View::GetAdjustedSlurAngle(Point *p1, Point *p2, curvature_CURVEDIR curveDir)
{
    float slurAngle = atan2(p2->y - p1->y, p2->x - p1->x);

    // the slope of the slur is high and needs to be corrected
    if (fabs(slurAngle) > TEMP_SLUR_MAX_SLOPE) {
        int side = (p2->x - p1->x) * sin(TEMP_SLUR_MAX_SLOPE) / sin(M_PI / 2 - TEMP_SLUR_MAX_SLOPE);
        if (p2->y > p1->y) {
            if (curveDir == curvature_CURVEDIR_above)
                p1->y = p2->y - side;
            else
                p2->y = p1->y + side;
            slurAngle = (float)TEMP_SLUR_MAX_SLOPE;
        }
        else {
            if (curveDir == curvature_CURVEDIR_above)
                p2->y = p1->y - side;
            else
                p1->y = p2->y + side;
            slurAngle = (float)-TEMP_SLUR_MAX_SLOPE;
        }
    }

    return slurAngle;
}

void View::GetControlPoints(
    Point *p1, Point *p2, Point *c1, Point *c2, curvature_CURVEDIR curveDir, int height, int staffSize)
{
    // Set the x position of the control points
    int cPos = std::min((p2->x - p1->x) / TEMP_SLUR_CONTROL_POINT_FACTOR, m_doc->GetDrawingStaffSize(staffSize));
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
    for (itPoint = spanningPoints->begin(); itPoint != spanningPoints->end(); itPoint++) {
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
    Point *c2, curvature_CURVEDIR curveDir, float angle, bool posRatio)
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
    maxHeight = dist / (maxHeightFactor * (TEMP_SLUR_CURVE_FACTOR
                                              + 5)); // 5 is the minimum - can be increased for limiting curvature
    if (posRatio) {
        // Do we want to set a max height?
        // maxHeight = std::min(maxHeight, m_doc->GetDrawingStaffSize(100));
    }

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
                    itPoint++;
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
                    itPoint++;
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

    // Check if we need further adjustment of the points with the adjusted curve
    /*
    bezier[1] = *c1;
    bezier[2] = *c2;
    for (itPoint = spanningPoints->begin(); itPoint != spanningPoints->end();) {
        y = View::CalcBezierAtPosition(bezier, itPoint->second.x);
        if (up) {
            //if (y > itPoint->second.y) itPoint = spanningPoints->erase(itPoint);
            //else itPoint++;
        }
        else {
            //if (y < itPoint->second.y) itPoint = spanningPoints->erase(itPoint);
            //else itPoint++;
        }
        itPoint++;
    }

    // We will need to adjust the further if the list is not empty
    return (!spanningPoints->empty());
    */

    if (hasReachedMaxHeight)
        return maxHeight;
    else
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
    int posX;

    ArrayOfLayerElementPointPairs::iterator itPoint;
    int y;

    for (itPoint = spanningPoints->begin(); itPoint != spanningPoints->end();) {
        y = BoundingBox::CalcBezierAtPosition(bezier, itPoint->second.x);

        // Weight the desired height according to the x position on the other side
        posXRatio = 1.0;
        bool leftPoint = true;
        posX = itPoint->second.x - p1->x;
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
            itPoint++;
        }
        else {
            // itPoint = spanningPoints->erase(itPoint);
            itPoint++;
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

    *angle = GetAdjustedSlurAngle(p1, p2, curveDir);
    *p2 = BoundingBox::CalcPositionAfterRotation(*p2, -(*angle), *p1);
}

void View::DrawTie(DeviceContext *dc, Tie *tie, int x1, int x2, Staff *staff, char spanningType, Object *graphic)
{
    assert(dc);
    assert(tie);
    assert(staff);

    Note *note1 = NULL;
    Note *note2 = NULL;
    Chord *parentChord1 = NULL;

    curvature_CURVEDIR drawingCurveDir = curvature_CURVEDIR_above;
    data_STEMDIRECTION noteStemDir = STEMDIRECTION_NONE;
    int y1, y2;

    /************** parent layers **************/

    note1 = dynamic_cast<Note *>(tie->GetStart());
    note2 = dynamic_cast<Note *>(tie->GetEnd());

    if (!note1 || !note2) {
        // no note, obviously nothing to do...
        // this also means that notes with tstamp events are not supported
        return;
    }

    parentChord1 = note1->IsChordTone();

    Layer *layer1 = dynamic_cast<Layer *>(note1->GetFirstParent(LAYER));
    Layer *layer2 = dynamic_cast<Layer *>(note2->GetFirstParent(LAYER));
    assert(layer1 && layer2);

    if (layer1->GetN() != layer2->GetN()) {
        LogWarning("Ties between different layers may not be fully supported.");
    }

    /************** x positions **************/

    bool isShortTie = false;
    // shortTie correction cannot be applied for chords
    if (!parentChord1 && (x2 - x1 < 3 * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize))) isShortTie = true;

    // the normal case
    if (spanningType == SPANNING_START_END) {
        y1 = note1->GetDrawingY();
        y2 = note2->GetDrawingY();
        if (!isShortTie) {
            x1 += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 3 / 2;
            x2 -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 3 / 2;
            if (note1->HasDots()) {
                x1 += m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * note1->GetDots();
            }
            else if (parentChord1 && parentChord1->HasDots()) {
                x1 += m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * parentChord1->GetDots();
            }
        }
        noteStemDir = note1->GetDrawingStemDir();
    }
    // This is the case when the tie is split over two system of two pages.
    // In this case, we are now drawing its beginning to the end of the measure (i.e., the last aligner)
    else if (spanningType == SPANNING_START) {
        y1 = note1->GetDrawingY();
        y2 = y1;
        if (!isShortTie) {
            x1 += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 3 / 2;
        }
        noteStemDir = note1->GetDrawingStemDir();
    }
    // Now this is the case when the tie is split but we are drawing the end of it
    else if (spanningType == SPANNING_END) {
        y1 = note2->GetDrawingY();
        y2 = y1;
        if (!isShortTie) {
            x2 -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 3 / 2;
        }
        noteStemDir = note2->GetDrawingStemDir();
    }
    // Finally
    else {
        LogDebug("Tie across an entire system is not supported");
        return;
    }

    /************** direction **************/

    // first should be the tie @curvedir
    if (tie->HasCurvedir()) {
        drawingCurveDir
            = (tie->GetCurvedir() == curvature_CURVEDIR_above) ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
    }
    // then layer direction trumps note direction
    else if (layer1 && layer1->GetDrawingStemDir() != STEMDIRECTION_NONE) {
        drawingCurveDir
            = layer1->GetDrawingStemDir() == STEMDIRECTION_up ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
    }
    // look if in a chord
    else if (parentChord1) {
        if (parentChord1->PositionInChord(note1) < 0) {
            drawingCurveDir = curvature_CURVEDIR_below;
        }
        else if (parentChord1->PositionInChord(note1) > 0) {
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
        int center = staff->GetDrawingY() - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * 2;
        drawingCurveDir = (y1 > center) ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
    }

    /************** y position **************/

    if (drawingCurveDir == curvature_CURVEDIR_above) {
        y1 += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2;
        y2 += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2;
        if (isShortTie) {
            y1 += m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
            y2 += m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        }
    }
    else {
        y1 -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2;
        y2 -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2;
        if (isShortTie) {
            y1 -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
            y2 -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        }
    }

    /************** bezier points **************/

    // the 'height' of the bezier
    int height;
    if (tie->HasBulge()) {
        height = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * tie->GetBulge();
    }
    else {
        height = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        // if the space between the to points is more than two staff height, increase the height
        if (x2 - x1 > 2 * m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize)) {
            height += m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        }
    }
    int thickness = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * m_doc->GetTieThickness() / PARAM_DENOMINATOR;

    // control points
    Point c1, c2;

    // the height of the control points
    height = height * 4 / 3;

    c1.x = x1 + (x2 - x1) / 4; // point at 1/4
    c2.x = x1 + (x2 - x1) / 4 * 3; // point at 3/4

    if (drawingCurveDir == curvature_CURVEDIR_above) {
        c1.y = y1 + height;
        c2.y = y2 + height;
    }
    else {
        c1.y = y1 - height;
        c2.y = y2 - height;
    }

    Point bezier[4];
    bezier[0] = Point(x1, y1);
    bezier[1] = c1;
    bezier[2] = c2;
    bezier[3] = Point(x2, y2);

    assert(tie->GetCurrentFloatingPositioner());
    tie->GetCurrentFloatingPositioner()->UpdateCurvePosition(bezier, 0.0, thickness, drawingCurveDir);

    if (graphic)
        dc->ResumeGraphic(graphic, graphic->GetUuid());
    else
        dc->StartGraphic(tie, "spanning-tie", "");
    DrawThickBezierCurve(dc, bezier, thickness, staff->m_drawingStaffSize);
    if (graphic)
        dc->EndResumedGraphic(graphic, this);
    else
        dc->EndGraphic(tie, this);
}

void View::DrawSylConnector(
    DeviceContext *dc, Syl *syl, int x1, int x2, Staff *staff, char spanningType, Object *graphic)
{
    assert(syl);
    assert(syl->GetStart() && syl->GetEnd());
    if (!syl->GetStart() || !syl->GetEnd()) return;

    int y = GetSylY(syl, staff);
    TextExtend extend;

    // The both correspond to the current system, which means no system break in-between (simple case)
    if (spanningType == SPANNING_START_END) {
        dc->SetFont(m_doc->GetDrawingLyricFont(staff->m_drawingStaffSize));
        dc->GetTextExtent(syl->GetText(syl), &extend);
        dc->ResetFont();
        // x position of the syl is two units back
        x1 += extend.m_width - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2;
    }
    // Only the first parent is the same, this means that the syl is "open" at the end of the system
    else if (spanningType == SPANNING_START) {
        dc->SetFont(m_doc->GetDrawingLyricFont(staff->m_drawingStaffSize));
        dc->GetTextExtent(syl->GetText(syl), &extend);
        dc->ResetFont();
        // idem
        x1 += extend.m_width - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2;
    }
    // We are in the system of the last note - draw the connector from the beginning of the system
    else if (spanningType == SPANNING_END) {
        // nothing to adjust
    }
    // Rare case where neither the first note nor the last note are in the current system - draw the connector
    // throughout the system
    else {
        // nothing to adjust
    }

    if (graphic)
        dc->ResumeGraphic(graphic, graphic->GetUuid());
    else
        dc->StartGraphic(syl, "spanning-connector", "");
    dc->DeactivateGraphic();
    DrawSylConnectorLines(dc, x1, x2, y, syl, staff);
    dc->ReactivateGraphic();
    if (graphic)
        dc->EndResumedGraphic(graphic, this);
    else
        dc->EndGraphic(syl, this);
}

void View::DrawSylConnectorLines(DeviceContext *dc, int x1, int x2, int y, Syl *syl, Staff *staff)
{
    if (syl->GetCon() == sylLog_CON_d) {

        y += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2 / 3;
        // x position of the syl is two units back
        x2 -= 2 * (int)m_doc->GetDrawingUnit(staff->m_drawingStaffSize);

        // if (x1 > x2) {
        //    DrawFilledRectangle(dc, x1, y + 2 * m_doc->GetDrawingBarLineWidth(staff->m_drawingStaffSize), x2, y + 3 *
        //    m_doc->GetDrawingBarLineWidth(staff->m_drawingStaffSize));
        //    LogDebug("x1 > x2 (%d %d)", x1, x2);
        //}

        // the length of the dash and the space between them - can be made a parameter
        int dashLength = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 4 / 3;
        int dashSpace = m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize) * 5 / 3;
        int halfDashLength = dashLength / 2;

        int dist = x2 - x1;
        int nbDashes = dist / dashSpace;

        int margin = dist / 2;
        // no dash if the distance is smaller than a dash length
        if (dist < dashLength) {
            nbDashes = 0;
        }
        // at least one dash
        else if (nbDashes < 2) {
            nbDashes = 1;
        }
        else {
            margin = (dist - ((nbDashes - 1) * dashSpace)) / 2;
        }

        int i, x;
        for (i = 0; i < nbDashes; i++) {
            x = x1 + margin + (i * dashSpace);
            x = std::max(x, x1);
            DrawFilledRectangle(dc, x - halfDashLength, y, x + halfDashLength,
                y + m_doc->GetDrawingBarLineWidth(staff->m_drawingStaffSize));
        }
    }
    else if (syl->GetCon() == sylLog_CON_u) {
        x1 += (int)m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2;
        DrawFilledRectangle(dc, x1, y, x2, y + m_doc->GetDrawingBarLineWidth(staff->m_drawingStaffSize));
    }
}

void View::DrawDir(DeviceContext *dc, Dir *dir, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(dir);

    // We cannot draw a dir that has no start position
    if (!dir->GetStart()) return;

    dc->StartGraphic(dir, "", dir->GetUuid());

    FontInfo dirTxt;

    // If we have not timestamp
    int x = dir->GetStart()->GetDrawingX();

    bool setX = false;
    bool setY = false;

    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList = dir->GetTstampStaves(measure);
    for (staffIter = staffList.begin(); staffIter != staffList.end(); staffIter++) {
        system->SetCurrentFloatingPositioner((*staffIter)->GetN(), dir, x, (*staffIter)->GetDrawingY());

        int y = dir->GetDrawingY();

        dirTxt.SetPointSize(m_doc->GetDrawingLyricFont((*staffIter)->m_drawingStaffSize)->GetPointSize());

        dc->SetBrush(m_currentColour, AxSOLID);
        dc->SetFont(&dirTxt);

        dc->StartText(ToDeviceContextX(x), ToDeviceContextY(y), LEFT);
        DrawTextChildren(dc, dir, x, y, setX, setY);
        dc->EndText();

        dc->ResetFont();
        dc->ResetBrush();
    }

    dc->EndGraphic(dir, this);
}

void View::DrawDynam(DeviceContext *dc, Dynam *dynam, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(dynam);

    // We cannot draw dynamics that have no start position
    if (!dynam->GetStart()) return;

    dc->StartGraphic(dynam, "", dynam->GetUuid());

    bool isSymbolOnly = dynam->IsSymbolOnly();
    std::wstring dynamSymbol;
    if (isSymbolOnly) {
        dynamSymbol = dynam->GetSymbolStr();
    }

    FontInfo dynamTxt;

    // If we have not timestamp
    int x = dynam->GetStart()->GetDrawingX();

    bool setX = false;
    bool setY = false;

    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList = dynam->GetTstampStaves(measure);
    for (staffIter = staffList.begin(); staffIter != staffList.end(); staffIter++) {
        system->SetCurrentFloatingPositioner((*staffIter)->GetN(), dynam, x, (*staffIter)->GetDrawingY());

        int y = dynam->GetDrawingY();

        dynamTxt.SetPointSize(m_doc->GetDrawingLyricFont((*staffIter)->m_drawingStaffSize)->GetPointSize());

        // If the dynamic is a symbol (pp, mf, etc.) draw it as one smufl string. This will not take into account
        // editorial element within the dynam as it would with text. Also, it is center only if it is a symbol.
        if (isSymbolOnly) {
            dc->SetFont(m_doc->GetDrawingSmuflFont((*staffIter)->m_drawingStaffSize, false));
            DrawSmuflString(dc, x, y, dynamSymbol, true, (*staffIter)->m_drawingStaffSize);
            dc->ResetFont();
        }
        else {
            dc->SetBrush(m_currentColour, AxSOLID);
            dc->SetFont(&dynamTxt);

            dc->StartText(ToDeviceContextX(x), ToDeviceContextY(y), LEFT);
            DrawTextChildren(dc, dynam, x, y, setX, setY);
            dc->EndText();

            dc->ResetFont();
            dc->ResetBrush();
        }
    }

    dc->EndGraphic(dynam, this);
}

void View::DrawFermata(DeviceContext *dc, Fermata *fermata, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(fermata);

    // We cannot draw a fermata that has no start position
    if (!fermata->GetStart()) return;

    dc->StartGraphic(fermata, "", fermata->GetUuid());

    int x = fermata->GetStart()->GetDrawingX();

    // for a start always put fermatas up
    int code = SMUFL_E4C0_fermataAbove;
    // check for shape
    if (fermata->GetShape() == fermataVis_SHAPE_angular) {
        if (fermata->GetForm() == fermataVis_FORM_inv
            || (fermata->GetPlace() == STAFFREL_below && !(fermata->GetForm() == fermataVis_FORM_norm)))
            code = SMUFL_E4C5_fermataShortBelow;
        else
            code = SMUFL_E4C4_fermataShortAbove;
    }
    else if (fermata->GetShape() == fermataVis_SHAPE_square) {
        if (fermata->GetForm() == fermataVis_FORM_inv
            || (fermata->GetPlace() == STAFFREL_below && !(fermata->GetForm() == fermataVis_FORM_norm)))
            code = SMUFL_E4C7_fermataLongBelow;
        else
            code = SMUFL_E4C6_fermataLongAbove;
    }
    else if (fermata->GetForm() == fermataVis_FORM_inv
        || (fermata->GetPlace() == STAFFREL_below && !(fermata->GetForm() == fermataVis_FORM_norm)))
        code = SMUFL_E4C1_fermataBelow;

    std::wstring str;
    str.push_back(code);

    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList = fermata->GetTstampStaves(measure);
    for (staffIter = staffList.begin(); staffIter != staffList.end(); staffIter++) {
        system->SetCurrentFloatingPositioner((*staffIter)->GetN(), fermata, x, (*staffIter)->GetDrawingY());
        int y = fermata->GetDrawingY();

        // Adjust the x position
        int drawingX = x - m_doc->GetGlyphWidth(code, (*staffIter)->m_drawingStaffSize, false) / 2;

        dc->SetFont(m_doc->GetDrawingSmuflFont((*staffIter)->m_drawingStaffSize, false));
        DrawSmuflString(dc, drawingX, y, str, false, (*staffIter)->m_drawingStaffSize);
        dc->ResetFont();
    }

    dc->EndGraphic(fermata, this);
}

void View::DrawHarm(DeviceContext *dc, Harm *harm, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(harm);

    // We cannot draw a harmony indication that has no start position
    if (!harm->GetStart()) return;

    dc->StartGraphic(harm, "", harm->GetUuid());

    FontInfo dirTxt;

    // If we have not timestamp
    int x = harm->GetStart()->GetDrawingX();

    bool setX = false;
    bool setY = false;

    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList = harm->GetTstampStaves(measure);
    for (staffIter = staffList.begin(); staffIter != staffList.end(); staffIter++) {
        system->SetCurrentFloatingPositioner((*staffIter)->GetN(), harm, x, (*staffIter)->GetDrawingY());

        int y = harm->GetDrawingY();

        dirTxt.SetPointSize(m_doc->GetDrawingLyricFont((*staffIter)->m_drawingStaffSize)->GetPointSize());

        dc->SetBrush(m_currentColour, AxSOLID);
        dc->SetFont(&dirTxt);

        dc->StartText(ToDeviceContextX(x), ToDeviceContextY(y), CENTER);
        DrawTextChildren(dc, harm, x, y, setX, setY);
        dc->EndText();

        dc->ResetFont();
        dc->ResetBrush();
    }

    dc->EndGraphic(harm, this);
}

void View::DrawPedal(DeviceContext *dc, Pedal *pedal, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(pedal);

    if (!pedal->GetStart()) return;

    dc->StartGraphic(pedal, "", pedal->GetUuid());

    int x = pedal->GetStart()->GetDrawingX();

    int code = SMUFL_E650_keyboardPedalPed;
    if (pedal->GetDir() == pedalLog_DIR_up) code = SMUFL_E655_keyboardPedalUp;
    std::wstring str;
    str.push_back(code);

    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList = pedal->GetTstampStaves(measure);
    for (staffIter = staffList.begin(); staffIter != staffList.end(); staffIter++) {
        system->SetCurrentFloatingPositioner((*staffIter)->GetN(), pedal, x, (*staffIter)->GetDrawingY());
        // Basic method that use bounding box
        int y = pedal->GetDrawingY();

        // Adjust the x position differently for up and down
        int drawingX = x;
        if (pedal->GetDir() == pedalLog_DIR_up)
            drawingX -= m_doc->GetGlyphWidth(SMUFL_E655_keyboardPedalUp, (*staffIter)->m_drawingStaffSize, false) / 2;
        else
            drawingX -= m_doc->GetGlyphWidth(SMUFL_E0A4_noteheadBlack, (*staffIter)->m_drawingStaffSize, false) / 2;

        dc->SetFont(m_doc->GetDrawingSmuflFont((*staffIter)->m_drawingStaffSize, false));
        DrawSmuflString(dc, drawingX, y, str, false, (*staffIter)->m_drawingStaffSize);
        dc->ResetFont();
    }

    dc->EndGraphic(pedal, this);
}

void View::DrawTempo(DeviceContext *dc, Tempo *tempo, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(tempo);

    dc->StartGraphic(tempo, "", tempo->GetUuid());

    FontInfo tempoTxt;

    // If we have not timestamp
    int x = measure->GetDrawingX();
    // First try to see if we have a meter sig attribute for this measure
    MeasureAlignerTypeComparison alignmentComparison(ALIGNMENT_SCOREDEF_METERSIG);
    Alignment *pos
        = dynamic_cast<Alignment *>(measure->m_measureAligner.FindChildByAttComparison(&alignmentComparison, 1));
    if (!pos) {
        // if not, try to get the first beat element
        alignmentComparison.SetType(ALIGNMENT_DEFAULT);
        pos = dynamic_cast<Alignment *>(measure->m_measureAligner.FindChildByAttComparison(&alignmentComparison, 1));
    }
    // if we found one, use it
    if (pos) {
        x += pos->GetXRel();
    }

    bool setX = false;
    bool setY = false;

    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList = tempo->GetTstampStaves(measure);
    for (staffIter = staffList.begin(); staffIter != staffList.end(); staffIter++) {
        system->SetCurrentFloatingPositioner((*staffIter)->GetN(), tempo, x, (*staffIter)->GetDrawingY());

        tempoTxt.SetPointSize(m_doc->GetDrawingLyricFont((*staffIter)->m_drawingStaffSize)->GetPointSize());

        int y = tempo->GetDrawingY();

        dc->SetBrush(m_currentColour, AxSOLID);
        dc->SetFont(&tempoTxt);

        dc->StartText(ToDeviceContextX(x), ToDeviceContextY(y), LEFT);
        DrawTextChildren(dc, tempo, x, y, setX, setY);
        dc->EndText();

        dc->ResetFont();
        dc->ResetBrush();
    }

    dc->EndGraphic(tempo, this);
}

void View::DrawTrill(DeviceContext *dc, Trill *trill, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(trill);
    
    // We cannot draw a fermata that has no start position
    if (!trill->GetStart()) return;
    
    dc->StartGraphic(trill, "", trill->GetUuid());
    
    int x = trill->GetStart()->GetDrawingX();
    
    // for a start always put trill up
    int code = SMUFL_E566_ornamentTrill;
    
    std::wstring str;
    str.push_back(code);
    
    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList = trill->GetTstampStaves(measure);
    for (staffIter = staffList.begin(); staffIter != staffList.end(); staffIter++) {
        system->SetCurrentFloatingPositioner((*staffIter)->GetN(), trill, x, (*staffIter)->GetDrawingY());
        int y = trill->GetDrawingY();
        
        // Adjust the x position
        int drawingX = x - m_doc->GetGlyphWidth(code, (*staffIter)->m_drawingStaffSize, false) / 2;
        
        dc->SetFont(m_doc->GetDrawingSmuflFont((*staffIter)->m_drawingStaffSize, false));
        DrawSmuflString(dc, drawingX, y, str, false, (*staffIter)->m_drawingStaffSize);
        dc->ResetFont();
    }
    
    dc->EndGraphic(trill, this);
}

//----------------------------------------------------------------------------
// View - FloatingObject - SystemElement
//----------------------------------------------------------------------------

void View::DrawSystemElement(DeviceContext *dc, SystemElement *element, System *system)
{
    assert(dc);
    assert(element);
    assert(system);

    if (element->Is() == BOUNDARY_END) {
        BoundaryEnd *boundaryEnd = dynamic_cast<BoundaryEnd *>(element);
        assert(boundaryEnd);
        assert(boundaryEnd->GetStart());
        dc->StartGraphic(element, boundaryEnd->GetStart()->GetUuid(), element->GetUuid());
        dc->EndGraphic(element, this);
    }
    else if (element->Is() == ENDING) {
        // Create placeholder - A graphic for the end boundary will be created
        // but only if it is on a different system - See View::DrawEnding
        // The Ending is added to the System drawing list by View::DrawMeasure
        dc->StartGraphic(element, "boundaryStart", element->GetUuid());
        dc->EndGraphic(element, this);
    }
    else if (element->Is() == PB) {
        dc->StartGraphic(element, "", element->GetUuid());
        dc->EndGraphic(element, this);
    }
    else if (element->Is() == SB) {
        dc->StartGraphic(element, "", element->GetUuid());
        dc->EndGraphic(element, this);
    }
    else if (element->Is() == SECTION) {
        dc->StartGraphic(element, "boundaryStart", element->GetUuid());
        dc->EndGraphic(element, this);
    }
}

void View::DrawEnding(DeviceContext *dc, Ending *ending, System *system)
{
    assert(dc);
    assert(ending);
    assert(system);

    if (dc->Is() == BBOX_DEVICE_CONTEXT) {
        BBoxDeviceContext *bBoxDC = dynamic_cast<BBoxDeviceContext *>(dc);
        assert(bBoxDC);
        if (!bBoxDC->UpdateVerticalValues()) {
            return;
        }
    }

    BoundaryEnd *endingEndBoundary = ending->GetEnd();

    // We need to make sure we have the end boudary and a measure (first and last) in each of them
    assert(endingEndBoundary);
    assert(ending->GetMeasure() && endingEndBoundary->GetMeasure());
    // in non debug mode
    if (!endingEndBoundary) return;
    if (!ending->GetMeasure() || !endingEndBoundary->GetMeasure()) return;

    // Get the parent system of the first and last note
    System *parentSystem1 = dynamic_cast<System *>(ending->GetFirstParent(SYSTEM));
    System *parentSystem2 = dynamic_cast<System *>(endingEndBoundary->GetFirstParent(SYSTEM));

    assert(parentSystem1 && parentSystem2);
    // in non debug mode
    if (!parentSystem1 || !parentSystem2) return;

    int x1, x2;
    Measure *measure = NULL;
    char spanningType = SPANNING_START_END;

    // The both correspond to the current system, which means no system break in-between (simple case)
    if ((system == parentSystem1) && (system == parentSystem2)) {
        measure = ending->GetMeasure();
        x1 = measure->GetDrawingX();
        // if it is the first measure of the system use the left barline position
        if (system->GetFirst(MEASURE) == measure) x1 += measure->GetLeftBarLineXRel();
        x2 = endingEndBoundary->GetMeasure()->GetDrawingX() + endingEndBoundary->GetMeasure()->GetRightBarLineXRel();
    }
    // Only the first parent is the same, this means that the ending is "open" at the end of the system
    else if (system == parentSystem1) {
        // We need the last measure of the system for x2 - we also use it for getting the staves later
        measure = dynamic_cast<Measure *>(system->FindChildByType(MEASURE, 1, BACKWARD));
        if (!Check(measure)) return;
        x1 = ending->GetMeasure()->GetDrawingX();
        // if it is the first measure of the system use the left barline position
        if (system->GetFirst(MEASURE) == ending->GetMeasure()) x1 += ending->GetMeasure()->GetLeftBarLineXRel();
        x2 = measure->GetDrawingX() + measure->GetRightBarLineXRel();
        spanningType = SPANNING_START;
    }
    // We are in the system where the ending ends - draw it from the beginning of the system
    else if (system == parentSystem2) {
        // We need the last measure of the system for x2
        measure = dynamic_cast<Measure *>(system->FindChildByType(MEASURE, 1, FORWARD));
        if (!Check(measure)) return;
        x1 = measure->GetDrawingX() + measure->GetLeftBarLineXRel();
        x2 = endingEndBoundary->GetMeasure()->GetDrawingX() + endingEndBoundary->GetMeasure()->GetRightBarLineXRel();
        spanningType = SPANNING_END;
    }
    // Rare case where neither the first note nor the last note are in the current system - draw the connector
    // throughout the system
    else {
        // We need the first measure of the system for x1 - we also use it for getting the staves later
        measure = dynamic_cast<Measure *>(system->FindChildByType(MEASURE, 1, FORWARD));
        if (!Check(measure)) return;
        x1 = measure->GetDrawingX() + measure->GetLeftBarLineXRel();
        // We need the last measure of the system for x2
        measure = dynamic_cast<Measure *>(system->FindChildByType(MEASURE, 1, BACKWARD));
        if (!Check(measure)) return;
        x2 = measure->GetDrawingX() + measure->GetRightBarLineXRel();
        spanningType = SPANNING_MIDDLE;
    }

    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START))
        dc->ResumeGraphic(ending, ending->GetUuid());
    else
        dc->StartGraphic(ending, "spanning-ending", "");

    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList;
    assert(system->GetDrawingScoreDef());
    assert(measure);
    // By default, endings are drawn on top of each group (@ending.rend="gouped") unless "top" is specified)
    if (system->GetDrawingScoreDef()->GetEndingRend() == endings_ENDINGREND_top) {
        Staff *staff = dynamic_cast<Staff *>(system->FindChildByType(STAFF, 2, FORWARD));
        if (!Check(staff)) return;
        staffList.push_back(staff);
    }
    else {
        staffList = measure->GetFirstStaffGrpStaves(system->GetDrawingScoreDef());
    }

    for (staffIter = staffList.begin(); staffIter != staffList.end(); staffIter++) {
        system->SetCurrentFloatingPositioner((*staffIter)->GetN(), ending, x1, (*staffIter)->GetDrawingY());

        int y1 = ending->GetDrawingY();

        FontInfo currentFont = *m_doc->GetDrawingLyricFont((*staffIter)->m_drawingStaffSize);
        // currentFont.SetWeight(FONTWEIGHT_bold);
        // currentFont.SetPointSize(currentFont.GetPointSize() * 2 / 3);
        dc->SetFont(&currentFont);

        TextExtend extend;
        dc->GetTextExtent("M", &extend);

        if (ending->HasN()) {
            std::wstringstream strStream;
            // Maybe we want to add ( ) after system breaks?
            if ((spanningType == SPANNING_END) || (spanningType == SPANNING_MIDDLE)) strStream << L"(";
            strStream << ending->GetN() << L".";
            if ((spanningType == SPANNING_END) || (spanningType == SPANNING_MIDDLE)) strStream << L")";

            Text text;
            text.SetText(strStream.str());

            bool setX = false;
            bool setY = false;

            int textX = x1;
            if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START)) {
                textX += m_doc->GetDrawingUnit((*staffIter)->m_drawingStaffSize) * 2 / 3;
            }
            dc->StartText(ToDeviceContextX(textX), ToDeviceContextY(y1), LEFT);
            DrawTextElement(dc, &text, textX, y1, setX, setY);
            dc->EndText();
        }

        dc->ResetFont();

        int y2 = y1 + extend.m_height + m_doc->GetDrawingUnit((*staffIter)->m_drawingStaffSize) * 2 / 3;

        DrawFilledRectangle(dc, x1, y2, x2, y2 + m_doc->GetDrawingBarLineWidth((*staffIter)->m_drawingStaffSize));
        if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START)) {
            DrawFilledRectangle(dc, x1, y1, x1 + m_doc->GetDrawingBarLineWidth((*staffIter)->m_drawingStaffSize), y2);
        }
        if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_END)) {
            DrawFilledRectangle(dc, x2 - m_doc->GetDrawingBarLineWidth((*staffIter)->m_drawingStaffSize), y1, x2, y2);
        }
    }

    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START))
        dc->EndResumedGraphic(ending, this);
    else
        dc->EndGraphic(ending, this);
}

} // namespace vrv
