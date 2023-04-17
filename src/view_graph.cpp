/////////////////////////////////////////////////////////////////////////////
// Name:        view_graph.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <sstream>

//----------------------------------------------------------------------------

#include "devicecontext.h"
#include "doc.h"
#include "graphic.h"
#include "options.h"
#include "svg.h"
#include "symboldef.h"
#include "vrv.h"

namespace vrv {

void View::DrawVerticalLine(DeviceContext *dc, int y1, int y2, int x1, int width, int dashLength, int gapLength)
{
    assert(dc);

    dc->SetPen(m_currentColour, std::max(1, ToDeviceContextX(width)), AxSOLID, dashLength, gapLength);
    dc->SetBrush(m_currentColour, AxSOLID);

    dc->DrawLine(ToDeviceContextX(x1), ToDeviceContextY(y1), ToDeviceContextX(x1), ToDeviceContextY(y2));

    dc->ResetPen();
    dc->ResetBrush();
    return;
}

void View::DrawHorizontalLine(DeviceContext *dc, int x1, int x2, int y1, int width, int dashLength, int gapLength)
{
    assert(dc);

    dc->SetPen(m_currentColour, std::max(1, ToDeviceContextX(width)), AxSOLID, dashLength, gapLength);
    dc->SetBrush(m_currentColour, AxSOLID);

    dc->DrawLine(ToDeviceContextX(x1), ToDeviceContextY(y1), ToDeviceContextX(x2), ToDeviceContextY(y1));

    dc->ResetPen();
    dc->ResetBrush();
    return;
}

void View::DrawVerticalSegmentedLine(
    DeviceContext *dc, int x1, SegmentedLine &line, int width, int dashLength, int gapLength)
{
    int start, end;
    for (int i = 0; i < line.GetSegmentCount(); ++i) {
        std::tie(start, end) = line.GetStartEnd(i);
        this->DrawVerticalLine(dc, start, end, x1, width, dashLength, gapLength);
    }
}

void View::DrawHorizontalSegmentedLine(
    DeviceContext *dc, int y1, SegmentedLine &line, int width, int dashLength, int gapLength)
{
    int start, end;
    for (int i = 0; i < line.GetSegmentCount(); ++i) {
        std::tie(start, end) = line.GetStartEnd(i);
        this->DrawHorizontalLine(dc, start, end, y1, width, dashLength, gapLength);
    }
}

void View::DrawNotFilledEllipse(DeviceContext *dc, int x1, int y1, int x2, int y2, int lineThickness)
{
    assert(dc); // DC cannot be NULL

    std::swap(y1, y2);

    dc->SetPen(m_currentColour, lineThickness, AxSOLID);
    dc->SetBrush(m_currentColour, AxTRANSPARENT);

    int width = x2 - x1;
    int height = y1 - y2;

    dc->DrawEllipse(ToDeviceContextX(x1), ToDeviceContextY(y1), width, height);

    dc->ResetPen();
    dc->ResetBrush();
}

/*
 * Draw rectangle partly filled in, as specified by <fillSection>: 1=top, 2=bottom, 3=left side,
 * 4=right side; 0=don't fill in any part. FIXME: <fillSection> IS IGNORED.
 */
void View::DrawPartFilledRectangle(DeviceContext *dc, int x1, int y1, int x2, int y2, int fillSection)
{
    assert(dc); // DC cannot be NULL

    std::swap(y1, y2);

    // dc->SetPen(m_currentColour, 0, AxSOLID );
    // dc->SetBrush(AxWHITE, AxTRANSPARENT);
    dc->SetPen(AxBLUE, 0, AxSOLID);
    dc->SetBrush(AxRED, AxTRANSPARENT);

    dc->DrawRectangle(ToDeviceContextX(x1), ToDeviceContextY(y1), ToDeviceContextX(x2 - x1), ToDeviceContextX(y1 - y2));

    dc->ResetPen();
    dc->ResetBrush();

    return;
}

void View::DrawNotFilledRectangle(DeviceContext *dc, int x1, int y1, int x2, int y2, int lineThickness, int radius = 0)
{
    assert(dc); // DC cannot be NULL

    std::swap(y1, y2);

    const int penWidth = lineThickness;
    dc->SetPen(m_currentColour, penWidth, AxSOLID);
    dc->SetBrush(m_currentColour, AxTRANSPARENT);

    dc->DrawRoundedRectangle(
        ToDeviceContextX(x1), ToDeviceContextY(y1), ToDeviceContextX(x2 - x1), ToDeviceContextX(y1 - y2), radius);

    dc->ResetPen();
    dc->ResetBrush();

    return;
}

/* Draw a filled rectangle with horizontal and vertical sides. */
void View::DrawFilledRectangle(DeviceContext *dc, int x1, int y1, int x2, int y2)
{
    assert(dc);

    this->DrawFilledRoundedRectangle(dc, x1, y1, x2, y2, 0);

    return;
}

void View::DrawFilledRoundedRectangle(DeviceContext *dc, int x1, int y1, int x2, int y2, int radius)
{
    assert(dc);

    std::swap(y1, y2);

    dc->SetPen(m_currentColour, 0, AxSOLID);
    dc->SetBrush(m_currentColour, AxSOLID);

    dc->DrawRoundedRectangle(
        ToDeviceContextX(x1), ToDeviceContextY(y1), ToDeviceContextX(x2 - x1), ToDeviceContextX(y1 - y2), radius);

    dc->ResetPen();
    dc->ResetBrush();

    return;
}

/* Draw an oblique quadrilateral: specifically, a parallelogram with vertical left
    and right sides, and with opposite vertices at (x1,y1) and (x2,y2). */
void View::DrawObliquePolygon(DeviceContext *dc, int x1, int y1, int x2, int y2, int height)
{
    Point p[4];

    dc->SetPen(m_currentColour, 0, AxSOLID);
    dc->SetBrush(m_currentColour, AxSOLID);

    height = ToDeviceContextX(height);
    p[0].x = ToDeviceContextX(x1);
    p[0].y = ToDeviceContextY(y1);
    p[1].x = ToDeviceContextX(x2);
    p[1].y = ToDeviceContextY(y2);
    p[2].x = p[1].x;
    p[2].y = p[1].y - height;
    p[3].x = p[0].x;
    p[3].y = p[0].y - height;

    dc->DrawPolygon(4, p);

    dc->ResetPen();
    dc->ResetBrush();
}

/* Draw an empty ("void") diamond with its top lefthand point at (x1, y1). */

void View::DrawDiamond(DeviceContext *dc, int x1, int y1, int height, int width, bool fill, int linewidth)
{
    Point p[4];

    dc->SetPen(m_currentColour, linewidth, AxSOLID);
    if (fill) {
        dc->SetBrush(m_currentColour, AxSOLID);
    }
    else {
        dc->SetBrush(m_currentColour, AxTRANSPARENT);
    }

    int dHeight = ToDeviceContextX(height);
    int dWidth = ToDeviceContextX(width);
    p[0].x = ToDeviceContextX(x1);
    p[0].y = ToDeviceContextY(y1);
    p[1].x = ToDeviceContextX(x1 + dWidth / 2);
    p[1].y = ToDeviceContextY(y1 + dHeight / 2);
    p[2].x = p[0].x + dWidth;
    p[2].y = p[0].y;
    p[3].x = ToDeviceContextX(x1 + dWidth / 2);
    p[3].y = ToDeviceContextY(y1 - dHeight / 2);

    dc->DrawPolygon(4, p);

    dc->ResetPen();
    dc->ResetBrush();
}

void View::DrawDot(DeviceContext *dc, int x, int y, int staffSize, bool dimin)
{
    int r = std::max(ToDeviceContextX(m_doc->GetDrawingDoubleUnit(staffSize) / 5), 2);
    if (dimin) r *= m_doc->GetOptions()->m_graceFactor.GetValue();

    dc->SetPen(m_currentColour, 0, AxSOLID);
    dc->SetBrush(m_currentColour, AxSOLID);

    dc->DrawCircle(ToDeviceContextX(x), ToDeviceContextY(y), r);

    dc->ResetPen();
    dc->ResetBrush();
}

void View::DrawVerticalDots(DeviceContext *dc, int x, const SegmentedLine &line, int barlineWidth, int interval)
{
    if (line.GetSegmentCount() > 1) return;

    const auto [top, bottom] = line.GetStartEnd(0);
    const int radius = std::max(barlineWidth, 2);
    int drawingPosition = top - interval / 2;

    dc->SetPen(m_currentColour, 0, AxSOLID);
    dc->SetBrush(m_currentColour, AxSOLID);

    while (drawingPosition > bottom) {
        dc->DrawCircle(ToDeviceContextX(x), ToDeviceContextY(drawingPosition), radius);
        drawingPosition -= interval;
    }

    dc->ResetPen();
    dc->ResetBrush();
}

void View::DrawSquareBracket(DeviceContext *dc, bool leftBracket, int x, int y, int height, int width,
    int horizontalThickness, int verticalThickness)
{
    assert(dc);

    const int sign = leftBracket ? 1 : -1;

    this->DrawFilledRectangle(dc, x, y - horizontalThickness / 2, x + sign * verticalThickness,
        y + height + horizontalThickness / 2); // vertical
    this->DrawFilledRectangle(
        dc, x, y - horizontalThickness / 2, x + sign * width, y + horizontalThickness / 2); // horizontal bottom
    this->DrawFilledRectangle(dc, x, y + height - horizontalThickness / 2, x + sign * width,
        y + height + horizontalThickness / 2); // horizontal top
}

void View::DrawEnclosingBrackets(DeviceContext *dc, int x, int y, int height, int width, int offset, int bracketWidth,
    int horizontalThickness, int verticalThickness)
{
    assert(dc);

    this->DrawSquareBracket(
        dc, true, x - offset, y - offset, height + 2 * offset, bracketWidth, horizontalThickness, verticalThickness);
    this->DrawSquareBracket(dc, false, x + width + offset, y - offset, height + 2 * offset, bracketWidth,
        horizontalThickness, verticalThickness);
}

void View::DrawSmuflCode(DeviceContext *dc, int x, int y, char32_t code, int staffSize, bool dimin, bool setBBGlyph)
{
    assert(dc);

    if (code == 0) return;

    std::u32string str;
    str.push_back(code);

    dc->SetBrush(m_currentColour, AxSOLID);
    dc->SetFont(m_doc->GetDrawingSmuflFont(staffSize, dimin));

    dc->DrawMusicText(str, ToDeviceContextX(x), ToDeviceContextY(y), setBBGlyph);

    dc->ResetFont();
    dc->ResetBrush();

    return;
}

void View::DrawSmuflLine(
    DeviceContext *dc, Point orig, int length, int staffSize, bool dimin, char32_t fill, char32_t start, char32_t end)
{
    assert(dc);

    if (length <= 0) return;

    const int startWidth = (start == 0) ? 0 : m_doc->GetGlyphAdvX(start, staffSize, dimin);
    const int endWidth = (end == 0) ? 0 : m_doc->GetGlyphAdvX(end, staffSize, dimin);
    int fillWidth = m_doc->GetGlyphAdvX(fill, staffSize, dimin);

    if (fillWidth == 0) fillWidth = m_doc->GetGlyphWidth(fill, staffSize, dimin);

    // We add half a fill length for an average shorter / longer line result
    const int count = (length + fillWidth / 2 - startWidth - endWidth) / fillWidth;

    dc->SetBrush(m_currentColour, AxSOLID);
    dc->SetFont(m_doc->GetDrawingSmuflFont(staffSize, dimin));

    std::u32string str;

    if (start != 0) {
        str.push_back(start);
    }

    for (int i = 0; i < count; ++i) {
        str.push_back(fill);
    }

    if (end != 0) {
        str.push_back(end);
    }

    dc->DrawMusicText(str, ToDeviceContextX(orig.x), ToDeviceContextY(orig.y), false);

    dc->ResetFont();
    dc->ResetBrush();
}

void View::DrawSmuflString(DeviceContext *dc, int x, int y, std::u32string s, data_HORIZONTALALIGNMENT alignment,
    int staffSize, bool dimin, bool setBBGlyph)
{
    assert(dc);

    int xDC = ToDeviceContextX(x);

    dc->SetBrush(m_currentColour, AxSOLID);
    dc->SetFont(m_doc->GetDrawingSmuflFont(staffSize, dimin));

    if (alignment == HORIZONTALALIGNMENT_center) {
        TextExtend extend;
        dc->GetSmuflTextExtent(s, &extend);
        xDC -= extend.m_width / 2;
    }
    else if (alignment == HORIZONTALALIGNMENT_right) {
        TextExtend extend;
        dc->GetSmuflTextExtent(s, &extend);
        xDC -= extend.m_width;
    }

    dc->DrawMusicText(s, xDC, ToDeviceContextY(y), setBBGlyph);

    dc->ResetFont();
    dc->ResetBrush();
}

void View::DrawThickBezierCurve(
    DeviceContext *dc, Point bezier[4], int thickness, int staffSize, int penWidth, int penStyle)
{
    assert(dc);

    Point bez1[4], bez2[4]; // filled array with control points and end point

    BoundingBox::CalcThickBezier(bezier, thickness, bez1, bez2);

    bez1[0] = ToDeviceContext(bez1[0]);
    bez1[1] = ToDeviceContext(bez1[1]);
    bez1[2] = ToDeviceContext(bez1[2]);
    bez1[3] = ToDeviceContext(bez1[3]);

    bez2[0] = ToDeviceContext(bez2[0]);
    bez2[1] = ToDeviceContext(bez2[1]);
    bez2[2] = ToDeviceContext(bez2[2]);
    bez2[3] = ToDeviceContext(bez2[3]);

    // Actually draw it
    if (penStyle == AxSOLID) {
        // Solid Thick Bezier Curves are made of two beziers, filled in.
        dc->SetPen(m_currentColour, std::max(1, m_doc->GetDrawingStemWidth(staffSize) / 2), penStyle);
        dc->DrawCubicBezierPathFilled(bez1, bez2);
    }
    else {
        // Dashed or Dotted Thick Bezier Curves have a uniform line width.
        dc->SetPen(m_currentColour, thickness, penStyle);
        dc->DrawCubicBezierPath(bez1);
    }
    dc->ResetPen();
}

void View::DrawSymbolDef(DeviceContext *dc, Object *parent, SymbolDef *symbolDef, int x, int y, int staffSize,
    bool dimin, data_HORIZONTALALIGNMENT alignment)
{
    assert(dc);
    assert(symbolDef);

    TextDrawingParams params;
    params.m_x = x;
    params.m_y = y;

    // Because image y coordinates are inverted we need to adjust the y position
    params.m_y += symbolDef->GetSymbolHeight(m_doc, staffSize, dimin);

    if (alignment != HORIZONTALALIGNMENT_left) {
        const int width = symbolDef->GetSymbolWidth(m_doc, staffSize, dimin);
        params.m_x -= (alignment == HORIZONTALALIGNMENT_center) ? (width / 2) : width;
    }

    // Because thg Svg is a child of symbolDef we need to temporarily change the parent for the bounding boxes
    // to be properly propagated in the device context
    symbolDef->SetTemporaryParent(parent);

    for (Object *current : symbolDef->GetChildren()) {
        if (current->Is(GRAPHIC)) {
            Graphic *graphic = vrv_cast<Graphic *>(current);
            assert(graphic);
            this->DrawGraphic(dc, graphic, params, staffSize, dimin);
        }
        if (current->Is(SVG)) {
            Svg *svg = vrv_cast<Svg *>(current);
            assert(svg);
            this->DrawSvg(dc, svg, params, staffSize, dimin);
        }
    }

    symbolDef->ResetTemporaryParent();
}

} // namespace vrv
