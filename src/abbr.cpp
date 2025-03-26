/////////////////////////////////////////////////////////////////////////////
// Name:        abbr.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "abbr.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Abbr
//----------------------------------------------------------------------------

static const ClassRegistrar<Abbr> s_factory("abbr", ABBR);

Abbr::Abbr() : EditorialElement(ABBR), AttSource()
{
    this->RegisterAttClass(ATT_SOURCE);

    this->Reset();
}

Abbr::~Abbr() {}

void Abbr::Reset()
{
    EditorialElement::Reset();
    this->ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv
