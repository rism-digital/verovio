/////////////////////////////////////////////////////////////////////////////
// Name:        sic.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "sic.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Sic
//----------------------------------------------------------------------------

static const ClassRegistrar<Sic> s_factory("sic", SIC);

Sic::Sic() : EditorialElement(SIC), AttSource()
{
    this->RegisterAttClass(ATT_SOURCE);

    this->Reset();
}

Sic::~Sic() {}

void Sic::Reset()
{
    EditorialElement::Reset();
    this->ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv
