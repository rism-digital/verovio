/////////////////////////////////////////////////////////////////////////////
// Name:        liquescent.cpp
// Author:      Gabby Halpin
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "liquescent.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "doc.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Liquescent
//----------------------------------------------------------------------------

Liquescent::Liquescent() : LayerElement(LIQUESCENT, "liquescent-"), PitchInterface(), PositionInterface(), AttColor()
{
    RegisterInterface(PitchInterface::GetAttClasses(), PitchInterface::IsInterface());
    RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);

    Reset();
}

Liquescent::~Liquescent() {}

void Liquescent::Reset()
{
    LayerElement::Reset();
    PitchInterface::Reset();
    PositionInterface::Reset();
    ResetColor();
}

} // namespace vrv
