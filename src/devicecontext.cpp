/////////////////////////////////////////////////////////////////////////////
// Name:        devicecontext.cpp
// Author:      Laurent Pugin
// Created:     2010
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "devicecontext.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <cassert>
#include <math.h>

//----------------------------------------------------------------------------

#include "boundingbox.h"
#include "doc.h"
#include "glyph.h"
#include "vrv.h"

namespace vrv {

void BezierCurve::SetControlSides(bool leftAbove, bool rightAbove)
{
    m_leftControlAbove = leftAbove;
    m_rightControlAbove = rightAbove;
}

void BezierCurve::Rotate(float angle, const Point &rotationPoint)
{
    p1 = BoundingBox::CalcPositionAfterRotation(p1, angle, rotationPoint);
    p2 = BoundingBox::CalcPositionAfterRotation(p2, angle, rotationPoint);
    c1 = BoundingBox::CalcPositionAfterRotation(c1, angle, rotationPoint);
    c2 = BoundingBox::CalcPositionAfterRotation(c2, angle, rotationPoint);
}

void BezierCurve::CalcInitialControlPointParams()
{
    const int dist = abs(p2.x - p1.x);
    this->SetControlOffset(dist / 3.0);
    this->SetControlHeight(0);
}

void BezierCurve::CalcInitialControlPointParams(const Doc *doc, float angle, int staffSize)
{
    // Note: For convex curves (both control points on the same side) we assume that the curve is rotated
    // such that p1.y == p2.y, but for curves with mixed curvature we assume that the curve is unrotated
    const int dist = abs(p2.x - p1.x);
    const int unit = doc->GetDrawingUnit(staffSize);

    // Initialize offset
    int offset = 0;
    if (m_leftControlAbove == m_rightControlAbove) {
        const double ratio = double(dist) / double(unit);
        double baseVal = (ratio > 4.0) ? 3.0 : 6.0;
        if ((ratio > 4.0) && (ratio < 32.0)) {
            // interpolate baseVal between 6.0 and 3.0
            baseVal = 8.0 - log2(ratio);
        }
        offset = dist / baseVal;
    }
    else {
        offset = dist / 12.0;
        offset = std::min(offset, 4 * unit);
    }

    m_leftControlOffset = offset;
    m_rightControlOffset = offset;

    // Initialize height
    int height = 0;
    if (m_leftControlAbove == m_rightControlAbove) {
        height = std::max<int>(dist / 5, 1.2 * unit);
        height = std::min(3 * unit, height);
        height *= doc->GetOptions()->m_slurCurveFactor.GetValue();
        height = std::min(height, 2 * doc->GetDrawingOctaveSize(staffSize));
        height = std::min<int>(height, 2 * offset * cos(angle));
    }
    else {
        height = std::max(std::abs(p2.y - p1.y), 4 * unit);
        height *= doc->GetOptions()->m_slurCurveFactor.GetValue();
    }
    this->SetControlHeight(height);
}

void BezierCurve::UpdateControlPointParams()
{
    m_leftControlOffset = c1.x - p1.x;
    m_rightControlOffset = p2.x - c2.x;
    int sign = m_leftControlAbove ? 1 : -1;
    m_leftControlHeight = sign * (c1.y - p1.y);
    sign = m_rightControlAbove ? 1 : -1;
    m_rightControlHeight = sign * (c2.y - p2.y);
}

void BezierCurve::UpdateControlPoints()
{
    c1.x = p1.x + m_leftControlOffset;
    c2.x = p2.x - m_rightControlOffset;
    int sign = m_leftControlAbove ? 1 : -1;
    c1.y = p1.y + sign * m_leftControlHeight;
    sign = m_rightControlAbove ? 1 : -1;
    c2.y = p2.y + sign * m_rightControlHeight;
}

std::pair<double, double> BezierCurve::EstimateCurveParamForControlPoints() const
{
    const double dist1 = BoundingBox::CalcDistance(p1, c1);
    const double dist2 = BoundingBox::CalcDistance(c1, c2);
    const double dist3 = BoundingBox::CalcDistance(c2, p2);
    const double distSum = dist1 + dist2 + dist3;
    if (distSum > 0.0) {
        return { dist1 / distSum, (dist1 + dist2) / distSum };
    }
    else {
        return { 0.0, 1.0 };
    }
}

//----------------------------------------------------------------------------
// DeviceContext
//----------------------------------------------------------------------------

DeviceContext::~DeviceContext()
{
    if (m_penStack.size() != 1) LogDebug("Pen stack should have only one pen");
    if (m_brushStack.size() != 1) LogDebug("Brush stack should have only one brush");
    if (!m_fontStack.empty()) LogDebug("Font stack should be empty");
}

const Resources *DeviceContext::GetResources(bool showWarning) const
{
    if (!m_resources && showWarning) LogWarning("Requested resources unavailable.");
    return m_resources;
}

void DeviceContext::SetViewBoxFactor(double ppuFactor)
{
    m_viewBoxFactor = double(DEFINITION_FACTOR) / ppuFactor;
}

void DeviceContext::SetPen(int width, PenStyle style, int dashLength, int gapLength, LineCapStyle lineCap,
    LineJoinStyle lineJoin, float opacity, int color)
{
    switch (style) {
        case PEN_SOLID: break;
        case PEN_DOT:
            dashLength = dashLength ? dashLength : 1;
            gapLength = gapLength ? gapLength : width * 3;
            break;
        case PEN_LONG_DASH:
            dashLength = dashLength ? dashLength : width * 4;
            gapLength = gapLength ? gapLength : width * 3;
            break;
        case PEN_SHORT_DASH:
            dashLength = dashLength ? dashLength : width * 2;
            gapLength = gapLength ? gapLength : width * 3;
            break;
        default: break; // solid brush by default
    }

    m_penStack.push(Pen(width, style, dashLength, gapLength, lineCap, lineJoin, opacity, color));
}

void DeviceContext::SetBrush(float opacity, int color)
{
    m_brushStack.push(Brush(opacity, color));
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
    assert(!m_fontStack.empty());
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
    std::u32string wtext(string.begin(), string.end());
    this->GetTextExtent(wtext, extend, typeSize);
}

void DeviceContext::GetTextExtent(const std::u32string &string, TextExtend *extend, bool typeSize)
{
    assert(m_fontStack.top());
    assert(extend);

    const Resources *resources = this->GetResources();
    assert(resources);

    extend->m_width = 0;
    extend->m_height = 0;

    if (typeSize) {
        this->AddGlyphToTextExtend(resources->GetTextGlyph(L'p'), extend);
        this->AddGlyphToTextExtend(resources->GetTextGlyph(L'M'), extend);
        extend->m_width = 0;
    }

    const Glyph *unknown = resources->GetTextGlyph(L'o');

    for (char32_t c : string) {
        const Glyph *glyph = resources->GetTextGlyph(c);
        if (!glyph) {
            glyph = resources->GetGlyph(c);
        }
        if (!glyph) {
            // There is no glyph for space, and we would use 'o' to increase extend width. However 'o' is wider than
            // space, which led to incorrect rendering. For the time being, set width to that of '.' instead.
            // This will probably need to be improved to change with font size/style
            if (c == U' ') {
                glyph = resources->GetTextGlyph(L'.');
            }
            else {
                glyph = unknown;
            }
        }
        this->AddGlyphToTextExtend(glyph, extend);
    }
}

void DeviceContext::GetSmuflTextExtent(const std::u32string &string, TextExtend *extend)
{
    assert(m_fontStack.top());
    assert(extend);

    const Resources *resources = this->GetResources();
    assert(resources);

    extend->m_width = 0;
    extend->m_height = 0;

    for (char32_t c : string) {
        const Glyph *glyph = resources->GetGlyph(c);
        if (!glyph) {
            continue;
        }
        this->AddGlyphToTextExtend(glyph, extend);
    }
}

void DeviceContext::AddGlyphToTextExtend(const Glyph *glyph, TextExtend *extend)
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

    const int letterSpacing = m_fontStack.top()->GetLetterSpacing();
    // This is not the first letter, add a letter spacing
    if ((letterSpacing != 0) && (extend->m_width > 0)) {
        extend->m_width += letterSpacing;
    }

    // Changed because the width should only be the sum of advX
    // Alternatively we could add what is below 0 for the first and what is beyond the advx for the last
    // extend->m_width += std::max(partialWidth + x, advX);
    extend->m_width += (advX == 0) ? partialWidth : advX;
    extend->m_height = std::max(partialHeight, extend->m_height);
    extend->m_ascent = std::max(partialHeight + y, extend->m_ascent);
    extend->m_descent = std::max(-y, extend->m_descent);
}

} // namespace vrv
