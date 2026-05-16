/////////////////////////////////////////////////////////////////////////////
// Name:        orig.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "orig.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Orig
//----------------------------------------------------------------------------

static const ClassRegistrar<Orig> s_factory("orig", ORIG);

Orig::Orig() : EditorialElement(ORIG), AttSource()
{
    this->RegisterAttClass(ATT_SOURCE);

    this->Reset();
}

Orig::~Orig() {}

void Orig::Reset()
{
    EditorialElement::Reset();
    this->ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv
