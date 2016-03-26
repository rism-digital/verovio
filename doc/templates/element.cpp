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

Element::Element() : DocObject("element-"), AttCommon()
{
    Reset();
}

Element::~Element()
{
}

void Element::AddChildElement(ChildElement *child)
{
    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

void Element::Reset()
{
    LayerElement::Reset();
    ResetCommon();
}

} // namespace vrv
