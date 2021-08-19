/////////////////////////////////////////////////////////////////////////////
// Name:        corr.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "corr.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Corr
//----------------------------------------------------------------------------

static const ClassRegistrar<Corr> s_factory("corr", CORR);

Corr::Corr() : EditorialElement(CORR, "corr-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Corr::~Corr() {}

void Corr::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv
