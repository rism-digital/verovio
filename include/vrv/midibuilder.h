/////////////////////////////////////////////////////////////////////////////
// Name:        midibuilder.h
// Author:      Daniel Kasper
// Created:     2026
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MIDIBUILDER_H__
#define __VRV_MIDIBUILDER_H__

#include <iosfwd>
#include <optional>
#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "MidiFile.h"

namespace vrv {

class Doc;

//----------------------------------------------------------------------------
// MIDIBuilder
//----------------------------------------------------------------------------

/**
 * Builds the MIDI events of a playback document for binary or JSON output.
 */
class MIDIBuilder {
public:
    enum class Format { Midi, Json };

    explicit MIDIBuilder(Format format);
    /** Use a caller-owned MIDI file, as in Doc::ExportMIDI(). */
    MIDIBuilder(Format format, smf::MidiFile &midiFile);
    MIDIBuilder(const MIDIBuilder &) = delete;
    MIDIBuilder &operator=(const MIDIBuilder &) = delete;

    /** Prepare timing as needed, collect events, and sort the binary tracks. */
    void Build(Doc &doc);
    bool Write(std::ostream &output);
    bool Write(const std::string &filename);
    std::string ToJson() const;
    int GetTPQ() const;

    /**
     * @name Add events, with the values written to the MIDI file
     */
    ///@{
    void AddNoteOn(int track, int tick, int channel, int key, int velocity, const std::string &id);
    void AddNoteOff(int track, int tick, int channel, int key);
    void AddSustainPedal(int track, int tick, int channel, bool on);
    void AddProgramChange(int track, int tick, int channel, int program);
    void AddTempo(int track, int tick, double bpm);
    void AddTimeSignature(int track, int tick, int numerator, int denominator);
    void AddKeySignature(int track, int tick, int fifths, bool minor);
    void AddTrackName(int track, int tick, const std::string &name);
    void AddLyric(int track, int tick, const std::string &text);
    void AddTuningEvent(int track, smf::MidiEvent &event);
    ///@}

    /**
     * Copy the note events in a tick range to the repeated beat.
     * Copied note-ons are assigned the repeat's ID in JSON.
     */
    void CopyNotesForBeatRepeat(int track, double firstTick, double lastTick, double tickOffset, const std::string &id);

private:
    void EnsureTrack(int track);

    /**
     * Mirrors the binary event indices while building the Tone.js-compatible JSON with note IDs.
     */
    class JsonEvents {
    public:
        enum class MetaType { Text, TrackName, Lyrics, Marker, CuePoint };

        explicit JsonEvents(int ppq);
        void AddNoteOn(int track, int tick, int channel, int key, int velocity, const std::string &id);
        void AddNoteOff(int track, int tick, int channel, int key);
        void AddController(int track, int tick, int channel, int number, int value);
        void AddProgramChange(int track, int tick, int channel, int program);
        void AddTempo(int track, int tick, double bpm);
        void AddTimeSignature(int track, int tick, int numerator, int denominator);
        void AddKeySignature(int track, int tick, int fifths, bool minor);
        void AddMeta(int track, int tick, MetaType type, const std::string &text);
        void AddOther(int track, int tick);
        void CopyNoteEvent(int track, int index, int tick, const std::string &id);
        int GetEventCount(int track) const;
        void Finalize(int trackCount, std::string &output) const;

    private:
        enum class EventType {
            NoteOn,
            NoteOff,
            Controller,
            ProgramChange,
            Tempo,
            TimeSignature,
            KeySignature,
            Meta,
            Other
        };

        struct Event {
            EventType type;
            int tick;
            int channel;
            // Key, controller, program, numerator or fifths
            int data1;
            // Velocity, controller value, denominator or minor (0/1)
            int data2;
            double bpm;
            MetaType metaType;
            // The text of a meta event or the ID of the note of a note-on event
            std::string text;
        };

        Event &AddEvent(int track, EventType type, int tick);
        static bool IsMeta(const Event &event);

        int m_ppq;
        std::vector<std::vector<Event>> m_tracks;
    };

    std::optional<smf::MidiFile> m_ownedMidiFile;
    smf::MidiFile &m_midiFile;
    std::optional<JsonEvents> m_json;
};

} // namespace vrv

#endif // __VRV_MIDIBUILDER_H__
