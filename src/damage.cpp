/////////////////////////////////////////////////////////////////////////////
// Name:        damage.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "damage.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Damage
//----------------------------------------------------------------------------

static const ClassRegistrar<Damage> s_factory("damage", DAMAGE);

Damage::Damage() : EditorialElement(DAMAGE), AttSource()
{
    this->RegisterAttClass(ATT_SOURCE);

    this->Reset();
}

Damage::~Damage() {}

void Damage::Reset()
{
    EditorialElement::Reset();
    this->ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv
