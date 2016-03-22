/////////////////////////////////////////////////////////////////////////////
// Name:        bboxdevicecontext.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "bboxdevicecontext.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "glyph.h"
#include "view.h"
#include "vrv.h"

namespace vrv {

extern "C" {
static inline double DegToRad(double deg)
{
    return (deg * M_PI) / 180.0;
}
// static inline double RadToDeg(double deg) { return (deg * 180.0) / M_PI; }
}

//----------------------------------------------------------------------------
// BBoxDeviceContext
//----------------------------------------------------------------------------

BBoxDeviceContext::BBoxDeviceContext(View *view, int width, int height) : DeviceContext()
{
    m_view = view;
    m_width = width;
    m_height = height;

    m_userScaleX = 1.0;
    m_userScaleY = 1.0;

    m_drawingText = false;
    m_alignment = LEFT;

    SetBrush(AxBLACK, AxSOLID);
    SetPen(AxBLACK, 1, AxSOLID);
}

BBoxDeviceContext::~BBoxDeviceContext()
{
}

void BBoxDeviceContext::StartGraphic(Object *object, std::string gClass, std::string gId)
{
    // add the object object
    object->BoundingBox::ResetBoundingBox();
    m_objects.push_back(object);
}

void BBoxDeviceContext::ResumeGraphic(Object *object, std::string gId)
{
    // I am not sure we actually have to reset the bounding box here...
    object->BoundingBox::ResetBoundingBox();
    m_objects.push_back(object);
}

void BBoxDeviceContext::EndGraphic(Object *object, View *view)
{
    // detach the object
    assert(m_objects.back() == object);
    m_objects.pop_back();
}

void BBoxDeviceContext::EndResumedGraphic(Object *object, View *view)
{
    // detach the object
    assert(m_objects.back() == object);
    m_objects.pop_back();
}

void BBoxDeviceContext::StartPage()
{
}

void BBoxDeviceContext::EndPage()
{
}

void BBoxDeviceContext::SetBackground(int colour, int style)
{
    // nothing to do, we do not handle Background
}

void BBoxDeviceContext::SetBackgroundMode(int mode)
{
    // nothing to do, we do not handle Background Mode
}

void BBoxDeviceContext::SetTextForeground(int colour)
{
}

void BBoxDeviceContext::SetTextBackground(int colour)
{
    // nothing to do, we do not handle Text Background Mode
}

void BBoxDeviceContext::SetLogicalOrigin(int x, int y)
{
}

void BBoxDeviceContext::SetUserScale(double xScale, double yScale)
{
    //// no idea how to handle this with the BB
    m_userScaleX = xScale;
    m_userScaleY = yScale;
}

Point BBoxDeviceContext::GetLogicalOrigin()
{
    return Point(0, 0);
}

// calculated better
void BBoxDeviceContext::DrawComplexBezierPath(Point bezier1[4], Point bezier2[4])
{
    Point pos;
    int width, height;

    ApproximateBezierBoundingBox(bezier1, &pos, &width, &height);

    LogDebug("x %d, y %d, width %d, height %d", pos.x, pos.y, width, height);

    UpdateBB(pos.x, pos.y, pos.x + width, pos.y + height);
}

void BBoxDeviceContext::DrawCircle(int x, int y, int radius)
{
    DrawEllipse(x - radius, y - radius, 2 * radius, 2 * radius);
}

void BBoxDeviceContext::DrawEllipse(int x, int y, int width, int height)
{
    UpdateBB(x, y, x + width, y + height);
}

void BBoxDeviceContext::DrawEllipticArc(int x, int y, int width, int height, double start, double end)
{
    /*
    Draws an arc of an ellipse. The current pen is used for drawing the arc
    and the current brush is used for drawing the pie. This function is
    currently only available for X window and PostScript device contexts.

    x and y specify the x and y coordinates of the upper-left corner of the
    rectangle that contains the ellipse.

    width and height specify the width and height of the rectangle that
    contains the ellipse.

    start and end specify the start and end of the arc relative to the
    three-o'clock position from the center of the rectangle. Angles are
    specified in degrees (360 is a complete circle). Positive values mean
    counter-clockwise motion. If start is equal to end, a complete ellipse
    will be drawn. */

    // known bug: SVG draws with the current pen along the radii, but this does not happen in wxMSW

    std::string s;
    // radius
    double rx = width / 2;
    double ry = height / 2;
    // center
    double xc = x + rx;
    double yc = y + ry;

    double xs, ys, xe, ye;
    xs = xc + rx * cos(DegToRad(start));
    xe = xc + rx * cos(DegToRad(end));
    ys = yc - ry * sin(DegToRad(start));
    ye = yc - ry * sin(DegToRad(end));

    /// now same as circle arc...

    double theta1 = atan2(ys - yc, xs - xc);
    double theta2 = atan2(ye - yc, xe - xc);

    int fArc; // flag for large or small arc 0 means less than 180 degrees
    if ((theta2 - theta1) > 0)
        fArc = 1;
    else
        fArc = 0;

    int fSweep;
    if (fabs(theta2 - theta1) > M_PI)
        fSweep = 1;
    else
        fSweep = 0;

    // WriteLine(StringFormat("<path d=\"M%d %d A%d %d 0.0 %d %d  %d %d \" />",
    //    int(xs), int(ys), int(rx), int(ry),
    //    fArc, fSweep, int(xe), int(ye)));

    int penWidth = m_penStack.top().GetWidth();
    if (penWidth % 2) {
        penWidth += 1;
    }
    // needs to be fixed - for now uses the entire rectangle
    UpdateBB(x - penWidth / 2, y - penWidth / 2, x + width + penWidth / 2, y + height + penWidth / 2);
}

void BBoxDeviceContext::DrawLine(int x1, int y1, int x2, int y2)
{
    if (x1 > x2) {
        int tmp = x1;
        x1 = x2;
        x2 = tmp;
    }
    if (y1 > y2) {
        int tmp = y1;
        y1 = y2;
        y2 = tmp;
    }

    int penWidth = m_penStack.top().GetWidth();
    int p1 = penWidth / 2;
    int p2 = p1;
    // how odd line width is handled might depend on the implementation of the device context.
    // however, we expect the actualy width to be shifted on the left/top
    // e.g. with 7, 4 on the left and 3 on the right
    if (penWidth % 2) {
        p1++;
    }

    UpdateBB(x1 - p1, y1 - p1, x2 + p2, y2 + p2);
}

void BBoxDeviceContext::DrawPolygon(int n, Point points[], int xoffset, int yoffset, int fill_style)
{
    if (n == 0) {
        return;
    }
    int x1 = points[0].x + xoffset;
    int x2 = x1;
    int y1 = points[0].y + yoffset;
    int y2 = y1;

    for (int i = 0; i < n; i++) {
        if (points[i].x + xoffset < x1) x1 = points[i].x + xoffset;
        if (points[i].x + xoffset > x2) x2 = points[i].x + xoffset;
        if (points[i].y + yoffset < y1) y1 = points[i].y + yoffset;
        if (points[i].y + yoffset > y2) y2 = points[i].y + yoffset;
    }
    UpdateBB(x1, y1, x2, y2);
}

void BBoxDeviceContext::DrawRectangle(int x, int y, int width, int height)
{
    DrawRoundedRectangle(x, y, width, height, 0);
}

void BBoxDeviceContext::DrawRoundedRectangle(int x, int y, int width, int height, double radius)
{
    // avoid negative heights or widths
    if (height < 0) {
        height = -height;
        y -= height;
    }
    if (width < 0) {
        width = -width;
        x -= width;
    }
    int penWidth = m_penStack.top().GetWidth();
    ;
    if (penWidth % 2) {
        penWidth += 1;
    }

    UpdateBB(x - penWidth / 2, y - penWidth / 2, x + width + penWidth / 2, y + height + penWidth / 2);
}

void BBoxDeviceContext::DrawPlaceholder(int x, int y)
{
    UpdateBB(x, y, x, y);
}

void BBoxDeviceContext::StartText(int x, int y, char alignment)
{
    assert(!m_drawingText);
    m_drawingText = true;
    m_textX = x;
    m_textY = y;
    m_textWidth = 0;
    m_textHeight = 0;
    m_textAscent = 0;
    m_textDescent = 0;
    m_alignment = alignment;
}

void BBoxDeviceContext::EndText()
{
    m_drawingText = false;
}

void BBoxDeviceContext::MoveTextTo(int x, int y)
{
    assert(m_drawingText);
    m_textX = x;
    m_textY = y;
}

void BBoxDeviceContext::DrawText(const std::string &text, const std::wstring wtext)
{
    assert(m_fontStack.top());

    TextExtend extend;
    GetTextExtent(wtext, &extend);
    m_textWidth += extend.m_width;
    // keep that maximum values for ascent and descent
    m_textAscent = std::max(m_textAscent, extend.m_ascent);
    m_textDescent = std::max(m_textDescent, extend.m_descent);
    m_textHeight = m_textAscent + m_textDescent;
    if (m_alignment == RIGHT) {
        m_textX -= extend.m_width;
    }
    else if (m_alignment == CENTER) {
        m_textX -= (extend.m_width / 2);
    }
    UpdateBB(m_textX, m_textY + m_textDescent, m_textX + m_textWidth, m_textY - m_textAscent);
}

void BBoxDeviceContext::DrawRotatedText(const std::string &text, int x, int y, double angle)
{
    // TODO
}

void BBoxDeviceContext::DrawMusicText(const std::wstring &text, int x, int y)
{
    assert(m_fontStack.top());

    int g_x, g_y, g_w, g_h;
    int lastCharWidth = 0;

    for (unsigned int i = 0; i < text.length(); i++) {
        wchar_t c = text[i];
        Glyph *glyph = Resources::GetGlyph(c);
        if (!glyph) {
            continue;
        }
        glyph->GetBoundingBox(&g_x, &g_y, &g_w, &g_h);

        int x_off = x + g_x * m_fontStack.top()->GetPointSize() / glyph->GetUnitsPerEm();
        // because we are in the drawing context, y position is already flipped
        int y_off = y - g_y * m_fontStack.top()->GetPointSize() / glyph->GetUnitsPerEm();

        UpdateBB(x_off, y_off, x_off + g_w * m_fontStack.top()->GetPointSize() / glyph->GetUnitsPerEm(),
            // idem, y position is flipped
            y_off - g_h * m_fontStack.top()->GetPointSize() / glyph->GetUnitsPerEm());

        lastCharWidth = g_w * m_fontStack.top()->GetPointSize() / glyph->GetUnitsPerEm();
        x += lastCharWidth; // move x to next char
    }
}

void BBoxDeviceContext::DrawSpline(int n, Point points[])
{
}

void BBoxDeviceContext::UpdateBB(int x1, int y1, int x2, int y2)
{
    if (m_isDeactivatedX && m_isDeactivatedY) {
        return;
    }

    // the array may not be empty
    assert(!m_objects.empty());

    // we need to store logical coordinates in the objects, we need to convert them back (this is why we need a View
    // object)
    if (!m_isDeactivatedX) (m_objects.back())->UpdateSelfBBoxX(m_view->ToLogicalX(x1), m_view->ToLogicalX(x2));
    if (!m_isDeactivatedY) (m_objects.back())->UpdateSelfBBoxY(m_view->ToLogicalY(y1), m_view->ToLogicalY(y2));

    int i;
    // Stretch the content BB of the other objects
    for (i = 0; i < (int)m_objects.size(); i++) {
        if (!m_isDeactivatedX) (m_objects.at(i))->UpdateContentBBoxX(m_view->ToLogicalX(x1), m_view->ToLogicalX(x2));
        if (!m_isDeactivatedY) (m_objects.at(i))->UpdateContentBBoxY(m_view->ToLogicalY(y1), m_view->ToLogicalY(y2));
    }
}

void BBoxDeviceContext::ApproximateBezierBoundingBox(Point bezier[], Point *pos, int *width, int *height)
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
    double step = 1.0 / 40.0; // precision
    int i;
    for (i = 0; i < 41; i++) {
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
        miny = std::min(miny, y);
        maxx = std::max(maxx, x);
        maxy = std::max(maxy, y);
    }
    pos->x = minx;
    pos->y = miny;
    (*width) = maxx - minx;
    (*height) = maxy - miny;
}

} // namespace vrv
