/////////////////////////////////////////////////////////////////////////////
// Name:        view_page.cpp
// Author:      Laurent Pugin and Chris Niven
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "view.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>
#include <typeinfo>

//----------------------------------------------------------------------------

#include "att_comparison.h"
#include "beam.h"
#include "barline.h"
#include "clef.h"
#include "doc.h"
#include "editorial.h"
#include "keysig.h"
#include "layer.h"
#include "layerelement.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "page.h"
#include "staff.h"
#include "system.h"
#include "slur.h"
#include "tie.h"
#include "tuplet.h"

namespace vrv {

//----------------------------------------------------------------------------
// View - Page
//----------------------------------------------------------------------------

void View::DrawCurrentPage( DeviceContext *dc, bool background )
{
	assert( dc ); // DC cannot be NULL
    assert( m_doc );
    
    m_currentPage = m_doc->SetDrawingPage( m_pageIdx );
    
    int i;
	System *system = NULL;
    
    // Set the current score def to the page one
    // The page one has previously been set by Object::SetCurrentScoreDef
    m_drawingScoreDef = m_currentPage->m_drawingScoreDef;

    if ( background )
        dc->DrawRectangle( 0, 0, m_doc->m_drawingPageWidth, m_doc->m_drawingPageHeight );
    
    dc->DrawBackgroundImage( );
    
    MusPoint origin = dc->GetLogicalOrigin();
    dc->SetLogicalOrigin( origin.x - m_doc->m_drawingPageLeftMar, origin.y - m_doc->m_drawingPageTopMar );

    dc->StartPage();

    for (i = 0; i < m_currentPage->GetSystemCount(); i++) 
	{
		system = dynamic_cast<System*>(m_currentPage->m_children[i]);
        DrawSystem( dc, system );
        
        // TODO here: also update x_abs and m_drawingY positions for system. How to calculate them?
    }
    
    dc->EndPage();
}

//----------------------------------------------------------------------------
// View - System
//----------------------------------------------------------------------------


// drawing

void View::DrawSystem( DeviceContext *dc, System *system ) 
{
	assert( system ); // other asserted before
    
    Measure *measure = NULL;
    ScoreDef *scoreDef = NULL;
    EditorialElement *editorialElement = NULL;
    
    dc->StartGraphic( system, "", system->GetUuid() );
    
    
    if ( system->m_yAbs == VRV_UNSET ) {
        assert( m_doc->GetType() == Raw );
        system->SetDrawingX( system->m_drawingXRel );
        system->SetDrawingY( system->m_drawingYRel );
    }
    else
    {
        assert( m_doc->GetType() == Transcription );
        system->SetDrawingX( system->m_xAbs );
        system->SetDrawingY( system->m_yAbs );
    }

    Object* current;
    for (current = system->GetFirst( ); current; current = system->GetNext( ) )
	{
		measure = dynamic_cast<Measure*>(current);
        scoreDef = dynamic_cast<ScoreDef*>(current);
        editorialElement = dynamic_cast<EditorialElement*>(current);
        if (measure) {
            DrawMeasure( dc , measure, system );
        }
        else if (editorialElement) {
            DrawEditorialElement( dc , editorialElement, system );
        }
        // scoreDef are not drawn directly, but anything else should not be possible
        else if (scoreDef) {
            m_drawingScoreDef.Replace(scoreDef);
        }
        else {
            assert(false);
        }
	}

    // We draw the groups after the staves because we use the m_drawingY member of the staves
    // that needs to be intialized.
    // Warning: we assume for now the scoreDef occuring in the system will not change the staffGrps content
    // and @symbol values, otherwise results will be unexpected...
    // First get the first measure of the system
    measure  = dynamic_cast<Measure*>(system->FindChildByType( &typeid(Measure) ) );
    if ( measure ) {
        // NULL for the Barline parameters indicates that we are drawing the scoreDef
        DrawScoreDef( dc, &m_drawingScoreDef, measure, system->GetDrawingX(), NULL );
        // Draw mesure number if > 1
        // This needs to be improved because we are now using (tuplet) oblique figures.
        // We should also have a better way to specify if the number has to be displayed or not
        if ( (measure->GetN() != VRV_UNSET) && (measure->GetN() > 1) ) {
            dc->DrawMusicText( IntToTupletFigures( measure->GetN() ) , ToDeviceContextX(system->GetDrawingX()), ToDeviceContextY(system->GetDrawingY() - m_doc->m_drawingStaffSize[0]  * 2 / 3) );
        }
    }
    
    dc->EndGraphic(system, this );

}

void View::DrawScoreDef( DeviceContext *dc, ScoreDef *scoreDef, Measure *measure, int x, Barline *barLine  )
{
    assert( scoreDef ); // other asserted before

    // we need at least one measure to be able to draw the groups - we need access to the staff elements,
    assert( measure );
    
    StaffGrp *staffGrp = dynamic_cast<StaffGrp*>(scoreDef->FindChildByType( &typeid(StaffGrp) ) );
    if ( !staffGrp ) {
        return;
    }
    
    if ( barLine == NULL) {
        // Draw the first staffGrp and from there its children recursively
        DrawStaffGrp( dc, measure, staffGrp, x );
        
        DrawStaffDefLabels( dc, measure, scoreDef, !scoreDef->DrawLabels() );
        // if this was true (non-abbreviated labels), set it to false for next one
        scoreDef->SetDrawLabels( false );
    }
    else{
        barLine->SetDrawingX( x );
        dc->StartGraphic( barLine, "", barLine->GetUuid() );
        DrawBarlines( dc, measure, staffGrp, barLine );
        dc->EndGraphic( barLine, this );
    }
    
	return;
}

void View::DrawStaffGrp( DeviceContext *dc, Measure *measure, StaffGrp *staffGrp, int x )
{
    assert( measure );
    assert( staffGrp );
    
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
    AttCommonNComparison comparisonFirst( &typeid(Staff), firstDef->GetN() );
    Staff *first = dynamic_cast<Staff*>(measure->FindChildByAttComparison(&comparisonFirst));
    AttCommonNComparison comparisonLast( &typeid(Staff), lastDef->GetN() );
    Staff *last = dynamic_cast<Staff*>(measure->FindChildByAttComparison(&comparisonLast, true));
    
    if (!first || !last ) {
        LogDebug("Could not get staff (%d; %d) while drawing staffGrp - Vrv::DrawStaffGrp", firstDef->GetN(), lastDef->GetN() );
        return;
    }
    
    int y_top = first->GetDrawingY();
    // for the bottom position we need to take into account the number of lines and the staff size
    int y_bottom = last->GetDrawingY() - (lastDef->GetLines() - 1) * m_doc->m_drawingInterl[last->staffSize];
    
    // ajdust the top and bottom according to staffline width
    y_top += m_doc->m_env.m_staffLineWidth / 2;
    y_bottom -= m_doc->m_env.m_staffLineWidth / 2;
    
    // actually draw the line, the brace or the bracket
    if ( staffGrp->GetSymbol() == STAFFGRP_LINE ) {
        DrawVerticalLine( dc , y_top, y_bottom, x, m_doc->m_env.m_barlineWidth );
    }
    else if ( staffGrp->GetSymbol() == STAFFGRP_BRACE ) {
        DrawBrace ( dc, x, y_top, y_bottom, last->staffSize );
    }
    else if ( staffGrp->GetSymbol() == STAFFGRP_BRACKET ) {
        DrawBracket( dc, x, y_top, y_bottom, last->staffSize );
        x -= 2 * m_doc->m_drawingBeamWidth[0] - m_doc->m_drawingBeamWhiteWidth[0];
    }
    
    // recursively draw the children
    int i;
    StaffGrp *childStaffGrp = NULL;
    for (i = 0; i < staffGrp->GetChildCount(); i++) {
        childStaffGrp = dynamic_cast<StaffGrp*>(staffGrp->GetChild( i ));
        if ( childStaffGrp ) {
            DrawStaffGrp( dc, measure, childStaffGrp, x );
        }
    }
}

    
void View::DrawStaffDefLabels( DeviceContext *dc, Measure *measure, ScoreDef *scoreDef, bool abbreviations )
{
    assert( measure );
    assert( scoreDef );
    
    ListOfObjects::iterator iter = scoreDef->m_list.begin();
    while (iter != scoreDef->m_list.end()) {
        StaffDef *staffDef = dynamic_cast<StaffDef*>(*iter);
        
        if (!staffDef) {
            LogDebug("Should be staffDef in View::DrawStaffDefLabels");
            ++iter;
            continue;
        }
        
        AttCommonNComparison comparison( &typeid(Staff), staffDef->GetN() );
        Staff *staff = dynamic_cast<Staff*>(measure->FindChildByAttComparison(&comparison));
        System *system = dynamic_cast<System*>(measure->GetFirstParent( &typeid(System) ) );
        
        if (!staff || !system) {
            LogDebug("Staff or System missing in View::DrawStaffDefLabels");
            ++iter;
            continue;
        }
        
        std::string label = staffDef->GetLabel();
        if ( abbreviations ) {
            label = staffDef->GetLabelAbbr();
        }
        
        if ( label.length() == 0) {
            ++iter;
            continue;
        }
        
        // keep the widest width for the system; the 1.1 is an arbitrary avg value of each letter with
        system->SetDrawingLabelsWidth( label.length() * m_doc->m_drawingInterl[staff->staffSize] * 1.1 );
        
        int x = system->GetDrawingX() - 3 * m_doc->m_drawingBeamWidth[0];
        int y = staff->GetDrawingY() - (staffDef->GetLines() * m_doc->m_drawingInterl[staff->staffSize] / 2);
        dc->DrawText( label, ToDeviceContextX( x ), ToDeviceContextY( y ), RIGHT );
        
        ++iter;
    }
}
    

void View::DrawBracket ( DeviceContext *dc, int x, int y1, int y2, int staffSize)
{
	int xg, xd, yg, yd, ecart, centre;

    dc->SetPen( m_currentColour , 2, AxSOLID );
    dc->SetBrush( m_currentColour , AxTRANSPARENT );
    
    ecart = m_doc->m_drawingBeamWidth[0] + m_doc->m_drawingBeamWhiteWidth[0];
    centre = x - ecart;
    
    xg = centre - ecart*2;
    xd = centre + ecart*2;
    
    yg = y1 + m_doc->m_drawingInterl[ staffSize ] * 2;
    yd = y1;
    SwapY( &yg, &yd );
    dc->DrawEllipticArc( ToDeviceContextX(xg), ToDeviceContextY(yg), ToDeviceContextX(xd-xg), ToDeviceContextX(yg-yd), 90, 40 );
    
    yg = y2;
    yd = y2 - m_doc->m_drawingInterl[ staffSize ] * 2;
    SwapY( &yg, &yd );
    dc->DrawEllipticArc( ToDeviceContextX(xg), ToDeviceContextY(yg), ToDeviceContextX(xd-xg), ToDeviceContextX(yg-yd), 320, 271 );
    
    dc->ResetPen();
    dc->ResetBrush();
    
    xg = x - (m_doc->m_drawingBeamWhiteWidth[0]);
    // determine le blanc entre barres grosse et mince
    DrawFullRectangle(dc, xg -  m_doc->m_drawingBeamWidth[0] , y1, xg, y2 );

	return;
}


void View::DrawBrace ( DeviceContext *dc, int x, int y1, int y2, int staffSize)
{	
    int new_coords[2][6];
    MusPoint points[4];
    
	assert( dc ); // DC cannot be NULL

	SwapY( &y1, &y2 );
	
	int ymed, xdec, fact;
    
    dc->SetPen( m_currentColour , 1, AxSOLID );
    dc->SetBrush( m_currentColour , AxSOLID );
    
	x -= m_doc->m_drawingBeamWhiteWidth[ staffSize ];  // distance entre barre et debut accolade
    
	ymed = (y1 + y2) / 2;
	fact = m_doc->m_drawingBeamWidth[ staffSize ]-1 + m_doc->m_env.m_barlineWidth;
	xdec = ToDeviceContextX(fact);
    
	points[0].x = ToDeviceContextX(x);
	points[0].y = ToDeviceContextY(y1);
	points[1].x = ToDeviceContextX(x - m_doc->m_drawingInterl[ staffSize ] );
	points[1].y = points[0].y - ToDeviceContextX( m_doc->m_drawingInterl[ staffSize ]*3);
	points[3].x = ToDeviceContextX(x - m_doc->m_drawingInterl[ staffSize ] );
	points[3].y = ToDeviceContextY(ymed);
	points[2].x = ToDeviceContextX(x + m_doc->m_drawingUnit);
	points[2].y = points[3].y + ToDeviceContextX( m_doc->m_drawingInterl[ staffSize ]);
    
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
	points[1].y = points[0].y + ToDeviceContextX( m_doc->m_drawingInterl[ staffSize ]*2);
    
    new_coords[1][0] = points[1].x;
    new_coords[1][1] = points[1].y;
    new_coords[1][2] = points[2].x;
    new_coords[1][3] = points[2].y;
    new_coords[1][4] = points[3].x;
    new_coords[1][5] = points[3].y;
    
    dc->DrawComplexBezierPath(ToDeviceContextX(x), ToDeviceContextY(y1), new_coords[0], new_coords[1]);
    
	// on produit l'image reflet vers le bas: 0 est identique 
	points[1].y = points[0].y - ToDeviceContextX( m_doc->m_drawingInterl[ staffSize ]*2);
	points[3].y = ToDeviceContextY(y2);
	points[2].y = points[3].y + ToDeviceContextX( m_doc->m_drawingInterl[ staffSize ]*3);
    
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
	points[2].y = points[3].y - ToDeviceContextX( m_doc->m_drawingInterl[ staffSize ]);
    
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


void View::DrawBarlines( DeviceContext *dc, Measure *measure, StaffGrp *staffGrp, Barline *barLine )
{
    assert( measure );
    assert( staffGrp );
    
    if ( !staffGrp->GetBarthru() ) {
        // recursively draw the children (staffDef or staffGrp)
        int i;
        StaffGrp *childStaffGrp = NULL;
        StaffDef *childStaffDef = NULL;
        for (i = 0; i < staffGrp->GetChildCount(); i++) {
            childStaffGrp = dynamic_cast<StaffGrp*>(staffGrp->GetChild( i ));
            childStaffDef = dynamic_cast<StaffDef*>(staffGrp->GetChild( i ));
            if ( childStaffGrp ) {
                DrawBarlines( dc, measure, childStaffGrp, barLine );
            }
            else if ( childStaffDef ) {
                AttCommonNComparison comparison( &typeid(Staff), childStaffDef->GetN() );
                Staff *staff = dynamic_cast<Staff*>(measure->FindChildByAttComparison(&comparison));
                if (!staff ) {
                    LogDebug("Could not get staff (%d) while drawing staffGrp - Vrv::DrawBarlines", childStaffDef->GetN() );
                    continue;
                }
                int y_top = staff->GetDrawingY();
                // for the bottom position we need to take into account the number of lines and the staff size
                int y_bottom = staff->GetDrawingY() - (childStaffDef->GetLines() - 1) * m_doc->m_drawingInterl[staff->staffSize];
                DrawBarline( dc, y_top, y_bottom, barLine );
                if ( barLine->HasRepetitionDots() ) {
                    DrawBarlineDots( dc, childStaffDef, staff, barLine );
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
        AttCommonNComparison comparisonFirst( &typeid(Staff), firstDef->GetN() );
        Staff *first = dynamic_cast<Staff*>(measure->FindChildByAttComparison(&comparisonFirst));
        AttCommonNComparison comparisonLast( &typeid(Staff), lastDef->GetN() );
        Staff *last = dynamic_cast<Staff*>(measure->FindChildByAttComparison(&comparisonLast, true));
        
        if (!first || !last ) {
            LogDebug("Could not get staff (%d; %d) while drawing staffGrp - Vrv::DrawStaffGrp", firstDef->GetN(), lastDef->GetN() );
            return;
        }
        
        int y_top = first->GetDrawingY();
        // for the bottom position we need to take into account the number of lines and the staff size
        int y_bottom = last->GetDrawingY() - (lastDef->GetLines() - 1) * m_doc->m_drawingInterl[last->staffSize];
        
        DrawBarline( dc, y_top, y_bottom, barLine );
        
        // Now we have a barthru barLine, but we have dots so we still need to go through each staff
        if ( barLine->HasRepetitionDots() ) {
            int i;
            StaffDef *childStaffDef = NULL;
            for (i = 0; i < staffGrp->GetChildCount(); i++) {
                childStaffDef = dynamic_cast<StaffDef*>(staffGrp->GetChild( i ));
                if ( childStaffDef ) {
                    AttCommonNComparison comparison( &typeid(Staff), childStaffDef->GetN() );
                    Staff *staff = dynamic_cast<Staff*>(measure->FindChildByAttComparison(&comparison));
                    if (!staff ) {
                        LogDebug("Could not get staff (%d) while drawing staffGrp - Vrv::DrawBarlines", childStaffDef->GetN() );
                        continue;
                    }
                    DrawBarlineDots( dc, childStaffDef, staff, barLine );
                }
            }
        }
    }
}

void View::DrawBarline( DeviceContext *dc, int y_top, int y_bottom, Barline *barLine )
{
    assert( dc );
    
    // adjust the top and bottom
    y_top += m_doc->m_env.m_staffLineWidth / 2;
    y_bottom -= m_doc->m_env.m_staffLineWidth / 2;

    int x = barLine->GetDrawingX();
	int x1 = x - m_doc->m_drawingBeamWidth[0] - m_doc->m_env.m_barlineWidth;
	int x2 = x + m_doc->m_drawingBeamWidth[0] + m_doc->m_env.m_barlineWidth;
    
	if (barLine->GetRend() == BARRENDITION_single)
    {
        DrawVerticalLine( dc , y_top, y_bottom, x, m_doc->m_env.m_barlineWidth);
    }
    else if (barLine->GetRend() == BARRENDITION_rptboth)
    {
        DrawVerticalLine( dc , y_top, y_bottom, x1, m_doc->m_env.m_barlineWidth);
        DrawVerticalLine( dc , y_top, y_bottom, x, m_doc->m_drawingBeamWidth[0]);
        DrawVerticalLine( dc , y_top, y_bottom, x2, m_doc->m_env.m_barlineWidth);
    }
    else if (barLine->GetRend()  == BARRENDITION_rptstart)
    {
        DrawVerticalLine( dc , y_top, y_bottom, x, m_doc->m_drawingBeamWidth[0]);
        DrawVerticalLine( dc , y_top, y_bottom, x2, m_doc->m_env.m_barlineWidth);
    }
    else if (barLine->GetRend() == BARRENDITION_rptend)
	{
        DrawVerticalLine( dc , y_top, y_bottom, x1, m_doc->m_env.m_barlineWidth);
        DrawVerticalLine( dc , y_top, y_bottom, x, m_doc->m_drawingBeamWidth[0]);
	}
	else if (barLine->GetRend()  == BARRENDITION_dbl)
	{
        // Narrow the bars a little bit - should be centered?
        x1 += m_doc->m_env.m_barlineWidth;
        DrawVerticalLine( dc , y_top, y_bottom, x, m_doc->m_env.m_barlineWidth);
        DrawVerticalLine( dc , y_top, y_bottom, x1, m_doc->m_env.m_barlineWidth);
	}
	else if (barLine->GetRend()  == BARRENDITION_end)
    {
        DrawVerticalLine( dc , y_top, y_bottom, x1, m_doc->m_env.m_barlineWidth);
        DrawVerticalLine( dc , y_top, y_bottom, x, m_doc->m_drawingBeamWidth[0]);
    }
}

 
void View::DrawBarlineDots ( DeviceContext *dc, StaffDef *staffDef, Staff *staff, Barline *barLine )
{
	assert( dc ); // DC cannot be NULL
    
    int x = barLine->GetDrawingX();
	int x1 = x - 2 * m_doc->m_drawingBeamWidth[0] - m_doc->m_env.m_barlineWidth;
	int x2 = x + 2 * m_doc->m_drawingBeamWidth[0] + m_doc->m_env.m_barlineWidth;
    
    int y_bottom = staff->GetDrawingY() - staffDef->GetLines()  * m_doc->m_drawingHalfInterl[staff->staffSize];
    int y_top = y_bottom + m_doc->m_drawingInterl[staff->staffSize];
 
    if ((barLine->GetRend()  == BARRENDITION_rptstart) || (barLine->GetRend() == BARRENDITION_rptboth))
    {
        DrawDot(dc, x2, y_bottom );
        DrawDot(dc, x2, y_top );

    }
    if ((barLine->GetRend() == BARRENDITION_rptend) || (barLine->GetRend() == BARRENDITION_rptboth))
	{
        DrawDot(dc, x1, y_bottom );
        DrawDot(dc, x1, y_top );
	}
    
	return;
}


void View::DrawPartialBarline ( DeviceContext *dc, System *system, int x, Staff *pportee)
{
	assert( dc ); // DC cannot be NULL
    
    if ( !system ) {
        return;
    }

    /* ax3
	int b, bb;

	Staff *next = system->GetNext( NULL );
	if ( next )
	{	
		b = pportee->m_drawingY - m_doc->m_drawingStaffSize[ pportee->staffSize ]*2;
		bb = next->m_drawingY - m_doc->m_drawingStaffSize[ next->staffSize];

		DrawVerticalLine ( dc, b, bb, x,  m_doc->m_env.m_barlineWidth);
		
	}
    */

}



//----------------------------------------------------------------------------
// View - Measure
//----------------------------------------------------------------------------

void View::DrawMeasure( DeviceContext *dc, Measure *measure, System *system )
{
	assert( dc ); // DC cannot be NULL
    
    Staff *staff = NULL;
    EditorialElement *editorialElement = NULL;
    
    // This is a special case where we do not draw (SVG, Bounding boxes, etc.) the measure if un-measured music
    if ( measure->IsMeasuredMusic()) {
        dc->StartGraphic( measure, "", measure->GetUuid() );
    }
    
    // Here we set the appropriate y value to be used for drawing
    // With Raw documents, we use m_drawingXRel that is calculated by the layout algorithm
    // With Transcription documents, we use the m_xAbs
    if ( measure->m_xAbs == VRV_UNSET ) {
        assert( m_doc->GetType() == Raw );
        measure->SetDrawingX( measure->m_drawingXRel + system->GetDrawingX() );
    }
    else
    {
        assert( m_doc->GetType() == Transcription );
        measure->SetDrawingX( measure->m_xAbs );
    }
    
    Object* current;
    for (current = measure->GetFirst( ); current; current = measure->GetNext( ) )
    {
        staff = dynamic_cast<Staff*>(current);
        editorialElement = dynamic_cast<EditorialElement*>(current);
        if (staff) {
            DrawStaff( dc, staff, measure, system );
        }
        else if (editorialElement) {
            DrawEditorialElement( dc , editorialElement, measure, system );
        }
        else {
            assert(false);
        }
    }

    if ( measure->GetLeftBarlineType() != BARRENDITION_NONE) {
        DrawScoreDef( dc, &m_drawingScoreDef, measure, measure->GetDrawingX(), measure->GetLeftBarline() );
    }
    if ( measure->GetRightBarlineType() != BARRENDITION_NONE) {
        DrawScoreDef( dc, &m_drawingScoreDef, measure, measure->GetDrawingX() + measure->GetRightBarlineX(), measure->GetRightBarline() );
    }
    
    if ( measure->IsMeasuredMusic()) {
        dc->EndGraphic( measure, this );
    }
}


//----------------------------------------------------------------------------
// View - Staff
//----------------------------------------------------------------------------

int View::CalculatePitchPosY ( Staff *staff, char pname, int dec_clef, int oct)
{
    assert(staff); // Pointer to staff cannot be NULL"
	
    static char touches[] = {PITCHNAME_c,PITCHNAME_d,PITCHNAME_e,PITCHNAME_f,PITCHNAME_g,PITCHNAME_a,PITCHNAME_b};
	int y_int;
	char *ptouche, i;
	ptouche=&touches[0];

	y_int = ((dec_clef + oct*7) - 9 ) * m_doc->m_drawingHalfInterl[staff->staffSize];
	if (staff->m_drawingLines > 5)
		y_int -= ((staff->m_drawingLines - 5) * 2) * m_doc->m_drawingHalfInterl[staff->staffSize];

	/* exprime distance separant m_drawingY de
	position 1e Si, corrigee par dec_clef et oct. Elle est additionnee
	ensuite, donc elle doit etre NEGATIVE si plus bas que m_drawingY */
	for (i=0; i<(signed)sizeof(touches); i++)
		if (*(ptouche+i) == pname)
			return(y_int += ((i+1)*m_doc->m_drawingHalfInterl[staff->staffSize]));
	return 0;
}

int View::CalculateRestPosY ( Staff *staff, char duration)
{
    assert(staff); // Pointer to staff cannot be NULL"

	int staff_space = m_doc->m_drawingHalfInterl[staff->staffSize];
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
        case VALSilSpec: offset = 15; break; // MM rests have same height as wholes
            
        default: offset = 12; break; // Signal an error, put the clef up high
    }
    return base + staff_space * offset;
}


void View::DrawStaff( DeviceContext *dc, Staff *staff, Measure *measure, System *system )
{
	assert( dc ); // DC cannot be NULL
    
    Layer *layer = NULL;
    EditorialElement *editorialElement = NULL;
    
    dc->StartGraphic( staff, "", staff->GetUuid());
    
    // Here we set the appropriate y value to be used for drawing
    // With Raw documents, we use m_drawingYRel that is calculated by the layout algorithm
    // With Transcription documents, we use the m_yAbs
    if ( staff->m_yAbs == VRV_UNSET ) {
        assert( m_doc->GetType() == Raw );
        staff->SetDrawingY( staff->GetYRel() + system->GetDrawingY() );
    }
    else
    {
        assert( m_doc->GetType() == Transcription );
        staff->SetDrawingY( staff->m_yAbs );
    }
    if ( StaffDef *staffDef = m_drawingScoreDef.GetStaffDef( staff->GetN() ) ) {
        staff->m_drawingLines = staffDef->GetLines( ) ;
    }
    
    DrawStaffLines( dc, staff, measure, system );
    
    Object* current;
    for (current = staff->GetFirst( ); current; current = staff->GetNext( ) )
    {
        layer = dynamic_cast<Layer*>(current);
        editorialElement = dynamic_cast<EditorialElement*>(current);
        if (layer) {
            DrawLayer( dc, layer, staff, measure );
        }
        else if (editorialElement) {
            DrawEditorialElement( dc , editorialElement, staff, measure );
        }
        else {
            assert(false);
        }
    }

    dc->EndGraphic( staff, this );
}
    
    
void View::DrawStaffLines( DeviceContext *dc, Staff *staff, Measure *measure, System *system )
{
    assert( dc ); // DC cannot be NULL
    
    if (staff->invisible)
        return;
    
    int j, x1, x2, yy;
    
    yy = staff->GetDrawingY();
    
    //x1 = system->m_systemLeftMar;
    //x2 = m_doc->m_drawingPageWidth - m_doc->m_drawingPageLeftMar - m_doc->m_drawingPageRightMar - system->m_systemRightMar;
    
    x1 = measure->GetDrawingX();
    x2 = x1 + measure->GetWidth(); // - m_doc->m_env.m_barlineWidth / 2;
    
    dc->SetPen( m_currentColour, ToDeviceContextX( m_doc->m_env.m_staffLineWidth ), AxSOLID );
    dc->SetBrush( m_currentColour , AxSOLID );
    
    x1 = ToDeviceContextX (x1);
    x2 = ToDeviceContextX (x2);
    
    for(j = 0;j < staff->m_drawingLines; j++)
    {
        dc->DrawLine( x1 , ToDeviceContextY ( yy ) , x2 , ToDeviceContextY ( yy ) );
        yy -= m_doc->m_drawingInterl[staff->staffSize];
    }
    
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
    assert(layer); // Pointer to layer cannot be NULL"
    assert(layer->m_parent); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<Staff*>(layer->m_parent)); // Pointer to parent has to be a staff
	
    static int touches[] = {PITCHNAME_c,PITCHNAME_d,PITCHNAME_e,PITCHNAME_f,PITCHNAME_g,PITCHNAME_a,PITCHNAME_b};
	int y_dec, yb, plafond;
	int degres, octaves, position, code;
	char clefId=0;

    Staff *parentStaff = dynamic_cast<Staff*>(layer->m_parent);
    int staffSize = parentStaff->staffSize;
	// calculer position du do central en fonction clef
	y_n += (int) m_doc->m_drawingVerticalUnit2[staffSize];
	yb = parentStaff->GetDrawingY() -  m_doc->m_drawingStaffSize[staffSize]*2; // UT1 default
	

	plafond = yb + 8 *  m_doc->m_drawingOctaveSize[staffSize];
	if (y_n > plafond)
		y_n = plafond;

    LayerElement *previous = NULL;
	LayerElement *pelement = layer->GetAtPos( x_pos );
	if ( (previous = layer->GetPrevious( pelement ) ) )
		pelement = previous;

	Clef *clef = layer->GetClef (pelement);
    if (clef) {
        clefId = clef->GetClefId();
        yb += (clef->GetClefOffset()) * m_doc->m_drawingHalfInterl[staffSize];	// UT1 reel
    }
	yb -= 4 *  m_doc->m_drawingOctaveSize[staffSize];	// UT, note la plus grave

	y_dec = y_n - yb;	// decalage par rapport a UT le plus grave

	if (y_dec< 0)
		y_dec = 0;

	degres = y_dec /  m_doc->m_drawingHalfInterl[staffSize];	// ecart en degres (PITCHNAME_c..PITCHNAME_b) par rapport a UT1
	octaves = degres / 7;
	position = degres % 7;

	code = touches[position];
	*octave = octaves /*- OCTBIT*/; // LP remove OCTBIT : oct 0 ‡ 7

	return (code);
}

MusPoint CalcPositionAfterRotation( MusPoint point , float rot_alpha, MusPoint center)
{
    int distCenterX = (point.x - center.x);
    int distCenterY = (point.y - center.y);
    // pythagore, distance entre le point d'origine et le centre
    int distCenter = (int)sqrt( pow( (double)distCenterX, 2 ) + pow( (double)distCenterY, 2 ) );
	
	// angle d'origine entre l'axe x et la droite passant par le point et le centre
    float alpha = atan ( (float)distCenterX / (float)(distCenterY) );
    
    MusPoint new_p = center;
    int new_distCenterX, new_distCenterY;

    new_distCenterX = ( (int)( sin( alpha - rot_alpha ) * distCenter ) );
	new_p.x += new_distCenterX;

    new_distCenterY = ( (int)( cos( alpha - rot_alpha ) * distCenter ) );
	new_p.y += new_distCenterY;

    return new_p;
}

void View::DrawLayer( DeviceContext *dc, Layer *layer, Staff *staff, Measure *measure)
{
	assert( dc ); // DC cannot be NULL

	LayerElement *element = NULL;
    EditorialElement *editorialElement = NULL;
    
    dc->StartGraphic( layer, "", layer->GetUuid());
    
    // first we need to clear the drawing list of postponed elements
    layer->ResetDrawingList();
    
    if (layer->GetDrawingClef()) {
        DrawElement(dc, layer->GetDrawingClef(), layer, staff, measure);
    }
    if (layer->GetDrawingKeySig()) {
        DrawElement(dc, layer->GetDrawingKeySig(), layer, staff, measure);
    }
    if (layer->GetDrawingMensur()) {
        DrawElement(dc, layer->GetDrawingMensur(), layer, staff, measure);
    }
    if (layer->GetDrawingMeterSig()) {
        DrawElement(dc, layer->GetDrawingMeterSig(), layer, staff, measure);
    }

    Object* current;
    for (current = layer->GetFirst( ); current; current = layer->GetNext( ) )
    {
        element = dynamic_cast<LayerElement*>(current);
        editorialElement = dynamic_cast<EditorialElement*>(current);
        if (element) {
            DrawElement( dc, element, layer, staff, measure );
        }
        else if (editorialElement) {
            DrawEditorialElement( dc , editorialElement, layer, staff, measure );
        }
        else {
            assert(false);
        }
    }
    
    // first draw the beams
    DrawLayerList(dc, layer, staff, measure, &typeid(Beam) );
    // then tuplets
    DrawLayerList(dc, layer, staff, measure, &typeid(Tuplet) );
    // then ties
    DrawLayerList(dc, layer, staff, measure, &typeid(Tie) );
    // then slurs
    DrawLayerList(dc, layer, staff, measure, &typeid(Slur) );
    
    
    dc->EndGraphic( layer, this );
    
}


void View::DrawLayerList( DeviceContext *dc, Layer *layer, Staff *staff, Measure *measure, const std::type_info *elementType )
{
	assert( dc ); // DC cannot be NULL
    
    ListOfObjects *drawingList = layer->GetDrawingList();
	LayerElement *element = NULL;
    
    ListOfObjects::iterator iter;
    
    for (iter = drawingList->begin(); iter != drawingList->end(); ++iter)
    {
        element = dynamic_cast<LayerElement*>(*iter);
        if (!element) continue; 
        
        if ( (typeid(*element) == *elementType) &&  (*elementType == typeid(Beam) ) ) {
            Beam *beam = dynamic_cast<Beam*>(element);
            dc->ResumeGraphic(beam, beam->GetUuid());
            DrawBeam( dc, layer, beam, staff );
            dc->EndResumedGraphic(beam, this);
        }
        else if ( (typeid(*element) == *elementType) &&  (*elementType == typeid(Tuplet) ) ) {
            Tuplet *tuplet = dynamic_cast<Tuplet*>(element);
            dc->ResumeGraphic(tuplet, tuplet->GetUuid());
            DrawTuplet( dc, tuplet, layer, staff );
            dc->EndResumedGraphic(tuplet, this);
        }
        else if ( (typeid(*element) == *elementType) &&  (*elementType == typeid(Tie) ) ) {
            // Not sure about ReStart and ReEnd Tie and Slur
            DrawTie( dc, element, layer, staff, measure );
        }
        else if ( (typeid(*element) == *elementType) &&  (*elementType == typeid(Slur) ) ) {
            // Not sure about ReStart and ReEnd Tie and Slur
            DrawTie( dc, element, layer, staff, measure );
        }
    }
}
    

//----------------------------------------------------------------------------
// View - Editorial
//----------------------------------------------------------------------------

void View::DrawEditorialElement( DeviceContext *dc, EditorialElement *element, System *system )
{
    if ( dynamic_cast<App*>(element) ) {
        assert( dynamic_cast<App*>(element)->GetLevel() == EDITORIAL_SYSTEM );
    }
    
    dc->StartGraphic( element, "", element->GetUuid());
    
    Measure *measure = NULL;
    ScoreDef *scoreDef = NULL;
    EditorialElement *editorialElement = NULL;
    
    Object* current;
    for (current = element->GetFirst( ); current; current = element->GetNext( ) )
    {
        measure = dynamic_cast<Measure*>(current);
        scoreDef = dynamic_cast<ScoreDef*>(current);
        editorialElement = dynamic_cast<EditorialElement*>(current);
        if (measure) {
            DrawMeasure( dc , measure, system );
        }
        else if (editorialElement) {
            DrawEditorialElement( dc , editorialElement, system );
        }
        // scoreDef are not drawn directly, but anything else should not be possible
        else if (scoreDef) {
            m_drawingScoreDef.Replace( scoreDef );
        }
        else {
            assert(false);
        }
    }
    
    dc->EndGraphic( element, this );
}

void View::DrawEditorialElement( DeviceContext *dc, EditorialElement *element, Measure *measure, System *system )
{
    if ( dynamic_cast<App*>(element) ) {
        assert( dynamic_cast<App*>(element)->GetLevel() == EDITORIAL_MEASURE );
    }
    
    dc->StartGraphic( element, "", element->GetUuid());
    
    Staff *staff = NULL;
    EditorialElement *editorialElement = NULL;
    
    Object* current;
    for (current = element->GetFirst( ); current; current = element->GetNext( ) )
    {
        staff = dynamic_cast<Staff*>(current);
        editorialElement = dynamic_cast<EditorialElement*>(current);
        if (staff) {
            DrawStaff( dc , staff, measure, system );
        }
        else if (editorialElement) {
            DrawEditorialElement( dc , editorialElement, measure, system );
        }
        else {
            assert(false);
        }
    }
    
    dc->EndGraphic( element, this );
}
    
void View::DrawEditorialElement( DeviceContext *dc, EditorialElement *element, Staff *staff,  Measure *measure )
{
    if ( dynamic_cast<App*>(element) ) {
        assert( dynamic_cast<App*>(element)->GetLevel() == EDITORIAL_STAFF );
    }
    
    dc->StartGraphic( element, "", element->GetUuid());
    
    Layer *layer = NULL;
    EditorialElement *editorialElement = NULL;
    
    Object* current;
    for (current = element->GetFirst( ); current; current = element->GetNext( ) )
    {
        layer = dynamic_cast<Layer*>(current);
        editorialElement = dynamic_cast<EditorialElement*>(current);
        if (layer) {
            DrawLayer( dc , layer, staff, measure );
        }
        else if (editorialElement) {
            DrawEditorialElement( dc , editorialElement, staff, measure );
        }
        else {
            assert(false);
        }
    }
    
    dc->EndGraphic( element, this );
}

void View::DrawEditorialElement( DeviceContext *dc, EditorialElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    if ( dynamic_cast<App*>(element) ) {
        assert( dynamic_cast<App*>(element)->GetLevel() == EDITORIAL_LAYER );
    }
    
    dc->StartGraphic( element, "", element->GetUuid());
    
    LayerElement *layerElement = NULL;
    EditorialElement *editorialElement = NULL;
    
    Object* current;
    for (current = element->GetFirst( ); current; current = element->GetNext( ) )
    {
        layerElement = dynamic_cast<LayerElement*>(current);
        editorialElement = dynamic_cast<EditorialElement*>(current);
        if (layerElement) {
            DrawElement( dc, layerElement, layer, staff, measure );
        }
        else if (editorialElement) {
            DrawEditorialElement( dc , editorialElement, layer, staff, measure );
        }
        else {
            assert(false);
        }
    }
    
    dc->EndGraphic( element, this );
}
    
} // namespace vrv




