/////////////////////////////////////////////////////////////////////////////
// Name:        bboxdevicecontext.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "bboxdevicecontext.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <cassert>

//----------------------------------------------------------------------------

#include "glyph.h"
#include "view.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// BBoxDeviceContext
//----------------------------------------------------------------------------

BBoxDeviceContext::BBoxDeviceContext(View *view, int width, int height, unsigned char update)
    : DeviceContext(BBOX_DEVICE_CONTEXT)
{
    m_view = view;
    m_width = width;
    m_height = height;

    m_userScaleX = 1.0;
    m_userScaleY = 1.0;

    m_drawingText = false;
    m_textAlignment = HORIZONTALALIGNMENT_left;

    m_update = update;

    this->ResetGraphicRotation();
}

BBoxDeviceContext::~BBoxDeviceContext() {}

void BBoxDeviceContext::StartGraphic(
    Object *object, const std::string &gClass, const std::string &gId, GraphicID graphicID, bool prepend)
{
    // add the object object
    object->BoundingBox::ResetBoundingBox();
    m_objects.push_back(object);

    this->ResetGraphicRotation();
}

void BBoxDeviceContext::ResumeGraphic(Object *object, std::string gId)
{
    m_objects.push_back(object);
}

void BBoxDeviceContext::EndGraphic(Object *object, View *view)
{
    // detach the object
    assert(m_objects.back() == object);
    m_objects.pop_back();

    this->ResetGraphicRotation();
}

void BBoxDeviceContext::EndResumedGraphic(Object *object, View *view)
{
    // detach the object
    assert(m_objects.back() == object);
    m_objects.pop_back();

    this->ResetGraphicRotation();
}

void BBoxDeviceContext::RotateGraphic(Point const &orig, double angle)
{
    assert(ApproximatelyEqual(m_rotationAngle, 0.0));

    m_rotationAngle = angle;
    m_rotationOrigin = orig;
}

void BBoxDeviceContext::StartPage() {}

void BBoxDeviceContext::EndPage() {}

void BBoxDeviceContext::SetBackground(int color, int style)
{
    // nothing to do, we do not handle Background
}

void BBoxDeviceContext::SetBackgroundMode(int mode)
{
    // nothing to do, we do not handle Background Mode
}

void BBoxDeviceContext::SetTextForeground(int color) {}

void BBoxDeviceContext::SetTextBackground(int color)
{
    // nothing to do, we do not handle Text Background Mode
}

void BBoxDeviceContext::SetLogicalOrigin(int x, int y) {}

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
void BBoxDeviceContext::DrawQuadBezierPath(Point bezier[3])
{
    Point pMin = Point::Min(bezier[0], bezier[2]);
    Point pMax = Point::Max(bezier[0], bezier[2]);

    // From https://iquilezles.org/www/articles/bezierbbox/bezierbbox.htm
    if ((bezier[1].x < pMin.x) || (bezier[1].x > pMax.x) || (bezier[1].y < pMin.y) || (bezier[1].y > pMax.y)) {
        // vec2 t = clamp((p0-p1)/(p0-2.0*p1+p2),0.0,1.0);
        int tx = std::clamp((bezier[0].x - bezier[1].x) / (bezier[0].x - 2.0 * bezier[1].x + bezier[2].x), 0.0, 1.0);
        int ty = std::clamp((bezier[0].y - bezier[1].y) / (bezier[0].y - 2.0 * bezier[1].y + bezier[2].y), 0.0, 1.0);
        // vec2 s = 1.0 - t;
        int sx = 1.0 - tx;
        int sy = 1.0 - ty;
        // vec2 q = s*s*p0 + 2.0*s*t*p1 + t*t*p2;
        int qx = sx * sx * bezier[0].x + 2.0 * sx * tx * bezier[1].x + tx * tx * bezier[2].x;
        int qy = sy * sy * bezier[0].y + 2.0 * sy * ty * bezier[1].y + ty * ty * bezier[2].y;
        pMin = Point::Min(pMin, Point(qx, qy));
        pMax = Point::Max(pMax, Point(qx, qy));
    }

    this->UpdateBB(pMin.x, pMin.y, pMax.x, pMax.y);
}

void BBoxDeviceContext::DrawCubicBezierPath(Point bezier[4])
{
    Point pos;
    int width, height;
    int minYPos, maxYPos;

    BoundingBox::ApproximateBezierBoundingBox(bezier, pos, width, height, minYPos, maxYPos);
    // LogDebug("x %d, y %d, width %d, height %d", pos.x, pos.y, width, height);
    this->UpdateBB(pos.x, pos.y, pos.x + width, pos.y + height);
}

void BBoxDeviceContext::DrawCubicBezierPathFilled(Point bezier1[4], Point bezier2[4])
{
    Point pos;
    int width, height;
    int minYPos, maxYPos;

    BoundingBox::ApproximateBezierBoundingBox(bezier1, pos, width, height, minYPos, maxYPos);
    // LogDebug("x %d, y %d, width %d, height %d", pos.x, pos.y, width, height);
    this->UpdateBB(pos.x, pos.y, pos.x + width, pos.y + height);
    BoundingBox::ApproximateBezierBoundingBox(bezier2, pos, width, height, minYPos, maxYPos);
    // LogDebug("x %d, y %d, width %d, height %d", pos.x, pos.y, width, height);
    this->UpdateBB(pos.x, pos.y, pos.x + width, pos.y + height);
}

void BBoxDeviceContext::DrawBentParallelogramFilled(Point side[4], int height)
{
    this->UpdateBB(side[0].x, side[0].y, side[3].x, side[3].y + height);
}

void BBoxDeviceContext::DrawCircle(int x, int y, int radius)
{
    this->DrawEllipse(x - radius, y - radius, 2 * radius, 2 * radius);
}

void BBoxDeviceContext::DrawEllipse(int x, int y, int width, int height)
{
    this->UpdateBB(x, y, x + width, y + height);
}

void BBoxDeviceContext::DrawEllipticArc(int x, int y, int width, int height, double start, double end)
{
    const std::pair<int, int> overlap = this->GetPenWidthOverlap();

    // needs to be fixed - for now uses the entire rectangle
    this->UpdateBB(x - overlap.first, y - overlap.second, x + width + overlap.second, y + height + overlap.first);
}

void BBoxDeviceContext::DrawLine(int x1, int y1, int x2, int y2)
{
    if (x1 > x2) std::swap(x1, x2);
    if (y1 > y2) std::swap(y1, y2);

    const std::pair<int, int> overlap = this->GetPenWidthOverlap();

    this->UpdateBB(x1 - overlap.first, y1 - overlap.second, x2 + overlap.second, y2 + overlap.first);
}

void BBoxDeviceContext::DrawPolyline(int n, Point points[], bool close)
{
    // Same bounding box as corresponding polygon
    this->DrawPolygon(n, points);
}

void BBoxDeviceContext::DrawPolygon(int n, Point points[])
{
    if (n == 0) {
        return;
    }
    int x1 = points[0].x;
    int x2 = x1;
    int y1 = points[0].y;
    int y2 = y1;

    for (int i = 0; i < n; ++i) {
        x1 = std::min(x1, points[i].x);
        x2 = std::max(x2, points[i].x);
        y1 = std::min(y1, points[i].y);
        y2 = std::max(y2, points[i].y);
    }

    const std::pair<int, int> overlap = this->GetPenWidthOverlap();

    this->UpdateBB(x1 - overlap.first, y1 - overlap.second, x2 + overlap.second, y2 + overlap.first);
}

void BBoxDeviceContext::DrawRectangle(int x, int y, int width, int height)
{
    this->DrawRoundedRectangle(x, y, width, height, 0);
}

void BBoxDeviceContext::DrawRoundedRectangle(int x, int y, int width, int height, int radius)
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

    const std::pair<int, int> overlap = this->GetPenWidthOverlap();

    this->UpdateBB(x - overlap.first, y - overlap.second, x + width + overlap.second, y + height + overlap.first);
}

void BBoxDeviceContext::DrawPlaceholder(int x, int y)
{
    this->UpdateBB(x, y, x, y);
}

void BBoxDeviceContext::StartText(int x, int y, data_HORIZONTALALIGNMENT alignment)
{
    assert(!m_drawingText);
    m_drawingText = true;
    m_textX = x;
    m_textY = y;
    m_textWidth = 0;
    m_textHeight = 0;
    m_textAscent = 0;
    m_textDescent = 0;
    m_textAlignment = alignment;
}

void BBoxDeviceContext::EndText()
{
    m_drawingText = false;
}

void BBoxDeviceContext::MoveTextTo(int x, int y, data_HORIZONTALALIGNMENT alignment)
{
    assert(m_drawingText);
    m_textX = x;
    m_textY = y;
    m_textWidth = 0;
    m_textHeight = 0;
    m_textAscent = 0;
    m_textDescent = 0;
    if (alignment != HORIZONTALALIGNMENT_NONE) {
        m_textAlignment = alignment;
    }
}

void BBoxDeviceContext::MoveTextVerticallyTo(int y)
{
    assert(m_drawingText);
    // Because this is used only for smaller subscript / superscript it seems
    // better not to change the y position for the BBoxDeviceContext because
    // otherwise it moves the full bounding box - to be improve / double checked
    // m_textY = y;
}

void BBoxDeviceContext::DrawText(
    const std::string &text, const std::u32string &wtext, int x, int y, int width, int height)
{
    assert(m_fontStack.top());

    if ((x != 0) && (y != 0) && (x != VRV_UNSET) && (y != VRV_UNSET) && (width != 0) && (height != 0)
        && (width != VRV_UNSET) && (height != VRV_UNSET)) {
        m_textX = x;
        m_textY = y;
        m_textWidth = width;
        m_textHeight = height;
        m_textAscent = 0;
        m_textDescent = 0;
        this->UpdateBB(m_textX, m_textY, m_textX + m_textWidth, m_textY + m_textHeight);
    }

    else {
        if ((x != VRV_UNSET) && (y != VRV_UNSET)) {
            m_textX = x;
            m_textY = y;
            m_textWidth = 0;
            m_textHeight = 0;
            m_textAscent = 0;
            m_textDescent = 0;
        }

        TextExtend extend;
        if (m_fontStack.top()->GetSmuflFont()) {
            this->GetSmuflTextExtent(wtext, &extend);
        }
        else {
            this->GetTextExtent(wtext, &extend, true);
        }
        m_textWidth += extend.m_width;
        // keep that maximum values for ascent and descent
        m_textAscent = std::max(m_textAscent, extend.m_ascent);
        m_textDescent = std::max(m_textDescent, extend.m_descent);
        m_textHeight = m_textAscent + m_textDescent;
        if (m_textAlignment == HORIZONTALALIGNMENT_right) {
            m_textX -= extend.m_width;
        }
        else if (m_textAlignment == HORIZONTALALIGNMENT_center) {
            m_textX -= (extend.m_width / 2);
        }
        this->UpdateBB(m_textX, m_textY + m_textDescent, m_textX + m_textWidth, m_textY - m_textAscent);
    }
}

void BBoxDeviceContext::DrawRotatedText(const std::string &text, int x, int y, double angle)
{
    // TODO
}

void BBoxDeviceContext::DrawMusicText(const std::u32string &text, int x, int y, bool setSmuflGlyph)
{
    assert(m_fontStack.top());

    const Resources *resources = this->GetResources();
    assert(resources);

    int g_x, g_y, g_w, g_h;
    int lastCharWidth = 0;

    char32_t smuflGlyph = 0;
    if (setSmuflGlyph && (text.length() == 1)) smuflGlyph = text.at(0);

    for (char32_t c : text) {
        const Glyph *glyph = resources->GetGlyph(c);
        if (!glyph) {
            continue;
        }
        glyph->GetBoundingBox(g_x, g_y, g_w, g_h);
        int advX = glyph->GetHorizAdvX();

        int x_off = x + g_x * m_fontStack.top()->GetPointSize() / glyph->GetUnitsPerEm();
        // because we are in the drawing context, y position is already flipped
        int y_off = y - g_y * m_fontStack.top()->GetPointSize() / glyph->GetUnitsPerEm();

        this->UpdateBB(x_off, y_off, x_off + g_w * m_fontStack.top()->GetPointSize() / glyph->GetUnitsPerEm(),
            // idem, y position is flipped
            y_off - g_h * m_fontStack.top()->GetPointSize() / glyph->GetUnitsPerEm(), smuflGlyph);

        lastCharWidth = advX * m_fontStack.top()->GetPointSize() / glyph->GetUnitsPerEm();
        x += lastCharWidth; // move x to next char
    }
}

void BBoxDeviceContext::DrawSpline(int n, Point points[]) {}

void BBoxDeviceContext::DrawGraphicUri(int x, int y, int width, int height, const std::string &uri)
{
    this->DrawRoundedRectangle(x, y, width, height, 0);
}

void BBoxDeviceContext::DrawSvgShape(int x, int y, int width, int height, double scale, pugi::xml_node svg)
{
    this->DrawRoundedRectangle(x, y, width, height, 0);
}

void BBoxDeviceContext::UpdateBB(int x1, int y1, int x2, int y2, char32_t glyph)
{
    if (m_isDeactivatedX && m_isDeactivatedY) {
        return;
    }

    if (!ApproximatelyEqual(m_rotationAngle, 0.0)) {
        Point p1 = BoundingBox::CalcPositionAfterRotation(Point(x1, y1), DegToRad(m_rotationAngle), m_rotationOrigin);
        Point p2 = BoundingBox::CalcPositionAfterRotation(Point(x2, y2), DegToRad(m_rotationAngle), m_rotationOrigin);
        x1 = p1.x;
        y1 = p1.y;
        x2 = p2.x;
        y2 = p2.y;
    }

    // the array may not be empty
    assert(!m_objects.empty());

    // we need to store logical coordinates in the objects, we need to convert them back
    // (this is why we need a View object)
    if (!m_isDeactivatedX) {
        (m_objects.back())->UpdateSelfBBoxX(m_view->ToLogicalX(x1), m_view->ToLogicalX(x2));
        if (glyph != 0) (m_objects.back())->SetBoundingBoxGlyph(glyph, m_fontStack.top()->GetPointSize());
    }
    if (!m_isDeactivatedY) {
        (m_objects.back())->UpdateSelfBBoxY(m_view->ToLogicalY(y1), m_view->ToLogicalY(y2));
        if (glyph != 0) (m_objects.back())->SetBoundingBoxGlyph(glyph, m_fontStack.top()->GetPointSize());
    }

    // Stretch the content BB of the other objects
    for (Object *object : m_objects) {
        if (!m_isDeactivatedX) object->UpdateContentBBoxX(m_view->ToLogicalX(x1), m_view->ToLogicalX(x2));
        if (!m_isDeactivatedY) object->UpdateContentBBoxY(m_view->ToLogicalY(y1), m_view->ToLogicalY(y2));
    }
}

void BBoxDeviceContext::ResetGraphicRotation()
{
    m_rotationAngle = 0.0;
    m_rotationOrigin.x = 0;
    m_rotationOrigin.y = 0;
}

std::pair<int, int> BBoxDeviceContext::GetPenWidthOverlap() const
{
    const int penWidth = m_penStack.top().GetWidth();
    int p1 = penWidth / 2;
    int p2 = p1;

    // How odd line width is handled might depend on the implementation of the device context.
    // However, we expect the actual width to be shifted on the left/top
    // e.g., with 7, 4 on the left and 3 on the right.
    if (penWidth % 2) ++p1;

    return { p1, p2 };
}

} // namespace vrv
