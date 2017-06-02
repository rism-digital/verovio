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

MultiRest::MultiRest() : LayerElement("multirest-"), AttLabelled(), AttMultiRestVis(), AttNumbered()
{
    RegisterAttClass(ATT_LABELLED);
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
    ResetLabelled();
    ResetMultiRestVis();
    ResetNumbered();
}

} // namespace vrv
