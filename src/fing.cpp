/////////////////////////////////////////////////////////////////////////////
// Name:        fing.h
// Author:      Andriy Makarchuk
// Created:     2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "fing.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

namespace vrv {
//----------------------------------------------------------------------------
// Fing
//----------------------------------------------------------------------------

Fing::Fing() : ControlElement("fing-"), TimePointInterface(), TextDirInterface(), AttNNumberLike()
{
    RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    RegisterAttClass(ATT_NNUMBERLIKE);

    Reset();
}

Fing::~Fing() {}

void Fing::Reset()
{
    ControlElement::Reset();
    TimePointInterface::Reset();
    TextDirInterface::Reset();
    ResetNNumberLike();
}
} // namespace vrv
