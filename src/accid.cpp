/////////////////////////////////////////////////////////////////////////////
// Name:        accid.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "accid.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Accid
//----------------------------------------------------------------------------

Accid::Accid():
    LayerElement("accid-"), PositionInterface(),
    AttAccidental()
{
    Reset();
}

Accid::~Accid()
{
}
    
void Accid::Reset()
{
    LayerElement::Reset();
    PositionInterface::Reset();
    
    ResetAccidental();
}
    

} // namespace vrv
