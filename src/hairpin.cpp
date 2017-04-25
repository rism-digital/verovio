/////////////////////////////////////////////////////////////////////////////
// Name:        hairpin.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "hairpin.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "verticalaligner.h"

namespace vrv {

//----------------------------------------------------------------------------
// Hairpin
//----------------------------------------------------------------------------

Hairpin::Hairpin() : ControlElement("hairpin-"), TimeSpanningInterface(), AttColor(), AttHairpinLog(), AttPlacement()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_HAIRPINLOG);
    RegisterAttClass(ATT_PLACEMENT);

    Reset();
}

Hairpin::~Hairpin()
{
}

void Hairpin::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    ResetColor();
    ResetHairpinLog();
    ResetPlacement();
}

//----------------------------------------------------------------------------
// Hairpin functor methods
//----------------------------------------------------------------------------

} // namespace vrv
