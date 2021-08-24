/////////////////////////////////////////////////////////////////////////////
// Name:        add.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "add.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Add
//----------------------------------------------------------------------------

static const ClassRegistrar<Add> s_factory("add", ADD);

Add::Add() : EditorialElement(ADD, "add-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Add::~Add() {}

void Add::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv
