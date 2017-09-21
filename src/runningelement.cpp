/////////////////////////////////////////////////////////////////////////////
// Name:        runningelement.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "runningelement.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "text.h"

namespace vrv {

//----------------------------------------------------------------------------
// RunningElement
//----------------------------------------------------------------------------

RunningElement::RunningElement() : Object("re"), AttTyped()
{
    RegisterAttClass(ATT_TYPED);

    Reset();
}

RunningElement::RunningElement(std::string classid) : Object(classid), AttTyped()
{
    RegisterAttClass(ATT_TYPED);

    Reset();
}

RunningElement::~RunningElement()
{
}

void RunningElement::Reset()
{
    Object::Reset();
    ResetTyped();
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

} // namespace vrv
