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

Quilisma::Quilisma() : LayerElement(QUILISMA, "quilisma-"), PitchInterface(), PositionInterface(), AttColor()
{
    RegisterInterface(PitchInterface::GetAttClasses(), PitchInterface::IsInterface());
    RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);

    Reset();
}

Quilisma::~Quilisma() {}

void Quilisma::Reset()
{
    LayerElement::Reset();
    PitchInterface::Reset();
    PositionInterface::Reset();
    ResetColor();
}

} // namespace vrv
