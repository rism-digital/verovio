/////////////////////////////////////////////////////////////////////////////
// Name:        space.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "space.h"

namespace vrv {

//----------------------------------------------------------------------------
// Space
//----------------------------------------------------------------------------

Space::Space( ):
    LayerElement("space-"), DurationInterface()
{
    Reset();
}

Space::~Space()
{
}
    
void Space::Reset()
{
    LayerElement::Reset();
    DurationInterface::Reset();
}

} // namespace vrv
