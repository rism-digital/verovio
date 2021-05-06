/////////////////////////////////////////////////////////////////////////////
// Name:        abbr.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "abbr.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Abbr
//----------------------------------------------------------------------------

static ClassRegistrar<Abbr> s_factory("abbr", ABBR);

Abbr::Abbr() : EditorialElement("abbr-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Abbr::~Abbr() {}

void Abbr::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv
