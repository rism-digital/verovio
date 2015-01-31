/////////////////////////////////////////////////////////////////////////////
// Name:        measureelement.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "measureelement.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

namespace vrv {


//----------------------------------------------------------------------------
// MeasureElement
//----------------------------------------------------------------------------

MeasureElement::MeasureElement():
    DocObject("me")
{
    Reset();
}

MeasureElement::MeasureElement(std::string classid):
    DocObject(classid)
{
    Reset();
}

MeasureElement::~MeasureElement()
{

}
    
void MeasureElement::Reset()
{
    DocObject::Reset();
}
    
} // namespace vrv
