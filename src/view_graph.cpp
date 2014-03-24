/////////////////////////////////////////////////////////////////////////////
// Name:        view_graph.cpp
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

void View::DrawVerticalLine ( DeviceContext *dc, int y1, int y2, int x1, int nbr)
{
	assert( dc ); // DC cannot be NULL

    dc->SetPen( m_currentColour, std::max( 1, ToDeviceContextX(nbr) ), AxSOLID );
    dc->SetBrush( m_currentColour, AxSOLID );

	dc->DrawLine( ToDeviceContextX(x1) , ToDeviceContextY(y1), ToDeviceContextX(x1), ToDeviceContextY(y2) );

    dc->ResetPen();
    dc->ResetBrush();
	return;
}

void View::DrawHorizontalLine ( DeviceContext *dc, int x1, int x2, int y1, int nbr)
{		
	assert( dc ); // DC cannot be NULL

    dc->SetPen( m_currentColour, std::max( 1, ToDeviceContextX(nbr) ), AxSOLID );
    dc->SetBrush( m_currentColour, AxSOLID );

	dc->DrawLine( ToDeviceContextX(x1) , ToDeviceContextY(y1), ToDeviceContextX(x2), ToDeviceContextY(y1) );

    dc->ResetPen();
    dc->ResetBrush();
	return;
}

void View::DrawFullRectangle( DeviceContext *dc, int x1, int y1, int x2, int y2 )	/* dessine rectangle plein */
{	
	assert( dc ); // DC cannot be NULL

	SwapY( &y1, &y2 );

    dc->SetPen( m_currentColour, 1, AxSOLID  );
    dc->SetBrush( m_currentColour, AxSOLID );

	dc->DrawRectangle( ToDeviceContextX( x1 ), ToDeviceContextY(y1), ToDeviceContextX(x2 - x1) , ToDeviceContextX( y1 - y2 ));

    dc->ResetPen();
    dc->ResetBrush();

	return;
}

void View::DrawObliqueLine ( DeviceContext *dc, int x1, int y1, int x2, int y2, int decal)
{	
	MusPoint p[4];
  
    dc->SetPen( m_currentColour, 1, AxSOLID );
    dc->SetBrush( m_currentColour, AxSOLID );

	decal = ToDeviceContextX(decal);
	p[0].x = ToDeviceContextX(x1);
	p[0].y =  ToDeviceContextY(y1);
	p[1].x = ToDeviceContextX(x2);
	p[1].y =  ToDeviceContextY(y2);
	p[2].x = p[1].x;
	p[2].y = p[1].y - decal;
	p[3].x = p[0].x;
	p[3].y = p[0].y - decal;

	dc->DrawPolygon ( 4, p );

    dc->ResetPen();
    dc->ResetBrush();

}


void View::DrawDot ( DeviceContext *dc, int x, int y )
{
	int r = std::max( ToDeviceContextX(3), 2 );
	
    dc->SetPen( m_currentColour, 1, AxSOLID );
    dc->SetBrush( m_currentColour, AxSOLID );

	dc->DrawCircle( ToDeviceContextX(x) , ToDeviceContextY(y), r );
		
    dc->ResetPen();
    dc->ResetBrush();
}

void View::DrawLeipzigFont ( DeviceContext *dc, int x, int y, unsigned char c, 
						 Staff *staff, bool dimin )
{  
	int staffSize = staff->staffSize;
	int fontCorr = 0;
    if (dc->CorrectMusicAscent()) {
        fontCorr = m_doc->m_drawingFontHeightAscent[staffSize][dimin];
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
                fontCorr = m_doc->m_drawingFontHeightAscent[ staffSize][0];
            }
		}
	}
	if (!staff->notAnc || !is_in (c, 241, 243))	// tout sauf clefs de tablature
	{
        dc->SetFont( &m_doc->m_drawingFonts[ staffSize ][ dimin ] );
	}

	if ( dc)
	{	
		dc->SetBackground( AxBLUE );
		dc->SetBackgroundMode( AxTRANSPARENT );
        
        char str_c[2];
        str_c[0] = c;
        str_c[1] = 0;
        std::string str = str_c;
		dc->SetTextForeground( m_currentColour );
        dc->SetPen( m_currentColour, 1, AxSOLID );
        dc->SetBrush( m_currentColour, AxSOLID );

		//LogDebug("Drawing text here, x: %d, y: %d, y (zoomed): %d, y + fontcorr: %d"
		//	   , ToDeviceContextX(x), y, ToDeviceContextY(y), ToDeviceContextY(y + fontCorr));
        // DrawMusicText is the same with AxWxDc but different with SvgDeviceContext
		dc->DrawMusicText( str, ToDeviceContextX(x), ToDeviceContextY(y + fontCorr) );

        dc->ResetPen();
        dc->ResetBrush();
	}

	return;
}

void View::DrawLeipzigString ( DeviceContext *dc, int x, int y, std::string s, int centrer, int staffSize)
{ 
	assert( dc ); // DC cannot be NULL

    int fontCorr = 0;
    
    dc->SetFont( &m_doc->m_drawingFonts[ staffSize ][0] );
    x = ToDeviceContextX(x);
    if (dc->CorrectMusicAscent()) {
        fontCorr = m_doc->m_drawingFontHeightAscent[staffSize][0];
    }
    
    
	if ( centrer )
	{
        LogDebug("Centering string not implemented with DeviceContext");
		
        int w, h;
		dc->GetTextExtent( s, &w, &h );
		x -= w / 2;
        
	}
	dc->SetTextForeground( m_currentColour );
	dc->DrawText( s, x, ToDeviceContextY(y + fontCorr ));
}

void View::DrawLyricString ( DeviceContext *dc, int x, int y, std::string s, int staffSize, bool cursor)
{ 
	assert( dc ); // DC cannot be NULL

    dc->SetFont( &m_doc->m_drawingLyricFonts[ staffSize ] );
	x = ToDeviceContextX(x);

	dc->SetTextForeground( m_currentColour );
	dc->DrawText( s, x, ToDeviceContextY( y ) );
    
    if (cursor)
        DoLyricCursor( x, y, dc, s );	
}

void View::DrawTieOrSlurBezier(DeviceContext *dc, int x, int y, int x1, int y1, bool direction)
{
    int height = std::max( MIN_TIE_HEIGHT, std::min( 2 * m_doc->m_drawingInterl[0] / 2, abs( x1 - x ) / 4 ) );
    
    int thickness = std::max( m_doc->m_drawingInterl[0] / 3, MIN_TIE_THICKNESS );
    
    int one, two; // control points at 1/4 and 3/4 of total lenght
    int bez1[6], bez2[6]; // filled array with control points and end point
    
    int top_y, top_y_fill; // Y for control points in both beziers
    
    one = (x1 - x) / 4; // point at 1/4
    two = (x1 - x) / 4 * 3; // point at 3/4
    
    if (direction) {
        // tie goes up
        top_y = std::min(y, y1) - height;
        // the second bezier in internal
        top_y_fill = top_y + thickness;
    } else {
        //tie goes down
        top_y = std::max(y, y1) + height;
        // second bezier is internal as above
        top_y_fill = top_y - thickness;
    }
    
    // Points for first bez, they go from xy to x1y1
    bez1[0] = ToDeviceContextX(x + one); bez1[1] = ToDeviceContextY(top_y);
    bez1[2] = ToDeviceContextX(x + two); bez1[3] = ToDeviceContextY(top_y);
    bez1[4] = ToDeviceContextX(x1); bez1[5] = ToDeviceContextY(y1);
    
    // second bez. goes back
    bez2[0] = ToDeviceContextX(x + two); bez2[1] = ToDeviceContextY(top_y_fill);
    bez2[2] = ToDeviceContextX(x + one); bez2[3] = ToDeviceContextY(top_y_fill);
    bez2[4] = ToDeviceContextX(x); bez2[5] = ToDeviceContextY(y);
    
    // Actually draw it
    dc->DrawComplexBezierPath(ToDeviceContextX(x), ToDeviceContextY(y), bez1, bez2);
}

} // namespace vrv