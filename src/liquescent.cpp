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

Liquescent::Liquescent()
    : LayerElement(LIQUESCENT), OffsetInterface(), PitchInterface(), PositionInterface(), AttColor()
{
    this->RegisterInterface(OffsetInterface::GetAttClasses(), OffsetInterface::IsInterface());
    this->RegisterInterface(PitchInterface::GetAttClasses(), PitchInterface::IsInterface());
    this->RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);

    this->Reset();
}

Liquescent::~Liquescent() {}

void Liquescent::Reset()
{
    LayerElement::Reset();
    OffsetInterface::Reset();
    PitchInterface::Reset();
    PositionInterface::Reset();
    this->ResetColor();
}

} // namespace vrv
