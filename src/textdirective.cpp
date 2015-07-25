/////////////////////////////////////////////////////////////////////////////
// Name:        textdirective.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "textdirective.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "measureelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// MeasureTempo
//----------------------------------------------------------------------------

MeasureTempo::MeasureTempo():
    MeasureElement("tempo-"), TempoInterface()
{
    Reset();
}


MeasureTempo::~MeasureTempo()
{
}    
    
void MeasureTempo::Reset()
{
    MeasureElement::Reset();
    TempoInterface::Reset();
}
    
} // namespace vrv
