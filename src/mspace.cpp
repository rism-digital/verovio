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
#include "mrest.h"

namespace vrv {

//----------------------------------------------------------------------------
// MSpace
//----------------------------------------------------------------------------

static const ClassRegistrar<MSpace> s_factory("mSpace", MSPACE);

MSpace::MSpace() : LayerElement(MSPACE)
{
    this->Reset();
}

MSpace::~MSpace() {}

void MSpace::Reset()
{
    LayerElement::Reset();
}

bool MSpace::IsSupportedChild(ClassId classId)
{
    return (classId == MREST);
}

void MSpace::InitShowMSpace()
{
    this->ClearChildren();
    MRest *mRest = new MRest();
    mRest->IsAttribute(true);
    if (!this->AddChild(mRest)) delete mRest;
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

FunctorCode MSpace::Accept(Functor &functor)
{
    return functor.VisitMSpace(this);
}

FunctorCode MSpace::Accept(ConstFunctor &functor) const
{
    return functor.VisitMSpace(this);
}

FunctorCode MSpace::AcceptEnd(Functor &functor)
{
    return functor.VisitMSpaceEnd(this);
}

FunctorCode MSpace::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitMSpaceEnd(this);
}

} // namespace vrv
