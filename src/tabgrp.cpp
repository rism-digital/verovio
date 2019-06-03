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

TabGrp::TabGrp() : LayerElement("tabgrp-"), DurationInterface()
{
    RegisterInterface(DurationInterface::GetAttClasses(), DurationInterface::IsInterface());

    Reset();
}

TabGrp::~TabGrp()
{
}

void TabGrp::Reset()
{
    LayerElement::Reset();
    DurationInterface::Reset();
}

void TabGrp::AddChild(Object *child)
{
    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

} // namespace vrv
