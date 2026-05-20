/////////////////////////////////////////////////////////////////////////////
// Name:        episema.cpp
// Author:      David Rizo
// Created:     2026
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "episema.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "doc.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Episema
//----------------------------------------------------------------------------

Episema::Episema()
    : LayerElement(EPISEMA), OffsetInterface(), PitchInterface(), PositionInterface(), AttColor(), AttEpisemaVis()
{
    this->RegisterInterface(OffsetInterface::GetAttClasses(), OffsetInterface::IsInterface());
    this->RegisterInterface(PitchInterface::GetAttClasses(), PitchInterface::IsInterface());
    this->RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_EPISEMAVIS);

    this->Reset();
}

Episema::~Episema() {}

void Episema::Reset()
{
    LayerElement::Reset();
    OffsetInterface::Reset();
    PitchInterface::Reset();
    PositionInterface::Reset();
    this->ResetColor();
    this->ResetEpisemaVis();
}

} // namespace vrv
