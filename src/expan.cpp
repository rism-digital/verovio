/////////////////////////////////////////////////////////////////////////////
// Name:        expan.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "expan.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Expan
//----------------------------------------------------------------------------

static const ClassRegistrar<Expan> s_factory("expan", EXPAN);

Expan::Expan() : EditorialElement(EXPAN), AttSource()
{
    this->RegisterAttClass(ATT_SOURCE);

    this->Reset();
}

Expan::~Expan() {}

void Expan::Reset()
{
    EditorialElement::Reset();
    this->ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv
