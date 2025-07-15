/////////////////////////////////////////////////////////////////////////////
// Name:        oriscus.cpp
// Author:      Klaus Rettinghaus
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "oriscus.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "doc.h"
#include "vrv.h"

//------------/Users/rettinghaus/git/verovio----------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Oriscus
//----------------------------------------------------------------------------

Oriscus::Oriscus() : LayerElement(ORISCUS), PitchInterface(), PositionInterface(), AttColor()
{
    this->RegisterInterface(PitchInterface::GetAttClasses(), PitchInterface::IsInterface());
    this->RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);

    this->Reset();
}

Oriscus::~Oriscus() {}

void Oriscus::Reset()
{
    LayerElement::Reset();
    PitchInterface::Reset();
    PositionInterface::Reset();
    this->ResetColor();
}

} // namespace vrv
