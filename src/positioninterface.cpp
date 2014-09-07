/////////////////////////////////////////////////////////////////////////////
// Name:        positioninterface.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "positioninterface.h"

//----------------------------------------------------------------------------

#include "vrvdef.h"

namespace vrv {

//----------------------------------------------------------------------------
// PositionInterface
//----------------------------------------------------------------------------

PositionInterface::PositionInterface():
    AttStafflocPitched()
{
    Reset();
}


PositionInterface::~PositionInterface()
{
}    

void PositionInterface::Reset()
{
    ResetStafflocPitched();
}
    
bool PositionInterface::HasIdenticalPositionInterface( PositionInterface *otherPositionInterface )
{
    if ( !otherPositionInterface ) {
        return false;
    }
    if ( this->GetOloc() != otherPositionInterface->GetOloc() ) {
        return false;
    }
    if ( this->GetPloc() != otherPositionInterface->GetPloc() ) {
        return false;
    }
    return true;
}

} // namespace vrv