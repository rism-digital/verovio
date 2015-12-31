/////////////////////////////////////////////////////////////////////////////
// Name:        view_page.cpp
// Author:      Laurent Pugin and Chris Niven
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "view.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "attcomparison.h"
#include "beam.h"
#include "devicecontext.h"
#include "doc.h"
#include "editorial.h"
#include "floatingelement.h"
#include "keysig.h"
#include "layer.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "note.h"
#include "page.h"
#include "smufl.h"
#include "staff.h"
#include "style.h"
#include "system.h"
#include "syl.h"
#include "text.h"
#include "tuplet.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// View - Page
//----------------------------------------------------------------------------

void View::DrawCurrentPage( DeviceContext *dc, bool background )
{
	assert( dc );
    assert( m_doc );
    
    m_currentPage = m_doc->SetDrawingPage( m_pageIdx );
    
    int i;
	System *system = NULL;
    Measure *measure = NULL;
    Staff *staff = NULL;
    Layer *layer = NULL;
    bool processLayerElement = false;
    ArrayPtrVoid params;
    params.push_back( m_doc );
    params.push_back( &system );
    params.push_back( &measure );
    params.push_back( &staff );
    params.push_back( &layer );
    params.push_back( this );
    params.push_back( &processLayerElement );
    Functor setDrawingXY( &Object::SetDrawingXY );
    // First pass without processing the LayerElements - we need this for cross-staff going down because
    // the elements will need the position of the staff below to have been set before
    m_currentPage->Process( &setDrawingXY, &params );
    // Second pass that process the LayerElements (only)
    processLayerElement = true;
    m_currentPage->Process( &setDrawingXY, &params );
    
    // Keep the width of the initial scoreDef
    SetScoreDefDrawingWidth(dc, &m_currentPage->m_drawingScoreDef);
    
    // Set the current score def to the page one
    // The page one has previously been set by Object::SetCurrentScoreDef
    m_drawingScoreDef = m_currentPage->m_drawingScoreDef;

    if ( background ) dc->DrawRectangle(0, 0, m_doc->m_drawingPageWidth, m_doc->m_drawingPageHeight);
    
    dc->DrawBackgroundImage( );
    
    Point origin = dc->GetLogicalOrigin();
    dc->SetLogicalOrigin( origin.x - m_doc->m_drawingPageLeftMar, origin.y - m_doc->m_drawingPageTopMar );

    dc->StartPage();

    for (i = 0; i < m_currentPage->GetSystemCount(); i++) 
	{
        // cast to System check in DrawSystem
		system = dynamic_cast<System*>(m_currentPage->m_children.at(i));
        DrawSystem( dc, system );
    }
    
    dc->EndPage();
}
    
void View::SetScoreDefDrawingWidth(DeviceContext *dc, ScoreDef *scoreDef)
{
    assert(dc);
    assert(scoreDef);
    
    char numAlteration = 0;
    
    // key signature of the scoreDef
    if (scoreDef->HasKeySigInfo()) {
        KeySig *keySig = scoreDef->GetKeySigCopy();
        assert(keySig);
        numAlteration = (keySig->GetAlterationNumber() > numAlteration) ? keySig->GetAlterationNumber() : numAlteration;
        delete keySig;
    }
    
    // longest key signature of the staffDefs
    ListOfObjects* scoreDefList = scoreDef->GetList(scoreDef); //make sure it's initialized
    for (ListOfObjects::iterator it = scoreDefList->begin(); it != scoreDefList->end(); it++) {
        StaffDef *staffDef = dynamic_cast<StaffDef*>(*it);
        assert( staffDef) ;
        if (!staffDef->HasKeySigInfo()) continue;
        KeySig *keySig = staffDef->GetKeySigCopy();
        assert(keySig);
        numAlteration = (keySig->GetAlterationNumber() > numAlteration) ? keySig->GetAlterationNumber() : numAlteration;
        delete keySig;
    }
    
    int width = 0;
    // G-clef as default width
    width += m_doc->GetLeftMargin(CLEF) + m_doc->GetGlyphWidth(SMUFL_E050_gClef, 100, false) + m_doc->GetRightMargin(CLEF);
    if (numAlteration > 0) {
        width += m_doc->GetLeftMargin(KEYSIG) + m_doc->GetGlyphWidth(SMUFL_E262_accidentalSharp, 100, false) * TEMP_STYLE_KEYSIG_STEP + m_doc->GetRightMargin(KEYSIG);
    }
    
    scoreDef->SetDrawingWidth(width);
}

//----------------------------------------------------------------------------
// View - System
//----------------------------------------------------------------------------

void View::DrawSystem( DeviceContext *dc, System *system ) 
{
    assert( dc );
	assert( system );
    
    dc->StartGraphic( system, "", system->GetUuid() );
    
    // first we need to clear the drawing list of postponed elements
    system->ResetDrawingList();

    // First get the first measure of the system
    Measure *measure  = dynamic_cast<Measure*>(system->FindChildByType( MEASURE ) );
    if ( measure ) {
        // NULL for the BarLine parameters indicates that we are drawing the scoreDef
        DrawScoreDef( dc, &m_drawingScoreDef, measure, system->GetDrawingX(), NULL );
        // Draw mesure number if > 1
        // This needs to be improved because we are now using (tuplet) oblique figures.
        // We should also have a better way to specify if the number has to be displayed or not
        if ( (measure->GetN() != VRV_UNSET) && (measure->GetN() > 1) ) {
            dc->SetFont( m_doc->GetDrawingSmuflFont( 100, false) );
            dc->DrawMusicText( IntToTupletFigures( measure->GetN() ) , ToDeviceContextX(system->GetDrawingX()), ToDeviceContextY(system->GetDrawingY() - m_doc->GetSpacingStaff() * m_doc->GetDrawingUnit(100)) );
            dc->ResetFont();
        }
    }
    
    DrawSystemChildren(dc, system, system);
    
    // first draw the beams
    DrawSystemList(dc, system, SYL );
    DrawSystemList(dc, system, TIE );
    DrawSystemList(dc, system, SLUR );
    
    dc->EndGraphic(system, this );

}

void View::DrawSystemList( DeviceContext *dc, System *system, const ClassId classId )
{
    assert( dc );
    assert( system );
    
    ListOfObjects *drawingList = system->GetDrawingList();
    DocObject *element = NULL;
    
    ListOfObjects::iterator iter;
    
    for (iter = drawingList->begin(); iter != drawingList->end(); ++iter)
    {
        // We need to cast to DocObject for calling DrawTimeSpanningElement
        element = dynamic_cast<DocObject*>(*iter);
        if (!element) continue;
        
        if ( (element->Is() == classId) && (classId == SYL) ) {
            DrawTimeSpanningElement( dc, element, system );
        }
        if ( (element->Is() == classId) && (classId == TIE) ) {
            DrawTimeSpanningElement(dc, element, system );
        }
        if ( (element->Is() == classId) && (classId == SLUR) ) {
            DrawTimeSpanningElement(dc, element, system );
        }
    }
}

void View::DrawScoreDef( DeviceContext *dc, ScoreDef *scoreDef, Measure *measure, int x, BarLine *barLine  )
{
    assert( dc );
    assert( scoreDef );
    // we need at least one measure to be able to draw the groups - we need access to the staff elements,
    assert( measure );
    
    StaffGrp *staffGrp = dynamic_cast<StaffGrp*>(scoreDef->FindChildByType( STAFFGRP ) );
    if ( !staffGrp ) {
        return;
    }
    
    if (barLine == NULL) {
        // Draw the first staffGrp and from there its children recursively
        DrawStaffGrp( dc, measure, staffGrp, x, true, !scoreDef->DrawLabels() );
        
        DrawStaffDefLabels( dc, measure, scoreDef, !scoreDef->DrawLabels() );
        // if this was true (non-abbreviated labels), set it to false for next one
        scoreDef->SetDrawLabels( false );
    }
    else {
        barLine->SetDrawingX( x );
        dc->StartGraphic( barLine, "", barLine->GetUuid() );
        DrawBarLines( dc, measure, staffGrp, barLine );
        dc->EndGraphic( barLine, this );
    }
    
	return;
}

void View::DrawStaffGrp( DeviceContext *dc, Measure *measure, StaffGrp *staffGrp, int x, bool topStaffGrp, bool abbreviations )
{
    assert( dc );
    assert( measure );
    assert( staffGrp );
    
    int w, h;
    
    ListOfObjects *staffDefs = staffGrp->GetList( staffGrp );
    if ( staffDefs->empty() ) {
        return;
    }
    
    // Get the first and last staffDef of the staffGrp
    StaffDef *firstDef = dynamic_cast<StaffDef*>(staffDefs->front());
    StaffDef *lastDef = dynamic_cast<StaffDef*>(staffDefs->back());
    
    if (!firstDef || !lastDef ) {
        LogDebug("Could not get staffDef while drawing staffGrp - Vrv::DrawStaffGrp");
        return;
    }
    
    // Get the corresponding staff looking at the previous (or first) measure
    AttCommonNComparison comparisonFirst( STAFF, firstDef->GetN() );
    Staff *first = dynamic_cast<Staff*>(measure->FindChildByAttComparison(&comparisonFirst, 1));
    AttCommonNComparison comparisonLast( STAFF, lastDef->GetN() );
    Staff *last = dynamic_cast<Staff*>(measure->FindChildByAttComparison(&comparisonLast, 1));
    
    if (!first || !last ) {
        LogDebug("Could not get staff (%d; %d) while drawing staffGrp - Vrv::DrawStaffGrp", firstDef->GetN(), lastDef->GetN() );
        return;
    }
    
    
    int y_top = first->GetDrawingY();
    // for the bottom position we need to take into account the number of lines and the staff size
    int y_bottom = last->GetDrawingY() - (lastDef->GetLines() - 1) * m_doc->GetDrawingDoubleUnit(last->m_drawingStaffSize);
    int barLineWidth = m_doc->GetDrawingBarLineWidth(100);
    
    // ajdust the top and bottom according to staffline width
    y_top += m_doc->GetDrawingStaffLineWidth(100) / 2;
    y_bottom -= m_doc->GetDrawingStaffLineWidth(100) / 2;
    
    if (staffGrp->HasLabel()) {
        std::string abbrLabel;
        std::string label = staffGrp->GetLabel();
        if (abbreviations) {
            label = staffGrp->GetLabelAbbr();
        }
        // We still store the abbreviated label for calculating max width with abbreviations (see below)
        else {
            abbrLabel = staffGrp->GetLabelAbbr();
        }
        
        if ( label.length() != 0) {
            // HARDCODED
            int space = 4 * m_doc->GetDrawingBeamWidth(100, false);
            int x_label = x - space;
            int y_label = y_bottom - (y_bottom - y_top) / 2 - m_doc->GetDrawingUnit(100);
        
            dc->SetBrush( m_currentColour, AxSOLID );
            dc->SetFont(m_doc->GetDrawingLyricFont(100));
        
            dc->GetTextExtent(label, &w, &h);
            
            // keep the widest width for the system
            System *system = dynamic_cast<System*>(measure->GetFirstParent( SYSTEM ) );
            if (!system) {
                LogDebug("Staff or System missing in View::DrawStaffDefLabels");
            }
            else {
                system->SetDrawingLabelsWidth( w + space );
            }
        
            dc->StartText( ToDeviceContextX( x_label ), ToDeviceContextY( y_label ), RIGHT );
            dc->DrawText( label );
            dc->EndText( );
            
            // also store in the system the maximum width with abbreviations
            if (!abbreviations && (abbrLabel.length() > 0)) {
                dc->GetTextExtent( abbrLabel, &w, &h);
                system->SetDrawingAbbrLabelsWidth( w + space );
            }
        
            dc->ResetFont();
            dc->ResetBrush();
        }
    }
    
    // actually draw the line, the brace or the bracket
    if ( topStaffGrp && ( (firstDef != lastDef) || (staffGrp->GetSymbol() != staffgroupingsym_SYMBOL_NONE) ) ) {
        DrawVerticalLine( dc , y_top, y_bottom, x, barLineWidth);
    }
    // this will need to be changed with the next version of MEI will line means additional thick line 
    if ( staffGrp->GetSymbol() == staffgroupingsym_SYMBOL_line ) {
        DrawVerticalLine( dc , y_top, y_bottom, x, barLineWidth);
    }
    else if ( staffGrp->GetSymbol() == staffgroupingsym_SYMBOL_brace ) {
        DrawBrace ( dc, x, y_top, y_bottom, last->m_drawingStaffSize );
    }
    else if ( staffGrp->GetSymbol() == staffgroupingsym_SYMBOL_bracket ) {
        DrawBracket( dc, x, y_top, y_bottom, last->m_drawingStaffSize );
        x -= 2 * m_doc->GetDrawingBeamWidth(100, false) - m_doc->GetDrawingBeamWhiteWidth(100, false);
    }
    
    // recursively draw the children
    int i;
    StaffGrp *childStaffGrp = NULL;
    for (i = 0; i < staffGrp->GetChildCount(); i++) {
        childStaffGrp = dynamic_cast<StaffGrp*>(staffGrp->GetChild( i ));
        if ( childStaffGrp ) {
            DrawStaffGrp( dc, measure, childStaffGrp, x, false, abbreviations );
        }
    }
}
    
void View::DrawStaffDefLabels( DeviceContext *dc, Measure *measure, ScoreDef *scoreDef, bool abbreviations )
{
    assert( dc );
    assert( measure );
    assert( scoreDef );
    
    int w, h;
    
    ListOfObjects* scoreDefChildren = scoreDef->GetList(scoreDef);
    ListOfObjects::iterator iter = scoreDefChildren->begin();
    while (iter != scoreDefChildren->end()) {
        StaffDef *staffDef = dynamic_cast<StaffDef*>(*iter);
        
        if (!staffDef) {
            LogDebug("Should be staffDef in View::DrawStaffDefLabels");
            ++iter;
            continue;
        }
        
        AttCommonNComparison comparison( STAFF, staffDef->GetN() );
        Staff *staff = dynamic_cast<Staff*>(measure->FindChildByAttComparison(&comparison, 1 ));
        System *system = dynamic_cast<System*>(measure->GetFirstParent( SYSTEM ) );
        
        if (!staff || !system) {
            LogDebug("Staff or System missing in View::DrawStaffDefLabels");
            ++iter;
            continue;
        }
        
        std::string abbrLabel;
        std::string label = staffDef->GetLabel();
        if (abbreviations) {
            label = staffDef->GetLabelAbbr();
        }
        // We still store the abbreviated label for calculating max width with abbreviations (see below)
        else {
            abbrLabel = staffDef->GetLabelAbbr();
        }
        
        if ( label.length() == 0) {
            ++iter;
            continue;
        }
        
        // HARDCODED
        int space = 3 * m_doc->GetDrawingBeamWidth(100, false);
        int x = system->GetDrawingX() - space;
        int y = staff->GetDrawingY() - (staffDef->GetLines() * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 2);
        
        dc->SetBrush( m_currentColour, AxSOLID );
        dc->SetFont(m_doc->GetDrawingLyricFont(100));
        
        // keep the widest width for the system
        dc->GetTextExtent( label, &w, &h);
        system->SetDrawingLabelsWidth( w + space );
        
        dc->StartText( ToDeviceContextX( x ), ToDeviceContextY( y ), RIGHT );
        dc->DrawText( label );
        dc->EndText( );
        
        // also store in the system the maximum width with abbreviations for justification
        if (!abbreviations && (abbrLabel.length() > 0)) {
            dc->GetTextExtent( abbrLabel, &w, &h);
            system->SetDrawingAbbrLabelsWidth( w + space );
        }
        
        dc->ResetFont();
        dc->ResetBrush();
        
        ++iter;
    }
}
    

void View::DrawBracket ( DeviceContext *dc, int x, int y1, int y2, int staffSize)
{
    assert( dc );
    
    int x1, x2;
    
    x2 = x - m_doc->GetDrawingBeamWidth(100, false);
    x1 = x2 - m_doc->GetDrawingBeamWidth(100, false);
    
    DrawSmuflCode( dc, x1, y1,SMUFL_E003_bracketTop, staffSize, false );
    DrawSmuflCode( dc, x1, y2,SMUFL_E004_bracketBottom, staffSize, false );
    
    // adjust to top and bottom position so we make sure the is not white space between
    // the glyphs and the line
    y1 += m_doc->GetDrawingStemWidth(100);
    y2 -= m_doc->GetDrawingStemWidth(100);
    DrawFullRectangle(dc, x1 , y1, x2, y2 );

	return;
}


void View::DrawBrace ( DeviceContext *dc, int x, int y1, int y2, int staffSize)
{
    assert( dc );
    
    int new_coords[2][6];
    Point points[4];

    int penWidth = m_doc->GetDrawingStemWidth(100);
    y1 -= penWidth;
    y2 += penWidth;
	SwapY( &y1, &y2 );
	
	int ymed, xdec, fact;
    
	x -= m_doc->GetDrawingBeamWhiteWidth(staffSize, false);  // distance entre barre et debut accolade
    
	ymed = (y1 + y2) / 2;
	fact = m_doc->GetDrawingBeamWhiteWidth(staffSize, false) + m_doc->GetDrawingStemWidth(100);
	xdec = ToDeviceContextX(fact);
    
	points[0].x = ToDeviceContextX(x);
	points[0].y = ToDeviceContextY(y1);
	points[1].x = ToDeviceContextX(x - m_doc->GetDrawingDoubleUnit(staffSize) * 2);
	points[1].y = points[0].y - ToDeviceContextX( m_doc->GetDrawingDoubleUnit(staffSize) * 3);
	points[3].x = ToDeviceContextX(x - m_doc->GetDrawingDoubleUnit(staffSize) );
	points[3].y = ToDeviceContextY(ymed);
	points[2].x = ToDeviceContextX(x + m_doc->GetDrawingUnit(staffSize));
	points[2].y = points[3].y + ToDeviceContextX( m_doc->GetDrawingDoubleUnit(staffSize));
    
    new_coords[0][0] = points[1].x;
    new_coords[0][1] = points[1].y;
    new_coords[0][2] = points[2].x;
    new_coords[0][3] = points[2].y;
    new_coords[0][4] = points[3].x;
    new_coords[0][5] = points[3].y;
    
    View::SwapPoints( &points[0], &points[3]);
	View::SwapPoints (&points[1], &points[2]);
	
	points[1].x += xdec;
	points[2].x += xdec;
	points[1].y = points[0].y + ToDeviceContextX( m_doc->GetDrawingDoubleUnit(staffSize) * 2);
    
    new_coords[1][0] = points[1].x;
    new_coords[1][1] = points[1].y;
    new_coords[1][2] = points[2].x;
    new_coords[1][3] = points[2].y;
    new_coords[1][4] = points[3].x;
    new_coords[1][5] = points[3].y;
    
    dc->SetPen( m_currentColour, std::max( 1, penWidth ), AxSOLID );
    dc->SetBrush( m_currentColour , AxSOLID );
    
    dc->DrawComplexBezierPath(ToDeviceContextX(x), ToDeviceContextY(y1), new_coords[0], new_coords[1]);
    
	// on produit l'image reflet vers le bas: 0 est identique 
	points[1].y = points[0].y - ToDeviceContextX( m_doc->GetDrawingDoubleUnit(staffSize)*2);
	points[3].y = ToDeviceContextY(y2);
	points[2].y = points[3].y + ToDeviceContextX( m_doc->GetDrawingDoubleUnit(staffSize)*3);
    
    new_coords[0][0] = points[1].x;
    new_coords[0][1] = points[1].y;
    new_coords[0][2] = points[2].x;
    new_coords[0][3] = points[2].y;
    new_coords[0][4] = points[3].x;
    new_coords[0][5] = points[3].y;
    
	View::SwapPoints (&points[0], &points[3]);
	View::SwapPoints (&points[1], &points[2]);
	
	points[1].x -= xdec;
	points[2].x -= xdec;
	points[2].y = points[3].y - ToDeviceContextX( m_doc->GetDrawingDoubleUnit(staffSize));
    
    new_coords[1][0] = points[1].x;
    new_coords[1][1] = points[1].y;
    new_coords[1][2] = points[2].x;
    new_coords[1][3] = points[2].y;
    new_coords[1][4] = points[3].x;
    new_coords[1][5] = points[3].y;
    
    dc->DrawComplexBezierPath(points[3].x, points[3].y, new_coords[0], new_coords[1]);
    
    dc->ResetPen();
    dc->ResetBrush();
        
	return;
}


void View::DrawBarLines( DeviceContext *dc, Measure *measure, StaffGrp *staffGrp, BarLine *barLine )
{
    assert( dc );
    assert( measure );
    assert( staffGrp );
    assert( barLine );
    
    if (staffGrp->GetBarthru() != BOOLEAN_true) {
        // recursively draw the children (staffDef or staffGrp) - we assume @barthru is false by default
        int i;
        StaffGrp *childStaffGrp = NULL;
        StaffDef *childStaffDef = NULL;
        for (i = 0; i < staffGrp->GetChildCount(); i++) {
            childStaffGrp = dynamic_cast<StaffGrp*>(staffGrp->GetChild( i ));
            childStaffDef = dynamic_cast<StaffDef*>(staffGrp->GetChild( i ));
            if ( childStaffGrp ) {
                DrawBarLines( dc, measure, childStaffGrp, barLine );
            }
            else if ( childStaffDef ) {
                AttCommonNComparison comparison( STAFF, childStaffDef->GetN() );
                Staff *staff = dynamic_cast<Staff*>(measure->FindChildByAttComparison(&comparison, 1 ) );
                if (!staff ) {
                    LogDebug("Could not get staff (%d) while drawing staffGrp - Vrv::DrawBarLines", childStaffDef->GetN() );
                    continue;
                }
                int y_top = staff->GetDrawingY();
                // for the bottom position we need to take into account the number of lines and the staff size
                int y_bottom = staff->GetDrawingY() - (childStaffDef->GetLines() - 1) * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
                DrawBarLine( dc, y_top, y_bottom, barLine );
                if ( barLine->HasRepetitionDots() ) {
                    DrawBarLineDots( dc, childStaffDef, staff, barLine );
                }
            }
        }
    }
    else {
 
        ListOfObjects *staffDefs = staffGrp->GetList( staffGrp );
        if ( staffDefs->empty() ) {
            return;
        }
        
        // Get the first and last staffDef of the staffGrp
        StaffDef *firstDef = dynamic_cast<StaffDef*>(staffDefs->front());
        StaffDef *lastDef = dynamic_cast<StaffDef*>(staffDefs->back());
        
        if (!firstDef || !lastDef ) {
            LogDebug("Could not get staffDef while drawing staffGrp - Vrv::DrawStaffGrp");
            return;
        }
        
        // Get the corresponding staff looking at the previous (or first) measure
        AttCommonNComparison comparisonFirst( STAFF, firstDef->GetN() );
        Staff *first = dynamic_cast<Staff*>(measure->FindChildByAttComparison(&comparisonFirst, 1));
        AttCommonNComparison comparisonLast( STAFF, lastDef->GetN() );
        Staff *last = dynamic_cast<Staff*>(measure->FindChildByAttComparison(&comparisonLast, 1));
        
        if (!first || !last ) {
            LogDebug("Could not get staff (%d; %d) while drawing staffGrp - Vrv::DrawStaffGrp", firstDef->GetN(), lastDef->GetN() );
            return;
        }
        
        int y_top = first->GetDrawingY();
        // for the bottom position we need to take into account the number of lines and the staff size
        int y_bottom = last->GetDrawingY() - (lastDef->GetLines() - 1) * m_doc->GetDrawingDoubleUnit(last->m_drawingStaffSize);
        
        DrawBarLine( dc, y_top, y_bottom, barLine );
        
        // Now we have a barthru barLine, but we have dots so we still need to go through each staff
        if ( barLine->HasRepetitionDots() ) {
            int i;
            StaffDef *childStaffDef = NULL;
            for (i = 0; i < staffGrp->GetChildCount(); i++) {
                childStaffDef = dynamic_cast<StaffDef*>(staffGrp->GetChild( i ));
                if ( childStaffDef ) {
                    AttCommonNComparison comparison( STAFF, childStaffDef->GetN() );
                    Staff *staff = dynamic_cast<Staff*>(measure->FindChildByAttComparison(&comparison, 1));
                    if (!staff ) {
                        LogDebug("Could not get staff (%d) while drawing staffGrp - Vrv::DrawBarLines", childStaffDef->GetN() );
                        continue;
                    }
                    DrawBarLineDots( dc, childStaffDef, staff, barLine );
                }
            }
        }
    }
}

