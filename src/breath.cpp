/////////////////////////////////////////////////////////////////////////////
// Name:        breath.cpp
// Author:      Klaus Rettinghaus
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "breath.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "verticalaligner.h"

namespace vrv {

//----------------------------------------------------------------------------
// Breath
//----------------------------------------------------------------------------

Breath::Breath() : ControlElement("breath-"), TimePointInterface(), AttColor(), AttPlacement()
{
    RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_PLACEMENT);

    Reset();
}

Breath::~Breath() {}

void Breath::Reset()
{
    ControlElement::Reset();
    TimePointInterface::Reset();
    ResetColor();
    ResetPlacement();
}

} // namespace vrv
