/////////////////////////////////////////////////////////////////////////////
// Name:        durationinterface.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "durationinterface.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <cstdlib>
#include <math.h>

//----------------------------------------------------------------------------

#include "beam.h"
#include "chord.h"
#include "functorparams.h"
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
    this->RegisterInterfaceAttClass(ATT_AUGMENTDOTS);
    this->RegisterInterfaceAttClass(ATT_BEAMSECONDARY);
    this->RegisterInterfaceAttClass(ATT_DURATIONGESTURAL);
    this->RegisterInterfaceAttClass(ATT_DURATIONLOGICAL);
    this->RegisterInterfaceAttClass(ATT_DURATIONQUALITY);
    this->RegisterInterfaceAttClass(ATT_DURATIONRATIO);
    this->RegisterInterfaceAttClass(ATT_FERMATAPRESENT);
    this->RegisterInterfaceAttClass(ATT_STAFFIDENT);

    this->Reset();
}

DurationInterface::~DurationInterface() {}

void DurationInterface::Reset()
{
    this->ResetAugmentDots();
    this->ResetBeamSecondary();
    this->ResetDurationGestural();
    this->ResetDurationLogical();
    this->ResetDurationQuality();
    this->ResetDurationRatio();
    this->ResetFermataPresent();
    this->ResetStaffIdent();

    m_durDefault = DURATION_NONE;

    m_scoreTimeOnset = 0.0;
    m_scoreTimeOffset = 0.0;
    m_realTimeOnsetMilliseconds = 0;
    m_realTimeOffsetMilliseconds = 0;
    m_scoreTimeTiedDuration = 0.0;
}

double DurationInterface::GetInterfaceAlignmentDuration(int num, int numBase) const
{
    int noteDur = this->GetDurGes() != DURATION_NONE ? this->GetActualDurGes() : this->GetActualDur();
    if (noteDur == DUR_NONE) noteDur = DUR_4;

    if (this->HasNum()) num *= this->GetNum();
    if (this->HasNumbase()) numBase *= this->GetNumbase();

    double duration = DUR_MAX / pow(2.0, (double)(noteDur - 2.0)) * numBase / num;

    int noteDots = (this->HasDotsGes()) ? this->GetDotsGes() : this->GetDots();
    if (noteDots != VRV_UNSET) {
        duration = 2 * duration - (duration / pow(2, noteDots));
    }
    // LogDebug("Duration %d; Dot %d; Alignment %f", noteDur, this->GetDots(), duration);
    return duration;
}

