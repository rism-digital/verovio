/////////////////////////////////////////////////////////////////////////////
// Name:        sic.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "sic.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Sic
//----------------------------------------------------------------------------

static const ClassRegistrar<Sic> s_factory("sic", SIC);

Sic::Sic() : EditorialElement(SIC, "sic-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Sic::~Sic() {}

void Sic::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv
