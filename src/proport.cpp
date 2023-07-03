/////////////////////////////////////////////////////////////////////////////
// Name:        proport.cpp
// Author:      Donald Byrd
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "proport.h"

//----------------------------------------------------------------------------

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// Proport
//----------------------------------------------------------------------------

static const ClassRegistrar<Proport> s_factory("proport", PROPORT);

Proport::Proport() : LayerElement(PROPORT, "prop-"), AttDurationRatio()
{
    this->RegisterAttClass(ATT_DURATIONRATIO);

    this->Reset();
}

Proport::~Proport() {}

void Proport::Reset()
{
    LayerElement::Reset();
    this->ResetDurationRatio();
}

FunctorCode Proport::Accept(Functor &functor)
{
    return functor.VisitProport(this);
}

FunctorCode Proport::Accept(ConstFunctor &functor) const
{
    return functor.VisitProport(this);
}

FunctorCode Proport::AcceptEnd(Functor &functor)
{
    return functor.VisitProportEnd(this);
}

FunctorCode Proport::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitProportEnd(this);
}

} // namespace vrv
