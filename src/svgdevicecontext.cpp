/////////////////////////////////////////////////////////////////////////////
// Name:        svgdevicecontext.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "svgdevicecontext.h"

//----------------------------------------------------------------------------

#include <fstream>

//----------------------------------------------------------------------------

#include "doc.h"
//#include "leipzigbbox.h"
#include "glyph.h"
#include "view.h"
#include "vrvdef.h"

//----------------------------------------------------------------------------

#include <math.h>

namespace vrv {

#define space " "
#define semicolon ";"
 
//#include "app/axapp.h"

extern "C" {
static inline double DegToRad(double deg) { return (deg * M_PI) / 180.0; }
//static inline double RadToDeg(double deg) { return (deg * 180.0) / M_PI; } // unused
}

//----------------------------------------------------------------------------
// SvgDeviceContext
//----------------------------------------------------------------------------


SvgDeviceContext::SvgDeviceContext(int width, int height):
    DeviceContext()
{	
	
    m_correctMusicAscent = false; // do not correct the ascent in the Leipzig font

    m_width = width;
    m_height = height;
    
    m_userScaleX = 1.0;
    m_userScaleY = 1.0;
    m_originX = 0;
    m_originY = 0;
    
    SetBrush( AxBLACK, AxSOLID );
    SetPen( AxBLACK, 1, AxSOLID );

    m_graphics = 0;
    m_indents = 1;
    
    m_leipzig_glyphs.clear();
    
    m_committed = false;
    
    m_svg.str("");
    m_svg.clear();
    
    m_outdata.clear();
}


SvgDeviceContext::~SvgDeviceContext ( )
{
}


bool SvgDeviceContext::CopyFileToStream(const std::string& filename, std::ostream& dest)
{
    std::ifstream source( filename.c_str(), std::ios::binary );
    dest << source.rdbuf();
    source.close();
    return true;
}




void SvgDeviceContext::Commit( bool xml_tag ) {

    if (m_committed) {
        return;
    }
    
    int i;
    // close unclosed graphics, just in case
    for (i = m_graphics; i < 0; m_graphics-- ) {
        WriteLine("/*- SvgDeviceContext::Flush - Unclosed graphic */");
        WriteLine("</g>");
        m_indents--;
    }
    m_indents = 0;
    WriteLine("</svg>\n") ;

    // header
    std::string s;
    if ( xml_tag ) {
        s = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
    }
    
    s += StringFormat ( "<svg width=\"%dpx\" height=\"%dpx\"", (int)((double)m_width * m_userScaleX), (int)((double)m_height * m_userScaleY));
    s += " version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\"  xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n";
    
    m_outdata << s;
    
    if (m_leipzig_glyphs.size() > 0)
    {
        m_outdata << "\t<defs>\n";
        
        std::vector<std::string>::const_iterator it;
        for(it = m_leipzig_glyphs.begin(); it != m_leipzig_glyphs.end(); ++it)
        {
            m_outdata << "\t\t";
            CopyFileToStream( (*it), m_outdata );
        }
        m_outdata << "\t</defs>\n";
    }
    // finally concatenate the svg
    m_outdata << m_svg.str();
    m_committed = true;
}


void SvgDeviceContext::WriteLine( std::string string )
{
    std::string output;
    output.append( m_indents, '\t' );
    output += string + "\n"; 
    m_svg << output;
}


void SvgDeviceContext::StartGraphic( DocObject *object, std::string gClass, std::string gId )
{
    WriteLine(StringFormat("<g class=\"%s\" id=\"%s\" style=\"%s %s %s %s\">", gClass.c_str(), gId.c_str(), m_penColour.c_str(), m_penStyle.c_str(),
        m_brushColour.c_str(), m_brushStyle.c_str() ) );
    m_graphics++;
    m_indents++;
}
  
      
void SvgDeviceContext::EndGraphic(DocObject *object, View *view ) 
{
 
    bool drawBoundingBox = false;
    if (drawBoundingBox) //(view) // && view->DrawBoundingBoxes()) // DrawBoundingBoxes is not defined
    {
        SetPen( AxRED, 1, AxDOT_DASH );
        SetBrush( AxWHITE, AxTRANSPARENT );
        StartGraphic( object, "bounding-box", "0");
        if ( object->HasSelfBB() ) {
            this->DrawRectangle( view->ToDeviceContextX( object->GetDrawingX() + object->m_selfBB_x1 ),
                                view->ToDeviceContextY( object->GetDrawingY() + object->m_selfBB_y1 ),
                                view->ToDeviceContextX( object->GetDrawingX() + object->m_selfBB_x2 ) -
                                view->ToDeviceContextX( object->GetDrawingX() + object->m_selfBB_x1 ),
                                view->ToDeviceContextY( object->GetDrawingY() + object->m_selfBB_y2 ) -
                                view->ToDeviceContextY( object->GetDrawingY() + object->m_selfBB_y1 ));
        }
        EndGraphic( object, NULL );
        
        SetPen( AxBLUE, 1, AxDOT_DASH );
        StartGraphic( object, "bounding-box", "0");
        if ( object->HasContentBB() ) {
            this->DrawRectangle( view->ToDeviceContextX( object->GetDrawingX() + object->m_contentBB_x1 ),
                                view->ToDeviceContextY( object->GetDrawingY() + object->m_contentBB_y1 ),
                                view->ToDeviceContextX( object->GetDrawingX() + object->m_contentBB_x2 ) -
                                view->ToDeviceContextX( object->GetDrawingX() + object->m_contentBB_x1 ),
                                view->ToDeviceContextY( object->GetDrawingY() + object->m_contentBB_y2 ) -
                                view->ToDeviceContextY( object->GetDrawingY() + object->m_contentBB_y1 ));
        }
        EndGraphic( object, NULL );
        
        SetPen( AxBLACK, 1 );
        SetBrush(AxBLACK, AxSOLID);
   
    }
    
    m_graphics--;
    m_indents--;
    WriteLine("</g>");
}


void SvgDeviceContext::StartPage( )
{
    // a graphic for scaling
    WriteLine(StringFormat("<g class=\"page-scale\" transform=\"scale(%f, %f)\">", m_userScaleX, m_userScaleY ) );
    m_graphics++;
    m_indents++;
    // a graphic for the origin
    WriteLine(StringFormat("<g class=\"page-margin\" transform=\"translate(%d, %d)\">", (int)((double)m_originX), (int)((double)m_originY) ) );
    m_graphics++;
    m_indents++;
}
 
       
void SvgDeviceContext::EndPage() 
{
    // end page-margin
    m_graphics--;
    m_indents--;
    WriteLine("</g>");
    // end page-scale
    m_graphics--;
    m_indents--;
    WriteLine("</g>");
}

        
void SvgDeviceContext::SetBrush( int colour, int style )
{
    m_brushColour = "fill:#" + GetColour(colour) + semicolon;
    switch ( style )
    {
        case AxSOLID :
            m_brushStyle = "fill-opacity:1.0; ";
            break ;
        case AxTRANSPARENT:
            m_brushStyle = "fill-opacity:0.0; ";
            break ;
        default :
            m_brushStyle = "fill-opacity:1.0; "; // solid brush as default
    }
}
        
void SvgDeviceContext::SetBackground( int colour, int style )
{
    // nothing to do, we do not handle Background
}

void SvgDeviceContext::SetBackgroundImage( void *image, double opacity )
{

}
        
void SvgDeviceContext::SetBackgroundMode( int mode )
{
    // nothing to do, we do not handle Background Mode
}
        
void SvgDeviceContext::SetPen( int colour, int width, int style )
{
    m_penColour = "stroke:#" + GetColour(colour)  + semicolon;
    m_penWidth = "stroke-width:" + StringFormat("%d", width) + semicolon;
    switch ( style )
    {
        case AxSOLID :
            m_penStyle = "stroke-opacity:1.0; ";
            break ;
        case AxTRANSPARENT:
            m_penStyle = "stroke-opacity:0.0; ";
            break ;
        default :
            m_penStyle = "stroke-opacity:1.0; "; // solid brush as default
    }
}
        
void SvgDeviceContext::SetFont( FontMetricsInfo *font_info )
{
    m_font = *font_info;
    //wxFont font( font_info->pointSize, (wxFontFamily)font_info->family, font_info->style,
    //    (wxFontWeight)font_info->weight, font_info->underlined, font_info->faceName,
    //    (wxFontEncoding)font_info->encoding );
    //m_dc->SetFont( font );
}
            

void SvgDeviceContext::SetTextForeground( int colour )
{
    m_brushColour = "fill:#" + GetColour(colour); // we use the brush colour for text
}
        
void SvgDeviceContext::SetTextBackground( int colour )
{
    // nothing to do, we do not handle Text Background Mode
}
       
void SvgDeviceContext::ResetBrush( )
{
    SetBrush( AxBLACK, AxSOLID );
}
        
void SvgDeviceContext::ResetPen( )
{
    SetPen( AxBLACK, 1, AxSOLID );
} 

void SvgDeviceContext::SetLogicalOrigin( int x, int y ) 
{
    m_originX = -x;
    m_originY = -y;
} 

void SvgDeviceContext::SetUserScale( double xScale, double yScale ) 
{
    m_userScaleX = xScale;
    m_userScaleY = yScale;
}       

void SvgDeviceContext::GetTextExtent( const std::string& string, int *w, int *h )
{
    LogDebug("SvgDeviceContext::GetTextExtent not implemented");
}
    
// Copied from bBoxDc, TODO find another more generic solution
void SvgDeviceContext::GetSmuflTextExtent( const std::wstring& string, int *w, int *h )
{
    double x, y, partial_w, partial_h;
    *w = 0;
    *h = 0;
    
    for (unsigned int i = 0; i < string.length(); i++)
    {
        wchar_t c = string[i];
        Glyph *glyph = Resources::GetGlyph(c);
        if (!glyph) {
            continue;
        }
        glyph->GetBoundingBox(&x, &y, &partial_w, &partial_h);
    
        partial_w *= ((m_font.GetPointSize() / glyph->GetUnitsPerEm()));
        partial_h *= ((m_font.GetPointSize() / glyph->GetUnitsPerEm()));
        
        *w += (int)partial_w;
        *h += (int)partial_h;
    }
}
       

MusPoint SvgDeviceContext::GetLogicalOrigin( ) 
{
    return MusPoint( m_originX, m_originY );
}



// Drawing mething
void SvgDeviceContext::DrawComplexBezierPath(int x, int y, int bezier1_coord[6], int bezier2_coord[6])
{
    WriteLine( StringFormat("<path d=\"M%d,%d C%d,%d %d,%d %d,%d C%d,%d %d,%d %d,%d\" style=\"fill:#000; fill-opacity:1.0; stroke:#000000; stroke-linecap:round; stroke-linejoin:round; stroke-opacity:1.0; stroke-width:1\" />", 
                                x, y, // M command
                                bezier1_coord[0], bezier1_coord[1], bezier1_coord[2], bezier1_coord[3], bezier1_coord[4], bezier1_coord[5], // First bezier
                                bezier2_coord[0], bezier2_coord[1], bezier2_coord[2], bezier2_coord[3], bezier2_coord[4], bezier2_coord[5] // Second Bezier
                                ) );
}

void SvgDeviceContext::DrawCircle(int x, int y, int radius)
{
    DrawEllipse(x - radius, y - radius, 2*radius, 2*radius);
}


void SvgDeviceContext::DrawEllipse(int x, int y, int width, int height)
{
    int rh = height / 2;
    int rw = width  / 2;

    WriteLine(StringFormat("<ellipse cx=\"%d\" cy=\"%d\" rx=\"%d\" ry=\"%d\" />", x+rw,y+rh, rw, rh ));
}

        
void SvgDeviceContext::DrawEllipticArc(int x, int y, int width, int height, double start, double end)
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

