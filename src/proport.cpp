/////////////////////////////////////////////////////////////////////////////
// Name:        proport.cpp
// Author:      Donald Byrd
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "proport.h"

namespace vrv {

//----------------------------------------------------------------------------
// Proport
//----------------------------------------------------------------------------

static const ClassRegistrar<Proport> s_factory("proport", PROPORT);

Proport::Proport() : LayerElement(PROPORT, "prop-"), AttDurationRatio()
{
    this->RegisterAttClass(ATT_DURATIONRATIO);

    this->Reset();
}

Proport::~Proport() {}

void Proport::Reset()
{
    LayerElement::Reset();
    this->ResetDurationRatio();
}

} // namespace vrv
