/////////////////////////////////////////////////////////////////////////////
// Name:        devicecontext.cpp
// Author:      Laurent Pugin
// Created:     2010
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "devicecontext.h"

//----------------------------------------------------------------------------

#include <assert.h>

namespace vrv {
    
//----------------------------------------------------------------------------
// DeviceContext
//----------------------------------------------------------------------------

void DeviceContext::SetPen( int colour, int width, int opacity )
{
    float opacityValue;
    
    switch ( opacity )
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

void DeviceContext::SetBrush( int colour, int opacity )
{
    float opacityValue;
    
    switch ( opacity )
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

void DeviceContext::SetFont( FontInfo *font )
{
    m_fontStack.push( font );
}
    
void DeviceContext::ResetPen( )
{
    m_penStack.pop();
}

void DeviceContext::ResetBrush( )
{
    m_brushStack.pop();
}
    
void DeviceContext::ResetFont( )
{
    m_fontStack.pop();
}

void DeviceContext::DeactivateGraphic()
{
    assert( !m_isDeactivated );
    m_isDeactivated = true;
}
    
void DeviceContext::ReactivateGraphic( )
{
    assert( m_isDeactivated );
    m_isDeactivated = false;
}

} // namespace vrv
