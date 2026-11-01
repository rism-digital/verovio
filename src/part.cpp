/////////////////////////////////////////////////////////////////////////////
// Name:        part.cpp
// Author:      Laurent Pugin
// Created:     2026
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "part.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "functor.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Part
//----------------------------------------------------------------------------

static const ClassRegistrar<Part> s_factory("part", PART);

Part::Part() : Part(true) {}

Part::Part(bool createScoreDef) : ScoreOrPartElement(PART, createScoreDef)
{
    this->Reset();
}

Part::~Part() {}

void Part::Reset()
{
    ScoreOrPartElement::Reset();
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode Part::Accept(Functor &functor)
{
    return functor.VisitPart(this);
}

FunctorCode Part::Accept(ConstFunctor &functor) const
{
    return functor.VisitPart(this);
}

FunctorCode Part::AcceptEnd(Functor &functor)
{
    return functor.VisitPartEnd(this);
}

FunctorCode Part::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitPartEnd(this);
}

} // namespace vrv
