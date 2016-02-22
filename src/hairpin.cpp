/////////////////////////////////////////////////////////////////////////////
// Name:        hairpin.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "hairpin.h"

//----------------------------------------------------------------------------

#include <assert.h>

namespace vrv {

//----------------------------------------------------------------------------
// Hairpin
//----------------------------------------------------------------------------

Hairpin::Hairpin() : FloatingElement("hairpin-"), TimeSpanningInterface(), AttHairpinLog(), AttPlacement()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());

    Reset();
}

Hairpin::~Hairpin()
{
}

void Hairpin::Reset()
{
    FloatingElement::Reset();
    TimeSpanningInterface::Reset();
    ResetHairpinLog();
    ResetPlacement();
}

} // namespace vrv
