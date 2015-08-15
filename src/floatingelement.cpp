/////////////////////////////////////////////////////////////////////////////
// Name:        floatingelement.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "floatingelement.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

namespace vrv {


//----------------------------------------------------------------------------
// FloatingElement
//----------------------------------------------------------------------------

FloatingElement::FloatingElement():
    DocObject("me")
{
    Reset();
}

FloatingElement::FloatingElement(std::string classid):
    DocObject(classid)
{
    Reset();
}

FloatingElement::~FloatingElement()
{

}
    
void FloatingElement::Reset()
{
    DocObject::Reset();
}
    
} // namespace vrv
