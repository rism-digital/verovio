/////////////////////////////////////////////////////////////////////////////
// Name:        supplied.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "supplied.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Supplied
//----------------------------------------------------------------------------

static const ClassRegistrar<Supplied> s_factory("supplied", SUPPLIED);

Supplied::Supplied() : EditorialElement(SUPPLIED), AttSource()
{
    this->RegisterAttClass(ATT_SOURCE);

    this->Reset();
}

Supplied::~Supplied() {}

void Supplied::Reset()
{
    EditorialElement::Reset();
    this->ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv
