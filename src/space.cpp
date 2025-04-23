/////////////////////////////////////////////////////////////////////////////
// Name:        space.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "space.h"
//----------------------------------------------------------------------------

#include <cassert>
#include <math.h>

//----------------------------------------------------------------------------

#include "functor.h"
#include "horizontalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Space
//----------------------------------------------------------------------------

static const ClassRegistrar<Space> s_factory("space", SPACE);

Space::Space() : LayerElement(SPACE), DurationInterface()
{
    this->RegisterInterface(DurationInterface::GetAttClasses(), DurationInterface::IsInterface());

    this->Reset();
}

Space::~Space() {}

void Space::Reset()
{
    LayerElement::Reset();
    DurationInterface::Reset();
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

FunctorCode Space::Accept(Functor &functor)
{
    return functor.VisitSpace(this);
}

FunctorCode Space::Accept(ConstFunctor &functor) const
{
    return functor.VisitSpace(this);
}

FunctorCode Space::AcceptEnd(Functor &functor)
{
    return functor.VisitSpaceEnd(this);
}

FunctorCode Space::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitSpaceEnd(this);
}

} // namespace vrv
