/////////////////////////////////////////////////////////////////////////////
// Name:        midifunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MIDIFUNCTOR_H__
#define __VRV_MIDIFUNCTOR_H__

#include "functor.h"

//----------------------------------------------------------------------------

#include "alignfunctor.h"

namespace smf {
class MidiFile;
}

namespace vrv {

class FeatureExtractor;
class Timemap;

//----------------------------------------------------------------------------
// InitOnsetOffsetFunctor
//----------------------------------------------------------------------------

/**
 * This class prepares Note onsets.
 */
class InitOnsetOffsetFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    InitOnsetOffsetFunctor(Doc *doc);
    virtual ~InitOnsetOffsetFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitChordEnd(Chord *chord) override;
    FunctorCode VisitLayer(Layer *layer) override;
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitStaff(Staff *staff) override;
    FunctorCode VisitTabGrpEnd(TabGrp *tabGrp) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The current score time in the measure (incremented by each element)
    Fraction m_currentScoreTime;
    // The current real time in seconds in the measure (incremented by each element)
    double m_currentRealTimeSeconds;
    // The current time alignment parameters
    AlignMeterParams m_meterParams;
    // The current notation type
    data_NOTATIONTYPE m_notationType;
    // The current tempo
    double m_currentTempo;
};

//----------------------------------------------------------------------------
// InitMaxMeasureDurationFunctor
//----------------------------------------------------------------------------

/**
 * This class calculates the maximum duration of each measure.
 */
class InitMaxMeasureDurationFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    InitMaxMeasureDurationFunctor();
    virtual ~InitMaxMeasureDurationFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Set the tempo
     */
    ///@{
    void SetCurrentTempo(double tempo) { m_currentTempo = tempo; }
    void SetTempoAdjustment(double adjustment) { m_tempoAdjustment = adjustment; }
    ///@}

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitMeasureEnd(Measure *measure) override;
    FunctorCode VisitScoreDef(ScoreDef *scoreDef) override;
    FunctorCode VisitTempo(Tempo *tempo) override;
    ///@}

protected:
    //
private:
    // Returns the current tempo with adjustment
    double GetAdjustedTempo() const { return m_currentTempo * m_tempoAdjustment; }

public:
    //
private:
    // The current score time
    Fraction m_currentScoreTime;
    // The current time in seconds
    double m_currentRealTimeSeconds;
    // The current tempo
    double m_currentTempo;
    // The tempo adjustment
    double m_tempoAdjustment;
    // The factor for multibar rests
    int m_multiRestFactor;
};

//----------------------------------------------------------------------------
// InitTimemapTiesFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts note timings based on ties.
 */
class InitTimemapTiesFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    InitTimemapTiesFunctor();
    virtual ~InitTimemapTiesFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitTie(Tie *tie) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    //
};
//----------------------------------------------------------------------------
// InitTimemapAdjustNotesFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts note duration for grace notes and arpeggios.
 */
class InitTimemapAdjustNotesFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    InitTimemapAdjustNotesFunctor();
    virtual ~InitTimemapAdjustNotesFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Setter for various properties
     */
    ///@{
    void SetNoCue(bool noCue) { m_noCue = noCue; }
    ///@}

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitArpeg(Arpeg *arpeg) override;
    FunctorCode VisitChord(Chord *chord) override;
    FunctorCode VisitGraceGrpEnd(GraceGrp *graceGrp) override;
    FunctorCode VisitLayerEnd(Layer *layer) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitNote(Note *note) override;
    ///@}

protected:
    //
private:
    /**
     * Helper struct to store grace note/chord sequences
     */
    struct Grace {
        std::list<Note *> notes;
        data_DURATION duration;
        data_PERCENT time;
    };

    /**
     * Set the grace note onset and offset times for the reference note
     */
    void SetGraceNotesFor(Note *refNote);

    /**
     * Set the start (and stop) time for a note (score and real times)
     */
    void SetNoteStartStop(Note *note, const Fraction &startTime, const Fraction &stopTime);
    void SetNoteStart(Note *note, const Fraction &startTime);

public:
    //
private:
    // Indicates whether cue notes should be included
    bool m_noCue;
    // Grace note/chord sequence
    std::list<Grace> m_graces;
    // Indicates whether the last grace note/chord was accented
    bool m_accentedGraceNote;
    // The current tempo
    double m_currentTempo;
    // The last (non grace) note
    Note *m_lastNote;
};

//----------------------------------------------------------------------------
// InitMIDIFunctor
//----------------------------------------------------------------------------

/**
 * Helper struct to store octave data
 */
struct OctaveInfo {
    const Octave *octave;
    int staffN;
    int layerN;
    int octaveShift;
    bool isActive;
};

/**
 * This class initializes the MIDI export.
 * Captures information (i.e. from control elements) for MIDI interpretation
 * which is required beforehand in GenerateMIDI.
 */
class InitMIDIFunctor : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    InitMIDIFunctor();
    virtual ~InitMIDIFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Setter and getter for properties
     */
    ///@{
    void SetCurrentTempo(double tempo) { m_currentTempo = tempo; }
    const std::list<OctaveInfo> &GetOctaves() const { return m_octaves; }
    ///@}

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitMeasure(const Measure *measure) override;
    FunctorCode VisitOctave(const Octave *octave) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The current tempo
    double m_currentTempo;
    // Octave info which is collected
    std::list<OctaveInfo> m_octaves;
};

//----------------------------------------------------------------------------
// GenerateMIDIFunctor
//----------------------------------------------------------------------------

/**
 * Helper struct to store note sequences which replace notes in MIDI output due to expanded ornaments and tremolandi
 */
struct MIDINote {
    int pitch;
    double duration;
};

using MIDINoteSequence = std::list<MIDINote>;

/**
 * Helper struct for held notes in tablature
 */
struct MIDIHeldNote {
    int m_pitch = 0;
    double m_stopTime = 0;
};

/**
 * This class performs the export to a MidiFile.
 */
class GenerateMIDIFunctor : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    GenerateMIDIFunctor(smf::MidiFile *midiFile);
    virtual ~GenerateMIDIFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Getter for properties
     */
    ///@{
    std::set<int> GetTempoEventTicks() const { return m_tempoEventTicks; }
    ///@}

    /*
     * Setter for various properties
     */
    ///@{
    void SetChannel(int channel) { m_midiChannel = channel; }
    void SetControlEvents(bool controlEvents) { m_controlEvents = controlEvents; }
    void SetNoCue(bool noCue) { m_noCue = noCue; }
    void SetCurrentTempo(double tempo) { m_currentTempo = tempo; }
    void SetDeferredNotes(const std::map<const Note *, double> &deferredNotes) { m_deferredNotes = deferredNotes; }
    void SetLayerN(int layerN) { m_layerN = layerN; }
    void SetOctaves(const std::list<OctaveInfo> &octaves) { m_octaves = octaves; }
    void SetStaffN(int staffN) { m_staffN = staffN; }
    void SetTempoEventTicks(const std::set<int> &ticks) { m_tempoEventTicks = ticks; }
    void SetTrack(int track) { m_midiTrack = track; }
    void SetTransSemi(int transSemi) { m_transSemi = transSemi; }
    ///@}

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitBeatRpt(const BeatRpt *beatRpt) override;
    FunctorCode VisitBTrem(const BTrem *bTrem) override;
    FunctorCode VisitChord(const Chord *chord) override;
    FunctorCode VisitFTrem(const FTrem *fTrem) override;
    FunctorCode VisitHalfmRpt(const HalfmRpt *halfmRpt) override;
    FunctorCode VisitLayer(const Layer *layer) override;
    FunctorCode VisitLayerEnd(const Layer *layer) override;
    FunctorCode VisitLayerElement(const LayerElement *layerElement) override;
    FunctorCode VisitMeasure(const Measure *measure) override;
    FunctorCode VisitMRpt(const MRpt *mRpt) override;
    FunctorCode VisitNote(const Note *note) override;
    FunctorCode VisitPedal(const Pedal *pedal) override;
    FunctorCode VisitScoreDef(const ScoreDef *scoreDef) override;
    FunctorCode VisitStaff(const Staff *staff) override;
    FunctorCode VisitStaffDef(const StaffDef *staffDef) override;
    FunctorCode VisitSyl(const Syl *syl) override;
    FunctorCode VisitVerse(const Verse *verse) override;
    ///@}

