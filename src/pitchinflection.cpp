/////////////////////////////////////////////////////////////////////////////
// Name:        pitchinflection.cpp
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pitchinflection.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// PitchInflection
//----------------------------------------------------------------------------

static const ClassRegistrar<PitchInflection> s_factory("pitchInflection", PITCHINFLECTION);

PitchInflection::PitchInflection() : ControlElement(PITCHINFLECTION, "pinflexion-"), TimeSpanningInterface()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());

    Reset();
}

PitchInflection::~PitchInflection() {}

void PitchInflection::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
}

//----------------------------------------------------------------------------
// PitchInflection functor methods
//----------------------------------------------------------------------------

} // namespace vrv
