/////////////////////////////////////////////////////////////////////////////
// Name:        devicecontextbase.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.   
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_DC_BASE_H__
#define __VRV_DC_BASE_H__

#include <string>

//----------------------------------------------------------------------------

#include "attdef.h"

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
    
// ---------------------------------------------------------------------------
// Pen/Brush
// ---------------------------------------------------------------------------

/**
 * These classes are used for storing drawing style parameters during SVG and 
 * bounding box engraving.
 */

class Pen
{
public:
    Pen()
        : m_penColour(0), m_penWidth(0), m_penOpacity(0.0)  { }
    Pen(int colour, int width, float opacity)
        : m_penColour(colour), m_penWidth(width), m_penOpacity(opacity) { }
    
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

class Brush
{
public:
    Brush()
        : m_brushColour(0), m_brushOpacity(0.0) { }
    Brush(int colour, float opacity)
        : m_brushColour(colour), m_brushOpacity(opacity) { }
    
    int GetColour() const { return m_brushColour; }
    void SetColour(int colour) { m_brushColour = colour; }
    float GetOpacity() const { return m_brushOpacity; }
    void SetOpacity(float opacity) { m_brushOpacity = opacity; }
    
public:
    int m_brushColour;
    float m_brushOpacity;
};

// ---------------------------------------------------------------------------
// FontInfo
// ---------------------------------------------------------------------------

/**
 * This class is store font properties.
 */

class FontInfo
{
public:
    FontInfo ()
    {
        pointSize = 0;
        family = 0; //was wxFONTFAMILY_DEFAULT;
        style = FONTSTYLE_NONE;
        weight = FONTWEIGHT_NONE;
        underlined = false;
        faceName.clear();
        encoding = 0; //was wxFONTENCODING_DEFAULT;
    }
    virtual ~FontInfo() {};
    
    // accessors and modifiers for the font elements
    int GetPointSize() { return pointSize; }
    int GetStyle() { return style; };
    int GetWeight() { return weight; }
    bool GetUnderlined() { return underlined; }
    std::string GetFaceName() { return faceName; }
    int GetFamily() { return family; }
    int GetEncoding() { return encoding; }
    
    void SetPointSize(int pointSize_) { pointSize = pointSize_; }
    void SetStyle(data_FONTSTYLE style_) { style = style_; }
    void SetWeight(data_FONTWEIGHT weight_) { weight = weight_; }
    void SetUnderlined(bool underlined_) { underlined = underlined_; }
    void SetFaceName(const char *faceName_) { faceName = faceName_; }
    void SetFamily(int family_) { family = family_; }
    void SetEncoding(int encoding_) { encoding = encoding_; }
    
    int           pointSize;
    int           family;
    data_FONTSTYLE style;
    data_FONTWEIGHT weight;
    bool          underlined;
    std::string   faceName;
    int           encoding;
};

// ---------------------------------------------------------------------------
// Point
// ---------------------------------------------------------------------------

/**
 * Simple class for representing points
 */

class Point
{
public:
    int x, y;
    
    Point() : x(0), y(0) { }
    Point(int xx, int yy) : x(xx), y(yy) { }
    
    // no copy ctor or assignment operator - the defaults are ok
    
    // comparison
    bool operator==(const Point& p) const { return x == p.x && y == p.y; }
    bool operator!=(const Point& p) const { return !(*this == p); }
    
    // arithmetic operations (component wise)
    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
    
    Point& operator+=(const Point& p) { x += p.x; y += p.y; return *this; }
    Point& operator-=(const Point& p) { x -= p.x; y -= p.y; return *this; }
    
    Point operator-() const { return Point(-x, -y); }
};
    
} // namespace vrv

#endif // __AX_DC_BASE_H__
