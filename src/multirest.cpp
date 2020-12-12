/////////////////////////////////////////////////////////////////////////////
// Name:        multirest.cpp
// Author:      Rodolfo Zitellini
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "multirest.h"

namespace vrv {

//----------------------------------------------------------------------------
// MultiRest
//----------------------------------------------------------------------------

MultiRest::MultiRest()
    : LayerElement("multirest-"), PositionInterface(), AttColor(), AttMultiRestVis(), AttNumbered(), AttWidth()
{
    RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_MULTIRESTVIS);
    RegisterAttClass(ATT_NUMBERED);
    RegisterAttClass(ATT_WIDTH);
    Reset();
}

MultiRest::~MultiRest() {}

void MultiRest::Reset()
{
    LayerElement::Reset();
    PositionInterface::Reset();
    ResetColor();
    ResetMultiRestVis();
    ResetNumbered();
    ResetWidth();
}

} // namespace vrv
