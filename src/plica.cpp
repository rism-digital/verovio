/////////////////////////////////////////////////////////////////////////////
// Name:        plica.cpp
// Author:      Laurent Pugin
// Created:     2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "plica.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "functor.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Plica
//----------------------------------------------------------------------------

static const ClassRegistrar<Plica> s_factory("plica", PLICA);

Plica::Plica() : LayerElement(PLICA), AttPlicaVis()
{
    this->RegisterAttClass(ATT_PLICAVIS);

    this->Reset();
}

Plica::~Plica() {}

void Plica::Reset()
{
    LayerElement::Reset();
    this->ResetPlicaVis();
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

FunctorCode Plica::Accept(Functor &functor)
{
    return functor.VisitPlica(this);
}

FunctorCode Plica::Accept(ConstFunctor &functor) const
{
    return functor.VisitPlica(this);
}

FunctorCode Plica::AcceptEnd(Functor &functor)
{
    return functor.VisitPlicaEnd(this);
}

FunctorCode Plica::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitPlicaEnd(this);
}

} // namespace vrv
