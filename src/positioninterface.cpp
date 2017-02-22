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

} // namespace vrv
