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
    , AttDurationGes()
    , AttDurationLog()
    , AttDurationQuality()
    , AttDurationRatio()
    , AttFermataPresent()
    , AttStaffIdent()
{
    this->RegisterInterfaceAttClass(ATT_AUGMENTDOTS);
    this->RegisterInterfaceAttClass(ATT_BEAMSECONDARY);
    this->RegisterInterfaceAttClass(ATT_DURATIONGES);
    this->RegisterInterfaceAttClass(ATT_DURATIONLOG);
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
    this->ResetDurationGes();
    this->ResetDurationLog();
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

Fraction DurationInterface::GetInterfaceAlignmentDuration(int num, int numBase) const
{
    data_DURATION noteDur = (this->GetDurGes() != DURATION_NONE) ? this->GetActualDurGes() : this->GetActualDur();
    if (noteDur == DURATION_NONE) noteDur = DURATION_4;

    if (this->HasNum()) num *= this->GetNum();
    if (this->HasNumbase()) numBase *= this->GetNumbase();

    // Fraction duration(noteDur);
    // duration = duration * numBase / num;
    Fraction duration = DUR_MAX / pow(2.0, (double)(noteDur - 2.0)) * numBase / num;

    int noteDots = (this->HasDotsGes()) ? this->GetDotsGes() : this->GetDots();
    if (noteDots != VRV_UNSET) {
        duration = duration * 2 - (duration / pow(2, noteDots));
    }
    // LogDebug("Duration %d; Dot %d; Alignment %f", noteDur, this->GetDots(), duration);
    return duration;
}

double DurationInterface::GetInterfaceAlignmentMensuralDuration(int num, int numBase, const Mensur *currentMensur) const
{
    int noteDur = this->GetDurGes() != DURATION_NONE ? this->GetActualDurGes() : this->GetActualDur();
    if (noteDur == DURATION_NONE) noteDur = DURATION_4;

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
        case DURATION_maxima:
            duration *= (double)abs(currentMensur->GetModusminor()) * (double)abs(currentMensur->GetModusmaior());
            break;
        case DURATION_long: duration *= (double)abs(currentMensur->GetModusminor()); break;
        case DURATION_breve: break;
        case DURATION_1: duration /= (double)abs(currentMensur->GetTempus()); break;
        default:
            ratio = pow(2.0, (double)(noteDur - DURATION_2));
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
    const Beam *beam = vrv_cast<const Beam *>(noteOrRest->GetFirstAncestor(BEAM, MAX_BEAM_DEPTH));
    if (!beam) {
        return false;
    }
    return (noteOrRest == beam->GetListFront());
}

bool DurationInterface::IsLastInBeam(const LayerElement *noteOrRest) const
{
    const Beam *beam = vrv_cast<const Beam *>(noteOrRest->GetFirstAncestor(BEAM, MAX_BEAM_DEPTH));
    if (!beam) {
        return false;
    }
    return (noteOrRest == beam->GetListBack());
}

data_DURATION DurationInterface::GetActualDur() const
{
    const data_DURATION dur = this->HasDur() ? this->GetDur() : this->GetDurDefault();
    return this->CalcActualDur(dur);
}

data_DURATION DurationInterface::GetActualDurGes() const
{
    const data_DURATION dur = this->HasDurGes() ? this->GetDurGes() : DURATION_NONE;
    return this->CalcActualDur(dur);
}

data_DURATION DurationInterface::CalcActualDur(data_DURATION dur) const
{
    // maxima (-1) is a mensural only value
    if (dur < DUR_MAX) return dur;
    // Mensural duration (except maxima)
    switch (dur) {
        case DURATION_longa: return DURATION_long;
        case DURATION_brevis: return DURATION_breve;
        case DURATION_semibrevis: return DURATION_1;
        case DURATION_minima: return DURATION_2;
        case DURATION_semiminima: return DURATION_4;
        case DURATION_fusa: return DURATION_8;
        case DURATION_semifusa: return DURATION_16;
        default: return DURATION_NONE;
    }
}

data_DURATION DurationInterface::GetNoteOrChordDur(const LayerElement *element) const
{
    if (element->Is(CHORD)) {
        data_DURATION duration = this->GetActualDur();
        if (duration != DURATION_NONE) return duration;

        const Chord *chord = vrv_cast<const Chord *>(element);
        for (const Note *note : { chord->GetTopNote(), chord->GetBottomNote() }) {
            duration = note->GetActualDur();
            if (duration != DURATION_NONE) {
                return duration;
            }
        }
    }
    else if (element->Is(NOTE)) {
        const Note *note = vrv_cast<const Note *>(element);
        assert(note);
        const Chord *chord = note->IsChordTone();
        return (chord && !this->HasDur()) ? chord->GetActualDur() : this->GetActualDur();
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
