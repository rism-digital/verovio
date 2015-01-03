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
#include <assert.h>

namespace vrv {

#define AxWHITE  255 << 16 | 255 << 8 | 255
#define AxBLACK 0
#define AxRED  255 << 16
#define AxBLUE 255
#define AxGREEN  255 << 8
#define AxCYAN 255 << 8 | 255
#define AxLIGHT_GREY 127 << 16 | 127 << 8 | 127

/*  Polygon filling mode */
enum
{
    AxODDEVEN_RULE = 1,
    AxWINDING_RULE
};

enum
{
    /*  Pen styles */
    AxSOLID      =   100,
    AxDOT,
    AxLONG_DASH,
    AxSHORT_DASH,
    AxDOT_DASH,
    AxUSER_DASH,
    AxTRANSPARENT
};
    
enum
{
    /*  text alignme */
    LEFT = 0,
    RIGHT,
    CENTER
};

class MusPoint;
class MusRect;
class FontMetricsInfo;
class DocObject;
class View;
    
// ---------------------------------------------------------------------------
// Pen/Brush
// ---------------------------------------------------------------------------

/**
 * These classes are used for storing drawing style parameters during SVG and 
 * bounding box engraving.
 */

class Pen{
    
public:
    Pen()
    : m_penColour(0), m_penWidth(0), m_penOpacity(0.0)
    { }
    Pen(int colour, int width, float opacity)
    : m_penColour(colour), m_penWidth(width), m_penOpacity(opacity)
    { }
    
    int GetColour() const { return m_penColour; }
    void SetColour(int colour) { m_penColour = colour; }
    int GetWidth() const { return m_penWidth; }
    void SetWidth (int width) { m_penWidth = width; }
    float GetOpacity() const { return m_penOpacity; }
    void SetOpacity (float opacity) { m_penOpacity = opacity; }
    
public:
    int m_penColour, m_penWidth;
    float m_penOpacity;
    
};

class Brush{
    
public:
    Brush()
    : m_brushColour(0), m_brushOpacity(0.0)
    { }
    Brush(int colour, float opacity)
    : m_brushColour(colour), m_brushOpacity(opacity)
    { }
    
    int GetColour() const { return m_brushColour; }
    void SetColour(int colour) { m_brushColour = colour; }
    float GetOpacity() const { return m_brushOpacity; }
    void SetOpacity(float opacity) { m_brushOpacity = opacity; }
    
public:
    int m_brushColour;
    float m_brushOpacity;
};


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
 * It uses FontMetricsInfo (instead of wxFont in wxDC).
*/

class DeviceContext
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    DeviceContext () { m_correctMusicAscent = true; m_drawingBoundingBoxes = false;};
    virtual ~DeviceContext() {};
    ///@}
    
    /**
     * @name Setters
     * Non virtual methods cannot be overriden and manage the Pen and Brush stacks
     */
    ///@{
    void SetBrush( int colour, int opacity );
    void SetPen( int colour, int width, int opacity );
    void ResetBrush( );
    void ResetPen( );
    virtual void SetBackground( int colour, int style = AxSOLID ) = 0;
    virtual void SetBackgroundImage( void *image, double opacity = 1.0 ) = 0;
    virtual void SetBackgroundMode( int mode ) = 0;
    virtual void SetFont( FontMetricsInfo *font_info ) = 0;
    virtual void SetTextForeground( int colour ) = 0;
    virtual void SetTextBackground( int colour ) = 0;
    virtual void SetLogicalOrigin( int x, int y ) = 0;
    ///}
    
    /**
     * @name Getters
     */
    ///@{
    virtual void GetTextExtent( const std::string& string, int *w, int *h ) = 0;
    virtual void GetSmuflTextExtent( const std::wstring& string, int *w, int *h ) = 0;
    virtual MusPoint GetLogicalOrigin( ) = 0;
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
    virtual void DrawPolygon(int n, MusPoint points[], int xoffset = 0, int yoffset = 0, int fill_style = AxODDEVEN_RULE) = 0;
    virtual void DrawRectangle(int x, int y, int width, int height) = 0;
    virtual void DrawRotatedText(const std::string& text, int x, int y, double angle) = 0;
    virtual void DrawRoundedRectangle(int x, int y, int width, int height, double radius) = 0;
    virtual void DrawText(const std::string& text, int x, int y, char alignement = LEFT ) = 0;
    virtual void DrawMusicText(const std::wstring& text, int x, int y) = 0;
    virtual void DrawSpline(int n, MusPoint points[]) = 0;
    virtual void DrawBackgroundImage( int x = 0, int y = 0 ) = 0;
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
    
protected:
    
    bool m_correctMusicAscent; // specify if the ascent has to be correct when view the music font (true wxDC, false SVG)
    bool m_drawingBoundingBoxes;
    
    std::stack<Pen> m_penStack;
    std::stack<Brush> m_brushStack;
};

// ---------------------------------------------------------------------------
// FontMetricsInfo
// ---------------------------------------------------------------------------

/**
 * This class is store font properties.
 * It is very similar to wxNativeFontInfo, but we need it for non-gui AxDCs 
*/

class FontMetricsInfo
{
public:
    FontMetricsInfo () 
    {
        pointSize = 0;
        family = 0; //was wxFONTFAMILY_DEFAULT;
        style = 0; //was wxFONTSTYLE_NORMAL;
        weight = 0; //was wxFONTWEIGHT_NORMAL;
        underlined = false;
        faceName.clear();
        encoding = 0; //was wxFONTENCODING_DEFAULT;
    }
    virtual ~FontMetricsInfo() {};
    
    // accessors and modifiers for the font elements
    int GetPointSize() { return pointSize; }
    int GetStyle() { return style; };
    int GetWeight() { return weight; }
    bool GetUnderlined() { return underlined; }
    std::string GetFaceName() { return faceName; }
    int GetFamily() { return family; }
    int GetEncoding() { return encoding; }

    void SetPointSize(int pointSize_) { pointSize = pointSize_; }
    void SetStyle(int style_) { style = style_; }
    void SetWeight(int weight_) { weight = weight_; }
    void SetUnderlined(bool underlined_) { underlined = underlined_; }
    void SetFaceName(std::string& faceName_) { faceName = faceName_; }
    void SetFamily(int family_) { family = family_; }
    void SetEncoding(int encoding_) { encoding = encoding_; }
    
    // in axdc.cpp
    bool FromString(const std::string& s);
    std::string ToString() const;
    
    int           pointSize;
    int           family;
    int           style;
    int           weight;
    bool          underlined;
    std::string   faceName;
    int           encoding;
};



// ---------------------------------------------------------------------------
// MusPoint
// ---------------------------------------------------------------------------

/**
 * This class is similar to wxPoint.
 * It is required for non-gui application because wxPoint in not available in
 * wxBase. All operations that use wxSize do not exist.
*/

class MusPoint
{
public:
    int x, y;

    MusPoint() : x(0), y(0) { }
    MusPoint(int xx, int yy) : x(xx), y(yy) { }

    // no copy ctor or assignment operator - the defaults are ok

    // comparison
    bool operator==(const MusPoint& p) const { return x == p.x && y == p.y; }
    bool operator!=(const MusPoint& p) const { return !(*this == p); }

    // arithmetic operations (component wise)
    MusPoint operator+(const MusPoint& p) const { return MusPoint(x + p.x, y + p.y); }
    MusPoint operator-(const MusPoint& p) const { return MusPoint(x - p.x, y - p.y); }

    MusPoint& operator+=(const MusPoint& p) { x += p.x; y += p.y; return *this; }
    MusPoint& operator-=(const MusPoint& p) { x -= p.x; y -= p.y; return *this; }

    //MusPoint& operator+=(const wxSize& s) { x += s.GetWidth(); y += s.GetHeight(); return *this; }
    //MusPoint& operator-=(const wxSize& s) { x -= s.GetWidth(); y -= s.GetHeight(); return *this; }

