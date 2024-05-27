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

Oriscus::Oriscus() : LayerElement(ORISCUS, "oriscus-"), PitchInterface(), PositionInterface(), AttColor()
{
    RegisterInterface(PitchInterface::GetAttClasses(), PitchInterface::IsInterface());
    RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);

    Reset();
}

Oriscus::~Oriscus() {}

void Oriscus::Reset()
{
    LayerElement::Reset();
    PitchInterface::Reset();
    PositionInterface::Reset();
    ResetColor();
}

} // namespace vrv
