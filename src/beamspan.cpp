/////////////////////////////////////////////////////////////////////////////
// Name:        beamspan.cpp
// Author:      Andriy Makarchuk
// Created:     2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "beamspan.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// BeamSpan
//----------------------------------------------------------------------------

BeamSpan::BeamSpan()
    : ControlElement("beamspan-")
    , TimePointInterface()
    , TimeSpanningInterface()
    , BeamDrawingInterface()
    , AttColor()
    , AttBeamedWith()
    , AttBeamRend()
{
    RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_BEAMEDWITH);
    RegisterAttClass(ATT_BEAMREND);

    Reset();
}

BeamSpan::~BeamSpan() {}

void BeamSpan::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    BeamDrawingInterface::Reset();
    ResetColor();
    ResetBeamedWith();
    ResetBeamRend();
}

} // namespace vrv