/////////////////////////////////////////////////////////////////////////////
// Name:        rdg.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "rdg.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Rdg
//----------------------------------------------------------------------------

static const ClassRegistrar<Rdg> s_factory("rdg", RDG);

Rdg::Rdg() : EditorialElement(RDG), AttSource()
{
    this->RegisterAttClass(ATT_SOURCE);

    this->Reset();
}

Rdg::~Rdg() {}

void Rdg::Reset()
{
    EditorialElement::Reset();
    this->ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv
