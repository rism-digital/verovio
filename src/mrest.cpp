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

MRest::MRest() : LayerElement("mrest-"), PositionInterface(), AttVisibility(), AttFermatapresent(), AttRelativesize()
{
    RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    RegisterAttClass(ATT_VISIBILITY);
    RegisterAttClass(ATT_FERMATAPRESENT);
    RegisterAttClass(ATT_RELATIVESIZE);

    Reset();
}

MRest::~MRest()
{
}

void MRest::Reset()
{
    LayerElement::Reset();
    PositionInterface::Reset();
    ResetVisibility();
    ResetFermatapresent();
    ResetRelativesize();
}

} // namespace vrv
