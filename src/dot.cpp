/////////////////////////////////////////////////////////////////////////////
// Name:        dot.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "dot.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Dot
//----------------------------------------------------------------------------

Dot::Dot():
    LayerElement("dot-"), PositionInterface()
{
    Reset();
}

Dot::~Dot()
{
}
    
void Dot::Reset()
{
    LayerElement::Reset();
    PositionInterface::Reset();
}    

} // namespace vrv
