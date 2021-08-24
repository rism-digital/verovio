/////////////////////////////////////////////////////////////////////////////
// Name:        btrem.cpp
// Author:      Klaus Rettinghaus
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "btrem.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "chord.h"
#include "comparison.h"
#include "editorial.h"
#include "functorparams.h"
#include "layer.h"
#include "note.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// BTrem
//----------------------------------------------------------------------------

static const ClassRegistrar<BTrem> s_factory("btrem", BTREM);

BTrem::BTrem() : LayerElement(BTREM, "btrem-"), AttBTremLog(), AttTremMeasured()
{
    RegisterAttClass(ATT_BTREMLOG);
    RegisterAttClass(ATT_TREMMEASURED);

    Reset();
}

BTrem::~BTrem() {}

void BTrem::Reset()
{
    LayerElement::Reset();
    ResetBTremLog();
    ResetTremMeasured();
}

bool BTrem::IsSupportedChild(Object *child)
{
    if (child->Is(CHORD)) {
        assert(dynamic_cast<Chord *>(child));
    }
    else if (child->Is(CLEF)) {
        assert(dynamic_cast<Clef *>(child));
    }
    else if (child->Is(NOTE)) {
        assert(dynamic_cast<Note *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

int BTrem::GenerateMIDI(FunctorParams *functorParams)
{
    GenerateMIDIParams *params = vrv_params_cast<GenerateMIDIParams *>(functorParams);
    assert(params);

    // Do nothing if the tremolo is unmeasured
    if (this->GetForm() == bTremLog_FORM_unmeas) {
        return FUNCTOR_CONTINUE;
    }

    // Calculate duration of individual note in tremolo
    const data_DURATION individualNoteDur = CalcIndividualNoteDuration();
    if (individualNoteDur == DURATION_NONE) return FUNCTOR_CONTINUE;
    const double noteInQuarterDur = pow(2.0, (DURATION_4 - individualNoteDur));

    // Define lambda which expands one note into multiple individual notes of the same pitch
    auto expandNote = [params, noteInQuarterDur](Object *obj) {
        Note *note = vrv_cast<Note *>(obj);
        assert(note);
        note->CalcMIDIPitch(params->m_transSemi);
        const char pitch = note->GetMIDIPitch();
        const double totalInQuarterDur = note->GetScoreTimeDuration() + note->GetScoreTimeTiedDuration();
        const int multiplicity = totalInQuarterDur / noteInQuarterDur;
        (params->m_expandedNotes)[note] = MIDINoteSequence(multiplicity, { pitch, noteInQuarterDur });
    };

    // Apply expansion either to all notes in chord or to first note
    Chord *chord = vrv_cast<Chord *>(this->FindDescendantByType(CHORD));
    if (chord) {
        ListOfObjects notes;
        ClassIdComparison noteComparison(NOTE);
        chord->FindAllDescendantByComparison(&notes, &noteComparison);
        std::for_each(notes.begin(), notes.end(), expandNote);
    }
    else {
        Object *note = this->FindDescendantByType(NOTE);
        if (note) {
            expandNote(note);
        }
    }

    return FUNCTOR_CONTINUE;
}

data_DURATION BTrem::CalcIndividualNoteDuration()
{
    // Check if duration is given by attribute
    if (this->HasUnitdur()) {
        return this->GetUnitdur();
    }

    // Otherwise consider duration and stem modifier of first child chord/note
    data_DURATION childDur = DURATION_NONE;
    data_STEMMODIFIER stemMod = STEMMODIFIER_NONE;
    Chord *chord = vrv_cast<Chord *>(this->FindDescendantByType(CHORD));
    if (chord) {
        childDur = chord->GetDur();
        stemMod = chord->GetStemMod();
    }
    else {
        Note *note = vrv_cast<Note *>(this->FindDescendantByType(NOTE));
        if (note) {
            childDur = note->GetDur();
            stemMod = note->GetStemMod();
        }
    }

    // Calculate duration from number of slashes
    if ((stemMod >= STEMMODIFIER_1slash) && (stemMod <= STEMMODIFIER_6slash)) {
        if ((childDur >= DURATION_long) && (childDur <= DURATION_1024)) {
            int value = std::max<int>(childDur, DURATION_4);
            value += (stemMod - STEMMODIFIER_1slash + 1);
            value = std::min<int>(value, DURATION_1024);
            assert((value >= DURATION_8) && (value <= DURATION_1024));
            return static_cast<data_DURATION>(value);
        }
    }
    return DURATION_NONE;
}

} // namespace vrv
