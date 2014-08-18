/////////////////////////////////////////////////////////////////////////////
// Name:        element.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "element.h"

//----------------------------------------------------------------------------

#include "childElement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Element
//----------------------------------------------------------------------------

Element::Element():
    DocObject("element-")
{
}

Element::~Element()
{
}

void Element::AddChildElement(ChildElement *child) {
    
    child->SetParent( this );
    m_children.push_back(child);
    Modify();
}

} // namespace vrv
