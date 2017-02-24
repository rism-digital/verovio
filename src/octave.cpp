/////////////////////////////////////////////////////////////////////////////
// Name:        octave.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "octave.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "aligner.h"

namespace vrv {

//----------------------------------------------------------------------------
// Octave
//----------------------------------------------------------------------------

Octave::Octave()
    : ControlElement("octave-"), TimeSpanningInterface(), AttColor(), AttLinerendBase(), AttOctavedisplacement()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_LINERENDBASE);
    RegisterAttClass(ATT_OCTAVEDISPLACEMENT);

    Reset();
}

Octave::~Octave()
{
}

void Octave::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    ResetColor();
    ResetLinerendBase();
    ResetOctavedisplacement();
}

//----------------------------------------------------------------------------
// Octave functor methods
//----------------------------------------------------------------------------

} // namespace vrv
