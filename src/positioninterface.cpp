/////////////////////////////////////////////////////////////////////////////
// Name:        positioninterface.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "positioninterface.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// PositionInterface
//----------------------------------------------------------------------------

PositionInterface::PositionInterface() : Interface(), AttStaffloc(), AttStafflocPitched()
{
    RegisterInterfaceAttClass(ATT_STAFFLOC);
    RegisterInterfaceAttClass(ATT_STAFFLOCPITCHED);

    Reset();
}

PositionInterface::~PositionInterface()
{
}

void PositionInterface::Reset()
{
    ResetStaffloc();
    ResetStafflocPitched();

    m_drawingLoc = 0;
}

bool PositionInterface::HasIdenticalPositionInterface(PositionInterface *otherPositionInterface)
{
    if (!otherPositionInterface) {
        return false;
    }
    if (this->GetLoc() != otherPositionInterface->GetLoc()) {
        return false;
    }
    if (this->GetOloc() != otherPositionInterface->GetOloc()) {
        return false;
    }
    if (this->GetPloc() != otherPositionInterface->GetPloc()) {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

int PositionInterface::InterfaceResetDrawing(FunctorParams *functorParams, Object *object)
{
    m_drawingLoc = 0;

    return FUNCTOR_CONTINUE;
}

int PositionInterface::InterfaceResetHorizontalAlignment(FunctorParams *functorParams, Object *object)
{
    m_drawingLoc = 0;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
