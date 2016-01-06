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

//----------------------------------------------------------------------------

#include "glyph.h"
#include "vrv.h"

namespace vrv {
    
//----------------------------------------------------------------------------
// DeviceContext
//----------------------------------------------------------------------------

void DeviceContext::SetPen(int colour, int width, int opacity)
{
    float opacityValue;
    
    switch (opacity)
    {
        case AxSOLID :
            opacityValue = 1.0;
            break ;
        case AxTRANSPARENT:
            opacityValue = 0.0;
            break ;
        default :
            opacityValue = 1.0; // solid brush as default
    }
    
    m_penStack.push(Pen(colour, width, opacityValue));
}

void DeviceContext::SetBrush(int colour, int opacity)
{
    float opacityValue;
    
    switch (opacity)
    {
        case AxSOLID :
            opacityValue = 1.0;
            break ;
        case AxTRANSPARENT:
            opacityValue = 0.0;
            break ;
        default :
            opacityValue = 1.0; // solid brush as default
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
    assert(!m_isDeactivated);
    m_isDeactivated = true;
}
    
void DeviceContext::ReactivateGraphic()
{
    assert(m_isDeactivated);
    m_isDeactivated = false;
}

void DeviceContext::GetTextExtent(const std::string& string, int *w, int *h)
{
    std::wstring wtext(string.begin(), string.end());
    GetTextExtent(wtext, w, h);
}
    
void DeviceContext::GetTextExtent(const std::wstring& string, int *w, int *h)
{
    assert(m_fontStack.top());
    
    int x, y, partial_w, partial_h;
    (*w) = 0;
    (*h) = 0;
    
    Glyph *unkown = Resources::GetTextGlyph(L'o');
    
    for (unsigned int i = 0; i < string.length(); i++)
    {
        wchar_t c = string[i];
        Glyph *glyph = Resources::GetTextGlyph(c);
        if (!glyph) {
            glyph = Resources::GetGlyph(c);
        }
        if (!glyph) {
            glyph = unkown;
        }
        glyph->GetBoundingBox(&x, &y, &partial_w, &partial_h);
        
        partial_w *= m_fontStack.top()->GetPointSize();
        partial_w /= glyph->GetUnitsPerEm();
        partial_h *= m_fontStack.top()->GetPointSize();
        partial_h /= glyph->GetUnitsPerEm();
        
        (*w) += partial_w;
        (*h) = std::max(partial_h, (*h));
    }
}

void DeviceContext::GetSmuflTextExtent(const std::wstring& string, int *w, int *h)
{
    assert(m_fontStack.top());
    
    int x, y, partial_w, partial_h;
    (*w) = 0;
    (*h) = 0;
    
    for (unsigned int i = 0; i < string.length(); i++)
    {
        wchar_t c = string[i];
        Glyph *glyph = Resources::GetGlyph(c);
        if (!glyph) {
            continue;
        }
        glyph->GetBoundingBox(&x, &y, &partial_w, &partial_h);
        
        partial_w *= m_fontStack.top()->GetPointSize();
        partial_w /= glyph->GetUnitsPerEm();
        partial_h *= m_fontStack.top()->GetPointSize();
        partial_h /= glyph->GetUnitsPerEm();
        
        (*w) += partial_w;
        (*h) = std::max(partial_h, (*h));
    }
}

} // namespace vrv
