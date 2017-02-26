/////////////////////////////////////////////////////////////////////////////
// Name:        layerelementpart.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "layerelementpart.h"

namespace vrv {

//----------------------------------------------------------------------------
// Stem
//----------------------------------------------------------------------------

Stem::Stem() : LayerElement("stem-")
{
 
    Reset();
}

Stem::~Stem()
{
}

void Stem::Reset()
{
    LayerElement::Reset();
}

} // namespace vrv
