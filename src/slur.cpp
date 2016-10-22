/////////////////////////////////////////////////////////////////////////////
// Name:        slur.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "slur.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Slur
//----------------------------------------------------------------------------

Slur::Slur() : ControlElement("slur-"), TimeSpanningInterface(), AttCurvature()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_CURVATURE);

    Reset();
}

Slur::~Slur()
{
}

void Slur::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    ResetCurvature();
}

} // namespace vrv
