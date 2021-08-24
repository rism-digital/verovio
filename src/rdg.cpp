/////////////////////////////////////////////////////////////////////////////
// Name:        rdg.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "rdg.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Rdg
//----------------------------------------------------------------------------

static const ClassRegistrar<Rdg> s_factory("rdg", RDG);

Rdg::Rdg() : EditorialElement(RDG, "rdg-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Rdg::~Rdg() {}

void Rdg::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv
