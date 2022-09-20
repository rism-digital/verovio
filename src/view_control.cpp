/////////////////////////////////////////////////////////////////////////////
// Name:        view_control.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <sstream>

//----------------------------------------------------------------------------

#include "arpeg.h"
#include "bboxdevicecontext.h"
#include "beamspan.h"
#include "bracketspan.h"
#include "breath.h"
#include "caesura.h"
#include "clef.h"
#include "comparison.h"
#include "devicecontext.h"
#include "dir.h"
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
#include "measure.h"
#include "mordent.h"
#include "note.h"
#include "octave.h"
#include "options.h"
#include "page.h"
#include "pedal.h"
#include "pitchinflection.h"
#include "reh.h"
#include "rend.h"
#include "slur.h"
#include "smufl.h"
#include "staff.h"
#include "syl.h"
#include "system.h"
#include "tempo.h"
#include "text.h"
#include "tie.h"
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
    if (element->Is(
            { BEAMSPAN, BRACKETSPAN, FIGURE, GLISS, HAIRPIN, LV, OCTAVE, PHRASE, PITCHINFLECTION, SLUR, TIE })) {
        // create placeholder
        dc->StartGraphic(element, "", element->GetID());
        dc->EndGraphic(element, this);
        system->AddToDrawingList(element);
    }
    else if (element->Is(ARPEG)) {
        Arpeg *arpeg = vrv_cast<Arpeg *>(element);
        assert(arpeg);
        this->DrawArpeg(dc, arpeg, measure, system);
    }
    else if (element->Is(BREATH)) {
        Breath *breath = vrv_cast<Breath *>(element);
        assert(breath);
        this->DrawBreath(dc, breath, measure, system);
    }
    else if (element->Is(CAESURA)) {
        Caesura *caesura = vrv_cast<Caesura *>(element);
        assert(caesura);
        this->DrawCaesura(dc, caesura, measure, system);
    }
    else if (element->Is(DIR)) {
        Dir *dir = vrv_cast<Dir *>(element);
        assert(dir);
        this->DrawDir(dc, dir, measure, system);
        system->AddToDrawingListIfNecessary(dir);
    }
    else if (element->Is(DYNAM)) {
        Dynam *dynam = vrv_cast<Dynam *>(element);
        assert(dynam);
        this->DrawDynam(dc, dynam, measure, system);
        system->AddToDrawingListIfNecessary(dynam);
    }
    else if (element->Is(FERMATA)) {
        Fermata *fermata = vrv_cast<Fermata *>(element);
        assert(fermata);
        this->DrawFermata(dc, fermata, measure, system);
    }
    else if (element->Is(FING)) {
        Fing *fing = vrv_cast<Fing *>(element);
        assert(fing);
        this->DrawFing(dc, fing, measure, system);
    }
    else if (element->Is(HARM)) {
        Harm *harm = vrv_cast<Harm *>(element);
        assert(harm);
        this->DrawHarm(dc, harm, measure, system);
    }
    else if (element->Is(MORDENT)) {
        Mordent *mordent = vrv_cast<Mordent *>(element);
        assert(mordent);
        this->DrawMordent(dc, mordent, measure, system);
    }
    else if (element->Is(PEDAL)) {
        Pedal *pedal = vrv_cast<Pedal *>(element);
        assert(pedal);
        this->DrawPedal(dc, pedal, measure, system);
        system->AddToDrawingListIfNecessary(pedal);
    }
    else if (element->Is(REH)) {
        Reh *reh = vrv_cast<Reh *>(element);
        assert(reh);
        this->DrawReh(dc, reh, measure, system);
    }
    else if (element->Is(TEMPO)) {
        Tempo *tempo = vrv_cast<Tempo *>(element);
        assert(tempo);
        this->DrawTempo(dc, tempo, measure, system);
    }
    else if (element->Is(TRILL)) {
        Trill *trill = vrv_cast<Trill *>(element);
        assert(trill);
        this->DrawTrill(dc, trill, measure, system);
        system->AddToDrawingListIfNecessary(trill);
    }
    else if (element->Is(TURN)) {
        Turn *turn = vrv_cast<Turn *>(element);
        assert(turn);
        this->DrawTurn(dc, turn, measure, system);
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
            if (element->Is({ BRACKETSPAN, HAIRPIN, OCTAVE, PITCHINFLECTION })) return;
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
            // The end link has to have a TimePointInterface, because we ensure it is a ControlElement
            TimePointInterface *nextInterface = linkingInterface->GetNextLink()->GetTimePointInterface();
            assert(nextInterface);
            end = nextInterface->GetStart();
        }
    }

    if (!this->HasValidTimeSpanningOrder(dc, element, start, end)) {
        return;
    }

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
    // throughout the system => recheck that the systems are in correct order
    else if (Object::IsPreOrdered(parentSystem1, system) && Object::IsPreOrdered(system, parentSystem2)) {
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
    // Return otherwise: this should only happen if the time spanning element is encoded in the wrong measure
    else {
        return;
    }

    // Overwrite the spanningType for open ended control events
    // We can identify them because they end on a right barline
    if ((spanningType == SPANNING_START_END) && end->Is(BARLINE)) {
        if (vrv_cast<BarLine *>(end)->GetPosition() == BarLinePosition::Right) {
            spanningType = SPANNING_START;
        }
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
            Staff *staff = *staffIter;
            if (element->Is({ PHRASE, SLUR })) {
                if (this->GetSlurHandling() == SlurHandling::Ignore) break;
                Slur *slur = vrv_cast<Slur *>(element);
                assert(slur);
                staff = slur->CalculateExtremalStaff(staff, x1, x2);
            }

            // Create the floating positioner
            if (!system->SetCurrentFloatingPositioner(
                    staff->GetN(), dynamic_cast<ControlElement *>(element), objectX, staff, spanningType)) {
                continue;
            }
        }

        if (element->Is(DIR)) {
            // cast to Dir check in DrawControlElementConnector
            this->DrawControlElementConnector(
                dc, dynamic_cast<Dir *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is(DYNAM)) {
            // cast to Dynam check in DrawControlElementConnector
            this->DrawControlElementConnector(
                dc, dynamic_cast<Dynam *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is(FIGURE)) {
            // cast to F check in DrawFConnector
            this->DrawFConnector(dc, dynamic_cast<F *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is(BEAMSPAN)) {
            // cast to BeamSpan check in DrawBeamSpan
            this->DrawBeamSpan(dc, vrv_cast<BeamSpan *>(element), system, graphic);
        }
        else if (element->Is(BRACKETSPAN)) {
            // cast to BracketSpan check in DrawBracketSpan
            this->DrawBracketSpan(dc, dynamic_cast<BracketSpan *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is(GLISS)) {
            // For gliss we limit support to one value in @staff
            if (staffIter != staffList.begin()) continue;
            // cast to Gliss check in DrawGliss
            this->DrawGliss(dc, dynamic_cast<Gliss *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is(HAIRPIN)) {
            // cast to Hairpin check in DrawHairpin
            this->DrawHairpin(dc, dynamic_cast<Hairpin *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is(LV)) {
            // For ties we limit support to one value in @staff
            if (staffIter != staffList.begin()) continue;
            // cast to Tie check in DrawTie
            this->DrawTie(dc, dynamic_cast<Tie *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is(PHRASE)) {
            // Check if slurs should be ignored
            if (this->GetSlurHandling() == SlurHandling::Ignore) continue;
            // For phrases (slurs) we limit support to one value in @staff
            if (staffIter != staffList.begin()) continue;
            // cast to Slur check in DrawSlur
            this->DrawSlur(dc, dynamic_cast<Slur *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is(OCTAVE)) {
            // cast to Slur check in DrawOctave
            this->DrawOctave(dc, dynamic_cast<Octave *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is(PEDAL)) {
            this->DrawPedalLine(dc, dynamic_cast<Pedal *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is(PITCHINFLECTION)) {
            // cast to PitchInflection check in DrawPitchInflection
            this->DrawPitchInflection(
                dc, dynamic_cast<PitchInflection *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is(SLUR)) {
            // Check if slurs should be ignored
            if (this->GetSlurHandling() == SlurHandling::Ignore) continue;
            // For slurs we limit support to one value in @staff
            if (staffIter != staffList.begin()) continue;
            // cast to Slur check in DrawSlur
            this->DrawSlur(dc, dynamic_cast<Slur *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is(SYL)) {
            // prolong to the end of the notehead
            x2 += endRadius;
            // cast to Syl check in DrawSylConnector
            this->DrawSylConnector(dc, dynamic_cast<Syl *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is(TIE)) {
            // For ties we limit support to one value in @staff
            if (staffIter != staffList.begin()) continue;
            // cast to Slur check in DrawTie
            this->DrawTie(dc, dynamic_cast<Tie *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
        else if (element->Is(TRILL)) {
            // cast to Trill check in DrawTrill
            this->DrawTrillExtension(dc, dynamic_cast<Trill *>(element), x1, x2, *staffIter, spanningType, graphic);
        }
    }
}

bool View::HasValidTimeSpanningOrder(DeviceContext *dc, Object *element, LayerElement *start, LayerElement *end) const
{
    if (!start || !end) return false;

    TimeSpanningInterface *interface = element->GetTimeSpanningInterface();
    if (interface && !interface->IsOrdered(start, end)) {
        // Handle exceptional cases
        if (element->Is(SLUR)) {
            if (start->GetAlignment() == end->GetAlignment()) {
                if (start->IsGraceNote() || end->IsGraceNote()) {
                    return true;
                }
            }
        }
        else if (element->Is({ OCTAVE, SYL })) {
            return true;
        }
        // To avoid showing the same warning multiple times, display a warning only during actual drawing
        if (!dc->Is(BBOX_DEVICE_CONTEXT) && (m_currentPage == vrv_cast<Page *>(start->GetFirstAncestor(PAGE)))) {
            LogWarning("%s '%s' is ignored, since start '%s' does not occur temporally before end '%s'.",
                element->GetClassName().c_str(), element->GetID().c_str(), start->GetID().c_str(),
                end->GetID().c_str());
        }
        return false;
    }

    return true;
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
        dc->ResumeGraphic(graphic, graphic->GetID());
    }
    else {
        dc->StartGraphic(bracketSpan, "", bracketSpan->GetID(), false);
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
            this->DrawFilledRectangle(dc, x1, y, x1 + bracketSize, y + lineWidth);
        }
        this->DrawFilledRectangle(dc, x1, y, x1 + lineWidth, y - bracketSize);
    }
    // Closing bracket
    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_END)) {
        // Ditto
        if ((bracketSpan->GetLform() != LINEFORM_dashed) && (bracketSpan->GetLform() != LINEFORM_solid)) {
            this->DrawFilledRectangle(dc, x2 - bracketSize, y, x2, y + lineWidth);
        }
        this->DrawFilledRectangle(dc, x2 - lineWidth, y, x2, y - bracketSize);
    }
    // We have a @lform - draw a full line
    if (bracketSpan->HasLform()) {
        if (bracketSpan->GetLform() == LINEFORM_solid) {
            this->DrawFilledRectangle(dc, x1, y, x2, y - lineWidth);
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
            dc->SetPen(m_currentColour, lineWidth, AxDOT, 0, 0, AxCAP_ROUND);
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
    const auto [leftOverlap, rightOverlap] = hairpin->GetBarlineOverlapAdjustment(
        m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize), x1, x2, spanningType);
    x1 += leftOverlap;
    x2 -= rightOverlap;

    hairpin->SetDrawingLength(x2 - x1);

    hairpinLog_FORM form = hairpin->GetForm();

    // For now we calculate everything based on cresc.
    int startY = 0;
    int endY = hairpin->CalcHeight(m_doc, staff->m_drawingStaffSize, spanningType, leftLink, rightLink);

    // To get things right, we need to consider the corresponding spanning type for dim.
    char correspSpanningType = spanningType;
    if (form == hairpinLog_FORM_dim) {
        if (spanningType == SPANNING_START) correspSpanningType = SPANNING_END;
        if (spanningType == SPANNING_END) correspSpanningType = SPANNING_START;
    }

    // Adjust start/end for broken hairpins
    if (correspSpanningType == SPANNING_START) {
        endY = endY * 2 / 3;
    }
    else if (correspSpanningType == SPANNING_END) {
        startY = endY / 3;
    }
    else if (correspSpanningType == SPANNING_MIDDLE) {
        startY = endY / 3;
        endY = endY * 2 / 3;
    }

    // Now swap start/end for dim.
    if (form == hairpinLog_FORM_dim) std::swap(startY, endY);

    int y1 = hairpin->GetDrawingY();
    int y2 = y1;

    // Improve alignment with dynamics
    if (hairpin->GetPlace() != STAFFREL_within) {
        int shiftY = -(m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;
        if (hairpin->GetPlace() != STAFFREL_between) {
            shiftY += m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        }
        y1 += shiftY;
        y2 += shiftY;
    }

    /************** draw it **************/

    if (graphic)
        dc->ResumeGraphic(graphic, graphic->GetID());
    else
        dc->StartGraphic(hairpin, "", hairpin->GetID(), false);
    // dc->DeactivateGraphic();

    const double hairpinThickness
        = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * m_options->m_hairpinThickness.GetValue();
    dc->SetPen(m_currentColour, hairpinThickness, AxSOLID, 0, 0, AxCAP_SQUARE, AxJOIN_BEVEL);
    if (startY == 0) {
        Point p[3];
        p[0] = { ToDeviceContextX(x2), ToDeviceContextY(y2 - endY / 2) };
        p[1] = { ToDeviceContextX(x1), ToDeviceContextY(y1) };
        p[2] = { p[0].x, ToDeviceContextY(y2 + endY / 2) };
        dc->DrawPolyline(3, p);
    }
    else if (endY == 0) {
        Point p[3];
        p[0] = { ToDeviceContextX(x1), ToDeviceContextY(y1 - startY / 2) };
        p[1] = { ToDeviceContextX(x2), ToDeviceContextY(y2) };
        p[2] = { p[0].x, ToDeviceContextY(y1 + startY / 2) };
        dc->DrawPolyline(3, p);
    }
    else {
        Point p[2];
        p[0] = { ToDeviceContextX(x1), ToDeviceContextY(y1 - startY / 2) };
        p[1] = { ToDeviceContextX(x2), ToDeviceContextY(y2 - endY / 2) };
        dc->DrawPolyline(2, p);
        p[0].y = ToDeviceContextY(y1 + startY / 2);
        p[1].y = ToDeviceContextY(y2 + endY / 2);
        dc->DrawPolyline(2, p);
    }
    dc->ResetPen();

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
            if (octave->GetEnd()->HasContentBB()) {
                x2 += octave->GetEnd()->GetContentX2();
            }
        }
    }

    /************** draw it **************/

    if (graphic)
        dc->ResumeGraphic(graphic, graphic->GetID());
    else
        dc->StartGraphic(octave, "", octave->GetID(), false);

    const bool altSymbols = m_doc->GetOptions()->m_octaveAlternativeSymbols.GetValue();
    int code = SMUFL_E511_ottavaAlta;
    if (disPlace == STAFFREL_basic_above) {
        switch (dis) {
            // here we could use other glyphs depending on the style
            case OCTAVE_DIS_8: {
                code = altSymbols ? SMUFL_E511_ottavaAlta : SMUFL_E510_ottava;
                break;
            }
            case OCTAVE_DIS_15: {
                code = altSymbols ? SMUFL_E515_quindicesimaAlta : SMUFL_E514_quindicesima;
                break;
            }
            case OCTAVE_DIS_22: {
                code = altSymbols ? SMUFL_E518_ventiduesimaAlta : SMUFL_E517_ventiduesima;
                break;
            }
            default: break;
        }
    }
    else {
        switch (dis) {
            // ditto
            case OCTAVE_DIS_8: {
                code = altSymbols ? SMUFL_E51C_ottavaBassaVb : SMUFL_E510_ottava;
                break;
            }
            case OCTAVE_DIS_15: {
                code = altSymbols ? SMUFL_E51D_quindicesimaBassaMb : SMUFL_E514_quindicesima;
                break;
            }
            case OCTAVE_DIS_22: {
                code = altSymbols ? SMUFL_E51E_ventiduesimaBassaMb : SMUFL_E517_ventiduesima;
                break;
            }
            default: break;
        }
    }
    std::u32string str;
    str.push_back(code);

    dc->SetFont(m_doc->GetDrawingSmuflFont(staff->m_drawingStaffSize, false));
    TextExtend extend;
    dc->GetSmuflTextExtent(str, &extend);
    const int yCode = (disPlace == STAFFREL_basic_above) ? y1 - extend.m_height : y1;
    const int octaveX = altSymbols ? x1 - extend.m_width / 2 : x1 - extend.m_width;
    this->DrawSmuflCode(dc, octaveX, yCode, code, staff->m_drawingStaffSize, false);
    dc->ResetFont();

    if (octave->GetExtender() != BOOLEAN_false) {
        int lineWidth
            = m_doc->GetOptions()->m_octaveLineThickness.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
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

        // adjust is to avoid the figure to touch the line
        x1 += m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
        if (altSymbols) x1 += extend.m_width / 2;
        y2 += (disPlace == STAFFREL_basic_above) ? -extend.m_height : extend.m_height;
        int lineShift = (disPlace == STAFFREL_basic_above) ? -lineWidth / 2 : lineWidth / 2;

        dc->SetPen(m_currentColour, lineWidth, AxSOLID, extend.m_height / 3);
        dc->SetBrush(m_currentColour, AxSOLID);
        int dotShift = 0;
        if (octave->HasLform()) {
            if (octave->GetLform() == LINEFORM_solid) {
                dc->SetPen(m_currentColour, lineWidth, AxSOLID);
                dc->SetBrush(m_currentColour, AxSOLID);
            }
            else if (octave->GetLform() == LINEFORM_dotted) {
                dc->SetPen(m_currentColour, lineWidth, AxDOT, 0, 0, AxCAP_ROUND);
                dc->SetBrush(m_currentColour, AxSOLID);
                dotShift = lineShift;
            }
        }

        if (x1 > x2) {
            // make sure we have a minimal extender line
            x2 = x1 + extend.m_height / 4;
        }
        dc->DrawLine(ToDeviceContextX(x1), ToDeviceContextY(y1 + lineShift), ToDeviceContextX(x2),
            ToDeviceContextY(y1 + lineShift));

        if (octave->GetLendsym() != LINESTARTENDSYMBOL_none) {
            x2 += lineWidth / 2;
            // draw the ending vertical line if not the end of the system
            if (spanningType == SPANNING_END || spanningType == SPANNING_START_END) {
                dc->DrawLine(
                    ToDeviceContextX(x2), ToDeviceContextY(y1 + dotShift), ToDeviceContextX(x2), ToDeviceContextY(y2));
            }
        }
    }

    if (graphic)
        dc->EndResumedGraphic(graphic, this);
    else
        dc->EndGraphic(octave, this);
}

void View::DrawPitchInflection(DeviceContext *dc, PitchInflection *pitchInflection, int x1, int x2, Staff *staff,
    char spanningType, Object *graphic)
{
    assert(dc);
    assert(pitchInflection);
    assert(staff);

    int topY = staff->GetDrawingY() + m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);

    Note *note1 = dynamic_cast<Note *>(pitchInflection->GetStart());
    // If the start is a note, use it y as base, otherwhise the position above the staff
    int baseY1 = (note1) ? note1->GetDrawingY() : topY;
    Note *note2 = dynamic_cast<Note *>(pitchInflection->GetEnd());
    // If the end is a note, use it y as base, otherwhise the position above the staff
    int baseY2 = (note2) ? note2->GetDrawingY() : topY;

    // If we start on a note, then going up
    bool up = note1 ? true : false;

    int y1 = (up) ? baseY1 : topY;
    int y2 = (up) ? topY : baseY2;
    int xControl = x2;
    int yControl = y1;
    // Always true for now
    bool drawArrow = true;

    // We need to handle system breaks
    if (spanningType == SPANNING_START) {
        drawArrow = false;
        // We need to re-calculate the y2 when going down (we need an end note) because we
        if (!up && note2) {
            // Make it relative to the current (start) staff
            y2 = staff->GetDrawingY() + note2->GetDrawingYRel();
        }
        // Adjust the control points - y2 is in the middle
        y2 -= (y2 - y1) / 2;
        yControl = y1 + (y2 - y1) / 4;
        xControl = x2 - (x2 - x1) / 4;
    }
    else if (spanningType == SPANNING_END) {
        // We need to recalcultate the y1 when going up (we need a start note)
        if (up && note1) {
            // Make it relative to the current (end) staff
            y1 = staff->GetDrawingY() + note1->GetDrawingYRel();
        }
        // Adjust the control points - y1 is in the middle
        y1 += (y2 - y1) / 2;
        yControl = y1 + (y2 - y1) / 4;
        xControl = x2 - (x2 - x1) / 4;
    }
    else if (spanningType == SPANNING_MIDDLE) {
        // For now just skip bend that span over an entire measure since they probably do not exist
        return;
    }

    Point points[3];
    points[0].x = ToDeviceContextX(x1);
    points[0].y = ToDeviceContextY(y1);
    points[1].x = ToDeviceContextX(xControl);
    points[1].y = ToDeviceContextY(yControl);
    points[2].x = ToDeviceContextX(x2);
    points[2].y = ToDeviceContextY(y2);

    int arrowWidth = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2;
    int arrowHeight = arrowWidth * 3 / 2;
    arrowHeight = (up) ? arrowHeight : -arrowHeight;
    Point arrow[3];
    arrow[0].x = ToDeviceContextX(x2 - arrowWidth);
    arrow[0].y = ToDeviceContextY(y2);
    arrow[1].x = ToDeviceContextX(x2 + arrowWidth);
    arrow[1].y = ToDeviceContextY(y2);
    arrow[2].x = ToDeviceContextX(x2);
    arrow[2].y = ToDeviceContextY(y2 + arrowHeight);

    /************** draw it **************/

    if (graphic) {
        dc->ResumeGraphic(graphic, graphic->GetID());
    }
    else {
        dc->StartGraphic(pitchInflection, "spanning-pinflection", "");
    }

    dc->SetPen(m_currentColour, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize), AxSOLID);
    dc->SetBrush(m_currentColour, AxSOLID);

    dc->DrawQuadBezierPath(points);
    if (drawArrow) {
        dc->DrawPolygon(3, arrow);
    }

    dc->ResetPen();
    dc->ResetBrush();

    if (graphic) {
        dc->EndResumedGraphic(graphic, this);
    }
    else {
        dc->EndGraphic(pitchInflection, this);
    }
}

void View::DrawTie(DeviceContext *dc, Tie *tie, int x1, int x2, Staff *staff, char spanningType, Object *graphic)
{
    assert(dc);
    assert(tie);
    assert(staff);

    Point bezier[4];
    if (!tie->CalculatePosition(m_doc, staff, x1, x2, spanningType, bezier)) return;

    int penStyle = AxSOLID;
    switch (tie->GetLform()) {
        case LINEFORM_dashed: penStyle = AxSHORT_DASH; break;
        case LINEFORM_dotted: penStyle = AxDOT; break;
        default: break;
    }

    if (graphic)
        dc->ResumeGraphic(graphic, graphic->GetID());
    else
        dc->StartGraphic(tie, "", tie->GetID(), false);

    // set pen width and calculate tie thickness coefficient to adjust tie width in according to it
    const int thickness
        = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * m_doc->GetOptions()->m_tieMidpointThickness.GetValue();
    const int penWidth
        = m_doc->GetOptions()->m_tieEndpointThickness.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    const double thicknessCoefficient = BoundingBox::GetBezierThicknessCoefficient(bezier, thickness, penWidth);
    this->DrawThickBezierCurve(
        dc, bezier, thicknessCoefficient * thickness, staff->m_drawingStaffSize, penWidth, penStyle);
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
        dc->ResumeGraphic(graphic, graphic->GetID());
    }
    else {
        dc->StartGraphic(pedal, "", pedal->GetID(), false);
    }

    const int bracketSize = m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
    const int lineWidth = m_options->m_pedalLineThickness.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);

    // Opening bracket
    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START)) {
        this->DrawFilledRectangle(dc, x1, y, x1 + bracketSize / 2, y + lineWidth);
        this->DrawFilledRectangle(dc, x1, y, x1 + lineWidth, y + bracketSize);
    }
    // Closing bracket
    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_END)) {
        this->DrawFilledRectangle(dc, x2 - bracketSize / 2, y, x2, y + lineWidth);
        this->DrawFilledRectangle(dc, x2 - lineWidth, y, x2, y + bracketSize);
    }
    this->DrawFilledRectangle(dc, x1 + bracketSize / 2, y, x2 - bracketSize / 2, y + lineWidth);

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
        dc->ResumeGraphic(graphic, graphic->GetID());
    else
        dc->StartGraphic(trill, "", trill->GetID(), false);

    this->DrawSmuflLine(dc, orig, length, staff->m_drawingStaffSize, false, SMUFL_E59D_ornamentZigZagLineNoRightEnd, 0,
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
    const int y = element->GetDrawingY() + width / 2;

    const int unit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    // the length of the dash and the space between them - can be made a parameter
    const int dashSpace = m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize) * 5 / 3;
    const int minDashSpace = m_doc->GetOptions()->m_extenderLineMinSpace.GetValue() * unit;
    const int halfDashLength = unit * 2 / 3;

    int dist = x2 - x1;
    int nbDashes = dist / dashSpace;

    int margin = dist / 2;
    // no dash if the distance is smaller than a dash length
    if (dist < minDashSpace) {
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
        dc->ResumeGraphic(graphic, graphic->GetID());
    }
    else {
        dc->StartGraphic(element, "", element->GetID(), false);
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

        this->DrawFilledRectangle(dc, x - halfDashLength, y, x + halfDashLength, y + width);
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

    const int y = this->GetFYRel(f, staff);
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

    // Because Syl is not a ControlElement (FloatingElement) with FloatingPositioner we need to instantiate a
    // temporary object in order not to reset the Syl bounding box.
    F fConnector;
    if (graphic) {
        dc->ResumeGraphic(graphic, graphic->GetID());
    }
    else
        dc->StartGraphic(&fConnector, "", f->GetID(), false);

    dc->DeactivateGraphic();

    int width = m_options->m_lyricLineThickness.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    // Adjust it proportionally to the lyric size
    width *= m_options->m_lyricSize.GetValue() / m_options->m_lyricSize.GetDefault();
    this->DrawFilledRectangle(dc, x1, y, x2, y + width);

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

    const int y = staff->GetDrawingY() + this->GetSylYRel(syl->m_drawingVerse, staff);

    // Invalid bounding boxes might occur for empty syllables without text child
    if (!syl->HasContentHorizontalBB()) return;
    if (syl->m_nextWordSyl && !syl->m_nextWordSyl->HasContentHorizontalBB()) return;

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

    // Because Syl is not a ControlElement (FloatingElement) with FloatingPositioner we need to instantiate a
    // temporary object in order not to reset the Syl bounding box.
    Syl sylConnector;
    if (graphic) {
        dc->ResumeGraphic(graphic, graphic->GetID());
    }
    else
        dc->StartGraphic(&sylConnector, "", syl->GetID(), false);

    dc->DeactivateGraphic();

    this->DrawSylConnectorLines(dc, x1, x2, y, syl, staff);

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
        const int halfDashLength = dashLength / 2;

        const int dashSpace = m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize) * 5 / 3;
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

            this->DrawFilledRectangle(dc, x - halfDashLength, y, x + halfDashLength, y + thickness);
        }
        // this->DrawFilledRectangle(dc, x1, y, x2, y + width);
    }
    else if (syl->GetCon() == sylLog_CON_u) {
        x1 += (int)m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2;
        if (x2 > x1) {
            this->DrawFilledRectangle(dc, x1, y, x2, y + thickness);
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

    const int top = topNote->GetDrawingY();
    const int bottom = bottomNote->GetDrawingY();

    // We arbitrarily look at the top note
    Staff *staff = topNote->GetAncestorStaff();
    const bool drawingCueSize = topNote->GetDrawingCueSize();

    // We are going to have only one FloatingPositioner - staff will be the top note one
    if (!system->SetCurrentFloatingPositioner(staff->GetN(), arpeg, topNote, staff)) {
        return;
    }
    // Special case: because the positioner objects are reset in ResetVerticalAlignment we
    // need to reset the value of the DrawingXRel each time. The value is stored in Arpeg.
    arpeg->GetCurrentFloatingPositioner()->SetDrawingXRel(arpeg->GetDrawingXRel());

    int length = top - bottom;
    // We add - substract a unit in order to have the line going to the edge
    const int unit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    const int x = arpeg->GetDrawingX();
    const int y = bottom - unit;

    const arpegLog_ORDER order = arpeg->GetOrder();
    if (order == arpegLog_ORDER_nonarp) {
        dc->StartGraphic(arpeg, "", arpeg->GetID());
        const int offset = unit / 2;
        const int thickness = m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
        this->DrawSquareBracket(dc, true, x - unit, bottom - offset, length + 2 * offset, unit, thickness, thickness);
        dc->EndGraphic(arpeg, this);
    }
    else {
        length += 2 * unit;
        char32_t startGlyph = SMUFL_EAA9_wiggleArpeggiatoUp;
        char32_t fillGlyph = SMUFL_EAA9_wiggleArpeggiatoUp;
        char32_t endGlyph = (arpeg->GetArrow() == BOOLEAN_true) ? SMUFL_EAAD_wiggleArpeggiatoUpArrow : 0;

        if (order == arpegLog_ORDER_down) {
            startGlyph = (arpeg->GetArrow() == BOOLEAN_true) ? SMUFL_EAAE_wiggleArpeggiatoDownArrow : 0;
            fillGlyph = SMUFL_EAAA_wiggleArpeggiatoDown;
            endGlyph = SMUFL_EAAA_wiggleArpeggiatoDown;
        }

        if (arpeg->GetArrowShape() == LINESTARTENDSYMBOL_none) endGlyph = 0;

        Point orig(x, y);

        dc->StartGraphic(arpeg, "", arpeg->GetID());

        // Smufl glyphs are horizontal - Rotate them counter clockwise
        const int angle = -90;
        dc->RotateGraphic(Point(ToDeviceContextX(x), ToDeviceContextY(y)), angle);

        this->DrawSmuflLine(
            dc, orig, length, staff->m_drawingStaffSize, drawingCueSize, fillGlyph, startGlyph, endGlyph);

        dc->EndGraphic(arpeg, this);

        // Possibly draw enclosing brackets
        this->DrawArpegEnclosing(dc, arpeg, staff, startGlyph, fillGlyph, endGlyph, x, y, length, drawingCueSize);
    }
}

void View::DrawArpegEnclosing(DeviceContext *dc, Arpeg *arpeg, Staff *staff, char32_t startGlyph, char32_t fillGlyph,
    char32_t endGlyph, int x, int y, int height, bool cueSize)
{
    assert(dc);
    assert(arpeg);
    assert(staff);

    if ((arpeg->GetEnclose() == ENCLOSURE_brack) || (arpeg->GetEnclose() == ENCLOSURE_box)) {
        // Calculate position and width
        const int unit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        int width = m_doc->GetGlyphHeight(fillGlyph, staff->m_drawingStaffSize, cueSize);
        int exceedingWidth = std::max(unit - width, 0);
        if (arpeg->GetArrow() == BOOLEAN_true) {
            int arrowWidth = 0;
            if (arpeg->GetOrder() == arpegLog_ORDER_down) {
                arrowWidth = m_doc->GetGlyphHeight(startGlyph, staff->m_drawingStaffSize, cueSize);
            }
            else {
                arrowWidth = m_doc->GetGlyphHeight(endGlyph, staff->m_drawingStaffSize, cueSize);
            }
            exceedingWidth = std::max(exceedingWidth, arrowWidth - width);
        }
        x -= (width + exceedingWidth / 2);
        width += exceedingWidth;

        // We use overlapping brackets to draw boxes :)
        // Set params for offset, bracket width and line thickness
        const int offset = 3 * unit / 4;
        const int bracketWidth = (arpeg->GetEnclose() == ENCLOSURE_brack) ? unit : (width + offset);
        const int verticalThickness = m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
        const int horizontalThickness = ((arpeg->GetEnclose() == ENCLOSURE_brack) ? 2 : 1) * verticalThickness;

        // Draw the brackets
        dc->StartGraphic(arpeg, "", arpeg->GetID());
        this->DrawEnclosingBrackets(
            dc, x, y, height, width, offset, bracketWidth, horizontalThickness, verticalThickness);
        dc->EndGraphic(arpeg, this);
    }
    else if (arpeg->HasEnclose() && (arpeg->GetEnclose() != ENCLOSURE_none)) {
        LogWarning("Only drawing of enclosing brackets and boxes is supported for arpeggio.");
    }
}

void View::DrawBreath(DeviceContext *dc, Breath *breath, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(breath);

    // Cannot draw a breath that has no start position
    if (!breath->GetStart()) return;

    dc->StartGraphic(breath, "", breath->GetID());

    int x = breath->GetStart()->GetDrawingX() + breath->GetStart()->GetDrawingRadius(m_doc);

    // use breath mark comma glyph
    int code = SMUFL_E4CE_breathMarkComma;

    std::u32string str;
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
        this->DrawSmuflString(dc, x, y, str, alignment, (*staffIter)->m_drawingStaffSize);
        dc->ResetFont();
    }

    dc->EndGraphic(breath, this);
}

void View::DrawCaesura(DeviceContext *dc, Caesura *caesura, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(caesura);

    // Cannot draw a caesura that has no start position
    if (!caesura->GetStart()) return;

    dc->StartGraphic(caesura, "", caesura->GetID());

    const char32_t code = caesura->GetCaesuraGlyph();
    const int x = caesura->GetStart()->GetDrawingX() + caesura->GetStart()->GetDrawingRadius(m_doc) * 3;

    std::vector<Staff *> staffList = caesura->GetTstampStaves(measure, caesura);
    for (Staff *staff : staffList) {
        if (!system->SetCurrentFloatingPositioner(staff->GetN(), caesura, caesura->GetStart(), staff)) {
            continue;
        }

        const int glyphHeight = m_doc->GetGlyphHeight(code, staff->m_drawingStaffSize, false);
        const int y = (caesura->HasPlace() && (caesura->GetPlace() != STAFFREL_within))
            ? caesura->GetDrawingY()
            : staff->GetDrawingY() - glyphHeight / 2;

        this->DrawSmuflCode(dc, x, y, code, staff->m_drawingStaffSize, false);
    }

    dc->EndGraphic(caesura, this);
}

void View::DrawDir(DeviceContext *dc, Dir *dir, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(dir);

    // Cannot draw a dir that has no start position
    if (!dir->GetStart()) return;

    dc->StartGraphic(dir, "", dir->GetID());

    FontInfo dirTxt;
    if (!dc->UseGlobalStyling()) {
        dirTxt.SetFaceName("Times");
        dirTxt.SetStyle(FONTSTYLE_italic);
    }

    TextDrawingParams params;

    const int lineCount = dir->GetNumberOfLines(dir);

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

        params.m_enclosedRend.clear();
        params.m_y = dir->GetDrawingY();
        params.m_pointSize = m_doc->GetDrawingLyricFont((*staffIter)->m_drawingStaffSize)->GetPointSize();

        int xAdjust = 0;
        const bool isBetweenStaves = (dir->GetPlace() == STAFFREL_between)
            || ((dir->GetPlace() == STAFFREL_below) && (*staffIter != measure->GetLast(STAFF)))
            || ((dir->GetPlace() == STAFFREL_above) && (*staffIter != measure->GetFirst(STAFF)));
        if (isBetweenStaves
            && (dir->GetStart()->GetAlignment()->GetTime()
                == measure->m_measureAligner.GetRightBarLineAlignment()->GetTime())
            && dir->AreChildrenAlignedTo(HORIZONTALALIGNMENT_right)) {
            xAdjust = m_doc->GetDrawingUnit((*staffIter)->m_drawingStaffSize) / 2;
        }

        dirTxt.SetPointSize(params.m_pointSize);

        if ((dir->GetPlace() == STAFFREL_between) || (dir->GetPlace() == STAFFREL_within)) {
            if (lineCount > 1) {
                params.m_y += (m_doc->GetTextLineHeight(&dirTxt, false) * (lineCount - 1) / 2);
            }
            params.m_y -= m_doc->GetTextXHeight(&dirTxt, false) / 2;
        }

        dc->SetBrush(m_currentColour, AxSOLID);
        dc->SetFont(&dirTxt);

        dc->StartText(ToDeviceContextX(params.m_x - xAdjust), ToDeviceContextY(params.m_y), alignment);
        DrawTextChildren(dc, dir, params);
        dc->EndText();

        dc->ResetFont();
        dc->ResetBrush();

        this->DrawTextEnclosure(dc, params, (*staffIter)->m_drawingStaffSize);
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

    dc->StartGraphic(dynam, "", dynam->GetID());

    bool isSymbolOnly = dynam->IsSymbolOnly();

    FontInfo dynamTxt;
    if (!dc->UseGlobalStyling()) {
        dynamTxt.SetFaceName("Times");
        dynamTxt.SetStyle(FONTSTYLE_italic);
    }

    TextDrawingParams params;

    const int lineCount = dynam->GetNumberOfLines(dynam);

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

        params.m_enclosedRend.clear();
        params.m_y = dynam->GetDrawingY();
        params.m_pointSize = m_doc->GetDrawingLyricFont((*staffIter)->m_drawingStaffSize)->GetPointSize();
        if (dynam->HasEnclose()) {
            params.m_textEnclose = dynam->GetEnclose();
        }

        dynamTxt.SetPointSize(params.m_pointSize);

        if (dynam->GetPlace() == STAFFREL_between) {
            if (lineCount > 1) {
                params.m_y += (m_doc->GetTextLineHeight(&dynamTxt, false) * (lineCount - 1) / 2);
            }
            params.m_y -= m_doc->GetTextXHeight(&dynamTxt, false) / 2;
        }

        // If the dynamic is a symbol (pp, mf, etc.) draw it as one SMuFL string. This will not take into account
        // editorial element within the dynam as it would with text. Also, it is center only if it is a symbol.
        if (isSymbolOnly) {
            const bool singleGlyphs = m_doc->GetOptions()->m_dynamSingleGlyphs.GetValue();
            std::u32string dynamSymbol = dynam->GetSymbolStr(singleGlyphs);
            this->DrawDynamSymbolOnly(dc, *staffIter, dynam, dynamSymbol, alignment, params);
        }
        else {
            dc->SetBrush(m_currentColour, AxSOLID);
            dc->SetFont(&dynamTxt);

            dc->StartText(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), alignment);
            this->DrawTextChildren(dc, dynam, params);
            dc->EndText();

            dc->ResetFont();
            dc->ResetBrush();
        }
        this->DrawTextEnclosure(dc, params, (*staffIter)->m_drawingStaffSize);
    }

    dc->EndGraphic(dynam, this);
}

