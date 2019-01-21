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

#include "chord.h"
#include "layer.h"
#include "note.h"
#include "rest.h"
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

PitchInterface::~PitchInterface() {}

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

void PitchInterface::AdjustPname(int &pname, int &oct)
{
    if (pname < PITCHNAME_c) {
        if (oct > 0) oct--;
        pname = PITCHNAME_b;
    }
    else if (pname > PITCHNAME_b) {
        if (oct < 7) oct++;
        pname = PITCHNAME_c;
    }
}

void PitchInterface::AdjustPitchByOffset(int pitchOffset)
{
    int pname = this->GetPname() + pitchOffset;
    int oct = this->GetOct();

    // Check if a change in octave is necessary
    while (pname > PITCHNAME_b) {
        pname -= 7;
        oct++;
    }
    while (pname < PITCHNAME_c) {
        pname += 7;
        oct--;
    }

    // If it falls out of allowed range, set to allowed extreme values
    if (oct > 9) {
        oct = 9;
        pname = PITCHNAME_b;
    }
    else if (oct < 0) {
        oct = 0;
        pname = PITCHNAME_c;
    }

    this->SetPname((data_PITCHNAME)pname);
    this->SetOct(oct);
}

int PitchInterface::PitchDifferenceTo(PitchInterface *pi)
{
    assert(pi);
    int pitchDifference = 0;

    pitchDifference = this->GetPname() - pi->GetPname();
    pitchDifference += 7 * (this->GetOct() - pi->GetOct());

    return pitchDifference;
}

//----------------------------------------------------------------------------
// Static methods
//----------------------------------------------------------------------------

int PitchInterface::CalcLoc(LayerElement *layerElement, Layer *layer, bool topChordNote)
{
    assert(layerElement);

    if (layerElement->Is(CHORD)) {
        Chord *chord = dynamic_cast<Chord *>(layerElement);
        assert(chord);
        Note *note = (topChordNote) ? chord->GetTopNote() : chord->GetBottomNote();
        return CalcLoc(note, layer);
    }
    else if (layerElement->Is(NOTE)) {
        Note *note = dynamic_cast<Note *>(layerElement);
        assert(note);
        if (note->HasLoc()) {
            return note->GetLoc();
        }
        return PitchInterface::CalcLoc(note->GetPname(), note->GetOct(), layer->GetClefLocOffset(layerElement));
    }
    else {
        assert(false);
        return 0;
    }
}

int PitchInterface::CalcLoc(data_PITCHNAME pname, int oct, int clefLocOffset)
{
    // E.g., C4 with clef C1: (4 - 4 * 7) + (1 - 1) + 0;
    return ((oct - OCTAVE_OFFSET) * 7 + (pname - 1) + clefLocOffset);
}

} // namespace vrv
