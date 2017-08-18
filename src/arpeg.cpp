/////////////////////////////////////////////////////////////////////////////
// Name:        arpeg.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "arpeg.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Arpeg
//----------------------------------------------------------------------------

Arpeg::Arpeg() : ControlElement("arpeg-"), TimePointInterface(), AttArpegLog(), AttArpegVis(), AttColor()
{
    RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    RegisterAttClass(ATT_ARPEGLOG);
    RegisterAttClass(ATT_ARPEGVIS);
    RegisterAttClass(ATT_COLOR);

    Reset();
}

Arpeg::~Arpeg()
{
}

void Arpeg::Reset()
{
    ControlElement::Reset();
    TimePointInterface::Reset();
    ResetArpegLog();
    ResetArpegVis();
    ResetColor();
}

//----------------------------------------------------------------------------
// Pedal functor methods
//----------------------------------------------------------------------------

} // namespace vrv
