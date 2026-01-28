/////////////////////////////////////////////////////////////////////////////
// Name:        quilisma.cpp
// Author:      Klaus Rettinghaus
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "quilisma.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "doc.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Quilisma
//----------------------------------------------------------------------------

Quilisma::Quilisma() : LayerElement(QUILISMA), OffsetInterface(), PitchInterface(), PositionInterface(), AttColor()
{
    this->RegisterInterface(OffsetInterface::GetAttClasses(), OffsetInterface::IsInterface());
    this->RegisterInterface(PitchInterface::GetAttClasses(), PitchInterface::IsInterface());
    this->RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);

    this->Reset();
}

Quilisma::~Quilisma() {}

void Quilisma::Reset()
{
    LayerElement::Reset();
    OffsetInterface::Reset();
    PitchInterface::Reset();
    PositionInterface::Reset();
    this->ResetColor();
}

} // namespace vrv
