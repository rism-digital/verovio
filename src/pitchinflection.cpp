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

#include "functor.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// PitchInflection
//----------------------------------------------------------------------------

static const ClassRegistrar<PitchInflection> s_factory("pitchInflection", PITCHINFLECTION);

PitchInflection::PitchInflection() : ControlElement(PITCHINFLECTION), TimeSpanningInterface()
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

FunctorCode PitchInflection::Accept(Functor &functor)
{
    return functor.VisitPitchInflection(this);
}

FunctorCode PitchInflection::Accept(ConstFunctor &functor) const
{
    return functor.VisitPitchInflection(this);
}

FunctorCode PitchInflection::AcceptEnd(Functor &functor)
{
    return functor.VisitPitchInflectionEnd(this);
}

FunctorCode PitchInflection::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitPitchInflectionEnd(this);
}

} // namespace vrv
