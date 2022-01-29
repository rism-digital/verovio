/////////////////////////////////////////////////////////////////////////////
// Name:        tabdursym.cpp
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tabdursym.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "functorparams.h"

namespace vrv {

//----------------------------------------------------------------------------
// TabDurSym
//----------------------------------------------------------------------------

static const ClassRegistrar<TabDurSym> s_factory("tabDurSym", TABDURSYM);

TabDurSym::TabDurSym() : LayerElement(TABDURSYM, "tabdursym-"), AttNNumberLike()
{
    this->RegisterAttClass(ATT_NNUMBERLIKE);

    this->Reset();
}

TabDurSym::~TabDurSym() {}

void TabDurSym::Reset()
{
    LayerElement::Reset();
    this->ResetNNumberLike();
}

bool TabDurSym::IsSupportedChild(Object *child)
{
    return false;
}

} // namespace vrv
