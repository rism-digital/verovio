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

#include "annotscore.h"
#include "arpeg.h"
#include "bboxdevicecontext.h"
#include "beamspan.h"
#include "bracketspan.h"
#include "breath.h"
#include "caesura.h"
#include "clef.h"
#include "comparison.h"
#include "cpmark.h"
#include "devicecontext.h"
#include "dir.h"
#include "doc.h"
#include "dynam.h"
#include "ending.h"
#include "f.h"
#include "fb.h"
#include "fermata.h"
#include "fing.h"
#include "gliss.h"
#include "hairpin.h"
#include "harm.h"
#include "layer.h"
#include "measure.h"
#include "mordent.h"
#include "note.h"
#include "octave.h"
#include "options.h"
#include "ornam.h"
#include "page.h"
#include "pedal.h"
#include "pitchinflection.h"
#include "reh.h"
#include "rend.h"
#include "repeatmark.h"
#include "score.h"
#include "slur.h"
#include "smufl.h"
#include "staff.h"
#include "syl.h"
#include "symboldef.h"
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
    if (element->Is({ ANNOTSCORE, BEAMSPAN, BRACKETSPAN, FIGURE, GLISS, HAIRPIN, LV, OCTAVE, PHRASE, PITCHINFLECTION,
            SLUR, TIE })) {
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
    else if (element->Is(CPMARK)) {
        CpMark *cpMark = vrv_cast<CpMark *>(element);
        assert(cpMark);
        this->DrawControlElementText(dc, cpMark, measure, system);
    }
    else if (element->Is(DIR)) {
        Dir *dir = vrv_cast<Dir *>(element);
        assert(dir);
        this->DrawControlElementText(dc, dir, measure, system);
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
    else if (element->Is(ORNAM)) {
        Ornam *ornam = vrv_cast<Ornam *>(element);
        assert(ornam);
        this->DrawControlElementText(dc, ornam, measure, system);
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
    else if (element->Is(REPEATMARK)) {
        RepeatMark *repeatMark = vrv_cast<RepeatMark *>(element);
        assert(repeatMark);
        this->DrawRepeatMark(dc, repeatMark, measure, system);
    }
    else if (element->Is(TEMPO)) {
        Tempo *tempo = vrv_cast<Tempo *>(element);
        assert(tempo);
        this->DrawTempo(dc, tempo, measure, system);
        system->AddToDrawingListIfNecessary(tempo);
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
            if (element->Is({ ANNOTSCORE, BRACKETSPAN, HAIRPIN, OCTAVE, PITCHINFLECTION })) return;
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
    System *parentSystem1 = vrv_cast<System *>(start->GetFirstAncestor(SYSTEM));
    System *parentSystem2 = vrv_cast<System *>(end->GetFirstAncestor(SYSTEM));

    int x1, x2;
    Object *objectX = NULL;
    Measure *measure = NULL;
    Object *graphic = NULL;
    char spanningType = SPANNING_START_END;

    // They both correspond to the current system, which means no system break in-between (simple case)
    if ((system == parentSystem1) && (system == parentSystem2)) {
        // we use the start measure
        measure = interface->GetStartMeasure();
        if (!measure) return;
        x1 = start->GetDrawingX();
        objectX = start;
        x2 = end->GetDrawingX();
        graphic = element;
    }
    // Only the first parent is the same, this means that the element is "open" at the end of the system
    else if (system == parentSystem1) {
        // We need the last measure of the system for x2 - we also use it for getting the staves later
        measure = vrv_cast<Measure *>(system->FindDescendantByType(MEASURE, 1, BACKWARD));
        if (!measure) return;
        x1 = start->GetDrawingX();
        objectX = start;
        x2 = measure->GetDrawingX() + measure->GetRightBarLineXRel();
        graphic = element;
        spanningType = SPANNING_START;
    }
    // We are in the system of the last note - draw the element from the beginning of the system
    else if (system == parentSystem2) {
        // We need the first measure of the system for x1 - we also use it for getting the staves later
        measure = vrv_cast<Measure *>(system->FindDescendantByType(MEASURE, 1, FORWARD));
        if (!measure) return;
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
        measure = vrv_cast<Measure *>(system->FindDescendantByType(MEASURE, 1, FORWARD));
        if (!measure) return;
        // We need the position of the first default in the first measure for x1
        x1 = measure->GetDrawingX() + measure->GetLeftBarLineXRel();
        objectX = measure->GetLeftBarLine();
        // We need the last measure of the system for x2
        Measure *last = vrv_cast<Measure *>(system->FindDescendantByType(MEASURE, 1, BACKWARD));
        if (!last) return;
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

    std::vector<Staff *> staffList = interface->GetTstampStaves(measure, element);
    bool isFirst = true;
    for (Staff *staff : staffList) {

        // TimeSpanning elements are not necessary floating elements (e.g., syl) - we have a bounding box only for them
        if (element->IsControlElement()) {
            if (element->Is({ PHRASE, SLUR })) {
                if (this->GetSlurHandling() == SlurHandling::Ignore) break;
                Slur *slur = vrv_cast<Slur *>(element);
                assert(slur);
                staff = slur->CalculatePrincipalStaff(staff, x1, x2);
            }

            // Create the floating positioner
            if (!system->SetCurrentFloatingPositioner(
                    staff->GetN(), dynamic_cast<ControlElement *>(element), objectX, staff, spanningType)) {
                continue;
            }
        }

        if (element->Is(ANNOTSCORE)) {
            // cast to AnnotScore check in DrawAnnotScore
            this->DrawAnnotScore(dc, dynamic_cast<AnnotScore *>(element), x1, x2, staff, spanningType, graphic);
        }
        else if (element->Is(DIR)) {
            // cast to Dir check in DrawControlElementConnector
            this->DrawControlElementConnector(dc, dynamic_cast<Dir *>(element), x1, x2, staff, spanningType, graphic);
        }
        else if (element->Is(DYNAM)) {
            // cast to Dynam check in DrawControlElementConnector
            this->DrawControlElementConnector(dc, dynamic_cast<Dynam *>(element), x1, x2, staff, spanningType, graphic);
        }
        else if (element->Is(FIGURE)) {
            // cast to F check in DrawFConnector
            this->DrawFConnector(dc, dynamic_cast<F *>(element), x1, x2, staff, spanningType, graphic);
        }
        else if (element->Is(BEAMSPAN)) {
            // cast to BeamSpan check in DrawBeamSpan
            this->DrawBeamSpan(dc, vrv_cast<BeamSpan *>(element), system, graphic);
        }
        else if (element->Is(BRACKETSPAN)) {
            // cast to BracketSpan check in DrawBracketSpan
            this->DrawBracketSpan(dc, dynamic_cast<BracketSpan *>(element), x1, x2, staff, spanningType, graphic);
        }
        else if (element->Is(GLISS)) {
            // For gliss we limit support to one value in @staff
            if (!isFirst) continue;
            // cast to Gliss check in DrawGliss
            this->DrawGliss(dc, dynamic_cast<Gliss *>(element), x1, x2, staff, spanningType, graphic);
        }
        else if (element->Is(HAIRPIN)) {
            // cast to Hairpin check in DrawHairpin
            this->DrawHairpin(dc, dynamic_cast<Hairpin *>(element), x1, x2, staff, spanningType, graphic);
        }
        else if (element->Is(LV)) {
            // For ties we limit support to one value in @staff
            if (!isFirst) continue;
            // cast to Tie check in DrawTie
            this->DrawTie(dc, dynamic_cast<Tie *>(element), x1, x2, staff, spanningType, graphic);
        }
        else if (element->Is(PHRASE)) {
            // Check if slurs should be ignored
            if (this->GetSlurHandling() == SlurHandling::Ignore) continue;
            // For phrases (slurs) we limit support to one value in @staff
            if (!isFirst) continue;
            // cast to Slur check in DrawSlur
            this->DrawSlur(dc, dynamic_cast<Slur *>(element), x1, x2, staff, spanningType, graphic);
        }
        else if (element->Is(OCTAVE)) {
            // cast to Slur check in DrawOctave
            this->DrawOctave(dc, dynamic_cast<Octave *>(element), x1, x2, staff, spanningType, graphic);
        }
        else if (element->Is(PEDAL)) {
            this->DrawPedalLine(dc, dynamic_cast<Pedal *>(element), x1, x2, staff, spanningType, graphic);
        }
        else if (element->Is(PITCHINFLECTION)) {
            // cast to PitchInflection check in DrawPitchInflection
            this->DrawPitchInflection(
                dc, dynamic_cast<PitchInflection *>(element), x1, x2, staff, spanningType, graphic);
        }
        else if (element->Is(SLUR)) {
            // Check if slurs should be ignored
            if (this->GetSlurHandling() == SlurHandling::Ignore) continue;
            // For slurs we limit support to one value in @staff
            if (!isFirst) continue;
            // cast to Slur check in DrawSlur
            this->DrawSlur(dc, dynamic_cast<Slur *>(element), x1, x2, staff, spanningType, graphic);
        }
        else if (element->Is(SYL)) {
            // prolong to the end of the notehead
            x2 += endRadius;
            // cast to Syl check in DrawSylConnector
            this->DrawSylConnector(dc, dynamic_cast<Syl *>(element), x1, x2, staff, spanningType, graphic);
        }
        else if (element->Is(TEMPO)) {
            // cast to Tempo check in DrawControlElementConnector
            this->DrawControlElementConnector(dc, dynamic_cast<Tempo *>(element), x1, x2, staff, spanningType, graphic);
        }
        else if (element->Is(TIE)) {
            // For ties we limit support to one value in @staff
            if (!isFirst) continue;
            // cast to Slur check in DrawTie
            this->DrawTie(dc, dynamic_cast<Tie *>(element), x1, x2, staff, spanningType, graphic);
        }
        else if (element->Is(TRILL)) {
            // cast to Trill check in DrawTrill
            this->DrawTrillExtension(dc, dynamic_cast<Trill *>(element), x1, x2, staff, spanningType, graphic);
        }
        isFirst = false;
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

void View::DrawAnnotScore(
    DeviceContext *dc, AnnotScore *annotScore, int x1, int x2, Staff *staff, char spanningType, Object *graphic)
{
    assert(dc);
    assert(annotScore);
    assert(staff);

    assert(annotScore->GetStart());
    assert(annotScore->GetEnd());

    // May need to set/tweak y pos
    const int y = annotScore->GetDrawingY();

    // This has been copied from bracketSpan and is likely to be wrong
    if (graphic) {
        dc->ResumeGraphic(graphic, graphic->GetID());
    }
    else {
        dc->StartGraphic(annotScore, "", annotScore->GetID(), SPANNING);
    }

    const int unit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    const int boxHeight = annotScore->GetBoxHeight(m_doc, unit);
    const int lineWidth = annotScore->GetLineWidth(m_doc, unit);
    const int halfLineWidth = lineWidth / 2;

    dc->SetPen(lineWidth, PEN_SOLID, 0, 0, LINECAP_BUTT, LINEJOIN_MITER);
    Point boxOutline[4];
    switch (spanningType) {
        case SPANNING_START:
            // Draw a box with an open right-hand side (to show it continues)
            if (!annotScore->GetStart()->Is(TIMESTAMP_ATTR)) {
                x1 -= annotScore->GetStart()->GetDrawingRadius(m_doc);
            }
            boxOutline[0] = { this->ToDeviceContextX(x2), this->ToDeviceContextY(y) };
            boxOutline[1] = { this->ToDeviceContextX(x1), this->ToDeviceContextY(y) };
            boxOutline[2] = { this->ToDeviceContextX(x1), this->ToDeviceContextY(y + boxHeight) };
            boxOutline[3] = { this->ToDeviceContextX(x2), this->ToDeviceContextY(y + boxHeight) };
            dc->DrawPolyline(4, boxOutline);
            dc->SetBrush(0.5, COLOR_RED);
            this->DrawFilledRectangle(dc, x1 + halfLineWidth, y + halfLineWidth, x2, y + boxHeight - halfLineWidth);
            dc->ResetBrush();
            break;
        case SPANNING_MIDDLE:
            // Draw a box with  both sides open (to show it continues)
            if (!annotScore->GetStart()->Is(TIMESTAMP_ATTR)) {
                x1 -= annotScore->GetStart()->GetDrawingRadius(m_doc);
            }
            dc->DrawLine(this->ToDeviceContextX(x1), this->ToDeviceContextY(y), this->ToDeviceContextX(x2),
                this->ToDeviceContextY(y));
            dc->DrawLine(this->ToDeviceContextX(x1), this->ToDeviceContextY(y + boxHeight), this->ToDeviceContextX(x2),
                this->ToDeviceContextY(y + boxHeight));
            dc->SetBrush(0.5, COLOR_RED);
            this->DrawFilledRectangle(dc, x1, y + halfLineWidth, x2, y + boxHeight - halfLineWidth);
            dc->ResetBrush();
            break;
        case SPANNING_START_END:
            // Draw a closed box
            if (!annotScore->GetStart()->Is(TIMESTAMP_ATTR)) {
                x1 -= annotScore->GetStart()->GetDrawingRadius(m_doc);
            }
            if (!annotScore->GetEnd()->Is(TIMESTAMP_ATTR)) {
                x2 += annotScore->GetEnd()->GetDrawingRadius(m_doc);
            }
            boxOutline[0] = { this->ToDeviceContextX(x2), this->ToDeviceContextY(y) };
            boxOutline[1] = { this->ToDeviceContextX(x1), this->ToDeviceContextY(y) };
            boxOutline[2] = { this->ToDeviceContextX(x1), this->ToDeviceContextY(y + boxHeight) };
            boxOutline[3] = { this->ToDeviceContextX(x2), this->ToDeviceContextY(y + boxHeight) };
            dc->DrawPolyline(4, boxOutline, true);
            dc->SetBrush(0.5, COLOR_RED);
            this->DrawFilledRectangle(
                dc, x1 + halfLineWidth, y + halfLineWidth, x2 - halfLineWidth, y + boxHeight - halfLineWidth);
            dc->ResetBrush();
            break;
        case SPANNING_END:
            // Draw a box with the left side open to show it continues from previous system
            if (!annotScore->GetEnd()->Is(TIMESTAMP_ATTR)) {
                x2 += annotScore->GetEnd()->GetDrawingRadius(m_doc);
            }
            boxOutline[0] = { this->ToDeviceContextX(x1), this->ToDeviceContextY(y) };
            boxOutline[1] = { this->ToDeviceContextX(x2), this->ToDeviceContextY(y) };
            boxOutline[2] = { this->ToDeviceContextX(x2), this->ToDeviceContextY(y + boxHeight) };
            boxOutline[3] = { this->ToDeviceContextX(x1), this->ToDeviceContextY(y + boxHeight) };
            dc->DrawPolyline(4, boxOutline);
            dc->SetBrush(0.5, COLOR_RED);
            this->DrawFilledRectangle(dc, x1, y + halfLineWidth, x2 - halfLineWidth, y + boxHeight - halfLineWidth);
            dc->ResetBrush();
            break;
    }
    dc->ResetPen();

    if (graphic) {
        dc->EndResumedGraphic(graphic, this);
    }
    else {
        dc->EndGraphic(annotScore, this);
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

    const int y = bracketSpan->GetDrawingY();

    if (graphic) {
        dc->ResumeGraphic(graphic, graphic->GetID());
    }
    else {
        dc->StartGraphic(bracketSpan, "", bracketSpan->GetID(), SPANNING);
    }

    const int unit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    const int lineWidth = bracketSpan->GetLineWidth(m_doc, unit);

    x1 += lineWidth / 2;
    x2 -= lineWidth / 2;

    dc->SetPen(lineWidth, PEN_SOLID, 0, 0, LINECAP_BUTT, LINEJOIN_MITER);

    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START)) {
        if (!bracketSpan->GetStart()->Is(TIMESTAMP_ATTR)) {
            x1 -= bracketSpan->GetStart()->GetDrawingRadius(m_doc);
        }
        if (bracketSpan->GetLstartsym() != LINESTARTENDSYMBOL_none) {
            // Left hook
            Point hookLeft[3];
            hookLeft[0] = { this->ToDeviceContextX(x1), this->ToDeviceContextY(y - unit * 2) };
            hookLeft[1] = { this->ToDeviceContextX(x1), this->ToDeviceContextY(y) };
            hookLeft[2] = { this->ToDeviceContextX(x1 + unit), this->ToDeviceContextY(y) };
            dc->DrawPolyline(3, hookLeft);
        }
    }
    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_END)) {
        if (!bracketSpan->GetEnd()->Is(TIMESTAMP_ATTR)) {
            x2 += bracketSpan->GetEnd()->GetDrawingRadius(m_doc);
        }
        if (bracketSpan->GetLendsym() != LINESTARTENDSYMBOL_none) {
            // Right hook
            Point hookRight[3];
            hookRight[0] = { this->ToDeviceContextX(x2), this->ToDeviceContextY(y - unit * 2) };
            hookRight[1] = { this->ToDeviceContextX(x2), this->ToDeviceContextY(y) };
            hookRight[2] = { this->ToDeviceContextX(x2 - unit), this->ToDeviceContextY(y) };
            dc->DrawPolyline(3, hookRight);
        }
    }
    // We have a @lform - draw a full line
    if (bracketSpan->HasLform()) {
        PenStyle penStyle = PEN_SOLID;
        LineCapStyle lineCapStyle = LINECAP_BUTT;
        if (bracketSpan->GetLform() == LINEFORM_dashed) {
            penStyle = PEN_LONG_DASH;
            lineCapStyle = LINECAP_SQUARE;
        }
        else if (bracketSpan->GetLform() == LINEFORM_dotted) {
            penStyle = PEN_DOT;
            lineCapStyle = LINECAP_ROUND;
            // Adjust start and end
            x1 += unit + lineWidth * 2;
            x2 -= unit + lineWidth * 2;
            const int diff = (x2 - x1) % (lineWidth * 3 + 1);
            x1 += diff / 2;
        }
        dc->SetPen(lineWidth, penStyle, 0, 0, lineCapStyle);
        dc->DrawLine(this->ToDeviceContextX(x1), this->ToDeviceContextY(y), this->ToDeviceContextX(x2),
            this->ToDeviceContextY(y));
        dc->ResetPen();
    }

    dc->ResetPen();

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

    const int unit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    const hairpinLog_FORM form = hairpin->GetForm();
    const bool niente = (hairpin->HasNiente()) ? (hairpin->GetNiente() == BOOLEAN_true) : false;

    int adjustedX1 = x1;
    if (leftLink) {
        adjustedX1 = leftLink->GetContentRight() + unit / 2;
        adjustedX1 += (niente && (form == hairpinLog_FORM_cres)) ? unit / 3 : 0;
    }
    int adjustedX2 = x2;
    if (rightLink) {
        adjustedX2 = rightLink->GetContentLeft() - unit / 2;
        adjustedX2 -= (niente && (form == hairpinLog_FORM_dim)) ? unit / 3 : 0;
    }

    // Beginning of a system, very short hairpin needs to be push left
    if (spanningType == SPANNING_END) {
        if ((adjustedX2 - adjustedX1) < (unit * 2)) {
            adjustedX1 = adjustedX2 - 2 * unit;
        }
    }

    // In any case, a hairpin should not be shorter than 2 units.
    // If shorter, with groups, this will screw up vertical alignment and push everything down - to be improved by
    // deactivating grp?
    if ((adjustedX2 - adjustedX1) >= unit * 2) {
        x1 = adjustedX1;
        x2 = adjustedX2;
    }

    // Store the full drawing length
    const auto [leftOverlap, rightOverlap] = hairpin->GetBarlineOverlapAdjustment(unit * 2, x1, x2, spanningType);
    x1 += leftOverlap;
    x2 -= rightOverlap;

    hairpin->SetDrawingLength(x2 - x1);

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

    int y = hairpin->GetDrawingY();

    // Improve alignment with dynamics
    if (hairpin->GetPlace() != STAFFREL_within) {
        int shiftY = -(m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;
        if (hairpin->GetPlace() != STAFFREL_between) {
            shiftY += unit;
        }
        y += shiftY;
    }

    /************** draw it **************/

    if (graphic) {
        dc->ResumeGraphic(graphic, graphic->GetID());
    }
    else {
        dc->StartGraphic(hairpin, "", hairpin->GetID(), SPANNING);
    }

    const double hairpinThickness = m_options->m_hairpinThickness.GetValue() * unit;

    PenStyle style;
    switch (hairpin->GetLform()) {
        case LINEFORM_dashed: style = PEN_LONG_DASH; break;
        case LINEFORM_dotted: style = PEN_DOT; break;
        default: style = PEN_SOLID; break;
    }

    const LineCapStyle cap = (style == PEN_DOT) ? LINECAP_ROUND : LINECAP_SQUARE;

    dc->SetPen(hairpinThickness, style, 0, 0, cap, LINEJOIN_MITER);

    if ((startY == 0) && !niente) {
        Point p[3];
        p[0] = { this->ToDeviceContextX(x2), this->ToDeviceContextY(y - endY / 2) };
        p[1] = { this->ToDeviceContextX(x1), this->ToDeviceContextY(y) };
        p[2] = { p[0].x, this->ToDeviceContextY(y + endY / 2) };
        dc->DrawPolyline(3, p);
    }
    else if ((endY == 0) && !niente) {
        Point p[3];
        p[0] = { this->ToDeviceContextX(x1), this->ToDeviceContextY(y - startY / 2) };
        p[1] = { this->ToDeviceContextX(x2), this->ToDeviceContextY(y) };
        p[2] = { p[0].x, this->ToDeviceContextY(y + startY / 2) };
        dc->DrawPolyline(3, p);
    }
    else {
        if (niente) {
            dc->SetBrush(0.0);
            if (startY == 0) {
                dc->DrawCircle(this->ToDeviceContextX(x1), this->ToDeviceContextY(y), unit / 2);
                startY = unit * endY / (x2 - x1) / 2;
                x1 += unit / 2;
            }
            else if (endY == 0) {
                dc->DrawCircle(this->ToDeviceContextX(x2), this->ToDeviceContextY(y), unit / 2);
                endY = unit * startY / (x2 - x1) / 2;
                x2 -= unit / 2;
            }
            dc->ResetBrush();
        }

        Point p[2];
        p[0] = { this->ToDeviceContextX(x1), this->ToDeviceContextY(y - startY / 2) };
        p[1] = { this->ToDeviceContextX(x2), this->ToDeviceContextY(y - endY / 2) };
        dc->DrawPolyline(2, p);
        p[0].y = this->ToDeviceContextY(y + startY / 2);
        p[1].y = this->ToDeviceContextY(y + endY / 2);
        dc->DrawPolyline(2, p);
    }
    dc->ResetPen();

    if (graphic) {
        dc->EndResumedGraphic(graphic, this);
    }
    else {
        dc->EndGraphic(hairpin, this);
    }
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

    const data_STAFFREL_basic disPlace = octave->GetDisPlace();

    int y1 = octave->GetDrawingY();
    int y2 = y1;
    const int unit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);

    /********** adjust the start / end positions ***********/

    if ((spanningType == SPANNING_END) || (spanningType == SPANNING_MIDDLE)) {
        x1 += (m_doc->GetGlyphWidth(SMUFL_E0A2_noteheadWhole, staff->m_drawingStaffSize, false) / 2);
        if (!m_doc->GetOptions()->m_octaveNoSpanningParentheses.GetValue()) {
            x1 += m_doc->GetGlyphWidth(SMUFL_E51A_octaveParensLeft, staff->m_drawingStaffSize, false);
        }
    }

    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_END)) {
        if (octave->HasEndid()) {
            if (octave->GetEnd()->HasContentBB()) {
                x2 += octave->GetEnd()->GetContentX2();
            }
        }
    }

    /************** draw it **************/

    if (graphic) {
        dc->ResumeGraphic(graphic, graphic->GetID());
    }
    else {
        dc->StartGraphic(octave, "", octave->GetID(), SPANNING);
    }

    const bool altSymbols = m_doc->GetOptions()->m_octaveAlternativeSymbols.GetValue();
    const char32_t code = octave->GetOctaveGlyph(altSymbols);
    std::u32string str;
    str.push_back(code);

    dc->SetFont(m_doc->GetDrawingSmuflFont(staff->m_drawingStaffSize, false));
    TextExtend extend;
    dc->GetSmuflTextExtent(str, &extend);
    const int yCode = (disPlace == STAFFREL_basic_above) ? y1 - extend.m_height : y1;
    const int octaveX = altSymbols ? x1 - extend.m_width / 2 : x1 - extend.m_width;
    this->DrawSmuflCode(dc, octaveX, yCode, code, staff->m_drawingStaffSize, false);
    if (((spanningType == SPANNING_END) || (spanningType == SPANNING_MIDDLE))
        && !m_doc->GetOptions()->m_octaveNoSpanningParentheses.GetValue()) {
        const int leftWidth = m_doc->GetGlyphWidth(SMUFL_E51A_octaveParensLeft, staff->m_drawingStaffSize, false);
        const int rightWidth = m_doc->GetGlyphWidth(SMUFL_E51B_octaveParensRight, staff->m_drawingStaffSize, false);
        const int octaveWidth = m_doc->GetGlyphWidth(code, staff->m_drawingStaffSize, false);
        this->DrawSmuflCode(
            dc, octaveX - leftWidth, yCode, SMUFL_E51A_octaveParensLeft, staff->m_drawingStaffSize, false);
        this->DrawSmuflCode(
            dc, octaveX + octaveWidth, yCode, SMUFL_E51B_octaveParensRight, staff->m_drawingStaffSize, false);
        x1 += rightWidth;
    }
    dc->ResetFont();

    if (octave->GetExtender() != BOOLEAN_false) {
        const int lineWidth = octave->GetLineWidth(m_doc, unit);
        const int gap = lineWidth * 4;

        // adjust is to avoid the figure to touch the line
        x1 += lineWidth;
        if (altSymbols) x1 += extend.m_width / 2;

        PenStyle penStyle = PEN_SHORT_DASH;
        LineCapStyle lineCapStyle = LINECAP_SQUARE;
        int actualGap = gap;
        int actualLineWidth = lineWidth;

        if (octave->HasLform()) {
            if (octave->GetLform() == LINEFORM_solid) {
                penStyle = PEN_SOLID;
                actualGap = 0;
            }
            else if (octave->GetLform() == LINEFORM_dotted) {
                if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_END)) {
                    // round to nearest multiple
                    const int diff = (x2 - x1) % (gap + 1);
                    x2 += (gap - diff < diff) ? gap - diff : -diff;
                }
                penStyle = PEN_SOLID;
                lineCapStyle = LINECAP_ROUND;
                actualLineWidth = lineWidth * 3 / 2;
            }
        }
        dc->SetPen(actualLineWidth, penStyle, 0, actualGap, lineCapStyle);

        // adjust vertical ends
        y1 += (disPlace == STAFFREL_basic_above) ? -lineWidth / 2 : lineWidth / 2;
        y2 = (disPlace == STAFFREL_basic_above) ? y1 - unit * 2 : y1 + unit * 2;

        if (x1 + unit > x2) {
            // make sure we have a minimal extender line
            x2 = x1 + unit - lineWidth / 2;
        }
        else {
            dc->DrawLine(this->ToDeviceContextX(x1), this->ToDeviceContextY(y1), this->ToDeviceContextX(x2),
                this->ToDeviceContextY(y1));
        }

        octave->SetDrawingExtenderX(x1, x2);

        if (octave->GetLendsym() != LINESTARTENDSYMBOL_none) {
            if (spanningType == SPANNING_END || spanningType == SPANNING_START_END) {
                if (octave->GetLform() == LINEFORM_dotted) {
                    // make sure we have at least two dots for the dotted hook
                    dc->SetPen(lineWidth * 3 / 2, PEN_DOT, 0, std::min(gap, unit * 2 - lineWidth), LINECAP_ROUND);
                    dc->DrawLine(this->ToDeviceContextX(x2), this->ToDeviceContextY(y1), this->ToDeviceContextX(x2),
                        this->ToDeviceContextY(y2));
                    dc->ResetPen();
                }
                else {
                    dc->SetPen(lineWidth, PEN_SOLID);
                    // Right hook
                    Point hookRight[3];
                    hookRight[0] = { this->ToDeviceContextX(x2), this->ToDeviceContextY(y2) };
                    hookRight[1] = { this->ToDeviceContextX(x2), this->ToDeviceContextY(y1) };
                    hookRight[2] = { this->ToDeviceContextX(x2 - unit), this->ToDeviceContextY(y1) };
                    dc->DrawPolyline(3, hookRight);
                    dc->ResetPen();
                }
            }
        }

        dc->ResetPen();
    }

    if (graphic) {
        dc->EndResumedGraphic(graphic, this);
    }
    else {
        dc->EndGraphic(octave, this);
    }
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
    points[0].x = this->ToDeviceContextX(x1);
    points[0].y = this->ToDeviceContextY(y1);
    points[1].x = this->ToDeviceContextX(xControl);
    points[1].y = this->ToDeviceContextY(yControl);
    points[2].x = this->ToDeviceContextX(x2);
    points[2].y = this->ToDeviceContextY(y2);

    int arrowWidth = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2;
    int arrowHeight = arrowWidth * 3 / 2;
    arrowHeight = (up) ? arrowHeight : -arrowHeight;
    Point arrow[3];
    arrow[0].x = this->ToDeviceContextX(x2 - arrowWidth);
    arrow[0].y = this->ToDeviceContextY(y2);
    arrow[1].x = this->ToDeviceContextX(x2 + arrowWidth);
    arrow[1].y = this->ToDeviceContextY(y2);
    arrow[2].x = this->ToDeviceContextX(x2);
    arrow[2].y = this->ToDeviceContextY(y2 + arrowHeight);

    /************** draw it **************/

    if (graphic) {
        dc->ResumeGraphic(graphic, graphic->GetID());
    }
    else {
        dc->StartGraphic(pitchInflection, "spanning-pinflection", "");
    }

    dc->SetPen(m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize), PEN_SOLID);

    dc->DrawQuadBezierPath(points);
    if (drawArrow) {
        dc->DrawPolygon(3, arrow);
    }

    dc->ResetPen();

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

    PenStyle penStyle = PEN_SOLID;
    switch (tie->GetLform()) {
        case LINEFORM_dashed: penStyle = PEN_SHORT_DASH; break;
        case LINEFORM_dotted: penStyle = PEN_DOT; break;
        default: break;
    }

    if (graphic) {
        dc->ResumeGraphic(graphic, graphic->GetID());
    }
    else {
        dc->StartGraphic(tie, "", tie->GetID(), SPANNING);
    }

    // set pen width and calculate tie thickness coefficient to adjust tie width in according to it
    const int thickness
        = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * m_doc->GetOptions()->m_tieMidpointThickness.GetValue();
    const int penWidth
        = m_doc->GetOptions()->m_tieEndpointThickness.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    const double thicknessCoefficient = BoundingBox::GetBezierThicknessCoefficient(bezier, thickness, penWidth);
    this->DrawThickBezierCurve(
        dc, bezier, thicknessCoefficient * thickness, staff->m_drawingStaffSize, penWidth, penStyle);

    if (graphic) {
        dc->EndResumedGraphic(graphic, this);
    }
    else {
        dc->EndGraphic(tie, this);
    }
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

    // Adjust the start if necessary
    if (spanningType == SPANNING_START_END || spanningType == SPANNING_START) {
        x1 -= startRadius;
        // With pedline we need to take into account the initial symbol
        if ((pedal->GetForm() == PEDALSTYLE_pedline) || (pedal->GetForm() == PEDALSTYLE_pedstar)) {
            x1 += m_doc->GetGlyphWidth(SMUFL_E650_keyboardPedalPed, staff->m_drawingStaffSize, false);
        }
    }
    // Adjust the end if necessary
    if (spanningType == SPANNING_START_END || spanningType == SPANNING_END) {
        x2 -= endRadius - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
    }

    if (graphic) {
        dc->ResumeGraphic(graphic, graphic->GetID());
    }
    else {
        dc->StartGraphic(pedal, "", pedal->GetID(), SPANNING);
    }

    const int bracketSize = m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
    const int lineWidth = m_options->m_pedalLineThickness.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);

    // Opening bracket - but only if not a pedline value
    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START)) {
        if ((pedal->GetForm() != PEDALSTYLE_pedline && pedal->GetForm() != PEDALSTYLE_pedstar)) {
            this->DrawFilledRectangle(dc, x1, y, x1 + bracketSize / 2, y + lineWidth);
            this->DrawFilledRectangle(dc, x1, y, x1 + lineWidth, y + bracketSize);
        }
    }
    // Closing bracket
    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_END)) {
        if (pedal->GetForm() != PEDALSTYLE_pedstar) {
            this->DrawFilledRectangle(dc, x2 - bracketSize / 2, y, x2, y + lineWidth);
            this->DrawFilledRectangle(dc, x2 - lineWidth, y, x2, y + bracketSize);
        }
        else {
            std::u32string str;
            str.push_back(SMUFL_E655_keyboardPedalUp);
            const int staffSize = staff->m_drawingStaffSize;
            data_HORIZONTALALIGNMENT alignment = HORIZONTALALIGNMENT_left;
            dc->SetFont(m_doc->GetDrawingSmuflFont(staffSize, false));
            this->DrawSmuflString(dc, x2, y, str, alignment, staffSize);
            dc->ResetFont();
        }
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
    if (!trill->GetEnd()->Is(TIMESTAMP_ATTR)) {
        x2 -= trill->GetEnd()->GetDrawingRadius(m_doc);
    }
    x2 -= m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);

    int length = x2 - x1;
    Point orig(x1, y);

    /************** draw it **************/

    if (graphic) {
        dc->ResumeGraphic(graphic, graphic->GetID());
    }
    else {
        dc->StartGraphic(trill, "", trill->GetID(), SPANNING);
    }

    this->DrawSmuflLine(dc, orig, length, staff->m_drawingStaffSize, false, SMUFL_E59D_ornamentZigZagLineNoRightEnd, 0,
        SMUFL_E59E_ornamentZigZagLineWithRightEnd);

    if (graphic) {
        dc->EndResumedGraphic(graphic, this);
    }
    else {
        dc->EndGraphic(trill, this);
    }
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

    const int dist = x2 - x1;
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
        dc->StartGraphic(element, "", element->GetID(), SPANNING);
    }

    bool deactivate = true;
    // If there is no end link and we are not starting the control element, then do not deactivate the element.
    // The vertical spacing will not be consistent but at least is does not leave the connector un-laidout
    if (!element->GetNextLink() && (spanningType != SPANNING_START_END) && (spanningType != SPANNING_START)) {
        deactivate = false;
    }

    if (deactivate) {
        dc->DeactivateGraphic();
        // If the extender line is not included in the bounding box, then store its width
        element->GetCurrentFloatingPositioner()->SetDrawingExtenderWidth(dist);
    }

    for (int i = 0; i < nbDashes; ++i) {
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

    // The both correspond to the current system, which means no system break in-between (simple case)
    if (spanningType == SPANNING_START_END) {
        x1 = f->GetContentRight();
    }
    // Only the first parent is the same, this means that the syl is "open" at the end of the system
    else if (spanningType == SPANNING_START) {
        Text *text = vrv_cast<Text *>(f->GetFirst(TEXT));
        if (text) {
            x1 = text->GetContentRight();
        }
    }
    // We are in the system of the last note - draw the connector from the beginning of the system
    else if (spanningType == SPANNING_END) {
        // nothing to adjust
    }
    else {
        // nothing to adjust
    }

    // Because <f> is a TextElement the extendor is placed in the parent <fb>
    Fb *fb = (graphic) ? vrv_cast<Fb *>(graphic->GetFirstAncestor(FB)) : NULL;

    // temporary object in order not to reset the F bounding box.
    F fConnector;
    if (fb) {
        dc->ResumeGraphic(fb, fb->GetID());
    }
    else {
        dc->StartGraphic(&fConnector, "", f->GetID(), SPANNING);
    }

    dc->DeactivateGraphic();

    int width = m_options->m_lyricLineThickness.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    Syl::AdjustToLyricSize(m_doc, width);

    this->DrawFilledRectangle(dc, x1, y, x2, y + width);

    dc->ReactivateGraphic();

    if (fb) {
        dc->EndResumedGraphic(fb, this);
    }
    else {
        dc->EndGraphic(&fConnector, this);
    }
}

void View::DrawSylConnector(
    DeviceContext *dc, Syl *syl, int x1, int x2, Staff *staff, char spanningType, Object *graphic)
{
    assert(syl);
    assert(syl->GetStart() && syl->GetEnd());
    if (!syl->GetStart() || !syl->GetEnd()) return;

    const int y = staff->GetDrawingY() + this->GetSylYRel(syl->m_drawingVerseN, staff, syl->m_drawingVersePlace);

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
        // If we do not want to show hyphens at the start of a system and the end is at time 0
        if (m_options->m_lyricNoStartHyphen.GetValue() && (syl->GetEnd()->GetAlignment()->GetTime() == 0)) {
            // Return but only if the end is in the first measure of the system...
            Measure *measure = vrv_cast<Measure *>(syl->GetEnd()->GetFirstAncestor(MEASURE));
            assert(measure);
            System *system = vrv_cast<System *>(measure->GetFirstAncestor(SYSTEM));
            assert(system);
            if (measure == vrv_cast<Measure *>(system->FindDescendantByType(MEASURE))) {
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
    else {
        dc->StartGraphic(&sylConnector, "", syl->GetID(), SPANNING);
    }

    dc->DeactivateGraphic();

    this->DrawSylConnectorLines(dc, x1, x2, y, syl, staff);

    dc->ReactivateGraphic();

    if (graphic) {
        dc->EndResumedGraphic(graphic, this);
    }
    else {
        dc->EndGraphic(&sylConnector, this);
    }
}

void View::DrawSylConnectorLines(DeviceContext *dc, int x1, int x2, int y, Syl *syl, Staff *staff)
{
    if (dc->Is(BBOX_DEVICE_CONTEXT)) {
        return;
    }

    int thickness = m_options->m_lyricLineThickness.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    Syl::AdjustToLyricSize(m_doc, thickness);

    if (syl->GetCon() == sylLog_CON_d) {

        y += (m_options->m_lyricSize.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 5);

        // the length of the dash and the space between them
        const int dashLength = syl->CalcHyphenLength(m_doc, staff->m_drawingStaffSize);
        const int halfDashLength = dashLength / 2;

        const int dashSpace = m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize) * 5 / 3;
        const int dist = x2 - x1;
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

        for (int i = 0; i < nbDashes; ++i) {
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
        dc->RotateGraphic(Point(this->ToDeviceContextX(x), this->ToDeviceContextY(y)), angle);

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

    SymbolDef *symbolDef = NULL;
    if (breath->HasAltsym() && breath->HasAltSymbolDef()) {
        symbolDef = breath->GetAltSymbolDef();
    }

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

    std::vector<Staff *> staffList = breath->GetTstampStaves(measure, breath);
    for (Staff *staff : staffList) {
        if (!system->SetCurrentFloatingPositioner(staff->GetN(), breath, breath->GetStart(), staff)) {
            continue;
        }
        const int staffSize = staff->m_drawingStaffSize;
        const int y = breath->GetDrawingY();

        if (symbolDef) {
            this->DrawSymbolDef(dc, breath, symbolDef, x, y, staffSize, false, alignment);
        }
        else {
            dc->SetFont(m_doc->GetDrawingSmuflFont(staffSize, false));
            this->DrawSmuflString(dc, x, y, str, alignment, staffSize);
            dc->ResetFont();
        }
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

    SymbolDef *symbolDef = NULL;
    if (caesura->HasAltsym() && caesura->HasAltSymbolDef()) {
        symbolDef = caesura->GetAltSymbolDef();
    }

    const char32_t code = caesura->GetCaesuraGlyph();
    const int x = caesura->GetStart()->GetDrawingX() + caesura->GetStart()->GetDrawingRadius(m_doc) * 3;

    std::vector<Staff *> staffList = caesura->GetTstampStaves(measure, caesura);
    for (Staff *staff : staffList) {
        if (!system->SetCurrentFloatingPositioner(staff->GetN(), caesura, caesura->GetStart(), staff)) {
            continue;
        }

        const int staffSize = staff->m_drawingStaffSize;
        const int glyphHeight = (symbolDef) ? symbolDef->GetSymbolHeight(m_doc, staffSize, false)
                                            : m_doc->GetGlyphHeight(code, staffSize, false);
        const int y = (caesura->HasPlace() && (caesura->GetPlace() != STAFFREL_within))
            ? caesura->GetDrawingY()
            : staff->GetDrawingY() - glyphHeight / 2;

        if (symbolDef) {
            this->DrawSymbolDef(dc, caesura, symbolDef, x, y, staffSize, false);
        }
        else {
            this->DrawSmuflCode(dc, x, y, code, staffSize, false);
        }
    }

    dc->EndGraphic(caesura, this);
}

void View::DrawControlElementText(DeviceContext *dc, ControlElement *element, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(element);

    // Cannot draw a dir that has no start position
    TimePointInterface *interface = element->GetTimePointInterface();
    assert(interface);
    TextDirInterface *interfaceTextDir = element->GetTextDirInterface();
    assert(interfaceTextDir);

    LayerElement *start = interface->GetStart();
    if (!start) return;

    dc->StartGraphic(element, "", element->GetID());

    const data_STAFFREL place = interfaceTextDir->GetPlace();

    FontInfo dirTxt;
    if (!dc->UseGlobalStyling()) {
        dirTxt.SetFaceName(m_doc->GetResources().GetTextFont());
        dirTxt.SetStyle(FONTSTYLE_italic);
    }

    const int lineCount = interfaceTextDir->GetNumberOfLines(element);

    data_HORIZONTALALIGNMENT alignment = element->GetChildRendAlignment();
    // dir are left aligned by default (with both @tstamp and @startid)
    if (alignment == HORIZONTALALIGNMENT_NONE) alignment = HORIZONTALALIGNMENT_left;

    std::vector<Staff *> staffList = interface->GetTstampStaves(measure, element);
    for (Staff *staff : staffList) {
        if (!system->SetCurrentFloatingPositioner(staff->GetN(), element, start, staff)) {
            continue;
        }
        const int staffSize = staff->m_drawingStaffSize;

        TextDrawingParams params;
        // If we have not timestamp
        params.m_x = start->GetDrawingX() + start->GetDrawingRadius(m_doc);
        params.m_y = element->GetDrawingY();
        params.m_pointSize = m_doc->GetDrawingLyricFont(staffSize)->GetPointSize();

        int xAdjust = 0;
        const bool isBetweenStaves = (place == STAFFREL_between)
            || ((place == STAFFREL_below) && (staff != measure->GetLast(STAFF)))
            || ((place == STAFFREL_above) && (staff != measure->GetFirst(STAFF)));
        if (isBetweenStaves
            && (interface->GetStart()->GetAlignment()->GetTime()
                == measure->m_measureAligner.GetRightBarLineAlignment()->GetTime())
            && interfaceTextDir->AreChildrenAlignedTo(element, HORIZONTALALIGNMENT_right)) {
            xAdjust = m_doc->GetDrawingUnit(staffSize) / 2;
        }

        dirTxt.SetPointSize(params.m_pointSize);

        if ((place == STAFFREL_between) || (place == STAFFREL_within)) {
            if (lineCount > 1) {
                params.m_y += (m_doc->GetTextLineHeight(&dirTxt, false) * (lineCount - 1) / 2);
            }
            params.m_y -= m_doc->GetTextXHeight(&dirTxt, false) / 2;
        }

        dc->SetFont(&dirTxt);

        dc->StartText(this->ToDeviceContextX(params.m_x - xAdjust), this->ToDeviceContextY(params.m_y), alignment);
        DrawTextChildren(dc, element, params);
        dc->EndText();

        dc->ResetFont();

        this->DrawTextEnclosure(dc, params, staffSize);
    }

    dc->EndGraphic(element, this);
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
        dynamTxt.SetFaceName(m_doc->GetResources().GetTextFont());
        dynamTxt.SetStyle(FONTSTYLE_italic);
    }

    const int lineCount = dynam->GetNumberOfLines(dynam);

    data_HORIZONTALALIGNMENT alignment = dynam->GetChildRendAlignment();
    // dynam are left aligned by default;
    if (alignment == 0) {
        // centre the dynam only with @startid
        alignment = (dynam->GetStart()->Is(TIMESTAMP_ATTR)) ? HORIZONTALALIGNMENT_left : HORIZONTALALIGNMENT_center;
    }

    std::vector<Staff *> staffList = dynam->GetTstampStaves(measure, dynam);
    for (Staff *staff : staffList) {
        if (!system->SetCurrentFloatingPositioner(staff->GetN(), dynam, dynam->GetStart(), staff)) {
            continue;
        }
        const int staffSize = staff->m_drawingStaffSize;

        TextDrawingParams params;
        // If we have not timestamp
        params.m_x = dynam->GetStart()->GetDrawingX() + dynam->GetStart()->GetDrawingRadius(m_doc);
        params.m_y = dynam->GetDrawingY();
        params.m_pointSize = m_doc->GetDrawingLyricFont(staffSize)->GetPointSize();

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
            this->DrawDynamSymbolOnly(dc, staff, dynam, dynamSymbol, alignment, params);
        }
        else {
            dc->SetFont(&dynamTxt);

            dc->StartText(this->ToDeviceContextX(params.m_x), this->ToDeviceContextY(params.m_y), alignment);
            this->DrawTextChildren(dc, dynam, params);
            dc->EndText();

            dc->ResetFont();
        }
        this->DrawTextEnclosure(dc, params, staffSize);
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

    fontDim->SetPointSize(m_doc->GetDrawingLyricFont(staff->m_drawingStaffSize)->GetPointSize());

    dc->SetFont(fontDim);

    for (Object *current : fb->GetChildren()) {
        dc->StartText(this->ToDeviceContextX(params.m_x), this->ToDeviceContextY(params.m_y), HORIZONTALALIGNMENT_left);
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

    SymbolDef *symbolDef = NULL;
    if (fermata->HasAltsym() && fermata->HasAltSymbolDef()) {
        symbolDef = fermata->GetAltSymbolDef();
    }

    const char32_t code = fermata->GetFermataGlyph();
    char32_t enclosingFront, enclosingBack;
    std::tie(enclosingFront, enclosingBack) = fermata->GetEnclosingGlyphs();

    const int x = fermata->GetStart()->GetDrawingX() + fermata->GetStart()->GetDrawingRadius(m_doc);

    std::vector<Staff *> staffList = fermata->GetTstampStaves(measure, fermata);
    for (Staff *staff : staffList) {
        if (!system->SetCurrentFloatingPositioner(staff->GetN(), fermata, fermata->GetStart(), staff)) {
            continue;
        }

        const int staffSize = staff->GetDrawingStaffNotationSize();
        const int y = fermata->GetDrawingY();

        const int width = (symbolDef) ? symbolDef->GetSymbolWidth(m_doc, staffSize, drawingCueSize)
                                      : m_doc->GetGlyphWidth(code, staffSize, drawingCueSize);
        const int height = (symbolDef) ? symbolDef->GetSymbolHeight(m_doc, staffSize, drawingCueSize)
                                       : m_doc->GetGlyphHeight(code, staffSize, drawingCueSize);

        // The correction for centering the glyph
        const int xCorr = width / 2;
        int yCorr = 0;

        const data_VERTICALALIGNMENT yAlignment = Fermata::GetVerticalAlignment(code);
        int enclosureYCorr = 0;
        if (yAlignment == VERTICALALIGNMENT_top) {
            yCorr = height / 2;
        }
        else if (yAlignment == VERTICALALIGNMENT_bottom) {
            yCorr = -height / 2;
        }
        else {
            const int glyphBottomY = m_doc->GetGlyphBottom(code, staffSize, false);
            if (fermata->GetPlace() == STAFFREL_above) {
                yCorr = height / 2 + glyphBottomY;
            }
            else {
                enclosureYCorr = height / 2 + glyphBottomY;
            }
        }

        // Draw glyph including possible enclosing brackets
        dc->SetFont(m_doc->GetDrawingSmuflFont(staffSize, drawingCueSize));

        if (enclosingFront) {
            const int xCorrEncl = xCorr + m_doc->GetDrawingUnit(staffSize) / 3
                + m_doc->GetGlyphWidth(enclosingFront, staffSize, drawingCueSize);
            this->DrawSmuflCode(
                dc, x - xCorrEncl, y + enclosureYCorr + yCorr, enclosingFront, staffSize, drawingCueSize);
        }

        if (symbolDef) {
            this->DrawSymbolDef(dc, fermata, symbolDef, x - xCorr, y, staffSize, drawingCueSize);
        }
        else {
            this->DrawSmuflCode(dc, x - xCorr, y, code, staffSize, drawingCueSize);
        }

        if (enclosingBack) {
            const int xCorrEncl = xCorr + m_doc->GetDrawingUnit(staffSize) / 3;
            this->DrawSmuflCode(
                dc, x + xCorrEncl, y + enclosureYCorr + yCorr, enclosingBack, staffSize, drawingCueSize);
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
        fingTxt.SetFaceName(m_doc->GetResources().GetTextFont());
    }

    // center fingering
    data_HORIZONTALALIGNMENT alignment = HORIZONTALALIGNMENT_center;

    std::vector<Staff *> staffList = fing->GetTstampStaves(measure, fing);
    for (Staff *staff : staffList) {
        if (!system->SetCurrentFloatingPositioner(staff->GetN(), fing, fing->GetStart(), staff)) {
            continue;
        }
        const int staffSize = staff->m_drawingStaffSize;

        TextDrawingParams params;
        params.m_x = fing->GetStart()->GetDrawingX() + fing->GetStart()->GetDrawingRadius(m_doc);
        params.m_y = fing->GetDrawingY();
        params.m_pointSize = m_doc->GetFingeringFont(staffSize)->GetPointSize();

        fingTxt.SetPointSize(params.m_pointSize);

        dc->SetFont(&fingTxt);

        dc->StartText(this->ToDeviceContextX(params.m_x), this->ToDeviceContextY(params.m_y), alignment);
        this->DrawTextChildren(dc, fing, params);
        dc->EndText();

        dc->ResetFont();

        this->DrawTextEnclosure(dc, params, staffSize);
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
        else if (gliss->GetLwidth().GetType() == LINEWIDTHTYPE_measurementunsigned) {
            if (gliss->GetLwidth().GetMeasurementunsigned().GetType() == MEASUREMENTTYPE_px) {
                lineWidth = gliss->GetLwidth().GetMeasurementunsigned().GetPx();
            }
            else {
                lineWidth = gliss->GetLwidth().GetMeasurementunsigned().GetVu()
                    * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
            }
        }
    }

    if (graphic) {
        dc->ResumeGraphic(graphic, graphic->GetID());
    }
    else {
        dc->StartGraphic(gliss, "", gliss->GetID(), SPANNING);
    }

    switch (gliss->GetLform()) {
        case LINEFORM_wavy: {
            const int length = static_cast<int>(hypot(x2 - x1, y2 - y1));
            const double angle = RadToDeg(atan2(y1 - y2, x2 - x1));
            // Smufl glyphs are horizontal - Rotate them counter clockwise
            dc->RotateGraphic(Point(this->ToDeviceContextX(x1), this->ToDeviceContextY(y1)), angle);

            const char32_t glissGlyph = SMUFL_EAAF_wiggleGlissando;
            const int height = m_doc->GetGlyphHeight(glissGlyph, staff->m_drawingStaffSize, false);
            const Point orig(x1, y1 - height / 2);
            this->DrawSmuflLine(dc, orig, length, staff->m_drawingStaffSize, false, glissGlyph);
            break;
        }
        case LINEFORM_dashed:
            dc->SetPen(lineWidth, PEN_SHORT_DASH, 0, 0, LINECAP_ROUND);
            dc->DrawLine(this->ToDeviceContextX(x1), this->ToDeviceContextY(y1), this->ToDeviceContextX(x2),
                this->ToDeviceContextY(y2));
            dc->ResetPen();
            break;
        case LINEFORM_dotted:
            dc->SetPen(lineWidth * 3 / 2, PEN_DOT, 0, 0, LINECAP_ROUND);
            dc->DrawLine(this->ToDeviceContextX(x1), this->ToDeviceContextY(y1), this->ToDeviceContextX(x2),
                this->ToDeviceContextY(y2));
            dc->ResetPen();
            break;
        case LINEFORM_solid: [[fallthrough]];
        default: {
            dc->SetPen(lineWidth, PEN_SOLID, 0, 0, LINECAP_ROUND);
            dc->DrawLine(this->ToDeviceContextX(x1), this->ToDeviceContextY(y1), this->ToDeviceContextX(x2),
                this->ToDeviceContextY(y2));
            dc->ResetPen();
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
        harmTxt.SetFaceName(m_doc->GetResources().GetTextFont());
    }

    data_HORIZONTALALIGNMENT alignment = harm->GetChildRendAlignment();
    // harm are centered aligned by default;
    if (alignment == 0) {
        // centre the harm only with @startid
        alignment = (harm->GetStart()->Is(TIMESTAMP_ATTR)) ? HORIZONTALALIGNMENT_left : HORIZONTALALIGNMENT_center;
    }

    std::vector<Staff *> staffList = harm->GetTstampStaves(measure, harm);
    for (Staff *staff : staffList) {
        if (!system->SetCurrentFloatingPositioner(staff->GetN(), harm, harm->GetStart(), staff)) {
            continue;
        }
        const int staffSize = staff->m_drawingStaffSize;

        TextDrawingParams params;
        // If we have not timestamp
        params.m_x = harm->GetStart()->GetDrawingX() + harm->GetStart()->GetDrawingRadius(m_doc);
        params.m_y = harm->GetDrawingY();

        if (harm->GetFirst() && harm->GetFirst()->Is(FB)) {
            this->DrawFb(dc, staff, dynamic_cast<Fb *>(harm->GetFirst()), params);
        }
        else {
            params.m_pointSize = m_doc->GetDrawingLyricFont(staffSize)->GetPointSize();

            harmTxt.SetPointSize(params.m_pointSize);

            dc->SetFont(&harmTxt);

            dc->StartText(this->ToDeviceContextX(params.m_x), this->ToDeviceContextY(params.m_y), alignment);
            this->DrawTextChildren(dc, harm, params);
            dc->EndText();

            dc->ResetFont();

            this->DrawTextEnclosure(dc, params, staffSize);
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

    SymbolDef *symbolDef = NULL;
    if (mordent->HasAltsym() && mordent->HasAltSymbolDef()) {
        symbolDef = mordent->GetAltSymbolDef();
    }

    int x = mordent->GetStart()->GetDrawingX() + mordent->GetStart()->GetDrawingRadius(m_doc);

    // set mordent glyph
    const int code = mordent->GetMordentGlyph();

    std::u32string str;
    str.push_back(code);

    std::vector<Staff *> staffList = mordent->GetTstampStaves(measure, mordent);

    for (Staff *staff : staffList) {
        if (!system->SetCurrentFloatingPositioner(staff->GetN(), mordent, mordent->GetStart(), staff)) {
            continue;
        }
        const int staffSize = staff->m_drawingStaffSize;
        int y = mordent->GetDrawingY();

        const int mordentHeight = (symbolDef) ? symbolDef->GetSymbolHeight(m_doc, staffSize, false)
                                              : m_doc->GetGlyphHeight(code, staffSize, false);
        const int mordentWidth = (symbolDef) ? symbolDef->GetSymbolWidth(m_doc, staffSize, false)
                                             : m_doc->GetGlyphWidth(code, staffSize, false);
        x -= (mordentWidth / 2);

        dc->SetFont(m_doc->GetDrawingSmuflFont(staffSize, false));

        if (mordent->HasAccidlower()) {
            char32_t accid = Accid::GetAccidGlyph(mordent->GetAccidlower());
            std::u32string accidStr;
            accidStr.push_back(accid);
            int accidY = y;
            int accidX = x;
            if (!symbolDef) {
                // Adjust the y position
                double xShift = 0.0;
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
                accidX += (1 + xShift) * mordentWidth / 2;
                accidY -= factor * m_doc->GetGlyphHeight(accid, staffSize, true) / 2;
            }
            else {
                accidX += mordentWidth / 2;
                accidY -= (m_doc->GetGlyphTop(accid, staffSize / 2, true) + m_doc->GetDrawingUnit(staffSize * 2 / 3));
            }
            this->DrawSmuflString(dc, accidX, accidY, accidStr, HORIZONTALALIGNMENT_center, staffSize / 2, false);
        }
        else if (mordent->HasAccidupper()) {
            char32_t accid = Accid::GetAccidGlyph(mordent->GetAccidupper());
            std::u32string accidStr;
            accidStr.push_back(accid);
            int accidY = y;
            int accidX = x;
            // Adjust the y position
            if (!symbolDef) {
                double xShift = 0.0;
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
                accidX += (1 + xShift) * mordentWidth / 2;
                accidY += factor * mordentHeight;
            }
            else {
                accidX += mordentWidth / 2;
                accidY += (mordentHeight - m_doc->GetGlyphBottom(accid, staffSize / 2, true)
                    + m_doc->GetDrawingUnit(staffSize * 2 / 3));
            }
            this->DrawSmuflString(dc, accidX, accidY, accidStr, HORIZONTALALIGNMENT_center, staffSize / 2, false);
        }

        if (symbolDef) {
            this->DrawSymbolDef(dc, mordent, symbolDef, x, y, staffSize, false);
        }
        else {
            this->DrawSmuflString(dc, x, y, str, HORIZONTALALIGNMENT_left, staffSize);
        }

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

    data_PEDALSTYLE form = pedal->GetPedalForm(m_doc, system);

    bool drawSymbol = (form != PEDALSTYLE_line);
    if (pedal->GetDir() == pedalLog_DIR_up && form == PEDALSTYLE_pedline) drawSymbol = false;

    // Draw a symbol, if it's not a line
    if (drawSymbol) {

        bool bounceStar = true;
        if (form == PEDALSTYLE_altpedstar) bounceStar = false;

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

        for (Staff *staff : staffList) {
            if (!system->SetCurrentFloatingPositioner(staff->GetN(), pedal, pedal->GetStart(), staff)) {
                continue;
            }
            const int staffSize = staff->m_drawingStaffSize;
            // Basic method that use bounding box
            const int y = pedal->GetDrawingY();

            dc->SetFont(m_doc->GetDrawingSmuflFont(staffSize, false));
            this->DrawSmuflString(dc, x, y, str, alignment, staffSize);
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
        rehTxt.SetFaceName(m_doc->GetResources().GetTextFont());
        rehTxt.SetWeight(FONTWEIGHT_bold);
    }

    TextDrawingParams params;

    // Number of units above the staff - 3 by default, 5 when above a clef
    int yMargin = 3;

    params.m_x = reh->GetStart()->GetDrawingX();
    const bool adjustPosition = ((reh->HasTstamp() && (reh->GetTstamp() == 0.0))
        || (reh->GetStart()->Is(BARLINE)
            && vrv_cast<BarLine *>(reh->GetStart())->GetPosition() == BarLinePosition::Left));
    if ((system->GetFirst(MEASURE) == measure) && adjustPosition) {
        // StaffDef information is always in the first layer
        Layer *layer = vrv_cast<Layer *>(measure->FindDescendantByType(LAYER));
        assert(layer);
        if (!system->IsFirstOfMdiv()) {
            if (Clef *clef = layer->GetStaffDefClef(); clef) {
                params.m_x = clef->GetDrawingX() + (clef->GetContentRight() - clef->GetContentLeft()) / 2;
                // Increase the margin when above the clef
                yMargin = 5;
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

    std::vector<Staff *> staffList = reh->GetTstampStaves(measure, reh);
    if (staffList.empty()) {
        Staff *staff = system->GetTopVisibleStaff();
        if (staff) staffList.push_back(staff);
    }

    for (Staff *staff : staffList) {
        if (!system->SetCurrentFloatingPositioner(staff->GetN(), reh, reh->GetStart(), staff)) {
            continue;
        }
        const int staffSize = staff->m_drawingStaffSize;

        if ((system->GetFirst(MEASURE) != measure) && adjustPosition) {
            params.m_x = staff->GetDrawingX();
        }

        params.m_enclosedRend.clear();
        params.m_y = reh->GetDrawingY() + yMargin * m_doc->GetDrawingUnit(staffSize);
        params.m_pointSize = m_doc->GetDrawingLyricFont(staffSize)->GetPointSize();

        rehTxt.SetPointSize(params.m_pointSize);

        dc->SetFont(&rehTxt);

        dc->StartText(this->ToDeviceContextX(params.m_x), this->ToDeviceContextY(params.m_y), alignment);
        this->DrawTextChildren(dc, reh, params);
        dc->EndText();

        dc->ResetFont();

        this->DrawTextEnclosure(dc, params, staffSize);
    }

    dc->EndGraphic(reh, this);
}

void View::DrawRepeatMark(DeviceContext *dc, RepeatMark *repeatMark, Measure *measure, System *system)
{
    assert(dc);
    assert(system);
    assert(measure);
    assert(repeatMark);

    // Cannot draw a mark that has no start position
    if (!repeatMark->GetStart()) return;

    if (repeatMark->GetChildCount() > 0) {
        this->DrawControlElementText(dc, repeatMark, measure, system);
        return;
    }

    dc->StartGraphic(repeatMark, "", repeatMark->GetID());

    SymbolDef *symbolDef = NULL;
    if (repeatMark->HasAltsym() && repeatMark->HasAltSymbolDef()) {
        symbolDef = repeatMark->GetAltSymbolDef();
    }

    const int x = repeatMark->GetStart()->GetDrawingX() + repeatMark->GetStart()->GetDrawingRadius(m_doc);

    // set norm as default
    const int code = repeatMark->GetMarkGlyph();

    data_HORIZONTALALIGNMENT alignment = HORIZONTALALIGNMENT_center;
    // center the mark only with @startid
    if (repeatMark->GetStart()->Is(TIMESTAMP_ATTR)) {
        alignment = HORIZONTALALIGNMENT_left;
    }

    std::u32string str;
    str.push_back(code);

    std::vector<Staff *> staffList = repeatMark->GetTstampStaves(measure, repeatMark);
    for (Staff *staff : staffList) {
        if (!system->SetCurrentFloatingPositioner(staff->GetN(), repeatMark, repeatMark->GetStart(), staff)) {
            continue;
        }
        const int staffSize = staff->m_drawingStaffSize;

        const int y = repeatMark->GetDrawingY();

        dc->SetFont(m_doc->GetDrawingSmuflFont(staffSize, false));

        if (symbolDef) {
            this->DrawSymbolDef(dc, repeatMark, symbolDef, x, y, staffSize, false, alignment);
        }
        else {
            this->DrawSmuflString(dc, x, y, str, alignment, staffSize);
        }

        dc->ResetFont();
    }

    dc->EndGraphic(repeatMark, this);
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
        tempoTxt.SetFaceName(m_doc->GetResources().GetTextFont());
        tempoTxt.SetWeight(FONTWEIGHT_bold);
    }

    int lineCount = tempo->GetNumberOfLines(tempo);

    data_HORIZONTALALIGNMENT alignment = tempo->GetChildRendAlignment();
    // Tempo are left aligned by default;
    if (alignment == 0) alignment = HORIZONTALALIGNMENT_left;

    std::vector<Staff *> staffList = tempo->GetTstampStaves(measure, tempo);
    for (Staff *staff : staffList) {
        if (!system->SetCurrentFloatingPositioner(staff->GetN(), tempo, tempo->GetStart(), staff)) {
            continue;
        }
        const int staffSize = staff->m_drawingStaffSize;

        TextDrawingParams params;
        params.m_x = tempo->GetDrawingXRelativeToStaff(staff->GetN());
        params.m_y = tempo->GetDrawingY();
        params.m_pointSize = m_doc->GetDrawingLyricFont(staffSize)->GetPointSize();

        tempoTxt.SetPointSize(params.m_pointSize);

        if (tempo->GetPlace() == STAFFREL_between) {
            if (lineCount > 1) {
                params.m_y += (m_doc->GetTextLineHeight(&tempoTxt, false) * (lineCount - 1) / 2);
            }
            params.m_y -= m_doc->GetTextXHeight(&tempoTxt, false) / 2;
        }

        dc->SetFont(&tempoTxt);

        dc->StartText(this->ToDeviceContextX(params.m_x), this->ToDeviceContextY(params.m_y), alignment);
        this->DrawTextChildren(dc, tempo, params);
        dc->EndText();

        dc->ResetFont();

        this->DrawTextEnclosure(dc, params, staffSize);
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

    SymbolDef *symbolDef = NULL;
    if (trill->HasAltsym() && trill->HasAltSymbolDef()) {
        symbolDef = trill->GetAltSymbolDef();
    }

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

    std::vector<Staff *> staffList = trill->GetTstampStaves(measure, trill);
    for (Staff *staff : staffList) {
        if (!system->SetCurrentFloatingPositioner(staff->GetN(), trill, trill->GetStart(), staff)) {
            continue;
        }
        const int staffSize = staff->m_drawingStaffSize;
        const int y = trill->GetDrawingY();

        const int trillHeight = (symbolDef) ? symbolDef->GetSymbolHeight(m_doc, staffSize, false)
                                            : m_doc->GetGlyphHeight(code, staffSize, false);
        const int trillWidth = (symbolDef) ? symbolDef->GetSymbolWidth(m_doc, staffSize, false)
                                           : m_doc->GetGlyphWidth(code, staffSize, false);

        dc->SetFont(m_doc->GetDrawingSmuflFont(staffSize, false));

        // Upper and lower accidentals are currently exclusive, but sould both be allowed at the same time.
        if (trill->HasAccidlower()) {
            int accidXShift = (alignment == HORIZONTALALIGNMENT_center) ? 0 : trillWidth / 2;
            char32_t accid = Accid::GetAccidGlyph(trill->GetAccidlower());
            std::u32string accidStr;
            accidStr.push_back(accid);
            int accidY = y - m_doc->GetGlyphTop(accid, staffSize / 2, true) - m_doc->GetDrawingUnit(staffSize * 2 / 3);
            this->DrawSmuflString(
                dc, x + accidXShift, accidY, accidStr, HORIZONTALALIGNMENT_center, staffSize / 2, false);
        }
        else if (trill->HasAccidupper()) {
            int accidXShift = (alignment == HORIZONTALALIGNMENT_center) ? 0 : trillWidth / 2;
            char32_t accid = Accid::GetAccidGlyph(trill->GetAccidupper());
            std::u32string accidStr;
            accidStr.push_back(accid);
            int accidY = y + trillHeight - m_doc->GetGlyphBottom(accid, staffSize / 2, true)
                + m_doc->GetDrawingUnit(staffSize * 2 / 3);
            this->DrawSmuflString(
                dc, x + accidXShift, accidY, accidStr, HORIZONTALALIGNMENT_center, staffSize / 2, false);
        }

        if (symbolDef) {
            this->DrawSymbolDef(dc, trill, symbolDef, x, y, staffSize, false, alignment);
        }
        else {
            this->DrawSmuflString(dc, x, y, str, alignment, staffSize);
        }

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

    SymbolDef *symbolDef = NULL;
    if (turn->HasAltsym() && turn->HasAltSymbolDef()) {
        symbolDef = turn->GetAltSymbolDef();
    }

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

    std::vector<Staff *> staffList = turn->GetTstampStaves(measure, turn);
    for (Staff *staff : staffList) {
        if (!system->SetCurrentFloatingPositioner(staff->GetN(), turn, turn->GetStart(), staff)) {
            continue;
        }
        const int staffSize = staff->m_drawingStaffSize;

        const int y = turn->GetDrawingY();

        const int turnHeight = (symbolDef) ? symbolDef->GetSymbolHeight(m_doc, staffSize, false)
                                           : m_doc->GetGlyphHeight(code, staffSize, false);
        const int turnWidth = (symbolDef) ? symbolDef->GetSymbolWidth(m_doc, staffSize, false)
                                          : m_doc->GetGlyphWidth(code, staffSize, false);

        dc->SetFont(m_doc->GetDrawingSmuflFont(staffSize, false));

        if (turn->HasAccidlower()) {
            int accidXShift = (alignment == HORIZONTALALIGNMENT_center) ? 0 : turnWidth / 2;
            char32_t accid = Accid::GetAccidGlyph(turn->GetAccidlower());
            std::u32string accidStr;
            accidStr.push_back(accid);
            int accidY = y - m_doc->GetGlyphTop(accid, staffSize / 2, true) - m_doc->GetDrawingUnit(staffSize * 2 / 3);
            this->DrawSmuflString(
                dc, x + accidXShift, accidY, accidStr, HORIZONTALALIGNMENT_center, staffSize / 2, false);
        }
        if (turn->HasAccidupper()) {
            int accidXShift = (alignment == HORIZONTALALIGNMENT_center) ? 0 : turnWidth / 2;
            data_ACCIDENTAL_WRITTEN glyph = turn->GetAccidupper();
            char32_t accid = Accid::GetAccidGlyph(glyph);
            std::u32string accidStr;
            accidStr.push_back(accid);
            int accidY = y + turnHeight - m_doc->GetGlyphBottom(accid, staffSize / 2, true)
                + m_doc->GetDrawingUnit(staffSize * 2 / 3);
            this->DrawSmuflString(
                dc, x + accidXShift, accidY, accidStr, HORIZONTALALIGNMENT_center, staffSize / 2, false);
        }

        if (symbolDef) {
            this->DrawSymbolDef(dc, turn, symbolDef, x, y, staffSize, false, alignment);
        }
        else {
            this->DrawSmuflString(dc, x, y, str, alignment, staffSize);
        }

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
    System *parentSystem1 = vrv_cast<System *>(ending->GetFirstAncestor(SYSTEM));
    System *parentSystem2 = vrv_cast<System *>(endingEndMilestone->GetFirstAncestor(SYSTEM));

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
        measure = vrv_cast<Measure *>(system->FindDescendantByType(MEASURE, 1, BACKWARD));
        if (!measure) return;
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
        measure = vrv_cast<Measure *>(system->FindDescendantByType(MEASURE, 1, FORWARD));
        if (!measure) return;
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
        measure = vrv_cast<Measure *>(system->FindDescendantByType(MEASURE, 1, FORWARD));
        if (!measure) return;
        x1 = measure->GetDrawingX() + measure->GetLeftBarLineXRel();
        objectX = measure->GetLeftBarLine();
        endingMeasure = measure;
        // We need the last measure of the system for x2
        measure = vrv_cast<Measure *>(system->FindDescendantByType(MEASURE, 1, BACKWARD));
        if (!measure) return;
        x2 = measure->GetDrawingX() + measure->GetRightBarLineXRel();
        spanningType = SPANNING_MIDDLE;
    }

    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START)) {
        dc->ResumeGraphic(ending, ending->GetID());
    }
    else {
        dc->StartGraphic(ending, "", ending->GetID(), SPANNING);
    }

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

    for (Staff *staff : staffList) {
        if (!system->SetCurrentFloatingPositioner(staff->GetN(), ending, objectX, staff)) {
            continue;
        }
        const int staffSize = staff->m_drawingStaffSize;
        const int y1 = ending->GetDrawingY();

        dc->StartCustomGraphic("voltaBracket");

        FontInfo currentFont = *m_doc->GetDrawingLyricFont(staffSize);
        // currentFont.SetWeight(FONTWEIGHT_bold);
        // currentFont.SetPointSize(currentFont.GetPointSize() * 2 / 3);
        dc->SetFont(&currentFont);

        TextExtend extend;
        dc->GetTextExtent("M", &extend, false);

        const int unit = m_doc->GetDrawingUnit(staffSize);
        if (ending->HasN() || ending->HasLabel()) {
            const std::string endingText = (ending->HasN()) ? ending->GetN() : ending->GetLabel();
            std::stringstream strStream;
            // Maybe we want to add ( ) after system breaks? Or . as a styling options?
            if ((spanningType == SPANNING_END) || (spanningType == SPANNING_MIDDLE)) strStream << "(";
            strStream << endingText; // << ".";
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

            dc->StartText(
                this->ToDeviceContextX(params.m_x), this->ToDeviceContextY(params.m_y), HORIZONTALALIGNMENT_left);
            this->DrawTextElement(dc, &text, params);
            dc->EndText();
        }

        dc->ResetFont();

        const int y2 = y1 + extend.m_height + unit * 2 / 3;
        const int lineWidth = m_options->m_repeatEndingLineThickness.GetValue() * unit;

        const int staffLineWidth = m_options->m_staffLineWidth.GetValue() * unit;
        const int startX = x1 - staffLineWidth;

        const int rightBarLineWidth = endingMeasure->CalculateRightBarLineWidth(m_doc, staffSize);
        int endX = x2;
        if ((spanningType == SPANNING_START) || (spanningType == SPANNING_MIDDLE)
            || (endingMeasure == system->FindDescendantByType(MEASURE, 1, BACKWARD))) {
            // Right align the ending in the last measure of the system
            endX += rightBarLineWidth - lineWidth / 2 - staffLineWidth;
        }
        else if (endingMeasure->GetDrawingRightBarLine() != BARRENDITION_invis) {
            // Ensure that successive endings do not overlap horizontally
            endX -= std::max(lineWidth + unit / 2 - rightBarLineWidth, 0);
        }

        PenStyle penStyle = PEN_SOLID;
        LineCapStyle capStyle = LINECAP_SQUARE;
        switch (ending->GetLform()) {
            case (LINEFORM_dashed): penStyle = PEN_LONG_DASH; break;
            case (LINEFORM_dotted):
                penStyle = PEN_DOT;
                capStyle = LINECAP_ROUND;
                break;
            default: penStyle = PEN_SOLID;
        }

        dc->SetPen(lineWidth, penStyle, 0, 0, capStyle);
        dc->DrawLine(this->ToDeviceContextX(startX), this->ToDeviceContextY(y2), this->ToDeviceContextX(endX),
            this->ToDeviceContextY(y2));
        if ((spanningType != SPANNING_END) && (spanningType != SPANNING_MIDDLE)
            && (ending->GetLstartsym() != LINESTARTENDSYMBOL_none)) {
            dc->DrawLine(this->ToDeviceContextX(startX), this->ToDeviceContextY(y2), this->ToDeviceContextX(startX),
                this->ToDeviceContextY(y1));
        }
        if ((spanningType != SPANNING_START) && (spanningType != SPANNING_MIDDLE)
            && (ending->GetLendsym() != LINESTARTENDSYMBOL_none)) {
            dc->DrawLine(this->ToDeviceContextX(endX), this->ToDeviceContextY(y2), this->ToDeviceContextX(endX),
                this->ToDeviceContextY(y1));
        }

        dc->ResetPen();

        dc->EndCustomGraphic();
    }

    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START)) {
        dc->EndResumedGraphic(ending, this);
    }
    else {
        dc->EndGraphic(ending, this);
    }
}

void View::DrawTextEnclosure(DeviceContext *dc, const TextDrawingParams &params, int staffSize)
{
    assert(dc);
    const int lineThickness = m_options->m_textEnclosureThickness.GetValue() * staffSize;
    const int margin = m_doc->GetDrawingUnit(staffSize);

    dc->SetPushBack();

    for (const auto rend : params.m_enclosedRend) {
        int x1 = rend->GetContentLeft() - margin;
        int x2 = rend->GetContentRight() + margin;
        int y1 = rend->GetContentBottom() - margin / 2;
        int y2 = rend->GetContentTop() + margin;
        const int width = std::abs(x2 - x1);
        const int height = std::abs(y2 - y1);

        if (params.m_enclose == TEXTRENDITION_box) {
            this->DrawNotFilledRectangle(dc, x1, y1, x2, y2, lineThickness, 0);
        }
        else if (params.m_enclose == TEXTRENDITION_dbox) {
            const int yCenter = y1 + (y2 - y1) / 2;
            this->DrawDiamond(dc, x1 - width / 2, yCenter, height * sqrt(2), width * 2, false, lineThickness);
        }
        else if (params.m_enclose == TEXTRENDITION_circle) {
            if (height > width) {
                // in this case draw a perfect circle
                const int cx = x1 + (x2 - x1) / 2;
                x1 = cx - height / 2;
                x2 = cx + height / 2;
            }
            else if (height < width) {
                x1 -= width / 8;
                x2 += width / 8;
            }
            this->DrawNotFilledEllipse(dc, x1, y1, x2, y2, lineThickness);
        }
    }

    dc->ResetPushBack();
}

} // namespace vrv