double DurationInterface::GetInterfaceAlignmentMensuralDuration(int num, int numBase, const Mensur *currentMensur) const
{
    int noteDur = this->GetDurGes() != DURATION_NONE ? this->GetActualDurGes() : this->GetActualDur();
    if (noteDur == DUR_NONE) noteDur = DUR_4;

    if (!currentMensur) {
        LogWarning("No current mensur for calculating duration");
        return DUR_MENSURAL_REF;
    }

    if (this->HasNum() || this->HasNumbase()) {
        if (this->HasNum()) num *= this->GetNum();
        if (this->HasNumbase()) numBase *= this->GetNumbase();
    }
    // perfecta in imperfect mensuration (two perfectas in the place of the original three imperfectas)
    else if (this->GetDurQuality() == DURQUALITY_mensural_perfecta) {
        if (((this->GetDur() == DURATION_longa) && (currentMensur->GetModusminor() == MODUSMINOR_2))
            || ((this->GetDur() == DURATION_brevis) && (currentMensur->GetTempus() == TEMPUS_2))
            || ((this->GetDur() == DURATION_semibrevis) && (currentMensur->GetProlatio() == PROLATIO_2))
            || (this->GetDur() == DURATION_minima) || (this->GetDur() == DURATION_semiminima)
            || (this->GetDur() == DURATION_fusa) || (this->GetDur() == DURATION_semifusa)) {
            num *= 2;
            numBase *= 3;
        }
    }
    // imperfecta in perfect mensuration (three imperfectas in the place of the two original perfectas)
    else if (this->GetDurQuality() == DURQUALITY_mensural_imperfecta) {
        if (((this->GetDur() == DURATION_longa) && (currentMensur->GetModusminor() != MODUSMINOR_2))
            || ((this->GetDur() == DURATION_brevis) && (currentMensur->GetTempus() != TEMPUS_2))
            || ((this->GetDur() == DURATION_semibrevis) && (currentMensur->GetProlatio() != PROLATIO_2))) {
            num *= 3;
            numBase *= 2;
        }
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
    // LogDebug("Duration %d; %d/%d; Alignment %f; Ratio %f", noteDur, num, numbase, duration, ratio);
    duration = durRound(duration);
    return duration;
}

bool DurationInterface::IsFirstInBeam(const LayerElement *noteOrRest) const
{
    const Beam *beam = dynamic_cast<const Beam *>(noteOrRest->GetFirstAncestor(BEAM, MAX_BEAM_DEPTH));
    if (!beam) {
        return false;
    }
    return (noteOrRest == beam->GetListFront(beam));
}

bool DurationInterface::IsLastInBeam(const LayerElement *noteOrRest) const
{
    const Beam *beam = dynamic_cast<const Beam *>(noteOrRest->GetFirstAncestor(BEAM, MAX_BEAM_DEPTH));
    if (!beam) {
        return false;
    }
    return (noteOrRest == beam->GetListBack(beam));
}

int DurationInterface::GetActualDur() const
{
    const data_DURATION dur = this->HasDur() ? this->GetDur() : this->GetDurDefault();
    return this->CalcActualDur(dur);
}

int DurationInterface::GetActualDurGes() const
{
    const data_DURATION dur = this->HasDurGes() ? this->GetDurGes() : DURATION_NONE;
    return this->CalcActualDur(dur);
}

int DurationInterface::CalcActualDur(data_DURATION dur) const
{
    if (dur == DURATION_NONE) return DUR_NONE;
    // maxima (-1) is a mensural only value
    if (dur == DURATION_maxima) return DUR_MX;
    return (dur & DUR_MENSURAL_MASK);
}

int DurationInterface::GetNoteOrChordDur(const LayerElement *element) const
{
    if (element->Is(CHORD)) {
        int duration = this->GetActualDur();
        if (duration != DUR_NONE) return duration;

        const Chord *chord = vrv_cast<const Chord *>(element);
        for (const Note *note : { chord->GetTopNote(), chord->GetBottomNote() }) {
            duration = note->GetActualDur();
            if (duration != DUR_NONE) {
                return duration;
            }
        }
    }
    else if (element->Is(NOTE)) {
        const Note *note = vrv_cast<const Note *>(element);
        assert(note);
        const Chord *chord = note->IsChordTone();
        if (chord && !this->HasDur())
            return chord->GetActualDur();
        else
            return this->GetActualDur();
    }
    return this->GetActualDur();
}

bool DurationInterface::IsMensuralDur() const
{
    // maxima (-1) is a mensural only value
    if (this->GetDur() == DURATION_maxima) return true;
    return (this->GetDur() > DUR_MENSURAL_MASK);
}

bool DurationInterface::HasIdenticalDurationInterface(const DurationInterface *otherDurationInterface) const
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

void DurationInterface::SetScoreTimeOnset(double scoreTime)
{
    m_scoreTimeOnset = scoreTime;
}

void DurationInterface::SetRealTimeOnsetSeconds(double timeInSeconds)
{
    // m_realTimeOnsetMilliseconds = int(timeInSeconds * 1000.0 + 0.5);
    m_realTimeOnsetMilliseconds = timeInSeconds * 1000.0;
}

void DurationInterface::SetScoreTimeOffset(double scoreTime)
{
    m_scoreTimeOffset = scoreTime;
}

void DurationInterface::SetRealTimeOffsetSeconds(double timeInSeconds)
{
    // m_realTimeOffsetMilliseconds = int(timeInSeconds * 1000.0 + 0.5);
    m_realTimeOffsetMilliseconds = timeInSeconds * 1000.0;
}

void DurationInterface::SetScoreTimeTiedDuration(double scoreTime)
{
    m_scoreTimeTiedDuration = scoreTime;
}

double DurationInterface::GetScoreTimeOnset() const
{
    return m_scoreTimeOnset;
}

double DurationInterface::GetRealTimeOnsetMilliseconds() const
{
    return m_realTimeOnsetMilliseconds;
}

double DurationInterface::GetScoreTimeOffset() const
{
    return m_scoreTimeOffset;
}

double DurationInterface::GetRealTimeOffsetMilliseconds() const
{
    return m_realTimeOffsetMilliseconds;
}

double DurationInterface::GetScoreTimeTiedDuration() const
{
    return m_scoreTimeTiedDuration;
}

double DurationInterface::GetScoreTimeDuration() const
{
    return this->GetScoreTimeOffset() - this->GetScoreTimeOnset();
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

} // namespace vrv
