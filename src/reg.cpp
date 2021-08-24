/////////////////////////////////////////////////////////////////////////////
// Name:        reg.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "reg.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Reg
//----------------------------------------------------------------------------

static const ClassRegistrar<Reg> s_factory("reg", REG);

Reg::Reg() : EditorialElement(REG, "reg-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Reg::~Reg() {}

void Reg::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv
