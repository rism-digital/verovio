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

MultiRest::MultiRest() : LayerElement("multirest-"), AttCommonPart(), AttMultiRestVis(), AttNumbered()
{
    RegisterAttClass(ATT_COMMONPART);
    RegisterAttClass(ATT_MULTIRESTVIS);
    RegisterAttClass(ATT_NUMBERED);
    Reset();
}

MultiRest::~MultiRest()
{
}

void MultiRest::Reset()
{
    LayerElement::Reset();
    ResetCommonPart();
    ResetMultiRestVis();
    ResetNumbered();
}

} // namespace vrv
