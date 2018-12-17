/////////////////////////////////////////////////////////////////////////////
// Name:        unclear.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "unclear.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Unclear
//----------------------------------------------------------------------------

Unclear::Unclear() : EditorialElement("unclear-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Unclear::~Unclear() {}

void Unclear::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv
