/////////////////////////////////////////////////////////////////////////////
// Name:        reg.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "reg.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Reg
//----------------------------------------------------------------------------

static const ClassRegistrar<Reg> s_factory("reg", REG);

Reg::Reg() : EditorialElement(REG), AttSource()
{
    this->RegisterAttClass(ATT_SOURCE);

    this->Reset();
}

Reg::~Reg() {}

void Reg::Reset()
{
    EditorialElement::Reset();
    this->ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv
