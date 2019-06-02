/////////////////////////////////////////////////////////////////////////////
// Name:        tabgrp.cpp
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tabgrp.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// TabGrp
//----------------------------------------------------------------------------

TabGrp::TabGrp() : LayerElement("element-"), AttNNumberLike()
{
    RegisterAttClass(ATT_NNUMBERLIKE);

    Reset();
}

TabGrp::~TabGrp()
{
}

void TabGrp::Reset()
{
    LayerElement::Reset();
    ResetNNumberLike();
}

void TabGrp::AddChild(Object *child)
{
    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

} // namespace vrv
