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

#include "glyph.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// DeviceContext
//----------------------------------------------------------------------------

ClassId DeviceContext::GetClassId() const
{
    // we should always have the method overridden
    assert(false);
    return DEVICE_CONTEXT;
};

void DeviceContext::SetPen(int colour, int width, int opacity, int dashLength)
{
    float opacityValue;

    switch (opacity) {
        case AxSOLID: opacityValue = 1.0; break;
        case AxTRANSPARENT: opacityValue = 0.0; break;
        default:
            opacityValue = 1.0; // solid brush by default
    }

    m_penStack.push(Pen(colour, width, opacityValue, dashLength));
}

void DeviceContext::SetBrush(int colour, int opacity)
{
    float opacityValue;

    switch (opacity) {
        case AxSOLID: opacityValue = 1.0; break;
        case AxTRANSPARENT: opacityValue = 0.0; break;
        default:
            opacityValue = 1.0; // solid brush by default
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

void DeviceContext::GetTextExtent(const std::string &string, TextExtend *extend)
{
    std::wstring wtext(string.begin(), string.end());
    GetTextExtent(wtext, extend);
}

void DeviceContext::GetTextExtent(const std::wstring &string, TextExtend *extend)
{
    assert(m_fontStack.top());
    assert(extend);

    extend->m_width = 0;
    extend->m_height = 0;

    Glyph *unkown = Resources::GetTextGlyph(L'o');

    for (unsigned int i = 0; i < string.length(); i++) {
        wchar_t c = string[i];
        Glyph *glyph = Resources::GetTextGlyph(c);
        if (!glyph) {
            glyph = Resources::GetGlyph(c);
        }
        if (!glyph) {
            glyph = unkown;
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

    for (unsigned int i = 0; i < string.length(); i++) {
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

    int x, y, partial_w, partial_h, adv_x;
    double tmp;

    glyph->GetBoundingBox(x, y, partial_w, partial_h);

    tmp = partial_w * m_fontStack.top()->GetPointSize();
    partial_w = ceil(tmp / (double)glyph->GetUnitsPerEm());
    tmp = partial_h * m_fontStack.top()->GetPointSize();
    partial_h = ceil(tmp / (double)glyph->GetUnitsPerEm());
    tmp = y * m_fontStack.top()->GetPointSize();
    y = ceil(tmp / (double)glyph->GetUnitsPerEm());
    tmp = x * m_fontStack.top()->GetPointSize();
    x = ceil(tmp / (double)glyph->GetUnitsPerEm());

    adv_x = glyph->GetHorizAdvX();
    tmp = adv_x * m_fontStack.top()->GetPointSize();
    adv_x = ceil(tmp / (double)glyph->GetUnitsPerEm());

    extend->m_width += std::max(partial_w + x, adv_x);
    extend->m_height = std::max(partial_h, extend->m_height);
    extend->m_ascent = std::max(partial_h + y, extend->m_ascent);
    extend->m_descent = std::max(-y, extend->m_descent);
}

} // namespace vrv
