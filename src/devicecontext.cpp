/////////////////////////////////////////////////////////////////////////////
// Name:        devicecontext.cpp
// Author:      Laurent Pugin
// Created:     2010
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "devicecontext.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "boundingbox.h"
#include "doc.h"
#include "glyph.h"
#include "vrv.h"

namespace vrv {

void BezierCurve::Rotate(float angle, const Point &rotationPoint)
{
    p1 = BoundingBox::CalcPositionAfterRotation(p1, angle, rotationPoint);
    p2 = BoundingBox::CalcPositionAfterRotation(p2, angle, rotationPoint);
    c1 = BoundingBox::CalcPositionAfterRotation(c1, angle, rotationPoint);
    c2 = BoundingBox::CalcPositionAfterRotation(c2, angle, rotationPoint);
}

void BezierCurve::CalcInitialControlPointParams(Doc *doc, float angle, int staffSize)
{
    const int dist = abs(p2.x - p1.x);
    const int unit = doc->GetDrawingUnit(staffSize);

    // Initialize offset
    const double ratio = double(dist) / double(unit);
    double baseVal = (ratio > 4.0) ? 2.0 : 5.0;
    if ((ratio > 4.0) && (ratio < 32.0)) {
        // interpolate baseVal between 5.0 and 2.0
        baseVal = 7.0 - log2(ratio);
    }
    const int offset = dist / (baseVal + doc->GetOptions()->m_slurControlPoints.GetValue() / 5.0);
    m_leftControlPointOffset = offset;
    m_rightControlPointOffset = offset;

    // Initialize height
    int height = std::max<int>(
        doc->GetOptions()->m_slurMinHeight.GetValue() * unit, dist / doc->GetOptions()->m_slurHeightFactor.GetValue());
    height = std::min<int>(doc->GetOptions()->m_slurMaxHeight.GetValue() * unit, height);
    height *= sqrt(doc->GetOptions()->m_slurCurveFactor.GetValue()) / 3.0;
    height = std::min(height, 2 * doc->GetDrawingOctaveSize(staffSize));
    height = std::min<int>(height, 2 * offset * cos(angle));
    this->SetControlHeight(height);
}

void BezierCurve::UpdateControlPointParams(curvature_CURVEDIR dir)
{
    m_leftControlPointOffset = c1.x - p1.x;
    m_rightControlPointOffset = p2.x - c2.x;
    const int sign = (dir == curvature_CURVEDIR_above) ? 1 : -1;
    m_leftControlHeight = sign * (c1.y - p1.y);
    m_rightControlHeight = sign * (c2.y - p2.y);
}

void BezierCurve::UpdateControlPoints(curvature_CURVEDIR dir)
{
    c1.x = p1.x + m_leftControlPointOffset;
    c2.x = p2.x - m_rightControlPointOffset;
    const int sign = (dir == curvature_CURVEDIR_above) ? 1 : -1;
    c1.y = p1.y + sign * m_leftControlHeight;
    c2.y = p2.y + sign * m_rightControlHeight;
}

//----------------------------------------------------------------------------
// DeviceContext
//----------------------------------------------------------------------------

void DeviceContext::SetPen(int colour, int width, int opacity, int dashLength, int lineCap)
{
    float opacityValue;

    switch (opacity) {
        case AxSOLID: opacityValue = 1.0; break;
        case AxDOT:
            dashLength = dashLength ? dashLength : width * 1;
            opacityValue = 1.0;
            break;
        case AxLONG_DASH:
            dashLength = dashLength ? dashLength : width * 4;
            opacityValue = 1.0;
            break;
        case AxSHORT_DASH:
            dashLength = dashLength ? dashLength : width * 2;
            opacityValue = 1.0;
            break;
        case AxTRANSPARENT: opacityValue = 0.0; break;
        default: opacityValue = 1.0; // solid brush by default
    }

    m_penStack.push(Pen(colour, width, opacityValue, dashLength, lineCap));
}

void DeviceContext::SetBrush(int colour, int opacity)
{
    float opacityValue;

    switch (opacity) {
        case AxSOLID: opacityValue = 1.0; break;
        case AxTRANSPARENT: opacityValue = 0.0; break;
        default: opacityValue = 1.0; // solid brush by default
    }

    m_brushStack.push(Brush(colour, opacityValue));
}

void DeviceContext::SetFont(FontInfo *font)
{
    // If we have a previous font on the stack and the new font has not size,
    // pass it because we need a font size in all cases
    if ((m_fontStack.size() > 0) && (font->GetPointSize() == 0)) {
        font->SetPointSize(m_fontStack.top()->GetPointSize());
    }
    m_fontStack.push(font);
}

FontInfo *DeviceContext::GetFont()
{
    assert(m_fontStack.top());
    return m_fontStack.top();
}

void DeviceContext::ResetPen()
{
    m_penStack.pop();
}

void DeviceContext::ResetBrush()
{
    m_brushStack.pop();
}

void DeviceContext::ResetFont()
{
    m_fontStack.pop();
}

void DeviceContext::DeactivateGraphic()
{
    assert(!m_isDeactivatedX && !m_isDeactivatedY);
    m_isDeactivatedX = true;
    m_isDeactivatedY = true;
}

void DeviceContext::DeactivateGraphicX()
{
    assert(!m_isDeactivatedX && !m_isDeactivatedY);
    m_isDeactivatedX = true;
}

void DeviceContext::DeactivateGraphicY()
{
    assert(!m_isDeactivatedX && !m_isDeactivatedY);
    m_isDeactivatedY = true;
}

void DeviceContext::ReactivateGraphic()
{
    assert(m_isDeactivatedX || m_isDeactivatedY);
    m_isDeactivatedY = false;
    m_isDeactivatedX = false;
}

void DeviceContext::GetTextExtent(const std::string &string, TextExtend *extend, bool typeSize)
{
    std::wstring wtext(string.begin(), string.end());
    GetTextExtent(wtext, extend, typeSize);
}

void DeviceContext::GetTextExtent(const std::wstring &string, TextExtend *extend, bool typeSize)
{
    assert(m_fontStack.top());
    assert(extend);

    extend->m_width = 0;
    extend->m_height = 0;

    if (typeSize) {
        AddGlyphToTextExtend(Resources::GetTextGlyph(L'p'), extend);
        AddGlyphToTextExtend(Resources::GetTextGlyph(L'M'), extend);
        extend->m_width = 0;
    }

    Glyph *unkown = Resources::GetTextGlyph(L'o');

    for (unsigned int i = 0; i < string.length(); ++i) {
        wchar_t c = string[i];
        Glyph *glyph = Resources::GetTextGlyph(c);
        if (!glyph) {
            glyph = Resources::GetGlyph(c);
        }
        if (!glyph) {
            // There is no glyph for space, and we would use 'o' to increase extend width. However 'o' is wider than
            // space, which led to incorrect rendering. For the time being, set width to that of '.' instead.
            // This will probably need to be improved to change with font size/style
            if (c == L' ') {
                glyph = Resources::GetTextGlyph(L'.');
            }
            else {
                glyph = unkown;
            }
        }
        AddGlyphToTextExtend(glyph, extend);
    }
}

void DeviceContext::GetSmuflTextExtent(const std::wstring &string, TextExtend *extend)
{
    assert(m_fontStack.top());
    assert(extend);

    extend->m_width = 0;
    extend->m_height = 0;

    for (unsigned int i = 0; i < string.length(); ++i) {
        wchar_t c = string[i];
        Glyph *glyph = Resources::GetGlyph(c);
        if (!glyph) {
            continue;
        }
        AddGlyphToTextExtend(glyph, extend);
    }
}

void DeviceContext::AddGlyphToTextExtend(Glyph *glyph, TextExtend *extend)
{
    assert(glyph);
    assert(extend);

    int x, y, partialWidth, partialHeight, advX;
    double tmp;

    glyph->GetBoundingBox(x, y, partialWidth, partialHeight);

    tmp = partialWidth * m_fontStack.top()->GetPointSize();
    partialWidth = ceil(tmp / (double)glyph->GetUnitsPerEm());
    tmp = partialHeight * m_fontStack.top()->GetPointSize();
    partialHeight = ceil(tmp / (double)glyph->GetUnitsPerEm());
    tmp = y * m_fontStack.top()->GetPointSize();
    y = ceil(tmp / (double)glyph->GetUnitsPerEm());
    // Following lines were commented out because result of these assignments were not used (dead store)
    // tmp = x * m_fontStack.top()->GetPointSize();
    // x = ceil(tmp / (double)glyph->GetUnitsPerEm());

    advX = glyph->GetHorizAdvX();
    tmp = advX * m_fontStack.top()->GetPointSize();
    advX = ceil(tmp / (double)glyph->GetUnitsPerEm());

    // Changed because the width should only be the sum of advX
    // Alternatively we could add what is below 0 for the first and what is beyond the advx for the last
    // extend->m_width += std::max(partialWidth + x, advX);
    extend->m_width += (advX == 0) ? partialWidth : advX;
    extend->m_height = std::max(partialHeight, extend->m_height);
    extend->m_ascent = std::max(partialHeight + y, extend->m_ascent);
    extend->m_descent = std::max(-y, extend->m_descent);
}

} // namespace vrv
