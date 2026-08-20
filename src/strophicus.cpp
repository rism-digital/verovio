/////////////////////////////////////////////////////////////////////////////
// Name:        strophicus.cpp
// Author:      David Rizo
// Created:     2025
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "strophicus.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "doc.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Strophicus
//----------------------------------------------------------------------------

Strophicus::Strophicus()
    : LayerElement(STROPHICUS), OffsetInterface(), PitchInterface(), PositionInterface(), AttColor()
{
    this->RegisterInterface(OffsetInterface::GetAttClasses(), OffsetInterface::IsInterface());
    this->RegisterInterface(PitchInterface::GetAttClasses(), PitchInterface::IsInterface());
    this->RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);

    this->Reset();
}

Strophicus::~Strophicus() {}

void Strophicus::Reset()
{
    LayerElement::Reset();
    OffsetInterface::Reset();
    PitchInterface::Reset();
    PositionInterface::Reset();
    this->ResetColor();
}

} // namespace vrv
