/////////////////////////////////////////////////////////////////////////////
// Name:        musrest.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "rest.h"

namespace vrv {

//----------------------------------------------------------------------------
// Rest
//----------------------------------------------------------------------------

Rest::Rest():
	LayerElement("rest-"), DurationInterface(), PositionInterface()
{
    // by default set automatic height
    m_pname = REST_AUTO;
}

Rest::~Rest()
{
}

bool Rest::operator==( Object& other )
{
    Rest *otherRest = dynamic_cast<Rest*>( &other );
    if ( !otherRest ) {
        return false;
    }
    if ( !this->HasIdenticalPositionInterface( otherRest ) ) {
        return false;
    }
    if ( !this->HasIdenticalDurationInterface( otherRest ) ) {
        return false;
    }
    return true;
}

} // namespace vrv