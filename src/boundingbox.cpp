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
#include "floatingobject.h"
#include "vrv.h"

#define BEZIER_APPROXIMATION 50.0

namespace vrv {

int BoundingBox::s_deCasteljau[4][4];

//----------------------------------------------------------------------------
// BoundingBox
//----------------------------------------------------------------------------

BoundingBox::BoundingBox()
{
    ResetBoundingBox();
}

ClassId BoundingBox::GetClassId() const
{
    // we should always have the method overridden
    assert(false);
    return BOUNDING_BOX;
};

void BoundingBox::UpdateContentBBoxX(int x1, int x2)
{
    // LogDebug("CB Was: %i %i %i %i", m_contentBB_x1, m_contentBB_y1, m_contentBB_x2, m_contentBB_y2);

    int min_x = std::min(x1, x2);
    int max_x = std::max(x1, x2);

    min_x -= m_drawingX;
    max_x -= m_drawingX;

    if (m_contentBB_x1 > min_x) m_contentBB_x1 = min_x;
    if (m_contentBB_x2 < max_x) m_contentBB_x2 = max_x;

    m_updatedBBoxX = true;
    // LogDebug("CB Is:  %i %i %i %i %s", m_contentBB_x1,m_contentBB_y1, m_contentBB_x2, m_contentBB_y2,
    // GetClassName().c_str());
}

void BoundingBox::UpdateContentBBoxY(int y1, int y2)
{
    // LogDebug("CB Was: %i %i %i %i", m_contentBB_x1, m_contentBB_y1, m_contentBB_x2, m_contentBB_y2);

    int min_y = std::min(y1, y2);
    int max_y = std::max(y1, y2);

    min_y -= m_drawingY;
    max_y -= m_drawingY;

    if (m_contentBB_y1 > min_y) m_contentBB_y1 = min_y;
    if (m_contentBB_y2 < max_y) m_contentBB_y2 = max_y;

    m_updatedBBoxY = true;
    // LogDebug("CB Is:  %i %i %i %i %s", m_contentBB_x1,m_contentBB_y1, m_contentBB_x2, m_contentBB_y2,
    // GetClassName().c_str());
}

void BoundingBox::UpdateSelfBBoxX(int x1, int x2)
{
    // LogDebug("SB Was: %i %i %i %i", m_selfBB_x1,m_selfBB_y1, m_selfBB_x2, m_selfBB_y2);

    int min_x = std::min(x1, x2);
    int max_x = std::max(x1, x2);

    min_x -= m_drawingX;
    max_x -= m_drawingX;

    if (m_selfBB_x1 > min_x) m_selfBB_x1 = min_x;
    if (m_selfBB_x2 < max_x) m_selfBB_x2 = max_x;

    m_updatedBBoxX = true;

    // LogDebug("SB Is:  %i %i %i %i", m_selfBB_x1,m_selfBB_y1, m_selfBB_x2, m_selfBB_y2);
}

void BoundingBox::UpdateSelfBBoxY(int y1, int y2)
{
    // LogDebug("SB Was: %i %i %i %i", m_selfBB_x1,m_selfBB_y1, m_selfBB_x2, m_selfBB_y2);

    int min_y = std::min(y1, y2);
    int max_y = std::max(y1, y2);

    min_y -= m_drawingY;
    max_y -= m_drawingY;

    if (m_selfBB_y1 > min_y) m_selfBB_y1 = min_y;
    if (m_selfBB_y2 < max_y) m_selfBB_y2 = max_y;

    m_updatedBBoxY = true;

    // LogDebug("SB Is:  %i %i %i %i", m_selfBB_x1,m_selfBB_y1, m_selfBB_x2, m_selfBB_y2);
}

void BoundingBox::ResetBoundingBox()
{
    m_contentBB_x1 = 0xFFFFFFF;
    m_contentBB_y1 = 0xFFFFFFF;
    m_contentBB_x2 = -0xFFFFFFF;
    m_contentBB_y2 = -0xFFFFFFF;
    m_selfBB_x1 = 0xFFFFFFF;
    m_selfBB_y1 = 0xFFFFFFF;
    m_selfBB_x2 = -0xFFFFFFF;
    m_selfBB_y2 = -0xFFFFFFF;

    m_updatedBBoxX = false;
    m_updatedBBoxY = false;
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

    m_updatedBBoxX = true;
    m_updatedBBoxY = true;
}

bool BoundingBox::HasEmptyBB()
{
    return (HasUpdatedBB() && (m_contentBB_x1 == 0) && (m_contentBB_y1 == 0) && (m_contentBB_x2 == 0)
        && (m_contentBB_y2 == 0));
}

bool BoundingBox::HasContentBB()
{
    return ((m_contentBB_x1 != 0xFFFF) && (m_contentBB_y1 != 0xFFFF) && (m_contentBB_x2 != -0xFFFF)
        && (m_contentBB_y2 != -0xFFFF));
}

bool BoundingBox::HasSelfBB()
{
    return ((m_selfBB_x1 != 0xFFFF) && (m_selfBB_y1 != 0xFFFF) && (m_selfBB_x2 != -0xFFFF) && (m_selfBB_y2 != -0xFFFF));
}

bool BoundingBox::HorizontalOverlap(const BoundingBox *other) const
{
    assert(other);

    if (this->GetContentRight() < other->GetContentLeft()) return false;
    if (this->GetContentLeft() > other->GetContentRight()) return false;
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
    assert((curve->GetObject()->Is(SLUR)) || (curve->GetObject()->Is(TIE)));

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

void BoundingBox::SwapPoints(Point *x1, Point *x2)
{
    Point a;
    a = *x1;
    *x1 = *x2;
    *x2 = a;
}

Point BoundingBox::CalcPositionAfterRotation(Point point, float rot_alpha, Point center)
{
    float s = sin(rot_alpha);
    float c = cos(rot_alpha);

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

    for (i = 0; i < n; i++) BoundingBox::s_deCasteljau[0][i] = bezier[i].y;
    for (j = 1; j < n; j++) {
        for (int i = 0; i < 4 - j; i++) {
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
    for (i = 0; i < (int)(BEZIER_APPROXIMATION + 1.0); i++) {
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

} // namespace vrv
