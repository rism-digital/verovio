/////////////////////////////////////////////////////////////////////////////
// Name:        view_graph.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <sstream>

//----------------------------------------------------------------------------

#include "devicecontext.h"
#include "doc.h"
#include "options.h"
#include "vrv.h"

namespace vrv {

void View::DrawVerticalLine(DeviceContext *dc, int y1, int y2, int x1, int width, int dashLength)
{
    assert(dc);

    dc->SetPen(m_currentColour, std::max(1, ToDeviceContextX(width)), AxSOLID, dashLength);
    dc->SetBrush(m_currentColour, AxSOLID);

    dc->DrawLine(ToDeviceContextX(x1), ToDeviceContextY(y1), ToDeviceContextX(x1), ToDeviceContextY(y2));

    dc->ResetPen();
    dc->ResetBrush();
    return;
}

void View::DrawHorizontalLine(DeviceContext *dc, int x1, int x2, int y1, int width, int dashLength)
{
    assert(dc);

    dc->SetPen(m_currentColour, std::max(1, ToDeviceContextX(width)), AxSOLID, dashLength);
    dc->SetBrush(m_currentColour, AxSOLID);

    dc->DrawLine(ToDeviceContextX(x1), ToDeviceContextY(y1), ToDeviceContextX(x2), ToDeviceContextY(y1));

    dc->ResetPen();
    dc->ResetBrush();
    return;
}

void View::DrawVerticalSegmentedLine(DeviceContext *dc, int x1, SegmentedLine &line, int width, int dashLength)
{
    int i, start, end;
    for (i = 0; i < line.GetSegmentCount(); i++) {
        line.GetStartEnd(start, end, i);
        DrawVerticalLine(dc, start, end, x1, width, dashLength);
    }
}

void View::DrawHorizontalSegmentedLine(DeviceContext *dc, int y1, SegmentedLine &line, int width, int dashLength)
{
    int i, start, end;
    for (i = 0; i < line.GetSegmentCount(); i++) {
        line.GetStartEnd(start, end, i);
        DrawHorizontalLine(dc, start, end, y1, width, dashLength);
    }
}

/*
 * Draw rectangle partly filled in, as specified by <fillSection>: 1=top, 2=bottom, 3=left side,
 * 4=right side; 0=don't fill in any part. FIXME: <fillSection> IS IGNORED.
 */
void View::DrawPartFilledRectangle(DeviceContext *dc, int x1, int y1, int x2, int y2, int fillSection)
{
    assert(dc); // DC cannot be NULL

    BoundingBox::Swap(y1, y2);

    // dc->SetPen(m_currentColour, 0, AxSOLID );
    // dc->SetBrush(AxWHITE, AxTRANSPARENT);
    dc->SetPen(AxBLUE, 0, AxSOLID);
    dc->SetBrush(AxRED, AxTRANSPARENT);

    dc->DrawRectangle(ToDeviceContextX(x1), ToDeviceContextY(y1), ToDeviceContextX(x2 - x1), ToDeviceContextX(y1 - y2));

    dc->ResetPen();
    dc->ResetBrush();

    return;
}

/* Draw a filled rectangle with horizontal and vertical sides. */
void View::DrawFilledRectangle(DeviceContext *dc, int x1, int y1, int x2, int y2)
{
    assert(dc);

    BoundingBox::Swap(y1, y2);

    dc->SetPen(m_currentColour, 0, AxSOLID);
    dc->SetBrush(m_currentColour, AxSOLID);

    dc->DrawRectangle(ToDeviceContextX(x1), ToDeviceContextY(y1), ToDeviceContextX(x2 - x1), ToDeviceContextX(y1 - y2));

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
    if (fill)
        dc->SetBrush(m_currentColour, AxSOLID);
    else
        dc->SetBrush(m_currentColour, AxTRANSPARENT);

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

void View::DrawDot(DeviceContext *dc, int x, int y, int staffSize)
{
    int r = std::max(ToDeviceContextX(m_doc->GetDrawingDoubleUnit(staffSize) / 5), 2);

    dc->SetPen(m_currentColour, 0, AxSOLID);
    dc->SetBrush(m_currentColour, AxSOLID);

    dc->DrawCircle(ToDeviceContextX(x), ToDeviceContextY(y), r);

    dc->ResetPen();
    dc->ResetBrush();
}

void View::DrawSmuflCode(DeviceContext *dc, int x, int y, wchar_t code, int staffSize, bool dimin, bool setBBGlyph)
{
    assert(dc);

    if (code == 0) return;

    std::wstring str;
    str.push_back(code);

    dc->SetBrush(m_currentColour, AxSOLID);
    dc->SetFont(m_doc->GetDrawingSmuflFont(staffSize, dimin));

    dc->DrawMusicText(str, ToDeviceContextX(x), ToDeviceContextY(y), setBBGlyph);

    dc->ResetFont();
    dc->ResetBrush();

    return;
}

void View::DrawSmuflLine(
    DeviceContext *dc, Point orig, int length, int staffSize, bool dimin, wchar_t fill, wchar_t start, wchar_t end)
{
    assert(dc);

    int startWidth = (start == 0) ? 0 : m_doc->GetGlyphAdvX(start, staffSize, dimin);
    int fillWidth = m_doc->GetGlyphAdvX(fill, staffSize, dimin);
    int endWidth = (end == 0) ? 0 : m_doc->GetGlyphAdvX(end, staffSize, dimin);

    if (length <= 0) return;

    // We add half a fill length for an average shorter / longer line result
    int count = (length + fillWidth / 2 - startWidth - endWidth) / fillWidth;

    dc->SetBrush(m_currentColour, AxSOLID);
    dc->SetFont(m_doc->GetDrawingSmuflFont(staffSize, dimin));

    std::wstring str;

    if (start != 0) {
        str.push_back(start);
    }

    int i;
    for (i = 0; i < count; ++i) {
        str.push_back(fill);
    }

    if (end != 0) {
        str.push_back(end);
    }

    dc->DrawMusicText(str, ToDeviceContextX(orig.x), ToDeviceContextY(orig.y), false);

    dc->ResetFont();
    dc->ResetBrush();
}

void View::DrawSmuflString(
    DeviceContext *dc, int x, int y, std::wstring s, bool center, int staffSize, bool dimin, bool setBBGlyph)
{
    assert(dc);

    int xDC = ToDeviceContextX(x);

    dc->SetBrush(m_currentColour, AxSOLID);
    dc->SetFont(m_doc->GetDrawingSmuflFont(staffSize, dimin));

    if (center) {
        TextExtend extend;
        dc->GetSmuflTextExtent(s, &extend);
        xDC -= extend.m_width / 2;
    }

    dc->DrawMusicText(s, xDC, ToDeviceContextY(y), setBBGlyph);

    dc->ResetFont();
    dc->ResetBrush();
}

void View::DrawLyricString(DeviceContext *dc, int x, int y, std::wstring s, int staffSize)
{
    assert(dc);

    std::wistringstream iss(s);
    std::wstring token;
    while (std::getline(iss, token, L'_')) {
        dc->DrawText(UTF16to8(token), token);
        // no _
        if (iss.eof()) break;

        FontInfo vrvTxt;
        vrvTxt.SetFaceName("VerovioText");
        dc->SetFont(&vrvTxt);
        std::wstring str;
        str.push_back(VRV_TEXT_E551);
        dc->DrawText(UTF16to8(str), str);
        dc->ResetFont();
    }
}

void View::DrawThickBezierCurve(DeviceContext *dc, Point bezier[4], int thickness, int staffSize, float angle)
{
    assert(dc);

    Point bez1[4], bez2[4]; // filled array with control points and end point

    BoundingBox::CalcThickBezier(bezier, thickness, angle, bez1, bez2);

    bez1[0] = ToDeviceContext(bez1[0]);
    bez1[1] = ToDeviceContext(bez1[1]);
    bez1[2] = ToDeviceContext(bez1[2]);
    bez1[3] = ToDeviceContext(bez1[3]);

    bez2[0] = ToDeviceContext(bez2[0]);
    bez2[1] = ToDeviceContext(bez2[1]);
    bez2[2] = ToDeviceContext(bez2[2]);
    bez2[3] = ToDeviceContext(bez2[3]);

    // Actually draw it
    dc->SetPen(m_currentColour, std::max(1, m_doc->GetDrawingStemWidth(staffSize) / 2), AxSOLID);
    dc->DrawComplexBezierPath(bez1, bez2);
    dc->ResetPen();
}

} // namespace vrv
