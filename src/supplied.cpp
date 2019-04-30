/////////////////////////////////////////////////////////////////////////////
// Name:        supplied.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "supplied.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Supplied
//----------------------------------------------------------------------------

Supplied::Supplied() : EditorialElement("supplied-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Supplied::~Supplied() {}

void Supplied::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv
