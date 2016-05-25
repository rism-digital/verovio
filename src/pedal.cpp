/////////////////////////////////////////////////////////////////////////////
// Name:        pedal.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pedal.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "aligner.h"

namespace vrv {

//----------------------------------------------------------------------------
// Pedal
//----------------------------------------------------------------------------

Pedal::Pedal() : FloatingElement("pedal-"), TimeSpanningInterface(), AttPlacement()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());

    Reset();
}

Pedal::~Pedal()
{
}

void Pedal::Reset()
{
    FloatingElement::Reset();
    TimeSpanningInterface::Reset();
    ResetPlacement();
}

//----------------------------------------------------------------------------
// Pedal functor methods
//----------------------------------------------------------------------------

} // namespace vrv
