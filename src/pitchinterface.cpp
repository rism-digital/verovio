/////////////////////////////////////////////////////////////////////////////
// Name:        pitchinterface.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "pitchinterface.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// PitchInterface
//----------------------------------------------------------------------------

PitchInterface::PitchInterface(): AttInterface(),
    AttAccidental(),
    AttOctave(),
    AttPitch()
    
{
    RegisterInterfaceAttClass(ATT_ACCIDENTAL);
    RegisterInterfaceAttClass(ATT_OCTAVE);
    RegisterInterfaceAttClass(ATT_PITCH);
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
    // This should never happen because it is fully implemented
    LogError( "PitchInterface::HasIdenticalPitchInterface missing" );
    assert( false );
    return false;
    /*
    if ( !otherPitchInterface ) {
        return false;
    }
    */
}
    
} // namespace vrv
