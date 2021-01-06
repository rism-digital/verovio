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
#include <sstream>

//----------------------------------------------------------------------------

#include "arpeg.h"
#include "bboxdevicecontext.h"
#include "bracketspan.h"
#include "breath.h"
#include "comparison.h"
#include "devicecontext.h"
#include "dir.h"
#include "doc.h"
#include "dynam.h"
#include "ending.h"
#include "f.h"
#include "fb.h"
#include "fermata.h"
#include "fing.h"
#include "functorparams.h"
#include "gliss.h"
#include "hairpin.h"
#include "harm.h"
#include "layer.h"
#include "layerelement.h"
#include "measure.h"
#include "mordent.h"
#include "note.h"
#include "octave.h"
#include "options.h"
#include "pedal.h"
#include "reh.h"
#include "rend.h"
#include "slur.h"
#include "smufl.h"
#include "staff.h"
#include "syl.h"
#include "system.h"
#include "tempo.h"
#include "text.h"
#include "textelement.h"
#include "tie.h"
#include "timeinterface.h"
#include "timestamp.h"
#include "trill.h"
#include "turn.h"
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

    // For dir, dynam, fermata, and harm, we do not consider the @tstamp2 for rendering
    if (element->Is({ BRACKETSPAN, FIGURE, GLISS, HAIRPIN, PHRASE, OCTAVE, SLUR, TIE })) {
        // create placeholder
        dc->StartGraphic(element, "", element->GetUuid());
        dc->EndGraphic(element, this);
        system->AddToDrawingList(element);
    }
    else if (element->Is(ARPEG)) {
        Arpeg *arpeg = vrv_cast<Arpeg *>(element);
        assert(arpeg);
        DrawArpeg(dc, arpeg, measure, system);
    }
    else if (element->Is(BREATH)) {
        Breath *breath = vrv_cast<Breath *>(element);
        assert(breath);
        DrawBreath(dc, breath, measure, system);
    }
    else if (element->Is(DIR)) {
        Dir *dir = vrv_cast<Dir *>(element);
        assert(dir);
        DrawDir(dc, dir, measure, system);
        system->AddToDrawingListIfNeccessary(dir);
    }
    else if (element->Is(DYNAM)) {
        Dynam *dynam = vrv_cast<Dynam *>(element);
        assert(dynam);
        DrawDynam(dc, dynam, measure, system);
        system->AddToDrawingListIfNeccessary(dynam);
    }
    else if (element->Is(FERMATA)) {
        Fermata *fermata = vrv_cast<Fermata *>(element);
        assert(fermata);
        DrawFermata(dc, fermata, measure, system);
    }
    else if (element->Is(FING)) {
        Fing *fing = vrv_cast<Fing *>(element);
        assert(fing);
        DrawFing(dc, fing, measure, system);
    }
    else if (element->Is(HARM)) {
        Harm *harm = vrv_cast<Harm *>(element);
        assert(harm);
        DrawHarm(dc, harm, measure, system);
    }
    else if (element->Is(MORDENT)) {
        Mordent *mordent = vrv_cast<Mordent *>(element);
        assert(mordent);
        DrawMordent(dc, mordent, measure, system);
    }
    else if (element->Is(PEDAL)) {
        Pedal *pedal = vrv_cast<Pedal *>(element);
        assert(pedal);
        DrawPedal(dc, pedal, measure, system);
        system->AddToDrawingListIfNeccessary(pedal);
    }
    else if (element->Is(REH)) {
        Reh *reh = vrv_cast<Reh *>(element);
        assert(reh);
        DrawReh(dc, reh, measure, system);
    }
    else if (element->Is(TEMPO)) {
        Tempo *tempo = vrv_cast<Tempo *>(element);
        assert(tempo);
        DrawTempo(dc, tempo, measure, system);
    }
    else if (element->Is(TRILL)) {
        Trill *trill = vrv_cast<Trill *>(element);
        assert(trill);
        DrawTrill(dc, trill, measure, system);
        system->AddToDrawingListIfNeccessary(trill);
    }
    else if (element->Is(TURN)) {
        Turn *turn = vrv_cast<Turn *>(element);
        assert(turn);
        DrawTurn(dc, turn, measure, system);
    }
}

void View::DrawTimeSpanningElement(DeviceContext *dc, Object *element, System *system)
{
    assert(dc);
    assert(element);
    assert(system);

    if (dc->Is(BBOX_DEVICE_CONTEXT)) {
        BBoxDeviceContext *bBoxDC = vrv_cast<BBoxDeviceContext *>(dc);
        assert(bBoxDC);
        if (!bBoxDC->UpdateVerticalValues()) {
            if (element->Is({ BRACKETSPAN, HAIRPIN, PHRASE, OCTAVE, SLUR, TIE })) return;
        }
    }

    TimeSpanningInterface *interface = element->GetTimeSpanningInterface();
    assert(interface);

    // The start is given by the TimePointInterface
    LayerElement *start = dynamic_cast<LayerElement *>(interface->GetStart());
    // The end is given either by the TimeSpanningInterface (end) or by the LinkingInterface (next)
    LayerElement *end = NULL;
    if (interface->GetEnd()) {
        end = dynamic_cast<LayerElement *>(interface->GetEnd());
    }
    else if (element->HasInterface(INTERFACE_LINKING)) {
        LinkingInterface *linkingInterface = element->GetLinkingInterface();
        assert(linkingInterface);
        if (linkingInterface->GetNextLink()) {
            // The end link has to have a TimePointInterface becase we ensure it is a ControlElement
            TimePointInterface *nextInterface = linkingInterface->GetNextLink()->GetTimePointInterface();
            assert(nextInterface);
            end = nextInterface->GetStart();
        }
    }
    if (!start || !end) return;

    // Get the parent system of the first and last note
    System *parentSystem1 = dynamic_cast<System *>(start->GetFirstAncestor(SYSTEM));
    System *parentSystem2 = dynamic_cast<System *>(end->GetFirstAncestor(SYSTEM));

    int x1, x2;
    Object *objectX = NULL;
    Measure *measure = NULL;
    Object *graphic = NULL;
    char spanningType = SPANNING_START_END;

    // They both correspond to the current system, which means no system break in-between (simple case)
    if ((system == parentSystem1) && (system == parentSystem2)) {
        // we use the start measure
        measure = interface->GetStartMeasure();
        if (!Check(measure)) return;
        x1 = start->GetDrawingX();
        objectX = start;
        x2 = end->GetDrawingX();
        graphic = element;
    }
    // Only the first parent is the same, this means that the element is "open" at the end of the system
    else if (system == parentSystem1) {
        // We need the last measure of the system for x2 - we also use it for getting the staves later
        measure = dynamic_cast<Measure *>(system->FindDescendantByType(MEASURE, 1, BACKWARD));
        if (!Check(measure)) return;
        x1 = start->GetDrawingX();
        objectX = start;
        x2 = measure->GetDrawingX() + measure->GetRightBarLineXRel();
        graphic = element;
        spanningType = SPANNING_START;
    }
    // We are in the system of the last note - draw the element from the beginning of the system
    else if (system == parentSystem2) {
        // We need the first measure of the system for x1 - we also use it for getting the staves later
        measure = dynamic_cast<Measure *>(system->FindDescendantByType(MEASURE, 1, FORWARD));
        if (!Check(measure)) return;
        // We need the position of the first default in the first measure for x1
        x1 = measure->GetDrawingX() + measure->GetLeftBarLineXRel();
        objectX = measure->GetLeftBarLine();
        x2 = end->GetDrawingX();
        spanningType = SPANNING_END;
    }
    // Rare case where neither the first note nor the last note are in the current system - draw the connector
    // throughout the system
    else {
        // We need the first measure of the system for x1 - we also use it for getting the staves later
        measure = dynamic_cast<Measure *>(system->FindDescendantByType(MEASURE, 1, FORWARD));
        if (!Check(measure)) return;
        // We need the position of the first default in the first measure for x1
        x1 = measure->GetDrawingX() + measure->GetLeftBarLineXRel();
        objectX = measure->GetLeftBarLine();
        // We need the last measure of the system for x2
        Measure *last = dynamic_cast<Measure *>(system->FindDescendantByType(MEASURE, 1, BACKWARD));
        if (!Check(last)) return;
        x2 = last->GetDrawingX() + last->GetRightBarLineXRel();
        spanningType = SPANNING_MIDDLE;
    }

    // Overwrite the spanningType for open ended control events
    // We can identify them because they end on a right barline attribute
    if ((spanningType == SPANNING_START_END) && end->Is(BARLINE_ATTR_RIGHT)) {
        spanningType = SPANNING_START;
    }

    int startRadius = 0;
    if (!start->Is(TIMESTAMP_ATTR)) {
        startRadius = start->GetDrawingRadius(m_doc);
    }

    int endRadius = 0;
    if (!end->Is(TIMESTAMP_ATTR)) {
        endRadius = end->GetDrawingRadius(m_doc);
    }

    /************** adjust the position according to the radius **************/

    if (spanningType == SPANNING_START_END) {
        x1 += startRadius;
        x2 += endRadius;
    }
    else if (spanningType == SPANNING_START) {
        x1 += startRadius;
    }
    else if (spanningType == SPANNING_END) {
        x2 += endRadius;
    }

    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList = interface->GetTstampStaves(measure, element);
    for (staffIter = staffList.begin(); staffIter != staffList.end(); ++staffIter) {

        // TimeSpanning element are not necessary floating elements (e.g., syl) - we have a bounding box only for them
        if (element->IsControlElement()) {
            if (!system->SetCurrentFloatingPositioner(
                    (*staffIter)->GetN(), dynamic_cast<ControlElement *>(element), objectX, *staffIter, spanningType)) {
                continue;
            }
        }

        if (element->Is(DIR)) {
            // cast to Dir check in DrawControlElementConnector
            DrawControlElementConnector(dc, dynamic_cast<Dir *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is(DYNAM)) {
            // cast to Dynam check in DrawControlElementConnector
            DrawControlElementConnector(dc, dynamic_cast<Dynam *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is(FIGURE)) {
            // cast to F check in DrawFConnector
            DrawFConnector(dc, dynamic_cast<F *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is(BRACKETSPAN)) {
            // cast to BracketSpan check in DrawBracketSpan
            DrawBracketSpan(dc, dynamic_cast<BracketSpan *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is(GLISS)) {
            // For gliss we limit support to one value in @staff
            if (staffIter != staffList.begin()) continue;
            // cast to Gliss check in DrawGliss
            DrawGliss(dc, dynamic_cast<Gliss *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is(HAIRPIN)) {
            // cast to Harprin check in DrawHairpin
            DrawHairpin(dc, dynamic_cast<Hairpin *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is(PHRASE)) {
            // For phrases (slurs) we limit support to one value in @staff
            if (staffIter != staffList.begin()) continue;
            // cast to Slur check in DrawSlur
            DrawSlur(dc, dynamic_cast<Slur *>(element), x1, x2, *staffIter, spanningType, graphic);
        }

        else if (element->Is(OCTAVE)) {
            // cast to Slur check in DrawOctave
            DrawOctave(dc, dynamic_cast<Octave *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is(PEDAL)) {
            DrawPedalLine(dc, dynamic_cast<Pedal *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is(SLUR)) {
            // For slurs we limit support to one value in @staff
            if (staffIter != staffList.begin()) continue;
            // cast to Slur check in DrawSlur
            DrawSlur(dc, dynamic_cast<Slur *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is(SYL)) {
            // prolong to the end of the notehead
            x2 += endRadius;
            // cast to Syl check in DrawSylConnector
            DrawSylConnector(dc, dynamic_cast<Syl *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is(TIE)) {
            // For ties we limit support to one value in @staff
            if (staffIter != staffList.begin()) continue;
            // cast to Slur check in DrawTie
            DrawTie(dc, dynamic_cast<Tie *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is(TRILL)) {
            // cast to Trill check in DrawTrill
            DrawTrillExtension(dc, dynamic_cast<Trill *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
    }
}

void View::DrawBracketSpan(
    DeviceContext *dc, BracketSpan *bracketSpan, int x1, int x2, Staff *staff, char spanningType, Object *graphic)
{
    assert(dc);
    assert(bracketSpan);
    assert(staff);

    assert(bracketSpan->GetStart());
    assert(bracketSpan->GetEnd());

    if (!bracketSpan->HasFunc()) {
        // we cannot draw a bracketSpan that has no func
        return;
    }

    int y = bracketSpan->GetDrawingY();

    int startRadius = 0;
    if (!bracketSpan->GetStart()->Is(TIMESTAMP_ATTR)) {
        startRadius = bracketSpan->GetStart()->GetDrawingRadius(m_doc);
    }

    int endRadius = 0;
    if (!bracketSpan->GetEnd()->Is(TIMESTAMP_ATTR)) {
        endRadius = bracketSpan->GetEnd()->GetDrawingRadius(m_doc);
    }

    // The both correspond to the current system, which means no system break in-between (simple case)
    if (spanningType == SPANNING_START_END) {
        x1 -= startRadius;
        x2 += endRadius;
    }
    // Only the first parent is the same, this means that the syl is "open" at the end of the system
    else if (spanningType == SPANNING_START) {
        x1 -= startRadius;
    }
    // We are in the system of the last note - draw the connector from the beginning of the system
    else if (spanningType == SPANNING_END) {
        x2 += endRadius;
    }
    else {
        // nothing to adjust
    }

    if (graphic) {
        dc->ResumeGraphic(graphic, graphic->GetUuid());
    }
    else {
        dc->StartGraphic(bracketSpan, "", bracketSpan->GetUuid(), false);
    }

    int bracketSize = 2 * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    int lineWidth = m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);

    if (bracketSpan->HasLwidth()) {
        if (bracketSpan->GetLwidth().GetType() == LINEWIDTHTYPE_lineWidthTerm) {
            if (bracketSpan->GetLwidth().GetLineWithTerm() == LINEWIDTHTERM_narrow) {
                lineWidth *= LINEWIDTHTERM_factor_narrow;
            }
            else if (bracketSpan->GetLwidth().GetLineWithTerm() == LINEWIDTHTERM_medium) {
                lineWidth *= LINEWIDTHTERM_factor_medium;
            }
            else if (bracketSpan->GetLwidth().GetLineWithTerm() == LINEWIDTHTERM_wide) {
                lineWidth *= LINEWIDTHTERM_factor_wide;
            }
        }
        else if (bracketSpan->GetLwidth().GetType() == LINEWIDTHTYPE_measurementAbs) {
            if (bracketSpan->GetLwidth().GetMeasurementAbs() != VRV_UNSET) {
                lineWidth
                    = bracketSpan->GetLwidth().GetMeasurementAbs() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
            }
        }
    }

    // Opening bracket
    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START)) {
        // Do not draw the horizontal line if the lines is dashed or solid as a full line will be drawn below
        // (Do draw the horizontal line for dotted lines as it looks better)
        if ((bracketSpan->GetLform() != LINEFORM_dashed) && (bracketSpan->GetLform() != LINEFORM_solid)) {
            DrawFilledRectangle(dc, x1, y, x1 + bracketSize, y + lineWidth);
        }
        DrawFilledRectangle(dc, x1, y, x1 + lineWidth, y - bracketSize);
    }
    // Closing bracket
    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_END)) {
        // Ditto
        if ((bracketSpan->GetLform() != LINEFORM_dashed) && (bracketSpan->GetLform() != LINEFORM_solid)) {
            DrawFilledRectangle(dc, x2 - bracketSize, y, x2, y + lineWidth);
        }
        DrawFilledRectangle(dc, x2 - lineWidth, y, x2, y - bracketSize);
    }
    // We have a @lform - draw a full line
    if (bracketSpan->HasLform()) {
        if (bracketSpan->GetLform() == LINEFORM_solid) {
            DrawFilledRectangle(dc, x1, y, x2, y - lineWidth);
        }
        else if (bracketSpan->GetLform() == LINEFORM_dashed) {
            dc->SetPen(m_currentColour, lineWidth, AxSOLID, bracketSize);
            dc->SetBrush(m_currentColour, AxSOLID);
            int yDotted = y + lineWidth / 2;
            dc->DrawLine(
                ToDeviceContextX(x1), ToDeviceContextY(yDotted), ToDeviceContextX(x2), ToDeviceContextY(yDotted));
            dc->ResetPen();
            dc->ResetBrush();
        }
        else if (bracketSpan->GetLform() == LINEFORM_dotted) {
            dc->SetPen(m_currentColour, lineWidth, AxDOT, lineWidth, 1);
            dc->SetBrush(m_currentColour, AxSOLID);
            // Adjust the start and end because the horizontal line of the was drawn in that case
            int x1Dotted
                = ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START)) ? x1 + bracketSize : x1;
            int x2Dotted
                = ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_END)) ? x2 - bracketSize : x2;
            int yDotted = y + lineWidth / 2;
            dc->DrawLine(ToDeviceContextX(x1Dotted + 1.5 * lineWidth), ToDeviceContextY(yDotted),
                ToDeviceContextX(x2Dotted), ToDeviceContextY(yDotted));
            dc->ResetPen();
            dc->ResetBrush();
        }
    }

    if (graphic) {
        dc->EndResumedGraphic(graphic, this);
    }
    else {
        dc->EndGraphic(bracketSpan, this);
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

    FloatingPositioner *leftLink = hairpin->GetCorrespFloatingPositioner(hairpin->GetLeftLink());
    FloatingPositioner *rightLink = hairpin->GetCorrespFloatingPositioner(hairpin->GetRightLink());

    int adjustedX1 = x1;
    if (leftLink) {
        adjustedX1 = leftLink->GetContentRight() + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2;
    }
    int adjustedX2 = x2;
    if (rightLink) {
        adjustedX2 = rightLink->GetContentLeft() - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2;
    }

    // Beginning of a system, very short hairpin needs to be push left
    if (spanningType == SPANNING_END) {
        if ((adjustedX2 - adjustedX1) < (m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2)) {
            adjustedX1 = adjustedX2 - 2 * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        }
    }

    // In any case, a hairpin should not be shorter than 2 units.
    // If shorter, with groups, this will screw up vertical alignment and push everything down - to be improved by
    // deactivating grp?
    if ((adjustedX2 - adjustedX1) >= m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2) {
        x1 = adjustedX1;
        x2 = adjustedX2;
    }

    // Store the full drawing length
    if (spanningType == SPANNING_START_END) {
        const auto [leftOverlap, rightOverlap]
            = hairpin->GetBarlineOverlapAdjustment(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize), x1, x2);
        x1 += leftOverlap;
        x2 -= rightOverlap;

        hairpin->SetDrawingLength(x2 - x1);
    }

    hairpinLog_FORM form = hairpin->GetForm();

    int startY = 0;
    int endY = hairpin->CalcHeight(m_doc, staff->m_drawingStaffSize, spanningType, leftLink, rightLink);

    m_doc->GetDrawingHairpinSize(staff->m_drawingStaffSize, false);

    // We calculate points for cresc by default. Start/End have to be swapped
    if (form == hairpinLog_FORM_dim) BoundingBox::Swap(startY, endY);

    int y1 = hairpin->GetDrawingY();
    if (hairpin->GetPlace() != STAFFREL_between) {
        y1 += m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    }
    int y2 = y1;

    /************** start / end opening **************/

    if (form == hairpinLog_FORM_cres) {
        // the normal case
        if (spanningType == SPANNING_START_END) {
            // nothing to adjust
        }
        // In this case, we are drawing the first half a a cresc. Reduce the opening end
        else if (spanningType == SPANNING_START) {
            endY = endY / 2;
        }
        // Now this is the case we are drawing the end of a cresc. Increase the opening start
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
        // In this case, we are drawing the first half a a dim. Increase the opening end
        else if (spanningType == SPANNING_START) {
            endY = startY / 2;
        }
        // Now this is the case we are drawing the end of a dim. Reduce the opening start
        else if (spanningType == SPANNING_END) {
            startY = startY / 2;
        }
        // Finally, dim accross the system, reduce the start and increase the end
        else {
            endY = m_doc->GetDrawingHairpinSize(staff->m_drawingStaffSize, false) / 3;
            startY = 2 * endY;
        }
    }

    /************** draw it **************/

    y1 -= m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2;
    y2 -= m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2;

    if (graphic)
        dc->ResumeGraphic(graphic, graphic->GetUuid());
    else
        dc->StartGraphic(hairpin, "", hairpin->GetUuid(), false);
    // dc->DeactivateGraphic();

    const double hairpinThickness
        = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * m_options->m_hairpinThickness.GetValue();
    DrawObliquePolygon(dc, x1, y1 - startY / 2, x2, y2 - endY / 2, hairpinThickness);
    DrawObliquePolygon(dc, x1, y1 + startY / 2, x2, y2 + endY / 2, hairpinThickness);

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

    data_OCTAVE_DIS dis = octave->GetDis();
    data_STAFFREL_basic disPlace = octave->GetDisPlace();

    int y1 = octave->GetDrawingY();
    int y2 = y1;

    /********** adjust the start / end positions ***********/

    if ((spanningType == SPANNING_END) || (spanningType == SPANNING_MIDDLE)) {
        x1 += (m_doc->GetGlyphWidth(SMUFL_E0A2_noteheadWhole, staff->m_drawingStaffSize, false) / 2);
    }

    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_END)) {
        if (octave->HasEndid()) {
            x2 += (m_doc->GetGlyphWidth(SMUFL_E0A2_noteheadWhole, staff->m_drawingStaffSize, false) / 2);
        }
    }

    /************** draw it **************/

    if (graphic)
        dc->ResumeGraphic(graphic, graphic->GetUuid());
    else
        dc->StartGraphic(octave, "", octave->GetUuid(), false);

    int code = SMUFL_E511_ottavaAlta;
    if (disPlace == STAFFREL_basic_above) {
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
    std::wstring str;
    str.push_back(code);

    if (octave->GetExtender() != BOOLEAN_false) {
        int lineWidth = m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
        if (octave->HasLwidth()) {
            if (octave->GetLwidth().GetType() == LINEWIDTHTYPE_lineWidthTerm) {
                if (octave->GetLwidth().GetLineWithTerm() == LINEWIDTHTERM_narrow) {
                    lineWidth *= LINEWIDTHTERM_factor_narrow;
                }
                else if (octave->GetLwidth().GetLineWithTerm() == LINEWIDTHTERM_medium) {
                    lineWidth *= LINEWIDTHTERM_factor_medium;
                }
                else if (octave->GetLwidth().GetLineWithTerm() == LINEWIDTHTERM_wide) {
                    lineWidth *= LINEWIDTHTERM_factor_wide;
                }
            }
            else if (octave->GetLwidth().GetType() == LINEWIDTHTYPE_measurementAbs) {
                if (octave->GetLwidth().GetMeasurementAbs() != VRV_UNSET) {
                    lineWidth
                        = octave->GetLwidth().GetMeasurementAbs() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
                }
            }
        }
        dc->SetFont(m_doc->GetDrawingSmuflFont(staff->m_drawingStaffSize, false));
        TextExtend extend;
        dc->GetSmuflTextExtent(str, &extend);
        const int yCode = (disPlace == STAFFREL_basic_above) ? y1 - extend.m_height : y1;
        DrawSmuflCode(dc, x1 - extend.m_width, yCode, code, staff->m_drawingStaffSize, false);
        dc->ResetFont();

        if (octave->GetLendsym() != LINESTARTENDSYMBOL_none)
            y2 += (disPlace == STAFFREL_basic_above) ? -extend.m_height : extend.m_height;
        // adjust is to avoid the figure to touch the line
        x1 += m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);

        if (octave->HasLform()) {
            if (octave->GetLform() == LINEFORM_solid) {
                extend.m_height *= 0;
            }
        }

        dc->SetPen(m_currentColour, lineWidth, AxSOLID, extend.m_height / 3);
        dc->SetBrush(m_currentColour, AxSOLID);

        dc->DrawLine(ToDeviceContextX(x1), ToDeviceContextY(y1), ToDeviceContextX(x2), ToDeviceContextY(y1));
        // draw the ending vertical line if not the end of the system
        if (spanningType == SPANNING_END || spanningType == SPANNING_START_END) {
            dc->DrawLine(ToDeviceContextX(x2), ToDeviceContextY(y1 + lineWidth / 2), ToDeviceContextX(x2),
                ToDeviceContextY(y2 + lineWidth / 2));
        }

        dc->ResetPen();
        dc->ResetBrush();
    }

    if (graphic)
        dc->EndResumedGraphic(graphic, this);
    else
        dc->EndGraphic(octave, this);
}

void View::DrawTie(DeviceContext *dc, Tie *tie, int x1, int x2, Staff *staff, char spanningType, Object *graphic)
{
    assert(dc);
    assert(tie);
    assert(staff);

    curvature_CURVEDIR drawingCurveDir = curvature_CURVEDIR_above;
    data_STEMDIRECTION noteStemDir = STEMDIRECTION_NONE;
    int y1, y2;
    int r1 = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    int r2 = r1;

    /************** parent layers **************/

    Note *note1 = dynamic_cast<Note *>(tie->GetStart());
    Note *note2 = dynamic_cast<Note *>(tie->GetEnd());

    if (!note1 && !note2) {
        // no note, obviously nothing to do...
        // this also means that notes with tstamp events are not supported
        return;
    }

    LayerElement *durElement = NULL;
    Chord *parentChord1 = NULL;
    Layer *layer1 = NULL;
    if (note1) {
        durElement = note1;
        layer1 = dynamic_cast<Layer *>(note1->GetFirstAncestor(LAYER));
        if (note1->m_crossStaff) layer1 = note1->m_crossLayer;
        parentChord1 = note1->IsChordTone();
    }
    if (parentChord1) {
        durElement = parentChord1;
        if (parentChord1->m_crossStaff) layer1 = parentChord1->m_crossLayer;
    }

    /************** x positions **************/

    bool isShortTie = false;
    // shortTie correction cannot be applied for chords
    if (!parentChord1 && (x2 - x1 < 3 * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize))) {
        isShortTie = true;
    }

    y1 = staff->GetDrawingY();
    y2 = staff->GetDrawingY();

    // the normal case
    if (spanningType == SPANNING_START_END) {
        if (note1) {
            y1 = note1->GetDrawingY();
            y2 = y1;
            noteStemDir = note1->GetDrawingStemDir();
        }
        else if (note2) {
            y2 = note2->GetDrawingY();
            y1 = y2;
        }
        // isShort is never true with tstamp1
        if (!isShortTie) {
            if (note1) r1 = note1->GetDrawingRadius(m_doc);
            if (note2) r2 = note2->GetDrawingRadius(m_doc);
            x1 += r1 + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2;
            x2 -= r2 + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2;
            if (note1 && note1->GetDots() > 0) {
                x1 += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * note1->GetDots() * 3 / 2;
            }
            else if (parentChord1 && (parentChord1->GetDots() > 0)) {
                x1 += m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * parentChord1->GetDots();
            }
        }
    }
    // This is the case when the tie is split over two system of two pages.
    // In this case, we are now drawing its beginning to the end of the measure (i.e., the last aligner)
    else if (spanningType == SPANNING_START) {
        if (note1) {
            y1 = note1->GetDrawingY();
            y2 = y1;
            r1 = note1->GetDrawingRadius(m_doc);
            noteStemDir = note1->GetDrawingStemDir();
        }
        if (!isShortTie) {
            x1 += r1 + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2;
            if (note1 && note1->GetDots() > 0) {
                x1 += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * note1->GetDots() * 3 / 2;
            }
            else if (parentChord1 && (parentChord1->GetDots() > 0)) {
                x1 += m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * parentChord1->GetDots();
            }
        }
        x2 -= (m_doc->GetDrawingUnit(staff->m_drawingStaffSize)
                  + m_doc->GetDrawingBarLineWidth(staff->m_drawingStaffSize))
            / 2;
    }
    // Now this is the case when the tie is split but we are drawing the end of it
    else if (spanningType == SPANNING_END) {
        if (note2) {
            y2 = note2->GetDrawingY();
            y1 = y2;
            r2 = note2->GetDrawingRadius(m_doc);
            noteStemDir = note2->GetDrawingStemDir();
        }
        if (!isShortTie) {
            x2 -= r2 + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2;
        }
    }
    // Finally - this make no sense ?
    else {
        LogDebug("Tie across an entire system is not supported");
        return;
    }

    /************** direction **************/

    data_STEMDIRECTION layerStemDir;

    // first should be the tie @curvedir
    if (tie->HasCurvedir()) {
        drawingCurveDir
            = (tie->GetCurvedir() == curvature_CURVEDIR_above) ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
    }
    // then layer direction trumps note direction
    else if (layer1 && ((layerStemDir = layer1->GetDrawingStemDir(note1)) != STEMDIRECTION_NONE)) {
        drawingCurveDir = (layerStemDir == STEMDIRECTION_up) ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
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
    int height = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    // if the space between the to points is more than two staff height, increase the height
    if (x2 - x1 > 2 * m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize)) {
        height += m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    }
    int thickness = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * m_options->m_tieMidpointThickness.GetValue();

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
    FloatingPositioner *positioner = tie->GetCurrentFloatingPositioner();
    assert(positioner && positioner->Is(FLOATING_CURVE_POSITIONER));
    FloatingCurvePositioner *curve = vrv_cast<FloatingCurvePositioner *>(positioner);
    assert(curve);
    curve->UpdateCurveParams(bezier, 0.0, thickness, drawingCurveDir);

    int penStyle = AxSOLID;
    switch (tie->GetLform()) {
        case LINEFORM_dashed: penStyle = AxSHORT_DASH; break;
        case LINEFORM_dotted: penStyle = AxDOT; break;
        default: break;
    }

    if (graphic)
        dc->ResumeGraphic(graphic, graphic->GetUuid());
    else
        dc->StartGraphic(tie, "", tie->GetUuid(), false);
    // set pen width and calculate tie thickness coeficient to adjust tie width in according to it
    const int penWidth
        = m_doc->GetOptions()->m_tieEndpointThickness.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    if (m_tieThicknessCoeficient <= 0) {
        m_tieThicknessCoeficient = BoundingBox::GetBezierThicknessCoeficient(bezier, thickness, 0, penWidth);
    }
    DrawThickBezierCurve(
        dc, bezier, m_tieThicknessCoeficient * thickness, staff->m_drawingStaffSize, penWidth, 0, penStyle);
    if (graphic)
        dc->EndResumedGraphic(graphic, this);
    else
        dc->EndGraphic(tie, this);
}

void View::DrawPedalLine(
    DeviceContext *dc, Pedal *pedal, int x1, int x2, Staff *staff, char spanningType, Object *graphic)
{
    assert(dc);
    assert(pedal);
    assert(staff);

    assert(pedal->GetStart());
    assert(pedal->GetEnd());

    int y = pedal->GetDrawingY();

    int startRadius = 0;
    if (!pedal->GetStart()->Is(TIMESTAMP_ATTR)) {
        startRadius = pedal->GetStart()->GetDrawingRadius(m_doc);
    }

    int endRadius = 0;
    if (!pedal->GetEnd()->Is(TIMESTAMP_ATTR)) {
        endRadius = pedal->GetEnd()->GetDrawingRadius(m_doc);
    }

    // The both correspond to the current system, which means no system break in-between (simple case)
    if (spanningType == SPANNING_START_END) {
        x1 -= startRadius;
        x2 -= endRadius - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
    }
    // Only the first parent is the same, this means that the syl is "open" at the end of the system
    else if (spanningType == SPANNING_START) {
        x1 -= startRadius;
    }
    // We are in the system of the last note - draw the connector from the beginning of the system
    else if (spanningType == SPANNING_END) {
        x2 -= endRadius - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
    }
    else {
        // nothing to adjust
    }

    if (graphic) {
        dc->ResumeGraphic(graphic, graphic->GetUuid());
    }
    else {
        dc->StartGraphic(pedal, "", pedal->GetUuid(), false);
    }

    int bracketSize = m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
    int lineWidth = m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);

    // Opening bracket
    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START)) {
        DrawFilledRectangle(dc, x1, y, x1 + bracketSize / 2, y + lineWidth);
        DrawFilledRectangle(dc, x1, y, x1 + lineWidth, y + bracketSize);
    }
    // Closing bracket
    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_END)) {
        DrawFilledRectangle(dc, x2 - bracketSize / 2, y, x2, y + lineWidth);
        DrawFilledRectangle(dc, x2 - lineWidth, y, x2, y + bracketSize);
    }
    DrawFilledRectangle(dc, x1 + bracketSize / 2, y, x2 - bracketSize / 2, y + lineWidth);

    if (graphic) {
        dc->EndResumedGraphic(graphic, this);
    }
    else {
        dc->EndGraphic(pedal, this);
    }
}

void View::DrawTrillExtension(
    DeviceContext *dc, Trill *trill, int x1, int x2, Staff *staff, char spanningType, Object *graphic)
{
    assert(dc);
    assert(trill);
    assert(staff);

    int y
        = trill->GetDrawingY() + m_doc->GetGlyphHeight(SMUFL_E566_ornamentTrill, staff->m_drawingStaffSize, false) / 3;

    // Adjust the x1 for the tr symbol
    if (trill->GetLstartsym() == LINESTARTENDSYMBOL_none) {
        x1 -= trill->GetStart()->GetDrawingRadius(m_doc);
        y += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2;
    }
    else if ((spanningType == SPANNING_START) || (spanningType == SPANNING_START_END)) {
        int offsetFactor = 2;
        if (trill->GetStart()->Is(TIMESTAMP_ATTR)) offsetFactor = 1;
        x1 += m_doc->GetGlyphWidth(SMUFL_E566_ornamentTrill, staff->m_drawingStaffSize, false) / offsetFactor;
    }

    // Adjust the x2 for endid
    if (!trill->GetEnd()->Is(TIMESTAMP_ATTR)) x2 -= trill->GetEnd()->GetDrawingRadius(m_doc);

    int length = x2 - x1;
    Point orig(x1, y);

    /************** draw it **************/

    if (graphic)
        dc->ResumeGraphic(graphic, graphic->GetUuid());
    else
        dc->StartGraphic(trill, "", trill->GetUuid(), false);

    DrawSmuflLine(dc, orig, length, staff->m_drawingStaffSize, false, SMUFL_E59D_ornamentZigZagLineNoRightEnd, 0,
        SMUFL_E59E_ornamentZigZagLineWithRightEnd);

    if (graphic)
        dc->EndResumedGraphic(graphic, this);
    else
        dc->EndGraphic(trill, this);
}

