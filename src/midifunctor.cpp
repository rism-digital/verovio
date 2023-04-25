/////////////////////////////////////////////////////////////////////////////
// Name:        midifunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "midifunctor.h"

//----------------------------------------------------------------------------

#include "beatrpt.h"
#include "layer.h"
#include "multirest.h"
#include "rest.h"
#include "staff.h"
#include "tabgrp.h"
#include "tempo.h"
#include "tie.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// InitOnsetOffsetFunctor
//----------------------------------------------------------------------------

InitOnsetOffsetFunctor::InitOnsetOffsetFunctor()
{
    m_currentScoreTime = 0.0;
    m_currentRealTimeSeconds = 0.0;
    m_currentMensur = NULL;
    m_currentMeterSig = NULL;
    m_notationType = NOTATIONTYPE_cmn;
    m_currentTempo = MIDI_TEMPO;
}

FunctorCode InitOnsetOffsetFunctor::VisitChordEnd(Chord *chord)
{
    LayerElement *element = chord->ThisOrSameasLink();

    double incrementScoreTime = element->GetAlignmentDuration(m_currentMensur, m_currentMeterSig, true, m_notationType);
    incrementScoreTime = incrementScoreTime / (DUR_MAX / DURATION_4);
    double realTimeIncrementSeconds = incrementScoreTime * 60.0 / m_currentTempo;

    m_currentScoreTime += incrementScoreTime;
    m_currentRealTimeSeconds += realTimeIncrementSeconds;

    return FUNCTOR_CONTINUE;
}

FunctorCode InitOnsetOffsetFunctor::VisitLayer(Layer *layer)
{
    m_currentScoreTime = 0.0;
    m_currentRealTimeSeconds = 0.0;

    m_currentMensur = layer->GetCurrentMensur();
    m_currentMeterSig = layer->GetCurrentMeterSig();

    return FUNCTOR_CONTINUE;
}

FunctorCode InitOnsetOffsetFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    LayerElement *element = layerElement->ThisOrSameasLink();

    double incrementScoreTime;

    if (element->Is(REST) || element->Is(SPACE)) {
        incrementScoreTime = element->GetAlignmentDuration(m_currentMensur, m_currentMeterSig, true, m_notationType);
        incrementScoreTime = incrementScoreTime / (DUR_MAX / DURATION_4);
        // For rests to be possibly added to the timemap
        if (element->Is(REST)) {
            Rest *rest = vrv_cast<Rest *>(element);
            double realTimeIncrementSeconds = incrementScoreTime * 60.0 / m_currentTempo;
            rest->SetScoreTimeOnset(m_currentScoreTime);
            rest->SetRealTimeOnsetSeconds(m_currentRealTimeSeconds);
            rest->SetScoreTimeOffset(m_currentScoreTime + incrementScoreTime);
            rest->SetRealTimeOffsetSeconds(m_currentRealTimeSeconds + realTimeIncrementSeconds);
        }
        m_currentScoreTime += incrementScoreTime;
        m_currentRealTimeSeconds += incrementScoreTime * 60.0 / m_currentTempo;
    }
    else if (element->Is(NOTE)) {
        Note *note = vrv_cast<Note *>(element);
        assert(note);

        // For now just ignore grace notes
        if (note->IsGraceNote()) return FUNCTOR_CONTINUE;

        Chord *chord = note->IsChordTone();
        TabGrp *tabGrp = note->IsTabGrpNote();

        // If the note has a @dur or a @dur.ges, take it into account
        // This means that overwriting only @dots or @dots.ges will not be taken into account
        if (chord && !note->HasDur() && !note->HasDurGes()) {
            incrementScoreTime = chord->GetAlignmentDuration(m_currentMensur, m_currentMeterSig, true, m_notationType);
        }
        else if (tabGrp && !note->HasDur() && !note->HasDurGes()) {
            incrementScoreTime = tabGrp->GetAlignmentDuration(m_currentMensur, m_currentMeterSig, true, m_notationType);
        }
        else {
            incrementScoreTime = note->GetAlignmentDuration(m_currentMensur, m_currentMeterSig, true, m_notationType);
        }
        incrementScoreTime = incrementScoreTime / (DUR_MAX / DURATION_4);
        double realTimeIncrementSeconds = incrementScoreTime * 60.0 / m_currentTempo;

        // LogDebug("Note Alignment Duration %f - Dur %d - Diatonic Pitch %d - Track %d", GetAlignmentDuration(),
        // note->GetNoteOrChordDur(element), note->GetDiatonicPitch(), *midiTrack);
        // LogDebug("Oct %d - Pname %d - Accid %d", note->GetOct(), note->GetPname(), note->GetAccid());

        // When we have a @sameas, do store the onset / offset values of the pointed note in the pointing note
        Note *storeNote = (layerElement == element) ? note : dynamic_cast<Note *>(layerElement);
        if (storeNote) {
            storeNote->SetScoreTimeOnset(m_currentScoreTime);
            storeNote->SetRealTimeOnsetSeconds(m_currentRealTimeSeconds);
            storeNote->SetScoreTimeOffset(m_currentScoreTime + incrementScoreTime);
            storeNote->SetRealTimeOffsetSeconds(m_currentRealTimeSeconds + realTimeIncrementSeconds);
        }

        // increase the currentTime accordingly, but only if not in a chord or tabGrp
        if (!note->IsChordTone() && !note->IsTabGrpNote()) {
            m_currentScoreTime += incrementScoreTime;
            m_currentRealTimeSeconds += realTimeIncrementSeconds;
        }
    }
    else if (element->Is(BEATRPT)) {
        BeatRpt *rpt = vrv_cast<BeatRpt *>(element);
        assert(rpt);

        incrementScoreTime = rpt->GetAlignmentDuration(m_currentMensur, m_currentMeterSig, true, m_notationType);
        incrementScoreTime = incrementScoreTime / (DUR_MAX / DURATION_4);
        rpt->SetScoreTimeOnset(m_currentScoreTime);
        m_currentScoreTime += incrementScoreTime;
        m_currentRealTimeSeconds += incrementScoreTime * 60.0 / m_currentTempo;
    }
    else if (layerElement->Is({ BEAM, LIGATURE, FTREM, TUPLET }) && layerElement->HasSameasLink()) {
        incrementScoreTime
            = layerElement->GetSameAsContentAlignmentDuration(m_currentMensur, m_currentMeterSig, true, m_notationType);
        incrementScoreTime = incrementScoreTime / (DUR_MAX / DURATION_4);
        m_currentScoreTime += incrementScoreTime;
        m_currentRealTimeSeconds += incrementScoreTime * 60.0 / m_currentTempo;
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode InitOnsetOffsetFunctor::VisitMeasure(Measure *measure)
{
    m_currentTempo = measure->GetCurrentTempo();

    return FUNCTOR_CONTINUE;
}

FunctorCode InitOnsetOffsetFunctor::VisitStaff(Staff *staff)
{
    StaffDef *drawingStaffDef = staff->m_drawingStaffDef;
    assert(drawingStaffDef);

    m_notationType = (drawingStaffDef->HasNotationtype()) ? drawingStaffDef->GetNotationtype() : NOTATIONTYPE_cmn;

    return FUNCTOR_CONTINUE;
}

FunctorCode InitOnsetOffsetFunctor::VisitTabGrpEnd(TabGrp *tabGrp)
{
    LayerElement *element = tabGrp->ThisOrSameasLink();

    double incrementScoreTime = element->GetAlignmentDuration(m_currentMensur, m_currentMeterSig, true, m_notationType);
    incrementScoreTime = incrementScoreTime / (DUR_MAX / DURATION_4);
    double realTimeIncrementSeconds = incrementScoreTime * 60.0 / m_currentTempo;

    m_currentScoreTime += incrementScoreTime;
    m_currentRealTimeSeconds += realTimeIncrementSeconds;

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// InitMaxMeasureDurationFunctor
//----------------------------------------------------------------------------

InitMaxMeasureDurationFunctor::InitMaxMeasureDurationFunctor()
{
    m_currentScoreTime = 0.0;
    m_currentRealTimeSeconds = 0.0;
    m_currentTempo = MIDI_TEMPO;
    m_tempoAdjustment = 1.0;
    m_multiRestFactor = 1;
}

FunctorCode InitMaxMeasureDurationFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (layerElement->Is(MULTIREST)) {
        MultiRest *multiRest = vrv_cast<MultiRest *>(layerElement);
        assert(multiRest);
        m_multiRestFactor = multiRest->GetNum();
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode InitMaxMeasureDurationFunctor::VisitMeasure(Measure *measure)
{
    measure->ClearScoreTimeOffset();
    measure->AddScoreTimeOffset(m_currentScoreTime);

    measure->ClearRealTimeOffset();
    measure->AddRealTimeOffset(m_currentRealTimeSeconds * 1000.0);

    return FUNCTOR_CONTINUE;
}

FunctorCode InitMaxMeasureDurationFunctor::VisitMeasureEnd(Measure *measure)
{
    const double scoreTimeIncrement
        = measure->m_measureAligner.GetRightAlignment()->GetTime() * m_multiRestFactor * DURATION_4 / DUR_MAX;
    m_currentTempo = m_currentTempo * m_tempoAdjustment;
    m_currentScoreTime += scoreTimeIncrement;
    m_currentRealTimeSeconds += scoreTimeIncrement * 60.0 / m_currentTempo;
    m_multiRestFactor = 1;

    return FUNCTOR_CONTINUE;
}

FunctorCode InitMaxMeasureDurationFunctor::VisitScoreDef(ScoreDef *scoreDef)
{
    if (scoreDef->HasMidiBpm()) {
        m_currentTempo = scoreDef->GetMidiBpm();
    }
    else if (scoreDef->HasMm()) {
        m_currentTempo = Tempo::CalcTempo(scoreDef);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode InitMaxMeasureDurationFunctor::VisitTempo(Tempo *tempo)
{
    if (tempo->HasMidiBpm()) {
        m_currentTempo = tempo->GetMidiBpm();
    }
    else if (tempo->HasMm()) {
        m_currentTempo = Tempo::CalcTempo(tempo);
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// InitTimemapTiesFunctor
//----------------------------------------------------------------------------

InitTimemapTiesFunctor::InitTimemapTiesFunctor() {}

FunctorCode InitTimemapTiesFunctor::VisitTie(Tie *tie)
{
    Note *note1 = dynamic_cast<Note *>(tie->GetStart());
    Note *note2 = dynamic_cast<Note *>(tie->GetEnd());

    if (!note1 || !note2) {
        return FUNCTOR_CONTINUE;
    }

    double sttd2 = note2->GetScoreTimeTiedDuration();
    double std2 = note2->GetScoreTimeDuration();

    if (sttd2 > 0.0) {
        note1->SetScoreTimeTiedDuration(sttd2 + std2);
    }
    else {
        note1->SetScoreTimeTiedDuration(std2);
    }
    note2->SetScoreTimeTiedDuration(-1.0);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
