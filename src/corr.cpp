/////////////////////////////////////////////////////////////////////////////
// Name:        corr.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "corr.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Corr
//----------------------------------------------------------------------------

static const ClassRegistrar<Corr> s_factory("corr", CORR);

Corr::Corr() : EditorialElement(CORR), AttSource()
{
    this->RegisterAttClass(ATT_SOURCE);

    this->Reset();
}

Corr::~Corr() {}

void Corr::Reset()
{
    EditorialElement::Reset();
    this->ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv
