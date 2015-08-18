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

//----------------------------------------------------------------------------

//#include "accid.h"
#include "att_comparison.h"
//#include "beam.h"
//#include "chord.h"
//#include "custos.h"
#include "devicecontext.h"
#include "doc.h"
//#include "dot.h"
#include "floatingelement.h"
//#include "keysig.h"
#include "layer.h"
#include "layerelement.h"
#include "measure.h"
//#include "mensur.h"
//#include "metersig.h"
//#include "mrest.h"
//#include "multirest.h"
#include "note.h"
//#include "rest.h"
#include "slur.h"
//#include "space.h"
//#include "smufl.h"
#include "staff.h"
#include "style.h"
#include "syl.h"
#include "system.h"
#include "tie.h"
#include "timeinterface.h"
//#include "tuplet.h"
//#include "verse.h"
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
        
        x1 = firstNote ? firstNote->GetDrawingX() - 2 * m_doc->GetDrawingDoubleUnit(staff->staffSize) : first->GetDrawingX();
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
        
        x1 = firstNote ? firstNote->GetDrawingX() - 2 * m_doc->GetDrawingDoubleUnit(staff->staffSize) : first->GetDrawingX();
        x2 = last->GetDrawingX() + last->GetRightBarlineX();
        spanningType = SPANNING_MIDDLE;
    }
    
    if (element->Is() == SLUR) {
        // cast to Slur check in DrawTieOrSlur
        DrawTieOrSlur(dc, dynamic_cast<Slur*>(element), x1, x2, staff, spanningType, graphic);
    }
    else if (element->Is() == SYL) {
        // cast to Syl check in DrawSylConnector
        DrawSylConnector(dc, dynamic_cast<Syl*>(element), x1, x2, staff, spanningType, graphic);
    }
    else if (element->Is() == TIE) {
        // cast to Slur check in DrawTieOrSlur
        DrawTieOrSlur(dc, dynamic_cast<Tie*>(element), x1, x2, staff, spanningType, graphic);
    }
    
}

void View::DrawTieOrSlur( DeviceContext *dc, FloatingElement *element, int x1, int x2, Staff *staff,
                         char spanningType, DocObject *graphic )
{
    assert( dc );
    assert( element );
    assert( staff );
    
    assert(dynamic_cast<Slur*>(element) || dynamic_cast<Tie*>(element)); // Element must be a Tie or a Slur
    
    TimeSpanningInterface *interface = dynamic_cast<TimeSpanningInterface*>(element);
    assert( interface );
    
    LayerElement *element1 = NULL;
    LayerElement *element2 = NULL;
    
    bool up = true;
    data_STEMDIRECTION noteStemDir = STEMDIRECTION_NONE;
    int y1, y2;
    
    element1 = interface->GetStart();
    element2 = interface->GetEnd();
    
    if ( !element1 || !element2 ) {
        // no note, obviously nothing to do...
        return;
    }
    
    Note *note1 = NULL;
    Note *note2 = NULL;
    Chord *chord1 = NULL;
    Chord *chord2 = NULL;
    if (element1->Is() == NOTE ) note1 = dynamic_cast<Note*>(element1);
    else chord1 = dynamic_cast<Chord*>(element1);
    assert( note1 || chord1 );
    if (element2->Is() == NOTE ) note2 = dynamic_cast<Note*>(element2);
    else chord2 = dynamic_cast<Chord*>(element2);
    assert( note2 || chord2 );
    
    Layer* layer1 = dynamic_cast<Layer*>(element1->GetFirstParent( LAYER ) );
    Layer* layer2 = dynamic_cast<Layer*>(element2->GetFirstParent( LAYER ) );
    assert( layer1 && layer2 );
    
    if ( layer1->GetN() != layer2->GetN() ) {
        LogWarning("Ties between different layers may not be fully supported.");
    }
    
    //the normal case
    if ( spanningType == SPANNING_START_END ) {
        // Copied from DrawNote
        // We could use the stamDir information
        // but then we have to take in account (1) beams (2) stemmed and non stemmed notes tied together
        y1 = element1->GetDrawingY();
        y2 = element2->GetDrawingY();
        // for now we only look at the first note - needs to be improved
        // m_drawingStemDir it not set properly in beam - needs to be fixed.
        if (note1) noteStemDir = note1->m_drawingStemDir;
        else noteStemDir = chord1->GetDrawingStemDir();
    }
    // This is the case when the tie is split over two system of two pages.
    // In this case, we are now drawing its beginning to the end of the measure (i.e., the last aligner)
    else if ( spanningType == SPANNING_START ) {
        y1 = element1->GetDrawingY();
        y2 = y1;
        // m_drawingStemDir it not set properly in beam - needs to be fixed.
        if (note1) noteStemDir = note1->m_drawingStemDir;
        else noteStemDir = chord1->GetDrawingStemDir();
    }
    // Now this is the case when the tie is split but we are drawing the end of it
    else if ( spanningType == SPANNING_END ) {
        y1 = element2->GetDrawingY();
        y2 = y1;
        x2 = element2->GetDrawingX();
        if (note2) noteStemDir = note2->m_drawingStemDir;
        else noteStemDir = chord2->GetDrawingStemDir();
    }
    // Finally
    else {
        LogDebug("Slur across an entire system is not supported");
        return;
    }
    
    //layer direction trumps note direction
    if (layer1 && layer1->GetDrawingStemDir() != STEMDIRECTION_NONE){
        up = layer1->GetDrawingStemDir() == STEMDIRECTION_up ? true : false;
    }
    else if (noteStemDir == STEMDIRECTION_up) {
        up = false;
    }
    else if (noteStemDir == STEMDIRECTION_NONE) {
        // no information from the note stem directions, look at the position in the notes
        int center = staff->GetDrawingY() - m_doc->GetDrawingDoubleUnit(staff->staffSize) * 2;
        up = (y1 > center) ? true : false;
    }
    
    // FIXME, take in account elements that can be netween notes, eg keys time etc
    // 20 height nice with 70, not nice with 50
    // Also remove HARDCODED values!
    if (up) {
        y1 += m_doc->GetDrawingUnit(staff->staffSize) * 1.6;
        y2 += m_doc->GetDrawingUnit(staff->staffSize) * 1.6;
    }
    else {
        y1 -= m_doc->GetDrawingUnit(staff->staffSize) * 1.6;
        y2 -= m_doc->GetDrawingUnit(staff->staffSize) * 1.6;
    }
    
    if ( graphic ) dc->ResumeGraphic(graphic, graphic->GetUuid());
    else dc->StartGraphic(element, "spanning-tie-or-slur", "");
    dc->DeactivateGraphic();
    DrawTieOrSlurBezier(dc, x1, y1, x2, y2, !up);
    dc->ReactivateGraphic();
    
    if ( graphic ) dc->EndResumedGraphic(graphic, this);
    else dc->EndGraphic(element, this);
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
        dc->SetFont( m_doc->GetDrawingLyricFont( staff->staffSize ) );
        dc->GetTextExtent(syl->GetText(), &w, &h);
        dc->ResetFont();
        // x position of the syl is two units back
        x1 += w - m_doc->GetDrawingUnit(staff->staffSize) * 2;
    }
    // Only the first parent is the same, this means that the syl is "open" at the end of the system
    else  if ( spanningType ==  SPANNING_START) {
        dc->SetFont( m_doc->GetDrawingLyricFont( staff->staffSize ) );
        dc->GetTextExtent(syl->GetText(), &w, &h);
        dc->ResetFont();
        // idem
        x1 += w - m_doc->GetDrawingUnit(staff->staffSize) * 2;
        
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
        
        y += m_doc->GetDrawingUnit(staff->staffSize) * 2 / 3;
        // x position of the syl is two units back
        x2 -= 2 * (int)m_doc->GetDrawingUnit(staff->staffSize);
        
        //if ( x1 > x2 ) {
        //    DrawFullRectangle(dc, x1, y + 2* m_doc->m_style->m_barlineWidth, x2, y + 3 * m_doc->m_style->m_barlineWidth);
        //    LogDebug("x1 > x2 (%d %d)", x1, x2 );
        //}
        
        // the length of the dash and the space between them - can be made a parameter
        int dashLength = m_doc->GetDrawingUnit(staff->staffSize) * 4 / 3;
        int dashSpace = m_doc->GetDrawingStaffSize(staff->staffSize) * 5 / 3;
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
            DrawFullRectangle(dc, x - halfDashLength, y, x + halfDashLength, y + m_doc->m_style->m_barlineWidth);
        }
        
    }
    else if (syl->GetCon() == CON_u) {
        x1 += (int)m_doc->GetDrawingUnit(staff->staffSize) / 2;
        DrawFullRectangle(dc, x1, y, x2, y + m_doc->m_style->m_barlineWidth);
    }
    
}

} // namespace vrv    
    
