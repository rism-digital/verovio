/////////////////////////////////////////////////////////////////////////////
// Name:        space.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "space.h"
//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "horizontalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Space
//----------------------------------------------------------------------------

Space::Space() : LayerElement("space-"), DurationInterface()
{
    RegisterInterface(DurationInterface::GetAttClasses(), DurationInterface::IsInterface());

    Reset();
}

Space::~Space() {}

void Space::Reset()
{
    LayerElement::Reset();
    DurationInterface::Reset();
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Space::FindSpaceInReferenceAlignments(FunctorParams *functorParams)
{
    FindSpaceInAlignmentParams *params = dynamic_cast<FindSpaceInAlignmentParams *>(functorParams);
    assert(params);

    double duration = this->GetAlignmentDuration(params->m_mensur, params->m_meterSig);
    double time = m_alignment->GetTime();

    // The event is starting after the end of the space
    if ((time + duration) < params->m_time) {
        return FUNCTOR_CONTINUE;
    }
    // The space is starting after the event end - we can stop here
    else if (time > (params->m_time + params->m_duration)) {
        return FUNCTOR_STOP;
    }
    // The space is enclosing the event
    else if ((time <= params->m_time) && ((time + duration) >= (params->m_time + params->m_duration))) {
        params->m_success = true;
        return FUNCTOR_STOP;
    }

    // LogMessage("Duration %f", this->GetAlignmentDuration(params->m_mensur, params->m_meterSig));

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
