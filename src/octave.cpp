/////////////////////////////////////////////////////////////////////////////
// Name:        octave.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "octave.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "verticalaligner.h"

namespace vrv {

//----------------------------------------------------------------------------
// Octave
//----------------------------------------------------------------------------

static const ClassRegistrar<Octave> s_factory("octave", OCTAVE);

Octave::Octave()
    : ControlElement(OCTAVE, "octave-")
    , TimeSpanningInterface()
    , AttColor()
    , AttExtender()
    , AttLineRend()
    , AttLineRendBase()
    , AttNNumberLike()
    , AttOctaveDisplacement()
{
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_EXTENDER);
    this->RegisterAttClass(ATT_LINEREND);
    this->RegisterAttClass(ATT_LINERENDBASE);
    this->RegisterAttClass(ATT_NNUMBERLIKE);
    this->RegisterAttClass(ATT_OCTAVEDISPLACEMENT);

    this->Reset();
}

Octave::~Octave() {}

void Octave::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    this->ResetColor();
    this->ResetExtender();
    this->ResetLineRend();
    this->ResetLineRendBase();
    this->ResetNNumberLike();
    this->ResetOctaveDisplacement();
}

//----------------------------------------------------------------------------
// Octave functor methods
//----------------------------------------------------------------------------

} // namespace vrv
