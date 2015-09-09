/////////////////////////////////////////////////////////////////////////////
// Name:        view_floating.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "view.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <iostream>
#include <math.h>

//----------------------------------------------------------------------------

#include "att_comparison.h"
#include "bboxdevicecontext.h"
#include "devicecontext.h"
#include "doc.h"
#include "floatingelement.h"
#include "layer.h"
#include "layerelement.h"
#include "measure.h"
#include "note.h"
#include "slur.h"
#include "staff.h"
#include "style.h"
#include "syl.h"
#include "system.h"
#include "tie.h"
#include "timeinterface.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// View - FloatingElement
//----------------------------------------------------------------------------
    
void View::DrawFloatingElement( DeviceContext *dc, FloatingElement *element, Measure *measure, System *system)
{
    assert( dc );
    assert( system );
    assert( measure );
    assert( element );
    
    if (element->HasInterface(INTERFACE_TIME_SPANNING)) {
        // creating placeholder
        dc->StartGraphic( element, "", element->GetUuid() );
        dc->EndGraphic( element, this);
        system->AddToDrawingList(element);
    }
}

void View::DrawTimeSpanningElement( DeviceContext *dc, DocObject *element, System *system )
{
    assert( dc );
    assert( element );
    assert( system );
    
    TimeSpanningInterface *interface = dynamic_cast<TimeSpanningInterface*>(element);
    assert( interface );
    
    if ( !interface->HasStartAndEnd() ) return;
    
    // Get the parent system of the first and last note
    System *parentSystem1 = dynamic_cast<System*>( interface->GetStart()->GetFirstParent( SYSTEM )  );
    System *parentSystem2 = dynamic_cast<System*>( interface->GetEnd()->GetFirstParent( SYSTEM )  );
    
    int x1, x2;
    Staff *staff = NULL;
    DocObject *graphic = NULL;
    char spanningType = SPANNING_START_END;
    
    // The both correspond to the current system, which means no system break in-between (simple case)
    if (( system == parentSystem1 ) && ( system == parentSystem2 )) {
        // Get the parent staff for calculating the y position
        staff = dynamic_cast<Staff*>( interface->GetStart()->GetFirstParent( STAFF ) );
        if ( !Check( staff ) ) return;
        
        x1 = interface->GetStart()->GetDrawingX();
        x2 = interface->GetEnd()->GetDrawingX();
        graphic = element;
    }
    // Only the first parent is the same, this means that the element is "open" at the end of the system
    else if ( system == parentSystem1 ) {
        // We need the last measure of the system for x2
        Measure *last = dynamic_cast<Measure*>( system->FindChildByType( MEASURE, 1, BACKWARD ) );
        if ( !Check( last ) ) return;
        staff = dynamic_cast<Staff*>( interface->GetStart()->GetFirstParent( STAFF ) );
        if ( !Check( staff ) ) return;
        
        x1 = interface->GetStart()->GetDrawingX();
        x2 = last->GetDrawingX() + last->GetRightBarlineX();
        graphic = element;
        spanningType = SPANNING_START;
    }
    // We are in the system of the last note - draw the element from the beginning of the system
    else if ( system == parentSystem2 ) {
        // We need the first measure of the system for x1
        Measure *first = dynamic_cast<Measure*>( system->FindChildByType( MEASURE, 1, FORWARD ) );
        if ( !Check( first ) ) return;
        // Get the staff of the first note - however, not the staff we need
        Staff *lastStaff = dynamic_cast<Staff*>( interface->GetEnd()->GetFirstParent( STAFF ) );
        if ( !Check( lastStaff ) ) return;
        // We need the first staff from the current system, i.e., the first measure.
        AttCommonNComparison comparison( STAFF, lastStaff->GetN() );
        staff = dynamic_cast<Staff*>(system->FindChildByAttComparison(&comparison, 2));
        if (!staff ) {
            LogDebug("Could not get staff (%d) while drawing staffGrp - View::DrawSylConnector", lastStaff->GetN() );
            return;
        }
        // Also try to get a first note - we should change this once we have a x position in measure that
        // takes into account the scoreDef
        Note *firstNote = dynamic_cast<Note*>( staff->FindChildByType( NOTE ) );
        
        x1 = firstNote ? firstNote->GetDrawingX() - 2 * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) : first->GetDrawingX();
        x2 = interface->GetEnd()->GetDrawingX();
        spanningType = SPANNING_END;
    }
    // Rare case where neither the first note and the last note are in the current system - draw the connector throughout the system
    else {
        // We need the first measure of the system for x1
        Measure *first = dynamic_cast<Measure*>( system->FindChildByType( MEASURE, 1, FORWARD ) );
        if ( !Check( first ) ) return;
        // Also try to get a first note - we should change this once we have a x position in measure that
        // takes into account the scoreDef
        Note *firstNote = dynamic_cast<Note*>( first->FindChildByType( NOTE ) );
        // We need the last measure of the system for x2
        Measure *last = dynamic_cast<Measure*>( system->FindChildByType( MEASURE, 1, BACKWARD ) );
        if ( !Check( last ) ) return;
        // Get the staff of the first note - however, not the staff we need
        Staff *firstStaff = dynamic_cast<Staff*>( interface->GetStart()->GetFirstParent( STAFF ) );
        if ( !Check( firstStaff ) ) return;
        
        // We need the staff from the current system, i.e., the first measure.
        AttCommonNComparison comparison( STAFF, firstStaff->GetN() );
        staff = dynamic_cast<Staff*>(first->FindChildByAttComparison(&comparison, 1));
        if (!staff ) {
            LogDebug("Could not get staff (%d) while drawing staffGrp - View::DrawSylConnector", firstStaff->GetN() );
            return;
        }
        
        x1 = firstNote ? firstNote->GetDrawingX() - 2 * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) : first->GetDrawingX();
        x2 = last->GetDrawingX() + last->GetRightBarlineX();
        spanningType = SPANNING_MIDDLE;
    }
    
    if (element->Is() == SLUR) {
        // cast to Slur check in DrawTieOrSlur
        DrawSlur(dc, dynamic_cast<Slur*>(element), x1, x2, staff, spanningType, graphic);
    }
    else if (element->Is() == SYL) {
        // cast to Syl check in DrawSylConnector
        DrawSylConnector(dc, dynamic_cast<Syl*>(element), x1, x2, staff, spanningType, graphic);
    }
    else if (element->Is() == TIE) {
        // cast to Slur check in DrawTieOrSlur
        DrawTie(dc, dynamic_cast<Tie*>(element), x1, x2, staff, spanningType, graphic);
    }
    
}

void View::DrawSlur( DeviceContext *dc, Slur *slur, int x1, int x2, Staff *staff,
                         char spanningType, DocObject *graphic )
{
    assert( dc );
    assert( slur );
    assert( staff );
    
    LayerElement *start = NULL;
    LayerElement *end = NULL;
    Beam *parentBeam = NULL;
    Chord *startParentChord = NULL;
    Chord *endParentChord = NULL;
    Note *startNote = NULL;
    Note *endNote = NULL;
    Chord *startChord = NULL;
    Chord *endChord = NULL;
    
    bool up = true;
    data_STEMDIRECTION noteStemDir = STEMDIRECTION_NONE;
    int y1 = staff->GetDrawingY();
    int y2 = staff->GetDrawingY();
    
    /************** parent layers **************/
    
    start = dynamic_cast<LayerElement*>(slur->GetStart());
    end = dynamic_cast<LayerElement*>(slur->GetEnd());
    
    if ( !start || !end ) {
        // no note, obviously nothing to do...
        return;
    }
    
    if (start->Is() == NOTE) {
        startNote = dynamic_cast<Note*>(start);
        assert(startNote);
        startParentChord = startNote->IsChordTone();
    }
    else if (start->Is() == CHORD) {
        startChord = dynamic_cast<Chord*>(start);
        assert(startChord);
    }
    if (end->Is() == NOTE) {
        endNote = dynamic_cast<Note*>(end);
        assert(endNote);
        endParentChord = endNote->IsChordTone();
    }
    else if (end->Is() == CHORD) {
        endChord = dynamic_cast<Chord*>(end);
        assert(endChord);
    }
    
    Layer* layer1 = dynamic_cast<Layer*>(start->GetFirstParent( LAYER ) );
    Layer* layer2 = dynamic_cast<Layer*>(end->GetFirstParent( LAYER ) );
    assert( layer1 && layer2 );
    
    if ( layer1->GetN() != layer2->GetN() ) {
        LogWarning("Slurs between different layers may not be fully supported.");
    }
    
    /************** note stem dir **************/
    
    // the normal case
    if (spanningType == SPANNING_START_END) {
        noteStemDir = start->GetDrawingStemDir();
    }
    // This is the case when the tie is split over two system of two pages.
    // In this case, we are now drawing its beginning to the end of the measure (i.e., the last aligner)
    else if (spanningType == SPANNING_START) {
        noteStemDir = start->GetDrawingStemDir();
    }
    // Now this is the case when the tie is split but we are drawing the end of it
    else if (spanningType == SPANNING_END) {
        noteStemDir = end->GetDrawingStemDir();
    }
    // Finally, slur accross an entire system; use the staff position and up (see below)
    else {
        noteStemDir = STEMDIRECTION_down;
    }
    
    /************** direction **************/
    
    // first should be the tie @curvedir
    if (slur->HasCurvedir()) {
        up = (slur->GetCurvedir() == CURVEDIR_above) ? true : false;
    }
    // then layer direction trumps note direction
    else if (layer1 && layer1->GetDrawingStemDir() != STEMDIRECTION_NONE){
        up = layer1->GetDrawingStemDir() == STEMDIRECTION_up ? true : false;
    }
    //  the look if in a chord
    else if (startParentChord) {
        if (startParentChord->PositionInChord(startNote) < 0) up = false;
        else if (startParentChord->PositionInChord(startNote) > 0) up = true;
        // away from the stem if odd number (center note)
        else up = (noteStemDir != STEMDIRECTION_up);
    }
    else if (noteStemDir == STEMDIRECTION_up) {
        up = false;
    }
    else if (noteStemDir == STEMDIRECTION_NONE) {
        // no information from the note stem directions, look at the position in the notes
        int center = staff->GetDrawingY() - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * 2;
        up = (start->GetDrawingY() > center) ? true : false;
    }
    
    /************** adjusting y position **************/
    
    int yChordMax, yChordMin;
    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_START)) {
        // first get the min max of the chord (if any)
        if (startParentChord) startParentChord->GetYExtremes(&yChordMax, &yChordMin);
        else if (startChord) startChord->GetYExtremes(&yChordMax, &yChordMin);
        // slur is up
        if (up) {
            // P(^)
            if (noteStemDir == STEMDIRECTION_down) y1 = start->GetDrawingY();
            //  d(^)d
            else if ((parentBeam = start->IsInBeam()) && !parentBeam->IsLastInBeam(start)) {
                y1 = start->m_drawingStemEnd.y;
            }
            // d(^)
            else {
                x1 += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 3 / 2;
                if (startChord || startParentChord) y1 = yChordMin + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2;
                else y1 = start->GetDrawingY() + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2;
            }
        }
        // slur is down
        else {
            // d(_)
            if (noteStemDir == STEMDIRECTION_up) y1 = start->GetDrawingY();
            // P(_)P
            else if ((parentBeam = start->IsInBeam()) && !parentBeam->IsLastInBeam(start)) {
                y1 = start->m_drawingStemEnd.y;
            }
            // P(_)
            else {
                //x1 += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 3 / 2;
                y1 = start->GetDrawingY() + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2;
                if (startChord || startParentChord) y1 = yChordMin + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2;
                else y1 = start->GetDrawingY() + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2;
            }
        }
    }
    if ((spanningType == SPANNING_START_END) || (spanningType == SPANNING_END)) {
        // get the min max of the chord if any
        if (endParentChord) endParentChord->GetYExtremes(&yChordMax, &yChordMin);
        else if (endChord) endChord->GetYExtremes(&yChordMax, &yChordMin);
        // get the stem direction of the end
        data_STEMDIRECTION endStemDir = end->GetDrawingStemDir();
        // slur is up
        if (up) {
            // (^)P
            if (endStemDir == STEMDIRECTION_down) y2 = end->GetDrawingY();
            // d(^)d
            else if ((parentBeam = end->IsInBeam()) && !parentBeam->IsFirstInBeam(end)) {
                y2 = end->m_drawingStemEnd.y;
            }
            // (^)d
            else {
                //x1 += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 3 / 2;
                if (endChord || endParentChord) y2 = yChordMin + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2;
                else y2 = end->GetDrawingY() + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2;
            }
        }
        else {
            // (_)d
            if (endStemDir == STEMDIRECTION_up) y2 = end->GetDrawingY();
            // P(_)P
            else if ((parentBeam = end->IsInBeam()) && !parentBeam->IsFirstInBeam(end)) {
                y2 = end->m_drawingStemEnd.y;
            }
            // (_)P
            else {
                x2 -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2;
                if (endChord || endParentChord) y2 = yChordMin + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2;
                else y2 = end->GetDrawingY() - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2;
            }
        }
    }
    
    if (spanningType == SPANNING_START) {
        if (up) y2 = std::max(staff->GetDrawingY(), y1);
        else y2 = std::min(staff->GetDrawingY() - m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize), y1);
    }
    // Now this is the case when the tie is split but we are drawing the end of it
    else if (spanningType == SPANNING_END) {
        if (up) y1 = std::max(staff->GetDrawingY(), y2);
        else y1 = std::min(staff->GetDrawingY() - m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize), y2);
    }
    // Finally, slur accross an entire system; use the staff position and up (see below)
    else if (spanningType != SPANNING_START_END) {
        // To be adjusted
        if (up) y1 = staff->GetDrawingY();
        else y1 = staff->GetDrawingY() - m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize);
        y2 = y1;
    }

    /************** y position **************/
    
    if (up) {
        y1 += 2 * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        y2 += 2 * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    }
    else {
        y1 -= 2 * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        y2 -= 2 * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    }
    
    Point points[4];
    points[0] = Point(x1, y1);
    points[1] = Point(x2, y2);
    
    float angle = AdjustSlurPosition(slur, staff, layer1->GetN(), up, points);
    
    int thickness =  m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * m_doc->GetSlurThickness() / DEFINITON_FACTOR ;
    
    if ( graphic ) dc->ResumeGraphic(graphic, graphic->GetUuid());
    else dc->StartGraphic(slur, "spanning-slur", "");
    dc->DeactivateGraphic();
    DrawThickBezierCurve(dc, points[0], points[1], points[2], points[3], thickness, staff->m_drawingStaffSize, angle);
    dc->ReactivateGraphic();
    
    if ( graphic ) dc->EndResumedGraphic(graphic, this);
    else dc->EndGraphic(slur, this);
}
    
