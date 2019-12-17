/////////////////////////////////////////////////////////////////////////////
// Name:        expansion.cpp
// Author:      Klaus Rettinghaus
// Created:     22/02/2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "expansion.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Expansion
//----------------------------------------------------------------------------

Expansion::Expansion() : SystemElement("expansion-"), PlistInterface()
{
    RegisterInterface(PlistInterface::GetAttClasses(), PlistInterface::IsInterface());

    Reset();
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
