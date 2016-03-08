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

    m_currentBoundingBox = NULL;
}

FloatingElement::~FloatingElement()
{
}

void FloatingElement::Reset()
{
    DocObject::Reset();
}

void FloatingElement::UpdateSelfBBoxX(int x1, int x2)
{
    // assert(m_currentBoundingBox);
    if (!m_currentBoundingBox) return;
    m_currentBoundingBox->UpdateSelfBBoxX(x1, x2);
}

void FloatingElement::UpdateSelfBBoxY(int y1, int y2)
{
    // assert(m_currentBoundingBox);
    if (!m_currentBoundingBox) return;
    m_currentBoundingBox->UpdateSelfBBoxY(y1, y2);
}

void FloatingElement::SetCurrentBoundingBox(BoundingBox *boundingBox)
{
    m_currentBoundingBox = boundingBox;
}

//----------------------------------------------------------------------------
// FloatingElement functor methods
//----------------------------------------------------------------------------

int FloatingElement::PrepareTimeSpanning(ArrayPtrVoid *params)
{
    // Pass it to the pseudo functor of the interface
    if (this->HasInterface(INTERFACE_TIME_SPANNING)) {
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
    m_currentBoundingBox = NULL;
    // Pass it to the pseudo functor of the interface
    if (this->HasInterface(INTERFACE_TIME_SPANNING)) {
        TimeSpanningInterface *interface = this->GetTimeSpanningInterface();
        assert(interface);
        return interface->InterfaceResetDrawing(params, this);
    }
    return FUNCTOR_CONTINUE;
};

} // namespace vrv
