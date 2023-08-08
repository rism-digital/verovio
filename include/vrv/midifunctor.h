/////////////////////////////////////////////////////////////////////////////
// Name:        midifunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MIDIFUNCTOR_H__
#define __VRV_MIDIFUNCTOR_H__

#include "functor.h"

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
class InitOnsetOffsetFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    InitOnsetOffsetFunctor();
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
    double m_currentScoreTime;
    // The current real time in seconds in the measure (incremented by each element)
    double m_currentRealTimeSeconds;
    // The current Mensur
    Mensur *m_currentMensur;
    // The current MeterSig
    MeterSig *m_currentMeterSig;
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
    double m_currentScoreTime;
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
// InitMIDIFunctor
//----------------------------------------------------------------------------

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
    const std::map<const Note *, double> &GetDeferredNotes() const { return m_deferredNotes; }
    ///@}

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitArpeg(const Arpeg *arpeg) override;
    FunctorCode VisitMeasure(const Measure *measure) override;
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
    // Deferred notes which start slightly later
    std::map<const Note *, double> m_deferredNotes;
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
 * Helper struct to store chord sequences in MIDI output due to grace notes
 */
struct MIDIChord {
    std::set<int> pitches;
    double duration;
};

using MIDIChordSequence = std::list<MIDIChord>;

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
     * Setter for various properties
     */
    ///@{
    void SetChannel(int channel) { m_midiChannel = channel; }
    void SetCueExclusion(bool cueExclusion) { m_cueExclusion = cueExclusion; }
    void SetCurrentTempo(double tempo) { m_currentTempo = tempo; }
    void SetDeferredNotes(const std::map<const Note *, double> &deferredNotes) { m_deferredNotes = deferredNotes; }
    void SetStaffN(int staffN) { m_staffN = staffN; }
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
    FunctorCode VisitGraceGrpEnd(const GraceGrp *graceGrp) override;
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
     * Creates the MIDI output of the grace note sequence
     */
    void GenerateGraceNoteMIDI(const Note *refNote, double startTime, int tpq, int channel, int velocity);

public:
    //
private:
    // The MidiFile we are writing to
    smf::MidiFile *m_midiFile;
    // The midi track number
    int m_midiTrack;
    // The midi channel number
    int m_midiChannel;
    // The score time from the start of the music to the start of the current measure
    double m_totalTime;
    // The current staff number
    int m_staffN;
    // The semi tone transposition for the current track
    int m_transSemi;
    // The current tempo
    double m_currentTempo;
    // The last (non grace) note that was performed
    const Note *m_lastNote;
    // Expanded notes due to ornaments and tremolandi
    std::map<const Note *, MIDINoteSequence> m_expandedNotes;
    // Deferred notes which start slightly later
    std::map<const Note *, double> m_deferredNotes;
    // Grace note sequence
    MIDIChordSequence m_graceNotes;
    // Indicates whether the last grace note/chord was accented
    bool m_accentedGraceNote;
    // Indicates whether cue notes should be included
    bool m_cueExclusion;
    // Tablature held notes indexed by (course - 1)
    std::vector<MIDIHeldNote> m_heldNotes;
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
    void SetCueExclusion(bool cueExclusion) { m_cueExclusion = cueExclusion; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLayerElement(const LayerElement *layerElement) override;
    FunctorCode VisitMeasure(const Measure *measure) override;
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
    double m_scoreTimeOffset;
    // Real time from the start of the piece to the previous barline in ms
    double m_realTimeOffsetMilliseconds;
    // The current tempo
    double m_currentTempo;
    // Indicates whether cue notes should be included
    bool m_cueExclusion;
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
