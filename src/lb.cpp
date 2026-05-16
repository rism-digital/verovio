/////////////////////////////////////////////////////////////////////////////
// Name:        lb.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "lb.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "functor.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Lb
//----------------------------------------------------------------------------

static const ClassRegistrar<Lb> s_factory("lb", LB);

Lb::Lb() : TextElement(LB)
{
    this->Reset();
}

Lb::~Lb() {}

void Lb::Reset()
{
    TextElement::Reset();
}

FunctorCode Lb::Accept(Functor &functor)
{
    return functor.VisitLb(this);
}

FunctorCode Lb::Accept(ConstFunctor &functor) const
{
    return functor.VisitLb(this);
}

FunctorCode Lb::AcceptEnd(Functor &functor)
{
    return functor.VisitLbEnd(this);
}

FunctorCode Lb::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitLbEnd(this);
}

} // namespace vrv
