/////////////////////////////////////////////////////////////////////////////
// Name:        floatingelement.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "floatingelement.h"
#include "timeinterface.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// FloatingElement
//----------------------------------------------------------------------------

FloatingElement::FloatingElement() : DocObject("fe")
{
    Reset();
}

FloatingElement::FloatingElement(std::string classid) : DocObject(classid)
{
    Reset();
}

FloatingElement::~FloatingElement()
{
}

void FloatingElement::Reset()
{
    DocObject::Reset();
}

//----------------------------------------------------------------------------
// FloatingElement functor methods
//----------------------------------------------------------------------------

int FloatingElement::PrepareTimeSpanning(ArrayPtrVoid *params)
{
    // Pass it to the pseudo functor of the interface
    if (this->HasInterface(INTERFACE_TIME_SPANNING)) {
        // For now we don't consider dir and  dynam since the time spanning is not relevant for display
        // Eventually, we could consider them but only if a @tstamp2 (or @endid) is provided
        if ((this->Is() == DIR) || (this->Is() == DYNAM)) return FUNCTOR_CONTINUE;

        TimeSpanningInterface *interface = this->GetTimeSpanningInterface();
        assert(interface);
        return interface->InterfacePrepareTimeSpanning(params, this);
    }
    return FUNCTOR_CONTINUE;
}

int FloatingElement::PrepareTimestamps(ArrayPtrVoid *params)
{
    // Pass it to the pseudo functor of the interface
    if (this->HasInterface(INTERFACE_TIME_POINT)) {
        TimePointInterface *interface = this->GetTimePointInterface();
        assert(interface);
        return interface->InterfacePrepareTimestamps(params, this);
    }
    else if (this->HasInterface(INTERFACE_TIME_SPANNING)) {
        TimeSpanningInterface *interface = this->GetTimeSpanningInterface();
        assert(interface);
        return interface->InterfacePrepareTimestamps(params, this);
    }
    return FUNCTOR_CONTINUE;
}

int FloatingElement::FillStaffCurrentTimeSpanning(ArrayPtrVoid *params)
{
    // Pass it to the pseudo functor of the interface
    if (this->HasInterface(INTERFACE_TIME_SPANNING)) {
        TimeSpanningInterface *interface = this->GetTimeSpanningInterface();
        assert(interface);
        return interface->InterfaceFillStaffCurrentTimeSpanning(params, this);
    }
    return FUNCTOR_CONTINUE;
}

int FloatingElement::ResetDrawing(ArrayPtrVoid *params)
{
    // Pass it to the pseudo functor of the interface
    if (this->HasInterface(INTERFACE_TIME_SPANNING)) {
        TimeSpanningInterface *interface = this->GetTimeSpanningInterface();
        assert(interface);
        return interface->InterfaceResetDrawing(params, this);
    }
    return FUNCTOR_CONTINUE;
};

} // namespace vrv
