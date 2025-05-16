/////////////////////////////////////////////////////////////////////////////
// Name:        pitchinterface.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pitchinterface.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "chord.h"
#include "clef.h"
#include "custos.h"
#include "layer.h"
#include "note.h"
#include "rest.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// PitchInterface
//----------------------------------------------------------------------------

PitchInterface::PitchInterface() : Interface(), AttNoteGes(), AttOctave(), AttPitch(), AttPitchGes()
{
    this->RegisterInterfaceAttClass(ATT_NOTEGES);
    this->RegisterInterfaceAttClass(ATT_OCTAVE);
    this->RegisterInterfaceAttClass(ATT_PITCH);
    this->RegisterInterfaceAttClass(ATT_PITCHGES);

    this->Reset();
}

PitchInterface::~PitchInterface() {}

void PitchInterface::Reset()
{
    this->ResetNoteGes();
    this->ResetOctave();
    this->ResetPitch();
    this->ResetPitchGes();

    m_octDefault = MEI_UNSET_OCT;
}

bool PitchInterface::HasIdenticalPitchInterface(const PitchInterface *otherPitchInterface) const
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

int PitchInterface::PitchDifferenceTo(const PitchInterface *pi) const
{
    assert(pi);
    int pitchDifference = 0;

    pitchDifference = this->GetPname() - pi->GetPname();
    pitchDifference += 7 * (this->GetOct() - pi->GetOct());

    return pitchDifference;
}

void PitchInterface::AdjustPitchForNewClef(const Clef *oldClef, const Clef *newClef)
{
    assert(oldClef);
    assert(newClef);

    int pitchDiff = -2 * (newClef->GetLine() - oldClef->GetLine());
    if (oldClef->GetShape() == CLEFSHAPE_F) {
        pitchDiff += 4;
    }
    else if (oldClef->GetShape() == CLEFSHAPE_G) {
        pitchDiff -= 4;
    }
    if (newClef->GetShape() == CLEFSHAPE_F) {
        pitchDiff -= 4;
    }
    else if (newClef->GetShape() == CLEFSHAPE_G) {
        pitchDiff += 4;
    }

    this->AdjustPitchByOffset(pitchDiff);
}

//----------------------------------------------------------------------------
// Static methods for PitchInterface
//----------------------------------------------------------------------------

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

int PitchInterface::CalcLoc(
    const LayerElement *layerElement, const Layer *layer, const LayerElement *crossStaffElement, bool topChordNote)
{
    assert(layerElement);

    if (layerElement->Is(CHORD)) {
        const Chord *chord = vrv_cast<const Chord *>(layerElement);
        assert(chord);
        const Note *note = (topChordNote) ? chord->GetTopNote() : chord->GetBottomNote();
        return CalcLoc(note, layer, crossStaffElement);
    }
    else if (layerElement->Is(NOTE)) {
        const Note *note = vrv_cast<const Note *>(layerElement);
        assert(note);
        if (note->HasLoc()) {
            return note->GetLoc();
        }
        else if (note->HasPname() && (note->HasOct() || note->HasOctDefault())) {
            int offset = layer->GetClefLocOffset(crossStaffElement);
            const Layer *parentLayer = vrv_cast<const Layer *>(layerElement->GetFirstAncestor(LAYER));
            if (parentLayer != layer) {
                offset = parentLayer->GetCrossStaffClefLocOffset(layerElement, offset);
            }
            const data_OCTAVE oct = (note->HasOct()) ? note->GetOct() : note->GetOctDefault();
            return PitchInterface::CalcLoc(note->GetPname(), oct, offset);
        }
        else {
            return 0;
        }
    }
    else if (layerElement->Is(CUSTOS)) {
        const Custos *custos = vrv_cast<const Custos *>(layerElement);
        assert(custos);
        if (custos->HasLoc()) {
            return custos->GetLoc();
        }
        return PitchInterface::CalcLoc(
            custos->GetPname(), custos->GetOct(), layer->GetClefLocOffset(crossStaffElement));
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
