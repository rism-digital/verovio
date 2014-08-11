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

PositionInterface::PositionInterface()
{
    m_oct = 5;
    m_pname = PITCHNAME_c;
}


PositionInterface::~PositionInterface()
{
}

void PositionInterface::SetPosition(int pname, int oct) 
{
    m_oct = oct;
    m_pname = pname;
}

bool PositionInterface::GetPosition(int *pname, int *oct) 
{
    *oct = m_oct;
    *pname = m_pname;
    return true;
}

bool PositionInterface::HasIdenticalPositionInterface( PositionInterface *otherPositionInterface )
{
    if ( !otherPositionInterface ) {
        return false;
    }
    if ( this->m_oct != otherPositionInterface->m_oct ) {
        return false;
    }
    if ( this->m_pname != otherPositionInterface->m_pname ) {
        return false;
    }
    return true;
}

} // namespace vrv