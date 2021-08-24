/////////////////////////////////////////////////////////////////////////////
// Name:        caesura.cpp
// Author:      Klaus Rettinghaus, Thomas Weber
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "caesura.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "verticalaligner.h"

namespace vrv {

//----------------------------------------------------------------------------
// Caesura
//----------------------------------------------------------------------------

static const ClassRegistrar<Caesura> s_factory("caesura", CAESURA);

Caesura::Caesura() : ControlElement(CAESURA, "caesura-"), TimePointInterface(), AttColor(), AttPlacementRelStaff()
{
    RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_PLACEMENTRELSTAFF);

    Reset();
}

Caesura::~Caesura() {}

void Caesura::Reset()
{
    ControlElement::Reset();
    TimePointInterface::Reset();
    ResetColor();
    ResetPlacementRelStaff();
}

} // namespace vrv
