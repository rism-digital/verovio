/////////////////////////////////////////////////////////////////////////////
// Name:        lem.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "lem.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Lem
//----------------------------------------------------------------------------

static const ClassRegistrar<Lem> s_factory("lem", LEM);

Lem::Lem() : EditorialElement(LEM), AttSource()
{
    this->RegisterAttClass(ATT_SOURCE);

    this->Reset();
}

Lem::~Lem() {}

void Lem::Reset()
{
    EditorialElement::Reset();
    this->ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv
