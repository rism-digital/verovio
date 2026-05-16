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

    m_scoreTimeOnset = 0;
    m_scoreTimeOffset = 0;
    m_realTimeOnsetMilliseconds = 0;
    m_realTimeOffsetMilliseconds = 0;
    m_scoreTimeTiedDuration = 0;
}

Fraction DurationInterface::GetInterfaceAlignmentDuration(int num, int numBase) const
{
    data_DURATION noteDur = (this->GetDurGes() != DURATION_NONE) ? this->GetActualDurGes() : this->GetActualDur();
    if (noteDur == DURATION_NONE) noteDur = DURATION_4;

    if (this->HasNum()) num *= this->GetNum();
    if (this->HasNumbase()) numBase *= this->GetNumbase();

    Fraction duration(noteDur);
    duration = duration * numBase / num;

    int noteDots = (this->HasDotsGes()) ? this->GetDotsGes() : this->GetDots();
    if (noteDots != VRV_UNSET) {
        Fraction durationReduction(duration.GetNumerator(), duration.GetDenominator() * pow(2, noteDots));
        duration = duration * 2 - durationReduction;
    }
    // LogDebug("Duration %d; Dot %d; Alignment %f", noteDur, this->GetDots(), duration);
    return duration;
}

Fraction DurationInterface::GetInterfaceAlignmentMensuralDuration(
    int num, int numBase, const Mensur *currentMensur, data_DURATION equivalence) const
{
    data_DURATION noteDur = this->GetDurGes() != DURATION_NONE ? this->GetActualDurGes() : this->GetActualDur();
    if (noteDur == DURATION_NONE) noteDur = DURATION_4;

    if (!currentMensur) {
        LogWarning("No current mensur for calculating duration");
        return Fraction(1);
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

    Fraction duration;
    if (equivalence == DURATION_minima) {
        duration = this->DurationWithMinimaEquivalence(num, numBase, currentMensur, noteDur);
    }
    else if (equivalence == DURATION_semibrevis) {
        duration = this->DurationWithSemibrevisEquivalence(num, numBase, currentMensur, noteDur);
    }
    else {
        duration = this->DurationWithBrevisEquivalence(num, numBase, currentMensur, noteDur);
    }
    duration = duration * numBase / num;

    return duration;
}

Fraction DurationInterface::DurationWithBrevisEquivalence(
    int num, int numBase, const Mensur *currentMensur, data_DURATION noteDur) const
{
    int ratio = 0;
    Fraction duration(DURATION_breve);
    switch (noteDur) {
        case DURATION_maxima:
            duration = duration * abs(currentMensur->GetModusminor()) * abs(currentMensur->GetModusmaior());
            break;
        case DURATION_long: duration = duration * abs(currentMensur->GetModusminor()); break;
        case DURATION_breve: break;
        case DURATION_1: duration = duration / abs(currentMensur->GetTempus()); break;
        default:
            // ratio will be 1 for DURATION_2 (minima) where we apply the tempus and the prolatio
            ratio = pow(2.0, (double)(noteDur - DURATION_2));
            assert(ratio);
            duration = duration / abs(currentMensur->GetTempus()) / abs(currentMensur->GetProlatio()) / ratio;
            break;
    }
    return duration;
}

Fraction DurationInterface::DurationWithSemibrevisEquivalence(
    int num, int numBase, const Mensur *currentMensur, data_DURATION noteDur) const
{
    int ratio = 0;
    Fraction duration(DURATION_1);
    switch (noteDur) {
        case DURATION_maxima: duration = duration * abs(currentMensur->GetModusmaior()); [[fallthrough]];
        case DURATION_long: duration = duration * abs(currentMensur->GetModusminor()); [[fallthrough]];
        case DURATION_breve: duration = duration * abs(currentMensur->GetTempus()); [[fallthrough]];
        case DURATION_1: break;
        default:
            // ratio will be 1 for DURATION_2 (minima) where we apply the prolatio
            ratio = pow(2.0, (double)(noteDur - DURATION_2));
            assert(ratio);
            duration = duration / abs(currentMensur->GetProlatio()) / ratio;
            break;
    }
    return duration;
}

Fraction DurationInterface::DurationWithMinimaEquivalence(
    int num, int numBase, const Mensur *currentMensur, data_DURATION noteDur) const
{
    int ratio = 0;
    Fraction duration(DURATION_2);
    switch (noteDur) {
        case DURATION_maxima: duration = duration * abs(currentMensur->GetModusmaior()); [[fallthrough]];
        case DURATION_long: duration = duration * abs(currentMensur->GetModusminor()); [[fallthrough]];
        case DURATION_breve: duration = duration * abs(currentMensur->GetTempus()); [[fallthrough]];
        case DURATION_1: duration = duration * abs(currentMensur->GetProlatio()); break;
        default:
            // ratio will be 1 for DURATION_2 (minima)
            ratio = pow(2.0, (double)(noteDur - DURATION_2));
            assert(ratio);
            duration = duration / ratio;
            break;
    }
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
    // No mapping needed for values below, including maxima and NONE
    if (dur < DURATION_longa) return dur;
    // Mensural durations (except maxima)
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
    return (this->GetDur() >= DURATION_longa);
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

void DurationInterface::SetScoreTimeOnset(Fraction scoreTime)
{
    m_scoreTimeOnset = scoreTime;
}

void DurationInterface::SetRealTimeOnsetSeconds(double timeInSeconds)
{
    // m_realTimeOnsetMilliseconds = int(timeInSeconds * 1000.0 + 0.5);
    m_realTimeOnsetMilliseconds = timeInSeconds * 1000.0;
}

void DurationInterface::SetScoreTimeOffset(Fraction scoreTime)
{
    m_scoreTimeOffset = scoreTime;
}

void DurationInterface::SetRealTimeOffsetSeconds(double timeInSeconds)
{
    // m_realTimeOffsetMilliseconds = int(timeInSeconds * 1000.0 + 0.5);
    m_realTimeOffsetMilliseconds = timeInSeconds * 1000.0;
}

void DurationInterface::SetScoreTimeTiedDuration(Fraction scoreTime)
{
    m_scoreTimeTiedDuration = scoreTime;
}

Fraction DurationInterface::GetScoreTimeOnset() const
{
    return m_scoreTimeOnset;
}

double DurationInterface::GetRealTimeOnsetMilliseconds() const
{
    return m_realTimeOnsetMilliseconds;
}

Fraction DurationInterface::GetScoreTimeOffset() const
{
    return m_scoreTimeOffset;
}

double DurationInterface::GetRealTimeOffsetMilliseconds() const
{
    return m_realTimeOffsetMilliseconds;
}

Fraction DurationInterface::GetScoreTimeTiedDuration() const
{
    return m_scoreTimeTiedDuration;
}

Fraction DurationInterface::GetScoreTimeDuration() const
{
    return this->GetScoreTimeOffset() - this->GetScoreTimeOnset();
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

} // namespace vrv
