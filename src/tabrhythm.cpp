/////////////////////////////////////////////////////////////////////////////
// Name:        tabrhythm.cpp
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tabrhythm.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "functorparams.h"

namespace vrv {

//----------------------------------------------------------------------------
// TabRhythm
//----------------------------------------------------------------------------

static ClassRegistrar<TabRhythm> s_factory("tabRhythm", TABRHYTHM);

TabRhythm::TabRhythm() : LayerElement("tabrhythm-"), AttNNumberLike()
{
    RegisterAttClass(ATT_NNUMBERLIKE);

    Reset();
}

TabRhythm::~TabRhythm() {}

void TabRhythm::Reset()
{
    LayerElement::Reset();
    ResetNNumberLike();
}

bool TabRhythm::IsSupportedChild(Object *child)
{
    return false;
}

} // namespace vrv
