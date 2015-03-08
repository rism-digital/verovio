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
    Reset();
}

MRest::~MRest()
{
}
    
void MRest::Reset()
{
    LayerElement::Reset();
}

} // namespace vrv