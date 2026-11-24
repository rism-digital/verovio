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
    /** Populate a caller-owned MIDI file and also collect JSON events if requested. */
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
     * @name Add events to the selected output
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
    enum class EventType {
        NoteOn,
        NoteOff,
        Controller,
        ProgramChange,
        Tempo,
        TimeSignature,
        KeySignature,
        Meta,
        Tuning
    };
    enum class MetaType { Text, TrackName, Lyrics, Marker, CuePoint };

    struct Event {
        EventType type;
        int tick;
        int channel = 0;
        // Key, controller, program, numerator or fifths
        int data1 = 0;
        // Velocity, controller value, denominator or minor (0/1)
        int data2 = 0;
        double bpm = 0.0;
        MetaType metaType = MetaType::Text;
        // The text of a meta event or the ID of the note of a note-on event
        std::string text;
    };

    void EnsureTrack(int track);
    Event &AddJsonEvent(int track, EventType type, int tick);
    void AddJsonMeta(int track, int tick, MetaType type, const std::string &text);
    void CopyJsonNotesForBeatRepeat(
        int track, double firstTick, double lastTick, double tickOffset, const std::string &id);
    static bool IsMeta(const Event &event);

    // Use the same resolution for both output formats when we own the timing.
    static constexpr int s_defaultTPQ = 120;
    std::optional<smf::MidiFile> m_ownedMidiFile;
    smf::MidiFile *m_midiFile = NULL;
    std::optional<std::vector<std::vector<Event>>> m_jsonTracks;
};

} // namespace vrv

#endif // __VRV_MIDIBUILDER_H__
