/////////////////////////////////////////////////////////////////////////////
// Name:        durationinterface.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "durationinterface.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>
#include <stdlib.h>

//----------------------------------------------------------------------------

#include "beam.h"
#include "chord.h"
#include "mensur.h"
#include "note.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// DurationInterface
//----------------------------------------------------------------------------

DurationInterface::DurationInterface()
    : Interface()
    , AttAugmentDots()
    , AttBeamSecondary()
    , AttDurationGestural()
    , AttDurationLogical()
    , AttDurationQuality()
    , AttDurationRatio()
    , AttFermataPresent()
    , AttStaffIdent()
{
    RegisterInterfaceAttClass(ATT_AUGMENTDOTS);
    RegisterInterfaceAttClass(ATT_BEAMSECONDARY);
    RegisterInterfaceAttClass(ATT_DURATIONGESTURAL);
    RegisterInterfaceAttClass(ATT_DURATIONLOGICAL);
    RegisterInterfaceAttClass(ATT_DURATIONQUALITY);
    RegisterInterfaceAttClass(ATT_DURATIONRATIO);
    RegisterInterfaceAttClass(ATT_FERMATAPRESENT);
    RegisterInterfaceAttClass(ATT_STAFFIDENT);

    Reset();
}

DurationInterface::~DurationInterface() {}

void DurationInterface::Reset()
{
    ResetAugmentDots();
    ResetBeamSecondary();
    ResetDurationGestural();
    ResetDurationLogical();
    ResetDurationQuality();
    ResetDurationRatio();
    ResetFermataPresent();
    ResetStaffIdent();
}

double DurationInterface::GetInterfaceAlignmentDuration(int num, int numBase)
{
    int noteDur = this->GetDurGes() != DURATION_NONE ? this->GetActualDurGes() : this->GetActualDur();

    if (this->HasNum()) num *= this->GetNum();
    if (this->HasNumbase()) numBase *= this->GetNumbase();

    double duration = DUR_MAX / pow(2.0, (double)(noteDur - 2.0)) * numBase / num;

    int noteDots = (this->HasDotsGes()) ? this->GetDotsGes() : this->GetDots();
    if (noteDots != -1) {
        duration = 2 * duration - (duration / pow(2, noteDots));
    }
    // LogDebug("Duration %d; Dot %d; Alignement %f", noteDur, GetDots(), duration);
    return duration;
}

double DurationInterface::GetInterfaceAlignmentMensuralDuration(int num, int numBase, Mensur *currentMensur)
{
    int noteDur = this->GetDurGes() != DURATION_NONE ? this->GetActualDurGes() : this->GetActualDur();

    if (!currentMensur) {
        LogWarning("No current mensur for calculating duration");
        return DUR_MENSURAL_REF;
    }

    if (this->HasNum() || this->HasNumbase()) {
        if (this->HasNum()) num *= this->GetNum();
        if (this->HasNumbase()) numBase *= this->GetNumbase();
    }
    // perfecta in tempus imperfectum
    else if ((this->GetDurQuality() == DURQUALITY_mensural_perfecta) && (currentMensur->GetTempus() == TEMPUS_2)) {
        num *= 2;
        numBase *= 3;
    }
    // imperfecta in tempus perfectum (e.g. not imperfectum since perfectum is assumed by default)
    else if ((this->GetDurQuality() == DURQUALITY_mensural_imperfecta) && (currentMensur->GetTempus() != TEMPUS_2)) {
        num *= 3;
        numBase *= 2;
    }
    // altera, maior, or duplex
    else if (this->HasDurQuality()
        && (this->GetDurQuality() == DURQUALITY_mensural_altera || this->GetDurQuality() == DURQUALITY_mensural_maior
            || this->GetDurQuality() == DURQUALITY_mensural_duplex)) {
        num *= 1;
        numBase *= 2;
    } // Any other case (minor, perfecta in tempus perfectum, and imperfecta in tempus imperfectum) follows the
      // mensuration and has no @num and @numbase attributes

    if (currentMensur->HasNum()) num *= currentMensur->GetNum();
    if (currentMensur->HasNumbase()) numBase *= currentMensur->GetNumbase();

    double ratio = 0.0;
    double duration = (double)DUR_MENSURAL_REF;
    switch (noteDur) {
        case DUR_MX:
            duration *= (double)abs(currentMensur->GetModusminor()) * (double)abs(currentMensur->GetModusmaior());
            break;
        case DUR_LG: duration *= (double)abs(currentMensur->GetModusminor()); break;
        case DUR_BR: break;
        case DUR_1: duration /= (double)abs(currentMensur->GetTempus()); break;
        default:
            ratio = pow(2.0, (double)(noteDur - DUR_2));
            duration /= (double)abs(currentMensur->GetTempus()) * (double)abs(currentMensur->GetProlatio()) * ratio;
            break;
    }
    duration *= (double)numBase / (double)num;
    // LogDebug("Duration %d; %d/%d; Alignement %f; Ratio %f", noteDur, num, numbase, duration, ratio);
    duration = durRound(duration);
    return duration;
}

bool DurationInterface::IsFirstInBeam(LayerElement *noteOrRest)
{
    Beam *beam = dynamic_cast<Beam *>(noteOrRest->GetFirstAncestor(BEAM, MAX_BEAM_DEPTH));
    if (!beam) {
        return false;
    }
    const ArrayOfObjects *notesOrRests = beam->GetList(beam);
    ArrayOfObjects::const_iterator iter = notesOrRests->begin();
    if (*iter == noteOrRest) {
        return true;
    }
    return false;
}

bool DurationInterface::IsLastInBeam(LayerElement *noteOrRest)
{
    Beam *beam = dynamic_cast<Beam *>(noteOrRest->GetFirstAncestor(BEAM, MAX_BEAM_DEPTH));
    if (!beam) {
        return false;
    }
    const ArrayOfObjects *notesOrRests = beam->GetList(beam);
    ArrayOfObjects::const_reverse_iterator iter = notesOrRests->rbegin();
    if (*iter == noteOrRest) {
        return true;
    }
    return false;
}

int DurationInterface::GetActualDur() const
{
    // maxima (-1) is a mensural only value
    if (this->GetDur() == DURATION_maxima) return DUR_MX;
    return (this->GetDur() & DUR_MENSURAL_MASK);
}

int DurationInterface::GetActualDurGes() const
{
    // maxima (-1) is a mensural only value
    if (this->GetDurGes() == DURATION_maxima) return DUR_MX;
    return (this->GetDurGes() & DUR_MENSURAL_MASK);
}

int DurationInterface::GetNoteOrChordDur(LayerElement *element)
{
    if (element->Is(CHORD)) {
        return this->GetActualDur();
    }
    else if (element->Is(NOTE)) {
        Note *note = vrv_cast<Note *>(element);
        assert(note);
        Chord *chord = note->IsChordTone();
        if (chord && !this->HasDur())
            return chord->GetActualDur();
        else
            return this->GetActualDur();
    }
    return this->GetActualDur();
}

bool DurationInterface::IsMensuralDur()
{
    // maxima (-1) is a mensural only value
    if (this->GetDur() == DURATION_maxima) return true;
    return (this->GetDur() > DUR_MENSURAL_MASK);
}

bool DurationInterface::HasIdenticalDurationInterface(DurationInterface *otherDurationInterface)
{
    // This should never happen because it is fully implemented
    LogError("DurationInterface::HasIdenticalDurationInterface missing");
    assert(false);
    return false;
    /*
    if (!otherDurationInterface) {
        return false;
    }
    */
}

} // namespace vrv
