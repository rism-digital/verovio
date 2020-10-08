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

#include "verticalaligner.h"

namespace vrv {

//----------------------------------------------------------------------------
// Octave
//----------------------------------------------------------------------------

Octave::Octave()
    : ControlElement("octave-")
    , TimeSpanningInterface()
    , AttColor()
    , AttExtender()
    , AttLineRend()
    , AttLineRendBase()
    , AttNNumberLike()
    , AttOctaveDisplacement()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_EXTENDER);
    RegisterAttClass(ATT_LINEREND);
    RegisterAttClass(ATT_LINERENDBASE);
    RegisterAttClass(ATT_NNUMBERLIKE);
    RegisterAttClass(ATT_OCTAVEDISPLACEMENT);

    Reset();
}

Octave::~Octave() {}

void Octave::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    ResetColor();
    ResetExtender();
    ResetLineRend();
    ResetLineRendBase();
    ResetNNumberLike();
    ResetOctaveDisplacement();
}

//----------------------------------------------------------------------------
// Octave functor methods
//----------------------------------------------------------------------------

} // namespace vrv
