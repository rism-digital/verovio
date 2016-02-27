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
    // param 1: the staffIdx (unused)
    // param 2: the staffN (unused)
    SystemAligner **systemAligner = static_cast<SystemAligner **>((*params).at(0));

    std::vector<int> staffList = this->GetStaff();
    std::vector<int>::iterator iter;
    for (iter = staffList.begin(); iter != staffList.end(); iter++) {
        // this gets (or creates) the measureAligner for the measure
        StaffAlignment *alignment = (*systemAligner)->GetStaffAlignmentForStaffN(*iter);

        if (!alignment) continue;

        if (this->GetPlace() == STAFFREL_above) alignment->SetHairpinAbove();
        if (this->GetPlace() == STAFFREL_below) alignment->SetHairpinBelow();
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
