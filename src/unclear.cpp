/////////////////////////////////////////////////////////////////////////////
// Name:        unclear.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "unclear.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Unclear
//----------------------------------------------------------------------------

static const ClassRegistrar<Unclear> s_factory("unclear", UNCLEAR);

Unclear::Unclear() : EditorialElement(UNCLEAR), AttSource()
{
    this->RegisterAttClass(ATT_SOURCE);

    this->Reset();
}

Unclear::~Unclear() {}

void Unclear::Reset()
{
    EditorialElement::Reset();
    this->ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv
