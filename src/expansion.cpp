/////////////////////////////////////////////////////////////////////////////
// Name:        expansion.cpp
// Author:      Klaus Rettinghaus
// Created:     22/02/2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "expansion.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// Expansion
//----------------------------------------------------------------------------

static const ClassRegistrar<Expansion> s_factory("expansion", EXPANSION);

Expansion::Expansion() : SystemElement(EXPANSION), PlistInterface()
{
    this->RegisterInterface(PlistInterface::GetAttClasses(), PlistInterface::IsInterface());

    this->Reset();
}

Expansion::~Expansion() {}

void Expansion::Reset()
{
    SystemElement::Reset();
    PlistInterface::Reset();
}

//----------------------------------------------------------------------------
// Expansion functor methods
//----------------------------------------------------------------------------

FunctorCode Expansion::Accept(Functor &functor)
{
    return functor.VisitExpansion(this);
}

FunctorCode Expansion::Accept(ConstFunctor &functor) const
{
    return functor.VisitExpansion(this);
}

FunctorCode Expansion::AcceptEnd(Functor &functor)
{
    return functor.VisitExpansionEnd(this);
}

FunctorCode Expansion::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitExpansionEnd(this);
}

} // namespace vrv