void View::DrawControlElementConnector(
    DeviceContext *dc, ControlElement *element, int x1, int x2, Staff *staff, char spanningType, Object *graphic)
{
    assert(element);

    TimeSpanningInterface *interface = element->GetTimeSpanningInterface();
    assert(interface);

    assert(element->GetNextLink() || interface->GetEnd());
    if (!element->GetNextLink() && !interface->GetEnd()) return;

    int y = element->GetDrawingY() + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2;

    // Adjust the x1
    if ((spanningType == SPANNING_START) || (spanningType == SPANNING_START_END)) {
        if (element->GetCurrentFloatingPositioner() && element->GetCurrentFloatingPositioner()->HasContentBB()) {
            x1 = element->GetCurrentFloatingPositioner()->GetContentRight();
        }
    }

    // Adjust the x2 for extensions with @endid
    if ((spanningType == SPANNING_END) || (spanningType == SPANNING_START_END)) {
        if (element->GetNextLink()) {
            FloatingPositioner *nextLink
                = element->GetCorrespFloatingPositioner(dynamic_cast<ControlElement *>(element->GetNextLink()));
            if (nextLink && nextLink->HasContentBB()) {
                x2 = nextLink->GetContentLeft();
            }
        }
    }

    const int width = m_options->m_lyricLineThickness.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);

    // the length of the dash and the space between them - can be made a parameter
    const int dashLength = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 4 / 3;
    const int dashSpace = m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize) * 5 / 3;
    const int halfDashLength = dashLength / 2;

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

    /************** draw it **************/

    if (graphic) {
        dc->ResumeGraphic(graphic, graphic->GetUuid());
    }
    else {
        dc->StartGraphic(element, "", element->GetUuid(), false);
    }

    bool deactivate = true;
    // If there is no end link and we are not starting the control element, then do not deactivate the element.
    // The vertical spacing will not be consistent but a least is does not leave the connector un-laidout
    if (!element->GetNextLink() && (spanningType != SPANNING_START_END) && (spanningType != SPANNING_START)) {
        deactivate = false;
    }

    if (deactivate) {
        dc->DeactivateGraphic();
    }

    int i;
    for (i = 0; i < nbDashes; ++i) {
        int x = x1 + margin + (i * dashSpace);
        x = std::max(x, x1);

        DrawFilledRectangle(dc, x - halfDashLength, y, x + halfDashLength, y + width);
    }

    if (deactivate) {
        dc->ReactivateGraphic();
    }

    if (graphic) {
        dc->EndResumedGraphic(graphic, this);
    }
    else {
        dc->EndGraphic(element, this);
    }
}

void View::DrawFConnector(DeviceContext *dc, F *f, int x1, int x2, Staff *staff, char spanningType, Object *graphic)
{

    assert(f);
    assert(f->GetStart() && f->GetEnd());
    if (!f->GetStart() || !f->GetEnd()) return;

    int y = GetFYRel(f, staff);
    TextExtend extend;

    // The both correspond to the current system, which means no system break in-between (simple case)
    if (spanningType == SPANNING_START_END) {
        x1 = f->GetContentRight();
    }
    // Only the first parent is the same, this means that the syl is "open" at the end of the system
    else if (spanningType == SPANNING_START) {
        x1 = f->GetContentRight();
    }
    // We are in the system of the last note - draw the connector from the beginning of the system
    else if (spanningType == SPANNING_END) {
        // nothing to adjust
    }
    else {
        // nothing to adjust
    }

    // Because Syl is not a ControlElement (FloatingElement) with FloatingPositioner we need to instanciate a temporary
    // object
    // in order not to reset the Syl bounding box.
    F fConnector;
    if (graphic) {
        dc->ResumeGraphic(graphic, graphic->GetUuid());
    }
    else
        dc->StartGraphic(&fConnector, "", f->GetUuid(), false);

    dc->DeactivateGraphic();

    int width = m_options->m_lyricLineThickness.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    // Adjust it proportionally to the lyric size
    width *= m_options->m_lyricSize.GetValue() / m_options->m_lyricSize.GetDefault();
    DrawFilledRectangle(dc, x1, y, x2, y + width);

    dc->ReactivateGraphic();

    if (graphic) {
        dc->EndResumedGraphic(graphic, this);
    }
    else
        dc->EndGraphic(&fConnector, this);
}

void View::DrawSylConnector(
    DeviceContext *dc, Syl *syl, int x1, int x2, Staff *staff, char spanningType, Object *graphic)
{
    assert(syl);
    assert(syl->GetStart() && syl->GetEnd());
    if (!syl->GetStart() || !syl->GetEnd()) return;

    int y = staff->GetDrawingY() + GetSylYRel(syl->m_drawingVerse, staff);
    TextExtend extend;

    // The both correspond to the current system, which means no system break in-between (simple case)
    if (spanningType == SPANNING_START_END) {
        x1 = syl->GetContentRight();
        if (syl->m_nextWordSyl) {
            x2 = syl->m_nextWordSyl->GetContentLeft();
        }
    }
    // Only the first parent is the same, this means that the syl is "open" at the end of the system
    else if (spanningType == SPANNING_START) {
        x1 = syl->GetContentRight();
    }
    // We are in the system of the last note - draw the connector from the beginning of the system
    else if (spanningType == SPANNING_END) {
        // If we do not want to show hyphens at the start of a system and the end is at time 0.0
        if (m_options->m_lyricNoStartHyphen.GetValue() && (syl->GetEnd()->GetAlignment()->GetTime() == 0.0)) {
            // Return but only if the end is in the first measure of the system...
            Measure *measure = vrv_cast<Measure *>(syl->GetEnd()->GetFirstAncestor(MEASURE));
            assert(measure);
            System *system = vrv_cast<System *>(measure->GetFirstAncestor(SYSTEM));
            assert(system);
            if (measure == dynamic_cast<Measure *>(system->FindDescendantByType(MEASURE))) {
                return;
            }
        }
        // Otherwise just adjust x2
        if (syl->m_nextWordSyl) {
            x2 = syl->m_nextWordSyl->GetContentLeft();
        }
        x1 -= m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
    }
    // Rare case where neither the first note nor the last note are in the current system - draw the connector
    // throughout the system
    else {
        // nothing to adjust
    }

    // Because Syl is not a ControlElement (FloatingElement) with FloatingPositioner we need to instanciate a temporary
    // object
    // in order not to reset the Syl bounding box.
    Syl sylConnector;
    if (graphic) {
        dc->ResumeGraphic(graphic, graphic->GetUuid());
    }
    else
        dc->StartGraphic(&sylConnector, "", syl->GetUuid(), false);

    dc->DeactivateGraphic();

    DrawSylConnectorLines(dc, x1, x2, y, syl, staff);

    dc->ReactivateGraphic();

    if (graphic) {
        dc->EndResumedGraphic(graphic, this);
    }
    else
        dc->EndGraphic(&sylConnector, this);
}

void View::DrawSylConnectorLines(DeviceContext *dc, int x1, int x2, int y, Syl *syl, Staff *staff)
{
    if (dc->Is(BBOX_DEVICE_CONTEXT)) {
        return;
    }

    int thickness = m_options->m_lyricLineThickness.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    // Adjust it proportionally to the lyric size
    thickness *= m_options->m_lyricSize.GetValue() / m_options->m_lyricSize.GetDefault();

    if (syl->GetCon() == sylLog_CON_d) {

        y += (m_options->m_lyricSize.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 5);

        // the length of the dash and the space between them
        int dashLength = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * m_options->m_lyricHyphenLength.GetValue();
        // Adjust it proportionally to the lyric size
        dashLength *= m_options->m_lyricSize.GetValue() / m_options->m_lyricSize.GetDefault();
        int halfDashLength = dashLength / 2;

        int dashSpace = m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize) * 5 / 3;
        int dist = x2 - x1;
        int nbDashes = dist / dashSpace;

        int margin = dist / 2;
        // no dash if the distance is smaller than a dash length
        if (dist < dashLength) {
            LogDebug("Hyphen space under the limit");
            nbDashes = 0;
        }
        else if (nbDashes < 2) {
            nbDashes = 1;
        }
        else {
            margin = (dist - ((nbDashes - 1) * dashSpace)) / 2;
        }

        int i;
        for (i = 0; i < nbDashes; ++i) {
            int x = x1 + margin + (i * dashSpace);
            x = std::max(x, x1);

            DrawFilledRectangle(dc, x - halfDashLength, y, x + halfDashLength, y + thickness);
        }
        // DrawFilledRectangle(dc, x1, y, x2, y + width);
    }
    else if (syl->GetCon() == sylLog_CON_u) {
        x1 += (int)m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2;
        if (x2 > x1) {
            DrawFilledRectangle(dc, x1, y, x2, y + thickness);
        }
    }
}

