/////////////////////////////////////////////////////////////////////////////
// Name:        hairpin.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "hairpin.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "aligner.h"

namespace vrv {

//----------------------------------------------------------------------------
// Hairpin
//----------------------------------------------------------------------------

Hairpin::Hairpin() : FloatingElement("hairpin-"), TimeSpanningInterface(), AttHairpinLog(), AttPlacement()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());

    Reset();
}

Hairpin::~Hairpin()
{
}

void Hairpin::Reset()
{
    FloatingElement::Reset();
    TimeSpanningInterface::Reset();
    ResetHairpinLog();
    ResetPlacement();
}

//----------------------------------------------------------------------------
// Hairpin functor methods
//----------------------------------------------------------------------------

int Hairpin::AlignVertically(ArrayPtrVoid *params)
{
    // param 0: the systemAligner
    // param 1: the staffNb
    SystemAligner **systemAligner = static_cast<SystemAligner **>((*params).at(0));
    int *staffNb = static_cast<int *>((*params).at(1));

    // this gets (or creates) the measureAligner for the measure
    StaffAlignment *alignment = (*systemAligner)->GetStaffAlignment(*staffNb);

    assert(alignment);

    // Add the number count
    if (this->GetPlace() == STAFFREL_above) alignment->SetDynamAbove();
    if (this->GetPlace() == STAFFREL_below) alignment->SetDynamBelow();

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