    //MusPoint operator+(const wxSize& s) const { return MusPoint(x + s.GetWidth(), y + s.GetHeight()); }
    //MusPoint operator-(const wxSize& s) const { return MusPoint(x - s.GetWidth(), y - s.GetHeight()); }

    MusPoint operator-() const { return MusPoint(-x, -y); }
};



// ---------------------------------------------------------------------------
// MusRect
// ---------------------------------------------------------------------------

/**
 * This class is similar to wxRect.
 * It is required for non-gui application because wxPoint in not available in
 * wxBase.
 * All operations that use wxSize do not exists.
 * All operations on rectangles (inflate, etc.) do not exists.
*/

class MusRect
{
public:
    MusRect()
        : x(0), y(0), width(0), height(0)
        { }
    MusRect(int xx, int yy, int ww, int hh)
        : x(xx), y(yy), width(ww), height(hh)
        { }
    MusRect(const MusPoint& topLeft, const MusPoint& bottomRight);
    //MusRect(const MusPoint& pt, const wxSize& size)
    //    : x(pt.x), y(pt.y), width(size.x), height(size.y)
    //    { }
    //MusRect(const wxSize& size)
    //    : x(0), y(0), width(size.x), height(size.y)
    //    { }

    // default copy ctor and assignment operators ok

    int GetX() const { return x; }
    void SetX(int xx) { x = xx; }

    int GetY() const { return y; }
    void SetY(int yy) { y = yy; }

    int GetWidth() const { return width; }
    void SetWidth(int w) { width = w; }

    int GetHeight() const { return height; }
    void SetHeight(int h) { height = h; }

    MusPoint GetPosition() const { return MusPoint(x, y); }
    void SetPosition( const MusPoint &p ) { x = p.x; y = p.y; }

    //wxSize GetSize() const { return wxSize(width, height); }
    //void SetSize( const wxSize &s ) { width = s.GetWidth(); height = s.GetHeight(); }

    bool IsEmpty() const { return (width <= 0) || (height <= 0); }

    int GetLeft()   const { return x; }
    int GetTop()    const { return y; }
    int GetBottom() const { return y + height - 1; }
    int GetRight()  const { return x + width - 1; }

    void SetLeft(int left) { x = left; }
    void SetRight(int right) { width = right - x + 1; }
    void SetTop(int top) { y = top; }
    void SetBottom(int bottom) { height = bottom - y + 1; }

    MusPoint GetTopLeft() const { return GetPosition(); }
    MusPoint GetLeftTop() const { return GetTopLeft(); }
    void SetTopLeft(const MusPoint &p) { SetPosition(p); }
    void SetLeftTop(const MusPoint &p) { SetTopLeft(p); }

    MusPoint GetBottomRight() const { return MusPoint(GetRight(), GetBottom()); }
    MusPoint GetRightBottom() const { return GetBottomRight(); }
    void SetBottomRight(const MusPoint &p) { SetRight(p.x); SetBottom(p.y); }
    void SetRightBottom(const MusPoint &p) { SetBottomRight(p); }

    MusPoint GetTopRight() const { return MusPoint(GetRight(), GetTop()); }
    MusPoint GetRightTop() const { return GetTopRight(); }
    void SetTopRight(const MusPoint &p) { SetRight(p.x); SetTop(p.y); }
    void SetRightTop(const MusPoint &p) { SetTopLeft(p); }

    MusPoint GetBottomLeft() const { return MusPoint(GetLeft(), GetBottom()); }
    MusPoint GetLeftBottom() const { return GetBottomLeft(); }
    void SetBottomLeft(const MusPoint &p) { SetLeft(p.x); SetBottom(p.y); }
    void SetLeftBottom(const MusPoint &p) { SetBottomLeft(p); }

public:
    int x, y, width, height;
};
    
} // namespace vrv

#endif // __AX_DC_H__
