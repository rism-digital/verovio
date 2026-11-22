/////////////////////////////////////////////////////////////////////////////
// Name:        midibuilder.cpp
// Author:      Daniel Kasper
// Created:     2026
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "midibuilder.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <fstream>
#include <set>
#include <sstream>
#include <utility>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "customtuning.h"
#include "doc.h"
#include "instrdef.h"
#include "keysig.h"
#include "label.h"
#include "metersig.h"
#include "midifunctor.h"
#include "miscfunctor.h"
#include "options.h"
#include "scoredef.h"
#include "scoreorpartelement.h"
#include "staffdef.h"
#include "staffgrp.h"
#include "tempo.h"
#include "vrv.h"

namespace vrv {

MIDIBuilder::MIDIBuilder(Format format) : m_ownedMidiFile(std::in_place), m_midiFile(*m_ownedMidiFile)
{
    if (format == Format::Json) m_json.emplace(this->GetTPQ());
}

MIDIBuilder::MIDIBuilder(Format format, smf::MidiFile &midiFile) : m_midiFile(midiFile)
{
    if (format == Format::Json) m_json.emplace(this->GetTPQ());
}

void MIDIBuilder::Build(Doc &doc)
{
    m_midiFile.absoluteTicks();

    if (!doc.HasTimemap()) {
        // generate MIDI timemap before progressing
        doc.CalculateTimemap();
    }
    if (!doc.HasTimemap()) {
        LogWarning("Calculation of the timemap failed, MIDI cannot be exported.");
    }

    double tempo = MIDI_TEMPO;
    std::set<int> tempoEventTicks; // track the ticks of added tempo events

    // set MIDI tempo
    ScoreDef *scoreDef = doc.GetFirstVisibleScoreOrPart()->GetScoreDef();
    assert(scoreDef);
    if (scoreDef->HasMidiBpm()) {
        tempo = scoreDef->GetMidiBpm();
        tempoEventTicks.insert(0);
        this->AddTempo(0, 0, tempo);
    }
    else if (scoreDef->HasMm()) {
        tempo = Tempo::CalcTempo(scoreDef);
        tempoEventTicks.insert(0);
        this->AddTempo(0, 0, tempo);
    }

    // set MIDI tuning
    const std::string midiTuningFile = doc.GetOptions()->m_midiTuningFile.GetStrValue();
    if (!midiTuningFile.empty()) {
        std::string tuningDef;
        std::ifstream f(midiTuningFile.c_str());
        if (f.good()) {
            const std::string ext(".ascl");
            if (midiTuningFile.size() >= ext.size()
                && midiTuningFile.substr(midiTuningFile.size() - ext.size()) == ext) {
                std::stringstream buffer;
                buffer << f.rdbuf();
                tuningDef = buffer.str();
            }
            else {
                LogError("Tuning file '%s' is not recognized", midiTuningFile.c_str());
            }
        }
        else {
            tuningDef = midiTuningFile;
        }
        if (!tuningDef.empty()) {
            CustomTuning *tuning = new CustomTuning(tuningDef, &doc, false);
            if (tuning->IsValid()) {
                scoreDef->SetCustomTuning(tuning);
            }
            else {
                delete tuning;
                LogWarning("Error parsing tuning %s", f.good() ? "file" : "definition");
            }
        }
    }

    // Capture information for MIDI generation, i.e. from control elements
    InitMIDIFunctor initMIDI;
    initMIDI.SetCurrentTempo(tempo);
    doc.Process(initMIDI);

    // We need to populate processing lists for processing the document by Layer (by Verse will not be used)
    InitProcessingListsFunctor initProcessingLists;

    // We first fill a tree of int with [staff/layer] and [staff/layer/verse] numbers (@n) to be processed
    doc.Process(initProcessingLists);
    const IntTree &layerTree = initProcessingLists.GetLayerTree();

    // The tree is used to process each staff/layer/verse separately
    // Match each staff and layer by type and @n.

    // Process notes and chords, rests, spaces layer by layer
    // track 0 (included by default) is reserved for meta messages common to all tracks
    int midiChannel = 0;
    int midiTrack = 1;
    Filters filters;
    for (const std::pair<const int, IntTree> &staves : layerTree.child) {
        int transSemi = 0;
        InstrDef *instrDef = NULL;
        if (StaffDef *staffDef = scoreDef->GetStaffDef(staves.first)) {
            // get the transposition (semi-tone) value for the staff
            if (staffDef->HasTransSemi()) transSemi = staffDef->GetTransSemi();
            midiTrack = staffDef->GetN();
            this->EnsureTrack(midiTrack);
            // set MIDI channel and instrument
            instrDef = vrv_cast<InstrDef *>(staffDef->FindDescendantByType(INSTRDEF, 1));
            if (!instrDef) {
                StaffGrp *staffGrp = vrv_cast<StaffGrp *>(staffDef->GetFirstAncestor(STAFFGRP));
                assert(staffGrp);
                instrDef = vrv_cast<InstrDef *>(staffGrp->FindDescendantByType(INSTRDEF, 1));
            }
            if (instrDef) {
                if (instrDef->HasMidiChannel()) midiChannel = instrDef->GetMidiChannel();
                if (instrDef->HasMidiTrack()) {
                    midiTrack = instrDef->GetMidiTrack();
                    this->EnsureTrack(midiTrack);
                    if (midiTrack > 255) {
                        LogWarning("A high MIDI track number was assigned to staff %d", staffDef->GetN());
                    }
                }
                if (instrDef->HasMidiInstrnum()) {
                    this->AddProgramChange(midiTrack, 0, midiChannel, instrDef->GetMidiInstrnum());
                }
            }
            // set MIDI track name
            Label *label = vrv_cast<Label *>(staffDef->FindDescendantByType(LABEL, 1));
            if (!label) {
                StaffGrp *staffGrp = vrv_cast<StaffGrp *>(staffDef->GetFirstAncestor(STAFFGRP));
                assert(staffGrp);
                label = vrv_cast<Label *>(staffGrp->FindDescendantByType(LABEL, 1));
            }
            if (label) {
                std::string trackName = UTF32to8(label->GetText()).c_str();
                if (!trackName.empty()) {
                    this->AddTrackName(midiTrack, 0, trackName);
                }
            }
            // set MIDI key signature
            KeySig *keySig = vrv_cast<KeySig *>(staffDef->FindDescendantByType(KEYSIG));
            if (!keySig && (scoreDef->HasKeySigInfo())) {
                keySig = vrv_cast<KeySig *>(scoreDef->GetKeySig());
            }
            if (keySig && keySig->HasSig()) {
                this->AddKeySignature(midiTrack, 0, keySig->GetFifthsInt(), (keySig->GetMode() == MODE_minor));
            }
            // set MIDI time signature
            MeterSig *meterSig = vrv_cast<MeterSig *>(staffDef->FindDescendantByType(METERSIG));
            if (!meterSig && (scoreDef->HasMeterSigInfo())) {
                meterSig = vrv_cast<MeterSig *>(scoreDef->GetMeterSig());
            }
            if (meterSig && meterSig->HasCount() && meterSig->HasUnit()) {
                this->AddTimeSignature(midiTrack, 0, meterSig->GetTotalCount(), meterSig->GetUnit());
            }
            else if (meterSig && meterSig->HasSym()) {
                this->AddTimeSignature(midiTrack, 0, meterSig->GetTotalCount(), meterSig->GetSymImplicitUnit());
            }
        }

        // Set initial scoreDef values for tuning
        GenerateMIDIFunctor generateScoreDefMIDI(*this);
        generateScoreDefMIDI.SetChannel(midiChannel);
        generateScoreDefMIDI.SetTrack(midiTrack);
        generateScoreDefMIDI.SetInstrDef(instrDef);

        scoreDef->Process(generateScoreDefMIDI);

        bool controlEvents = true;

        for (const std::pair<const int, IntTree> &layers : staves.second.child) {
            filters.Clear();
            AttNIntegerComparison matchStaff(STAFF, staves.first);
            AttNIntegerComparison matchLayer(LAYER, layers.first);
            filters.Add(&matchStaff);
            filters.Add(&matchLayer);

            GenerateMIDIFunctor generateMIDI(*this);
            generateMIDI.SetFilters(&filters);

            generateMIDI.SetChannel(midiChannel);
            generateMIDI.SetTrack(midiTrack);
            generateMIDI.SetStaffN(staves.first);
            generateMIDI.SetLayerN(layers.first);
            generateMIDI.SetTempoEventTicks(tempoEventTicks);
            generateMIDI.SetTransSemi(transSemi);
            generateMIDI.SetCurrentTempo(tempo);
            generateMIDI.SetOctaves(initMIDI.GetOctaves());
            generateMIDI.SetNoCue(doc.GetOptions()->m_midiNoCue.GetValue());
            generateMIDI.SetControlEvents(controlEvents);
            generateMIDI.SetInstrDef(instrDef);
            generateMIDI.SetCustomTuning(scoreDef->GetCustomTuning());

            // LogDebug("Exporting track %d ----------------", midiTrack);
            doc.Process(generateMIDI);

            tempoEventTicks = generateMIDI.GetTempoEventTicks();
            // Process them only once per staff
            controlEvents = false;
        }
    }
    m_midiFile.sortTracksNoteOffsBeforeOns();
}

bool MIDIBuilder::Write(std::ostream &output)
{
    return m_midiFile.write(output);
}

bool MIDIBuilder::Write(const std::string &filename)
{
    return m_midiFile.write(filename);
}

std::string MIDIBuilder::ToJson() const
{
    assert(m_json);
    std::string output;
    m_json->Finalize(m_midiFile.getTrackCount(), output);
    return output;
}

int MIDIBuilder::GetTPQ() const
{
    return m_midiFile.getTPQ();
}

void MIDIBuilder::AddNoteOn(int track, int tick, int channel, int key, int velocity, const std::string &id)
{
    m_midiFile.addNoteOn(track, tick, channel, key, velocity);
    if (m_json) m_json->AddNoteOn(track, tick, channel, key, velocity, id);
}

void MIDIBuilder::AddNoteOff(int track, int tick, int channel, int key)
{
    m_midiFile.addNoteOff(track, tick, channel, key);
    if (m_json) m_json->AddNoteOff(track, tick, channel, key);
}

void MIDIBuilder::AddSustainPedal(int track, int tick, int channel, bool on)
{
    if (on) {
        m_midiFile.addSustainPedalOn(track, tick, channel);
    }
    else {
        m_midiFile.addSustainPedalOff(track, tick, channel);
    }
    // The sustain pedal is controller 64.
    if (m_json) m_json->AddController(track, tick, channel, 64, on ? 127 : 0);
}

void MIDIBuilder::AddProgramChange(int track, int tick, int channel, int program)
{
    m_midiFile.addPatchChange(track, tick, channel, program);
    if (m_json) m_json->AddProgramChange(track, tick, channel, program);
}

void MIDIBuilder::AddTempo(int track, int tick, double bpm)
{
    m_midiFile.addTempo(track, tick, bpm);
    if (m_json) m_json->AddTempo(track, tick, bpm);
}

void MIDIBuilder::AddTimeSignature(int track, int tick, int numerator, int denominator)
{
    m_midiFile.addTimeSignature(track, tick, numerator, denominator);
    if (m_json) m_json->AddTimeSignature(track, tick, numerator, denominator);
}

void MIDIBuilder::AddKeySignature(int track, int tick, int fifths, bool minor)
{
    m_midiFile.addKeySignature(track, tick, fifths, minor);
    if (m_json) m_json->AddKeySignature(track, tick, fifths, minor);
}

void MIDIBuilder::AddTrackName(int track, int tick, const std::string &name)
{
    m_midiFile.addTrackName(track, tick, name);
    if (m_json) m_json->AddMeta(track, tick, JsonEvents::MetaType::TrackName, name);
}

void MIDIBuilder::AddLyric(int track, int tick, const std::string &text)
{
    m_midiFile.addLyric(track, tick, text);
    if (m_json) m_json->AddMeta(track, tick, JsonEvents::MetaType::Lyrics, text);
}

void MIDIBuilder::AddTuningEvent(int track, smf::MidiEvent &event)
{
    m_midiFile.addEvent(track, event);
    // Keep the JSON event indices aligned even though tuning has no JSON representation.
    if (m_json) m_json->AddOther(track, event.tick);
}

void MIDIBuilder::CopyNotesForBeatRepeat(
    int track, double firstTick, double lastTick, double tickOffset, const std::string &id)
{
    const int eventCount = m_midiFile.getEventCount(track);
    assert(!m_json || m_json->GetEventCount(track) == eventCount);
    smf::MidiEvent event;
    for (int i = 0; i < eventCount; ++i) {
        event = m_midiFile.getEvent(track, i);
        if (event.tick > lastTick) break;
        const int command = event.getCommandNibble();
        if (event.tick >= firstTick && (command == 0x80 || command == 0x90)) {
            const int tick = event.tick + tickOffset;
            m_midiFile.addEvent(track, tick, event);
            if (m_json) m_json->CopyNoteEvent(track, i, tick, id);
        }
    }
}

void MIDIBuilder::EnsureTrack(int track)
{
    const int missingTracks = track + 1 - m_midiFile.getTrackCount();
    if (missingTracks > 0) m_midiFile.addTracks(missingTracks);
}

} // namespace vrv
