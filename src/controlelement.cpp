/////////////////////////////////////////////////////////////////////////////
// Name:        controlelement.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "controlelement.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// ControlElement
//----------------------------------------------------------------------------

ControlElement::ControlElement() : FloatingObject("me"), AttCommon(), AttTyped()
{
    RegisterAttClass(ATT_COMMON);
    RegisterAttClass(ATT_TYPED);

    Reset();
}

ControlElement::ControlElement(std::string classid) : FloatingObject(classid), AttCommon(), AttTyped()
{
    RegisterAttClass(ATT_COMMON);
    RegisterAttClass(ATT_TYPED);

    Reset();
}

ControlElement::~ControlElement()
{
}

void ControlElement::Reset()
{
    FloatingObject::Reset();
    ResetCommon();
    ResetTyped();
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

} // namespace vrv
