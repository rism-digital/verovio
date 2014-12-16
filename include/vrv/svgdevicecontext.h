/////////////////////////////////////////////////////////////////////////////
// Name:        svgdevicecontext.h 
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.   
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_SVG_DC_H__
#define __VRV_SVG_DC_H__

#include <iostream>
#include <fstream>
#include <list>
#include <sstream>
#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "devicecontext.h"
#include "pugixml.hpp"

namespace vrv {

//----------------------------------------------------------------------------
// BBoxDeviceContext
//----------------------------------------------------------------------------

/** 
 * This class implements a drawing context for generating SVG files.
 * The music font is embedded by incorporating ./data/[fontname]/[glyph].xml glyphs within
 * the SVG file.
 */
class SvgDeviceContext: public DeviceContext
{
public:

    SvgDeviceContext ( int width, int height );
    virtual ~SvgDeviceContext();
    
    // Setters
    
    virtual void SetBrush( int colour, int style = AxSOLID );
    
    virtual void SetBackground( int colour, int style = AxSOLID );
    
    virtual void SetBackgroundImage( void *image, double opacity = 1.0 );
    
    virtual void SetBackgroundMode( int mode );
    
    virtual void SetPen( int colour, int width = 1, int style = AxSOLID );
    
    virtual void SetFont( FontMetricsInfo *font_info );
        
    virtual void SetTextForeground( int colour );
    
    virtual void SetTextBackground( int colour );
    
    virtual void ResetBrush( );
    
    virtual void ResetPen( );
    
    virtual void SetLogicalOrigin( int x, int y );
    
    virtual void SetUserScale( double xScale, double yScale );
    
    // Getters
    
    virtual void GetTextExtent( const std::string& string, int *w, int *h );
    
    virtual void GetSmuflTextExtent( const std::wstring& string, int *w, int *h );
    
    virtual MusPoint GetLogicalOrigin( );

    /**
     * Get the the SVG into a string
     * Add the xml tag if necessary.
     */
    std::string GetStringSVG( bool xml_tag = false );
    
    // Drawing methods
    
    virtual void DrawComplexBezierPath(int x, int y, int bezier1_coord[6], int bezier2_coord[6]);
    
    virtual void DrawCircle(int x, int y, int radius);
    
    virtual void DrawEllipse(int x, int y, int width, int height);
    
    virtual void DrawEllipticArc(int x, int y, int width, int height, double start, double end);
    
    virtual void DrawLine(int x1, int y1, int x2, int y2);
    
    virtual void DrawPolygon(int n, MusPoint points[], int xoffset, int yoffset, int fill_style = AxODDEVEN_RULE);
    
    virtual void DrawRectangle(int x, int y, int width, int height);
    
    virtual void DrawRotatedText(const std::string& text, int x, int y, double angle);
    
    virtual void DrawRoundedRectangle(int x, int y, int width, int height, double radius);
    
    virtual void DrawText(const std::string& text, int x, int y, char alignement = LEFT );
    
    virtual void DrawMusicText(const std::wstring& text, int x, int y);
    
    virtual void DrawSpline(int n, MusPoint points[]);
    
    virtual void DrawBackgroundImage( int x = 0, int y = 0 );
    
    // 
    virtual void StartGraphic( DocObject *object, std::string gClass, std::string gId );
    
    virtual void EndGraphic( DocObject *object, View *view  );
    
    virtual void StartPage();
    
    virtual void EndPage();
    
private:
    
    /**
     * Copy the content of a file to the output stream.
     * This is used for copying <defs> items.
     */
    bool CopyFileToStream(const std::string& filename, std::ostream& dest);
    
    // we use a std::stringstream because we want to prepend the <defs> which will know only when we reach the end of the page
    // some viewer seem to support to have the <defs> at the end, but some do not (pdf2svg, for example)
    // for this reason, the full svg is finally written a string from the destructor or when Flush() is called
    std::stringstream m_outdata;
    
    // the std::stringstream we are writing the svg (without <defs>)
    std::stringstream m_svg;
    
    bool m_committed; // did we flushed the file?
    int m_width, m_height;
    int m_originX, m_originY;
    double m_userScaleX, m_userScaleY;
    FontMetricsInfo m_font;
      
    // holds the list of glyphs from the smufl font used so far
    // they will be added at the end of the file as <defs>
    std::vector<std::string> m_smufl_glyphs;
    
    /**
     * Flush the data to the internal buffer.
     * Adds the xml tag if necessary and the <defs> from m_smufl_glyphs
     */
    void Commit( bool xml_tag );
    
    void WriteLine( std::string );
    
    //
    std::string m_brushColour;
    std::string m_brushStyle;
    std::string m_penColour;
    std::string m_penWidth;
    std::string m_penStyle;
    
    std::string GetColour( int colour );
    
    //pugixml data
    pugi::xml_document m_svgDoc;
    pugi::xml_node m_svgNode;
    pugi::xml_node m_currentNode;
    std::list<pugi::xml_node> m_svgNodeStack;
    
};

} // namespace vrv

#endif // __VRV_SVG_DC_H__
