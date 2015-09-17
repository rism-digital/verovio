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

MultiRest::MultiRest():
    LayerElement("multirest-"),
    AttNumbered()
{
    RegisterAttClass(ATT_NUMBERED);
    Reset();
}

MultiRest::~MultiRest()
{
}

void MultiRest::Reset()
{
    LayerElement::Reset();
    ResetNumbered();
}

} // namespace vrv