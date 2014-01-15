/////////////////////////////////////////////////////////////////////////////
// Name:        bboxdc.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "bboxdc.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "vrvdef.h"
#include "leipzigbbox.h"
#include "view.h"

namespace vrv {

extern "C" {
static inline double DegToRad(double deg) { return (deg * M_PI) / 180.0; }
static inline double RadToDeg(double deg) { return (deg * 180.0) / M_PI; }
}

//----------------------------------------------------------------------------
// BBoxDeviceContext
//----------------------------------------------------------------------------


BBoxDeviceContext::BBoxDeviceContext ( View *rc, int width, int height):
    DeviceContext()
{	
    m_correctMusicAscent = false; // do not correct the ascent in the Leipzig font    
    
    m_rc = rc;
    m_width = width;
    m_height = height;
    
    m_userScaleX = 1.0;
    m_userScaleY = 1.0;
    m_originX = 0;
    m_originY = 0;
    
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
      
void BBoxDeviceContext::EndGraphic(DocObject *object, View *rc ) 
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

void BBoxDeviceContext::SetBrush( int colour, int style )
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
        
void BBoxDeviceContext::SetPen( int colour, int width, int style )
{
    m_penWidth = width;
}
        
void BBoxDeviceContext::SetFont( FontMetricsInfo *font_info )
{
    m_font = *font_info;
}
            

void BBoxDeviceContext::SetTextForeground( int colour )
{
}
        
void BBoxDeviceContext::SetTextBackground( int colour )
{
    // nothing to do, we do not handle Text Background Mode
}
       
void BBoxDeviceContext::ResetBrush( )
{
}
        
void BBoxDeviceContext::ResetPen( )
{
    SetPen( AxBLACK, 1, AxSOLID );
} 

void BBoxDeviceContext::SetLogicalOrigin( int x, int y ) 
{
    //// no idea how to handle this with the BB
    m_originX = -x;
    m_originY = -y;
} 

void BBoxDeviceContext::SetUserScale( double xScale, double yScale ) 
{
    //// no idea how to handle this with the BB
    m_userScaleX = xScale;
    m_userScaleY = yScale;
}       

void BBoxDeviceContext::GetTextExtent( const std::string& string, int *w, int *h )
{
    int x, y, partial_w, partial_h;
    
    *w = 0;
    *h = 0;
    
    for (unsigned int i = 0; i < string.length(); i++) {
        
        LeipzigBBox::GetCharBounds(string.c_str()[i], &x, &y, &partial_w, &partial_h);
        
        partial_w *= ((m_font.GetPointSize() / LEIPZIG_UNITS_PER_EM));
        partial_h *= ((m_font.GetPointSize() / LEIPZIG_UNITS_PER_EM));
        
        *w += partial_w;
        *h += partial_h;
    }
    
}
       

MusPoint BBoxDeviceContext::GetLogicalOrigin( ) 
{
    return MusPoint( m_originX, m_originY );
}

// claculated better
void BBoxDeviceContext::DrawComplexBezierPath(int x, int y, int bezier1_coord[6], int bezier2_coord[6])
{
    int vals[4];
    FindPointsForBounds( MusPoint(x, y), 
                        MusPoint(bezier1_coord[0], bezier1_coord[1]), 
                        MusPoint(bezier1_coord[2], bezier1_coord[3]),
                        MusPoint(bezier1_coord[4], bezier1_coord[5]),
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
    
    int penWidth = m_penWidth;
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
    int penWidth = m_penWidth;
    if ( penWidth % 2 ) {
        penWidth += 1;
    }
    
    UpdateBB( x1 - penWidth / 2, y1 - penWidth / 2, x2 + penWidth / 2, y2 + penWidth / 2);
}
 
               
void BBoxDeviceContext::DrawPolygon(int n, MusPoint points[], int xoffset, int yoffset, int fill_style)
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
    int penWidth = m_penWidth;
    if ( penWidth % 2 ) {
        penWidth += 1;
    }
    
    UpdateBB( x - penWidth / 2, y - m_penWidth / 2, x + width + m_penWidth / 2, y + height + m_penWidth / 2);
}

        
void BBoxDeviceContext::DrawText(const std::string& text, int x, int y)
{
    DrawMusicText( text, x, y);
}



void BBoxDeviceContext::DrawRotatedText(const std::string& text, int x, int y, double angle)
{
    //known bug; if the font is drawn in a scaled DC, it will not behave exactly as wxMSW

    std::string s;

    // calculate bounding box
    int w, h, desc;
    //DoGetTextExtent(sText, &w, &h, &desc);
    w = h = desc = 0;

    //double rad = DegToRad(angle);

    
    //if (m_backgroundMode == AxSOLID)
    {
        //WriteLine("/*- MusSVGFileDC::DrawRotatedText - Backgound not implemented */") ;
    }
}


void BBoxDeviceContext::DrawMusicText(const std::string& text, int x, int y)
{  
    
    int g_x, g_y, g_w, g_h;
    int lastCharWidth = 0;
    
    for (unsigned int i = 0; i < text.length(); i++) {
        unsigned char c = (unsigned char)text[i];
        
        LeipzigBBox::GetCharBounds(c, &g_x, &g_y, &g_w, &g_h);
    
        int x_off = x + (g_x * ((double)(m_font.GetPointSize() / LEIPZIG_UNITS_PER_EM)) ) - 2;
        // because we are in the rendering context, y position are already flipped
        int y_off = y - (g_y * ((double)(m_font.GetPointSize() / LEIPZIG_UNITS_PER_EM)) ) + 2;
        // the +/- 2 is to compesate a couple pixels down the figure (rounding error?)
         
        UpdateBB(x_off, y_off, 
                  x_off + (g_w * ((double)(m_font.GetPointSize() / LEIPZIG_UNITS_PER_EM)) ),
        // idem, y position are flipped
                  y_off - (g_h * ((double)(m_font.GetPointSize() / LEIPZIG_UNITS_PER_EM)) ));
        
        lastCharWidth = (g_w * ((double)(m_font.GetPointSize() / LEIPZIG_UNITS_PER_EM)));
        x += lastCharWidth; // move x to next char
     
    }
    
    /*
    int x_off = x + (bbox->m_bBox[glyph].m_x * ((double)(m_font.GetPointSize() / LEIPZIG_UNITS_PER_EM)) );
    // because we are in the rendering context, y position are already flipped
    int y_off = y - (bbox->m_bBox[glyph].m_y * ((double)(m_font.GetPointSize() / LEIPZIG_UNITS_PER_EM)) );
    
    UpdateBB(x_off, y_off, 
        x_off + (bbox->m_bBox[glyph].m_width * ((double)(m_font.GetPointSize() / LEIPZIG_UNITS_PER_EM)) ),
        // idem, y position are flipped
        y_off - (bbox->m_bBox[glyph].m_height * ((double)(m_font.GetPointSize() / LEIPZIG_UNITS_PER_EM)) ));
    */
}


void BBoxDeviceContext::DrawSpline(int n, MusPoint points[])
{
    //m_dc->DrawSpline( n, (wxPoint*)points );
}

void BBoxDeviceContext::UpdateBB(int x1, int y1, int x2, int y2) 
{
    /*
    DocObject *first = &m_objects[m_objects.Count() - 1];
    
    first->UpdateOwnBB(x1, y1, x2, y2);
    
    // Stretch the content BB of the other objects
    // Check that we are not the only elem in the list
    if (m_objects.Count() > 1) {
        
        // The second element in the list stretches in base of the new BBox of the first
        m_objects[m_objects.Count() - 2].UpdateContentBB(first->m_selfBB_x1, first->m_selfBB_y1, first->m_selfBB_x2, first->m_selfBB_y2);
        
        // All the next ones, stretch using contentBB
        for (int i = m_objects.Count() - 3; i >= 0; i--) {
            DocObject *precedent = &m_objects[i + 1];
            m_objects[i].UpdateContentBB(precedent->m_contentBB_x1, precedent->m_contentBB_y1, precedent->m_contentBB_x2, precedent->m_contentBB_y2);
        }
    }
    */
    
    // simpler version 
    
    // the array should not be empty
    assert( !m_objects.empty() ); // Array cannot be empty
    
    
    // we need to store logical coordinates in the objects, we need to convert them back (this is why we need a View object)
    ((DocObject*)m_objects.back())->UpdateSelfBB( m_rc->ToLogicalX(x1), m_rc->ToLogicalY(y1), m_rc->ToLogicalX(x2), m_rc->ToLogicalY(y2) );
    
    int i;
    for (i = 0; i < (int)m_objects.size(); i++) {
        ((DocObject*)m_objects[i])->UpdateContentBB( m_rc->ToLogicalX(x1), m_rc->ToLogicalY(y1), m_rc->ToLogicalX(x2), m_rc->ToLogicalY(y2) );
    }
}

// Ok, shame on me, found off the internet and modified, but for now it works
void BBoxDeviceContext::FindPointsForBounds(MusPoint P0, MusPoint P1, MusPoint P2, MusPoint P3, int *ret)
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