/////////////////////////////////////////////////////////////////////////////
// Name:        muspitchinterface.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "pitchinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// PitchInterface
//----------------------------------------------------------------------------

PitchInterface::PitchInterface()
{
    m_accid = 0;
    m_oct = 0;
    m_pname = 0;
}


PitchInterface::~PitchInterface()
{
}

void PitchInterface::SetPitch(int pname, int oct) 
{
    m_oct = oct;
    m_pname = pname;
}

bool PitchInterface::GetPitch(int *pname, int *oct) 
{
    *oct = m_oct;
    *pname = m_pname;
    return true;
}

bool PitchInterface::HasIdenticalPitchInterface( PitchInterface *otherPitchInterface )
{
    if ( !otherPitchInterface ) {
        return false;
    }
    if ( this->m_accid != otherPitchInterface->m_accid ) {
        return false;
    }
    if ( this->m_oct != otherPitchInterface->m_oct ) {
        return false;
    }
    if ( this->m_pname != otherPitchInterface->m_pname ) {
        return false;
    }
    return true;
}
    
} // namespace vrv
