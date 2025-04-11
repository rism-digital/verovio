/////////////////////////////////////////////////////////////////////////////
// Name:        add.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "add.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Add
//----------------------------------------------------------------------------

static const ClassRegistrar<Add> s_factory("add", ADD);

Add::Add() : EditorialElement(ADD), AttSource()
{
    this->RegisterAttClass(ATT_SOURCE);

    this->Reset();
}

Add::~Add() {}

void Add::Reset()
{
    EditorialElement::Reset();
    this->ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv
