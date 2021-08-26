/////////////////////////////////////////////////////////////////////////////
// Name:        damage.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "damage.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Damage
//----------------------------------------------------------------------------

static const ClassRegistrar<Damage> s_factory("damage", DAMAGE);

Damage::Damage() : EditorialElement(DAMAGE, "damage-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Damage::~Damage() {}

void Damage::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv
