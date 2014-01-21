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

#include "beam.h"
#include "barline.h"
#include "clef.h"
#include "doc.h"
#include "keysig.h"
#include "layer.h"
#include "layerelement.h"
#include "measure.h"
#include "mensur.h"
#include "page.h"
#include "staff.h"
#include "system.h"
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
    m_drawScoreDef = m_currentPage->m_drawScoreDef;

    if ( background )
        dc->DrawRectangle( 0, 0, m_doc->m_drawPageWidth, m_doc->m_drawPageHeight );
    
    dc->DrawBackgroundImage( );
    
    MusPoint origin = dc->GetLogicalOrigin();
    dc->SetLogicalOrigin( origin.x - m_doc->m_drawPageLeftMar, origin.y - m_doc->m_drawPageTopMar );

    dc->StartPage();

    for (i = 0; i < m_currentPage->GetSystemCount(); i++) 
	{
		system = (System*)m_currentPage->m_children[i];
        DrawSystem( dc, system );
        
        // TODO here: also update x_abs and m_drawY positions for system. How to calculate them?
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
    
	int i;
    Measure *measure;
    
    dc->StartGraphic( system, "system", system->GetUuid() );
    
    
    if ( system->m_yAbs == VRV_UNSET ) {
        assert( m_doc->GetType() == Raw );
        system->m_drawY = system->m_drawYRel;
        system->m_drawX = system->m_drawXRel;
    }
    else
    {
        assert( m_doc->GetType() == Transcription );
        system->m_drawY = system->m_yAbs;
        system->m_drawX = system->m_xAbs;
    }
    
    
    for (i = 0; i < (int)system->GetMeasureCount(); i++)
	{
		measure = (Measure*)system->m_children[i];
        DrawMeasure( dc , measure, system );
	}

    // We draw the groups after the staves because we use the m_drawY member of the staves
    // that needs to be intialized.
    // Warning: we assume for now the scoreDef occuring in the system will not change the staffGrps content
    // and @symbol values, otherwise results will be unexpected...
    // First get the first measure of the system
    measure  = dynamic_cast<Measure*>(system->GetFirstChild( &typeid(Measure) ) );
    if ( measure ) {
        // NULL for the Barline parameters indicates that we are drawing the scoreDef
        DrawScoreDef( dc, &m_drawScoreDef, measure, system->m_drawX, NULL );
    }
    
    dc->EndGraphic(system, this );

}

void View::DrawScoreDef( DeviceContext *dc, ScoreDef *scoreDef, Measure *measure, int x, Barline *barline  )
{
    assert( scoreDef ); // other asserted before

    // we need at least one measure to be able to draw the groups - we need access to the staff elements,
    assert( measure );
    
    StaffGrp *staffGrp = dynamic_cast<StaffGrp*>(scoreDef->GetFirstChild( &typeid(StaffGrp) ) );
    if ( !staffGrp ) {
        return;
    }
    
    if ( barline == NULL) {
        // Draw the first staffGrp and from there its children recursively
        DrawStaffGrp( dc, measure, staffGrp, x );
    }
    else{
        dc->StartGraphic( barline, "barline", barline->GetUuid() );
        DrawBarlines( dc, measure, staffGrp, x, barline );
        dc->EndGraphic( barline, this );
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
    Staff *first = measure->GetStaffWithNo( firstDef->GetStaffNo() );
    Staff *last = measure->GetStaffWithNo( lastDef->GetStaffNo() );
    
    if (!first || !last ) {
        LogDebug("Could not get staff (%d; %d) while drawing staffGrp - Vrv::DrawStaffGrp", firstDef->GetStaffNo(), lastDef->GetStaffNo() );
        return;
    }
    
    int y_top = first->m_drawY;
    // for the bottom position we need to take into account the number of lines and the staff size
    int y_bottom = last->m_drawY - (last->portNbLine - 1) * m_doc->m_drawInterl[last->staffSize];
    
    // actually draw the line, the brace or the bracket
    if ( staffGrp->GetSymbol() == STAFFGRP_LINE ) {
        v_bline( dc , y_top, y_bottom, x, m_doc->m_env.m_barlineWidth);
    }
    else if ( staffGrp->GetSymbol() == STAFFGRP_BRACE ) {
        DrawBrace ( dc, x, y_top, y_bottom, last->staffSize );
    }
    else if ( staffGrp->GetSymbol() == STAFFGRP_BRACKET ) {
        DrawBracket( dc, x, y_top, y_bottom, last->staffSize );
        x -= 2 * m_doc->m_drawBeamWidth[0] - m_doc->m_drawBeamWhiteWidth[0];
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


void View::DrawBracket ( DeviceContext *dc, int x, int y1, int y2, int staffSize)
{
	int xg, xd, yg, yd, ecart, centre;

    dc->SetPen( m_currentColour , 2, AxSOLID );
    dc->SetBrush( m_currentColour , AxTRANSPARENT );
    
    ecart = m_doc->m_drawBeamWidth[0] + m_doc->m_drawBeamWhiteWidth[0];
    centre = x - ecart;
    
    xg = centre - ecart*2;
    xd = centre + ecart*2;
    
    yg = y1 + m_doc->m_drawInterl[ staffSize ] * 2;
    yd = y1;
    SwapY( &yg, &yd );
    dc->DrawEllipticArc( ToRendererX(xg), ToRendererY(yg), ToRendererX(xd-xg), ToRendererX(yg-yd), 90, 40 );
    
    yg = y2;
    yd = y2 - m_doc->m_drawInterl[ staffSize ] * 2;
    SwapY( &yg, &yd );
    dc->DrawEllipticArc( ToRendererX(xg), ToRendererY(yg), ToRendererX(xd-xg), ToRendererX(yg-yd), 320, 270 );
    
    dc->ResetPen();
    dc->ResetBrush();
    
    xg = x - (m_doc->m_drawBeamWhiteWidth[0] + 1);
    // determine le blanc entre barres grosse et mince
    v_bline2( dc, y1, y2, xg, m_doc->m_drawBeamWidth[0]);

	return;
}


void View::DrawBrace ( DeviceContext *dc, int x, int y1, int y2, int staffSize)
{	
    int new_coords[2][6];
    
	assert( dc ); // DC cannot be NULL

	SwapY( &y1, &y2 );
	
	int ymed, xdec, fact, nbrInt;
    
    dc->SetPen( m_currentColour , 1, AxSOLID );
    dc->SetBrush( m_currentColour , AxSOLID );
    
	x -= m_doc->m_drawBeamWhiteWidth[ staffSize ];  // distance entre barre et debut accolade
    
	nbrInt = BEZIER_NB_POINTS;
    
	ymed = (y1 + y2) / 2;
	fact = m_doc->m_drawBeamWidth[ staffSize ]-1 + m_doc->m_env.m_barlineWidth;
	xdec = ToRendererX(fact);
    
	point_[0].x = ToRendererX(x);
	point_[0].y = ToRendererY(y1);
	point_[1].x = ToRendererX(x - m_doc->m_drawStep2);
	point_[1].y = point_[0].y - ToRendererX( m_doc->m_drawInterl[ staffSize ]*3);
	point_[3].x = ToRendererX(x - m_doc->m_drawStep1*2);
	point_[3].y = ToRendererY(ymed);
	point_[2].x = ToRendererX(x + m_doc->m_drawStep1);
	point_[2].y = point_[3].y + ToRendererX( m_doc->m_drawInterl[ staffSize ]);
    
    new_coords[0][0] = point_[1].x;
    new_coords[0][1] = point_[1].y;
    new_coords[0][2] = point_[2].x;
    new_coords[0][3] = point_[2].y;
    new_coords[0][4] = point_[3].x;
    new_coords[0][5] = point_[3].y;
    
	pntswap (&point_[0], &point_[3]);
	pntswap (&point_[1], &point_[2]);
	
	point_[1].x += xdec;
	point_[2].x += xdec;
	point_[1].y = point_[0].y + ToRendererX( m_doc->m_drawInterl[ staffSize ]*2);
    
    new_coords[1][0] = point_[1].x;
    new_coords[1][1] = point_[1].y;
    new_coords[1][2] = point_[2].x;
    new_coords[1][3] = point_[2].y;
    new_coords[1][4] = point_[3].x;
    new_coords[1][5] = point_[3].y;
    
    dc->DrawComplexBezierPath(ToRendererX(x), ToRendererY(y1), new_coords[0], new_coords[1]);
    
	// on produit l'image reflet vers le bas: 0 est identique 
	point_[1].y = point_[0].y - ToRendererX( m_doc->m_drawInterl[ staffSize ]*2);
	point_[3].y = ToRendererY(y2);
	point_[2].y = point_[3].y + ToRendererX( m_doc->m_drawInterl[ staffSize ]*3);
    
    new_coords[0][0] = point_[1].x;
    new_coords[0][1] = point_[1].y;
    new_coords[0][2] = point_[2].x;
    new_coords[0][3] = point_[2].y;
    new_coords[0][4] = point_[3].x;
    new_coords[0][5] = point_[3].y;
    
	pntswap (&point_[0], &point_[3]);
	pntswap (&point_[1], &point_[2]);
	
	point_[1].x -= xdec;
	point_[2].x -= xdec;
	point_[2].y = point_[3].y - ToRendererX( m_doc->m_drawInterl[ staffSize ]);
    
    new_coords[1][0] = point_[1].x;
    new_coords[1][1] = point_[1].y;
    new_coords[1][2] = point_[2].x;
    new_coords[1][3] = point_[2].y;
    new_coords[1][4] = point_[3].x;
    new_coords[1][5] = point_[3].y;
    
    dc->DrawComplexBezierPath(point_[3].x, point_[3].y, new_coords[0], new_coords[1]);
    
    dc->ResetPen();
    dc->ResetBrush();
        
	return;
}


void View::DrawBarlines( DeviceContext *dc, Measure *measure, StaffGrp *staffGrp, int x, Barline *barline )
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
                DrawBarlines( dc, measure, childStaffGrp, x, barline );
            }
            else if ( childStaffDef ) {
                Staff *staff = measure->GetStaffWithNo( childStaffDef->GetStaffNo() );
                if (!staff ) {
                    LogDebug("Could not get staff (%d) while drawing staffGrp - Vrv::DrawBarlines", childStaffDef->GetStaffNo() );
                    continue;
                }
                int y_top = staff->m_drawY;
                // for the bottom position we need to take into account the number of lines and the staff size
                int y_bottom = staff->m_drawY - (staff->portNbLine - 1) * m_doc->m_drawInterl[staff->staffSize];
                DrawBarline( dc, x, y_top, y_bottom, barline );
                if ( barline->HasRepetitionDots() ) {
                    DrawBarlineDots( dc, x, staff, barline );
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
        Staff *first = measure->GetStaffWithNo( firstDef->GetStaffNo() );
        Staff *last = measure->GetStaffWithNo( lastDef->GetStaffNo() );
        
        if (!first || !last ) {
            LogDebug("Could not get staff (%d; %d) while drawing staffGrp - Vrv::DrawStaffGrp", firstDef->GetStaffNo(), lastDef->GetStaffNo() );
            return;
        }
        
        int y_top = first->m_drawY;
        // for the bottom position we need to take into account the number of lines and the staff size
        int y_bottom = last->m_drawY - (last->portNbLine - 1) * m_doc->m_drawInterl[last->staffSize];
        
        DrawBarline( dc, x, y_top, y_bottom, barline );
        
        // Now we have a barthru barline, but we have dots so we still need to go through each staff
        if ( barline->HasRepetitionDots() ) {
            int i;
            StaffDef *childStaffDef = NULL;
            for (i = 0; i < staffGrp->GetChildCount(); i++) {
                childStaffDef = dynamic_cast<StaffDef*>(staffGrp->GetChild( i ));
                if ( childStaffDef ) {
                    Staff *staff = measure->GetStaffWithNo( childStaffDef->GetStaffNo() );
                    if (!staff ) {
                        LogDebug("Could not get staff (%d) while drawing staffGrp - Vrv::DrawBarlines", childStaffDef->GetStaffNo() );
                        continue;
                    }
                    DrawBarlineDots( dc, x, staff, barline );
                }
            }
        }
    }
}

void View::DrawBarline( DeviceContext *dc, int x, int y_top, int y_bottom, Barline *barline )
{
    assert( dc );

	int x1 = x - m_doc->m_drawBeamWidth[0] - m_doc->m_env.m_barlineWidth;
	int x2 = x + m_doc->m_drawBeamWidth[0] + m_doc->m_env.m_barlineWidth;
    
	if (barline->m_barlineType == BARLINE_SINGLE)
    {
        v_bline( dc , y_top, y_bottom, x, m_doc->m_env.m_barlineWidth);
    }
    else if (barline->m_barlineType == BARLINE_RPTBOTH)
    {
        v_bline( dc , y_top, y_bottom, x1, m_doc->m_env.m_barlineWidth);
        v_bline( dc , y_top, y_bottom, x, m_doc->m_drawBeamWidth[0]);
        v_bline( dc , y_top, y_bottom, x2, m_doc->m_env.m_barlineWidth);
    }
    else if (barline->m_barlineType  == BARLINE_RPTSTART)
    {
        v_bline( dc , y_top, y_bottom, x, m_doc->m_drawBeamWidth[0]);
        v_bline( dc , y_top, y_bottom, x2, m_doc->m_env.m_barlineWidth);
    }
    else if (barline->m_barlineType == BARLINE_RPTEND)
	{
        v_bline( dc , y_top, y_bottom, x1, m_doc->m_env.m_barlineWidth);
        v_bline( dc , y_top, y_bottom, x, m_doc->m_drawBeamWidth[0]);
	}
	else if (barline->m_barlineType  == BARLINE_DBL)
	{
        // Narrow the bars a little bit - should be centered?
        x1 += m_doc->m_env.m_barlineWidth;
        v_bline( dc , y_top, y_bottom, x, m_doc->m_env.m_barlineWidth);
        v_bline( dc , y_top, y_bottom, x1, m_doc->m_env.m_barlineWidth);
	}
	else if (barline->m_barlineType  == BARLINE_END)
    {
        v_bline( dc , y_top, y_bottom, x1, m_doc->m_env.m_barlineWidth);
        v_bline( dc , y_top, y_bottom, x, m_doc->m_drawBeamWidth[0]);
    }
}

 
void View::DrawBarlineDots ( DeviceContext *dc, int x, Staff *staff, Barline *barline )
{
	assert( dc ); // DC cannot be NULL
    
	int x1 = x - 2 * m_doc->m_drawBeamWidth[0] - m_doc->m_env.m_barlineWidth;
	int x2 = x + 2 * m_doc->m_drawBeamWidth[0] + m_doc->m_env.m_barlineWidth;
    
    int y_bottom = staff->m_drawY - staff->portNbLine  * m_doc->m_drawHalfInterl[staff->staffSize];
    int y_top = y_bottom + m_doc->m_drawInterl[staff->staffSize];
 
    if ((barline->m_barlineType  == BARLINE_RPTSTART) || (barline->m_barlineType == BARLINE_RPTBOTH))
    {
        DoDrawDot(dc, x2, y_bottom );
        DoDrawDot(dc, x2, y_top );

    }
    if ((barline->m_barlineType == BARLINE_RPTEND) || (barline->m_barlineType == BARLINE_RPTBOTH))
	{
        DoDrawDot(dc, x1, y_bottom );
        DoDrawDot(dc, x1, y_top );
	}
    
	return;
}


void View::DrawPartialBarline ( DeviceContext *dc, System *system, int x, Staff *pportee)
{
	assert( dc ); // DC cannot be NULL

    /* ax3
	int b, bb;

	Staff *next = system->GetNext( NULL );
	if ( next )
	{	
		b = pportee->m_drawY - m_doc->m_drawStaffSize[ pportee->staffSize ]*2;
		bb = next->m_drawY - m_doc->m_drawStaffSize[ next->staffSize];

		if (m_doc->m_env.m_barlineWidth > 2)	// barres plus epaisses qu'un 1/2 mm
			v_bline2 ( dc, b, bb, x,  m_doc->m_env.m_barlineWidth);
		else
			v_bline ( dc, b, bb, x,  m_doc->m_env.m_barlineWidth);
		
	}
    */

}



//----------------------------------------------------------------------------
// View - Measure
//----------------------------------------------------------------------------

void View::DrawMeasure( DeviceContext *dc, Measure *measure, System *system )
{
	assert( dc ); // DC cannot be NULL
    
    // This is a special case where we do not draw (SVG, Bounding boxes, etc.) the measure if un-measured music
    if ( measure->IsMeasuredMusic()) {
        dc->StartGraphic( measure, "measure", measure->GetUuid() );
    }
    
    // Here we set the appropriate y value to be used for drawing
    // With Raw documents, we use m_drawXRel that is calculated by the layout algorithm
    // With Transcription documents, we use the m_xAbs
    if ( measure->m_xAbs == VRV_UNSET ) {
        assert( m_doc->GetType() == Raw );
        measure->m_drawX = measure->m_drawXRel + system->m_drawX;
    }
    else
    {
        assert( m_doc->GetType() == Transcription );
        measure->m_drawX = measure->m_xAbs;
    }
    
	Staff *staff = NULL;
	int j;
    
	for(j = 0; j < measure->GetStaffCount(); j++)
	{
		staff = (Staff*)measure->m_children[j];
		DrawStaff( dc, staff, measure, system );
    }

    if ( measure->GetLeftBarlineType() != BARLINE_NONE) {
        DrawScoreDef( dc, &m_drawScoreDef, measure, measure->m_drawX, measure->GetLeftBarline() );
    }
    if ( measure->GetRightBarlineType() != BARLINE_NONE) {
        DrawScoreDef( dc, &m_drawScoreDef, measure, measure->m_drawX + measure->GetXRelRight(), measure->GetRightBarline() );
    }
    
    if ( measure->IsMeasuredMusic()) {
        dc->EndGraphic( measure, this );
    }
}


//----------------------------------------------------------------------------
// View - Staff
//----------------------------------------------------------------------------

int View::CalculateNeumePosY ( Staff *staff, char note, int dec_clef, int oct)
{
    assert(staff); // Pointer to staff cannot be NULL"

	static char notes[] = {1,2,3,4,5,6,7};
	int y_int;
	char *pnote, i;
	pnote = &notes[0] - 1;
	
	y_int = ((dec_clef + oct*7) - 17 ) * m_doc->m_drawHalfInterl[staff->staffSize];
	if (staff->portNbLine > 4)
		y_int -= ((staff->portNbLine - 4) * 2) * m_doc->m_drawHalfInterl[staff->staffSize];
	
	for (i=0; i<(signed)sizeof(notes); i++)
		if (*(pnote+i) == note)
			return(y_int += (i*m_doc->m_drawHalfInterl[staff->staffSize]));
	return 0;
}

int View::CalculatePitchPosY ( Staff *staff, char pname, int dec_clef, int oct)
{
    assert(staff); // Pointer to staff cannot be NULL"
	
    static char touches[] = {PITCH_C,PITCH_D,PITCH_E,PITCH_F,PITCH_G,PITCH_A,PITCH_B};
	int y_int;
	char *ptouche, i;
	ptouche=&touches[0];

	y_int = ((dec_clef + oct*7) - 9 ) * m_doc->m_drawHalfInterl[staff->staffSize];
	if (staff->portNbLine > 5)
		y_int -= ((staff->portNbLine - 5) * 2) * m_doc->m_drawHalfInterl[staff->staffSize];

	/* exprime distance separant m_drawY de
	position 1e Si, corrigee par dec_clef et oct. Elle est additionnee
	ensuite, donc elle doit etre NEGATIVE si plus bas que m_drawY */
	for (i=0; i<(signed)sizeof(touches); i++)
		if (*(ptouche+i) == pname)
			return(y_int += ((i+1)*m_doc->m_drawHalfInterl[staff->staffSize]));
	return 0;
}

int View::CalculateRestPosY ( Staff *staff, char duration)
{
    assert(staff); // Pointer to staff cannot be NULL"

	int staff_space = m_doc->m_drawHalfInterl[staff->staffSize];
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

void View::DrawStaffLines( DeviceContext *dc, Staff *staff, Measure *measure, System *system )
{
	assert( dc ); // DC cannot be NULL
        
    if (staff->invisible)
        return;

	int j, x1, x2, yy;

	yy = staff->m_drawY;

	x1 = system->m_systemLeftMar;
    x2 = m_doc->m_drawPageWidth - m_doc->m_drawPageLeftMar - m_doc->m_drawPageRightMar - system->m_systemRightMar;

	dc->SetPen( m_currentColour, ToRendererX( m_doc->m_env.m_staffLineWidth ), AxSOLID );
    dc->SetBrush( m_currentColour , AxSOLID );

	x1 = ToRendererX (x1);
	x2 = ToRendererX (x2);

	for(j = 0;j < staff->portNbLine; j++)
	{
		dc->DrawLine( x1 , ToRendererY ( yy ) , x2 , ToRendererY ( yy ) );
		yy -= m_doc->m_drawInterl[staff->staffSize];
	}
    
    dc->ResetPen( );
    dc->ResetBrush( );
    
	return;
}



void View::DrawStaff( DeviceContext *dc, Staff *staff, Measure *measure, System *system )
{
	assert( dc ); // DC cannot be NULL
    
    dc->StartGraphic( staff, "staff", staff->GetUuid());
    
    // Here we set the appropriate y value to be used for drawing
    // With Raw documents, we use m_drawYRel that is calculated by the layout algorithm
    // With Transcription documents, we use the m_yAbs
    if ( staff->m_yAbs == VRV_UNSET ) {
        assert( m_doc->GetType() == Raw );
        staff->m_drawY = staff->GetYRel() + system->m_drawY;
    }
    else
    {
        assert( m_doc->GetType() == Transcription );
        staff->m_drawY = staff->m_yAbs;
    }
    
    DrawStaffLines( dc, staff, measure, system );
        
	Layer *layer = NULL;
	int j;
    
	for(j = 0; j < staff->GetLayerCount(); j++)
	{
		layer = (Layer*)staff->m_children[j];
		DrawLayer( dc, layer, staff, measure );
	}
    
    dc->EndGraphic( staff, this );
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
	
    static int touches[] = {PITCH_C,PITCH_D,PITCH_E,PITCH_F,PITCH_G,PITCH_A,PITCH_B};
	int y_dec, yb, plafond;
	int degres, octaves, position, code;
	char clefId=0;

    int staffSize = ((Staff*)layer->m_parent)->staffSize;
	// calculer position du do central en fonction clef
	y_n += (int) m_doc->m_drawVerticalUnit2[staffSize];
	yb = ((Staff*)layer->m_parent)->m_drawY -  m_doc->m_drawStaffSize[((Staff*)layer->m_parent)->staffSize]*2; // UT1 default
	

	plafond = yb + 8 *  m_doc->m_drawOctaveSize[staffSize];
	if (y_n > plafond)
		y_n = plafond;

    LayerElement *previous = NULL;
	LayerElement *pelement = layer->GetAtPos( x_pos );
	if ( (previous = layer->GetPrevious( pelement ) ) )
		pelement = previous;

	Clef *clef = layer->GetClef (pelement);
    if (clef) {
        clefId = clef->m_clefId;
        yb += (clef->GetClefOffset()) * m_doc->m_drawHalfInterl[staffSize];	// UT1 reel
    }
	yb -= 4 *  m_doc->m_drawOctaveSize[staffSize];	// UT, note la plus grave

	y_dec = y_n - yb;	// decalage par rapport a UT le plus grave

	if (y_dec< 0)
		y_dec = 0;

	degres = y_dec /  m_doc->m_drawHalfInterl[staffSize];	// ecart en degres (PITCH_C..PITCH_B) par rapport a UT1
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

/**
  x1 y1 = point de depart
  x2 y2 = point d'arrivee
  up = liaison vers le haut
  heigth = hauteur de la liaison ( ‡ plat )
  **/
void View::DrawSlur( DeviceContext *dc, Layer *layer, int x1, int y1, int x2, int y2, bool up, int height )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(layer->m_parent); // Pointer to staff cannot be NULL"

    dc->SetPen( m_currentColour, 1, AxSOLID );
    dc->SetBrush( m_currentColour, AxSOLID );


    int distX = x1 - x2;
    int distY = y1 - y2;
    // pythagore, distance entre les points de depart et d'arrivee
    int dist = (int)sqrt( pow( (double)distX, 2 ) + pow( (double)distY, 2 ) );

	// angle
    float alpha2 = float( distY ) / float( distX );
    alpha2 = atan( alpha2 );
	MusPoint orig(0,0);

	int step = dist / 10; // espace entre les points
	int nbpoints = dist / step;
	dist += 2*step; // ajout d'un pas de chaque cote, supprime ‡ l'affichage
	if ( nbpoints <= 2)
		nbpoints = 3;
	else if ( !(nbpoints % 2) ) // nombre impair de points
		nbpoints++;

	int i,j, k;
	double a, b;
	a = dist / 2; // largeur de l'ellipse
	int nbp2 = nbpoints/2;
	MusPoint *points = new MusPoint[2*nbpoints]; // buffer double - aller retour

	// aller
	b = 100; // hauteur de l'ellipse
	points[nbp2].x = (int)a; // point central
	points[nbp2].y = (int)b;
	for(i = 0, j = nbpoints - 1, k = 1; i < nbp2; i++, j--, k++) // calcul de l'ellipse
	{
		points[i].x = k*step;
		points[i+nbp2+1].y = (int)sqrt( (1 - pow((double)points[i].x,2) / pow(a,2)) * pow(b,2) );
		points[j].x = dist - points[i].x;
		points[j-nbp2-1].y = points[i+nbp2+1].y;		
	}
	int dec_y = points[0].y; // decalage y ‡ cause des 2 pas ajoutes
	for(i = 0; i < nbpoints; i++)
	{
		points[i] = CalcPositionAfterRotation( points[i], alpha2, orig ); // rotation		
		points[i].x = ToRendererX( points[i].x + x1 - 1*step ); // transposition
		points[i].y = ToRendererY( points[i].y + y1 - dec_y );
	}
	dc->DrawSpline( nbpoints, points );

	// retour idem
	b = 90;	
	points[nbp2+nbpoints].x = (int)a;
	points[nbp2+nbpoints].y = (int)b;
	for(i = nbpoints, j = 2*nbpoints - 1, k = 1; i < nbp2+nbpoints; i++, j--, k++)
	{	
		points[j].x = k*step;
		points[i+nbp2+1].y = (int)sqrt( (1 - pow((double)points[j].x,2) / pow(a,2)) * pow(b,2) );
		points[i].x = dist - points[j].x;
		points[j-nbp2-1].y = points[i+nbp2+1].y;	
	}
	dec_y = points[nbpoints].y;

	for(i = nbpoints; i < 2*nbpoints; i++)
	{
		points[i] = CalcPositionAfterRotation( points[i], alpha2, orig );
		points[i].x = ToRendererX( points[i].x + x1 - 1*step );
		points[i].y = ToRendererY( points[i].y + y1 - dec_y );
	}
	dc->DrawSpline( nbpoints, points+nbpoints );

	// remplissage ?

    dc->ResetPen( );
    dc->ResetBrush( );

	delete[] points;

}

void View::DrawLayer( DeviceContext *dc, Layer *layer, Staff *staff, Measure *measure)
{
	assert( dc ); // DC cannot be NULL

	LayerElement *element = NULL;
	int j;
    
    // first we need to clear the drawing list of postponed elements
    layer->ResetDrawingList();
    
    if (layer->GetClefAttr()) {
        DrawElement(dc, layer->GetClefAttr(), layer, measure, staff);
    }
    if (layer->GetKeySigAttr()) {
        DrawElement(dc, layer->GetKeySigAttr(), layer, measure, staff);
    }
    if (layer->GetMensurAttr()) {
        DrawElement(dc, layer->GetMensurAttr(), layer, measure, staff);
    }
    
	for(j = 0; j < layer->GetElementCount(); j++)
	{
		element = (LayerElement*)layer->m_children[j];
        
        if ( !element->m_in_layer_app ) {
            DrawElement( dc, element, layer, measure, staff );
        }
	}
    
    // first draw the beams
    DrawLayerList(dc, layer, staff, measure, &typeid(Beam) );
    // then tuplets
    DrawLayerList(dc, layer, staff, measure, &typeid(Tuplet) );
    // then ties
    DrawLayerList(dc, layer, staff, measure, &typeid(Tie) );
    
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
            DrawBeam( dc, layer, beam, staff );
        }
        else if ( (typeid(*element) == *elementType) &&  (*elementType == typeid(Tuplet) ) ) {
            Tuplet *tuplet = dynamic_cast<Tuplet*>(element);
            DrawTuplet( dc, tuplet, layer, staff );
        }
        else if ( (typeid(*element) == *elementType) &&  (*elementType == typeid(Tie) ) ) {
            Tie *tie = dynamic_cast<Tie*>(element);
            DrawTie( dc, tie, layer, staff, measure );
        }
    }
}

} // namespace vrv




