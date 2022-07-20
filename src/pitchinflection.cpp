/////////////////////////////////////////////////////////////////////////////
// Name:        pitchinflection.cpp
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pitchinflection.h"

//----------------------------------------------------------------------------

#include <cassert>

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
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());

    this->Reset();
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
