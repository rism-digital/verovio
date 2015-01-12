/////////////////////////////////////////////////////////////////////////////
// Name:        devicecontext.cpp
// Author:      Laurent Pugin
// Created:     2010
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "devicecontext.h"

//----------------------------------------------------------------------------

#include <sstream>
#include <string>

//----------------------------------------------------------------------------

#include "doc.h"

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
    
    Pen currentPen = Pen(colour, width, opacityValue);
    m_penStack.push(currentPen);
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
    
    Brush currentBrush = Brush(colour, opacityValue);
    m_brushStack.push(currentBrush);

}

void DeviceContext::ResetPen( )
{
    m_penStack.pop();
}

void DeviceContext::ResetBrush( )
{
    m_brushStack.pop();
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

//----------------------------------------------------------------------------
// FontMetricsInfo
//----------------------------------------------------------------------------

bool FontMetricsInfo::FromString(const std::string& ss)
{
    std::istringstream iss( ss );
    std::string token;

    getline( iss, token, ';');
    //
    //  Ignore the version for now
    //

    getline( iss, token, ';');
    pointSize = atoi( token.c_str() );

    getline( iss, token, ';');
    family = atoi( token.c_str() );

    getline( iss, token, ';');
    style = atoi( token.c_str() );

    getline( iss, token, ';');
    weight = atoi( token.c_str() );

    getline( iss, token, ';');
    underlined = (atoi( token.c_str() ) != 0);

    getline( iss, token, ';');
    faceName = token;
#ifndef __WXMAC__
    if( faceName.empty() )
        return false;
#endif

    getline( iss, token, ';');
    encoding = atoi( token.c_str() );;

    return true;
}

std::string FontMetricsInfo::ToString() const
{
    std::string s = StringFormat( "%d;%d;%d;%d;%d;%d;%s;%d",
             0,                                 // version
             pointSize,
             family,
             style,
             weight,
             underlined,
             faceName.c_str(),
             encoding);

    return s;
}

} // namespace vrv
