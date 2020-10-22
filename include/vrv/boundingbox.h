/////////////////////////////////////////////////////////////////////////////
// Name:        boundingbox.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_BOUNDING_BOX_H__
#define __VRV_BOUNDING_BOX_H__

//----------------------------------------------------------------------------

#include "vrvdef.h"

namespace vrv {

class Doc;
class FloatingCurvePositioner;
class Glyph;

//----------------------------------------------------------------------------
// BoundingBox
//----------------------------------------------------------------------------

/**
 * This class represents a basic object in the layout domain
 */
class BoundingBox {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    BoundingBox();
    virtual ~BoundingBox(){};
    virtual ClassId GetClassId() const;
    bool Is(ClassId classId) const { return (this->GetClassId() == classId); }
    bool Is(const std::vector<ClassId> &classIds) const;
    ///@}

    /**
     * @name Methods for updating the bounding boxes and for providing information about their status.
     */
    ///@{
    virtual void UpdateContentBBoxX(int x1, int x2);
    virtual void UpdateContentBBoxY(int y1, int y2);
    virtual void UpdateSelfBBoxX(int x1, int x2);
    virtual void UpdateSelfBBoxY(int y1, int y2);
    void SetEmptyBB();
    //
    bool HasSelfBB() const;
    bool HasSelfHorizontalBB() const;
    bool HasSelfVerticalBB() const;
    bool HasContentBB() const;
    bool HasContentHorizontalBB() const;
    bool HasContentVerticalBB() const;
    bool HasEmptyBB() const;
    ///@}

    /**
     * Set and get the smuflGlyph / fontsize for a bounding box that is the one of a single SMuFL glyph.
     */
    ///@{
    void SetBoundingBoxGlyph(wchar_t smuflGlyph, int fontSize);
    wchar_t GetBoundingBoxGlyph() const { return m_smuflGlyph; }
    int GetBoundingBoxGlyphFontSize() const { return m_smuflGlyphFontSize; }
    ///@}

    /**
     * Reset the bounding box values
     */
    virtual void ResetBoundingBox();

    /**
     * @name Get the X and Y drawing position.
     * Pure virtual methods
     */
    ///@{
    virtual int GetDrawingX() const = 0;
    virtual int GetDrawingY() const = 0;
    ///@}

    /**
     * @name Reset the cached values of the drawingX and Y values.
     * Pure virtual methods.
     */
    ///@{
    virtual void ResetCachedDrawingX() const = 0;
    virtual void ResetCachedDrawingY() const = 0;
    ///@}

    /**
     * @name Get positions for self and content
     */
    ///@{
    int GetSelfBottom() const { return (this->GetDrawingY() + m_selfBB_y1); }
    int GetSelfTop() const { return (this->GetDrawingY() + m_selfBB_y2); }
    int GetSelfLeft() const { return (this->GetDrawingX() + m_selfBB_x1); }
    int GetSelfRight() const { return (this->GetDrawingX() + m_selfBB_x2); }
    int GetContentBottom() const { return (this->GetDrawingY() + m_contentBB_y1); }
    int GetContentTop() const { return (this->GetDrawingY() + m_contentBB_y2); }
    int GetContentLeft() const { return (this->GetDrawingX() + m_contentBB_x1); }
    int GetContentRight() const { return (this->GetDrawingX() + m_contentBB_x2); }
    //
    int GetSelfX1() const { return m_selfBB_x1; }
    int GetSelfX2() const { return m_selfBB_x2; }
    int GetSelfY1() const { return m_selfBB_y1; }
    int GetSelfY2() const { return m_selfBB_y2; }
    int GetContentX1() const { return m_contentBB_x1; }
    int GetContentX2() const { return m_contentBB_x2; }
    int GetContentY1() const { return m_contentBB_y1; }
    int GetContentY2() const { return m_contentBB_y2; }
    ///@}

    /**
     * @name Get wrappers
     */
    ///@{
    int GetBottomBy(Accessor type) const { return ((type == SELF) ? GetSelfBottom() : GetContentBottom()); }
    int GetTopBy(Accessor type) const { return ((type == SELF) ? GetSelfTop() : GetContentTop()); }
    int GetLeftBy(Accessor type) const { return ((type == SELF) ? GetSelfLeft() : GetContentLeft()); }
    int GetRightBy(Accessor type) const { return ((type == SELF) ? GetSelfRight() : GetContentRight()); }
    int GetX1By(Accessor type) const { return ((type == SELF) ? GetSelfX1() : GetContentX1()); }
    int GetX2By(Accessor type) const { return ((type == SELF) ? GetSelfX2() : GetContentX2()); }
    int GetY1By(Accessor type) const { return ((type == SELF) ? GetSelfY1() : GetContentY1()); }
    int GetY2By(Accessor type) const { return ((type == SELF) ? GetSelfY2() : GetContentY2()); }
    ///@}

    /**
     * @name Return true if the bounding box has a horizontal / vertical overlap with the other one.
     * Makes an overal bounding box overlap calculation without looking at anchor points
     */
    ///@{
    bool HorizontalContentOverlap(const BoundingBox *other, int margin = 0) const;
    bool VerticalContentOverlap(const BoundingBox *other, int margin = 0) const;
    bool HorizontalSelfOverlap(const BoundingBox *other, int margin = 0) const;
    bool VerticalSelfOverlap(const BoundingBox *other, int margin = 0) const;
    ///@}

    /**
     * @name Return the overlap on the left / right / top / bottom looking at bounding box anchor points
     */
    ///@{
    int HorizontalLeftOverlap(const BoundingBox *other, Doc *doc, int margin = 0, int vMargin = 0) const;
    int HorizontalRightOverlap(const BoundingBox *other, Doc *doc, int margin = 0, int vMaring = 0) const;
    int VerticalTopOverlap(const BoundingBox *other, Doc *doc, int margin = 0, int hMargin = 0) const;
    int VerticalBottomOverlap(const BoundingBox *other, Doc *doc, int margin = 0, int hMargin = 0) const;
    ////}

    /**
     * Return true if the bounding box encloses the point.
     */
    bool Encloses(const Point point) const;

    /**
     * Return true if the bounding box intersects with the curve represented by the FloatingPositioner.
     * The Object pointed by the FloatingPositioner is expected to be a SLUR or a TIE
     */
    int Intersects(FloatingCurvePositioner *curve, Accessor type, int margin = 0) const;

    /**
     * Swap values.
     * This is useful for example when switching to the device context world.
     */
    static void Swap(int &v1, int &v2);

    /**
     * Swap the points passed as reference.
     * This is useful for example when calculating bezier positions.
     */
    static void SwapPoints(Point &p1, Point &p2);

    /**
     * Calculate the slope represented by two points
     */
    static double CalcSlope(Point const &p1, Point const &p2);

    /**
     * Calculate the position of a point after a rotation of alpha (in radian) around the center
     */
    static Point CalcPositionAfterRotation(Point point, float alpha, Point center);

    /**
     * Calculate the y position of a bezier at position x
     */
    static int CalcBezierAtPosition(const Point bezier[4], int x);

    /**
     * Calculate linear interpolation between two points at time t
     */
    static void CalcLinearInterpolation(Point &dest, const Point &a, const Point &b, double t);

    /**
     * Calculate point (X,Y) coordinaties on the bezier curve
     */
    static Point CalcPointAtBezier(const Point bezier[4], double t);

    /**
     * Calculate thickness coeficient to be applient for bezier curve to fit MEI units thickness
     */
    static double GetBezierThicknessCoeficient(const Point bezier[4], int currentThickness, double angle, int penWidth);

    /**
     * Calculate the point bezier point position for a t between 0.0 and 1.0
     */
    static Point CalcDeCasteljau(const Point bezier[4], double t);

    /**
     * Calculate the position of the bezier above and below for a thick bezier
     */
    static void CalcThickBezier(
        const Point bezier[4], int thickness, float angle, Point *topBezier, Point *bottomBezier);

    /**
     * Approximate the bounding box of a bezier taking into accound the height and the width.
     */
    static void ApproximateBezierBoundingBox(
        const Point bezier[4], Point &pos, int &width, int &height, int &minYPos, int &maxYPos);

    /**
     * Calculate the left / right / top / bottom overlap of two rectangle taking into account the margin / v-h-Margins
     */
    ///@{
    static int RectLeftOverlap(const Point rect1[2], const Point rect2[2], int margin, int vMargin);
    static int RectRightOverlap(const Point rect1[2], const Point rect2[2], int margin, int vMargin);
    static int RectTopOverlap(const Point rect1[2], const Point rect2[2], int margin, int hMargin);
    static int RectBottomOverlap(const Point rect1[2], const Point rect2[2], int margin, int hMargin);
    ///@}

private:
    /**
     * Get the rectangles covering the inside of a bounding box given two anchors (e.g., NW and NE, or NE and SE)
     * Looks at the anchors for the smufl glpyh (if any) and return the number of rectangles needed to represent the
     * bounding box.
     * Return 1 with no smufl glyph or no anchor, 2 with on anchor point, and 3 with 2 anchor points.
     */
    int GetRectangles(
        const SMuFLGlyphAnchor &anchor1, const SMuFLGlyphAnchor &anchor2, Point rect[3][2], Doc *doc) const;

    /**
     * Calculate the rectangles with 2 anchor points.
     * Return false (and one single rectangle) when anchor points are out of the boundaries.
     */
    bool GetGlyph2PointRectangles(const SMuFLGlyphAnchor &anchor1, const SMuFLGlyphAnchor &anchor2, Glyph *glyph1,
        Point rect[3][2], Doc *doc) const;

    /**
     * Calculate the rectangles with 1 anchor point.
     * Return false (and one single rectangle) when anchor points are out of the boundaries.
     */
    bool GetGlyph1PointRectangles(const SMuFLGlyphAnchor &anchor, Glyph *glyph, Point rect[2][2], Doc *doc) const;

public:
    //
protected:
    /**
     * The cached version of the drawingX and drawingY values.
     * These are reset by ResetCachedDrawingX/Y methods when necessary.
     * Mutable because to be updated in GetDrawingX/Y const.
     */
    ///@{
    mutable int m_cachedDrawingX;
    mutable int m_cachedDrawingY;
    ///@}
private:
    /**
     * Bounding box positions
     */
    ///@{
    int m_contentBB_x1, m_contentBB_y1, m_contentBB_x2, m_contentBB_y2;
    int m_selfBB_x1, m_selfBB_y1, m_selfBB_x2, m_selfBB_y2;
    ///@}

    /**
     * The SMuFL glyph when anchor bounding box calculation is desired.
     * Currently only one glyph is supported. Eventually, we could have start / end glyph
     */
    wchar_t m_smuflGlyph;

    /**
     * The font size for the smufl glyph used for calculating the bounding box rectangles.
     */
    int m_smuflGlyphFontSize;
};

//----------------------------------------------------------------------------
// SegmentedLine
//----------------------------------------------------------------------------

/**
 */
class SegmentedLine {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    SegmentedLine(int start, int end);
    virtual ~SegmentedLine(){};
    ///@}

    /**
     * Check if the segmented line is empty
     */
    bool IsEmpty() { return (m_segments.empty()); }

    /**
     * Check if the line is one single segment
     */
    bool IsUnsegmented() { return (m_segments.size() == 1); }

    /**
     * The number of segments
     */
    int GetSegmentCount() { return (int)m_segments.size(); }

    /**
     * Get the start and end of a segment
     */
    void GetStartEnd(int &start, int &end, int idx);

    /**
     * Add a gap in the line
     */
    void AddGap(int start, int end);

protected:
    //
private:
    //
public:
    //
protected:
    //
private:
    /**
     * An vector of line segments
     */
    ArrayOfIntPairs m_segments;
};

} // namespace vrv

#endif