void View::DrawArpeg(DeviceContext *dc, Arpeg *arpeg, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(arpeg);

    Note *topNote = NULL;
    Note *bottomNote = NULL;

    arpeg->GetDrawingTopBottomNotes(topNote, bottomNote);

    // We cannot draw without a top and bottom note
    if (!topNote || !bottomNote) return;

    int top = topNote->GetDrawingY();
    int bottom = bottomNote->GetDrawingY();

    // We arbitrarily look at the top note
    Staff *staff = vrv_cast<Staff *>(topNote->GetFirstAncestor(STAFF));
    assert(staff);
    bool drawingCueSize = topNote->GetDrawingCueSize();

    // We are going to have only one FloatingPositioner - staff will be the top note one
    if (!system->SetCurrentFloatingPositioner(staff->GetN(), arpeg, topNote, staff)) {
        return;
    }
    // Special case: because the positionner objects are reset in ResetVerticalAlignment we
    // need to reset the value of the DrawingXRel each time. The value is stored in Arpeg.
    arpeg->GetCurrentFloatingPositioner()->SetDrawingXRel(arpeg->GetDrawingXRel());

    int length = top - bottom;
    // We add - substract a unit in order to have the line going to the edge
    length += m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
    int y = bottom - m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    int x = arpeg->GetDrawingX();
    int angle = -90;

    wchar_t fillGlyph = SMUFL_EAA9_wiggleArpeggiatoUp;
    wchar_t endGlyph = (arpeg->GetArrow() == BOOLEAN_true) ? SMUFL_EAAD_wiggleArpeggiatoUpArrow : 0;

    if (arpeg->GetOrder() == arpegLog_ORDER_down) {
        y = top + m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        x -= m_doc->GetGlyphWidth(SMUFL_EAAA_wiggleArpeggiatoDown, staff->m_drawingStaffSize, drawingCueSize) / 2;
        fillGlyph = SMUFL_EAAA_wiggleArpeggiatoDown;
        endGlyph = (arpeg->GetArrow() == BOOLEAN_true) ? SMUFL_EAAE_wiggleArpeggiatoDownArrow : 0;
        angle = 90;
    }

    if (arpeg->GetArrowShape() == LINESTARTENDSYMBOL_none) endGlyph = 0;

    Point orig(x, y);

    dc->StartGraphic(arpeg, "", arpeg->GetUuid());

    // Smufl glyphs are horizontal - Rotate them counter clockwise
    dc->RotateGraphic(Point(ToDeviceContextX(x), ToDeviceContextY(y)), angle);

    DrawSmuflLine(dc, orig, length, staff->m_drawingStaffSize, drawingCueSize, fillGlyph, 0, endGlyph);

    dc->EndGraphic(arpeg, this);
}

void View::DrawBreath(DeviceContext *dc, Breath *breath, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(breath);

    // Cannot draw a breath that has no start position
    if (!breath->GetStart()) return;

    dc->StartGraphic(breath, "", breath->GetUuid());

    int x = breath->GetStart()->GetDrawingX() + breath->GetStart()->GetDrawingRadius(m_doc);

    // use breath mark comma glyph
    int code = SMUFL_E4CE_breathMarkComma;

    std::wstring str;
    str.push_back(code);

    data_HORIZONTALALIGNMENT alignment = HORIZONTALALIGNMENT_center;
    // center the glyph only with @startid
    if (breath->GetStart()->Is(TIMESTAMP_ATTR)) {
        alignment = HORIZONTALALIGNMENT_left;
    }

    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList = breath->GetTstampStaves(measure, breath);
    for (staffIter = staffList.begin(); staffIter != staffList.end(); ++staffIter) {
        if (!system->SetCurrentFloatingPositioner((*staffIter)->GetN(), breath, breath->GetStart(), *staffIter)) {
            continue;
        }
        int y = breath->GetDrawingY();

        dc->SetFont(m_doc->GetDrawingSmuflFont((*staffIter)->m_drawingStaffSize, false));
        DrawSmuflString(dc, x, y, str, alignment, (*staffIter)->m_drawingStaffSize);
        dc->ResetFont();
    }

    dc->EndGraphic(breath, this);
}

void View::DrawDir(DeviceContext *dc, Dir *dir, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(dir);

    // Cannot draw a dir that has no start position
    if (!dir->GetStart()) return;

    dc->StartGraphic(dir, "", dir->GetUuid());

    FontInfo dirTxt;
    if (!dc->UseGlobalStyling()) {
        dirTxt.SetFaceName("Times");
        dirTxt.SetStyle(FONTSTYLE_italic);
    }

    TextDrawingParams params;

    int lineCount = dir->GetNumberOfLines(dir);

    // If we have not timestamp
    params.m_x = dir->GetStart()->GetDrawingX() + dir->GetStart()->GetDrawingRadius(m_doc);

    data_HORIZONTALALIGNMENT alignment = dir->GetChildRendAlignment();
    // dir are left aligned by default (with both @tstamp and @startid)
    if (alignment == HORIZONTALALIGNMENT_NONE) alignment = HORIZONTALALIGNMENT_left;

    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList = dir->GetTstampStaves(measure, dir);
    for (staffIter = staffList.begin(); staffIter != staffList.end(); ++staffIter) {
        if (!system->SetCurrentFloatingPositioner((*staffIter)->GetN(), dir, dir->GetStart(), *staffIter)) {
            continue;
        }
        params.m_boxedRend.clear();
        params.m_y = dir->GetDrawingY();
        params.m_pointSize = m_doc->GetDrawingLyricFont((*staffIter)->m_drawingStaffSize)->GetPointSize();

        dirTxt.SetPointSize(params.m_pointSize);

        if (dir->GetPlace() == STAFFREL_between) {
            if (lineCount > 1) {
                params.m_y += (m_doc->GetTextLineHeight(&dirTxt, false) * (lineCount - 1) / 2);
            }
            params.m_y -= m_doc->GetTextXHeight(&dirTxt, false) / 2;
        }

        dc->SetBrush(m_currentColour, AxSOLID);
        dc->SetFont(&dirTxt);

        dc->StartText(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), alignment);
        DrawTextChildren(dc, dir, params);
        dc->EndText();

        dc->ResetFont();
        dc->ResetBrush();

        DrawTextBoxes(dc, params.m_boxedRend, (*staffIter)->m_drawingStaffSize);
    }

    dc->EndGraphic(dir, this);
}

void View::DrawDynam(DeviceContext *dc, Dynam *dynam, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(dynam);

    // Cannot draw dynamics that have no start position
    if (!dynam->GetStart()) return;

    dc->StartGraphic(dynam, "", dynam->GetUuid());

    bool isSymbolOnly = dynam->IsSymbolOnly();
    std::wstring dynamSymbol;
    if (isSymbolOnly) {
        dynamSymbol = dynam->GetSymbolStr();
    }

    FontInfo dynamTxt;
    if (!dc->UseGlobalStyling()) {
        dynamTxt.SetFaceName("Times");
        dynamTxt.SetStyle(FONTSTYLE_italic);
    }

    TextDrawingParams params;

    int lineCount = dynam->GetNumberOfLines(dynam);

    // If we have not timestamp
    params.m_x = dynam->GetStart()->GetDrawingX() + dynam->GetStart()->GetDrawingRadius(m_doc);

    data_HORIZONTALALIGNMENT alignment = dynam->GetChildRendAlignment();
    // dynam are left aligned by default;
    if (alignment == 0) {
        // centre the dynam only with @startid
        alignment = (dynam->GetStart()->Is(TIMESTAMP_ATTR)) ? HORIZONTALALIGNMENT_left : HORIZONTALALIGNMENT_center;
    }

    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList = dynam->GetTstampStaves(measure, dynam);
    for (staffIter = staffList.begin(); staffIter != staffList.end(); ++staffIter) {
        if (!system->SetCurrentFloatingPositioner((*staffIter)->GetN(), dynam, dynam->GetStart(), *staffIter)) {
            continue;
        }

        params.m_boxedRend.clear();
        params.m_y = dynam->GetDrawingY();
        params.m_pointSize = m_doc->GetDrawingLyricFont((*staffIter)->m_drawingStaffSize)->GetPointSize();

        dynamTxt.SetPointSize(params.m_pointSize);

        if (dynam->GetPlace() == STAFFREL_between) {
            if (lineCount > 1) {
                params.m_y += (m_doc->GetTextLineHeight(&dynamTxt, false) * (lineCount - 1) / 2);
            }
            params.m_y -= m_doc->GetTextXHeight(&dynamTxt, false) / 2;
        }

        // If the dynamic is a symbol (pp, mf, etc.) draw it as one smufl string. This will not take into account
        // editorial element within the dynam as it would with text. Also, it is center only if it is a symbol.
        if (isSymbolOnly) {
            dc->SetFont(m_doc->GetDrawingSmuflFont((*staffIter)->m_drawingStaffSize, false));
            DrawSmuflString(dc, params.m_x, params.m_y, dynamSymbol, alignment, (*staffIter)->m_drawingStaffSize);
            dc->ResetFont();
        }
        else {
            dc->SetBrush(m_currentColour, AxSOLID);
            dc->SetFont(&dynamTxt);

            dc->StartText(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), alignment);
            DrawTextChildren(dc, dynam, params);
            dc->EndText();

            dc->ResetFont();
            dc->ResetBrush();
        }
        DrawTextBoxes(dc, params.m_boxedRend, (*staffIter)->m_drawingStaffSize);
    }

    dc->EndGraphic(dynam, this);
}

void View::DrawFb(DeviceContext *dc, Staff *staff, Fb *fb, TextDrawingParams &params)
{
    assert(dc);
    assert(fb);

    dc->StartGraphic(fb, "", fb->GetUuid());

    FontInfo *fontDim = m_doc->GetDrawingLyricFont(staff->m_drawingStaffSize);
    int lineHeight = m_doc->GetTextLineHeight(fontDim, false);
    int startX = params.m_x;

    fontDim->SetPointSize(m_doc->GetDrawingLyricFont((staff)->m_drawingStaffSize)->GetPointSize());

    dc->SetBrush(m_currentColour, AxSOLID);
    dc->SetFont(fontDim);

    for (auto current : *fb->GetChildren()) {
        dc->StartText(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), HORIZONTALALIGNMENT_left);
        if (current->Is(FIGURE)) {
            // dynamic_cast assert in DrawF
            DrawF(dc, dynamic_cast<F *>(current), params);
        }
        else if (current->IsEditorialElement()) {
            // cast to EditorialElement check in DrawLayerEditorialElement
            DrawFbEditorialElement(dc, dynamic_cast<EditorialElement *>(current), params);
        }
        else {
            assert(false);
        }
        dc->EndText();

        params.m_y -= lineHeight;
        params.m_x = startX;
    }

    dc->ResetFont();
    dc->ResetBrush();

    dc->EndGraphic(fb, this);
}

void View::DrawFermata(DeviceContext *dc, Fermata *fermata, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(fermata);

    // Cannot draw a fermata that has no start position
    if (!fermata->GetStart()) return;

    dc->StartGraphic(fermata, "", fermata->GetUuid());

    int x = fermata->GetStart()->GetDrawingX() + fermata->GetStart()->GetDrawingRadius(m_doc);

    // for a start always put fermatas up
    int code = fermata->GetFermataGlyph();

    std::wstring str;
    str.push_back(code);

    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList = fermata->GetTstampStaves(measure, fermata);
    for (staffIter = staffList.begin(); staffIter != staffList.end(); ++staffIter) {
        if (!system->SetCurrentFloatingPositioner((*staffIter)->GetN(), fermata, fermata->GetStart(), *staffIter)) {
            continue;
        }
        int y = fermata->GetDrawingY();

        dc->SetFont(m_doc->GetDrawingSmuflFont((*staffIter)->m_drawingStaffSize, false));
        DrawSmuflString(dc, x, y, str, HORIZONTALALIGNMENT_center, (*staffIter)->m_drawingStaffSize);
        dc->ResetFont();
    }

    dc->EndGraphic(fermata, this);
}

