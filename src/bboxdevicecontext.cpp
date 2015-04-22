/////////////////////////////////////////////////////////////////////////////
// Name:        bboxdevicecontext.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "bboxdevicecontext.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "glyph.h"
#include "view.h"
#include "vrv.h"

namespace vrv {

extern "C" {
static inline double DegToRad(double deg) { return (deg * M_PI) / 180.0; }
//static inline double RadToDeg(double deg) { return (deg * 180.0) / M_PI; }
}

//----------------------------------------------------------------------------
// BBoxDeviceContext
//----------------------------------------------------------------------------


BBoxDeviceContext::BBoxDeviceContext ( View *view, int width, int height):
    DeviceContext()
{	
    m_correctMusicAscent = false; // do not correct the ascent in the music font    
    
    m_view = view;
    m_width = width;
    m_height = height;
    
    m_userScaleX = 1.0;
    m_userScaleY = 1.0;
    
    m_drawingText = false;
    
    SetBrush( AxBLACK, AxSOLID );
    SetPen( AxBLACK, 1, AxSOLID );
}


BBoxDeviceContext::~BBoxDeviceContext ( )
{
}

void BBoxDeviceContext::StartGraphic( DocObject *object, std::string gClass, std::string gId )
{
    // add object
    object->ResetBB();
    m_objects.push_back( object );
}
    
void BBoxDeviceContext::ResumeGraphic( DocObject *object, std::string gId )
{
    // add object
    object->ResetBB();
    m_objects.push_back( object );
}
      
void BBoxDeviceContext::EndGraphic(DocObject *object, View *view ) 
{
    // detach the object
    assert( m_objects.back() == object );
    m_objects.pop_back();
}
    
void BBoxDeviceContext::EndResumedGraphic(DocObject *object, View *view )
{
    // detach the object
    assert( m_objects.back() == object );
    m_objects.pop_back();
}

void BBoxDeviceContext::StartPage( )
{
}
 
void BBoxDeviceContext::EndPage() 
{
}
        
void BBoxDeviceContext::SetBackground( int colour, int style )
{
    // nothing to do, we do not handle Background
}
        
void BBoxDeviceContext::SetBackgroundMode( int mode )
{
    // nothing to do, we do not handle Background Mode
}

void BBoxDeviceContext::SetTextForeground( int colour )
{
}
        
void BBoxDeviceContext::SetTextBackground( int colour )
{
    // nothing to do, we do not handle Text Background Mode
}

void BBoxDeviceContext::SetLogicalOrigin( int x, int y ) 
{
} 

void BBoxDeviceContext::SetUserScale( double xScale, double yScale ) 
{
    //// no idea how to handle this with the BB
    m_userScaleX = xScale;
    m_userScaleY = yScale;
}

Point BBoxDeviceContext::GetLogicalOrigin( ) 
{
    return Point( 0, 0 );
}

// claculated better
void BBoxDeviceContext::DrawComplexBezierPath(int x, int y, int bezier1_coord[6], int bezier2_coord[6])
{
    int vals[4];
    FindPointsForBounds( Point(x, y), 
                        Point(bezier1_coord[0], bezier1_coord[1]), 
                        Point(bezier1_coord[2], bezier1_coord[3]),
                        Point(bezier1_coord[4], bezier1_coord[5]),
                        vals);
    
    UpdateBB(vals[0], vals[1], vals[2], vals[3]);
}

void BBoxDeviceContext::DrawCircle(int x, int y, int radius)
{
    DrawEllipse(x - radius, y - radius, 2*radius, 2*radius);
}


void BBoxDeviceContext::DrawEllipse(int x, int y, int width, int height)
{
    UpdateBB(x, y, x + width, y + height);
}

        
void BBoxDeviceContext::DrawEllipticArc(int x, int y, int width, int height, double start, double end)
{
    /*
    Draws an arc of an ellipse. The current pen is used for drawing the arc
    and the current brush is used for drawing the pie. This function is
    currently only available for X window and PostScript device contexts.

    x and y specify the x and y coordinates of the upper-left corner of the
    rectangle that contains the ellipse.

    width and height specify the width and height of the rectangle that
    contains the ellipse.

    start and end specify the start and end of the arc relative to the
    three-o'clock position from the center of the rectangle. Angles are
    specified in degrees (360 is a complete circle). Positive values mean
    counter-clockwise motion. If start is equal to end, a complete ellipse
    will be drawn. */

    //known bug: SVG draws with the current pen along the radii, but this does not happen in wxMSW

    std::string s ;
    //radius
    double rx = width / 2 ;
    double ry = height / 2 ;
    // center
    double xc = x + rx ;
    double yc = y + ry ;

    double xs, ys, xe, ye ;
    xs = xc + rx * cos (DegToRad(start)) ;
    xe = xc + rx * cos (DegToRad(end)) ;
    ys = yc - ry * sin (DegToRad(start)) ;
    ye = yc - ry * sin (DegToRad(end)) ;

    ///now same as circle arc...

    double theta1 = atan2(ys-yc, xs-xc);
    double theta2 = atan2(ye-yc, xe-xc);

    int fArc  ;                  // flag for large or small arc 0 means less than 180 degrees
    if ( (theta2 - theta1) > 0 ) fArc = 1; else fArc = 0 ;

    int fSweep ;
    if ( fabs(theta2 - theta1) > M_PI) fSweep = 1; else fSweep = 0 ;

    //WriteLine( StringFormat("<path d=\"M%d %d A%d %d 0.0 %d %d  %d %d \" />",
    //    int(xs), int(ys), int(rx), int(ry),
    //    fArc, fSweep, int(xe), int(ye) ) );
    
    int penWidth = m_penStack.top().GetWidth();
    if ( penWidth % 2 ) {
        penWidth += 1;
    }
    // needs to be fixed - for now uses the entire rectangle
    UpdateBB( x - penWidth / 2, y - penWidth / 2, x + width + penWidth / 2, y + height + penWidth / 2);
}
  
              
void BBoxDeviceContext::DrawLine(int x1, int y1, int x2, int y2)
{
    if ( x1 > x2 ) {
        int tmp = x1;
        x1 = x2;
        x2 = tmp;
    }
    if ( y1 > y2 ) {
        int tmp = y1;
        y1 = y2;
        y2 = tmp;
    }
    
    int penWidth = m_penStack.top().GetWidth();
    int p1 = penWidth / 2;
    int p2 = p1;
    // how odd line width is handled might depend on the implementation of the device context.
    // however, we expect the actualy with to be shifted on the left/top
    // e.g. with 7, 4 on the left and 3 on the right
    if ( penWidth % 2 ) {
        p1++;
    }
    
    UpdateBB( x1 - p1, y1 - p1, x2 + p2, y2 + p2);
}
 
               
void BBoxDeviceContext::DrawPolygon(int n, Point points[], int xoffset, int yoffset, int fill_style)
{
    if ( n == 0 ) {
        return;
    }
    int x1 = points[0].x + xoffset;
    int x2 = x1;
    int y1 = points[0].y + yoffset;
    int y2 = y1;
    
    for (int i = 0; i < n;  i++)
    {
        if ( points[i].x + xoffset < x1 ) x1 = points[i].x + xoffset;
        if ( points[i].x + xoffset > x2 ) x2 = points[i].x + xoffset;
        if ( points[i].y + yoffset < y1 ) y1 = points[i].y + yoffset;
        if ( points[i].y + yoffset > y2 ) y2 = points[i].y + yoffset;

    }
    UpdateBB(x1, y1, x2, y2);
}
    
            
void BBoxDeviceContext::DrawRectangle(int x, int y, int width, int height)
{
    DrawRoundedRectangle( x, y, width, height, 0 );
}


void BBoxDeviceContext::DrawRoundedRectangle(int x, int y, int width, int height, double radius)
{
    // avoid negative heights or widths
    if ( height < 0 ) {
        height = -height;
        y -= height;
    }
    if ( width < 0 ) {
        width = -width;
        x -= width;
    }
    int penWidth = m_penStack.top().GetWidth();;
    if ( penWidth % 2 ) {
        penWidth += 1;
    }
    
    UpdateBB( x - penWidth / 2, y - penWidth / 2, x + width + penWidth / 2, y + height + penWidth / 2);
}
    
void BBoxDeviceContext::DrawPlaceholder( int x, int y )
{
    UpdateBB( x, y, x, y );
}
    
void BBoxDeviceContext::StartText(int x, int y, char alignement)
{
    assert( !m_drawingText );
    m_drawingText = true;
    m_textX = x;
    m_textY = y;
    m_textWidth = 0;
    m_textHeight = 0;
}

void BBoxDeviceContext::EndText()
{
    m_drawingText = false;
}
        
void BBoxDeviceContext::DrawText(const std::string& text, const std::wstring wtext)
{
    assert( m_fontStack.top() );
    
    //unsigned long length = wtext.length();
    int w, h;
    GetTextExtent(wtext, &w, &h);
    m_textWidth += w;
    // very approximative, we should use GetTextExtend once implemented
    //m_textWidth += length * m_fontStack.top()->GetPointSize() / 7;
    // ignore y bounding boxes for text
    //m_textHeight = m_fontStack.top()->GetPointSize();
        UpdateBB( m_textX, m_textY, m_textX + m_textWidth, m_textY + m_textHeight);
    
    
}

void BBoxDeviceContext::DrawRotatedText(const std::string& text, int x, int y, double angle)
{
    // TODO
}


void BBoxDeviceContext::DrawMusicText(const std::wstring& text, int x, int y)
{  
    assert( m_fontStack.top() );
    
    int g_x, g_y, g_w, g_h;
    int lastCharWidth = 0;
    
    for (unsigned int i = 0; i < text.length(); i++)
    {
        wchar_t c = text[i];
        Glyph *glyph = Resources::GetGlyph(c);
        if (!glyph) {
            continue;
        }
        glyph->GetBoundingBox(&g_x, &g_y, &g_w, &g_h);
    
        int x_off = x + g_x * m_fontStack.top()->GetPointSize() / glyph->GetUnitsPerEm();
        // because we are in the drawing context, y position are already flipped
        int y_off = y - g_y * m_fontStack.top()->GetPointSize() / glyph->GetUnitsPerEm();
         
        UpdateBB(x_off, y_off, 
                  x_off + g_w * m_fontStack.top()->GetPointSize() / glyph->GetUnitsPerEm(),
        // idem, y position are flipped
                  y_off - g_h * m_fontStack.top()->GetPointSize() / glyph->GetUnitsPerEm());
        
        lastCharWidth = g_w * m_fontStack.top()->GetPointSize() / glyph->GetUnitsPerEm();
        x += lastCharWidth; // move x to next char
     
    }
}

void BBoxDeviceContext::DrawSpline(int n, Point points[])
{

}

void BBoxDeviceContext::UpdateBB(int x1, int y1, int x2, int y2)
{
    if (m_isDeactivated) {
        return;
    }
    
    // the array should not be empty
    assert( !m_objects.empty() ); // Array cannot be empty
    
    
    // we need to store logical coordinates in the objects, we need to convert them back (this is why we need a View object)
    (m_objects.back())->UpdateSelfBB( m_view->ToLogicalX(x1), m_view->ToLogicalY(y1), m_view->ToLogicalX(x2), m_view->ToLogicalY(y2) );
    
    int i;
    // Stretch the content BB of the other objects
    for (i = 0; i < (int)m_objects.size(); i++) {
        (m_objects[i])->UpdateContentBB( m_view->ToLogicalX(x1), m_view->ToLogicalY(y1), m_view->ToLogicalX(x2), m_view->ToLogicalY(y2) );
    }
}

// Ok, shame on me, found off the internet and modified, but for now it works
void BBoxDeviceContext::FindPointsForBounds(Point P0, Point P1, Point P2, Point P3, int *ret)
{
    
    int A = P3.x - 3 * P2.x + 3 * P1.x - P0.x;
    int B = 3 * P2.x - 6 * P1.x + 3 * P0.x;
    int C = 3 * P1.x - 3 * P0.x;
    int D = P0.x;
    
    int E = P3.y - 3 * P2.y + 3 * P1.y - P0.y;
    int F = 3 * P2.y - 6 * P1.y + 3 * P0.y;
    int G = 3 * P1.y - 3 * P0.y;
    int H = P0.y;
    
    float x, y;
    float xMin = 0xFFFF;
    float yMin = 0xFFFF;
    float xMax = 0;
    float yMax = 0;
    
    for (float t = 0.0f; t <= 1.0f; t += 0.01f)
    {
        x = A * t * t * t + B * t * t + C * t + D;
        if (x < xMin)
            xMin = x;
        if (x > xMax)
            xMax = x;
        y = E * t * t * t + F * t * t + G * t + H;
        if (y < yMin)
            yMin = y;
        if (y > yMax)
            yMax = y; 
    }
    
    ret[0] = (int)xMin;
    ret[1] = (int)yMin;
    
    ret[2] = (int)xMax;
    ret[3] = (int)yMax;
}

} // namespace vrv
