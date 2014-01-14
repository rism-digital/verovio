/////////////////////////////////////////////////////////////////////////////
// Name:        musrc_graph.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "view.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "doc.h"
#include "staff.h"

namespace vrv {

void View::v_bline ( DeviceContext *dc, int y1, int y2, int x1, int nbr)
{
	assert( dc ); // DC cannot be NULL

    dc->SetPen( m_currentColour, std::max( 1, ToRendererX(nbr) ), AxSOLID );
    dc->SetBrush( m_currentColour, AxSOLID );

	dc->DrawLine( ToRendererX(x1) , ToRendererY(y1), ToRendererX(x1), ToRendererY(y2) );

    dc->ResetPen();
    dc->ResetBrush();
	return;
}



void View::v_bline2 ( DeviceContext *dc, int y1, int y2, int x1, int nbr)	
{
	assert( dc ); // DC cannot be NULL

	SwapY( &y1, &y2 );

	int x2;
  	x2 = x1 - nbr;
	if (ToRendererX(nbr) < 1)
		nbr = ToLogicalX(1);

    dc->SetPen( m_currentColour, 1, AxSOLID );
    dc->SetBrush( m_currentColour, AxSOLID );

	dc->DrawRectangle( ToRendererX( x2 ), ToRendererY(y1), ToRendererX(nbr) , ToRendererX( y1 - y2 ));

    dc->ResetPen();
    dc->ResetBrush();
	
	return;
}


void View::h_bline ( DeviceContext *dc, int x1, int x2, int y1, int nbr)
{		
	assert( dc ); // DC cannot be NULL

    dc->SetPen( m_currentColour, std::max( 1, ToRendererX(nbr) ), AxSOLID );
    dc->SetBrush( m_currentColour, AxTRANSPARENT );

	dc->DrawLine( ToRendererX(x1) , ToRendererY(y1), ToRendererX(x2), ToRendererY(y1) );

    dc->ResetPen();
    dc->ResetBrush();
	return;
}

//draw a box, lol. It's like a rectangle, but not filled in.
void View::box( DeviceContext *dc, int x1, int y1, int x2, int y2)
{
	assert( dc ); // DC cannot be NULL
	
	SwapY( &y1, &y2 );

    dc->SetPen( m_currentColour, 3, AxSOLID);
	
	dc->DrawRectangle( ToRendererX(x1), ToRendererY(y1), ToRendererX(x2 - x1), ToRendererX(y1 - y2));
	
    dc->ResetPen();
	
	return;
}

void View::rect_plein2( DeviceContext *dc, int x1, int y1, int x2, int y2 )	/* dessine rectangle plein */
{	
	assert( dc ); // DC cannot be NULL

	SwapY( &y1, &y2 );

    dc->SetPen( m_currentColour, 1, AxSOLID  );
    dc->SetBrush( m_currentColour, AxSOLID );

	dc->DrawRectangle( ToRendererX( x1 ), ToRendererY(y1), ToRendererX(x2 - x1) , ToRendererX( y1 - y2 ));

    dc->ResetPen();
    dc->ResetBrush();

	return;
}

int View::hGrosseligne ( DeviceContext *dc, int x1, int y1, int x2, int y2, int decal)
{	
	MusPoint p[4];
	int dec = decal;
  
    dc->SetPen( m_currentColour, 1, AxSOLID );
    dc->SetBrush( m_currentColour, AxSOLID );

	decal = ToRendererX(decal);
	p[0].x = ToRendererX(x1);
	p[0].y =  ToRendererY(y1);
	p[1].x = ToRendererX(x2);
	p[1].y =  ToRendererY(y2);
	p[2].x = p[1].x;
	p[2].y = p[1].y - decal;
	p[3].x = p[0].x;
	p[3].y = p[0].y - decal;

	dc->DrawPolygon ( 4, p ); //(sizeof (bcoord)*2) / sizeof (POINT)); nbrInt*2+ 1;

    dc->ResetPen();
    dc->ResetBrush();
	return dec;
}


int View::DoDrawDot ( DeviceContext *dc, int x, int y )
{
	int r = std::max( ToRendererX(3), 2 );
	
    dc->SetPen( m_currentColour, 1, AxSOLID );
    dc->SetBrush( m_currentColour, AxSOLID );

	dc->DrawCircle( ToRendererX(x) , ToRendererY(y), r );
		
    dc->ResetPen();
    dc->ResetBrush();

	return y;
}

void View::DrawLeipzigFont ( DeviceContext *dc, int x, int y, unsigned char c, 
						 Staff *staff, bool dimin )
{  
	int staffSize = staff->staffSize;
	int fontCorr = 0;
    if (dc->CorrectMusicAscent()) {
        fontCorr = m_doc->m_rendFontHeightAscent[staffSize][dimin];
    }

	assert( dc ); // DC cannot be NULL

    // Font offset management for clef in mensural mode - needs improvement (font modification?)
	if (staff->notAnc && (unsigned char)c >= LEIPZIG_OFFSET_IN_FONT)
	{	
		c+= LEIPZIG_OFFSET_MENSURAL;
		if (dimin && is_in (c, 227, 229))	// les trois clefs
		{	
			c+= 14;	// les cles d===e tablature
            if (dc->CorrectMusicAscent()) {
                fontCorr = m_doc->m_rendFontHeightAscent[ staffSize][0];
            }
		}
	}
	if (!staff->notAnc || !is_in (c, 241, 243))	// tout sauf clefs de tablature
	{
        dc->SetFont( &m_doc->m_rendFonts[ staffSize ][ dimin ] );
	}

	if ( dc)
	{	
		dc->SetBackground( AxBLUE );
		dc->SetBackgroundMode( AxTRANSPARENT );

		m_str = (char)c;
		dc->SetTextForeground( m_currentColour );
        dc->SetPen( m_currentColour, 1, AxSOLID );
        dc->SetBrush( m_currentColour, AxSOLID );

		//LogDebug("Drawing text here, x: %d, y: %d, y (zoomed): %d, y + fontcorr: %d"
		//	   , ToRendererX(x), y, ToRendererY(y), ToRendererY(y + fontCorr));
        // DrawMusicText is the same with AxWxDc but different with SvgDeviceContext
		dc->DrawMusicText( m_str, ToRendererX(x), ToRendererY(y + fontCorr) );

        dc->ResetPen();
        dc->ResetBrush();
	}

	return;
}

void View::putstring ( DeviceContext *dc, int x, int y, std::string s, int centrer, int staffSize)
{ 
	assert( dc ); // DC cannot be NULL

    int fontCorr = 0;
    
    dc->SetFont( &m_doc->m_rendFonts[ staffSize ][0] );
    x = ToRendererX(x);
    if (dc->CorrectMusicAscent()) {
        fontCorr = m_doc->m_rendFontHeightAscent[staffSize][0];
    }
    
    
	if ( centrer )
	{
        LogDebug("Centering string not implemented with DeviceContext");
		
        int w, h;
		dc->GetTextExtent( s, &w, &h );
		x -= w / 2;
        
	}
	dc->SetTextForeground( m_currentColour );
	dc->DrawText( s, x, ToRendererY(y + fontCorr ));
}

void View::putlyric ( DeviceContext *dc, int x, int y, std::string s, int staffSize, bool cursor)
{ 
	assert( dc ); // DC cannot be NULL

    dc->SetFont( &m_doc->m_rendLyricFonts[ staffSize ] );
	x = ToRendererX(x);

	dc->SetTextForeground( m_currentColour );
	dc->DrawText( s, x, ToRendererY( y ) );
    
    if (cursor)
        DoLyricCursor( x, y, dc, s );	
}

void View::DrawTieBezier(DeviceContext *dc, int x, int y, int x1, bool direction)
{
    int height = std::max( MIN_TIE_HEIGHT, std::min( 1 * m_doc->m_rendInterl[0], abs( x1 - x ) / 4 ) );
    
    int thickness = std::max( m_doc->m_rendInterl[0] / 3, MIN_TIE_THICKNESS );
    
    int one, two; // control points at 1/4 and 3/4 of total lenght
    int bez1[6], bez2[6]; // filled array with control points and end point
    
    int top_y, top_y_fill; // Y for control points in both beziers
    
    one = (x1 - x) / 4; // point at 1/4
    two = (x1 - x) / 4 * 3; // point at 3/4
    
    if (direction) {
        // tie goes up
        top_y = y - height;
        // the second bezier in internal
        top_y_fill = top_y + thickness;
    } else {
        //tie goes down
        top_y = y + height;
        // second bezier is internal as above
        top_y_fill = top_y - thickness;
    }
    
    // Points for first bez, they go from xy to x1y1
    bez1[0] = ToRendererX(x + one); bez1[1] = ToRendererY(top_y);
    bez1[2] = ToRendererX(x + two); bez1[3] = ToRendererY(top_y);
    bez1[4] = ToRendererX(x1); bez1[5] = ToRendererY(y);
    
    // second bez. goes back
    bez2[0] = ToRendererX(x + two); bez2[1] = ToRendererY(top_y_fill);
    bez2[2] = ToRendererX(x + one); bez2[3] = ToRendererY(top_y_fill);
    bez2[4] = ToRendererX(x); bez2[5] = ToRendererY(y);
    
    // Actually draw it
    dc->DrawComplexBezierPath(ToRendererX(x), ToRendererY(y), bez1, bez2);
}

} // namespace vrv