void View::DrawFing(DeviceContext *dc, Fing *fing, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(fing);

    // Cannot draw a fing that has no start position
    if (!fing->GetStart()) return;

    dc->StartGraphic(fing, "", fing->GetUuid());

    FontInfo fingTxt;
    if (!dc->UseGlobalStyling()) {
        fingTxt.SetFaceName("Times");
    }

    TextDrawingParams params;
    params.m_x = fing->GetStart()->GetDrawingX() + fing->GetStart()->GetDrawingRadius(m_doc);
    // center fingering
    data_HORIZONTALALIGNMENT alignment = HORIZONTALALIGNMENT_center;

    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList = fing->GetTstampStaves(measure, fing);
    for (staffIter = staffList.begin(); staffIter != staffList.end(); ++staffIter) {
        if (!system->SetCurrentFloatingPositioner((*staffIter)->GetN(), fing, fing->GetStart(), *staffIter)) {
            continue;
        }

        params.m_boxedRend.clear();
        params.m_y = fing->GetDrawingY();

        params.m_pointSize = m_doc->GetDrawingLyricFont((*staffIter)->m_drawingStaffSize)->GetPointSize();

        fingTxt.SetPointSize(params.m_pointSize / 2);

        dc->SetBrush(m_currentColour, AxSOLID);
        dc->SetFont(&fingTxt);

        dc->StartText(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), alignment);
        DrawTextChildren(dc, fing, params);
        dc->EndText();

        dc->ResetFont();
        dc->ResetBrush();

        DrawTextBoxes(dc, params.m_boxedRend, (*staffIter)->m_drawingStaffSize);
    }

    dc->EndGraphic(fing, this);
}

void View::DrawGliss(DeviceContext *dc, Gliss *gliss, int x1, int x2, Staff *staff, char spanningType, Object *graphic)
{
    assert(dc);
    assert(gliss);
    assert(staff);

    int y1 = staff->GetDrawingY();
    int y2 = staff->GetDrawingY();
    double slope = 0.0;

    /************** parent layers **************/

    Note *note1 = dynamic_cast<Note *>(gliss->GetStart());
    Note *note2 = dynamic_cast<Note *>(gliss->GetEnd());

    if (!note1 || !note2) {
        // no note, obviously nothing to do...
        // this also means that notes with tstamp events are not supported
        return;
    }

    if (note1 || note2) {
        int firstLoc = note1->GetDrawingLoc();
        int secondLoc = note2->GetDrawingLoc();
        if (x1 != x2)
            slope = (secondLoc - firstLoc) * m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / (double)(x2 - x1);
    }
    // only half at system breaks
    if (spanningType != SPANNING_START_END) slope = slope / 2;

    // the normal case
    if (spanningType == SPANNING_START_END || spanningType == SPANNING_START) {
        if (note1) {
            x1 += m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
            y1 = note1->GetDrawingY() + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * slope;
        }
        if (note1 && (note1->GetDots() > 0) && (abs(slope) < 1.0)) {
            x1 += m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * note1->GetDots();
            y1 += m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * note1->GetDots() * slope;
        }
        else {
            x1 += m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
            y1 += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * slope;
        }
    }
    else {
        y1 = note2->GetDrawingY() - (x2 - x1) * slope;
    }
    if (spanningType == SPANNING_START_END || spanningType == SPANNING_END) {
        if (note2) {
            x2 -= m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
            y2 = note2->GetDrawingY() - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * slope;
        }
        if (note2 && note2->GetDrawingAccid()) {
            x2 -= m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
            y2 -= m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * slope;
        }
    }
    else {
        // shorten it
        x2 -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        y2 = y1 + (x2 - x1) * slope;
    }

    int lineWidth = m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) * 1.5;
    if (gliss->HasLwidth()) {
        if (gliss->GetLwidth().GetType() == LINEWIDTHTYPE_lineWidthTerm) {
            if (gliss->GetLwidth().GetLineWithTerm() == LINEWIDTHTERM_narrow) {
                lineWidth *= LINEWIDTHTERM_factor_narrow;
            }
            else if (gliss->GetLwidth().GetLineWithTerm() == LINEWIDTHTERM_medium) {
                lineWidth *= LINEWIDTHTERM_factor_medium;
            }
            else if (gliss->GetLwidth().GetLineWithTerm() == LINEWIDTHTERM_wide) {
                lineWidth *= LINEWIDTHTERM_factor_wide;
            }
        }
        else if (gliss->GetLwidth().GetType() == LINEWIDTHTYPE_measurementAbs) {
            if (gliss->GetLwidth().GetMeasurementAbs() != VRV_UNSET) {
                lineWidth
                    = gliss->GetLwidth().GetMeasurementAbs() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize * 2);
            }
        }
    }

    if (graphic) {
        dc->ResumeGraphic(graphic, graphic->GetUuid());
    }
    else {
        dc->StartGraphic(gliss, "", gliss->GetUuid(), false);
    }

    // only solid lines for now
    DrawRoundedLine(dc, x1, y1, x2, y2, lineWidth);

    if (graphic) {
        dc->EndResumedGraphic(graphic, this);
    }
    else {
        dc->EndGraphic(gliss, this);
    }
}

void View::DrawHarm(DeviceContext *dc, Harm *harm, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(harm);

    // Cannot draw a harmony indication that has no start position
    if (!harm->GetStart()) return;

    dc->StartGraphic(harm, "", harm->GetUuid());

    FontInfo harmTxt;
    if (!dc->UseGlobalStyling()) {
        harmTxt.SetFaceName("Times");
    }

    TextDrawingParams params;

    // If we have not timestamp
    params.m_x = harm->GetStart()->GetDrawingX() + harm->GetStart()->GetDrawingRadius(m_doc);

    data_HORIZONTALALIGNMENT alignment = harm->GetChildRendAlignment();
    // harm are centered aligned by default;
    if (alignment == 0) {
        // centre the harm only with @startid
        alignment = (harm->GetStart()->Is(TIMESTAMP_ATTR)) ? HORIZONTALALIGNMENT_left : HORIZONTALALIGNMENT_center;
    }

    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList = harm->GetTstampStaves(measure, harm);
    for (staffIter = staffList.begin(); staffIter != staffList.end(); ++staffIter) {
        if (!system->SetCurrentFloatingPositioner((*staffIter)->GetN(), harm, harm->GetStart(), *staffIter)) {
            continue;
        }

        params.m_boxedRend.clear();
        params.m_y = harm->GetDrawingY();

        if (harm->GetFirst() && harm->GetFirst()->Is(FB)) {
            DrawFb(dc, *staffIter, dynamic_cast<Fb *>(harm->GetFirst()), params);
        }
        else {
            params.m_pointSize = m_doc->GetDrawingLyricFont((*staffIter)->m_drawingStaffSize)->GetPointSize();

            harmTxt.SetPointSize(params.m_pointSize);

            dc->SetBrush(m_currentColour, AxSOLID);
            dc->SetFont(&harmTxt);

            dc->StartText(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), alignment);
            DrawTextChildren(dc, harm, params);
            dc->EndText();

            dc->ResetFont();
            dc->ResetBrush();

            DrawTextBoxes(dc, params.m_boxedRend, (*staffIter)->m_drawingStaffSize);
        }
    }

    dc->EndGraphic(harm, this);
}

void View::DrawMordent(DeviceContext *dc, Mordent *mordent, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(mordent);

    // Cannot draw a mordent that has no start position
    if (!mordent->GetStart()) return;

    dc->StartGraphic(mordent, "", mordent->GetUuid());

    int x = mordent->GetStart()->GetDrawingX() + mordent->GetStart()->GetDrawingRadius(m_doc);

    // set mordent glyph
    int code = mordent->GetMordentGlyph();

    std::wstring str;
    str.push_back(code);

    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList = mordent->GetTstampStaves(measure, mordent);
    double xShift = 0.0;
    for (staffIter = staffList.begin(); staffIter != staffList.end(); ++staffIter) {
        if (!system->SetCurrentFloatingPositioner((*staffIter)->GetN(), mordent, mordent->GetStart(), *staffIter)) {
            continue;
        }
        int y = mordent->GetDrawingY();

        if (mordent->HasAccidlower()) {
            wchar_t accid = Accid::GetAccidGlyph(mordent->GetAccidlower());
            std::wstring accidStr;
            accidStr.push_back(accid);
            dc->SetFont(m_doc->GetDrawingSmuflFont((*staffIter)->m_drawingStaffSize, false));
            DrawSmuflString(
                dc, x, y, accidStr, HORIZONTALALIGNMENT_center, (*staffIter)->m_drawingStaffSize / 2, false);
            // Adjust the y position
            double factor = 1.0;
            data_ACCIDENTAL_WRITTEN meiaccid = mordent->GetAccidlower();
            // optimized vertical kerning for Leipzig font:
            if (meiaccid == ACCIDENTAL_WRITTEN_ff) {
                factor = 1.20;
                xShift = 0.14;
            }
            else if (meiaccid == ACCIDENTAL_WRITTEN_f) {
                factor = 1.20;
                xShift = -0.02;
            }
            else if (meiaccid == ACCIDENTAL_WRITTEN_n) {
                factor = 0.90;
                xShift = -0.04;
            }
            else if (meiaccid == ACCIDENTAL_WRITTEN_s) {
                factor = 1.15;
            }
            else if (meiaccid == ACCIDENTAL_WRITTEN_x) {
                factor = 2.00;
            }
            y += factor * m_doc->GetGlyphHeight(accid, (*staffIter)->m_drawingStaffSize, true) / 2;
        }
        else if (mordent->HasAccidupper()) {
            double mordentHeight = m_doc->GetGlyphHeight(code, (*staffIter)->m_drawingStaffSize, false);
            int accid = Accid::GetAccidGlyph(mordent->GetAccidupper());
            std::wstring accidStr;
            accidStr.push_back(accid);
            dc->SetFont(m_doc->GetDrawingSmuflFont((*staffIter)->m_drawingStaffSize, false));
            DrawSmuflString(
                dc, x, y, accidStr, HORIZONTALALIGNMENT_center, (*staffIter)->m_drawingStaffSize / 2, false);
            // Adjust the y position
            double factor = 1.75;
            data_ACCIDENTAL_WRITTEN meiaccid = mordent->GetAccidupper();
            // optimized vertical kerning for Leipzig font:
            if (meiaccid == ACCIDENTAL_WRITTEN_ff) {
                factor = 1.40;
            }
            else if (meiaccid == ACCIDENTAL_WRITTEN_f) {
                factor = 1.25;
            }
            else if (meiaccid == ACCIDENTAL_WRITTEN_n) {
                factor = 1.60;
                xShift = -0.10;
            }
            else if (meiaccid == ACCIDENTAL_WRITTEN_s) {
                factor = 1.60;
                xShift = -0.06;
            }
            else if (meiaccid == ACCIDENTAL_WRITTEN_x) {
                factor = 1.35;
                xShift = -0.08;
            }
            y -= factor * mordentHeight;
        }

        // Adjust the x position
        int drawingX = x - (1 + xShift) * m_doc->GetGlyphWidth(code, (*staffIter)->m_drawingStaffSize, false) / 2;

        dc->SetFont(m_doc->GetDrawingSmuflFont((*staffIter)->m_drawingStaffSize, false));
        DrawSmuflString(dc, drawingX, y, str, HORIZONTALALIGNMENT_left, (*staffIter)->m_drawingStaffSize);
        dc->ResetFont();
    }

    dc->EndGraphic(mordent, this);
}

