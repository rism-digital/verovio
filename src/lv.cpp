/////////////////////////////////////////////////////////////////////////////
// Name:        lv.cpp
// Author:      Andriy Makarchuk
// Created:     01/07/2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "lv.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Lv
//----------------------------------------------------------------------------

static const ClassRegistrar<Lv> s_factory("lv", LV);

Lv::Lv() : ControlElement("lv-"), TimeSpanningInterface(), AttColor(), AttCurvature(), AttCurveRend()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_CURVATURE);
    RegisterAttClass(ATT_CURVEREND);

    Reset();
}

Lv::~Lv() {}

void Lv::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    ResetColor();
    ResetCurvature();
    ResetCurveRend();
}

//----------------------------------------------------------------------------
// Lv functor methods
//----------------------------------------------------------------------------


} // namespace vrv