    // this version closes the arc
    //s.Printf ( "<path d=\"M%d %d A%d %d 0.0 %d %d  %d %d L %d %d z ",
    //    int(xs), int(ys), int(rx), int(ry),
    //    fArc, fSweep, int(xe), int(ye), int(xc), int(yc)  );

    WriteLine( StringFormat("<path d=\"M%d %d A%d %d 0.0 %d %d  %d %d \" />",
        int(xs), int(ys), abs(int(rx)), abs(int(ry)),
        fArc, fSweep, int(xe), int(ye) ) );
}
  
              
void SvgDeviceContext::DrawLine(int x1, int y1, int x2, int y2)
{
    WriteLine( StringFormat("<path d=\"M%d %d L%d %d\" style=\"%s\" />", x1,y1,x2,y2, m_penWidth.c_str()) );
}
 
               
void SvgDeviceContext::DrawPolygon(int n, MusPoint points[], int xoffset, int yoffset, int fill_style)
{

    std::string s ;
    s = "<polygon style=\"";
    //if ( fillStyle == wxODDEVEN_RULE )
    //    s = s + "fill-rule:evenodd; ";
    //else
    s += "fill-rule:nonzero; ";

    s += "\" points=\"" ;

    for (int i = 0; i < n;  i++)
    {
        s += StringFormat("%d,%d ", points [i].x+xoffset, points[i].y+yoffset );
        //CalcBoundingBox ( points [i].x+xoffset, points[i].y+yoffset);
    }
    s += "\" /> " ;
    WriteLine(s);
}
    
            
void SvgDeviceContext::DrawRectangle(int x, int y, int width, int height)
{
    DrawRoundedRectangle( x, y, width, height, 0 );
}


void SvgDeviceContext::DrawRoundedRectangle(int x, int y, int width, int height, double radius)
{

    std::string s ;
    
    // negative heights or widths are not allowed in SVG
    if ( height < 0 ) {
        height = -height;
        y -= height;
    }
     if ( width < 0 ) {
        width = -width;
        x -= width;
    }   

    WriteLine ( StringFormat(" <rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" rx=\"%.2g\" />", x, y, width, height, radius ) );

}

        
void SvgDeviceContext::DrawText(const std::string& text, int x, int y, char alignement)
{
    
    std::string s;
    std::string anchor;
    
    if ( alignement == RIGHT ) {
        anchor = " text-anchor=\"end\"";
    }
    if ( alignement == CENTER ) {
        anchor = " text-anchor=\"middle\"";
    }
    
    s = StringFormat(" <text x=\"%d\" y=\"%d\" dx=\"%d\" dy=\"%d\" style=\"font-family: Garamond, Georgia, serif; font-size: 36px;\" %s>", x, y, 0, 0, anchor.c_str()) ;
    s = s + text + "</text> " ;
    WriteLine(s);
}



void SvgDeviceContext::DrawRotatedText(const std::string& text, int x, int y, double angle)
{
    //known bug; if the font is drawn in a scaled DC, it will not behave exactly as wxMSW

    std::string s;

    // calculate bounding box
    int w, h, desc;
    //DoGetTextExtent(sText, &w, &h, &desc);
    w = h = desc = 0;

    //double rad = DegToRad(angle);

    
    //if (m_backgroundMode == AxSOLID)
    //{
    //    WriteLine("/*- SvgDeviceContext::DrawRotatedText - Backgound not implemented */") ;
    //    WriteLine( text ) ;
    //}
    s = StringFormat(" <text x=\"%d\" y=\"%d\" dx=\"%d\" dy=\"%d\" style=\"font-family: Garamond, Georgia, serif; font-size: 36px;\">", x, y, 0, 0) ;

    // For some reason, some browsers (e.g., Chrome) do not like spaces or dots in font names...
    /*
    sTmp.Replace(" ", "");
    sTmp.Replace(".", "");
    if (sTmp.Len () > 0)  s = s + "style=\"font-family: '" + sTmp + "'; ";
    else s = s + "style=\" " ;

    std::string fontweights [3] = { "normal", "lighter", "bold" };
    s = s + "font-weight:" + fontweights[m_font.GetWeight() - wxNORMAL] + semicolon + space;

    std::string fontstyles [5] = { "normal", "style error", "style error", "italic", "oblique" };
    s = s + "font-style:" + fontstyles[m_font.GetStyle() - wxNORMAL] + semicolon  + space;

    sTmp.Printf ("font-size:%dpt; ", (int)((double)m_font.GetPointSize() * 1) );
    s = s + sTmp ;
    // remove the color information because normaly already in the graphic element
    //s = s + "fill:#" + wxColStr (m_textForegroundColour) + "; stroke:#" + wxColStr (m_textForegroundColour) + "; " ;
    sTmp.Printf ( "stroke-width:0;\"  transform=\"rotate( %.2g %d %d )  \" >",  -angle, x,y ) ;
    */
    s = s + text + "</text> " ;
    WriteLine(s);
}

