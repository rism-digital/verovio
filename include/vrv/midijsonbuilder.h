/////////////////////////////////////////////////////////////////////////////
// Name:        midijsonbuilder.h
// Author:      Daniel Kasper
// Created:     2026
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MIDIJSONBUILDER_H__
#define __VRV_MIDIJSONBUILDER_H__

#include <string>
#include <vector>

namespace vrv {

//----------------------------------------------------------------------------
// MIDIJsonBuilder
//----------------------------------------------------------------------------

/**
 * This class collects the events of a MIDI export and builds the MIDI JSON from them.
 * Doc::ExportMIDI() and GenerateMIDIFunctor add every event they write to the MIDI file here too, with the same
 * track, tick, channel and values, and the ID of the note for note-on events. The events of a track are kept in the
 * order of the MIDI file, so that an event can be found by its index.
 * Finalize() builds the JSON once all events are collected. The structure is the one of \@tonejs/midi (Midi.toJSON())
 * with the ID added to each note.
 */
class MIDIJsonBuilder {
public:
    enum class MetaType { Text, TrackName, Lyrics, Marker, CuePoint };

    /**
     * @name Constructors, destructors
     */
    ///@{
    MIDIJsonBuilder(int ppq);
    virtual ~MIDIJsonBuilder() = default;
    ///@}

    /**
     * @name Add events, with the values written to the MIDI file
     */
    ///@{
    void AddNoteOn(int track, int tick, int channel, int key, int velocity, const std::string &id);
    void AddNoteOff(int track, int tick, int channel, int key);
    void AddController(int track, int tick, int channel, int number, int value);
    void AddProgramChange(int track, int tick, int channel, int program);
    void AddTempo(int track, int tick, double bpm);
    void AddTimeSignature(int track, int tick, int numerator, int denominator);
    void AddKeySignature(int track, int tick, int fifths, bool minor);
    void AddMeta(int track, int tick, MetaType type, const std::string &text);
    // An event without a JSON representation (e.g., tuning), kept for the event indices
    void AddOther(int track, int tick);
    ///@}

    /**
     * Copy the note-on or note-off event at the index in the track to another tick.
     * A copied note-on is given the ID passed.
     */
    void CopyNoteEvent(int track, int index, int tick, const std::string &id);

    /**
     * The number of events in a track, as in the MIDI file
     */
    int GetEventCount(int track) const;

    /**
     * Build the JSON. trackCount is the number of tracks of the MIDI file.
     */
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

private:
    // The ticks per quarter note
    int m_ppq;
    // The events of each MIDI track, in the order of the MIDI file
    std::vector<std::vector<Event>> m_tracks;
};

} // namespace vrv

#endif // __VRV_MIDIJSONBUILDER_H__
