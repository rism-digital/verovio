/////////////////////////////////////////////////////////////////////////////
// Name:        midifunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "midifunctor.h"

//----------------------------------------------------------------------------

#include "arpeg.h"
#include "beatrpt.h"
#include "btrem.h"
#include "featureextractor.h"
#include "ftrem.h"
#include "gracegrp.h"
#include "layer.h"
#include "multirest.h"
#include "pedal.h"
#include "rest.h"
#include "staff.h"
#include "syl.h"
#include "tabgrp.h"
#include "tempo.h"
#include "text.h"
#include "tie.h"
#include "timemap.h"
#include "tuplet.h"
#include "verse.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "MidiFile.h"

namespace vrv {

//----------------------------------------------------------------------------
// InitOnsetOffsetFunctor
//----------------------------------------------------------------------------

InitOnsetOffsetFunctor::InitOnsetOffsetFunctor() : Functor()
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
    else if (layerElement->Is(MENSUR)) {
        this->m_currentMensur = vrv_cast<Mensur *>(layerElement);
    }
    else if (layerElement->Is(METERSIG)) {
        this->m_currentMeterSig = vrv_cast<MeterSig *>(layerElement);
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

InitMaxMeasureDurationFunctor::InitMaxMeasureDurationFunctor() : Functor()
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
    const double tempo = this->GetAdjustedTempo();
    measure->SetCurrentTempo(tempo);

    const double scoreTimeIncrement = measure->m_measureAligner.GetRightAlignment()->GetTime() * m_multiRestFactor
        * static_cast<int>(DURATION_4) / DUR_MAX;
    m_currentScoreTime += scoreTimeIncrement;
    m_currentRealTimeSeconds += scoreTimeIncrement * 60.0 / tempo;
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

InitTimemapTiesFunctor::InitTimemapTiesFunctor() : Functor() {}

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

//----------------------------------------------------------------------------
// InitMidiFunctor
//----------------------------------------------------------------------------

InitMIDIFunctor::InitMIDIFunctor() : ConstFunctor()
{
    m_currentTempo = MIDI_TEMPO;
}

FunctorCode InitMIDIFunctor::VisitArpeg(const Arpeg *arpeg)
{
    // Sort the involved notes by playing order
    const bool playTopDown = (arpeg->GetOrder() == arpegLog_ORDER_down);
    std::set<const Note *> notes = arpeg->GetNotes();
    std::vector<const Note *> sortedNotes;
    std::copy(notes.begin(), notes.end(), std::back_inserter(sortedNotes));
    std::sort(sortedNotes.begin(), sortedNotes.end(), [playTopDown](const Note *note1, const Note *note2) {
        const int pitch1 = note1->GetMIDIPitch();
        const int pitch2 = note2->GetMIDIPitch();
        return playTopDown ? (pitch1 > pitch2) : (pitch1 < pitch2);
    });

    // Defer the notes in playing order
    double shift = 0.0;
    const double increment = UNACC_GRACENOTE_DUR * m_currentTempo / 60000.0;
    for (const Note *note : sortedNotes) {
        if (shift > 0.0) m_deferredNotes[note] = shift;
        shift += increment;
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode InitMIDIFunctor::VisitMeasure(const Measure *measure)
{
    m_currentTempo = measure->GetCurrentTempo();

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// GenerateMIDIFunctor
//----------------------------------------------------------------------------

GenerateMIDIFunctor::GenerateMIDIFunctor(smf::MidiFile *midiFile) : ConstFunctor()
{
    m_midiFile = midiFile;
    m_midiTrack = 1;
    m_midiChannel = 0;
    m_totalTime = 0.0;
    m_staffN = 0;
    m_transSemi = 0;
    m_currentTempo = MIDI_TEMPO;
    m_lastNote = NULL;
    m_accentedGraceNote = false;
    m_cueExclusion = false;
}

FunctorCode GenerateMIDIFunctor::VisitBeatRpt(const BeatRpt *beatRpt)
{
    // Sameas not taken into account for now
    double beatLength = beatRpt->GetAlignmentDuration() / (DUR_MAX / DURATION_4);
    double startTime = m_totalTime + beatRpt->GetScoreTimeOnset();
    int tpq = m_midiFile->getTPQ();

    // filter last beat and copy all notes
    smf::MidiEvent event;
    int eventCount = m_midiFile->getEventCount(m_midiTrack);
    for (int i = 0; i < eventCount; ++i) {
        event = m_midiFile->getEvent(m_midiTrack, i);
        if (event.tick > startTime * tpq)
            break;
        else if (event.tick >= (startTime - beatLength) * tpq) {
            if (((event[0] & 0xf0) == 0x80) || ((event[0] & 0xf0) == 0x90)) {
                m_midiFile->addEvent(m_midiTrack, event.tick + beatLength * tpq, event);
            }
        }
    }

    for (int i = 0; i < beatLength * tpq; ++i) {
        // LogWarning("%i", i);
        // smf::MidiEvent event = m_midiFile->getEvent(m_midiTrack, startTime * tpq);
        // event.clearVariables();
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode GenerateMIDIFunctor::VisitBTrem(const BTrem *bTrem)
{
    // Do nothing if the tremolo is unmeasured
    if (bTrem->GetForm() == tremForm_FORM_unmeas) {
        return FUNCTOR_CONTINUE;
    }

    // Adjust duration of the bTrem if it's nested within tuplet
    int num = 0;
    const Tuplet *tuplet = vrv_cast<const Tuplet *>(bTrem->GetFirstAncestor(TUPLET, MAX_TUPLET_DEPTH));
    if (tuplet) {
        num = (tuplet->GetNum() > 0) ? tuplet->GetNum() : 0;
    }
    // Get num value if it's set
    if (bTrem->HasNum()) {
        num = bTrem->GetNum();
    }

    // Calculate duration of individual note in tremolo
    const data_DURATION individualNoteDur = bTrem->CalcIndividualNoteDuration();
    if (individualNoteDur == DURATION_NONE) return FUNCTOR_CONTINUE;
    const double noteInQuarterDur = pow(2.0, (DURATION_4 - individualNoteDur));

    // Define lambda which expands one note into multiple individual notes of the same pitch
    auto expandNote = [this, noteInQuarterDur, num](const Object *obj) {
        const Note *note = vrv_cast<const Note *>(obj);
        assert(note);
        const int pitch = note->GetMIDIPitch(m_transSemi);
        const double totalInQuarterDur = note->GetScoreTimeDuration() + note->GetScoreTimeTiedDuration();
        int multiplicity = totalInQuarterDur / noteInQuarterDur;
        double noteDuration = noteInQuarterDur;
        // if NUM has been set for the bTrem, override calculated values
        if (num) {
            multiplicity = num;
            noteDuration = totalInQuarterDur / double(num);
        }
        m_expandedNotes[note] = MIDINoteSequence(multiplicity, { pitch, noteDuration });
    };

    // Apply expansion either to all notes in chord or to first note
    const Chord *chord = vrv_cast<const Chord *>(bTrem->FindDescendantByType(CHORD));
    if (chord) {
        ListOfConstObjects notes = chord->FindAllDescendantsByType(NOTE, false);
        std::for_each(notes.begin(), notes.end(), expandNote);
    }
    else {
        const Object *note = bTrem->FindDescendantByType(NOTE);
        if (note) {
            expandNote(note);
        }
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode GenerateMIDIFunctor::VisitChord(const Chord *chord)
{
    // Handle grace chords
    if (chord->IsGraceNote()) {
        std::set<int> pitches;
        const ListOfConstObjects &notes = chord->GetList();
        for (const Object *obj : notes) {
            const Note *note = vrv_cast<const Note *>(obj);
            assert(note);
            pitches.insert(note->GetMIDIPitch(m_transSemi));
        }

        double quarterDuration = 0.0;
        const data_DURATION dur = chord->GetDur();
        if ((dur >= DURATION_long) && (dur <= DURATION_1024)) {
            quarterDuration = pow(2.0, (DURATION_4 - dur));
        }

        m_graceNotes.push_back({ pitches, quarterDuration });

        bool accented = (chord->GetGrace() == GRACE_acc);
        const GraceGrp *graceGrp = vrv_cast<const GraceGrp *>(chord->GetFirstAncestor(GRACEGRP));
        if (graceGrp && (graceGrp->GetGrace() == GRACE_acc)) accented = true;
        m_accentedGraceNote = accented;

        return FUNCTOR_SIBLINGS;
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode GenerateMIDIFunctor::VisitFTrem(const FTrem *fTrem)
{
    if (fTrem->HasUnitdur()) {
        LogWarning("FTrem produces incorrect MIDI output");
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode GenerateMIDIFunctor::VisitGraceGrpEnd(const GraceGrp *graceGrp)
{
    // Handling of Nachschlag
    if (!m_graceNotes.empty() && (graceGrp->GetAttach() == graceGrpLog_ATTACH_pre) && !m_accentedGraceNote
        && m_lastNote) {
        double startTime = m_totalTime + m_lastNote->GetScoreTimeOffset();
        const double graceNoteDur = UNACC_GRACENOTE_DUR * m_currentTempo / 60000.0;
        const double totalDur = graceNoteDur * m_graceNotes.size();
        startTime -= totalDur;
        startTime = std::max(startTime, 0.0);

        int velocity = MIDI_VELOCITY;
        if (m_lastNote->HasVel()) velocity = m_lastNote->GetVel();
        const int tpq = m_midiFile->getTPQ();

        for (const MIDIChord &chord : m_graceNotes) {
            const double stopTime = startTime + graceNoteDur;
            for (int pitch : chord.pitches) {
                m_midiFile->addNoteOn(m_midiTrack, startTime * tpq, m_midiChannel, pitch, velocity);
                m_midiFile->addNoteOff(m_midiTrack, stopTime * tpq, m_midiChannel, pitch);
            }
            startTime = stopTime;
        }

        m_graceNotes.clear();
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode GenerateMIDIFunctor::VisitHalfmRpt(const HalfmRpt *halfmRpt)
{
    LogWarning("HalfmRpt produces empty MIDI output");

    return FUNCTOR_CONTINUE;
}

FunctorCode GenerateMIDIFunctor::VisitLayer(const Layer *layer)
{
    if ((layer->GetCue() == BOOLEAN_true) && m_cueExclusion) return FUNCTOR_SIBLINGS;

    return FUNCTOR_CONTINUE;
}

FunctorCode GenerateMIDIFunctor::VisitLayerEnd(const Layer *layer)
{
    // stop all previously held notes
    for (auto &held : m_heldNotes) {
        if (held.m_pitch > 0) {
            m_midiFile->addNoteOff(m_midiTrack, held.m_stopTime * m_midiFile->getTPQ(), m_midiChannel, held.m_pitch);
        }
    }

    m_heldNotes.clear();

    return FUNCTOR_CONTINUE;
}

FunctorCode GenerateMIDIFunctor::VisitLayerElement(const LayerElement *layerElement)
{
    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    // Only resolve simple sameas links to avoid infinite recursion
    const LayerElement *sameas = dynamic_cast<const LayerElement *>(layerElement->GetSameasLink());
    if (sameas && !sameas->HasSameasLink()) {
        sameas->Process(*this);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode GenerateMIDIFunctor::VisitMeasure(const Measure *measure)
{
    // Here we need to update the m_totalTime from the starting time of the measure.
    m_totalTime = measure->GetLastTimeOffset();

    if (measure->GetCurrentTempo() != m_currentTempo) {
        m_currentTempo = measure->GetCurrentTempo();
        m_midiFile->addTempo(0, m_totalTime * m_midiFile->getTPQ(), m_currentTempo);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode GenerateMIDIFunctor::VisitMRpt(const MRpt *mRpt)
{
    LogWarning("MRpt produces empty MIDI output");

    return FUNCTOR_CONTINUE;
}

FunctorCode GenerateMIDIFunctor::VisitNote(const Note *note)
{
    // Skip linked notes
    if (note->HasSameasLink()) {
        return FUNCTOR_SIBLINGS;
    }

    // Skip cue notes when midiNoCue is activated
    if ((note->GetCue() == BOOLEAN_true) && m_cueExclusion) {
        return FUNCTOR_SIBLINGS;
    }

    // If the note is a secondary tied note, then ignore it
    if (note->GetScoreTimeTiedDuration() < 0.0) {
        return FUNCTOR_SIBLINGS;
    }

    // Handle grace notes
    if (note->IsGraceNote()) {
        const int pitch = note->GetMIDIPitch(m_transSemi);

        double quarterDuration = 0.0;
        const data_DURATION dur = note->GetDur();
        if ((dur >= DURATION_long) && (dur <= DURATION_1024)) {
            quarterDuration = pow(2.0, (DURATION_4 - dur));
        }

        m_graceNotes.push_back({ { pitch }, quarterDuration });

        bool accented = (note->GetGrace() == GRACE_acc);
        const GraceGrp *graceGrp = vrv_cast<const GraceGrp *>(note->GetFirstAncestor(GRACEGRP));
        if (graceGrp && (graceGrp->GetGrace() == GRACE_acc)) accented = true;
        m_accentedGraceNote = accented;

        return FUNCTOR_SIBLINGS;
    }

    const int channel = m_midiChannel;
    int velocity = MIDI_VELOCITY;
    if (note->HasVel()) velocity = note->GetVel();

    double startTime = m_totalTime + note->GetScoreTimeOnset();
    const int tpq = m_midiFile->getTPQ();

    // Check if some grace notes must be performed
    if (!m_graceNotes.empty()) {
        this->GenerateGraceNoteMIDI(note, startTime, tpq, channel, velocity);
        m_graceNotes.clear();
    }

    // Check if note is deferred
    if (m_deferredNotes.find(note) != m_deferredNotes.end()) {
        startTime += m_deferredNotes.at(note);
        m_deferredNotes.erase(note);
    }

    // Check if note was expanded into sequence of short notes due to trills/tremolandi
    // Play either the expanded note sequence or a single note
    if (m_expandedNotes.find(note) != m_expandedNotes.end()) {
        for (const auto &midiNote : m_expandedNotes.at(note)) {
            const double stopTime = startTime + midiNote.duration;

            m_midiFile->addNoteOn(m_midiTrack, startTime * tpq, channel, midiNote.pitch, velocity);
            m_midiFile->addNoteOff(m_midiTrack, stopTime * tpq, channel, midiNote.pitch);

            startTime = stopTime;
        }
    }
    else {
        const int pitch = note->GetMIDIPitch(m_transSemi);

        if (note->HasTabCourse() && (note->GetTabCourse() >= 1)) {
            // Tablature 'rule of holds'.  A note on a course is held until the next note
            // on that course is required, or until a default hold duration is reached.

            const int course = note->GetTabCourse();
            if (m_heldNotes.size() < static_cast<size_t>(course)) {
                m_heldNotes.resize(course); // make room
            }

            // if a previously held note on this course is already sounding, end it now.
            if (m_heldNotes[course - 1].m_pitch > 0) {
                m_heldNotes[course - 1].m_stopTime = startTime; // stop now
            }

            // end all previously held notes that have reached their stoptime
            // or if the new pitch is already sounding, on any course
            for (auto &held : m_heldNotes) {
                if ((held.m_pitch > 0) && ((held.m_stopTime <= startTime) || (held.m_pitch == pitch))) {
                    m_midiFile->addNoteOff(m_midiTrack, held.m_stopTime * tpq, channel, held.m_pitch);
                    held.m_pitch = 0;
                    held.m_stopTime = 0;
                }
            }

            // hold this note until the greater of its rhythm sign and the default duration.
            // TODO optimize the default hold duration
            const double defaultHoldTime = 4; // quarter notes
            m_heldNotes[course - 1].m_pitch = pitch;
            m_heldNotes[course - 1].m_stopTime = m_totalTime
                + std::max(defaultHoldTime, note->GetScoreTimeOffset() + note->GetScoreTimeTiedDuration());

            // start this note
            m_midiFile->addNoteOn(m_midiTrack, startTime * tpq, channel, pitch, velocity);
        }
        else {
            const double stopTime = m_totalTime + note->GetScoreTimeOffset() + note->GetScoreTimeTiedDuration();

            m_midiFile->addNoteOn(m_midiTrack, startTime * tpq, channel, pitch, velocity);
            m_midiFile->addNoteOff(m_midiTrack, stopTime * tpq, channel, pitch);
        }
    }

    // Store reference, i.e. for Nachschlag
    m_lastNote = note;

    return FUNCTOR_CONTINUE;
}

FunctorCode GenerateMIDIFunctor::VisitPedal(const Pedal *pedal)
{
    if (!pedal->HasDir()) return FUNCTOR_CONTINUE;

    double pedalTime = pedal->GetStart()->GetAlignment()->GetTime() * static_cast<int>(DURATION_4) / DUR_MAX;
    double startTime = m_totalTime + pedalTime;
    int tpq = m_midiFile->getTPQ();

    // todo: check pedal @func to switch between sustain/soften/damper pedals?
    switch (pedal->GetDir()) {
        case pedalLog_DIR_down: m_midiFile->addSustainPedalOn(m_midiTrack, (startTime * tpq), m_midiChannel); break;
        case pedalLog_DIR_up: m_midiFile->addSustainPedalOff(m_midiTrack, (startTime * tpq), m_midiChannel); break;
        case pedalLog_DIR_bounce:
            m_midiFile->addSustainPedalOff(m_midiTrack, (startTime * tpq), m_midiChannel);
            m_midiFile->addSustainPedalOn(m_midiTrack, (startTime * tpq) + 0.1, m_midiChannel);
            break;
        default: return FUNCTOR_CONTINUE;
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode GenerateMIDIFunctor::VisitScoreDef(const ScoreDef *scoreDef)
{
    double totalTime = m_totalTime;
    // check next measure for the time offset
    const Object *parent = scoreDef->GetParent();
    if (parent && (parent->GetLast() != scoreDef)) {
        const Object *next = parent->GetNext(scoreDef);
        if (next && next->Is(MEASURE)) {
            const Measure *nextMeasure = vrv_cast<const Measure *>(next);
            totalTime = nextMeasure->GetLastTimeOffset();
        }
    }
    const double currentTick = totalTime * m_midiFile->getTPQ();

    smf::MidiEvent midiEvent;
    midiEvent.tick = currentTick;
    // calculate reference pitch class based on @tune.pname
    int referencePitchClass = 0;
    if (scoreDef->HasTunePname()) {
        referencePitchClass = Note::PnameToPclass(scoreDef->GetTunePname());
    }
    // set temperament event if corresponding attribute present
    if (scoreDef->HasTuneTemper()) {
        switch (scoreDef->GetTuneTemper()) {
            case TEMPERAMENT_equal: midiEvent.makeTemperamentEqual(referencePitchClass); break;
            case TEMPERAMENT_just: midiEvent.makeTemperamentBad(100.0, referencePitchClass); break;
            case TEMPERAMENT_mean: midiEvent.makeTemperamentMeantone(referencePitchClass); break;
            case TEMPERAMENT_pythagorean: midiEvent.makeTemperamentPythagorean(referencePitchClass); break;
            default: break;
        }
        m_midiFile->addEvent(m_midiTrack, midiEvent);
    }
    // set tuning
    if (scoreDef->HasTuneHz()) {
        const double tuneHz = scoreDef->GetTuneHz();
        // Add tuning for all keys from 0 to 127
        std::vector<std::pair<int, double>> tuneFrequencies;
        for (int i = 0; i < 127; ++i) {
            double freq = pow(2.0, (i - 69.0) / 12.0) * tuneHz;
            tuneFrequencies.push_back(std::make_pair(i, freq));
        }
        midiEvent.makeMts2_KeyTuningsByFrequency(tuneFrequencies);
        m_midiFile->addEvent(m_midiTrack, midiEvent);
    }
    // set MIDI key signature
    if (scoreDef->HasKeySigInfo()) {
        const KeySig *keySig = vrv_cast<const KeySig *>(scoreDef->GetKeySig());
        if (keySig && keySig->HasSig()) {
            // m_midiFile->addKeySignature(
            //     m_midiTrack, currentTick, keySig->GetFifthsInt(), (keySig->GetMode() == MODE_minor));
        }
    }
    // set MIDI time signature
    if (scoreDef->HasMeterSigInfo()) {
        const MeterSig *meterSig = vrv_cast<const MeterSig *>(scoreDef->GetMeterSig());
        if (meterSig && meterSig->HasCount() && meterSig->HasUnit()) {
            m_midiFile->addTimeSignature(m_midiTrack, currentTick, meterSig->GetTotalCount(), meterSig->GetUnit());
        }
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode GenerateMIDIFunctor::VisitStaff(const Staff *staff)
{
    m_expandedNotes.clear();

    return FUNCTOR_CONTINUE;
}

FunctorCode GenerateMIDIFunctor::VisitStaffDef(const StaffDef *staffDef)
{
    if (staffDef->GetN() == m_staffN) {
        // Update the semitone transposition
        if (staffDef->HasTransSemi()) m_transSemi = staffDef->GetTransSemi();
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode GenerateMIDIFunctor::VisitSyl(const Syl *syl)
{
    const double startTime = m_totalTime + m_lastNote->GetScoreTimeOnset();
    const std::string sylText = UTF32to8(syl->GetText());

    m_midiFile->addLyric(m_midiTrack, startTime * m_midiFile->getTPQ(), sylText);

    return FUNCTOR_SIBLINGS;
}

FunctorCode GenerateMIDIFunctor::VisitVerse(const Verse *verse)
{
    const LayerElement *parent = vrv_cast<const Note *>(verse->GetFirstAncestor(NOTE));
    if (!parent) parent = vrv_cast<const Chord *>(verse->GetFirstAncestor(CHORD));
    assert(parent);

    const Verse *previousVerse = vrv_cast<const Verse *>(parent->GetPrevious(verse, VERSE));

    if (previousVerse) return FUNCTOR_SIBLINGS;

    return FUNCTOR_CONTINUE;
}

void GenerateMIDIFunctor::DeferMIDINote(const Note *refNote, double shift, bool includeChordSiblings)
{
    // Recursive call for chords
    const Chord *chord = refNote->IsChordTone();
    if (chord && includeChordSiblings) {
        const ListOfConstObjects &notes = chord->GetList();

        for (const Object *obj : notes) {
            const Note *note = vrv_cast<const Note *>(obj);
            assert(note);
            this->DeferMIDINote(note, shift, false);
        }
        return;
    }

    // Register the shift
    if (shift < refNote->GetScoreTimeDuration() + refNote->GetScoreTimeTiedDuration()) {
        m_deferredNotes[refNote] = shift;
    }
}

void GenerateMIDIFunctor::GenerateGraceNoteMIDI(
    const Note *refNote, double startTime, int tpq, int channel, int velocity)
{
    double graceNoteDur = 0.0;
    if (m_accentedGraceNote && !m_graceNotes.empty()) {
        const double totalDur = refNote->GetScoreTimeDuration() / 2.0;
        this->DeferMIDINote(refNote, totalDur, true);
        graceNoteDur = totalDur / m_graceNotes.size();
    }
    else {
        graceNoteDur = UNACC_GRACENOTE_DUR * m_currentTempo / 60000.0;
        const double totalDur = graceNoteDur * m_graceNotes.size();
        if (startTime >= totalDur) {
            startTime -= totalDur;
        }
        else {
            this->DeferMIDINote(refNote, totalDur, true);
        }
    }

    for (const MIDIChord &chord : m_graceNotes) {
        const double stopTime = startTime + graceNoteDur;
        for (int pitch : chord.pitches) {
            m_midiFile->addNoteOn(m_midiTrack, startTime * tpq, channel, pitch, velocity);
            m_midiFile->addNoteOff(m_midiTrack, stopTime * tpq, channel, pitch);
        }
        startTime = stopTime;
    }
}

//----------------------------------------------------------------------------
// GenerateTimemapFunctor
//----------------------------------------------------------------------------

GenerateTimemapFunctor::GenerateTimemapFunctor(Timemap *timemap) : ConstFunctor()
{
    m_scoreTimeOffset = 0.0;
    m_realTimeOffsetMilliseconds = 0.0;
    m_currentTempo = MIDI_TEMPO;
    m_cueExclusion = false;
    m_timemap = timemap;
}

FunctorCode GenerateTimemapFunctor::VisitLayerElement(const LayerElement *layerElement)
{
    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    // Only resolve simple sameas links to avoid infinite recursion
    const LayerElement *sameas = dynamic_cast<const LayerElement *>(layerElement->GetSameasLink());
    if (sameas && !sameas->HasSameasLink()) {
        sameas->Process(*this);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode GenerateTimemapFunctor::VisitMeasure(const Measure *measure)
{
    m_scoreTimeOffset = measure->GetLastTimeOffset();
    m_realTimeOffsetMilliseconds = measure->GetLastRealTimeOffset();
    m_currentTempo = measure->GetCurrentTempo();

    this->AddTimemapEntry(measure);

    return FUNCTOR_CONTINUE;
}

FunctorCode GenerateTimemapFunctor::VisitNote(const Note *note)
{
    if (note->HasGrace()) return FUNCTOR_SIBLINGS;

    // Skip cue notes when midiNoCue is activated
    if ((note->GetCue() == BOOLEAN_true) && m_cueExclusion) {
        return FUNCTOR_SIBLINGS;
    }

    note = dynamic_cast<const Note *>(note->ThisOrSameasLink());
    assert(note);

    this->AddTimemapEntry(note);

    return FUNCTOR_SIBLINGS;
}

FunctorCode GenerateTimemapFunctor::VisitRest(const Rest *rest)
{
    this->AddTimemapEntry(rest);

    return FUNCTOR_SIBLINGS;
}

void GenerateTimemapFunctor::AddTimemapEntry(const Object *object)
{
    assert(object);

    if (object->Is({ NOTE, REST })) {
        const DurationInterface *interface = object->GetDurationInterface();
        assert(interface);

        double realTimeStart = round(m_realTimeOffsetMilliseconds + interface->GetRealTimeOnsetMilliseconds());
        double scoreTimeStart = m_scoreTimeOffset + interface->GetScoreTimeOnset();

        double realTimeEnd = round(m_realTimeOffsetMilliseconds + interface->GetRealTimeOffsetMilliseconds());
        double scoreTimeEnd = m_scoreTimeOffset + interface->GetScoreTimeOffset();

        bool isRest = (object->Is(REST));

        /*********** start values ***********/

        TimemapEntry &startEntry = m_timemap->GetEntry(realTimeStart);

        // Should check if value for realTimeStart already exists and if so, then
        // ensure that it is equal to scoreTimeStart:
        startEntry.qstamp = scoreTimeStart;

        // Store the element ID in list to turn on at given time - note or rest
        if (!isRest) startEntry.notesOn.push_back(object->GetID());
        if (isRest) startEntry.restsOn.push_back(object->GetID());

        // Also add the tempo
        startEntry.tempo = m_currentTempo;

        /*********** end values ***********/

        TimemapEntry &endEntry = m_timemap->GetEntry(realTimeEnd);

        // Should check if value for realTimeEnd already exists and if so, then
        // ensure that it is equal to scoreTimeEnd:
        endEntry.qstamp = scoreTimeEnd;

        // Store the element ID in list to turn off at given time - notes or rest
        if (!isRest) endEntry.notesOff.push_back(object->GetID());
        if (isRest) endEntry.restsOff.push_back(object->GetID());
    }
    else if (object->Is(MEASURE)) {

        const Measure *measure = vrv_cast<const Measure *>(object);
        assert(measure);

        // Deal with repeated music later, for now get the last times.
        double scoreTimeStart = m_scoreTimeOffset;
        double realTimeStart = round(m_realTimeOffsetMilliseconds);

        TimemapEntry &startEntry = m_timemap->GetEntry(realTimeStart);

        // Should check if value for realTimeStart already exists and if so, then
        // ensure that it is equal to scoreTimeStart:
        startEntry.qstamp = scoreTimeStart;

        // Add the measureOn
        startEntry.measureOn = measure->GetID();
    }
}

//----------------------------------------------------------------------------
// GenerateFeaturesFunctor
//----------------------------------------------------------------------------

GenerateFeaturesFunctor::GenerateFeaturesFunctor(FeatureExtractor *extractor) : ConstFunctor()
{
    m_extractor = extractor;
}

FunctorCode GenerateFeaturesFunctor::VisitObject(const Object *object)
{
    m_extractor->Extract(object);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