std::string FilenameLookup(unsigned char c) {
    std::string glyph;
    return glyph;
}

void SvgDeviceContext::DrawMusicText(const std::wstring& text, int x, int y)
{

    int w, h, gx, gy;
        
    // print chars one by one
    for (unsigned int i = 0; i < text.length(); i++)
    {
        wchar_t c = text[i];
        Glyph *glyph = Resources::GetGlyph(c);
        if (!glyph)
        {
            continue;
        }
        
        std::string path = glyph->GetPath();
        
        // Add the glyph to the array for the <defs>
        std::vector<std::string>::const_iterator it = std::find(m_leipzig_glyphs.begin(), m_leipzig_glyphs.end(), path);
        if (it == m_leipzig_glyphs.end())
        {
            m_leipzig_glyphs.push_back( path );
        }
        
        
        // Write the char in the SVG
        WriteLine ( StringFormat( "<use xlink:href=\"#%s\" x=\"%d\" y=\"%d\" height=\"%dpx\" width=\"%dpx\"></use>",
            glyph->GetCodeStr().c_str(), x, y, m_font.GetPointSize(), m_font.GetPointSize() )  );
        
        // Get the bounds of the char
        //LeipzigBBox::GetCharBounds(c, &gx, &gy, &w, &h);
        // Sum it to x so we move it to the start of the next char
        // FIXME
        //x += (w * ((double)(m_font.GetPointSize() / LEIPZIG_UNITS_PER_EM)));

    }
}


void SvgDeviceContext::DrawSpline(int n, MusPoint points[])
{

}

void SvgDeviceContext::DrawBackgroundImage( int x, int y )
{
   
}


std::string SvgDeviceContext::GetColour( int colour )
{
    std::stringstream ss;
    ss << std::hex;

    switch ( colour )
    {
    case (AxBLACK): return "000000";
    case (AxWHITE): return "FFFFFF";
    case (AxRED): return "FF0000";
    case (AxGREEN): return "00FF00";
    case (AxBLUE): return "0000FF";
    case (AxCYAN): return "00FFFF";
    case (AxLIGHT_GREY): return "777777";
    default:
        int blue =  (colour & 255);
        int green = (colour >> 8) & 255;
        int red = (colour >> 16) & 255;
        ss << red << green << blue;
        //std::strin = wxDecToHex(char(red)) + wxDecToHex(char(green)) + wxDecToHex(char(blue)) ;  // ax3
        return ss.str();
    }
}

std::string SvgDeviceContext::GetStringSVG( bool xml_tag )
{
    if (!m_committed)
        Commit( xml_tag );
    
    return m_outdata.str();
}

} // namespace vrv