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

PitchInterface::PitchInterface() : Interface(), AttNoteGes(), AttOctave(), AttPitch()
{
    RegisterInterfaceAttClass(ATT_NOTEGES);
    RegisterInterfaceAttClass(ATT_OCTAVE);
    RegisterInterfaceAttClass(ATT_PITCH);

    Reset();
}

PitchInterface::~PitchInterface()
{
}

void PitchInterface::Reset()
{
    ResetNoteGes();
    ResetOctave();
    ResetPitch();
}

bool PitchInterface::HasIdenticalPitchInterface(PitchInterface *otherPitchInterface)
{
    // This should never happen because it is fully implemented
    LogError("PitchInterface::HasIdenticalPitchInterface missing");
    assert(false);
    return false;
    /*
    if (!otherPitchInterface) {
        return false;
    }
    */
}

int PitchInterface::CalcLoc(data_PITCHNAME pname, int oct, int clefLocOffset)
{
    // E.g., C4 with clef C1: (4 - 4 * 7) + (1 - 1) + 0;
    return ((oct - OCTAVE_OFFSET) * 7 + (pname - 1) + clefLocOffset);
}

} // namespace vrv