void View::DrawBarLine( DeviceContext *dc, int y_top, int y_bottom, BarLine *barLine )
{
    assert( dc );
    assert( barLine );
    
    // adjust the top and bottom
    y_top += m_doc->GetDrawingStaffLineWidth(100) / 2;
    y_bottom -= m_doc->GetDrawingStaffLineWidth(100) / 2;

    int x = barLine->GetDrawingX();
    int barLineWidth = m_doc->GetDrawingBarLineWidth(100);
	int x1 = x - m_doc->GetDrawingBeamWidth(100, false) - barLineWidth;
	int x2 = x + m_doc->GetDrawingBeamWidth(100, false) + barLineWidth;
    
	if (barLine->GetForm() == BARRENDITION_single)
    {
        DrawVerticalLine( dc , y_top, y_bottom, x, barLineWidth);
    }
    else if (barLine->GetForm() == BARRENDITION_rptboth)
    {
        DrawVerticalLine( dc , y_top, y_bottom, x1, barLineWidth);
        DrawVerticalLine( dc , y_top, y_bottom, x, m_doc->GetDrawingBeamWidth(100, false));
        DrawVerticalLine( dc , y_top, y_bottom, x2, barLineWidth);
    }
    else if (barLine->GetForm()  == BARRENDITION_rptstart)
    {
        DrawVerticalLine( dc , y_top, y_bottom, x, m_doc->GetDrawingBeamWidth(100, false));
        DrawVerticalLine( dc , y_top, y_bottom, x2, barLineWidth);
    }
    else if (barLine->GetForm() == BARRENDITION_rptend)
	{
        DrawVerticalLine( dc , y_top, y_bottom, x1, barLineWidth);
        DrawVerticalLine( dc , y_top, y_bottom, x, m_doc->GetDrawingBeamWidth(100, false));
	}
	else if (barLine->GetForm()  == BARRENDITION_dbl)
	{
        // Narrow the bars a little bit - should be centered?
        x1 += barLineWidth;
        DrawVerticalLine( dc , y_top, y_bottom, x, barLineWidth);
        DrawVerticalLine( dc , y_top, y_bottom, x1, barLineWidth);
	}
	else if (barLine->GetForm()  == BARRENDITION_end)
    {
        DrawVerticalLine( dc , y_top, y_bottom, x1, barLineWidth);
        DrawVerticalLine( dc , y_top, y_bottom, x, m_doc->GetDrawingBeamWidth(100, false));
    }
    else {
        barLine->SetEmptyBB();
    }
}

 
void View::DrawBarLineDots ( DeviceContext *dc, StaffDef *staffDef, Staff *staff, BarLine *barLine )
{
    assert( dc );
    assert( staffDef );
    assert( staff );
    assert( barLine );
    
    int x = barLine->GetDrawingX();
	int x1 = x - 2 * m_doc->GetDrawingBeamWidth(staff->m_drawingStaffSize, false) - m_doc->GetDrawingBarLineWidth(staff->m_drawingStaffSize);
	int x2 = x + 2 * m_doc->GetDrawingBeamWidth(staff->m_drawingStaffSize, false) + m_doc->GetDrawingBarLineWidth(staff->m_drawingStaffSize);
    
    int y_bottom = staff->GetDrawingY() - staffDef->GetLines()  * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    int y_top = y_bottom + m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
 
    if ((barLine->GetForm()  == BARRENDITION_rptstart) || (barLine->GetForm() == BARRENDITION_rptboth))
    {
        DrawDot(dc, x2, y_bottom, staff->m_drawingStaffSize);
        DrawDot(dc, x2, y_top, staff->m_drawingStaffSize);

    }
    if ((barLine->GetForm() == BARRENDITION_rptend) || (barLine->GetForm() == BARRENDITION_rptboth))
	{
        DrawDot(dc, x1, y_bottom, staff->m_drawingStaffSize);
        DrawDot(dc, x1, y_top, staff->m_drawingStaffSize);
	}
    
	return;
}

//----------------------------------------------------------------------------
// View - Measure
//----------------------------------------------------------------------------

