/////////////////////////////////////////////////////////////////////////////
// Name:        measureelement.cpp
// Author:      Laurent Pugin
// Created:     2016
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

MeasureElement::MeasureElement() : FloatingObject("me")
{
    Reset();
}

MeasureElement::MeasureElement(std::string classid) : FloatingObject(classid)
{
    Reset();
}

MeasureElement::~MeasureElement()
{
}

void MeasureElement::Reset()
{
    FloatingObject::Reset();
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

} // namespace vrv
