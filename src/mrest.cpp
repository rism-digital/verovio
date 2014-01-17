/////////////////////////////////////////////////////////////////////////////
// Name:        mrest.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "mrest.h"

namespace vrv {

//----------------------------------------------------------------------------
// MRest
//----------------------------------------------------------------------------

MRest::MRest( ):
    LayerElement("mrest-")
{
}

MRest::~MRest()
{
}

bool MRest::operator==( Object& other )
{
    MRest *otherMRest = dynamic_cast<MRest*>( &other );
    if ( !otherMRest ) {
        return false;
    }
    return true;
}

} // namespace vrv