void View::DrawPedal(DeviceContext *dc, Pedal *pedal, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(pedal);

    // Cannot draw a pedal that has no start position
    if (!pedal->GetStart()) return;

    // just as without a dir attribute
    if (!pedal->HasDir()) return;

    dc->StartGraphic(pedal, "", pedal->GetUuid());

    // Draw a symbol, if it's not a line
    if (pedal->GetForm() != pedalVis_FORM_line) {

        bool bounceStar = true;
        if (pedal->GetForm() == pedalVis_FORM_altpedstar) bounceStar = false;

        int x = pedal->GetStart()->GetDrawingX() + pedal->GetStart()->GetDrawingRadius(m_doc);

        data_HORIZONTALALIGNMENT alignment = HORIZONTALALIGNMENT_center;
        // center the pedal only with @startid
        if (pedal->GetStart()->Is(TIMESTAMP_ATTR)) {
            alignment = HORIZONTALALIGNMENT_left;
        }

        std::vector<Staff *>::iterator staffIter;
        std::vector<Staff *> staffList = pedal->GetTstampStaves(measure, pedal);

        int code = SMUFL_E655_keyboardPedalUp;
        std::wstring str;
        if (bounceStar && (pedal->GetDir() == pedalLog_DIR_bounce)) {
            str.push_back(code);
            // Get the staff size of the first staff
            const int staffSize
                = (staffList.begin() != staffList.end()) ? (*staffList.begin())->m_drawingStaffSize : 100;
            x -= m_doc->GetGlyphWidth(SMUFL_E655_keyboardPedalUp, staffSize, false);
        }
        if (pedal->GetDir() != pedalLog_DIR_up) {
            code = pedal->GetPedalGlyph();
        }
        str.push_back(code);

        for (staffIter = staffList.begin(); staffIter != staffList.end(); ++staffIter) {
            if (!system->SetCurrentFloatingPositioner((*staffIter)->GetN(), pedal, pedal->GetStart(), *staffIter)) {
                continue;
            }
            // Basic method that use bounding box
            const int y = pedal->GetDrawingY();

            dc->SetFont(m_doc->GetDrawingSmuflFont((*staffIter)->m_drawingStaffSize, false));
            DrawSmuflString(dc, x, y, str, alignment, (*staffIter)->m_drawingStaffSize);
            dc->ResetFont();
        }
    }

    dc->EndGraphic(pedal, this);
}

void View::DrawReh(DeviceContext *dc, Reh *reh, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(reh);

    // Reh should be drawn at measure start
    if (!reh->GetStart()) return;

    dc->StartGraphic(reh, "", reh->GetUuid());

    FontInfo rehTxt;
    if (!dc->UseGlobalStyling()) {
        rehTxt.SetFaceName("Times");
        rehTxt.SetWeight(FONTWEIGHT_bold);
    }

    TextDrawingParams params;

    params.m_x = reh->GetStart()->GetDrawingX();

    data_HORIZONTALALIGNMENT alignment = reh->GetChildRendAlignment();
    // Rehearsal marks are center aligned by default;
    if (alignment == 0) alignment = HORIZONTALALIGNMENT_center;

    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList = reh->GetTstampStaves(measure, reh);
    for (staffIter = staffList.begin(); staffIter != staffList.end(); ++staffIter) {
        if (!system->SetCurrentFloatingPositioner((*staffIter)->GetN(), reh, reh->GetStart(), *staffIter)) {
            continue;
        }

        params.m_boxedRend.clear();
        params.m_y = reh->GetDrawingY();
        params.m_pointSize = m_doc->GetDrawingLyricFont((*staffIter)->m_drawingStaffSize)->GetPointSize();

        rehTxt.SetPointSize(params.m_pointSize);

        dc->SetBrush(m_currentColour, AxSOLID);
        dc->SetFont(&rehTxt);

        dc->StartText(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), alignment);
        DrawTextChildren(dc, reh, params);
        dc->EndText();

        dc->ResetFont();
        dc->ResetBrush();

        DrawTextBoxes(dc, params.m_boxedRend, (*staffIter)->m_drawingStaffSize);
    }

    dc->EndGraphic(reh, this);
}

void View::DrawTempo(DeviceContext *dc, Tempo *tempo, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(tempo);

    // Cannot draw a tempo that has no start position
    if (!tempo->GetStart()) return;

    dc->StartGraphic(tempo, "", tempo->GetUuid());

    FontInfo tempoTxt;
    if (!dc->UseGlobalStyling()) {
        tempoTxt.SetFaceName("Times");
        tempoTxt.SetWeight(FONTWEIGHT_bold);
    }

    int lineCount = tempo->GetNumberOfLines(tempo);

    TextDrawingParams params;

    // see if we have a meter signature for this measure
    MeasureAlignerTypeComparison alignmentComparison(ALIGNMENT_SCOREDEF_METERSIG);
    Alignment *pos
        = dynamic_cast<Alignment *>(measure->m_measureAligner.FindDescendantByComparison(&alignmentComparison, 1));
    params.m_x = tempo->GetStart()->GetDrawingX();
    if (!tempo->HasStartid()) {
        if ((tempo->GetTstamp() <= 1) && pos)
            params.m_x = measure->GetDrawingX() + pos->GetXRel();
        else
            params.m_x -= 2 * tempo->GetStart()->GetDrawingRadius(m_doc);
    }

    data_HORIZONTALALIGNMENT alignment = tempo->GetChildRendAlignment();
    // Tempo are left aligned by default;
    if (alignment == 0) alignment = HORIZONTALALIGNMENT_left;

    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList = tempo->GetTstampStaves(measure, tempo);
    for (staffIter = staffList.begin(); staffIter != staffList.end(); ++staffIter) {
        if (!system->SetCurrentFloatingPositioner((*staffIter)->GetN(), tempo, tempo->GetStart(), *staffIter)) {
            continue;
        }

        params.m_boxedRend.clear();
        params.m_y = tempo->GetDrawingY();
        params.m_pointSize = m_doc->GetDrawingLyricFont((*staffIter)->m_drawingStaffSize)->GetPointSize();

        tempoTxt.SetPointSize(params.m_pointSize);

        if (tempo->GetPlace() == STAFFREL_between) {
            if (lineCount > 1) {
                params.m_y += (m_doc->GetTextLineHeight(&tempoTxt, false) * (lineCount - 1) / 2);
            }
            params.m_y -= m_doc->GetTextXHeight(&tempoTxt, false) / 2;
        }

        dc->SetBrush(m_currentColour, AxSOLID);
        dc->SetFont(&tempoTxt);

        dc->StartText(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), alignment);
        DrawTextChildren(dc, tempo, params);
        dc->EndText();

        dc->ResetFont();
        dc->ResetBrush();

        DrawTextBoxes(dc, params.m_boxedRend, (*staffIter)->m_drawingStaffSize);
    }

    dc->EndGraphic(tempo, this);
}

void View::DrawTrill(DeviceContext *dc, Trill *trill, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(trill);

    // Cannot draw a trill that has no start position
    if (!trill->GetStart()) return;

    dc->StartGraphic(trill, "", trill->GetUuid());

    int x = trill->GetStart()->GetDrawingX();

    data_HORIZONTALALIGNMENT alignment = HORIZONTALALIGNMENT_center;
    // center the trill only with @startid
    if (trill->GetStart()->Is(TIMESTAMP_ATTR)) {
        alignment = HORIZONTALALIGNMENT_left;
    }
    else {
        x += trill->GetStart()->GetDrawingRadius(m_doc);
    }

    // for a start always put trill up
    int code = trill->GetTrillGlyph();
    std::wstring str;

    if (trill->GetLstartsym() != LINESTARTENDSYMBOL_none) {
        str.push_back(code);
    }

    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList = trill->GetTstampStaves(measure, trill);
    for (staffIter = staffList.begin(); staffIter != staffList.end(); ++staffIter) {
        if (!system->SetCurrentFloatingPositioner((*staffIter)->GetN(), trill, trill->GetStart(), *staffIter)) {
            continue;
        }
        int y = trill->GetDrawingY();

        // Upper and lower accidentals are currently exclusive, but sould both be allowed at the same time.
        if (trill->HasAccidlower()) {
            int accidXShift = (alignment == HORIZONTALALIGNMENT_center)
                ? 0
                : m_doc->GetGlyphWidth(code, (*staffIter)->m_drawingStaffSize, false) / 2;
            wchar_t accid = Accid::GetAccidGlyph(trill->GetAccidlower());
            std::wstring accidStr;
            accidStr.push_back(accid);
            dc->SetFont(m_doc->GetDrawingSmuflFont((*staffIter)->m_drawingStaffSize, false));
            int accidY = y - m_doc->GetGlyphHeight(accid, (*staffIter)->m_drawingStaffSize, true) / 2;
            DrawSmuflString(dc, x + accidXShift, accidY, accidStr, HORIZONTALALIGNMENT_center,
                (*staffIter)->m_drawingStaffSize / 2, false);
        }
        else if (trill->HasAccidupper()) {
            int accidXShift = (alignment == HORIZONTALALIGNMENT_center)
                ? 0
                : m_doc->GetGlyphWidth(code, (*staffIter)->m_drawingStaffSize, false) / 2;
            double trillHeight = m_doc->GetGlyphHeight(code, (*staffIter)->m_drawingStaffSize, false);
            wchar_t accid = Accid::GetAccidGlyph(trill->GetAccidupper());
            std::wstring accidStr;
            accidStr.push_back(accid);
            dc->SetFont(m_doc->GetDrawingSmuflFont((*staffIter)->m_drawingStaffSize, false));
            int accidY = y + trillHeight * 1.5;
            DrawSmuflString(dc, x + accidXShift, accidY, accidStr, HORIZONTALALIGNMENT_center,
                (*staffIter)->m_drawingStaffSize / 2, false);
        }

        dc->SetFont(m_doc->GetDrawingSmuflFont((*staffIter)->m_drawingStaffSize, false));
        DrawSmuflString(dc, x, y, str, alignment, (*staffIter)->m_drawingStaffSize);
        dc->ResetFont();
    }

    dc->EndGraphic(trill, this);
}

