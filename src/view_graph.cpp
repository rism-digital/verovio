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
	assert( dc );

    dc->SetPen( m_currentColour, std::max( 1, ToDeviceContextX(nbr) ), AxSOLID );
    dc->SetBrush( m_currentColour, AxSOLID );

	dc->DrawLine( ToDeviceContextX(x1) , ToDeviceContextY(y1), ToDeviceContextX(x1), ToDeviceContextY(y2) );

    dc->ResetPen();
    dc->ResetBrush();
	return;
}

void View::DrawHorizontalLine ( DeviceContext *dc, int x1, int x2, int y1, int nbr)
{		
	assert( dc );

    dc->SetPen( m_currentColour, std::max( 1, ToDeviceContextX(nbr) ), AxSOLID );
    dc->SetBrush( m_currentColour, AxSOLID );

	dc->DrawLine( ToDeviceContextX(x1) , ToDeviceContextY(y1), ToDeviceContextX(x2), ToDeviceContextY(y1) );

    dc->ResetPen();
    dc->ResetBrush();
	return;
}

    /* Draw rectangle partly filled in, as specified by <fillSection>: 1=top, 2=bottom, 3=left side,
        4=right side; 0=don't fill in any part. ??SO FAR, <fillSection> IS IGNORED.
     */
    void View::DrawPartFullRectangle( DeviceContext *dc, int x1, int y1, int x2, int y2, int fillSection )
    {
        assert( dc ); // DC cannot be NULL
        
        SwapY( &y1, &y2 );
        
        //dc->SetPen( m_currentColour, 0, AxSOLID  );
        //dc->SetBrush( AxWHITE, AxTRANSPARENT );
        dc->SetPen( AxBLUE, 0, AxSOLID  );
        dc->SetBrush( AxRED, AxTRANSPARENT );
        
        dc->DrawRectangle( ToDeviceContextX( x1 ), ToDeviceContextY(y1), ToDeviceContextX(x2 - x1) , ToDeviceContextX( y1 - y2 ));
        
        dc->ResetPen();
        dc->ResetBrush();
        
        return;
    }
    

    void View::DrawFullRectangle( DeviceContext *dc, int x1, int y1, int x2, int y2 )	/* dessine rectangle plein */
{	
    assert( dc );

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


void View::DrawDot ( DeviceContext *dc, int x, int y, int staffSize )
{
	int r = std::max( ToDeviceContextX( m_doc->GetDrawingDoubleUnit( staffSize ) / 5 ), 2 );
	
    dc->SetPen( m_currentColour, 1, AxSOLID );
    dc->SetBrush( m_currentColour, AxSOLID );

	dc->DrawCircle( ToDeviceContextX(x) , ToDeviceContextY(y), r );
		
    dc->ResetPen();
    dc->ResetBrush();
}

void View::DrawSmuflCode ( DeviceContext *dc, int x, int y, wchar_t code,
						 int staffSize, bool dimin )
{
    assert( dc );
	
    dc->SetBackground( AxBLUE );
    dc->SetBackgroundMode( AxTRANSPARENT );
    
    std::wstring str;
    str.push_back(code);
    
    dc->SetBrush( m_currentColour, AxSOLID );
    dc->SetFont( m_doc->GetDrawingSmuflFont(staffSize, dimin) );

    dc->DrawMusicText( str, ToDeviceContextX(x), ToDeviceContextY(y) );

    dc->ResetFont();
    dc->ResetBrush();

	return;
}

void View::DrawSmuflString ( DeviceContext *dc, int x, int y, std::wstring s, bool center, int staffSize)
{ 
	assert( dc );
    
    int xDC = ToDeviceContextX(x);
    
	if ( center )
	{
        int w, h;
		dc->GetSmuflTextExtent( s, &w, &h );
		xDC -= w / 2;
        
	}
    dc->SetBrush( m_currentColour, AxSOLID );
    dc->SetFont( m_doc->GetDrawingSmuflFont(staffSize, 0) );
    
	dc->DrawMusicText( s, xDC, ToDeviceContextY(y) );
    
    dc->ResetFont();
    dc->ResetBrush();
}

void View::DrawLyricString ( DeviceContext *dc, int x, int y, std::wstring s, int staffSize)
{
    assert( dc );
    
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
        vrvTxt.SetPointSize( m_doc->GetDrawingLyricFont(staffSize)->GetPointSize() );
        
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
    assert( dc );
    
    int height = std::max( MIN_TIE_HEIGHT * DEFINITON_FACTOR, std::min( 5 * m_doc->GetDrawingDoubleUnit( 100 ) / 3, abs( x1 - x ) / 4 ) );
    
    int thickness = std::max( m_doc->GetDrawingDoubleUnit(100) / 3, MIN_TIE_THICKNESS * DEFINITON_FACTOR );
    
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
    dc->SetPen( m_currentColour, std::max( 1,  m_doc->GetDrawingStemWidth(100) / 2 ), AxSOLID );
    dc->DrawComplexBezierPath(ToDeviceContextX(x), ToDeviceContextY(y), bez1, bez2);
    dc->ResetPen();
}

} // namespace vrv