void View::DrawDynamSymbolOnly(DeviceContext *dc, Staff *staff, Dynam *dynam, const std::u32string &dynamSymbol,
    data_HORIZONTALALIGNMENT alignment, TextDrawingParams &params)
{
    assert(dc);
    assert(staff);
    assert(dynam);

    dc->SetFont(m_doc->GetDrawingSmuflFont(staff->m_drawingStaffSize, false));

    char32_t enclosingFront, enclosingBack;
    std::tie(enclosingFront, enclosingBack) = dynam->GetEnclosingGlyphs();

    // calculate width of the symbol-only dynamic; generally it consists of only one symbol, but in case when it's
    // combination of different dynamics glyphs we need to get total width of all elements
    const int left = m_doc->GetGlyphLeft(dynamSymbol.at(0), staff->m_drawingStaffSize, false);
    int width = 0;
    for (int i = 0; i < (int)dynamSymbol.size(); ++i) {
        if (i == (int)dynamSymbol.size() - 1) {
            width += m_doc->GetGlyphRight(dynamSymbol.at(i), staff->m_drawingStaffSize, false);
        }
        else {
            width += m_doc->GetGlyphAdvX(dynamSymbol.at(i), staff->m_drawingStaffSize, false);
        }
    }

    // draw opening symbol
    const int unit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    if (enclosingFront) {
        std::u32string open;
        open.push_back(enclosingFront);

        this->DrawSmuflString(dc, params.m_x, params.m_y + unit, open, alignment, staff->m_drawingStaffSize);
        params.m_x += m_doc->GetGlyphWidth(enclosingFront, staff->m_drawingStaffSize, false) - left + unit / 6;
    }

    // draw dynamics itself
    this->DrawSmuflString(dc, params.m_x, params.m_y, dynamSymbol, alignment, staff->m_drawingStaffSize);

    // draw closing symbol
    if (enclosingBack) {
        std::u32string close;
        close.push_back(enclosingBack);

        params.m_x += width + unit / 6;
        this->DrawSmuflString(dc, params.m_x, params.m_y + unit, close, alignment, staff->m_drawingStaffSize);
    }

    dc->ResetFont();
}

