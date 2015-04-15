/////////////////////////////////////////////////////////////////////////////
// Name:        svgdevicecontext.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "svgdevicecontext.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "doc.h"
#include "glyph.h"
#include "view.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

#define space " "
#define semicolon ";"

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
	
    m_correctMusicAscent = false; // do not correct the ascent in the music font

    m_width = width;
    m_height = height;
    
    m_userScaleX = 1.0;
    m_userScaleY = 1.0;
    m_originX = 0;
    m_originY = 0;
    
    SetBrush( AxBLACK, AxSOLID );
    SetPen( AxBLACK, 1, AxSOLID );
    
    m_smufl_glyphs.clear();
    
    m_committed = false;
    
    //create the initial SVG element
    //width and height need to be set later; these are taken care of in "commit"
    m_svgNode = m_svgDoc.append_child("svg");
    m_svgNode.append_attribute( "version" ) = "1.1";
    m_svgNode.append_attribute( "xmlns" ) = "http://www.w3.org/2000/svg";
    m_svgNode.append_attribute( "xmlns:xlink" ) = "http://www.w3.org/1999/xlink";
    m_svgNode.append_attribute( "overflow" ) = "visible";
    
    //start the stack
    m_svgNodeStack.push_back(m_svgNode);
    m_currentNode = m_svgNode;
    
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




void SvgDeviceContext::Commit( bool xml_declaration ) {
    
    if (m_committed) {
        return;
    }
    
    // take care of width/height once userScale is updated
    m_svgNode.prepend_attribute( "height" ) = StringFormat("%dpx", (int)((double)m_height * m_userScaleY)).c_str();
    m_svgNode.prepend_attribute( "width" ) = StringFormat("%dpx", (int)((double)m_width * m_userScaleX)).c_str();
    
    // add the woff VerovioText font in needed
    if (m_vrvTextFont) {
        std::string woff = Resources::GetPath() + "/woff.xml";
        pugi::xml_document woffDoc;
        woffDoc.load_file(woff.c_str());
        m_svgNode.prepend_copy( woffDoc.first_child() );
    }

    // header
    if (m_smufl_glyphs.size() > 0)
    {
        
        pugi::xml_node defs = m_svgNode.prepend_child( "defs" );
        pugi::xml_document sourceDoc;
        
        //for each needed glyph
        std::vector<std::string>::const_iterator it;
        for(it = m_smufl_glyphs.begin(); it != m_smufl_glyphs.end(); ++it)
        {
            //load the XML file that contains it as a pugi::xml_document
            std::ifstream source( (*it).c_str() );
            sourceDoc.load(source);
            
            //copy all the nodes inside into the master document
            for (pugi::xml_node child = sourceDoc.first_child(); child; child = child.next_sibling())
            {
                defs.append_copy(child);
            }
        }
    }
    
    unsigned int output_flags = pugi::format_default | pugi::format_no_declaration;
    if (xml_declaration) {
        //edit the xml declaration
        output_flags = pugi::format_default;
        pugi::xml_node decl = m_svgDoc.prepend_child(pugi::node_declaration);
        decl.append_attribute("version") = "1.0";
        decl.append_attribute("encoding") = "UTF-8";
        decl.append_attribute("standalone") = "no";
    }
    
    // save the glyph data to m_outdata
    m_svgDoc.save(m_outdata, "\t", output_flags);
    
    m_committed = true;
}


void SvgDeviceContext::StartGraphic( DocObject *object, std::string gClass, std::string gId )
{
    //Pen currentPen = m_penStack.top();
    //Brush currentBrush = m_brushStack.top();
    
    std::string baseClass = object->GetClassName();
    std::transform( baseClass.begin(), baseClass.begin() + 1, baseClass.begin(), ::tolower );
    if (gClass.length() > 0) {
        baseClass.append(" " + gClass);
    }
    
    m_currentNode = m_currentNode.append_child("g");
    m_svgNodeStack.push_back(m_currentNode);
    m_currentNode.append_attribute( "class" ) = baseClass.c_str();
    m_currentNode.append_attribute( "id" ) = gId.c_str();
    //m_currentNode.append_attribute( "style" ) = StringFormat("stroke: #%s; stroke-opacity: %f; fill: #%s; fill-opacity: %f;", GetColour(currentPen.GetColour()).c_str(), currentPen.GetOpacity(), GetColour(currentBrush.GetColour()).c_str(), currentBrush.GetOpacity()).c_str();
}
    
