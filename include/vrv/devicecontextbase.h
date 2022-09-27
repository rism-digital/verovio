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

class Doc;

#define AxNONE -1
#define AxWHITE 255 << 16 | 255 << 8 | 255
#define AxBLACK 0
#define AxRED 255 << 16
#define AxBLUE 255
#define AxGREEN 255 << 8
#define AxCYAN 255 << 8 | 255
#define AxLIGHT_GREY 127 << 16 | 127 << 8 | 127
#undef max
#undef min

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

enum {
    /* Line cap styles */
    AxCAP_UNKNOWN = 0,
    AxCAP_BUTT,
    AxCAP_ROUND,
    AxCAP_SQUARE
};

enum {
    /* Line join styles */
    AxJOIN_UNKNOWN = 0,
    AxJOIN_ARCS,
    AxJOIN_BEVEL,
    AxJOIN_MITER,
    AxJOIN_MITER_CLIP,
    AxJOIN_ROUND
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
    Pen()
        : m_penColour(0), m_penWidth(0), m_dashLength(0), m_gapLength(0), m_lineCap(0), m_lineJoin(0), m_penOpacity(0.0)
    {
    }
    Pen(int colour, int width, float opacity, int dashLength, int gapLength, int lineCap, int lineJoin)
        : m_penColour(colour)
        , m_penWidth(width)
        , m_dashLength(dashLength)
        , m_gapLength(gapLength)
        , m_lineCap(lineCap)
        , m_lineJoin(lineJoin)
        , m_penOpacity(opacity)
    {
    }

    int GetColour() const { return m_penColour; }
    void SetColour(int colour) { m_penColour = colour; }
    int GetWidth() const { return m_penWidth; }
    void SetWidth(int width) { m_penWidth = width; }
    int GetDashLength() const { return m_dashLength; }
    void SetDashLength(int dashLength) { m_dashLength = dashLength; }
    int GetGapLength() const { return m_gapLength; }
    void SetGapLength(int gapLength) { m_gapLength = gapLength; }
    int GetLineCap() const { return m_lineCap; }
    void SetLineCap(int lineCap) { m_lineCap = lineCap; }
    int GetLineJoin() const { return m_lineJoin; }
    void SetLineJoin(int lineJoin) { m_lineJoin = lineJoin; }
    float GetOpacity() const { return m_penOpacity; }
    void SetOpacity(float opacity) { m_penOpacity = opacity; }

private:
    int m_penColour, m_penWidth, m_dashLength, m_gapLength, m_lineCap, m_lineJoin;
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
        m_smuflFont = false;
    }
    virtual ~FontInfo(){};

    // accessors and modifiers for the font elements
    int GetPointSize() const { return m_pointSize; }
    data_FONTSTYLE GetStyle() const { return m_style; }
    data_FONTWEIGHT GetWeight() const { return m_weight; }
    bool GetUnderlined() const { return m_underlined; }
    bool GetSupSubScript() const { return m_supSubScript; }
    std::string GetFaceName() const { return m_faceName; }
    int GetFamily() const { return m_family; }
    int GetEncoding() const { return m_encoding; }
    float GetWidthToHeightRatio() const { return m_widthToHeightRatio; }
    bool GetSmuflFont() const { return m_smuflFont; }

    void SetPointSize(int pointSize) { m_pointSize = pointSize; }
    void SetStyle(data_FONTSTYLE style) { m_style = style; }
    void SetWeight(data_FONTWEIGHT weight) { m_weight = weight; }
    void SetUnderlined(bool underlined) { m_underlined = underlined; }
    void SetSupSubScript(bool supSubScript) { m_supSubScript = supSubScript; }
    void SetFaceName(const std::string &faceName) { m_faceName = faceName; }
    void SetFamily(int family) { m_family = family; }
    void SetEncoding(int encoding) { m_encoding = encoding; }
    void SetWidthToHeightRatio(float ratio) { m_widthToHeightRatio = ratio; }
    void SetSmuflFont(bool smuflFont) { m_smuflFont = smuflFont; }

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
    bool m_smuflFont;
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

    Point min(const Point &p) const
    {
        int x = std::min(this->x, p.x);
        int y = std::min(this->y, p.y);
        return Point(x, y);
    }

    Point max(const Point &p) const
    {
        int x = std::max(this->x, p.x);
        int y = std::max(this->y, p.y);
        return Point(x, y);
    }
};

// ---------------------------------------------------------------------------
// BezierCurve
// ---------------------------------------------------------------------------

/**
 * Simple class for representing bezier cCurve
 */

class BezierCurve {
public:
    Point p1; // start point
    Point c1, c2; // control points
    Point p2; // end point

    // no copy ctor or assignment operator - the defaults are ok

    BezierCurve() {}
    BezierCurve(const Point &p1, const Point &c1, const Point &c2, const Point &p2) : p1(p1), c1(c1), c2(c2), p2(p2) {}

    // Helper to rotate all points within bezier curve around @rotationPoint by @angle
    void Rotate(float angle, const Point &rotationPoint);

    /**
     * @name Getter/setter for control point offset (as well as method to calculate it from options)
     */
    ///@{
    void SetControlOffset(int offset) { m_leftControlOffset = m_rightControlOffset = offset; }
    void SetLeftControlOffset(int offset) { m_leftControlOffset = offset; }
    void SetRightControlOffset(int offset) { m_rightControlOffset = offset; }
    int GetLeftControlOffset() const { return m_leftControlOffset; }
    int GetRightControlOffset() const { return m_rightControlOffset; }
    ///@}

    /**
     * @name Getter/setter for the height of control points (left and right)
     */
    ///@{
    void SetControlHeight(int height) { m_leftControlHeight = m_rightControlHeight = height; }
    void SetLeftControlHeight(int height) { m_leftControlHeight = height; }
    void SetRightControlHeight(int height) { m_rightControlHeight = height; }
    int GetLeftControlHeight() const { return m_leftControlHeight; }
    int GetRightControlHeight() const { return m_rightControlHeight; }
    ///@}

    /**
     * @name Getter/setter for the side of the control points (left and right)
     */
    ///@{
    void SetControlSides(bool leftAbove, bool rightAbove);
    bool IsLeftControlAbove() const { return m_leftControlAbove; }
    bool IsRightControlAbove() const { return m_rightControlAbove; }
    ///@}

    /**
     * @name Initialize control point height and offset from end point positions
     */
    ///@{
    void CalcInitialControlPointParams();
    void CalcInitialControlPointParams(const Doc *doc, float angle, int staffSize);
    ///@}

    /**
     * Calculate control point offset and height from points or vice versa
     */
    ///@{
    void UpdateControlPointParams();
    void UpdateControlPoints();
    ///@}

    /**
     * Estimate the curve parameter corresponding to the control points
     * Based on the polyline P1-C1-C2-P2
     */
    std::pair<double, double> EstimateCurveParamForControlPoints() const;

private:
    // Control point X-axis offset for both start/end points
    int m_leftControlOffset = 0;
    int m_rightControlOffset = 0;
    int m_leftControlHeight = 0;
    int m_rightControlHeight = 0;
    bool m_leftControlAbove = true;
    bool m_rightControlAbove = true;

    // no copy ctor or assignment operator - the defaults are ok
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
