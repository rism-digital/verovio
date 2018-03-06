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

int BoundingBox::s_deCasteljau[4][4];

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
        LogError("Illogical values for anchor points in glyph '%02x'", m_smuflGlyph);
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
        assert(false);
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

int BoundingBox::Intersects(FloatingPositioner *curve, int margin) const
{
    assert(curve);
    assert(curve->GetObject());
    assert(curve->GetObject()->Is({ SLUR, TIE }));

    // for lisability
    Point p1 = curve->m_cuvrePoints[0];
    Point p2 = curve->m_cuvrePoints[3];

    // first check if they overlap at all
    if (p2.x < this->GetContentLeft()) return 0;
    if (p1.x > this->GetContentRight()) return 0;

    Point topBezier[4], bottomBezier[4];
    BoundingBox::CalcThickBezier(
        curve->m_cuvrePoints, curve->m_cuvreThickness, curve->m_cuvreAngle, topBezier, bottomBezier);

    // The curve overflows on both sides
    if ((p1.x < this->GetContentLeft()) && p2.x > this->GetContentRight()) {
        // LogDebug("overflows both sides");
        if (curve->m_cuvreDir == curvature_CURVEDIR_above) {
            // The curve is already below the content
            if ((curve->GetContentTop() + margin) < this->GetContentBottom()) return 0;
            int xMaxY = curve->CalcXMinMaxY(topBezier);
            // Check if the box is below (should be + margin, but because of de Casteljau different it is ommitted)
            int leftY = BoundingBox::CalcBezierAtPosition(bottomBezier, this->GetContentLeft());
            int rightY = BoundingBox::CalcBezierAtPosition(bottomBezier, this->GetContentRight());
            // Everything is underneath
            if ((leftY >= this->GetContentTop()) && (rightY >= this->GetContentTop())) return 0;
            // Recalculate for above
            leftY = BoundingBox::CalcBezierAtPosition(topBezier, this->GetContentLeft()) + margin;
            rightY = BoundingBox::CalcBezierAtPosition(topBezier, this->GetContentRight()) + margin;
            // The box is above the summit of the curve
            if ((this->GetContentLeft() < (p1.x + xMaxY)) && (this->GetContentRight() > (p1.x + xMaxY)))
                return (curve->GetContentTop() - this->GetContentBottom() + margin);
            // The content is on the left
            if (this->GetContentRight() < (p1.x + xMaxY))
                return (rightY - this->GetContentBottom());
            else
                return (leftY - this->GetContentBottom());
        }
        else {
            // The curve is already above the content
            if ((curve->GetContentBottom() - margin) > this->GetContentTop()) return 0;
            int xMinY = curve->CalcXMinMaxY(bottomBezier);
            // Check if the box is above
            int leftY = BoundingBox::CalcBezierAtPosition(topBezier, this->GetContentLeft());
            int rightY = BoundingBox::CalcBezierAtPosition(topBezier, this->GetContentRight());
            // Everything is above (should be - margin, but because of de Casteljau different it is ommitted)
            if ((leftY <= this->GetContentBottom()) && (rightY <= this->GetContentBottom())) return 0;
            // Recalculate for below
            leftY = BoundingBox::CalcBezierAtPosition(bottomBezier, this->GetContentLeft()) - margin;
            rightY = BoundingBox::CalcBezierAtPosition(bottomBezier, this->GetContentRight()) - margin;
            // The box is above the summit of the curve
            if ((this->GetContentLeft() < (p1.x + xMinY)) && (this->GetContentRight() > (p1.x + xMinY)))
                return (curve->GetContentBottom() - this->GetContentTop() - margin);
            // The content is on the left
            if (this->GetContentRight() < (p1.x + xMinY))
                return (rightY - this->GetContentTop());
            else
                return (leftY - this->GetContentTop());
        }
    }
    // The curve overflows on the left
    else if ((p1.x < this->GetContentLeft()) && p2.x <= this->GetContentRight()) {
        // LogDebug("left T-L %d %d ; B-R %d %d", this->GetContentTop(), this->GetContentLeft(),
        // this->GetContentBottom(), this->GetContentRight());
        if (curve->m_cuvreDir == curvature_CURVEDIR_above) {
            int xMaxY = curve->CalcXMinMaxY(topBezier);
            // The starting point is above the top
            if (p2.y > this->GetContentTop() + margin) return 0;
            // The left point is beyond (before) the summit of the curve
            if (this->GetContentLeft() < (p1.x + xMaxY))
                return (curve->GetContentTop() - this->GetContentBottom() + margin);
            // Calcultate the Y position of the curve one the left
            int leftY = BoundingBox::CalcBezierAtPosition(topBezier, this->GetContentLeft()) + margin;
            // LogDebug("leftY %d, %d, %d", leftY, this->GetContentBottom(), this->GetContentTop());
            // The content left is below the bottom
            if (leftY < this->GetContentBottom()) return 0;
            // Else return the shift needed
            return (leftY - this->GetContentBottom());
        }
        else {
            int xMinY = curve->CalcXMinMaxY(topBezier);
            // The starting point is below the bottom
            if (p2.y < this->GetContentBottom() + margin) return 0;
            // The left point is beyond (before) the summit of the curve
            if (this->GetContentLeft() < (p1.x + xMinY))
                return (curve->GetContentBottom() - this->GetContentTop() - margin);
            // Calcultate the Y position of the curve one the left
            int leftY = BoundingBox::CalcBezierAtPosition(bottomBezier, this->GetContentLeft()) - margin;
            // LogDebug("leftY %d, %d, %d", leftY, this->GetContentBottom(), this->GetContentTop());
            // The content left is above the top
            if (leftY > this->GetContentTop()) return 0;
            // Else return the shift needed
            return (leftY - this->GetContentTop());
        }
    }
    // The curve overflows on the right
    else if ((p1.x >= this->GetContentLeft()) && p2.x > this->GetContentRight()) {
        // LogDebug("right T-L %d %d ; B-R %d %d", this->GetContentTop(), this->GetContentLeft(),
        // this->GetContentBottom(), this->GetContentRight());
        if (curve->m_cuvreDir == curvature_CURVEDIR_above) {
            int xMaxY = curve->CalcXMinMaxY(topBezier);
            // The starting point is above the top
            if (p1.y > this->GetContentTop() + margin) return 0;
            // The right point is beyond the summit of the curve
            if (this->GetContentRight() > (p1.x + xMaxY))
                return (curve->GetContentTop() - this->GetContentBottom() + margin);
            // Calcultate the Y position of the curve one the right
            int rightY = BoundingBox::CalcBezierAtPosition(topBezier, this->GetContentRight()) + margin;
            // LogDebug("rightY %d, %d, %d", rightY, this->GetContentBottom(), this->GetContentTop());
            // The content right is below the bottom
            if (rightY < this->GetContentBottom()) return 0;
            // Return the shift needed
            return (rightY - this->GetContentBottom());
        }
        else {
            int xMinY = curve->CalcXMinMaxY(bottomBezier);
            // The starting point is below the bottom
            if (p1.y < this->GetContentBottom() + margin) return 0;
            // The right point is beyond the summit of the curve
            if (this->GetContentRight() > (p1.x + xMinY))
                return (curve->GetContentBottom() - this->GetContentTop() - margin);
            // Calcultate the Y position of the curve one the right
            int rightY = BoundingBox::CalcBezierAtPosition(bottomBezier, this->GetContentRight()) - margin;
            // LogDebug("rightY %d, %d, %d", rightY, this->GetContentBottom(), this->GetContentTop());
            // The content right is above the top
            if (rightY > this->GetContentTop()) return 0;
            // Return the shift needed
            return (rightY - this->GetContentTop());
        }
    }
    // The curve in inside the left and right side of the content
    else if ((p1.x >= this->GetContentLeft()) && p2.x <= this->GetContentRight()) {
        // LogDebug("inside");
        if (curve->m_cuvreDir == curvature_CURVEDIR_above) {
            return (curve->GetContentTop() - this->GetContentBottom() + margin);
        }
        else {
            return (curve->GetContentBottom() - this->GetContentTop() - margin);
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
    int i, j;
    double t = 0.0;
    // avoid division by 0
    if (bezier[3].x != bezier[0].x) t = (double)(x - bezier[0].x) / (double)(bezier[3].x - bezier[0].x);
    t = std::min(1.0, std::max(0.0, t));
    int n = 4;

    for (i = 0; i < n; ++i) BoundingBox::s_deCasteljau[0][i] = bezier[i].y;
    for (j = 1; j < n; ++j) {
        for (int i = 0; i < 4 - j; ++i) {
            BoundingBox::s_deCasteljau[j][i]
                = BoundingBox::s_deCasteljau[j - 1][i] * (1 - t) + BoundingBox::s_deCasteljau[j - 1][i + 1] * t;
        }
    }
    return BoundingBox::s_deCasteljau[n - 1][0];
}

void BoundingBox::CalcThickBezier(
    const Point bezier[4], int thickness, float angle, Point *topBezier, Point *bottomBezier)
{
    assert(topBezier && bottomBezier); // size should be 4 each

    Point c1Rotated = bezier[1];
    Point c2Rotated = bezier[2];
    c1Rotated.y += thickness / 2;
    c2Rotated.y += thickness / 2;
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
    c1Rotated.y -= thickness / 2;
    c2Rotated.y -= thickness / 2;
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

} // namespace vrv
