/////////////////////////////////////////////////////////////////////////////
// Name:        f.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "f.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// F (Figure)
//----------------------------------------------------------------------------

F::F() : TextElement("f-"), TimeSpanningInterface(), AttExtender()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_EXTENDER);

    Reset();
}

F::~F() {}

void F::Reset()
{
    TextElement::Reset();
    TimeSpanningInterface::Reset();
    ResetExtender();
}

bool F::IsSupportedChild(Object *child)
{
    if (child->Is(TEXT)) {
        assert(dynamic_cast<Text *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------
// F functor methods
//----------------------------------------------------------------------------

int F::PrepareTimePointing(FunctorParams *functorParams)
{
    // At this stage we require <f> to have a @startid - eventually we can
    // modify this method and set as start the parent <harm> so @startid would not be
    // required anymore

    // Pass it to the pseudo functor of the interface
    TimePointInterface *interface = this->GetTimePointInterface();
    assert(interface);
    return interface->InterfacePrepareTimePointing(functorParams, this);
}

int F::PrepareTimeSpanning(FunctorParams *functorParams)
{
    // Pass it to the pseudo functor of the interface
    TimeSpanningInterface *interface = this->GetTimeSpanningInterface();
    assert(interface);
    return interface->InterfacePrepareTimeSpanning(functorParams, this);
}

int F::PrepareTimestamps(FunctorParams *functorParams)
{
    // Using @tstamp on <f> will work only if @staff is also given on <f>

    // Pass it to the pseudo functor of the interface
    TimeSpanningInterface *interface = this->GetTimeSpanningInterface();
    assert(interface);
    return interface->InterfacePrepareTimestamps(functorParams, this);
}

int F::FillStaffCurrentTimeSpanning(FunctorParams *functorParams)
{
    TimeSpanningInterface *interface = this->GetTimeSpanningInterface();
    assert(interface);
    return interface->InterfaceFillStaffCurrentTimeSpanning(functorParams, this);
}

int F::ResetDrawing(FunctorParams *functorParams)
{
    TextElement::ResetDrawing(functorParams);

    TimeSpanningInterface *interface = this->GetTimeSpanningInterface();
    assert(interface);
    return interface->InterfaceResetDrawing(functorParams, this);
}

} // namespace vrv