void SvgDeviceContext::ResumeGraphic( DocObject *object, std::string gId )
{
    std::string xpath = "//g[@id=\"" + gId + "\"]";
    pugi::xpath_node selection = m_currentNode.select_single_node( xpath.c_str() );
    if ( selection ) {
        m_currentNode = selection.node();
    }
    m_svgNodeStack.push_back(m_currentNode);
}
  
      
void SvgDeviceContext::EndGraphic(DocObject *object, View *view ) 
{
 
    bool drawBoundingBox = false;
    if (drawBoundingBox && view) // && view->DrawBoundingBoxes()) // DrawBoundingBoxes is not defined
    {
        SetPen( AxRED, 10, AxDOT_DASH );
        SetBrush( AxWHITE, AxTRANSPARENT );
        StartGraphic( object, "self-bounding-box", "0");
        if ( object->HasSelfBB() ) {
            this->DrawRectangle( view->ToDeviceContextX( object->GetDrawingX() + object->m_selfBB_x1 ),
                                view->ToDeviceContextY( object->GetDrawingY() + object->m_selfBB_y1 ),
                                view->ToDeviceContextX( object->GetDrawingX() + object->m_selfBB_x2 ) -
                                view->ToDeviceContextX( object->GetDrawingX() + object->m_selfBB_x1 ),
                                view->ToDeviceContextY( object->GetDrawingY() + object->m_selfBB_y2 ) -
                                view->ToDeviceContextY( object->GetDrawingY() + object->m_selfBB_y1 ));
        }
        EndGraphic( object, NULL );
        
        SetPen( AxBLUE, 10, AxDOT_DASH );
        StartGraphic( object, "content-bounding-box", "0");
        if ( object->HasContentBB() ) {
            this->DrawRectangle( view->ToDeviceContextX( object->GetDrawingX() + object->m_contentBB_x1 ),
                                view->ToDeviceContextY( object->GetDrawingY() + object->m_contentBB_y1 ),
                                view->ToDeviceContextX( object->GetDrawingX() + object->m_contentBB_x2 ) -
                                view->ToDeviceContextX( object->GetDrawingX() + object->m_contentBB_x1 ),
                                view->ToDeviceContextY( object->GetDrawingY() + object->m_contentBB_y2 ) -
                                view->ToDeviceContextY( object->GetDrawingY() + object->m_contentBB_y1 ));
        }
        EndGraphic( object, NULL );
        
        SetPen( AxBLACK, 1, AxSOLID);
        SetBrush(AxBLACK, AxSOLID);
   
    }

    m_svgNodeStack.pop_back();
    m_currentNode = m_svgNodeStack.back();
}
    
void SvgDeviceContext::EndResumedGraphic(DocObject *object, View *view )
{
    m_svgNodeStack.pop_back();
    m_currentNode = m_svgNodeStack.back();
}

