/////////////////////////////////////////////////////////////////////////////
// Name:        pitchinterface.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "pitchinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// PitchInterface
//----------------------------------------------------------------------------

PitchInterface::PitchInterface():
    AttAccidental(),
    AttOctave(),
    AttPitch()
    
{
    Reset();
}


PitchInterface::~PitchInterface()
{
}
    
void PitchInterface::Reset()
{
    ResetAccidental();
    ResetOctave();
    ResetPitch();
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
