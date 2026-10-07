/////////////////////////////////////////////////////////////////////////////
// Name:        refrain.cpp
// Author:      Simon Waloschek
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "refrain.h"

//----------------------------------------------------------------------------

#include "functor.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Refrain
//----------------------------------------------------------------------------

static const ClassRegistrar<Refrain> s_factory("refrain", REFRAIN);

Refrain::Refrain() : LyricElement(REFRAIN), AttNNumberLike()
{
    this->RegisterAttClass(ATT_NNUMBERLIKE);
    this->Reset();
}

Refrain::~Refrain() {}

void Refrain::Reset()
{
    LyricElement::Reset();
    this->ResetNNumberLike();
}

FunctorCode Refrain::Accept(Functor &functor)
{
    return functor.VisitRefrain(this);
}

FunctorCode Refrain::Accept(ConstFunctor &functor) const
{
    return functor.VisitRefrain(this);
}

FunctorCode Refrain::AcceptEnd(Functor &functor)
{
    return functor.VisitRefrainEnd(this);
}

FunctorCode Refrain::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitRefrainEnd(this);
}

} // namespace vrv
