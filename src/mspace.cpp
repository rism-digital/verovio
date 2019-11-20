/////////////////////////////////////////////////////////////////////////////
// Name:        mspace.cpp
// Author:      Klaus Rettinghaus
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "mspace.h"
//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "horizontalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// MSpace
//----------------------------------------------------------------------------

MSpace::MSpace() : LayerElement("mSpace-"), DurationInterface()
{
    RegisterInterface(DurationInterface::GetAttClasses(), DurationInterface::IsInterface());

    Reset();
}

MSpace::~MSpace() {}

void MSpace::Reset()
{
    LayerElement::Reset();
    DurationInterface::Reset();
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

} // namespace vrv
