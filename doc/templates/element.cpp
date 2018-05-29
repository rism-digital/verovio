/////////////////////////////////////////////////////////////////////////////
// Name:        element.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "element.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "childElement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Element
//----------------------------------------------------------------------------

Element::Element() : LayerElement("element-"), AttNNumberLike()
{
    RegisterAttClass(ATT_NNUMBERLIKE);

    Reset();
}

Element::~Element()
{
}

void Element::Reset()
{
    LayerElement::Reset();
    ResetNNumberLike();
}

void Element::AddChild(Object *child)
{
    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

} // namespace vrv
