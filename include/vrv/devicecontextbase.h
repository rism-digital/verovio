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
#include "vrv.h"

namespace vrv {

#define AxNONE -1
#define AxWHITE 255 << 16 | 255 << 8 | 255
#define AxBLACK 0
#define AxRED 255 << 16
#define AxBLUE 255
#define AxGREEN 255 << 8
#define AxCYAN 255 << 8 | 255
#define AxLIGHT_GREY 127 << 16 | 127 << 8 | 127

/*  Polygon filling mode */
enum { AxODDEVEN_RULE = 1, AxWINDING_RULE };

enum {
    /*  Pen styles */
    AxSOLID = 100,
    AxDOT,
    AxLONG_DASH,
    AxSHORT_DASH,
    AxDOT_DASH,
    AxUSER_DASH,
    AxTRANSPARENT
};

// ---------------------------------------------------------------------------
// Pen/Brush
// ---------------------------------------------------------------------------

/**
 * These classes are used for storing drawing style parameters during SVG and
 * bounding box engraving.
 */

class Pen {
public:
    Pen() : m_penColour(0), m_penWidth(0), m_dashLength(0), m_lineCap(0), m_penOpacity(0.0) {}
    Pen(int colour, int width, float opacity, int dashLength, int lineCap)
        : m_penColour(colour), m_penWidth(width), m_dashLength(dashLength), m_lineCap(lineCap), m_penOpacity(opacity)
    {
    }

    int GetColour() const { return m_penColour; }
    void SetColour(int colour) { m_penColour = colour; }
    int GetWidth() const { return m_penWidth; }
    void SetWidth(int width) { m_penWidth = width; }
    int GetDashLength() const { return m_dashLength; }
    void SetDashLength(int dashLength) { m_dashLength = dashLength; }
    int GetLineCap() const { return m_lineCap; }
    void SetLineCap(int lineCap) { m_lineCap = lineCap; }
    float GetOpacity() const { return m_penOpacity; }
    void SetOpacity(float opacity) { m_penOpacity = opacity; }

private:
    int m_penColour, m_penWidth, m_dashLength, m_lineCap;
    float m_penOpacity;
};

class Brush {
public:
    Brush() : m_brushColour(0), m_brushOpacity(0.0) {}
    Brush(int colour, float opacity) : m_brushColour(colour), m_brushOpacity(opacity) {}

    int GetColour() const { return m_brushColour; }
    void SetColour(int colour) { m_brushColour = colour; }
    float GetOpacity() const { return m_brushOpacity; }
    void SetOpacity(float opacity) { m_brushOpacity = opacity; }

private:
    int m_brushColour;
    float m_brushOpacity;
};

// ---------------------------------------------------------------------------
// FontInfo
// ---------------------------------------------------------------------------

/**
 * This class is store font properties.
 */

class FontInfo {
public:
    FontInfo()
    {
        m_pointSize = 0;
        m_family = 0; // was wxFONTFAMILY_DEFAULT;
        m_style = FONTSTYLE_NONE;
        m_weight = FONTWEIGHT_NONE;
        m_underlined = false;
        m_supSubScript = false;
        m_faceName.clear();
        m_encoding = 0; // was wxFONTENCODING_DEFAULT;
        m_widthToHeightRatio = 1.0;
    }
    virtual ~FontInfo(){};

    // accessors and modifiers for the font elements
    int GetPointSize() { return m_pointSize; }
    data_FONTSTYLE GetStyle() { return m_style; }
    data_FONTWEIGHT GetWeight() { return m_weight; }
    bool GetUnderlined() { return m_underlined; }
    bool GetSupSubScript() { return m_supSubScript; }
    std::string GetFaceName() { return m_faceName; }
    int GetFamily() { return m_family; }
    int GetEncoding() { return m_encoding; }
    float GetWidthToHeightRatio() { return m_widthToHeightRatio; }

    void SetPointSize(int pointSize) { m_pointSize = pointSize; }
    void SetStyle(data_FONTSTYLE style) { m_style = style; }
    void SetWeight(data_FONTWEIGHT weight) { m_weight = weight; }
    void SetUnderlined(bool underlined) { m_underlined = underlined; }
    void SetSupSubScript(bool supSubScript) { m_supSubScript = supSubScript; }
    void SetFaceName(const char *faceName) { m_faceName = faceName; }
    void SetFamily(int family) { m_family = family; }
    void SetEncoding(int encoding) { m_encoding = encoding; }
    void SetWidthToHeightRatio(float ratio) { m_widthToHeightRatio = ratio; }

private:
    int m_pointSize;
    int m_family;
    data_FONTSTYLE m_style;
    data_FONTWEIGHT m_weight;
    bool m_underlined;
    bool m_supSubScript;
    std::string m_faceName;
    int m_encoding;
    float m_widthToHeightRatio;
};

// ---------------------------------------------------------------------------
// Point
// ---------------------------------------------------------------------------

/**
 * Simple class for representing points
 */

class Point {
public:
    int x, y;

    Point() : x(0), y(0) {}
    Point(int xx, int yy) : x(xx), y(yy) {}

    // no copy ctor or assignment operator - the defaults are ok

    // comparison
    bool operator==(const Point &p) const { return x == p.x && y == p.y; }
    bool operator!=(const Point &p) const { return !(*this == p); }

    // arithmetic operations (component wise)
    Point operator+(const Point &p) const { return Point(x + p.x, y + p.y); }
    Point operator-(const Point &p) const { return Point(x - p.x, y - p.y); }

    Point &operator+=(const Point &p)
    {
        x += p.x;
        y += p.y;
        return *this;
    }
    Point &operator-=(const Point &p)
    {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    Point operator-() const { return Point(-x, -y); }
};

// ---------------------------------------------------------------------------
// TextExtend
// ---------------------------------------------------------------------------

/**
 * Simple class for representing text extends
 */

class TextExtend {
public:
    int m_width, m_height, m_leftBearing, m_ascent, m_descent, m_advX;

    TextExtend() : m_width(0), m_height(0), m_leftBearing(0), m_ascent(0), m_descent(0), m_advX(0) {}

    // no copy ctor or assignment operator - the defaults are ok
};

} // namespace vrv

#endif // __AX_DC_BASE_H__
