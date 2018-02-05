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

Proport::Proport() : LayerElement("prop-"), AttDurationRatio()
{
    RegisterAttClass(ATT_DURATIONRATIO);

    Reset();
}

Proport::~Proport() {}

void Proport::Reset()
{
    LayerElement::Reset();
    ResetDurationRatio();
}

} // namespace vrv
