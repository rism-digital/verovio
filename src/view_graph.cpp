/////////////////////////////////////////////////////////////////////////////
// Name:        view_graph.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "view.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <sstream>

//----------------------------------------------------------------------------

#include "devicecontext.h"
#include "doc.h"
#include "style.h"
#include "vrv.h"

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

    dc->SetPen( m_currentColour, 0, AxSOLID  );
    dc->SetBrush( m_currentColour, AxSOLID );

	dc->DrawRectangle( ToDeviceContextX( x1 ), ToDeviceContextY(y1), ToDeviceContextX(x2 - x1) , ToDeviceContextX( y1 - y2 ));

    dc->ResetPen();
    dc->ResetBrush();

	return;
}

void View::DrawObliquePolygon ( DeviceContext *dc, int x1, int y1, int x2, int y2, int height)
{	
	Point p[4];
  
    dc->SetPen( m_currentColour, 0, AxSOLID );
    dc->SetBrush( m_currentColour, AxSOLID );

	height = ToDeviceContextX(height);
	p[0].x = ToDeviceContextX(x1);
	p[0].y =  ToDeviceContextY(y1);
	p[1].x = ToDeviceContextX(x2);
	p[1].y =  ToDeviceContextY(y2);
	p[2].x = p[1].x;
	p[2].y = p[1].y - height;
	p[3].x = p[0].x;
	p[3].y = p[0].y - height;

	dc->DrawPolygon ( 4, p );

    dc->ResetPen();
    dc->ResetBrush();

}


void View::DrawDot ( DeviceContext *dc, int x, int y )
{
	int r = std::max( ToDeviceContextX( m_doc->m_drawingDoubleUnit[0] / 5 ), 2 );
	
    dc->SetPen( m_currentColour, 1, AxSOLID );
    dc->SetBrush( m_currentColour, AxSOLID );

	dc->DrawCircle( ToDeviceContextX(x) , ToDeviceContextY(y), r );
		
    dc->ResetPen();
    dc->ResetBrush();
}

void View::DrawSmuflCode ( DeviceContext *dc, int x, int y, wchar_t code,
						 int staffSize, bool dimin )
{  
	int fontCorr = 0;
    
    if (dc->CorrectMusicAscent()) {
        fontCorr = m_doc->m_drawingFontHeightAscent[staffSize][dimin];
    }

	assert( dc ); // DC cannot be NULL
    

	if ( dc)
	{	
		dc->SetBackground( AxBLUE );
		dc->SetBackgroundMode( AxTRANSPARENT );
        
        std::wstring str;
        str.push_back(code);
        
        dc->SetBrush( m_currentColour, AxSOLID );
        dc->SetFont( &m_doc->m_drawingSmuflFonts[staffSize][dimin] );

		dc->DrawMusicText( str, ToDeviceContextX(x), ToDeviceContextY(y + fontCorr) );

        dc->ResetFont();
        dc->ResetBrush();

	}

	return;
}

void View::DrawSmuflString ( DeviceContext *dc, int x, int y, std::wstring s, int centrer, int staffSize)
{ 
	assert( dc ); // DC cannot be NULL

    int fontCorr = 0;
    
    int xDC = ToDeviceContextX(x);
    if (dc->CorrectMusicAscent()) {
        fontCorr = m_doc->m_drawingFontHeightAscent[staffSize][0];
    }
    
	if ( centrer )
	{
        LogDebug("Centering string not implemented with DeviceContext");
		
        int w, h;
		dc->GetSmuflTextExtent( s, &w, &h );
		xDC -= w / 2;
        
	}
    dc->SetBrush( m_currentColour, AxSOLID );
    dc->SetFont( &m_doc->m_drawingSmuflFonts[staffSize][0] );
    
	dc->DrawMusicText( s, xDC, ToDeviceContextY(y + fontCorr ));
    
    dc->ResetFont();
    dc->ResetBrush();
}

void View::DrawLyricString ( DeviceContext *dc, int x, int y, std::wstring s, int staffSize)
{
    assert( dc ); // DC cannot be NULL
    
    dc->StartText( ToDeviceContextX( x ), ToDeviceContextY( y ) );
    
    std::wistringstream iss( s  );
    std::wstring token;
    while( std::getline( iss, token, L'_' ))
    {
        dc->DrawText( UTF16to8( token.c_str() ), token );
        // no _
        if (iss.eof())
            break;
        
        FontInfo vrvTxt;
        vrvTxt.SetFaceName("VerovioText");
        vrvTxt.SetPointSize( m_doc->m_drawingLyricFonts[staffSize].GetPointSize());
        
        dc->SetFont( &vrvTxt );
        dc->VrvTextFont();
        std::wstring str;
        str.push_back(VRV_TEXT_E551);
        dc->DrawText( UTF16to8( str.c_str() ), str );
        dc->ResetFont();
    }
    //std::wcout << std::endl;
    
    dc->EndText( );
}

void View::DrawTieOrSlurBezier(DeviceContext *dc, int x, int y, int x1, int y1, bool direction)
{
    int height = std::max( MIN_TIE_HEIGHT * DEFINITON_FACTOR, std::min( 5 * m_doc->m_drawingDoubleUnit[0] / 3, abs( x1 - x ) / 4 ) );
    
    int thickness = std::max( m_doc->m_drawingDoubleUnit[0] / 3, MIN_TIE_THICKNESS * DEFINITON_FACTOR );
    
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
    dc->SetPen( m_currentColour, std::max( 1,  m_doc->m_style->m_stemWidth / 2 ), AxSOLID );
    dc->DrawComplexBezierPath(ToDeviceContextX(x), ToDeviceContextY(y), bez1, bez2);
    dc->ResetPen();
}

} // namespace vrv
