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

FunctorCode Proport::Visit(MutableFunctor &functor)
{
    return functor.VisitProport(this);
}

FunctorCode Proport::Visit(ConstFunctor &functor) const
{
    return functor.VisitProport(this);
}

FunctorCode Proport::VisitEnd(MutableFunctor &functor)
{
    return functor.VisitProportEnd(this);
}

FunctorCode Proport::VisitEnd(ConstFunctor &functor) const
{
    return functor.VisitProportEnd(this);
}

} // namespace vrv
