/////////////////////////////////////////////////////////////////////////////
// Name:        mspace.cpp
// Author:      Klaus Rettinghaus
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "mspace.h"
//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "functor.h"
#include "functorparams.h"

namespace vrv {

//----------------------------------------------------------------------------
// MSpace
//----------------------------------------------------------------------------

static const ClassRegistrar<MSpace> s_factory("mSpace", MSPACE);

MSpace::MSpace() : LayerElement(MSPACE, "mSpace-")
{
    this->Reset();
}

MSpace::~MSpace() {}

void MSpace::Reset()
{
    LayerElement::Reset();
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

FunctorCode MSpace::Visit(MutableFunctor &functor)
{
    return functor.VisitMSpace(this);
}

FunctorCode MSpace::Visit(ConstFunctor &functor) const
{
    return functor.VisitMSpace(this);
}

FunctorCode MSpace::VisitEnd(MutableFunctor &functor)
{
    return functor.VisitMSpaceEnd(this);
}

FunctorCode MSpace::VisitEnd(ConstFunctor &functor) const
{
    return functor.VisitMSpaceEnd(this);
}

} // namespace vrv
