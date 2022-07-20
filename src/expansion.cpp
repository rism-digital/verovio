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

namespace vrv {

//----------------------------------------------------------------------------
// Expansion
//----------------------------------------------------------------------------

static const ClassRegistrar<Expansion> s_factory("expansion", EXPANSION);

Expansion::Expansion() : SystemElement(EXPANSION, "expansion-"), PlistInterface()
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

} // namespace vrv