void View::DrawFb(DeviceContext *dc, Staff *staff, Fb *fb, TextDrawingParams &params)
{
    assert(dc);
    assert(fb);

    dc->StartGraphic(fb, "", fb->GetID());

    FontInfo *fontDim = m_doc->GetDrawingLyricFont(staff->m_drawingStaffSize);
    const int lineHeight = m_doc->GetTextLineHeight(fontDim, false);
    const int startX = params.m_x;

    fontDim->SetPointSize(m_doc->GetDrawingLyricFont((staff)->m_drawingStaffSize)->GetPointSize());

    dc->SetBrush(m_currentColour, AxSOLID);
    dc->SetFont(fontDim);

    for (auto current : fb->GetChildren()) {
        dc->StartText(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), HORIZONTALALIGNMENT_left);
        if (current->Is(FIGURE)) {
            // dynamic_cast assert in DrawF
            this->DrawF(dc, dynamic_cast<F *>(current), params);
        }
        else if (current->IsEditorialElement()) {
            // cast to EditorialElement check in DrawLayerEditorialElement
            this->DrawFbEditorialElement(dc, dynamic_cast<EditorialElement *>(current), params);
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

    const bool drawingCueSize = false;

    dc->StartGraphic(fermata, "", fermata->GetID());

    const char32_t code = fermata->GetFermataGlyph();
    char32_t enclosingFront, enclosingBack;
    std::tie(enclosingFront, enclosingBack) = fermata->GetEnclosingGlyphs();

    const int x = fermata->GetStart()->GetDrawingX() + fermata->GetStart()->GetDrawingRadius(m_doc);

    std::vector<Staff *> staffList = fermata->GetTstampStaves(measure, fermata);
    for (Staff *staff : staffList) {
        if (!system->SetCurrentFloatingPositioner(staff->GetN(), fermata, fermata->GetStart(), staff)) {
            continue;
        }

        const int y = fermata->GetDrawingY();

        // The correction for centering the glyph
        const int xCorr = m_doc->GetGlyphWidth(code, staff->m_drawingStaffSize, drawingCueSize) / 2;
        int yCorr = 0;
        const int height = m_doc->GetGlyphHeight(code, staff->m_drawingStaffSize, drawingCueSize);
        const data_VERTICALALIGNMENT yAlignment = Fermata::GetVerticalAlignment(code);
        int enclosureYCorr = 0;
        if (yAlignment == VERTICALALIGNMENT_top) {
            yCorr = height / 2;
        }
        else if (yAlignment == VERTICALALIGNMENT_bottom) {
            yCorr = -height / 2;
        }
        else {
            const int glyphBottomY = m_doc->GetGlyphBottom(code, staff->m_drawingStaffSize, false);
            if (fermata->GetPlace() == STAFFREL_above) {
                yCorr = height / 2 + glyphBottomY;
            }
            else {
                enclosureYCorr = height / 2 + glyphBottomY;
            }
        }

        // Draw glyph including possible enclosing brackets
        dc->SetFont(m_doc->GetDrawingSmuflFont(staff->m_drawingStaffSize, drawingCueSize));

        if (enclosingFront) {
            const int xCorrEncl = xCorr + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 3
                + m_doc->GetGlyphWidth(enclosingFront, staff->m_drawingStaffSize, drawingCueSize);
            this->DrawSmuflCode(dc, x - xCorrEncl, y + enclosureYCorr + yCorr, enclosingFront,
                staff->m_drawingStaffSize, drawingCueSize);
        }

        this->DrawSmuflCode(dc, x - xCorr, y, code, staff->m_drawingStaffSize, drawingCueSize);

        if (enclosingBack) {
            const int xCorrEncl = xCorr + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 3;
            this->DrawSmuflCode(dc, x + xCorrEncl, y + enclosureYCorr + yCorr, enclosingBack, staff->m_drawingStaffSize,
                drawingCueSize);
        }

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

    dc->StartGraphic(fing, "", fing->GetID());

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

        params.m_enclosedRend.clear();
        params.m_y = fing->GetDrawingY();
        params.m_pointSize = m_doc->GetFingeringFont((*staffIter)->m_drawingStaffSize)->GetPointSize();

        fingTxt.SetPointSize(params.m_pointSize);

        dc->SetBrush(m_currentColour, AxSOLID);
        dc->SetFont(&fingTxt);

        dc->StartText(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), alignment);
        this->DrawTextChildren(dc, fing, params);
        dc->EndText();

        dc->ResetFont();
        dc->ResetBrush();

        this->DrawTextEnclosure(dc, params, (*staffIter)->m_drawingStaffSize);
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

    /************** parent layers **************/

    Note *note1 = dynamic_cast<Note *>(gliss->GetStart());
    Note *note2 = dynamic_cast<Note *>(gliss->GetEnd());

    if (!note1 || !note2) {
        // no note, obviously nothing to do...
        // this also means that notes with tstamp events are not supported
        return;
    }

    const int unit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    const int firstLoc = note1->GetDrawingLoc();
    const int secondLoc = note2->GetDrawingLoc();
    const int diff = (secondLoc - firstLoc) * unit;
    double angle = atan2(diff, (double)(x2 - x1));

    // only half at system breaks
    if (spanningType != SPANNING_START_END) angle = angle / 2;

    // the normal case
    if (spanningType == SPANNING_START_END || spanningType == SPANNING_START) {
        double slope = 0.0;
        if (x1 != x2) slope = diff / (double)(x2 - x1);
        int offset = note1->GetDrawingRadius(m_doc) + unit;
        if ((note1->GetDots() > 0) && (abs(slope) < 1.0)) {
            offset += 1.5 * unit * note1->GetDots();
        }
        x1 += cos(angle) * offset;
        y1 = note1->GetDrawingY() + offset * sin(angle);
    }
    else {
        y1 = note2->GetDrawingY() - (x2 - x1) * sin(angle);
    }
    if (spanningType == SPANNING_START_END || spanningType == SPANNING_END) {
        Accid *accid = note2->GetDrawingAccid();
        if (accid && (accid->GetAccid() != ACCIDENTAL_WRITTEN_NONE)) {
            const int dist = x2 - accid->GetContentLeft() + 0.5 * unit;
            x2 -= dist;
            y2 = note2->GetDrawingY() - dist * tan(angle);
            while (((firstLoc > secondLoc) && (y2 + 0.5 * unit * sin(angle) > accid->GetContentTop()))
                || ((secondLoc > firstLoc) && (y2 + 0.5 * unit * sin(angle) < accid->GetContentBottom()))) {
                y2 += unit * sin(angle);
                x2 += unit * cos(angle);
            }
        }
        else {
            const int offset = note2->GetDrawingRadius(m_doc) + unit;
            x2 -= cos(angle) * offset;
            y2 = note2->GetDrawingY() - offset * sin(angle);
        }
    }
    else {
        // shorten it
        x2 -= unit;
        y2 = y1 + (x2 - x1) * sin(angle);
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
        dc->ResumeGraphic(graphic, graphic->GetID());
    }
    else {
        dc->StartGraphic(gliss, "", gliss->GetID(), false);
    }

    switch (gliss->GetLform()) {
        case LINEFORM_wavy: {
            const int length = static_cast<int>(hypot(x2 - x1, y2 - y1));
            const double angle = RadToDeg(atan2(y1 - y2, x2 - x1));
            // Smufl glyphs are horizontal - Rotate them counter clockwise
            dc->RotateGraphic(Point(ToDeviceContextX(x1), ToDeviceContextY(y1)), angle);

            const char32_t glissGlyph = SMUFL_EAAF_wiggleGlissando;
            const int height = m_doc->GetGlyphHeight(glissGlyph, staff->m_drawingStaffSize, false);
            const Point orig(x1, y1 - height / 2);
            this->DrawSmuflLine(dc, orig, length, staff->m_drawingStaffSize, false, glissGlyph);

            break;
        }
        case LINEFORM_solid:
        default: {
            // only solid lines for now
            this->DrawRoundedLine(dc, x1, y1, x2, y2, lineWidth);
            break;
        }
    }

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

    dc->StartGraphic(harm, "", harm->GetID());

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

        params.m_enclosedRend.clear();
        params.m_y = harm->GetDrawingY();

        if (harm->GetFirst() && harm->GetFirst()->Is(FB)) {
            this->DrawFb(dc, *staffIter, dynamic_cast<Fb *>(harm->GetFirst()), params);
        }
        else {
            params.m_pointSize = m_doc->GetDrawingLyricFont((*staffIter)->m_drawingStaffSize)->GetPointSize();

            harmTxt.SetPointSize(params.m_pointSize);

            dc->SetBrush(m_currentColour, AxSOLID);
            dc->SetFont(&harmTxt);

            dc->StartText(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), alignment);
            this->DrawTextChildren(dc, harm, params);
            dc->EndText();

            dc->ResetFont();
            dc->ResetBrush();

            this->DrawTextEnclosure(dc, params, (*staffIter)->m_drawingStaffSize);
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

    dc->StartGraphic(mordent, "", mordent->GetID());

    const int x = mordent->GetStart()->GetDrawingX() + mordent->GetStart()->GetDrawingRadius(m_doc);

    // set mordent glyph
    const int code = mordent->GetMordentGlyph();

    std::u32string str;
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
            char32_t accid = Accid::GetAccidGlyph(mordent->GetAccidlower());
            std::u32string accidStr;
            accidStr.push_back(accid);
            dc->SetFont(m_doc->GetDrawingSmuflFont((*staffIter)->m_drawingStaffSize, false));
            this->DrawSmuflString(
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
            char32_t accid = Accid::GetAccidGlyph(mordent->GetAccidupper());
            std::u32string accidStr;
            accidStr.push_back(accid);
            dc->SetFont(m_doc->GetDrawingSmuflFont((*staffIter)->m_drawingStaffSize, false));
            this->DrawSmuflString(
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
        this->DrawSmuflString(dc, drawingX, y, str, HORIZONTALALIGNMENT_left, (*staffIter)->m_drawingStaffSize);
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

    dc->StartGraphic(pedal, "", pedal->GetID());

    pedalVis_FORM form = pedal->GetPedalForm(m_doc, system);

    // Draw a symbol, if it's not a line
    if (form != pedalVis_FORM_line) {

        bool bounceStar = true;
        if (form == pedalVis_FORM_altpedstar) bounceStar = false;

        int x = pedal->GetStart()->GetDrawingX() + pedal->GetStart()->GetDrawingRadius(m_doc);

        data_HORIZONTALALIGNMENT alignment = HORIZONTALALIGNMENT_center;
        // center the pedal only with @startid
        if (pedal->GetStart()->Is(TIMESTAMP_ATTR)) {
            if (pedal->GetStart()->GetAlignment()->GetTime()
                == measure->m_measureAligner.GetRightBarLineAlignment()->GetTime()) {
                alignment = HORIZONTALALIGNMENT_right;
            }
            else {
                alignment = HORIZONTALALIGNMENT_left;
            }
        }

        std::vector<Staff *>::iterator staffIter;
        std::vector<Staff *> staffList = pedal->GetTstampStaves(measure, pedal);

        int code = SMUFL_E655_keyboardPedalUp;
        std::u32string str;
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
            this->DrawSmuflString(dc, x, y, str, alignment, (*staffIter)->m_drawingStaffSize);
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

    dc->StartGraphic(reh, "", reh->GetID());

    FontInfo rehTxt;
    if (!dc->UseGlobalStyling()) {
        rehTxt.SetFaceName("Times");
        rehTxt.SetWeight(FONTWEIGHT_bold);
    }

    TextDrawingParams params;

    params.m_x = reh->GetStart()->GetDrawingX();
    const bool adjustPosition = ((reh->HasTstamp() && (reh->GetTstamp() == 0.0))
        || (reh->GetStart()->Is(BARLINE)
            && vrv_cast<BarLine *>(reh->GetStart())->GetPosition() == BarLinePosition::Left));
    if ((system->GetFirst(MEASURE) == measure) && adjustPosition) {
        // StaffDef information is always in the first layer
        Layer *layer = dynamic_cast<Layer *>(measure->FindDescendantByType(LAYER));
        assert(layer);
        if (!system->IsFirstOfMdiv()) {
            if (Clef *clef = layer->GetStaffDefClef(); clef) {
                params.m_x = clef->GetDrawingX() + (clef->GetContentRight() - clef->GetContentLeft()) / 2;
            }
        }
        else {
            if (MeterSig *metersig = layer->GetStaffDefMeterSig(); metersig) {
                params.m_x = metersig->GetDrawingX() + (metersig->GetContentRight() - metersig->GetContentLeft()) / 2;
            }
        }
    }

    data_HORIZONTALALIGNMENT alignment = reh->GetChildRendAlignment();
    // Rehearsal marks are center aligned by default;
    if (alignment == 0) alignment = HORIZONTALALIGNMENT_center;

    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList = reh->GetTstampStaves(measure, reh);
    for (staffIter = staffList.begin(); staffIter != staffList.end(); ++staffIter) {
        if (!system->SetCurrentFloatingPositioner((*staffIter)->GetN(), reh, reh->GetStart(), *staffIter)) {
            continue;
        }

        params.m_enclosedRend.clear();
        params.m_y = reh->GetDrawingY() + 3 * m_doc->GetDrawingUnit((*staffIter)->m_drawingStaffSize);
        params.m_pointSize = m_doc->GetDrawingLyricFont((*staffIter)->m_drawingStaffSize)->GetPointSize();

        rehTxt.SetPointSize(params.m_pointSize);

        dc->SetBrush(m_currentColour, AxSOLID);
        dc->SetFont(&rehTxt);

        dc->StartText(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), alignment);
        this->DrawTextChildren(dc, reh, params);
        dc->EndText();

        dc->ResetFont();
        dc->ResetBrush();

        this->DrawTextEnclosure(dc, params, (*staffIter)->m_drawingStaffSize);
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

    dc->StartGraphic(tempo, "", tempo->GetID());

    FontInfo tempoTxt;
    if (!dc->UseGlobalStyling()) {
        tempoTxt.SetFaceName("Times");
        tempoTxt.SetWeight(FONTWEIGHT_bold);
    }

    int lineCount = tempo->GetNumberOfLines(tempo);

    TextDrawingParams params;
    data_HORIZONTALALIGNMENT alignment = tempo->GetChildRendAlignment();
    // Tempo are left aligned by default;
    if (alignment == 0) alignment = HORIZONTALALIGNMENT_left;

    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList = tempo->GetTstampStaves(measure, tempo);
    for (staffIter = staffList.begin(); staffIter != staffList.end(); ++staffIter) {
        if (!system->SetCurrentFloatingPositioner((*staffIter)->GetN(), tempo, tempo->GetStart(), *staffIter)) {
            continue;
        }

        params.m_x = tempo->GetDrawingXRelativeToStaff((*staffIter)->GetN());
        params.m_enclosedRend.clear();
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
        this->DrawTextChildren(dc, tempo, params);
        dc->EndText();

        dc->ResetFont();
        dc->ResetBrush();

        this->DrawTextEnclosure(dc, params, (*staffIter)->m_drawingStaffSize);
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

    dc->StartGraphic(trill, "", trill->GetID());

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
    std::u32string str;

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
            char32_t accid = Accid::GetAccidGlyph(trill->GetAccidlower());
            std::u32string accidStr;
            accidStr.push_back(accid);
            dc->SetFont(m_doc->GetDrawingSmuflFont((*staffIter)->m_drawingStaffSize, false));
            int accidY = y - m_doc->GetGlyphHeight(accid, (*staffIter)->m_drawingStaffSize, true) / 2;
            this->DrawSmuflString(dc, x + accidXShift, accidY, accidStr, HORIZONTALALIGNMENT_center,
                (*staffIter)->m_drawingStaffSize / 2, false);
        }
        else if (trill->HasAccidupper()) {
            int accidXShift = (alignment == HORIZONTALALIGNMENT_center)
                ? 0
                : m_doc->GetGlyphWidth(code, (*staffIter)->m_drawingStaffSize, false) / 2;
            double trillHeight = m_doc->GetGlyphHeight(code, (*staffIter)->m_drawingStaffSize, false);
            char32_t accid = Accid::GetAccidGlyph(trill->GetAccidupper());
            std::u32string accidStr;
            accidStr.push_back(accid);
            dc->SetFont(m_doc->GetDrawingSmuflFont((*staffIter)->m_drawingStaffSize, false));
            int accidY = y + trillHeight * 1.5;
            this->DrawSmuflString(dc, x + accidXShift, accidY, accidStr, HORIZONTALALIGNMENT_center,
                (*staffIter)->m_drawingStaffSize / 2, false);
        }

        dc->SetFont(m_doc->GetDrawingSmuflFont((*staffIter)->m_drawingStaffSize, false));
        this->DrawSmuflString(dc, x, y, str, alignment, (*staffIter)->m_drawingStaffSize);
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

    dc->StartGraphic(turn, "", turn->GetID());

    int x = turn->GetStart()->GetDrawingX() + turn->GetStart()->GetDrawingRadius(m_doc);

    if (turn->m_drawingEndElement) {
        // Get the parent system of the start and end element
        LayerElement *end = turn->m_drawingEndElement;
        Object *parentSystem1 = turn->GetStart()->GetFirstAncestor(SYSTEM);
        Object *parentSystem2 = end->GetFirstAncestor(SYSTEM);
        // We have a system break, use the measure right bar line instead
        if (parentSystem1 != parentSystem2) end = measure->GetRightBarLine();
        x += ((end->GetDrawingX() - x) / 2);
    }

    // set norm as default
    int code = turn->GetTurnGlyph();

    data_HORIZONTALALIGNMENT alignment = HORIZONTALALIGNMENT_center;
    // center the turn only with @startid
    if (turn->GetStart()->Is(TIMESTAMP_ATTR)) {
        alignment = HORIZONTALALIGNMENT_left;
    }

    std::u32string str;
    str.push_back(code);

    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList = turn->GetTstampStaves(measure, turn);
    for (staffIter = staffList.begin(); staffIter != staffList.end(); ++staffIter) {
        if (!system->SetCurrentFloatingPositioner((*staffIter)->GetN(), turn, turn->GetStart(), *staffIter)) {
            continue;
        }
        int y = turn->GetDrawingY();

        const int shift = m_doc->GetGlyphHeight(code, (*staffIter)->m_drawingStaffSize, false);
        dc->SetFont(m_doc->GetDrawingSmuflFont((*staffIter)->m_drawingStaffSize, false));
        if (turn->HasAccidupper()) {
            int accidXShift = (alignment == HORIZONTALALIGNMENT_center)
                ? 0
                : m_doc->GetGlyphWidth(code, (*staffIter)->m_drawingStaffSize, false) / 2;
            data_ACCIDENTAL_WRITTEN glyph = turn->GetAccidupper();
            char32_t accid = Accid::GetAccidGlyph(glyph);
            std::u32string accidStr;
            accidStr.push_back(accid);
            int accidYShift = shift - m_doc->GetGlyphBottom(accid, (*staffIter)->m_drawingStaffSize, true);
            this->DrawSmuflString(dc, x + accidXShift, y + accidYShift, accidStr, HORIZONTALALIGNMENT_center,
                (*staffIter)->m_drawingStaffSize / 2, false);
        }
        if (turn->HasAccidlower()) {
            int accidXShift = (alignment == HORIZONTALALIGNMENT_center)
                ? 0
                : m_doc->GetGlyphWidth(code, (*staffIter)->m_drawingStaffSize, false) / 2;
            char32_t accid = Accid::GetAccidGlyph(turn->GetAccidlower());
            std::u32string accidStr;
            accidStr.push_back(accid);
            int accidYShift = -m_doc->GetGlyphHeight(accid, (*staffIter)->m_drawingStaffSize, true) / 2;
            this->DrawSmuflString(dc, x + accidXShift, y + accidYShift, accidStr, HORIZONTALALIGNMENT_center,
                (*staffIter)->m_drawingStaffSize / 2, false);
        }

        this->DrawSmuflString(dc, x, y, str, alignment, (*staffIter)->m_drawingStaffSize);
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

    if (element->Is(SYSTEM_MILESTONE_END)) {
        SystemMilestoneEnd *elementEnd = vrv_cast<SystemMilestoneEnd *>(element);
        assert(elementEnd);
        assert(elementEnd->GetStart());
        dc->StartGraphic(element, elementEnd->GetStart()->GetID(), element->GetID());
        dc->EndGraphic(element, this);
    }
    else if (element->Is(ENDING)) {
        // Create placeholder - A graphic for the end milestone will be created
        // but only if it is on a different system - See View::DrawEnding
        // The Ending is added to the System drawing list by View::DrawMeasure
        dc->StartGraphic(element, "systemMilestone", element->GetID());
        dc->EndGraphic(element, this);
    }
    else if (element->Is(PB)) {
        dc->StartGraphic(element, "", element->GetID());
        dc->EndGraphic(element, this);
    }
    else if (element->Is(SB)) {
        dc->StartGraphic(element, "", element->GetID());
        dc->EndGraphic(element, this);
    }
    else if (element->Is(SECTION)) {
        dc->StartGraphic(element, "systemMilestone", element->GetID());
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

    SystemMilestoneEnd *endingEndMilestone = ending->GetEnd();

    // We need to make sure we have the end boudary and a measure (first and last) in each of them
    assert(endingEndMilestone);
    assert(ending->GetMeasure() && endingEndMilestone->GetMeasure());
    // in non debug mode
    if (!endingEndMilestone) return;
    if (!ending->GetMeasure() || !endingEndMilestone->GetMeasure()) return;

    // Get the parent system of the first and last note
    System *parentSystem1 = dynamic_cast<System *>(ending->GetFirstAncestor(SYSTEM));
    System *parentSystem2 = dynamic_cast<System *>(endingEndMilestone->GetFirstAncestor(SYSTEM));

    assert(parentSystem1 && parentSystem2);
    // in non debug mode
    if (!parentSystem1 || !parentSystem2) return;

    int x1, x2;
    Object *objectX;
    Measure *measure = NULL;
    char spanningType = SPANNING_START_END;
    Measure *endingMeasure = NULL;

    // They both correspond to the current system, which means no system break in-between (simple case)
    if ((system == parentSystem1) && (system == parentSystem2)) {
        measure = ending->GetMeasure();
        x1 = measure->GetDrawingX();
        endingMeasure = endingEndMilestone->GetMeasure();
        objectX = measure;
        // if it is the first measure of the system use the left barline position
        if (system->GetFirst(MEASURE) == measure) x1 += measure->GetLeftBarLineXRel();
        x2 = endingEndMilestone->GetMeasure()->GetDrawingX() + endingEndMilestone->GetMeasure()->GetRightBarLineXRel();
    }
    // Only the first parent is the same, this means that the ending is "open" at the end of the system
    else if (system == parentSystem1) {
        // We need the last measure of the system for x2 - we also use it for getting the staves later
        measure = dynamic_cast<Measure *>(system->FindDescendantByType(MEASURE, 1, BACKWARD));
        if (!Check(measure)) return;
        x1 = ending->GetMeasure()->GetDrawingX();
        objectX = measure;
        endingMeasure = measure;
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
        endingMeasure = endingEndMilestone->GetMeasure();
        x2 = endingEndMilestone->GetMeasure()->GetDrawingX() + endingEndMilestone->GetMeasure()->GetRightBarLineXRel();
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
        endingMeasure = measure;
        // We need the last measure of the system for x2
        measure = dynamic_cast<Measure *>(system->FindDescendantByType(MEASURE, 1, BACKWARD));
        if (!Check(measure)) return;
        x2 = measure->GetDrawingX() + measure->GetRightBarLineXRel();
        spanningType = SPANNING_MIDDLE;
    }

    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START))
        dc->ResumeGraphic(ending, ending->GetID());
    else
        dc->StartGraphic(ending, "", ending->GetID(), false);

    std::vector<Staff *>::iterator staffIter;
    std::vector<Staff *> staffList;
    assert(system->GetDrawingScoreDef());
    assert(measure);
    // By default, endings are drawn on top of each group (@ending.rend="gouped") unless "top" is specified)
    if (system->GetDrawingScoreDef()->GetEndingRend() == endings_ENDINGREND_top) {
        Staff *staff = NULL;
        ListOfObjects systemStaves = system->FindAllDescendantsByType(STAFF, false);
        for (const auto staffObject : systemStaves) {
            staff = vrv_cast<Staff *>(staffObject);
            StaffDef *staffDef = system->GetDrawingScoreDef()->GetStaffDef(staff->GetN());
            if (staffDef && (staffDef->GetDrawingVisibility() != OPTIMIZATION_HIDDEN)) break;
        }
        if (!staff) return;
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

        dc->StartCustomGraphic("voltaBracket");

        FontInfo currentFont = *m_doc->GetDrawingLyricFont((*staffIter)->m_drawingStaffSize);
        // currentFont.SetWeight(FONTWEIGHT_bold);
        // currentFont.SetPointSize(currentFont.GetPointSize() * 2 / 3);
        dc->SetFont(&currentFont);

        TextExtend extend;
        dc->GetTextExtent("M", &extend, false);

        const int unit = m_doc->GetDrawingUnit((*staffIter)->m_drawingStaffSize);
        if (ending->HasN()) {
            std::stringstream strStream;
            // Maybe we want to add ( ) after system breaks? Or . as a styling options?
            if ((spanningType == SPANNING_END) || (spanningType == SPANNING_MIDDLE)) strStream << "(";
            strStream << ending->GetN(); // << ".";
            if ((spanningType == SPANNING_END) || (spanningType == SPANNING_MIDDLE)) strStream << ")";

            Text text;
            text.SetParent(ending);
            text.SetText(UTF8to32(strStream.str()));

            int textX = x1;
            if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START)) {
                textX += unit * 2 / 3;
            }

            TextDrawingParams params;
            params.m_x = textX;
            params.m_y = y1;
            params.m_pointSize = currentFont.GetPointSize();

            dc->StartText(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), HORIZONTALALIGNMENT_left);
            this->DrawTextElement(dc, &text, params);
            dc->EndText();
        }

        dc->ResetFont();

        const int y2 = y1 + extend.m_height + unit * 2 / 3;
        const int lineWidth = m_options->m_repeatEndingLineThickness.GetValue() * unit;

        const int staffLineWidth = m_options->m_staffLineWidth.GetValue() * unit;
        const int startX = x1 - staffLineWidth;
        const int endX = (measure == system->FindDescendantByType(MEASURE, 1, BACKWARD))
            ? x2 + endingMeasure->CalculateRightBarLineWidth(m_doc, (*staffIter)->m_drawingStaffSize) - lineWidth / 2
                - staffLineWidth
            : x2;
        dc->SetPen(m_currentColour, lineWidth, AxSOLID, 0, 0, AxCAP_SQUARE, AxJOIN_ARCS);
        Point p[4];
        p[0] = { ToDeviceContextX(startX), ToDeviceContextY(y1) };
        p[1] = { ToDeviceContextX(startX), ToDeviceContextY(y2) };
        p[2] = { ToDeviceContextX(endX), ToDeviceContextY(y2) };
        p[3] = { ToDeviceContextX(endX), ToDeviceContextY(y1) };
        if ((spanningType == SPANNING_END) || (ending->GetLstartsym() == LINESTARTENDSYMBOL_none)) {
            p[0] = p[1];
        }
        if ((spanningType == SPANNING_START) || (ending->GetLendsym() == LINESTARTENDSYMBOL_none)) {
            p[3] = p[2];
        }
        dc->DrawPolyline(4, p);
        dc->ResetPen();

        dc->EndCustomGraphic();
    }

    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START))
        dc->EndResumedGraphic(ending, this);
    else
        dc->EndGraphic(ending, this);
}

void View::DrawTextEnclosure(DeviceContext *dc, const TextDrawingParams &params, int staffSize)
{
    assert(dc);
    const int lineThickness = m_options->m_textEnclosureThickness.GetValue() * staffSize;

    for (const auto rend : params.m_enclosedRend) {
        int x1 = rend->GetContentLeft() - staffSize;
        int x2 = rend->GetContentRight() + staffSize;
        int y1 = rend->GetContentBottom() - staffSize / 2;
        int y2 = rend->GetContentTop() + staffSize;

        if (params.m_enclose == TEXTRENDITION_box) {
            this->DrawNotFilledRectangle(dc, x1, y1, x2, y2, lineThickness, 0);
        }
        else if (params.m_enclose == TEXTRENDITION_circle) {
            const int width = std::abs(x2 - x1);
            const int height = std::abs(y2 - y1);
            if (width > height) {
                y1 -= (width - height) / 2;
                y2 += (width - height) / 2;
            }
            else if (height > width) {
                x1 -= (height - width) / 2;
                x2 += (height - width) / 2;
            }
            this->DrawNotFilledEllipse(dc, x1, y1, x2, y2, lineThickness);
        }
    }
}

} // namespace vrv
