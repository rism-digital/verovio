/////////////////////////////////////////////////////////////////////////////
// Name:        plica.cpp
// Author:      Laurent Pugin
// Created:     2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "plica.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Plica
//----------------------------------------------------------------------------

static const ClassRegistrar<Plica> s_factory("plica", PLICA);

Plica::Plica() : LayerElement(PLICA, "plica-"), AttPlicaVis()
{
    this->RegisterAttClass(ATT_PLICAVIS);

    this->Reset();
}

Plica::~Plica() {}

void Plica::Reset()
{
    LayerElement::Reset();
    this->ResetPlicaVis();
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

} // namespace vrv
