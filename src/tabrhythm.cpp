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

namespace vrv {

//----------------------------------------------------------------------------
// TabRhythm
//----------------------------------------------------------------------------

TabRhythm::TabRhythm() : LayerElement("tabrhythm-"), AttNNumberLike()
{
    RegisterAttClass(ATT_NNUMBERLIKE);

    Reset();
}

TabRhythm::~TabRhythm()
{
}

void TabRhythm::Reset()
{
    LayerElement::Reset();
    ResetNNumberLike();
}

void TabRhythm::AddChild(Object *child)
{
    // Nothing of now
    assert(false);
    
    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

} // namespace vrv