void SvgDeviceContext::StartPage( )
{
    // Initialize the flag to false because we want to know if the font needs to be included in the SVG
    m_vrvTextFont = false;
    
    // a graphic for definition scaling
    m_currentNode = m_currentNode.append_child("svg");
    m_svgNodeStack.push_back(m_currentNode);
    m_currentNode.append_attribute("id") = "definition-scale";
    m_currentNode.append_attribute("viewBox") = StringFormat("0 0 %d %d",
        m_width * DEFINITON_FACTOR, m_height * DEFINITON_FACTOR).c_str();

    // a graphic for the origin
    m_currentNode = m_currentNode.append_child("g");
    m_svgNodeStack.push_back(m_currentNode);
    m_currentNode.append_attribute("class") = "page-margin";
    m_currentNode.append_attribute("transform") = StringFormat("translate(%d, %d)", (int)((double)m_originX), (int)((double)m_originY)).c_str();
    m_currentNode.append_attribute( "style" ) = "stroke: #000; stroke-opacity: 1.0; fill: #000; fill-opacity: 1.0";

}
 
       
void SvgDeviceContext::EndPage() 
{
    // end page-margin
    m_svgNodeStack.pop_back();
    // end definition-scale
    m_svgNodeStack.pop_back();
    // end page-scale
    //m_svgNodeStack.pop_back();
    m_currentNode = m_svgNodeStack.back();
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

void SvgDeviceContext::SetTextForeground( int colour )
{
    m_brushStack.top().SetColour(colour); // we use the brush colour for text
}
        
void SvgDeviceContext::SetTextBackground( int colour )
{
    // nothing to do, we do not handle Text Background Mode
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

Point SvgDeviceContext::GetLogicalOrigin( ) 
{
    return Point( m_originX, m_originY );
}

// Drawing mething
void SvgDeviceContext::DrawComplexBezierPath(int x, int y, int bezier1_coord[6], int bezier2_coord[6])
{
    pugi::xml_node pathChild = m_currentNode.append_child("path");
    pathChild.append_attribute("d") = StringFormat("M%d,%d C%d,%d %d,%d %d,%d C%d,%d %d,%d %d,%d", x, y, // M command
       bezier1_coord[0], bezier1_coord[1], bezier1_coord[2], bezier1_coord[3], bezier1_coord[4], bezier1_coord[5], // First bezier
       bezier2_coord[0], bezier2_coord[1], bezier2_coord[2], bezier2_coord[3], bezier2_coord[4], bezier2_coord[5] // Second Bezier
       ).c_str();
    //pathChild.append_attribute("style") = StringFormat("fill:#000; fill-opacity:1.0; stroke:#000000; stroke-linecap:round; stroke-linejoin:round; stroke-opacity:1.0; stroke-width: %d", m_penStack.top().GetWidth() ).c_str();
    // without colour
    pathChild.append_attribute("style") = StringFormat("fill-opacity:1.0; stroke-linecap:round; stroke-linejoin:round; stroke-opacity:1.0; stroke-width: %d", m_penStack.top().GetWidth() ).c_str();
}

void SvgDeviceContext::DrawCircle(int x, int y, int radius)
{
    DrawEllipse(x - radius, y - radius, 2*radius, 2*radius);
}


void SvgDeviceContext::DrawEllipse(int x, int y, int width, int height)
{
    assert( m_penStack.size() );
    assert( m_brushStack.size() );
    
    Pen currentPen = m_penStack.top();
    Brush currentBrush = m_brushStack.top();
    
    int rh = height / 2;
    int rw = width  / 2;
    
    pugi::xml_node ellipseChild = m_currentNode.append_child("ellipse");
    ellipseChild.append_attribute("cx") = x+rw;
    ellipseChild.append_attribute("cy") = y+rh;
    ellipseChild.append_attribute("rx") = rw;
    ellipseChild.append_attribute("ry") = rh;
    
    //ellipseChild.append_attribute( "style" ) = StringFormat("stroke: #%s; stroke-opacity: %f; stroke-width: %d; fill: #%s; fill-opacity: %f;", GetColour(currentPen.GetColour()).c_str(), currentPen.GetOpacity(), currentPen.GetWidth(),
    //    GetColour(currentBrush.GetColour()).c_str(), currentBrush.GetOpacity()).c_str();
    // without colour
    ellipseChild.append_attribute( "style" ) = StringFormat("stroke-opacity: %f; stroke-width: %d; fill-opacity: %f;", currentPen.GetOpacity(), currentPen.GetWidth(), currentBrush.GetOpacity()).c_str();
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
    
    assert( m_penStack.size() );
    assert( m_brushStack.size() );
    
    Pen currentPen = m_penStack.top();
    Brush currentBrush = m_brushStack.top();

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

    int fArc  ;
    // flag for large or small arc 0 means less than 180 degrees
    if ( (theta2 - theta1) > 0 ) fArc = 1; else fArc = 0 ;

    int fSweep ;
    if ( fabs(theta2 - theta1) > M_PI) fSweep = 1; else fSweep = 0 ;

    pugi::xml_node pathChild = m_currentNode.append_child("path");
    pathChild.append_attribute("d") = StringFormat("M%d %d A%d %d 0.0 %d %d %d %d",int(xs), int(ys), abs(int(rx)), abs(int(ry)), fArc, fSweep, int(xe), int(ye)).c_str();
    //pathChild.append_attribute( "style" ) = StringFormat("stroke: #%s; stroke-opacity: %f; stroke-width: %d; fill: #%s; fill-opacity: %f;", GetColour(currentPen.GetColour()).c_str(), currentPen.GetOpacity(), currentPen.GetWidth(),
    //                                                        GetColour(currentBrush.GetColour()).c_str(), currentBrush.GetOpacity()).c_str();
    // without colour
    pathChild.append_attribute( "style" ) = StringFormat("stroke-opacity: %f; stroke-width: %d; fill-opacity: %f;", currentPen.GetOpacity(), currentPen.GetWidth(), currentBrush.GetOpacity()).c_str();
}
  
              
void SvgDeviceContext::DrawLine(int x1, int y1, int x2, int y2)
{
    pugi::xml_node pathChild = m_currentNode.append_child("path");
    pathChild.append_attribute("d") = StringFormat("M%d %d L%d %d", x1,y1,x2,y2).c_str();
    pathChild.append_attribute("style") = StringFormat("stroke-width: %d;", m_penStack.top().GetWidth()).c_str();
}
 
               
void SvgDeviceContext::DrawPolygon(int n, Point points[], int xoffset, int yoffset, int fill_style)
{
    assert( m_penStack.size() );
    
    pugi::xml_node polygonChild = m_currentNode.append_child("polygon");
    //if ( fillStyle == wxODDEVEN_RULE )
    //    polygonChild.append_attribute("style") = "fill-rule:evenodd;";
    //else
    polygonChild.append_attribute("style") = "fill-rule:nonzero;";
    polygonChild.append_attribute("stroke-width") = StringFormat("%d", m_penStack.top().GetWidth()).c_str();
    
    std::string pointsString;
    for (int i = 0; i < n;  i++)
    {
        pointsString += StringFormat("%d,%d ", points [i].x+xoffset, points[i].y+yoffset );
    }
    polygonChild.append_attribute("points") = pointsString.c_str();
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

    pugi::xml_node rectChild = m_currentNode.append_child( "rect" );
    rectChild.append_attribute( "x" ) = x;
    rectChild.append_attribute( "y" ) = y;
    rectChild.append_attribute( "width" ) = width;
    rectChild.append_attribute( "height" ) = height;
    rectChild.append_attribute( "rx" ) = radius;
    rectChild.append_attribute( "style") = StringFormat("stroke-width: %d;", m_penStack.top().GetWidth()).c_str();
}

void SvgDeviceContext::StartText(int x, int y, char alignement)
{
    std::string s;
    std::string anchor;
    
    if ( alignement == RIGHT ) {
        anchor = "end";
    }
    if ( alignement == CENTER ) {
        anchor = "middle";
    }
    
    m_currentNode = m_currentNode.append_child( "text" );
    m_svgNodeStack.push_back(m_currentNode);
    m_currentNode.append_attribute( "x" ) = x;
    m_currentNode.append_attribute( "y" ) = y;
    m_currentNode.append_attribute( "dx" ) = 0;
    m_currentNode.append_attribute( "dy" ) = 0;
    if ( !anchor.empty() ) {
        m_currentNode.append_attribute( "text-anchor" ) = anchor.c_str();
    }
    // font-size seems to be required in <text> in FireFox - however, we set it to 0px because otherwise we
    // end up with spaces between tspan because of the linebreaks in the SVG. Needs to be investigated
    m_currentNode.append_attribute("font-size") = StringFormat("0px").c_str();
}
    
void SvgDeviceContext::EndText()
{
    m_svgNodeStack.pop_back();
    m_currentNode = m_svgNodeStack.back();
}
        
void SvgDeviceContext::DrawText(const std::string& text, const std::wstring wtext)
{
    assert( m_fontStack.top() );
    
    pugi::xml_node textChild = m_currentNode.append_child( "tspan" );
    if ( !m_fontStack.top()->GetFaceName().empty() ) {
        textChild.append_attribute( "font-family" ) = m_fontStack.top()->GetFaceName().c_str();
    }
    if ( m_fontStack.top()->GetPointSize() != 0 ) {
        textChild.append_attribute("font-size") = StringFormat("%dpx", m_fontStack.top()->GetPointSize() ).c_str();
    }
    if ( m_fontStack.top()->GetStyle() != FONTWEIGHT_NONE ) {
        if ( m_fontStack.top()->GetStyle() == FONTSTYLE_italic ) {
            textChild.append_attribute("font-style") = "italic";
        }
        else if ( m_fontStack.top()->GetStyle() == FONTSTYLE_normal ) {
            textChild.append_attribute("font-style") = "normal";
        }
        else if ( m_fontStack.top()->GetStyle() == FONTSTYLE_oblique ) {
            textChild.append_attribute("font-style") = "oblique";
        }
    }
    textChild.append_child(pugi::node_pcdata).set_value(text.c_str());
}



void SvgDeviceContext::DrawRotatedText(const std::string& text, int x, int y, double angle)
{
    // TODO
}

std::string FilenameLookup(unsigned char c) {
    std::string glyph;
    return glyph;
}

void SvgDeviceContext::DrawMusicText(const std::wstring& text, int x, int y)
{
    assert( m_fontStack.top() );

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
        std::vector<std::string>::const_iterator it = std::find(m_smufl_glyphs.begin(), m_smufl_glyphs.end(), path);
        if (it == m_smufl_glyphs.end())
        {
            m_smufl_glyphs.push_back( path );
        }
        
        
        // Write the char in the SVG
        pugi::xml_node useChild = m_currentNode.append_child( "use" );
        useChild.append_attribute( "xlink:href" ) = StringFormat("#%s", glyph->GetCodeStr().c_str()).c_str();
        useChild.append_attribute( "x" ) = x;
        useChild.append_attribute( "y" ) = y;
        useChild.append_attribute( "height" ) = StringFormat("%dpx", m_fontStack.top()->GetPointSize()).c_str();
        useChild.append_attribute( "width" ) = StringFormat("%dpx", m_fontStack.top()->GetPointSize()).c_str();
        
        // Get the bounds of the char
        glyph->GetBoundingBox(&gx, &gy, &w, &h);
        x += w * m_fontStack.top()->GetPointSize() / glyph->GetUnitsPerEm();
    }
}


void SvgDeviceContext::DrawSpline(int n, Point points[])
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

std::string SvgDeviceContext::GetStringSVG( bool xml_declaration )
{
    if (!m_committed)
        Commit( xml_declaration );
    
    return m_outdata.str();
}

} // namespace vrv