void View::DrawTurn(DeviceContext *dc, Turn *turn, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(turn);

    // Cannot draw a turn that has no start position
    if (!turn->GetStart()) return;

    dc->StartGraphic(turn, "", turn->GetUuid());

    int x = turn->GetStart()->GetDrawingX() + turn->GetStart()->GetDrawingRadius(m_doc);
    if (turn->GetDelayed() == BOOLEAN_true && !turn->HasTstamp()) LogWarning("delayed turns not supported");

    // set norm as default
    int code = turn->GetTurnGlyph();

    data_HORIZONTALALIGNMENT alignment = HORIZONTALALIGNMENT_center;
    // center the turn only with @startid
    if (turn->GetStart()->Is(TIMESTAMP_ATTR)) {
        alignment = HORIZONTALALIGNMENT_left;
    }

    std::wstring str;
    str.push_back(code);

    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList = turn->GetTstampStaves(measure, turn);
    for (staffIter = staffList.begin(); staffIter != staffList.end(); ++staffIter) {
        if (!system->SetCurrentFloatingPositioner((*staffIter)->GetN(), turn, turn->GetStart(), *staffIter)) {
            continue;
        }
        int y = turn->GetDrawingY();

        if (turn->HasAccidupper()) {
            int accidXShift = (alignment == HORIZONTALALIGNMENT_center)
                ? 0
                : m_doc->GetGlyphWidth(code, (*staffIter)->m_drawingStaffSize, false) / 2;
            wchar_t accid = Accid::GetAccidGlyph(turn->GetAccidupper());
            std::wstring accidStr;
            accidStr.push_back(accid);
            dc->SetFont(m_doc->GetDrawingSmuflFont((*staffIter)->m_drawingStaffSize, false));
            int accidYShit = m_doc->GetGlyphHeight(accid, (*staffIter)->m_drawingStaffSize, true);
            DrawSmuflString(dc, x + accidXShift, y + accidYShit, accidStr, HORIZONTALALIGNMENT_center,
                (*staffIter)->m_drawingStaffSize / 2, false);
        }
        if (turn->HasAccidlower()) {
            int accidXShift = (alignment == HORIZONTALALIGNMENT_center)
                ? 0
                : m_doc->GetGlyphWidth(code, (*staffIter)->m_drawingStaffSize, false) / 2;
            wchar_t accid = Accid::GetAccidGlyph(turn->GetAccidlower());
            std::wstring accidStr;
            accidStr.push_back(accid);
            dc->SetFont(m_doc->GetDrawingSmuflFont((*staffIter)->m_drawingStaffSize, false));
            int accidYShit = -m_doc->GetGlyphHeight(accid, (*staffIter)->m_drawingStaffSize, true) / 2;
            DrawSmuflString(dc, x + accidXShift, y + accidYShit, accidStr, HORIZONTALALIGNMENT_center,
                (*staffIter)->m_drawingStaffSize / 2, false);
        }

        dc->SetFont(m_doc->GetDrawingSmuflFont((*staffIter)->m_drawingStaffSize, false));
        DrawSmuflString(dc, x, y, str, alignment, (*staffIter)->m_drawingStaffSize);
        dc->ResetFont();
    }

    dc->EndGraphic(turn, this);
}

//----------------------------------------------------------------------------
// View - FloatingObject - SystemElement
//----------------------------------------------------------------------------

void View::DrawSystemElement(DeviceContext *dc, SystemElement *element, System *system)
{
    assert(dc);
    assert(element);
    assert(system);

    if (element->Is(BOUNDARY_END)) {
        BoundaryEnd *boundaryEnd = vrv_cast<BoundaryEnd *>(element);
        assert(boundaryEnd);
        assert(boundaryEnd->GetStart());
        dc->StartGraphic(element, boundaryEnd->GetStart()->GetUuid(), element->GetUuid());
        dc->EndGraphic(element, this);
    }
    else if (element->Is(ENDING)) {
        // Create placeholder - A graphic for the end boundary will be created
        // but only if it is on a different system - See View::DrawEnding
        // The Ending is added to the System drawing list by View::DrawMeasure
        dc->StartGraphic(element, "boundaryStart", element->GetUuid());
        dc->EndGraphic(element, this);
    }
    else if (element->Is(PB)) {
        dc->StartGraphic(element, "", element->GetUuid());
        dc->EndGraphic(element, this);
    }
    else if (element->Is(SB)) {
        dc->StartGraphic(element, "", element->GetUuid());
        dc->EndGraphic(element, this);
    }
    else if (element->Is(SECTION)) {
        dc->StartGraphic(element, "boundaryStart", element->GetUuid());
        dc->EndGraphic(element, this);
    }
}

void View::DrawEnding(DeviceContext *dc, Ending *ending, System *system)
{
    assert(dc);
    assert(ending);
    assert(system);

    if (dc->Is(BBOX_DEVICE_CONTEXT)) {
        BBoxDeviceContext *bBoxDC = vrv_cast<BBoxDeviceContext *>(dc);
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
    System *parentSystem1 = dynamic_cast<System *>(ending->GetFirstAncestor(SYSTEM));
    System *parentSystem2 = dynamic_cast<System *>(endingEndBoundary->GetFirstAncestor(SYSTEM));

    assert(parentSystem1 && parentSystem2);
    // in non debug mode
    if (!parentSystem1 || !parentSystem2) return;

    int x1, x2;
    Object *objectX;
    Measure *measure = NULL;
    char spanningType = SPANNING_START_END;

    // They both correspond to the current system, which means no system break in-between (simple case)
    if ((system == parentSystem1) && (system == parentSystem2)) {
        measure = ending->GetMeasure();
        x1 = measure->GetDrawingX();
        objectX = measure;
        // if it is the first measure of the system use the left barline position
        if (system->GetFirst(MEASURE) == measure) x1 += measure->GetLeftBarLineXRel();
        x2 = endingEndBoundary->GetMeasure()->GetDrawingX() + endingEndBoundary->GetMeasure()->GetRightBarLineXRel();
    }
    // Only the first parent is the same, this means that the ending is "open" at the end of the system
    else if (system == parentSystem1) {
        // We need the last measure of the system for x2 - we also use it for getting the staves later
        measure = dynamic_cast<Measure *>(system->FindDescendantByType(MEASURE, 1, BACKWARD));
        if (!Check(measure)) return;
        x1 = ending->GetMeasure()->GetDrawingX();
        objectX = measure;
        // if it is the first measure of the system use the left barline position
        if (system->GetFirst(MEASURE) == ending->GetMeasure()) x1 += ending->GetMeasure()->GetLeftBarLineXRel();
        x2 = measure->GetDrawingX() + measure->GetRightBarLineXRel();
        spanningType = SPANNING_START;
    }
    // We are in the system where the ending ends - draw it from the beginning of the system
    else if (system == parentSystem2) {
        // We need the last measure of the system for x2
        measure = dynamic_cast<Measure *>(system->FindDescendantByType(MEASURE, 1, FORWARD));
        if (!Check(measure)) return;
        x1 = measure->GetDrawingX() + measure->GetLeftBarLineXRel();
        objectX = measure->GetLeftBarLine();
        x2 = endingEndBoundary->GetMeasure()->GetDrawingX() + endingEndBoundary->GetMeasure()->GetRightBarLineXRel();
        spanningType = SPANNING_END;
    }
    // Rare case where neither the first note nor the last note are in the current system - draw the connector
    // throughout the system
    else {
        // We need the first measure of the system for x1 - we also use it for getting the staves later
        measure = dynamic_cast<Measure *>(system->FindDescendantByType(MEASURE, 1, FORWARD));
        if (!Check(measure)) return;
        x1 = measure->GetDrawingX() + measure->GetLeftBarLineXRel();
        objectX = measure->GetLeftBarLine();
        // We need the last measure of the system for x2
        measure = dynamic_cast<Measure *>(system->FindDescendantByType(MEASURE, 1, BACKWARD));
        if (!Check(measure)) return;
        x2 = measure->GetDrawingX() + measure->GetRightBarLineXRel();
        spanningType = SPANNING_MIDDLE;
    }

    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START))
        dc->ResumeGraphic(ending, ending->GetUuid());
    else
        dc->StartGraphic(ending, "", ending->GetUuid(), false);

    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList;
    assert(system->GetDrawingScoreDef());
    assert(measure);
    // By default, endings are drawn on top of each group (@ending.rend="gouped") unless "top" is specified)
    if (system->GetDrawingScoreDef()->GetEndingRend() == endings_ENDINGREND_top) {
        Staff *staff = dynamic_cast<Staff *>(system->FindDescendantByType(STAFF, 2, FORWARD));
        if (!Check(staff)) return;
        staffList.push_back(staff);
    }
    else {
        staffList = measure->GetFirstStaffGrpStaves(system->GetDrawingScoreDef());
    }

    for (staffIter = staffList.begin(); staffIter != staffList.end(); ++staffIter) {
        if (!system->SetCurrentFloatingPositioner((*staffIter)->GetN(), ending, objectX, *staffIter)) {
            continue;
        }

        const int y1 = ending->GetDrawingY();

        FontInfo currentFont = *m_doc->GetDrawingLyricFont((*staffIter)->m_drawingStaffSize);
        // currentFont.SetWeight(FONTWEIGHT_bold);
        // currentFont.SetPointSize(currentFont.GetPointSize() * 2 / 3);
        dc->SetFont(&currentFont);

        TextExtend extend;
        dc->GetTextExtent("M", &extend, false);

        if (ending->HasN()) {
            std::stringstream strStream;
            // Maybe we want to add ( ) after system breaks? Or . as a styling options?
            if ((spanningType == SPANNING_END) || (spanningType == SPANNING_MIDDLE)) strStream << "(";
            strStream << ending->GetN(); // << ".";
            if ((spanningType == SPANNING_END) || (spanningType == SPANNING_MIDDLE)) strStream << ")";

            Text text;
            text.SetParent(ending);
            text.SetText(UTF8to16(strStream.str()));

            int textX = x1;
            if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START)) {
                textX += m_doc->GetDrawingUnit((*staffIter)->m_drawingStaffSize) * 2 / 3;
            }

            TextDrawingParams params;
            params.m_x = textX;
            params.m_y = y1;
            params.m_pointSize = currentFont.GetPointSize();

            dc->StartText(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), HORIZONTALALIGNMENT_left);
            DrawTextElement(dc, &text, params);
            dc->EndText();
        }

        dc->ResetFont();

        const int y2 = y1 + extend.m_height + m_doc->GetDrawingUnit((*staffIter)->m_drawingStaffSize) * 2 / 3;
        const int lineWidth = m_options->m_repeatEndingLineThickness.GetValue()
            * m_doc->GetDrawingUnit((*staffIter)->m_drawingStaffSize);

        DrawFilledRectangle(dc, x1, y2, x2, y2 + lineWidth);
        if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START)) {
            DrawFilledRectangle(dc, x1, y1, x1 + lineWidth, y2);
        }
        if (((spanningType == SPANNING_START_END) || (spanningType == SPANNING_END))
            && (ending->GetLendsym() != LINESTARTENDSYMBOL_none)) {
            DrawFilledRectangle(dc, x2 - lineWidth, y1, x2, y2);
        }
    }

    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START))
        dc->EndResumedGraphic(ending, this);
    else
        dc->EndGraphic(ending, this);
}

void View::DrawTextBoxes(DeviceContext *dc, const std::vector<TextElement *> &boxedRend, int staffSize)
{
    assert(dc);
    const int lineThickness = m_options->m_textEnclosureThickness.GetValue() * staffSize;
    const int boxMargin = staffSize / 2;

    for (const auto rend : boxedRend) {
        const int x1 = rend->GetContentLeft() - boxMargin;
        const int y1 = rend->GetContentBottom() - boxMargin;
        const int x2 = rend->GetContentRight() + boxMargin;
        const int y2 = rend->GetContentTop() + 2 * boxMargin;

        DrawNotFilledRectangle(dc, x1, y1, x2, y2, lineThickness, 0);
    }
}

} // namespace vrv
