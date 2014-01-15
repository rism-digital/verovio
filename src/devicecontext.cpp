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
