/////////////////////////////////////////////////////////////////////////////
// Name:        mrest.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "mrest.h"

namespace vrv {

//----------------------------------------------------------------------------
// MRest
//----------------------------------------------------------------------------

MRest::MRest() : LayerElement("mrest-"), AttVisibility(), AttFermatapresent()
{
    RegisterAttClass(ATT_VISIBILITY);
    RegisterAttClass(ATT_FERMATAPRESENT);

    Reset();
}

MRest::~MRest()
{
}

void MRest::Reset()
{
    LayerElement::Reset();
    ResetVisibility();
    ResetFermatapresent();
}

} // namespace vrv
