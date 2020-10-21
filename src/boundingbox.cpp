/////////////////////////////////////////////////////////////////////////////
// Name:        boundingbox.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "boundingbox.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "devicecontextbase.h"
#include "doc.h"
#include "floatingobject.h"
#include "glyph.h"
#include "vrv.h"

#define BEZIER_APPROXIMATION 50.0

namespace vrv {

//----------------------------------------------------------------------------
// BoundingBox
//----------------------------------------------------------------------------

BoundingBox::BoundingBox()
{
    Point rect1[2];
    Point rect2[2];

    rect1[0] = Point(0, 0);
    rect1[1] = Point(100, 100);

    rect2[0] = Point(25, 25);
    rect2[1] = Point(100, 100);

    ResetBoundingBox();
}

ClassId BoundingBox::GetClassId() const
{
    // we should always have the method overridden
    assert(false);
    return BOUNDING_BOX;
}

bool BoundingBox::Is(const std::vector<ClassId> &classIds) const
{
    return (std::find(classIds.begin(), classIds.end(), this->GetClassId()) != classIds.end());
}

void BoundingBox::UpdateContentBBoxX(int x1, int x2)
{
    // LogDebug("CB Was: %i %i %i %i", m_contentBB_x1, m_contentBB_y1, m_contentBB_x2, m_contentBB_y2);

    int minX = std::min(x1, x2);
    int maxX = std::max(x1, x2);

    int drawingX = GetDrawingX();

    minX -= drawingX;
    maxX -= drawingX;

    if (m_contentBB_x1 > minX) m_contentBB_x1 = minX;
    if (m_contentBB_x2 < maxX) m_contentBB_x2 = maxX;

    // LogDebug("CB Is:  %i %i %i %i %s", m_contentBB_x1,m_contentBB_y1, m_contentBB_x2, m_contentBB_y2,
    // GetClassName().c_str());
}

void BoundingBox::UpdateContentBBoxY(int y1, int y2)
{
    // LogDebug("CB Was: %i %i %i %i", m_contentBB_x1, m_contentBB_y1, m_contentBB_x2, m_contentBB_y2);

    int min_y = std::min(y1, y2);
    int max_y = std::max(y1, y2);

    int drawingY = GetDrawingY();

    min_y -= drawingY;
    max_y -= drawingY;

    if (m_contentBB_y1 > min_y) m_contentBB_y1 = min_y;
    if (m_contentBB_y2 < max_y) m_contentBB_y2 = max_y;

    // LogDebug("CB Is:  %i %i %i %i %s", m_contentBB_x1,m_contentBB_y1, m_contentBB_x2, m_contentBB_y2,
    // GetClassName().c_str());
}

void BoundingBox::UpdateSelfBBoxX(int x1, int x2)
{
    // LogDebug("SB Was: %i %i %i %i", m_selfBB_x1,m_selfBB_y1, m_selfBB_x2, m_selfBB_y2);

    int minX = std::min(x1, x2);
    int maxX = std::max(x1, x2);

    int drawingX = GetDrawingX();

    minX -= drawingX;
    maxX -= drawingX;

    if (m_selfBB_x1 > minX) m_selfBB_x1 = minX;
    if (m_selfBB_x2 < maxX) m_selfBB_x2 = maxX;

    // LogDebug("SB Is:  %i %i %i %i", m_selfBB_x1,m_selfBB_y1, m_selfBB_x2, m_selfBB_y2);
}

void BoundingBox::UpdateSelfBBoxY(int y1, int y2)
{
    // LogDebug("SB Was: %i %i %i %i", m_selfBB_x1,m_selfBB_y1, m_selfBB_x2, m_selfBB_y2);

    int min_y = std::min(y1, y2);
    int max_y = std::max(y1, y2);

    int drawingY = GetDrawingY();

    min_y -= drawingY;
    max_y -= drawingY;

    if (m_selfBB_y1 > min_y) m_selfBB_y1 = min_y;
    if (m_selfBB_y2 < max_y) m_selfBB_y2 = max_y;

    // LogDebug("SB Is:  %i %i %i %i", m_selfBB_x1,m_selfBB_y1, m_selfBB_x2, m_selfBB_y2);
}

void BoundingBox::ResetBoundingBox()
{
    m_contentBB_x1 = -VRV_UNSET;
    m_contentBB_y1 = -VRV_UNSET;
    m_contentBB_x2 = VRV_UNSET;
    m_contentBB_y2 = VRV_UNSET;
    m_selfBB_x1 = -VRV_UNSET;
    m_selfBB_y1 = -VRV_UNSET;
    m_selfBB_x2 = VRV_UNSET;
    m_selfBB_y2 = VRV_UNSET;

    m_cachedDrawingX = VRV_UNSET;
    m_cachedDrawingY = VRV_UNSET;

    m_smuflGlyph = 0;
    m_smuflGlyphFontSize = 100;
}

void BoundingBox::SetEmptyBB()
{
    m_contentBB_x1 = 0;
    m_contentBB_y1 = 0;
    m_contentBB_x2 = 0;
    m_contentBB_y2 = 0;
    m_selfBB_x1 = 0;
    m_selfBB_y1 = 0;
    m_selfBB_x2 = 0;
    m_selfBB_y2 = 0;
}

bool BoundingBox::HasEmptyBB() const
{
    // We are checking only the content bounding box - this should be OK
    return ((m_contentBB_x1 == 0) && (m_contentBB_y1 == 0) && (m_contentBB_x2 == 0) && (m_contentBB_y2 == 0));
}

bool BoundingBox::HasContentBB() const
{
    return (HasContentHorizontalBB() && HasContentVerticalBB());
}

bool BoundingBox::HasContentHorizontalBB() const
{
    return ((m_contentBB_x1 != -VRV_UNSET) && (m_contentBB_x2 != VRV_UNSET));
}

bool BoundingBox::HasContentVerticalBB() const
{
    return ((m_contentBB_y1 != -VRV_UNSET) && (m_contentBB_y2 != VRV_UNSET));
}

bool BoundingBox::HasSelfBB() const
{
    return (HasSelfHorizontalBB() && HasSelfVerticalBB());
}

bool BoundingBox::HasSelfHorizontalBB() const
{
    return ((m_selfBB_x1 != -VRV_UNSET) && (m_selfBB_x2 != VRV_UNSET));
}

bool BoundingBox::HasSelfVerticalBB() const
{
    return ((m_selfBB_y1 != -VRV_UNSET) && (m_selfBB_y2 != VRV_UNSET));
}

void BoundingBox::SetBoundingBoxGlyph(wchar_t smuflGlyph, int fontSize)
{
    assert(smuflGlyph);
    m_smuflGlyph = smuflGlyph;
    m_smuflGlyphFontSize = fontSize;
}

bool BoundingBox::HorizontalContentOverlap(const BoundingBox *other, int margin) const
{
    assert(other);

    if (!this->HasContentBB() || !other->HasContentBB()) return false;
    if (this->GetContentRight() <= other->GetContentLeft() - margin) return false;
    if (this->GetContentLeft() >= other->GetContentRight() + margin) return false;

    return true;
}

bool BoundingBox::VerticalContentOverlap(const BoundingBox *other, int margin) const
{
    assert(other);

    if (!this->HasContentBB() || !other->HasContentBB()) return false;
    if (this->GetContentTop() <= other->GetContentBottom() - margin) return false;
    if (this->GetContentBottom() >= other->GetContentTop() + margin) return false;
    return true;
}

bool BoundingBox::HorizontalSelfOverlap(const BoundingBox *other, int margin) const
{
    assert(other);

    if (!this->HasSelfBB() || !other->HasSelfBB()) return false;
    if (this->GetSelfRight() <= other->GetSelfLeft() - margin) return false;
    if (this->GetSelfLeft() >= other->GetSelfRight() + margin) return false;
    return true;
}

bool BoundingBox::VerticalSelfOverlap(const BoundingBox *other, int margin) const
{
    assert(other);

    if (!this->HasSelfBB() || !other->HasSelfBB()) return false;
    if (this->GetSelfTop() <= other->GetSelfBottom() - margin) return false;
    if (this->GetSelfBottom() >= other->GetSelfTop() + margin) return false;
    return true;
}

int BoundingBox::HorizontalLeftOverlap(const BoundingBox *other, Doc *doc, int margin, int vMargin) const
{
    Point BB1rect[3][2], BB2rect[3][2];
    int i, j;
    int anchor1, anchor2;
    int overlap = 0;

    anchor1 = this->GetRectangles(SMUFL_cutOutNW, SMUFL_cutOutSW, BB1rect, doc);
    anchor2 = other->GetRectangles(SMUFL_cutOutNE, SMUFL_cutOutSE, BB2rect, doc);
    for (i = 0; i < anchor1; ++i) {
        for (j = 0; j < anchor2; ++j) {
            overlap = std::max(overlap, RectLeftOverlap(BB1rect[i], BB2rect[j], margin, vMargin));
        }
    }

    return overlap;
}

int BoundingBox::HorizontalRightOverlap(const BoundingBox *other, Doc *doc, int margin, int vMargin) const
{
    Point BB1rect[3][2], BB2rect[3][2];
    int i, j;
    int anchor1, anchor2;
    int overlap = 0;

    anchor1 = this->GetRectangles(SMUFL_cutOutNE, SMUFL_cutOutSE, BB1rect, doc);
    anchor2 = other->GetRectangles(SMUFL_cutOutNW, SMUFL_cutOutSW, BB2rect, doc);
    for (i = 0; i < anchor1; ++i) {
        for (j = 0; j < anchor2; ++j) {
            overlap = std::max(overlap, RectRightOverlap(BB1rect[i], BB2rect[j], margin, vMargin));
        }
    }

    return overlap;
}

int BoundingBox::VerticalTopOverlap(const BoundingBox *other, Doc *doc, int margin, int hMargin) const
{
    Point BB1rect[3][2], BB2rect[3][2];
    int i, j;
    int anchor1, anchor2;
    int overlap = 0;

    anchor1 = this->GetRectangles(SMUFL_cutOutNW, SMUFL_cutOutNE, BB1rect, doc);
    anchor2 = other->GetRectangles(SMUFL_cutOutSW, SMUFL_cutOutSE, BB2rect, doc);
    for (i = 0; i < anchor1; ++i) {
        for (j = 0; j < anchor2; ++j) {
            overlap = std::max(overlap, RectTopOverlap(BB1rect[i], BB2rect[j], margin, hMargin));
        }
    }

    return overlap;
}

int BoundingBox::VerticalBottomOverlap(const BoundingBox *other, Doc *doc, int margin, int hMargin) const
{
    Point BB1rect[3][2], BB2rect[3][2];
    int i, j;
    int anchor1, anchor2;
    int overlap = 0;

    anchor1 = this->GetRectangles(SMUFL_cutOutSW, SMUFL_cutOutSE, BB1rect, doc);
    anchor2 = other->GetRectangles(SMUFL_cutOutNW, SMUFL_cutOutNE, BB2rect, doc);
    for (i = 0; i < anchor1; ++i) {
        for (j = 0; j < anchor2; ++j) {
            overlap = std::max(overlap, RectBottomOverlap(BB1rect[i], BB2rect[j], margin, hMargin));
        }
    }

    return overlap;
}

int BoundingBox::GetRectangles(
    const SMuFLGlyphAnchor &anchor1, const SMuFLGlyphAnchor &anchor2, Point rect[3][2], Doc *doc) const
{
    Glyph *glyph = NULL;

    bool glyphRect = true;

    if (m_smuflGlyph != 0) {
        glyph = Resources::GetGlyph(m_smuflGlyph);
        assert(glyph);

        if (glyph->HasAnchor(anchor1) && glyph->HasAnchor(anchor2)) {
            glyphRect = this->GetGlyph2PointRectangles(anchor1, anchor2, glyph, rect, doc);
            if (glyphRect) return 3;
        }
        else if (glyph->HasAnchor(anchor1)) {
            glyphRect = this->GetGlyph1PointRectangles(anchor1, glyph, rect, doc);
            if (glyphRect) return 2;
        }
        else if (glyph->HasAnchor(anchor2)) {
            glyphRect = this->GetGlyph1PointRectangles(anchor2, glyph, rect, doc);
            if (glyphRect) return 2;
        }
    }
    if (!glyphRect) {
        LogDebug("Illogical values for anchor points in glyph '%02x'", m_smuflGlyph);
    }

    rect[0][0] = Point(this->GetSelfLeft(), this->GetSelfTop());
    rect[0][1] = Point(this->GetSelfRight(), this->GetSelfBottom());

    return 1;
}

bool BoundingBox::GetGlyph2PointRectangles(
    const SMuFLGlyphAnchor &anchor1, const SMuFLGlyphAnchor &anchor2, Glyph *glyph, Point rect[3][2], Doc *doc) const
{
    assert(glyph);

    const Point *fontPoint1 = glyph->GetAnchor(anchor1);
    assert(fontPoint1);
    const Point *fontPoint2 = glyph->GetAnchor(anchor2);
    assert(fontPoint2);
    Point p1, p2;
    int x, y, w, h;
    glyph->GetBoundingBox(x, y, w, h);

    int selfLeft = this->GetSelfLeft();
    int selfRight = this->GetSelfRight();
    int selfTop = this->GetSelfTop();
    int selfBottom = this->GetSelfBottom();

    p1.x = selfLeft - x * m_smuflGlyphFontSize / glyph->GetUnitsPerEm();
    p1.y = selfBottom - y * m_smuflGlyphFontSize / glyph->GetUnitsPerEm();
    p2 = p1;

    p1.x += (fontPoint1->x * m_smuflGlyphFontSize / glyph->GetUnitsPerEm());
    p1.y += (fontPoint1->y * m_smuflGlyphFontSize / glyph->GetUnitsPerEm());

    p2.x += (fontPoint2->x * m_smuflGlyphFontSize / glyph->GetUnitsPerEm());
    p2.y += (fontPoint2->y * m_smuflGlyphFontSize / glyph->GetUnitsPerEm());

    if (p1.x < selfLeft) return false;
    if (p1.x > selfRight) return false;
    if (p1.y > selfTop) return false;
    if (p1.y < selfBottom) return false;

    if (p2.x < selfLeft) return false;
    if (p2.x > selfRight) return false;
    if (p2.y > selfTop) return false;
    if (p2.y < selfBottom) return false;

    if ((anchor1 == SMUFL_cutOutNW) && (anchor2 == SMUFL_cutOutNE)) {
        // - | X | -
        // X | X | X
        rect[0][0] = Point(selfLeft, p1.y);
        rect[0][1] = Point(p1.x, selfBottom);
        rect[1][0] = Point(p1.x, selfTop);
        rect[1][1] = Point(p2.x, selfBottom);
        rect[2][0] = Point(p2.x, p2.y);
        rect[2][1] = Point(selfRight, selfBottom);
    }
    else if ((anchor1 == SMUFL_cutOutNE) && (anchor2 == SMUFL_cutOutSE)) {
        // X | -
        // X . X
        // X | -
        rect[0][0] = Point(selfLeft, selfTop);
        rect[0][1] = Point(p1.x, p1.y);
        rect[1][0] = Point(selfLeft, p1.y);
        rect[1][1] = Point(selfRight, p2.y);
        rect[2][0] = Point(selfLeft, p2.y);
        rect[2][1] = Point(p2.x, selfBottom);
    }
    else if ((anchor1 == SMUFL_cutOutSW) && (anchor2 == SMUFL_cutOutSE)) {
        // X | X | X
        // - | X | -
        rect[0][0] = Point(selfLeft, selfTop);
        rect[0][1] = Point(p1.x, p1.y);
        rect[1][0] = Point(p1.x, selfTop);
        rect[1][1] = Point(p2.x, selfBottom);
        rect[2][0] = Point(p2.x, selfTop);
        rect[2][1] = Point(selfRight, p2.y);
    }
    else if ((anchor1 == SMUFL_cutOutNW) && (anchor2 == SMUFL_cutOutSW)) {
        // - | X
        // X . X
        // - | X
        rect[0][0] = Point(p1.x, selfTop);
        rect[0][1] = Point(selfRight, p1.y);
        rect[1][0] = Point(selfLeft, p1.y);
        rect[1][1] = Point(selfRight, p2.y);
        rect[2][0] = Point(p2.x, p2.y);
        rect[2][1] = Point(selfRight, selfBottom);
    }
    else {
        // Check the order / combination of anchors - they should match one of the four above
        assert(false);
    }

    return true;
}

bool BoundingBox::GetGlyph1PointRectangles(
    const SMuFLGlyphAnchor &anchor, Glyph *glyph, Point rect[2][2], Doc *doc) const
{
    assert(glyph);

    const Point *fontPoint = glyph->GetAnchor(anchor);
    assert(fontPoint);
    Point p;
    int x, y, w, h;
    glyph->GetBoundingBox(x, y, w, h);

    int selfLeft = this->GetSelfLeft();
    int selfRight = this->GetSelfRight();
    int selfTop = this->GetSelfTop();
    int selfBottom = this->GetSelfBottom();

    p.x = selfLeft - x * m_smuflGlyphFontSize / glyph->GetUnitsPerEm();
    p.x += (fontPoint->x * m_smuflGlyphFontSize / glyph->GetUnitsPerEm());
    p.y = selfBottom - y * m_smuflGlyphFontSize / glyph->GetUnitsPerEm();
    p.y += (fontPoint->y * m_smuflGlyphFontSize / glyph->GetUnitsPerEm());

    if (p.x < selfLeft) return false;
    if (p.x > selfRight) return false;
    if (p.y > selfTop) return false;
    if (p.y < selfBottom) return false;

    if (anchor == SMUFL_cutOutNE) {
        // X | -
        // X . X
        rect[0][0] = Point(selfLeft, selfTop);
        rect[0][1] = Point(p.x, p.y);
        rect[1][0] = Point(selfLeft, p.y);
        rect[1][1] = Point(selfRight, selfBottom);
    }
    else if (anchor == SMUFL_cutOutSE) {
        // X . X
        // X | -
        rect[0][0] = Point(selfLeft, selfTop);
        rect[0][1] = Point(selfRight, p.y);
        rect[1][0] = Point(selfLeft, p.y);
        rect[1][1] = Point(p.x, selfBottom);
    }
    else if (anchor == SMUFL_cutOutSW) {
        // X . X
        // - | X
        rect[0][0] = Point(selfLeft, selfTop);
        rect[0][1] = Point(selfRight, p.y);
        rect[1][0] = Point(p.x, p.y);
        rect[1][1] = Point(selfRight, selfBottom);
    }
    else if (anchor == SMUFL_cutOutNW) {
        // - | X
        // X . X
        rect[0][0] = Point(p.x, selfTop);
        rect[0][1] = Point(selfRight, p.y);
        rect[1][0] = Point(selfLeft, p.y);
        rect[1][1] = Point(selfRight, selfBottom);
    }
    else {
        // assert(false);
        return false;
    }

    return true;
}

bool BoundingBox::Encloses(const Point point) const
{
    if (this->GetContentRight() < point.x) return false;
    if (this->GetContentLeft() > point.x) return false;
    if (this->GetContentTop() < point.y) return false;
    if (this->GetContentBottom() > point.y) return false;
    return true;
}

int BoundingBox::Intersects(FloatingCurvePositioner *curve, Accessor type, int margin) const
{
    assert(curve);
    assert(curve->GetObject());
    assert(curve->GetObject()->Is({ PHRASE, SLUR, TIE }));

    // for lisability
    Point points[4];
    curve->GetPoints(points);
    Point p1 = points[0];
    Point p2 = points[3];

    // first check if they overlap at all
    if (p2.x < this->GetLeftBy(type)) return 0;
    if (p1.x > this->GetRightBy(type)) return 0;

    Point topBezier[4], bottomBezier[4];
    BoundingBox::CalcThickBezier(points, curve->GetThickness(), curve->GetAngle(), topBezier, bottomBezier);

    // The curve overflows on both sides
    if ((p1.x < this->GetLeftBy(type)) && p2.x > this->GetRightBy(type)) {
        // LogDebug("overflows both sides");
        if (curve->GetDir() == curvature_CURVEDIR_above) {
            // The curve is already below the content
            if ((curve->GetTopBy(type) + margin) < this->GetBottomBy(type)) return 0;
            int xMaxY = curve->CalcMinMaxY(topBezier);
            int leftY = BoundingBox::CalcBezierAtPosition(bottomBezier, this->GetLeftBy(type)) + margin;
            int rightY = BoundingBox::CalcBezierAtPosition(bottomBezier, this->GetRightBy(type)) + margin;
            // Everything is underneath
            if ((leftY >= this->GetTopBy(type)) && (rightY >= this->GetTopBy(type))) return 0;
            // Recalculate for above
            leftY = BoundingBox::CalcBezierAtPosition(topBezier, this->GetLeftBy(type)) + margin;
            rightY = BoundingBox::CalcBezierAtPosition(topBezier, this->GetRightBy(type)) + margin;
            // The box is above the summit of the curve
            if ((this->GetLeftBy(type) < (p1.x + xMaxY)) && (this->GetRightBy(type) > (p1.x + xMaxY)))
                return (curve->GetTopBy(type) - this->GetBottomBy(type) + margin);
            // The content is on the left
            if (this->GetRightBy(type) < (p1.x + xMaxY))
                return (rightY - this->GetBottomBy(type));
            else
                return (leftY - this->GetBottomBy(type));
        }
        else {
            // The curve is already above the content
            if ((curve->GetBottomBy(type) - margin) > this->GetTopBy(type)) return 0;
            int xMinY = curve->CalcMinMaxY(bottomBezier);
            // Check if the box is above
            int leftY = BoundingBox::CalcBezierAtPosition(topBezier, this->GetLeftBy(type)) - margin;
            int rightY = BoundingBox::CalcBezierAtPosition(topBezier, this->GetRightBy(type)) - margin;
            if ((leftY <= this->GetBottomBy(type)) && (rightY <= this->GetBottomBy(type))) return 0;
            // Recalculate for below
            leftY = BoundingBox::CalcBezierAtPosition(bottomBezier, this->GetLeftBy(type)) - margin;
            rightY = BoundingBox::CalcBezierAtPosition(bottomBezier, this->GetRightBy(type)) - margin;
            // The box is above the summit of the curve
            if ((this->GetLeftBy(type) < (p1.x + xMinY)) && (this->GetRightBy(type) > (p1.x + xMinY)))
                return (curve->GetBottomBy(type) - this->GetTopBy(type) - margin);
            // The content is on the left
            if (this->GetRightBy(type) < (p1.x + xMinY))
                return (rightY - this->GetTopBy(type));
            else
                return (leftY - this->GetTopBy(type));
        }
    }
    // The curve overflows on the left
    else if ((p1.x < this->GetLeftBy(type)) && p2.x <= this->GetRightBy(type)) {
        // LogDebug("left T-L %d %d ; B-R %d %d", this->GetTopBy(type), this->GetLeftBy(type),
        // this->GetBottom(), this->GetRight());
        if (curve->GetDir() == curvature_CURVEDIR_above) {
            int xMaxY = curve->CalcMinMaxY(topBezier);
            // The starting point is above the top
            if (p2.y > this->GetTopBy(type) + margin) return 0;
            // The left point is beyond (before) the summit of the curve
            if (this->GetLeftBy(type) < (p1.x + xMaxY))
                return (curve->GetTopBy(type) - this->GetBottomBy(type) + margin);
            // Calcultate the Y position of the curve one the left
            int leftY = BoundingBox::CalcBezierAtPosition(topBezier, this->GetLeftBy(type)) + margin;
            // LogDebug("leftY %d, %d, %d", leftY, this->GetBottomBy(type), this->GetTopBy(type));
            // The content left is below the bottom
            if (leftY < this->GetBottomBy(type)) return 0;
            // Else return the shift needed
            return (leftY - this->GetBottomBy(type));
        }
        else {
            int xMinY = curve->CalcMinMaxY(topBezier);
            // The starting point is below the bottom
            if (p2.y < this->GetBottomBy(type) + margin) return 0;
            // The left point is beyond (before) the summit of the curve
            if (this->GetLeftBy(type) < (p1.x + xMinY))
                return (curve->GetBottomBy(type) - this->GetTopBy(type) - margin);
            // Calcultate the Y position of the curve one the left
            int leftY = BoundingBox::CalcBezierAtPosition(bottomBezier, this->GetLeftBy(type)) - margin;
            // LogDebug("leftY %d, %d, %d", leftY, this->GetBottomBy(type), this->GetTopBy(type));
            // The content left is above the top
            if (leftY > this->GetTopBy(type)) return 0;
            // Else return the shift needed
            return (leftY - this->GetTopBy(type));
        }
    }
    // The curve overflows on the right
    else if ((p1.x >= this->GetLeftBy(type)) && p2.x > this->GetRightBy(type)) {
        // LogDebug("right T-L %d %d ; B-R %d %d", this->GetTopBy(type), this->GetLeftBy(type),
        // this->GetBottomBy(type), this->GetRightBy(type));
        if (curve->GetDir() == curvature_CURVEDIR_above) {
            int xMaxY = curve->CalcMinMaxY(topBezier);
            // The starting point is above the top
            if (p1.y > this->GetTopBy(type) + margin) return 0;
            // The right point is beyond the summit of the curve
            if (this->GetRightBy(type) > (p1.x + xMaxY))
                return (curve->GetTopBy(type) - this->GetBottomBy(type) + margin);
            // Calcultate the Y position of the curve one the right
            int rightY = BoundingBox::CalcBezierAtPosition(topBezier, this->GetRightBy(type)) + margin;
            // LogDebug("rightY %d, %d, %d", rightY, this->GetBottomBy(type), this->GetTopBy(type));
            // The content right is below the bottom
            if (rightY < this->GetBottomBy(type)) return 0;
            // Return the shift needed
            return (rightY - this->GetBottomBy(type));
        }
        else {
            int xMinY = curve->CalcMinMaxY(bottomBezier);
            // The starting point is below the bottom
            if (p1.y < this->GetBottomBy(type) + margin) return 0;
            // The right point is beyond the summit of the curve
            if (this->GetRightBy(type) > (p1.x + xMinY))
                return (curve->GetBottomBy(type) - this->GetTopBy(type) - margin);
            // Calcultate the Y position of the curve one the right
            int rightY = BoundingBox::CalcBezierAtPosition(bottomBezier, this->GetRightBy(type)) - margin;
            // LogDebug("rightY %d, %d, %d", rightY, this->GetBottomBy(type), this->GetTopBy(type));
            // The content right is above the top
            if (rightY > this->GetTopBy(type)) return 0;
            // Return the shift needed
            return (rightY - this->GetTopBy(type));
        }
    }
    // The curve in inside the left and right side of the content
    else if ((p1.x >= this->GetLeftBy(type)) && p2.x <= this->GetRightBy(type)) {
        // LogDebug("inside");
        if (curve->GetDir() == curvature_CURVEDIR_above) {
            return (curve->GetTopBy(type) - this->GetBottomBy(type) + margin);
        }
        else {
            return (curve->GetBottomBy(type) - this->GetTopBy(type) - margin);
        }
    }
    else {
        LogDebug("This should not happen");
    }

    return 0;
}

//----------------------------------------------------------------------------
// Static methods for BoundingBox
//----------------------------------------------------------------------------

void BoundingBox::SwapPoints(Point &p1, Point &p2)
{
    Point tmp = p1;
    p1 = p2;
    p2 = tmp;
}

void BoundingBox::Swap(int &v1, int &v2)
{
    int tmp = v1;
    v1 = v2;
    v2 = tmp;
}

Point BoundingBox::CalcPositionAfterRotation(Point point, float alpha, Point center)
{
    float s = sin(alpha);
    float c = cos(alpha);

    // translate point back to origin:
    point.x -= center.x;
    point.y -= center.y;

    // rotate point
    float xnew = point.x * c - point.y * s;
    float ynew = point.x * s + point.y * c;

    // translate point back:
    point.x = xnew + center.x;
    point.y = ynew + center.y;
    return point;
}

double BoundingBox::CalcSlope(Point const &p1, Point const &p2)
{
    if ((p1.y == p2.y) || (p1.x == p2.x)) return 0.0;

    return (double)(p2.y - p1.y) / (double)(p2.x - p1.x);
}

/*
int BoundingBox::CalcBezierAtPosition(const Point bezier[4], int x) {

    Point P;
    double t = 0.0;
    // avoid division by 0
    if (bezier[3].x != bezier[0].x) t = (double)(x - bezier[0].x) / (double)(bezier[3].x - bezier[0].x);
    t = std::min(1.0, std::max(0.0, t));

    P.y = pow((1 - t), 3) * bezier[0].y + 3 * t * pow((1 -t), 2) * bezier[1].y + 3 * (1-t) * pow(t, 2)* bezier[2].y +
pow (t, 3)* bezier[3].y;
    LogDebug("%f - %d - %d", t, x, P.y);
    return P.y;
}
*/

int BoundingBox::CalcBezierAtPosition(const Point bezier[4], int x)
{
    double t = 0.0;
    // avoid division by 0
    if (bezier[3].x != bezier[0].x) t = (double)(x - bezier[0].x) / (double)(bezier[3].x - bezier[0].x);
    t = std::min(1.0, std::max(0.0, t));

    Point p = BoundingBox::CalcDeCasteljau(bezier, t);

    return p.y;
}

void BoundingBox::CalcLinearInterpolation(Point &dest, const Point &a, const Point &b, double t)
{
    dest.x = a.x + (b.x - a.x) * t;
    dest.y = a.y + (b.y - a.y) * t;
}

Point BoundingBox::CalcPointAtBezier(const Point bezier[4], double t)
{
    Point p1, p2, p3, p4, p5;
    CalcLinearInterpolation(p1, bezier[0], bezier[1], t);
    CalcLinearInterpolation(p2, bezier[1], bezier[2], t);
    CalcLinearInterpolation(p3, bezier[2], bezier[3], t);
    CalcLinearInterpolation(p4, p1, p2, t);
    CalcLinearInterpolation(p5, p2, p3, t);
    Point midPoint;
    CalcLinearInterpolation(midPoint, p4, p5, t); // middle point on the bezier-curve
    return midPoint;
}

double BoundingBox::GetBezierThicknessCoeficient(
    const Point bezier[4], int currentThickness, double angle, int penWidth)
{
    Point top[4], bottom[4];
    CalcThickBezier(bezier, currentThickness, angle, top, bottom);

    Point topMidpoint = CalcPointAtBezier(top, 0.5);
    Point bottomMidpoint = CalcPointAtBezier(bottom, 0.5);

    int actualThickness = sqrt((topMidpoint.x - bottomMidpoint.x) * (topMidpoint.x - bottomMidpoint.x)
        + (topMidpoint.y - bottomMidpoint.y) * (topMidpoint.y - bottomMidpoint.y));
    double adjustedThickness = currentThickness - penWidth;
    if (adjustedThickness < 0) adjustedThickness = 0;
    return adjustedThickness / actualThickness;
}

Point BoundingBox::CalcDeCasteljau(const Point bezier[4], double t)
{
    Point p;

    p.x = pow((1 - t), 3) * bezier[0].x + 3 * t * pow((1 - t), 2) * bezier[1].x + 3 * (1 - t) * pow(t, 2) * bezier[2].x
        + pow(t, 3) * bezier[3].x;
    p.y = pow((1 - t), 3) * bezier[0].y + 3 * t * pow((1 - t), 2) * bezier[1].y + 3 * (1 - t) * pow(t, 2) * bezier[2].y
        + pow(t, 3) * bezier[3].y;

    return p;
}

void BoundingBox::CalcThickBezier(
    const Point bezier[4], int thickness, float angle, Point *topBezier, Point *bottomBezier)
{
    assert(topBezier && bottomBezier); // size should be 4 each

    Point c1Rotated = bezier[1];
    Point c2Rotated = bezier[2];
    c1Rotated.y += thickness * 0.5;
    c2Rotated.y += thickness * 0.5;
    if (angle != 0.0) {
        c1Rotated = BoundingBox::CalcPositionAfterRotation(c1Rotated, angle, bezier[1]);
        c2Rotated = BoundingBox::CalcPositionAfterRotation(c2Rotated, angle, bezier[2]);
    }

    topBezier[0] = bezier[0];
    bottomBezier[0] = topBezier[0];

    // Points for first bez, they go from xy to x1y1
    topBezier[1] = c1Rotated;
    topBezier[2] = c2Rotated;
    topBezier[3] = bezier[3];

    c1Rotated = bezier[1];
    c2Rotated = bezier[2];
    c1Rotated.y -= thickness * 0.5;
    c2Rotated.y -= thickness * 0.5;
    if (angle != 0.0) {
        c1Rotated = BoundingBox::CalcPositionAfterRotation(c1Rotated, angle, bezier[1]);
        c2Rotated = BoundingBox::CalcPositionAfterRotation(c2Rotated, angle, bezier[2]);
    }

    // second bez. goes back
    bottomBezier[1] = c1Rotated;
    bottomBezier[2] = c2Rotated;
    bottomBezier[3] = bezier[3];
}

void BoundingBox::ApproximateBezierBoundingBox(
    const Point bezier[4], Point &pos, int &width, int &height, int &minYPos, int &maxYPos)
{
    int ax = bezier[0].x;
    int ay = bezier[0].y;
    int bx = bezier[1].x;
    int by = bezier[1].y;
    int cx = bezier[2].x;
    int cy = bezier[2].y;
    int dx = bezier[3].x;
    int dy = bezier[3].y;

    double px, py, qx, qy, rx, ry, sx, sy, tx, ty, tobx, toby, tocx, tocy, todx, tody, toqx, toqy, torx, tory, totx,
        toty;
    int x, y, minx, miny, maxx, maxy;

    minx = miny = -VRV_UNSET;
    maxx = maxy = VRV_UNSET;

    tobx = bx - ax;
    toby = by - ay; // directions
    tocx = cx - bx;
    tocy = cy - by;
    todx = dx - cx;
    tody = dy - cy;
    double step = 1.0 / BEZIER_APPROXIMATION;
    int i;
    for (i = 0; i < (int)(BEZIER_APPROXIMATION + 1.0); ++i) {
        double d = i * step;
        px = ax + d * tobx;
        py = ay + d * toby;
        qx = bx + d * tocx;
        qy = by + d * tocy;
        rx = cx + d * todx;
        ry = cy + d * tody;
        toqx = qx - px;
        toqy = qy - py;
        torx = rx - qx;
        tory = ry - qy;

        sx = px + d * toqx;
        sy = py + d * toqy;
        tx = qx + d * torx;
        ty = qy + d * tory;
        totx = tx - sx;
        toty = ty - sy;

        x = sx + d * totx;
        y = sy + d * toty;
        minx = std::min(minx, x);
        if (miny > y) {
            miny = y;
            minYPos = (bezier[3].x - bezier[0].x) * d;
        }
        maxx = std::max(maxx, x);
        if (maxy < y) {
            maxy = y;
            maxYPos = (bezier[3].x - bezier[0].x) * d;
        }
    }
    pos.x = minx;
    pos.y = miny;
    width = maxx - minx;
    height = maxy - miny;
}

int BoundingBox::RectLeftOverlap(const Point rect1[2], const Point rect2[2], int margin, int vMargin)
{
    if ((rect1[0].y < rect2[1].y - vMargin) || (rect1[1].y > rect2[0].y + vMargin)) return 0;
    return std::max(0, rect2[1].x - rect1[0].x + margin);
}

int BoundingBox::RectRightOverlap(const Point rect1[2], const Point rect2[2], int margin, int vMargin)
{
    if ((rect1[0].y < rect2[1].y - vMargin) || (rect1[1].y > rect2[0].y + vMargin)) return 0;
    return std::max(0, rect1[1].x - rect2[0].x + margin);
}

int BoundingBox::RectTopOverlap(const Point rect1[2], const Point rect2[2], int margin, int hMargin)
{
    if ((rect1[0].x > rect2[1].x + hMargin) || (rect1[1].x < rect2[0].x - hMargin)) return 0;
    return std::max(0, rect1[1].y - rect2[0].y + margin);
}

int BoundingBox::RectBottomOverlap(const Point rect1[2], const Point rect2[2], int margin, int hMargin)
{
    if ((rect1[0].x > rect2[1].x + hMargin) || (rect1[1].x < rect2[0].x - hMargin)) return 0;
    return std::max(0, rect2[1].y - rect1[0].y + margin);
}

//----------------------------------------------------------------------------
// SegmentedLine
//----------------------------------------------------------------------------

SegmentedLine::SegmentedLine(int start, int end)
{
    if (start > end) {
        BoundingBox::Swap(start, end);
    }
    m_segments.push_back(std::make_pair(start, end));
}

void SegmentedLine::GetStartEnd(int &start, int &end, int idx)
{
    assert(idx >= 0);
    assert(idx < GetSegmentCount());

    start = m_segments.at(idx).first;
    end = m_segments.at(idx).second;
}

void SegmentedLine::AddGap(int start, int end)
{
    assert(start != end);

    if (start > end) {
        BoundingBox::Swap(start, end);
    }

    // nothing to do
    if (m_segments.empty()) return;

    // insert the gap
    ArrayOfIntPairs::iterator iter = m_segments.begin();
    while (iter != m_segments.end()) {
        // drop the segment because the gap encompass it
        if ((start <= iter->first) && (end >= iter->second)) {
            iter = m_segments.erase(iter);
            continue;
        }
        // cut the segment because the gap in within it
        if ((iter->first <= start) && (iter->second >= end)) {
            iter = m_segments.insert(iter, std::make_pair(iter->first, start));
            ++iter;
            iter->first = end;
            break;
        }
        // move the start of the segment
        if ((start < iter->first) && (end >= iter->first)) {
            iter->first = end;
        }
        // move the end of the segment
        if ((end > iter->second) && (start <= iter->second)) {
            iter->second = start;
        }
        ++iter;
    }
}

} // namespace vrv
