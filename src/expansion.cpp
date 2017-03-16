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

Expansion::Expansion() : SystemElement("expansion-"), AttCommonPart(), AttPlist()
{
    RegisterAttClass(ATT_COMMONPART);
    RegisterAttClass(ATT_PLIST);

    Reset();
}

Expansion::~Expansion()
{
}

void Expansion::Reset()
{
    SystemElement::Reset();
    ResetCommonPart();
    ResetPlist();
}

//----------------------------------------------------------------------------
// Expansion functor methods
//----------------------------------------------------------------------------

} // namespace vrv
