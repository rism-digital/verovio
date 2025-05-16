/////////////////////////////////////////////////////////////////////////////
// Name:        breath.cpp
// Author:      Klaus Rettinghaus
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "breath.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "functor.h"
#include "verticalaligner.h"

namespace vrv {

//----------------------------------------------------------------------------
// Breath
//----------------------------------------------------------------------------

static const ClassRegistrar<Breath> s_factory("breath", BREATH);

Breath::Breath() : ControlElement(BREATH), TimePointInterface(), AttPlacementRelStaff()
{
    this->RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    this->RegisterAttClass(ATT_PLACEMENTRELSTAFF);

    this->Reset();
}

Breath::~Breath() {}

void Breath::Reset()
{
    ControlElement::Reset();
    TimePointInterface::Reset();
    this->ResetPlacementRelStaff();
}

FunctorCode Breath::Accept(Functor &functor)
{
    return functor.VisitBreath(this);
}

FunctorCode Breath::Accept(ConstFunctor &functor) const
{
    return functor.VisitBreath(this);
}

FunctorCode Breath::AcceptEnd(Functor &functor)
{
    return functor.VisitBreathEnd(this);
}

FunctorCode Breath::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitBreathEnd(this);
}

} // namespace vrv
