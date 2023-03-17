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

FunctorCode MSpace::Accept(MutableFunctor &functor)
{
    return functor.VisitMSpace(this);
}

FunctorCode MSpace::Accept(ConstFunctor &functor) const
{
    return functor.VisitMSpace(this);
}

FunctorCode MSpace::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitMSpaceEnd(this);
}

FunctorCode MSpace::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitMSpaceEnd(this);
}

} // namespace vrv