protected:
    //
private:
    /**
     * Register deferred notes for MIDI
     */
    void DeferMIDINote(const Note *refNote, double shift, bool includeChordSiblings);

    /**
     * Change the octave shift at the begin/end of octaves
     */
    void HandleOctave(const LayerElement *layerElement);

    /**
     * Convenience helper
     */
    int GetMIDIPitch(const Note *note) { return note->GetMIDIPitch(m_transSemi, m_octaveShift); }

public:
    //
private:
    // The MidiFile we are writing to
    smf::MidiFile *m_midiFile;
    // The MIDI track number
    int m_midiTrack;
    // The MIDI channel number
    int m_midiChannel;
    // The score time from the start of the music to the start of the current measure
    double m_totalTime;
    // The current staff number
    int m_staffN;
    // The current layer number
    int m_layerN;
    // The semi tone transposition for the current track
    int m_transSemi;
    // The octave shift for the current track
    int m_octaveShift;
    // The current tempo
    double m_currentTempo;
    // Tempo events are always added on track 0
    // This set contains the ticks of all added tempo events to avoid multiple events at the same time
    std::set<int> m_tempoEventTicks;
    // The last (non grace) note that was performed
    const Note *m_lastNote;
    // Expanded notes due to ornaments and tremolandi
    std::map<const Note *, MIDINoteSequence> m_expandedNotes;
    // Deferred notes which start slightly later
    std::map<const Note *, double> m_deferredNotes;
    // Octave info which is used to determine the octave shift
    std::list<OctaveInfo> m_octaves;
    // Indicates whether cue notes should be included
    bool m_noCue;
    // Tablature held notes indexed by (course - 1)
    std::vector<MIDIHeldNote> m_heldNotes;
    // A flag indicating we want to process control events
    bool m_controlEvents;
};

//----------------------------------------------------------------------------
// GenerateTimemapFunctor
//----------------------------------------------------------------------------

/**
 * This class exports the object to a JSON timemap file.
 */
class GenerateTimemapFunctor : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    GenerateTimemapFunctor(Timemap *timemap);
    virtual ~GenerateTimemapFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Set the cue exclusion flag
     */
    void SetNoCue(bool noCue) { m_noCue = noCue; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLayerElement(const LayerElement *layerElement) override;
    FunctorCode VisitMeasure(const Measure *measure) override;
    FunctorCode VisitMRest(const MRest *mRest) override;
    FunctorCode VisitMultiRest(const MultiRest *multiRest) override;
    FunctorCode VisitNote(const Note *note) override;
    FunctorCode VisitRest(const Rest *rest) override;
    ///@}

protected:
    //
private:
    /**
     * Add an entry to the timemap.
     */
    void AddTimemapEntry(const Object *object);

public:
    //
private:
    // The score time from the start of the piece to the previous barline in quarter notes
    Fraction m_currentScoreTime;
    // Real time from the start of the piece to the previous barline in ms
    double m_currentRealTimeMilliseconds;
    // The current tempo
    double m_currentTempo;
    // Indicates whether cue notes should be included
    bool m_noCue;
    // The timemap
    Timemap *m_timemap;
};

//----------------------------------------------------------------------------
// GenerateFeaturesFunctor
//----------------------------------------------------------------------------

/**
 * This class exports the object to a JSON feature file.
 */
class GenerateFeaturesFunctor : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    GenerateFeaturesFunctor(FeatureExtractor *extractor);
    virtual ~GenerateFeaturesFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitObject(const Object *object) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The FeatureExtractor to which extraction is delegated
    FeatureExtractor *m_extractor;
};

} // namespace vrv

#endif // __VRV_MIDIFUNCTOR_H__
