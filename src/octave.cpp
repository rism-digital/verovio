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

Octave::Octave() : MeasureElement("octave-"), TimeSpanningInterface(), AttOctavedisplacement()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_OCTAVEDISPLACEMENT);

    Reset();
}

Octave::~Octave()
{
}

void Octave::Reset()
{
    MeasureElement::Reset();
    TimeSpanningInterface::Reset();
    ResetOctavedisplacement();
}

//----------------------------------------------------------------------------
// Octave functor methods
//----------------------------------------------------------------------------

} // namespace vrv