void View::DrawMeasure( DeviceContext *dc, Measure *measure, System *system )
{
	assert( dc );
    assert( measure );
    assert( system );
    
    // This is a special case where we do not draw (SVG, Bounding boxes, etc.) the measure if un-measured music
    if ( measure->IsMeasuredMusic()) {
        dc->StartGraphic( measure, "", measure->GetUuid() );
    }

    DrawMeasureChildren(dc, measure, measure, system);

    if ( measure->GetLeftBarLineType() != BARRENDITION_NONE) {
        DrawScoreDef( dc, &m_drawingScoreDef, measure, measure->GetDrawingX(), measure->GetLeftBarLine() );
    }
    if ( measure->GetRightBarLineType() != BARRENDITION_NONE) {
        DrawScoreDef( dc, &m_drawingScoreDef, measure, measure->GetDrawingX() + measure->GetRightBarLineX(), measure->GetRightBarLine() );
    }
    
    if ( measure->IsMeasuredMusic()) {
        dc->EndGraphic( measure, this );
    }
}

    
//----------------------------------------------------------------------------
// View - Staff
//----------------------------------------------------------------------------

int View::CalculatePitchPosY ( Staff *staff, data_PITCHNAME pname, int dec_clef, int oct)
{
    assert(staff); // Pointer to staff cannot be NULL"
	
    static char touches[] = {PITCHNAME_c,PITCHNAME_d,PITCHNAME_e,PITCHNAME_f,PITCHNAME_g,PITCHNAME_a,PITCHNAME_b};
	int y_int;
	char *ptouche, i;
	ptouche=&touches[0];

    // Old Wolfgang code with octave stored in an unsigned char - this could be refactored
    oct -= OCTAVE_OFFSET;
	y_int = ((dec_clef + oct*7) - 9 ) * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    if (staff->m_drawingLines > 5) {
		y_int -= ((staff->m_drawingLines - 5) * 2) * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    }

	/* exprime distance separant m_drawingY de
	position 1e Si, corrigee par dec_clef et oct. Elle est additionnee
	ensuite, donc elle doit etre NEGATIVE si plus bas que m_drawingY */
    for (i=0; i<(signed)sizeof(touches); i++) {
		if (*(ptouche+i) == pname) return (y_int += ((i+1)*m_doc->GetDrawingUnit(staff->m_drawingStaffSize)));
    }
	return 0;
}

int View::CalculateRestPosY ( Staff *staff, char duration)
{
    assert(staff); // Pointer to staff cannot be NULL"

	int staff_space = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    int base = -17 * staff_space; // -17 is a magic number copied from above
    int offset;
    
    switch (duration) {
        case DUR_LG: offset = 12; break;
        case DUR_BR: offset = 13; break;
        case DUR_1: offset = 15; break;
        case DUR_2: offset = 13; break;
        case DUR_4: offset = 11; break;
        case DUR_8: offset = 11; break;
        case DUR_16: offset = 11; break;
        case DUR_32: offset = 11; break;
        case DUR_64: offset = 10; break;
        case DUR_128: offset = 10; break;
        case DUR_256: offset = 9; break;
        default: offset = 12; break; // Signal an error, put the clef up high
    }
    return base + staff_space * offset;
}


void View::DrawStaff( DeviceContext *dc, Staff *staff, Measure *measure, System *system )
{
	assert( dc );
    assert( staff );
    assert( measure );
    assert( system );
    
    dc->StartGraphic( staff, "", staff->GetUuid());
    
    // Doing it here might be problematic with cross-staff, even though the default value will be 5
    if ( StaffDef *staffDef = m_drawingScoreDef.GetStaffDef( staff->GetN() ) ) {
        staff->m_drawingLines = staffDef->GetLines( ) ;
        if (staffDef->HasScale()) {
            staff->m_drawingStaffSize = staffDef->GetScale();
        }
    }
    
    DrawStaffLines( dc, staff, measure, system );
    
    DrawStaffChildren(dc, staff, staff, measure);
    
    std::vector<DocObject*>::iterator iter;
    for (iter = staff->m_timeSpanningElements.begin(); iter != staff->m_timeSpanningElements.end(); ++iter) {
        system->AddToDrawingList(*iter);
    }
    
    dc->EndGraphic( staff, this );
}
    
    
/* Linewidth for staff lines in mensural notation, relative to the "normal" width of staff lines */
#define MENSURAL_LINEWIDTH_FACTOR 0.5

void View::DrawStaffLines( DeviceContext *dc, Staff *staff, Measure *measure, System *system )
{
    assert( dc );
    assert( staff );
    assert( measure );
    assert( system );
    
    if (staff->invisible)
        return;
    
    int j, x1, x2, yy;
    
    yy = staff->GetDrawingY();
    
    //x1 = system->m_systemLeftMar;
    //x2 = m_doc->m_drawingPageWidth - m_doc->m_drawingPageLeftMar - m_doc->m_drawingPageRightMar - system->m_systemRightMar;
    
    x1 = measure->GetDrawingX();
    x2 = x1 + measure->GetWidth();
    
    //dc->SetPen( m_currentColour, ToDeviceContextX( m_doc->m_style->m_staffLineWidth ), AxSOLID );
    int lineWidth = (int)(m_doc->GetDrawingStaffLineWidth(staff->m_drawingStaffSize));
    if (true) lineWidth = lineWidth * MENSURAL_LINEWIDTH_FACTOR;            // ??DON'T DO IF NOT MENSURAL NOTATION!
    dc->SetPen( m_currentColour, ToDeviceContextX( lineWidth ), AxSOLID );
    //dc->SetPen( m_currentColour, ToDeviceContextX( m_doc->GetDrawingStaffLineWidth(staff->m_drawingStaffSize) ), AxSOLID );
    dc->SetBrush( m_currentColour , AxSOLID );
    
    for(j = 0;j < staff->m_drawingLines; j++)
    {
        dc->DrawLine( ToDeviceContextX (x1) , ToDeviceContextY ( yy ) , ToDeviceContextX (x2) , ToDeviceContextY ( yy ) );
        // For drawing rectangles insteam of line
        yy -= m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
    }
    
    staff->m_drawingHeight = staff->GetDrawingY() - yy;
    
    dc->ResetPen( );
    dc->ResetBrush( );
    
    return;
}

//----------------------------------------------------------------------------
// View - Layer
//----------------------------------------------------------------------------


// a partir d'un y, trouve la hauteur d'une note exprimee en code touche et
// octave. Retourne code clavier, et situe l'octave. 

int View::CalculatePitchCode ( Layer *layer, int y_n, int x_pos, int *octave )
{
    assert( layer );
    assert( octave );
    
    Staff *parentStaff = dynamic_cast<Staff*>(layer->GetFirstParent( STAFF ) );
    assert( parentStaff ); // Pointer to parent has to be a staff
	
    static int touches[] = {PITCHNAME_c,PITCHNAME_d,PITCHNAME_e,PITCHNAME_f,PITCHNAME_g,PITCHNAME_a,PITCHNAME_b};
	int y_dec, yb, plafond;
	int degres, octaves, position, code;
	char clefId=0;

    int staffSize = parentStaff->m_drawingStaffSize;
	// calculer position du do central en fonction clef
	//y_n += (int) m_doc->GetDrawingUnit(staffSize)/4;
	yb = parentStaff->GetDrawingY() -  m_doc->GetDrawingStaffSize(staffSize); // UT1 default
	

	plafond = yb + 8 *  m_doc->GetDrawingOctaveSize(staffSize);
	if (y_n > plafond)
		y_n = plafond;

    LayerElement *previous = NULL;
	LayerElement *pelement = layer->GetAtPos( x_pos );
	if ( (previous = layer->GetPrevious( pelement ) ) )
		pelement = previous;

	Clef *clef = layer->GetClef (pelement);
    if (clef) {
        clefId = clef->GetClefId();
        yb += (clef->GetClefOffset()) * m_doc->GetDrawingUnit(staffSize);	// UT1 reel
    }
	yb -= 4 *  m_doc->GetDrawingOctaveSize(staffSize);	// UT, note la plus grave

	y_dec = y_n - yb;	// decalage par rapport a UT le plus grave

	if (y_dec< 0)
		y_dec = 0;

	degres = y_dec /  m_doc->GetDrawingUnit(staffSize);	// ecart en degres (PITCHNAME_c..PITCHNAME_b) par rapport a UT1
	octaves = degres / 7;
	position = degres % 7;

	code = touches[position];
	*octave = octaves /*- OCTBIT*/; // LP remove OCTBIT : oct 0 ‡ 7

	return (code);
}

void View::DrawLayer( DeviceContext *dc, Layer *layer, Staff *staff, Measure *measure)
{
	assert( dc );
    assert( layer );
    assert( staff );
    assert( measure );
    
    dc->StartGraphic( layer, "", layer->GetUuid());
    
    // first we need to clear the drawing list of postponed elements
    layer->ResetDrawingList();
    
    // the draw the scoreDef when required
    if (layer->GetDrawingClef()) {
        DrawLayerElement(dc, layer->GetDrawingClef(), layer, staff, measure);
    }
    if (layer->GetDrawingKeySig()) {
        DrawLayerElement(dc, layer->GetDrawingKeySig(), layer, staff, measure);
    }
    if (layer->GetDrawingMensur()) {
        DrawLayerElement(dc, layer->GetDrawingMensur(), layer, staff, measure);
    }
    if (layer->GetDrawingMeterSig()) {
        DrawLayerElement(dc, layer->GetDrawingMeterSig(), layer, staff, measure);
    }

    DrawLayerChildren(dc, layer, layer, staff, measure);
    
    // first draw the postponed tuplets
    DrawLayerList(dc, layer, staff, measure, TUPLET );
    
    dc->EndGraphic( layer, this );
}


void View::DrawLayerList( DeviceContext *dc, Layer *layer, Staff *staff, Measure *measure, const ClassId classId )
{
	assert( dc );
    assert( layer );
    assert( staff );
    assert( measure );
    
    ListOfObjects *drawingList = layer->GetDrawingList();
    ListOfObjects::iterator iter;
    
    for (iter = drawingList->begin(); iter != drawingList->end(); ++iter)
    {
        if ( ((*iter)->Is() == classId) &&  (classId == TUPLET) ) {
            Tuplet *tuplet = dynamic_cast<Tuplet*>((*iter));
            assert( tuplet );
            dc->ResumeGraphic(tuplet, tuplet->GetUuid());
            DrawTupletPostponed( dc, tuplet, layer, staff );
            dc->EndResumedGraphic(tuplet, this);
        }
        else {
            // This should never happen
            LogError("Element '%s' in the layer list cannot be drawn", (*iter)->GetClassName().c_str());
        }
    }
}
    
//----------------------------------------------------------------------------
// View - Children
//----------------------------------------------------------------------------

void View::DrawSystemChildren( DeviceContext *dc, Object *parent, System *system )
{
    assert( dc );
    assert( parent );
    assert( system );
    
    Object* current;
    for (current = parent->GetFirst( ); current; current = parent->GetNext( ) )
    {
        if (current->Is() == MEASURE) {
            // cast to Measure check in DrawMeasure
            DrawMeasure( dc , dynamic_cast<Measure*>(current), system );
        }
        else if (current->IsEditorialElement()) {
            // cast to EditorialElement check in DrawSystemEditorial element
            DrawSystemEditorialElement( dc , dynamic_cast<EditorialElement*>(current), system );
        }
        // scoreDef are not drawn directly, but anything else should not be possible
        else if (current->Is() == SCOREDEF) {
            // nothing to do, then
            ScoreDef *scoreDef = dynamic_cast<ScoreDef*>(current);
            assert( scoreDef );
            SetScoreDefDrawingWidth(dc, scoreDef);
        }
        else {
            assert(false);
        }
    }
}

void View::DrawMeasureChildren( DeviceContext *dc, Object *parent, Measure *measure, System *system )
{
    assert( dc );
    assert( parent );
    assert( measure );
    assert( system );
    
    Object* current;
    for (current = parent->GetFirst( ); current; current = parent->GetNext( ) )
    {
        if (current->Is() == STAFF) {
            // cast to Staff check in DrawStaff
            DrawStaff(dc,  dynamic_cast<Staff*>(current), measure, system);
        }
        else if (current->IsFloatingElement()) {
            // cast to FloatingElement check in DrawFloatingElement
            DrawFloatingElement( dc , dynamic_cast<FloatingElement*>(current), measure, system );
        }
        else if (current->IsEditorialElement()) {
            // cast to EditorialElement check in DrawMeasureEditorialElement
            DrawMeasureEditorialElement( dc , dynamic_cast<EditorialElement*>(current), measure, system );
        }
        else {
            LogDebug("Current is %s", current->GetClassName().c_str() );
            assert(false);
        }
    }
}

void View::DrawStaffChildren( DeviceContext *dc, Object *parent, Staff *staff,  Measure *measure )
{
    assert( dc );
    assert( parent );
    assert( staff );
    assert( measure );
    
    Object* current;
    for (current = parent->GetFirst( ); current; current = parent->GetNext( ) )
    {
        if (current->Is() == LAYER) {
            // cast to Layer check in DrawLayer
            DrawLayer( dc , dynamic_cast<Layer*>(current), staff, measure );
        }
        else if (current->IsEditorialElement()) {
            // cast to EditorialElement check in DrawStaffEditorialElement
            DrawStaffEditorialElement( dc , dynamic_cast<EditorialElement*>(current), staff, measure );
        }
        else {
            assert(false);
        }
    }
}

void View::DrawLayerChildren( DeviceContext *dc, Object *parent, Layer *layer, Staff *staff, Measure *measure )
{
    assert( dc );
    assert( parent );
    assert( layer );
    assert( staff );
    assert( measure );
    
    Object* current;
    for (current = parent->GetFirst( ); current; current = parent->GetNext( ) )
    {
        if (current->IsLayerElement()) {
            DrawLayerElement( dc, dynamic_cast<LayerElement*>(current), layer, staff, measure );
        }
        else if (current->IsEditorialElement()) {
            // cast to EditorialElement check in DrawLayerEditorialElement
            DrawLayerEditorialElement( dc , dynamic_cast<EditorialElement*>(current), layer, staff, measure );
        }
        else {
            assert(false);
        }
    }
}
    
void View::DrawTextChildren( DeviceContext *dc, Object *parent, int x, int y, bool &setX, bool &setY  )
{
    assert( dc );
    assert( parent );
    
    Object* current;
    for (current = parent->GetFirst( ); current; current = parent->GetNext( ) )
    {
        if (current->IsTextElement()) {
            DrawTextElement( dc, dynamic_cast<TextElement*>(current), x, y, setX, setY );
        }
        else if (current->IsEditorialElement()) {
            // cast to EditorialElement check in DrawLayerEditorialElement
            DrawTextEditorialElement( dc , dynamic_cast<EditorialElement*>(current), x, y, setX, setY);
        }
        else {
            assert(false);
        }
    }
}    

//----------------------------------------------------------------------------
// View - Editorial
//----------------------------------------------------------------------------

void View::DrawSystemEditorialElement( DeviceContext *dc, EditorialElement *element, System *system )
{
    assert( element );
    if ( element->Is() == APP ) {
        assert( dynamic_cast<App*>(element)->GetLevel() == EDITORIAL_SYSTEM );
    }
    
    dc->StartGraphic( element, "", element->GetUuid());
    if (element->m_visibility == Visible) {
        DrawSystemChildren( dc, element, system);
    }
    dc->EndGraphic( element, this );
}

void View::DrawMeasureEditorialElement( DeviceContext *dc, EditorialElement *element, Measure *measure, System *system )
{
    assert( element );
    if ( element->Is() == APP ) {
        assert( dynamic_cast<App*>(element)->GetLevel() == EDITORIAL_MEASURE );
    }
    
    dc->StartGraphic( element, "", element->GetUuid());
    if (element->m_visibility == Visible) {
        DrawMeasureChildren(dc, element, measure, system);
    }
    dc->EndGraphic( element, this );
}
    
void View::DrawStaffEditorialElement( DeviceContext *dc, EditorialElement *element, Staff *staff,  Measure *measure )
{
    assert( element );
    if ( element->Is() == APP ) {
        assert( dynamic_cast<App*>(element)->GetLevel() == EDITORIAL_STAFF );
    }
    
    dc->StartGraphic( element, "", element->GetUuid());
    if (element->m_visibility == Visible) {
        DrawStaffChildren(dc, element, staff, measure);
    }
    dc->EndGraphic( element, this );
}

void View::DrawLayerEditorialElement( DeviceContext *dc, EditorialElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert( element );
    if ( element->Is() == APP ) {
        assert( dynamic_cast<App*>(element)->GetLevel() == EDITORIAL_LAYER );
    }
    
    dc->StartGraphic( element, "", element->GetUuid());
    if (element->m_visibility == Visible) {
        DrawLayerChildren(dc, element, layer, staff, measure);
    }
    dc->EndGraphic( element, this );
}
    
void View::DrawTextEditorialElement( DeviceContext *dc, EditorialElement *element, int x, int y, bool &setX, bool &setY  )
{
    assert( element );
    if ( element->Is() == APP ) {
        assert( dynamic_cast<App*>(element)->GetLevel() == EDITORIAL_TEXT );
    }
    
    dc->StartTextGraphic( element, "", element->GetUuid());
    if (element->m_visibility == Visible) {
        DrawTextChildren(dc, element, x, y, setX, setY);
    }
    dc->EndTextGraphic( element, this );
}
    
} // namespace vrv




