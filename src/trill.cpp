/////////////////////////////////////////////////////////////////////////////
// Name:        trill.cpp
// Author:      Klaus Rettinghaus
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "trill.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "verticalaligner.h"

namespace vrv {

//----------------------------------------------------------------------------
// Trill
//----------------------------------------------------------------------------

Trill::Trill() : ControlElement("trill-"), TimePointInterface(), AttColor(), AttOrnamentaccid(), AttPlacement()
{
    RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_PLACEMENT);

    Reset();
}

Trill::~Trill()
{
}

void Trill::Reset()
{
    ControlElement::Reset();
    TimePointInterface::Reset();
    ResetColor();
    ResetOrnamentaccid();
    ResetPlacement();
}

//----------------------------------------------------------------------------
// Trill functor methods
//----------------------------------------------------------------------------

} // namespace vrv
