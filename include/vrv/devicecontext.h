/////////////////////////////////////////////////////////////////////////////
// Name:        devicecontext.h 
// Author:      Laurent Pugin
// Created:     2010
// Copyright (c) Authors and others. All rights reserved.   
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_DC_H__
#define __VRV_DC_H__

#include <string>
#include <stack>

//----------------------------------------------------------------------------

#include "devicecontextbase.h"
#include "vrvdef.h"

namespace vrv {

class DocObject;
class View;

// ---------------------------------------------------------------------------
// DeviceContext
// ---------------------------------------------------------------------------

/**
 * This class is an abstract device context.
 * It enables different types of concrete classes to be implemented.
 * For example:
 *  MusWxDC - a wrapper to wxDCs with wxWidgets;
 *  SvgDeviceContext - a non-gui file DC;
 *  MusCairoDC - a wrapper to a Cairo surface;
 * The class uses int-based colour encoding (instead of wxColour in wxDC).
 * It uses FontInfo (instead of wxFont in wxDC).
*/

class DeviceContext
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    DeviceContext () { m_correctMusicAscent = true; m_drawingBoundingBoxes = false; m_isDeactivated = false; };
    virtual ~DeviceContext() {};
    ///@}
    
    /**
     * @name Setters
     * Non virtual methods cannot be overriden and manage the Pen, Brush and FontInfo stacks
     */
    ///@{
    void SetBrush( int colour, int opacity );
    void SetPen( int colour, int width, int opacity );
    void SetFont( FontInfo *font );
    void ResetBrush( );
    void ResetPen( );
    void ResetFont( );
    virtual void SetBackground( int colour, int style = AxSOLID ) = 0;
    virtual void SetBackgroundImage( void *image, double opacity = 1.0 ) = 0;
    virtual void SetBackgroundMode( int mode ) = 0;
    virtual void SetTextForeground( int colour ) = 0;
    virtual void SetTextBackground( int colour ) = 0;
    virtual void SetLogicalOrigin( int x, int y ) = 0;
    ///}

    /**
     * @name Getters for text extend (non virtual)
     */
    ///@{
    virtual void GetTextExtent( const std::string& string, int *w, int *h );
    virtual void GetTextExtent( const std::wstring& string, int *w, int *h );
    virtual void GetSmuflTextExtent( const std::wstring& string, int *w, int *h );
    
    /**
     * @name Getters
     */
    ///@{
    virtual Point GetLogicalOrigin( ) = 0;
    virtual bool CorrectMusicAscent( ) { return m_correctMusicAscent; };
    ///@}

    /**
     * @name Drawing methods
     */
    ///@{
    virtual void DrawComplexBezierPath(int x, int y, int bezier1_coord[6], int bezier2_coord[6]) = 0;
    virtual void DrawCircle(int x, int y, int radius) = 0;
    virtual void DrawEllipse(int x, int y, int width, int height) = 0;
    virtual void DrawEllipticArc(int x, int y, int width, int height, double start, double end) = 0;
    virtual void DrawLine(int x1, int y1, int x2, int y2) = 0;
    virtual void DrawPolygon(int n, Point points[], int xoffset = 0, int yoffset = 0, int fill_style = AxODDEVEN_RULE) = 0;
    virtual void DrawRectangle(int x, int y, int width, int height) = 0;
    virtual void DrawRotatedText(const std::string& text, int x, int y, double angle) = 0;
    virtual void DrawRoundedRectangle(int x, int y, int width, int height, double radius) = 0;
    virtual void DrawText(const std::string& text, const std::wstring wtext = L"") = 0;
    virtual void DrawMusicText(const std::wstring& text, int x, int y) = 0;
    virtual void DrawSpline(int n, Point points[]) = 0;
    virtual void DrawBackgroundImage( int x = 0, int y = 0 ) = 0;
    ///@}
    
    /**
     * Special method for forcing bounding boxes to be updated
     * Used for invisible elements (e.g. <space>) that needs to be take into account in spacing
     */
    virtual void DrawPlaceholder( int x, int y ) {};

    /**
     * @name Method for starting and ending a text
     * Once started, DrawText should be called for actually drawing the text.
     * Font can be changed between called for DrawText
     */
    ///@{
    virtual void StartText(int x, int y, char alignement = LEFT ) = 0;
    virtual void EndText() = 0;
    
    /**
     * @name Temporarily deactivate a graphic
     * This can be used for example for not taking into account the bounding box of parts of the graphic.
     * One example is the connectors in lyrics.
     * This is not a virtual method and cannot be overriden. In only changes a flag. The effect of the flag
     * has to be defined in the child class. It should not be call twice in a row.
     */
    ///@{
    void DeactivateGraphic( );
    void ReactivateGraphic( );
    ///@}
    
    /**
     * @name Method for starting and ending a graphic
     * For example, the method can be used for grouping shapes in <g></g> in SVG
     */
    ///@{
    virtual void StartGraphic( DocObject *object, std::string gClass, std::string gId ) = 0;
    virtual void EndGraphic( DocObject *object, View *view  ) = 0;
    ///@}
    
    /**
     * @name Methods for re-starting and ending a graphic for objects drawn in separate steps
     * The methods can be used to the output together, for example for a Beam
     */
    ///@{
    virtual void ResumeGraphic( DocObject *object, std::string gId ) = 0;
    virtual void EndResumedGraphic( DocObject *object, View *view  ) = 0;
    ///@}
 
    /**
     * @name Method for starting and ending page
     */
    ///@{
    virtual void StartPage( ) = 0;
    virtual void EndPage( ) = 0;
    ///@}
    
    /** Colour conversion method **/
    static int RGB2Int( char red, char green, char blue ) { return (red << 16 | green << 8 | blue); };

    /**
     * @name Getter and setter for drawing bounding box option (debug)
     */
    ///@{
    virtual void SetDrawBoundingBoxes(bool b) {m_drawingBoundingBoxes = b;};
    virtual bool GetDrawBoundingBoxes() {return m_drawingBoundingBoxes;};
    ///@}
    
    /**
     * Change the flag for indicating the use of the VerovioText font
     */
    void VrvTextFont() { m_vrvTextFont = true; };
    
protected:
    
    bool m_correctMusicAscent; // specify if the ascent has to be correct when view the music font (true wxDC, false SVG)
    bool m_drawingBoundingBoxes;
    /**
     * Flag for indicating if the VerovioText font is currently used.
     * If used, it has to be initialized to false (e.g., in the overriden version of StartPage) and will be changed in DeviceContext::VrvTextFont
     */
    bool m_vrvTextFont;
    
    
    std::stack<Pen> m_penStack;
    std::stack<Brush> m_brushStack;
    std::stack<FontInfo*> m_fontStack;
    
    /** flag for indicating if the graphic is deactivated */
    bool m_isDeactivated;
    
};
    
} // namespace vrv

#endif // __AX_DC_H__
