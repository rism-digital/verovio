/////////////////////////////////////////////////////////////////////////////
// Name:        midibuilder.cpp
// Author:      Daniel Kasper
// Created:     2026
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "midibuilder.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <list>
#include <map>
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

//----------------------------------------------------------------------------

#include "jsonxx.h"

namespace vrv {

namespace {

    // The names of the General MIDI programs and families, as in \@tonejs/midi
    const std::array<const char *, 128> instrumentNames = { "acoustic grand piano", "bright acoustic piano",
        "electric grand piano", "honky-tonk piano", "electric piano 1", "electric piano 2", "harpsichord", "clavi",
        "celesta", "glockenspiel", "music box", "vibraphone", "marimba", "xylophone", "tubular bells", "dulcimer",
        "drawbar organ", "percussive organ", "rock organ", "church organ", "reed organ", "accordion", "harmonica",
        "tango accordion", "acoustic guitar (nylon)", "acoustic guitar (steel)", "electric guitar (jazz)",
        "electric guitar (clean)", "electric guitar (muted)", "overdriven guitar", "distortion guitar",
        "guitar harmonics", "acoustic bass", "electric bass (finger)", "electric bass (pick)", "fretless bass",
        "slap bass 1", "slap bass 2", "synth bass 1", "synth bass 2", "violin", "viola", "cello", "contrabass",
        "tremolo strings", "pizzicato strings", "orchestral harp", "timpani", "string ensemble 1", "string ensemble 2",
        "synthstrings 1", "synthstrings 2", "choir aahs", "voice oohs", "synth voice", "orchestra hit", "trumpet",
        "trombone", "tuba", "muted trumpet", "french horn", "brass section", "synthbrass 1", "synthbrass 2",
        "soprano sax", "alto sax", "tenor sax", "baritone sax", "oboe", "english horn", "bassoon", "clarinet",
        "piccolo", "flute", "recorder", "pan flute", "blown bottle", "shakuhachi", "whistle", "ocarina",
        "lead 1 (square)", "lead 2 (sawtooth)", "lead 3 (calliope)", "lead 4 (chiff)", "lead 5 (charang)",
        "lead 6 (voice)", "lead 7 (fifths)", "lead 8 (bass + lead)", "pad 1 (new age)", "pad 2 (warm)",
        "pad 3 (polysynth)", "pad 4 (choir)", "pad 5 (bowed)", "pad 6 (metallic)", "pad 7 (halo)", "pad 8 (sweep)",
        "fx 1 (rain)", "fx 2 (soundtrack)", "fx 3 (crystal)", "fx 4 (atmosphere)", "fx 5 (brightness)",
        "fx 6 (goblins)", "fx 7 (echoes)", "fx 8 (sci-fi)", "sitar", "banjo", "shamisen", "koto", "kalimba", "bag pipe",
        "fiddle", "shanai", "tinkle bell", "agogo", "steel drums", "woodblock", "taiko drum", "melodic tom",
        "synth drum", "reverse cymbal", "guitar fret noise", "breath noise", "seashore", "bird tweet", "telephone ring",
        "helicopter", "applause", "gunshot" };

    const std::array<const char *, 16> instrumentFamilies
        = { "piano", "chromatic percussion", "organ", "guitar", "bass", "strings", "ensemble", "brass", "reed", "pipe",
              "synth lead", "synth pad", "synth effects", "world", "percussive", "sound effects" };

    const std::map<int, const char *> drumKits = { { 0, "standard kit" }, { 8, "room kit" }, { 16, "power kit" },
        { 24, "electronic kit" }, { 25, "tr-808 kit" }, { 32, "jazz kit" }, { 40, "brush kit" },
        { 48, "orchestra kit" }, { 56, "sound fx kit" } };

    // Major and minor keys by the number of fifths (from -7 to 7)
    const std::array<std::array<const char *, 15>, 2> keyNames
        = { { { "Cb", "Gb", "Db", "Ab", "Eb", "Bb", "F", "C", "G", "D", "A", "E", "B", "F#", "C#" },
            { "Ab", "Eb", "Bb", "F", "C", "G", "D", "A", "E", "B", "F#", "C#", "G#", "D#", "A#" } } };

    const std::array<const char *, 12> pitchClasses
        = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

    // The MIDI channel of percussion in General MIDI
    const int percussionChannel = 9;

} // namespace

MIDIBuilder::MIDIBuilder(Format format)
{
    if (format == Format::Json) {
        m_jsonTracks.emplace(1); // Track 0 holds common meta events.
    }
    else {
        m_midiFile = &m_ownedMidiFile.emplace();
        m_midiFile->setTPQ(s_defaultTPQ);
    }
}

MIDIBuilder::MIDIBuilder(Format format, smf::MidiFile &midiFile) : m_midiFile(&midiFile)
{
    if (format == Format::Json) m_jsonTracks.emplace(midiFile.getTrackCount());
}

void MIDIBuilder::Build(Doc &doc)
{
    if (m_midiFile) m_midiFile->absoluteTicks();

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
    if (m_midiFile) m_midiFile->sortTracksNoteOffsBeforeOns();
}

bool MIDIBuilder::Write(std::ostream &output)
{
    return m_midiFile && m_midiFile->write(output);
}

bool MIDIBuilder::Write(const std::string &filename)
{
    return m_midiFile && m_midiFile->write(filename);
}

int MIDIBuilder::GetTPQ() const
{
    return m_midiFile ? m_midiFile->getTPQ() : s_defaultTPQ;
}

// JSON uses the same channel, key and value ranges as the MIDI messages.

void MIDIBuilder::AddNoteOn(int track, int tick, int channel, int key, int velocity, const std::string &id)
{
    if (m_midiFile) m_midiFile->addNoteOn(track, tick, channel, key, velocity);
    if (!m_jsonTracks) return;
    // A note-on with velocity 0 is a note-off.
    const bool isNoteOn = ((velocity & 0x7f) > 0);
    Event &event = this->AddJsonEvent(track, isNoteOn ? EventType::NoteOn : EventType::NoteOff, tick);
    event.channel = channel & 0x0f;
    event.data1 = key & 0x7f;
    event.data2 = velocity & 0x7f;
    if (isNoteOn) event.text = id;
}

void MIDIBuilder::AddNoteOff(int track, int tick, int channel, int key)
{
    if (m_midiFile) m_midiFile->addNoteOff(track, tick, channel, key);
    if (!m_jsonTracks) return;
    Event &event = this->AddJsonEvent(track, EventType::NoteOff, tick);
    event.channel = channel & 0x0f;
    event.data1 = key & 0x7f;
}

void MIDIBuilder::AddSustainPedal(int track, int tick, int channel, bool on)
{
    if (m_midiFile) {
        if (on) {
            m_midiFile->addSustainPedalOn(track, tick, channel);
        }
        else {
            m_midiFile->addSustainPedalOff(track, tick, channel);
        }
    }
    if (!m_jsonTracks) return;
    // The sustain pedal is controller 64.
    Event &event = this->AddJsonEvent(track, EventType::Controller, tick);
    event.channel = channel & 0x0f;
    event.data1 = 64;
    event.data2 = on ? 127 : 0;
}

void MIDIBuilder::AddProgramChange(int track, int tick, int channel, int program)
{
    if (m_midiFile) m_midiFile->addPatchChange(track, tick, channel, program);
    if (!m_jsonTracks) return;
    Event &event = this->AddJsonEvent(track, EventType::ProgramChange, tick);
    event.channel = channel & 0x0f;
    event.data1 = program & 0x7f;
}

void MIDIBuilder::AddTempo(int track, int tick, double bpm)
{
    if (m_midiFile) m_midiFile->addTempo(track, tick, bpm);
    if (m_jsonTracks) this->AddJsonEvent(track, EventType::Tempo, tick).bpm = bpm;
}

void MIDIBuilder::AddTimeSignature(int track, int tick, int numerator, int denominator)
{
    if (m_midiFile) m_midiFile->addTimeSignature(track, tick, numerator, denominator);
    if (!m_jsonTracks) return;
    Event &event = this->AddJsonEvent(track, EventType::TimeSignature, tick);
    event.data1 = numerator;
    event.data2 = denominator;
}

void MIDIBuilder::AddKeySignature(int track, int tick, int fifths, bool minor)
{
    if (m_midiFile) m_midiFile->addKeySignature(track, tick, fifths, minor);
    if (!m_jsonTracks) return;
    Event &event = this->AddJsonEvent(track, EventType::KeySignature, tick);
    event.data1 = fifths;
    event.data2 = minor ? 1 : 0;
}

void MIDIBuilder::AddTrackName(int track, int tick, const std::string &name)
{
    if (m_midiFile) m_midiFile->addTrackName(track, tick, name);
    if (m_jsonTracks) this->AddJsonMeta(track, tick, MetaType::TrackName, name);
}

void MIDIBuilder::AddLyric(int track, int tick, const std::string &text)
{
    if (m_midiFile) m_midiFile->addLyric(track, tick, text);
    if (m_jsonTracks) this->AddJsonMeta(track, tick, MetaType::Lyrics, text);
}

void MIDIBuilder::AddTuningEvent(int track, smf::MidiEvent &event)
{
    if (m_midiFile) m_midiFile->addEvent(track, event);
    // Tuning events contribute to the end-of-track tick in JSON.
    if (m_jsonTracks) this->AddJsonEvent(track, EventType::Tuning, event.tick);
}

void MIDIBuilder::CopyNotesForBeatRepeat(
    int track, double firstTick, double lastTick, double tickOffset, const std::string &id)
{
    if (m_jsonTracks) this->CopyJsonNotesForBeatRepeat(track, firstTick, lastTick, tickOffset, id);
    if (!m_midiFile) return;
    const int eventCount = m_midiFile->getEventCount(track);
    smf::MidiEvent event;
    for (int i = 0; i < eventCount; ++i) {
        event = m_midiFile->getEvent(track, i);
        if (event.tick > lastTick) break;
        const int command = event.getCommandNibble();
        if (event.tick >= firstTick && (command == 0x80 || command == 0x90)) {
            const int tick = event.tick + tickOffset;
            m_midiFile->addEvent(track, tick, event);
        }
    }
}

void MIDIBuilder::EnsureTrack(int track)
{
    if (m_midiFile) {
        const int missingTracks = track + 1 - m_midiFile->getTrackCount();
        if (missingTracks > 0) m_midiFile->addTracks(missingTracks);
    }
    if (m_jsonTracks && (track >= (int)m_jsonTracks->size())) {
        m_jsonTracks->resize(track + 1);
    }
}

MIDIBuilder::Event &MIDIBuilder::AddJsonEvent(int track, EventType type, int tick)
{
    assert(track >= 0);
    if (track >= (int)m_jsonTracks->size()) m_jsonTracks->resize(track + 1);
    return m_jsonTracks->at(track).emplace_back(Event{ type, tick });
}

void MIDIBuilder::AddJsonMeta(int track, int tick, MetaType type, const std::string &text)
{
    Event &event = this->AddJsonEvent(track, EventType::Meta, tick);
    event.metaType = type;
    event.text = text;
}

void MIDIBuilder::CopyJsonNotesForBeatRepeat(
    int track, double firstTick, double lastTick, double tickOffset, const std::string &id)
{
    if ((track < 0) || (track >= (int)m_jsonTracks->size())) return;
    std::vector<Event> &events = m_jsonTracks->at(track);
    const int eventCount = (int)events.size();
    for (int i = 0; i < eventCount; ++i) {
        const Event &source = events.at(i);
        if (source.tick > lastTick) break;
        const bool isNote = (source.type == EventType::NoteOn) || (source.type == EventType::NoteOff);
        if (source.tick >= firstTick && isNote) {
            Event event = source;
            event.tick = source.tick + tickOffset;
            if (event.type == EventType::NoteOn) event.text = id;
            events.push_back(std::move(event));
        }
    }
}

//----------------------------------------------------------------------------
// JSON serialization
//----------------------------------------------------------------------------

std::string MIDIBuilder::ToJson() const
{
    assert(m_jsonTracks);
    const int tpq = this->GetTPQ();

    // A track of the JSON: the events of one MIDI track with one channel and program (see below)
    struct JsonTrack {
        std::vector<const Event *> events;
        // The first JSON track of a MIDI track also gets its end and its events without a channel
        bool isFirst;
        int endOfTrackTicks;
    };

    const auto hasChannel = [](const Event &event) {
        return (event.type == EventType::NoteOn) || (event.type == EventType::NoteOff)
            || (event.type == EventType::Controller) || (event.type == EventType::ProgramChange);
    };

    // The order of the events in a track of the MIDI file: by tick, then meta events before channel events, notes
    // after other events, note-ons before note-offs, and notes and controllers by number
    const auto isBefore = [](const Event *a, const Event *b) {
        if (a->tick != b->tick) return (a->tick < b->tick);
        if (IsMeta(*a) != IsMeta(*b)) return IsMeta(*a);
        if (IsMeta(*a)) return false;
        const bool aIsNote = (a->type == EventType::NoteOn) || (a->type == EventType::NoteOff);
        const bool bIsNote = (b->type == EventType::NoteOn) || (b->type == EventType::NoteOff);
        if (aIsNote != bIsNote) return bIsNote;
        if (aIsNote) {
            if (a->type != b->type) return (a->type == EventType::NoteOn);
            return (a->data1 < b->data1);
        }
        if ((a->type == EventType::Controller) && (b->type == EventType::Controller)) {
            if (a->data1 != b->data1) return (a->data1 < b->data1);
            return (a->data2 < b->data2);
        }
        return false;
    };

    // Split the MIDI tracks so that each JSON track has one channel and one program, as \@tonejs/midi does
    std::vector<JsonTrack> jsonTracks;
    for (const std::vector<Event> &trackEvents : *m_jsonTracks) {
        std::vector<const Event *> events;
        events.reserve(trackEvents.size());
        for (const Event &event : trackEvents) events.push_back(&event);
        std::stable_sort(events.begin(), events.end(), isBefore);

        const int firstTrack = (int)jsonTracks.size();
        jsonTracks.push_back({ {}, true, events.empty() ? 0 : events.back()->tick });
        // The JSON track by program and channel, and the current program of each channel
        std::map<std::pair<int, int>, int> trackIndices;
        std::array<int, 16> programs;
        programs.fill(0);
        for (const Event *event : events) {
            int target = firstTrack;
            if (hasChannel(*event)) {
                if (event->type == EventType::ProgramChange) programs.at(event->channel) = event->data1;
                const std::pair<int, int> key = { programs.at(event->channel), event->channel };
                if (trackIndices.count(key) == 0) {
                    const int index = firstTrack + (int)trackIndices.size();
                    trackIndices[key] = index;
                    if (index >= (int)jsonTracks.size()) jsonTracks.push_back({ {}, false, 0 });
                }
                target = trackIndices.at(key);
            }
            jsonTracks.at(target).events.push_back(event);
        }
    }

    // Header: tempos, time and key signatures, lyrics and markers from all the tracks, the name, text and cue points
    // from the first one
    struct TempoEvent {
        int ticks;
        double bpm;
        double time;
    };
    struct TimeSignatureEvent {
        int ticks;
        int numerator;
        int denominator;
        double measures;
    };
    std::vector<TempoEvent> tempos;
    std::vector<TimeSignatureEvent> timeSignatures;
    jsonxx::Array keySignatures;
    jsonxx::Array meta;
    std::string name;

    const auto metaEvent = [](const Event &event) {
        static const std::map<MetaType, std::string> types
            = { { MetaType::Text, "text" }, { MetaType::TrackName, "trackName" }, { MetaType::Lyrics, "lyrics" },
                  { MetaType::Marker, "marker" }, { MetaType::CuePoint, "cuePoint" } };
        jsonxx::Object object;
        object << "text" << event.text;
        object << "ticks" << event.tick;
        object << "type" << types.at(event.metaType);
        return object;
    };

    for (const JsonTrack &jsonTrack : jsonTracks) {
        for (const Event *event : jsonTrack.events) {
            if (event->type == EventType::Tempo) {
                tempos.push_back({ event->tick, event->bpm, 0.0 });
            }
            else if (event->type == EventType::TimeSignature) {
                timeSignatures.push_back({ event->tick, event->data1, event->data2, 0.0 });
            }
            else if (event->type == EventType::KeySignature) {
                jsonxx::Object keySignature;
                if ((event->data1 >= -7) && (event->data1 <= 7)) {
                    keySignature << "key" << keyNames.at(event->data2).at(event->data1 + 7);
                }
                keySignature << "scale" << ((event->data2 == 0) ? "major" : "minor");
                keySignature << "ticks" << event->tick;
                keySignatures << keySignature;
            }
            else if ((event->type == EventType::Meta)
                && ((event->metaType == MetaType::Lyrics) || (event->metaType == MetaType::Marker))) {
                meta << metaEvent(*event);
            }
        }
    }
    if (!jsonTracks.empty()) {
        for (const Event *event : jsonTracks.front().events) {
            if (event->type != EventType::Meta) continue;
            if (event->metaType == MetaType::TrackName) {
                name = event->text;
            }
            else if ((event->metaType == MetaType::Text) || (event->metaType == MetaType::CuePoint)) {
                meta << metaEvent(*event);
            }
        }
    }

    // Times in seconds of the tempo changes and positions in measures of the time signatures
    std::stable_sort(
        tempos.begin(), tempos.end(), [](const TempoEvent &a, const TempoEvent &b) { return a.ticks < b.ticks; });
    // Up to the first tempo event, its own tempo applies
    for (int i = 0; i < (int)tempos.size(); ++i) {
        const double lastTime = (i > 0) ? tempos.at(i - 1).time : 0.0;
        const int lastTicks = (i > 0) ? tempos.at(i - 1).ticks : 0;
        const double lastBpm = tempos.at(std::max(0, i - 1)).bpm;
        tempos.at(i).time = lastTime + 60.0 / lastBpm * (tempos.at(i).ticks - lastTicks) / tpq;
    }
    std::stable_sort(timeSignatures.begin(), timeSignatures.end(),
        [](const TimeSignatureEvent &a, const TimeSignatureEvent &b) { return a.ticks < b.ticks; });
    for (int i = 1; i < (int)timeSignatures.size(); ++i) {
        const TimeSignatureEvent &last = timeSignatures.at(i - 1);
        const double quarters = (double)(timeSignatures.at(i).ticks - last.ticks) / tpq;
        timeSignatures.at(i).measures = last.measures + quarters * last.denominator / last.numerator / 4.0;
    }

    // Without a tempo event, 120 bpm is assumed
    const auto ticksToSeconds = [&tempos, tpq](int ticks) {
        std::vector<TempoEvent>::const_reverse_iterator tempo = std::find_if(
            tempos.rbegin(), tempos.rend(), [ticks](const TempoEvent &event) { return event.ticks <= ticks; });
        if (tempo == tempos.rend()) return 0.5 * ticks / tpq;
        return tempo->time + 60.0 / tempo->bpm * (ticks - tempo->ticks) / tpq;
    };

    jsonxx::Object header;
    header << "keySignatures" << keySignatures;
    header << "meta" << meta;
    header << "name" << name;
    header << "ppq" << tpq;
    jsonxx::Array tempoArray;
    for (const TempoEvent &tempo : tempos) {
        jsonxx::Object tempoEvent;
        tempoEvent << "bpm" << tempo.bpm;
        tempoEvent << "ticks" << tempo.ticks;
        tempoArray << tempoEvent;
    }
    header << "tempos" << tempoArray;
    jsonxx::Array timeSignatureArray;
    for (const TimeSignatureEvent &timeSignature : timeSignatures) {
        jsonxx::Array value;
        value << timeSignature.numerator << timeSignature.denominator;
        jsonxx::Object timeSignatureEvent;
        timeSignatureEvent << "measures" << timeSignature.measures;
        timeSignatureEvent << "ticks" << timeSignature.ticks;
        timeSignatureEvent << "timeSignature" << value;
        timeSignatureArray << timeSignatureEvent;
    }
    header << "timeSignatures" << timeSignatureArray;

    // Tracks
    jsonxx::Array tracks;
    for (int i = 0; i < (int)jsonTracks.size(); ++i) {
        const JsonTrack &jsonTrack = jsonTracks.at(i);
        std::string trackName;
        bool hasTrackName = false;
        int program = 0;
        bool hasProgram = false;
        int channel = 0;
        std::vector<const Event *> noteOns;
        // Note-off events by key, in their order
        std::map<int, std::list<const Event *>> noteOffs;
        std::map<int, jsonxx::Array> controlChanges;
        for (const Event *event : jsonTrack.events) {
            switch (event->type) {
                case EventType::NoteOn: noteOns.push_back(event); break;
                case EventType::NoteOff: noteOffs[event->data1].push_back(event); break;
                case EventType::Controller: {
                    // Controller 127 is left out as in \@tonejs/midi
                    if (event->data1 >= 127) break;
                    jsonxx::Object controlChange;
                    controlChange << "number" << event->data1;
                    controlChange << "ticks" << event->tick;
                    controlChange << "time" << ticksToSeconds(event->tick);
                    controlChange << "value" << event->data2 / 127.0;
                    controlChanges[event->data1] << controlChange;
                    break;
                }
                case EventType::ProgramChange:
                    if (!hasProgram) program = event->data1;
                    hasProgram = true;
                    break;
                case EventType::Meta:
                    if ((event->metaType == MetaType::TrackName) && !hasTrackName) {
                        trackName = event->text;
                        hasTrackName = true;
                    }
                    break;
                default: break;
            }
        }

        // Pair each note-on with the first following note-off of the same key
        jsonxx::Array notes;
        double endTime = 0.0;
        for (const Event *noteOn : noteOns) {
            channel = noteOn->channel;
            std::list<const Event *> &offs = noteOffs[noteOn->data1];
            std::list<const Event *>::iterator noteOff = std::find_if(
                offs.begin(), offs.end(), [noteOn](const Event *event) { return event->tick >= noteOn->tick; });
            if (noteOff == offs.end()) continue;
            const int ticks = noteOn->tick;
            const int durationTicks = (*noteOff)->tick - ticks;
            offs.erase(noteOff);

            const double time = ticksToSeconds(ticks);
            const double duration = ticksToSeconds(ticks + durationTicks) - time;
            endTime = std::max(endTime, time + duration);

            jsonxx::Object note;
            note << "duration" << duration;
            note << "durationTicks" << durationTicks;
            if (!noteOn->text.empty()) note << "id" << noteOn->text;
            note << "midi" << noteOn->data1;
            note << "name"
                 << (std::string(pitchClasses.at(noteOn->data1 % 12)) + std::to_string(noteOn->data1 / 12 - 1));
            note << "ticks" << ticks;
            note << "time" << time;
            note << "velocity" << noteOn->data2 / 127.0;
            notes << note;
        }
        // The first track is left out when it has no notes (it holds the tempo events)
        if ((i == 0) && (endTime == 0.0)) continue;

        jsonxx::Object controlChangeObject;
        for (const std::pair<const int, jsonxx::Array> &controlChange : controlChanges) {
            controlChangeObject << std::to_string(controlChange.first) << controlChange.second;
        }
        jsonxx::Object instrument;
        if (channel == percussionChannel) {
            instrument << "family" << "drums";
            if (drumKits.count(program)) instrument << "name" << drumKits.at(program);
        }
        else {
            instrument << "family" << instrumentFamilies.at(program / 8);
            instrument << "name" << instrumentNames.at(program);
        }
        instrument << "number" << program;

        jsonxx::Object trackObject;
        trackObject << "channel" << channel;
        trackObject << "controlChanges" << controlChangeObject;
        if (jsonTrack.isFirst) trackObject << "endOfTrackTicks" << jsonTrack.endOfTrackTicks;
        trackObject << "instrument" << instrument;
        trackObject << "name" << trackName;
        trackObject << "notes" << notes;
        trackObject << "pitchBends" << jsonxx::Array();
        tracks << trackObject;
    }

    jsonxx::Object midi;
    midi << "header" << header;
    midi << "tracks" << tracks;
    return midi.json();
}

bool MIDIBuilder::IsMeta(const Event &event)
{
    switch (event.type) {
        case EventType::Tempo:
        case EventType::TimeSignature:
        case EventType::KeySignature:
        case EventType::Meta: return true;
        default: return false;
    }
}

} // namespace vrv