float View::AdjustSlurPosition(Slur *slur, Staff *staff, int layerN, bool up,  Point points[])
{
    // For readability makes them p1 and p2
    Point *p1 = &points[0];
    Point *p2 = &points[1];
    
    /************** angle **************/
    
    float slurAngle = atan2(p2->y - p1->y, p2->x - p1->x);
    
    // the slope of the slur is high and needs to be corrected
    if (fabs(slurAngle) > TEMP_STYLE_MAX_SLUR_SLOPE) {
        int side = (p2->x - p1->x) * sin(TEMP_STYLE_MAX_SLUR_SLOPE) / sin(M_PI / 2 - TEMP_STYLE_MAX_SLUR_SLOPE);
        if (p2->y > p1->y) {
            if (up) p1->y = p2->y - side;
            else p2->y = p1->y + side;
            slurAngle = TEMP_STYLE_MAX_SLUR_SLOPE;
        }
        else {
            if (up) p2->y = p1->y - side;
            else p1->y = p2->y + side;
            slurAngle = -TEMP_STYLE_MAX_SLUR_SLOPE;
        }
    }
    
    Point rotatedP2 = View::CalcPositionAfterRotation(*p2, -slurAngle, *p1);
    //LogDebug("P1 %d %d, P2 %d %d, Angle %f, Pres %d %d", x1, y1, x2, y2, slurAnge, rotadedP2.x, rotatedP2.y);
    
    /************** height **************/
    
    // the 'height' of the bezier
    int height;
    if (slur->HasBulge()){
        height = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * slur->GetBulge();
    }
    else {
        int dist = abs( p2->x - p1->x );
        height = std::max(  m_doc->GetSlurMinHeight() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / DEFINITON_FACTOR, dist / TEMP_STYLE_SLUR_HEIGHT_FACTOR);
        height = std::min( m_doc->GetSlurMaxHeight() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / DEFINITON_FACTOR, height );
    }
    
    // the height of the control points
    height = height * 4 / 3;
    
    /************** rotation **************/
    
    // control points
    Point rotatecC1, rotatedC2;
    
    int cPos = std::min((rotatedP2.x - p1->x) / TEMP_STYLE_SLUR_CONTROL_POINT_FACTOR, m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize));
    rotatecC1.x = p1->x + cPos; // point at 1/4
    rotatedC2.x = rotatedP2.x - cPos;; // point at 3/4
    
    if (up) {
        rotatecC1.y = p1->y + height;
        rotatedC2.y = rotatedP2.y + height;
    } else {
        rotatecC1.y = p1->y - height;
        rotatedC2.y = rotatedP2.y - height;
    }
    
    /************** content **************/
    
    
    System *system = dynamic_cast<System*>(staff->GetFirstParent(SYSTEM));
    assert(system);
    std::vector<LayerElement*>spanningContent;
    ArrayPtrVoid params;
    params.push_back(&spanningContent);
    params.push_back(&p1->x);
    params.push_back(&p2->x);
    std::vector<AttComparison*> filters;
    // Create ad comparison object for each type / @n
    // For now we only look at one layer (assumed layer1 == layer2)
    AttCommonNComparison matchStaff( STAFF, staff->GetN() );
    AttCommonNComparison matchLayer( LAYER, layerN );
    filters.push_back( &matchStaff );
    filters.push_back( &matchLayer );
    
    Functor timeSpanningLayerElements( &Object::TimeSpanningLayerElements );
    //LogDebug("*** %d - %d", note1->GetDrawingX(), note2->GetDrawingX() ) ;
    system->Process( &timeSpanningLayerElements, &params, NULL, &filters );
    if (spanningContent.size() > 12) LogDebug("### %d %s", spanningContent.size(), slur->GetUuid().c_str());
    
    
    points[1] = View::CalcPositionAfterRotation(rotatedP2, slurAngle, *p1);
    points[2] = View::CalcPositionAfterRotation(rotatecC1, slurAngle, *p1);
    points[3] = View::CalcPositionAfterRotation(rotatedC2, slurAngle, *p1);
    
    
    /*
     Point bezier[4];
     bezier[0].x = 100;
     bezier[1].x = 200;
     bezier[2].x = 300;
     bezier[3].x = 400;
     bezier[0].y = 000;
     bezier[1].y = 100;
     bezier[2].y = 100;
     bezier[3].y = 000;
     
     int p = View::CalcBezierAtPosition(bezier, 24550);
     p;
     */
    
    return slurAngle;
}
    
void View::DrawTie( DeviceContext *dc, Tie *tie, int x1, int x2, Staff *staff,
                         char spanningType, DocObject *graphic )
{
    assert( dc );
    assert( tie );
    assert( staff );
    
    Note *note1 = NULL;
    Note *note2 = NULL;
    Chord *parentChord = NULL;
    
    bool up = true;
    data_STEMDIRECTION noteStemDir = STEMDIRECTION_NONE;
    int y1, y2;
    
    /************** parent layers **************/
    
    note1 = dynamic_cast<Note*>(tie->GetStart());
    note2 = dynamic_cast<Note*>(tie->GetEnd());
    
    if ( !note1 || !note2 ) {
        // no note, obviously nothing to do...
        return;
    }
    
    Chord *chordParent1 = note1->IsChordTone();
    
    Layer* layer1 = dynamic_cast<Layer*>(note1->GetFirstParent( LAYER ) );
    Layer* layer2 = dynamic_cast<Layer*>(note2->GetFirstParent( LAYER ) );
    assert( layer1 && layer2 );
    
    if ( layer1->GetN() != layer2->GetN() ) {
        LogWarning("Ties between different layers may not be fully supported.");
    }
    
    /************** x positions **************/
    
    bool isShortTie = false;
    // shortTie correction cannot be applied for chords
    if (!chordParent1 && (x2 - x1 < 3 * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize))) isShortTie = true;
    
    // the normal case
    if ( spanningType == SPANNING_START_END ) {
        y1 = note1->GetDrawingY();
        y2 = note2->GetDrawingY();
        if (!isShortTie) {
            x1 += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 3 / 2;
            x2 -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 3 / 2;
            if (note1->HasDots()) {
                x1 += m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * note1->GetDots();
            }
            else if (chordParent1 && chordParent1->HasDots()) {
                x1 += m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * chordParent1->GetDots();
            }
        }
        noteStemDir = note1->GetDrawingStemDir();
    }
    // This is the case when the tie is split over two system of two pages.
    // In this case, we are now drawing its beginning to the end of the measure (i.e., the last aligner)
    else if ( spanningType == SPANNING_START ) {
        y1 = note1->GetDrawingY();
        y2 = y1;
        if (!isShortTie) {
            x1 += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 3 / 2;
        }
        noteStemDir = note1->GetDrawingStemDir();
    }
    // Now this is the case when the tie is split but we are drawing the end of it
    else if ( spanningType == SPANNING_END ) {
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
        up = (tie->GetCurvedir() == CURVEDIR_above) ? true : false;
    }
    // then layer direction trumps note direction
    else if (layer1 && layer1->GetDrawingStemDir() != STEMDIRECTION_NONE){
        up = layer1->GetDrawingStemDir() == STEMDIRECTION_up ? true : false;
    }
    //  the look if in a chord
    else if (parentChord) {
        if (parentChord->PositionInChord(note1) < 0) up = false;
        else if (parentChord->PositionInChord(note1) > 0) up = true;
        // away from the stem if odd number (center note)
        else up = (noteStemDir != STEMDIRECTION_up);
    }
    else if (noteStemDir == STEMDIRECTION_up) {
        up = false;
    }
    else if (noteStemDir == STEMDIRECTION_NONE) {
        // no information from the note stem directions, look at the position in the notes
        int center = staff->GetDrawingY() - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * 2;
        up = (y1 > center) ? true : false;
    }
    
    /************** y position **************/

    if (up) {
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
    if (tie->HasBulge()){
        height = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * tie->GetBulge();
    }
    else {
        height = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        // if the space between the to points is more than two staff height, increase the height
        if (x2 - x1 > 2 * m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize)) {
            height +=  m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        }
    }
    int thickness =  m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * m_doc->GetTieThickness() / DEFINITON_FACTOR ;
    
    // control points
    Point c1, c2;
    
    // the height of the control points
    height = height * 4 / 3;
    
    c1.x = x1 + (x2 - x1) / 4; // point at 1/4
    c2.x = x1 + (x2 - x1) / 4 * 3; // point at 3/4
    
    if (up) {
        c1.y = y1 + height;
        c2.y = y2 + height;
    } else {
        c1.y = y1 - height;
        c2.y = y2 - height;
    }
    
    if ( graphic ) dc->ResumeGraphic(graphic, graphic->GetUuid());
    else dc->StartGraphic(tie, "spanning-tie", "");
    dc->DeactivateGraphic();
    DrawThickBezierCurve(dc, Point(x1, y1), Point(x2, y2), c1, c2, thickness, staff->m_drawingStaffSize );
    dc->ReactivateGraphic();
    if ( graphic ) dc->EndResumedGraphic(graphic, this);
    else dc->EndGraphic(tie, this);
}

void View::DrawSylConnector( DeviceContext *dc, Syl *syl, int x1, int x2, Staff *staff, char spanningType, DocObject *graphic )
{
    assert( syl );
    assert( syl->GetStart() && syl->GetEnd() );
    if ( !syl->GetStart() || !syl->GetEnd() ) return;
    
    int y = GetSylY(syl, staff);
    int w, h;
    
    // The both correspond to the current system, which means no system break in-between (simple case)
    if ( spanningType ==  SPANNING_START_END ) {
        dc->SetFont( m_doc->GetDrawingLyricFont( staff->m_drawingStaffSize ) );
        dc->GetTextExtent(syl->GetText(), &w, &h);
        dc->ResetFont();
        // x position of the syl is two units back
        x1 += w - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2;
    }
    // Only the first parent is the same, this means that the syl is "open" at the end of the system
    else  if ( spanningType ==  SPANNING_START) {
        dc->SetFont( m_doc->GetDrawingLyricFont( staff->m_drawingStaffSize ) );
        dc->GetTextExtent(syl->GetText(), &w, &h);
        dc->ResetFont();
        // idem
        x1 += w - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2;
        
    }
    // We are in the system of the last note - draw the connector from the beginning of the system
    else if ( spanningType ==  SPANNING_END ) {
        // nothing to adjust
    }
    // Rare case where neither the first note and the last note are in the current system - draw the connector throughout the system
    else {
        // nothing to adjust
    }
    
    if ( graphic ) dc->ResumeGraphic(graphic, graphic->GetUuid());
    else dc->StartGraphic(syl, "spanning-connector", "");
    dc->DeactivateGraphic();
    DrawSylConnectorLines( dc, x1, x2, y, syl, staff);
    dc->ReactivateGraphic();
    if ( graphic ) dc->EndResumedGraphic(graphic, this);
    else dc->EndGraphic(syl, this);
    
}

void View::DrawSylConnectorLines( DeviceContext *dc, int x1, int x2, int y, Syl *syl, Staff *staff )
{
    if (syl->GetCon() == CON_d) {
        
        y += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2 / 3;
        // x position of the syl is two units back
        x2 -= 2 * (int)m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        
        //if ( x1 > x2 ) {
        //    DrawFullRectangle(dc, x1, y + 2* m_doc->GetDrawingBarLineWidth(staff->m_drawingStaffSize), x2, y + 3 * m_doc->GetDrawingBarLineWidth(staff->m_drawingStaffSize));
        //    LogDebug("x1 > x2 (%d %d)", x1, x2 );
        //}
        
        // the length of the dash and the space between them - can be made a parameter
        int dashLength = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 4 / 3;
        int dashSpace = m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize) * 5 / 3;
        int halfDashLength = dashLength / 2;
        
        int dist = x2 - x1;
        int nbDashes = dist / dashSpace;
        
        int margin = dist / 2;
        // at least one dash
        if (nbDashes < 2) {
            nbDashes = 1;
        }
        else {
            margin = (dist - ((nbDashes - 1) * dashSpace)) / 2;
        }
        margin -= dashLength / 2;
        int i, x;
        for (i = 0; i < nbDashes; i++) {
            x = x1 + margin + (i *  dashSpace);
            DrawFullRectangle(dc, x - halfDashLength, y, x + halfDashLength, y + m_doc->GetDrawingBarLineWidth(staff->m_drawingStaffSize));
        }
        
    }
    else if (syl->GetCon() == CON_u) {
        x1 += (int)m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2;
        DrawFullRectangle(dc, x1, y, x2, y + m_doc->GetDrawingBarLineWidth(staff->m_drawingStaffSize));
    }
    
}

} // namespace vrv    
    
