/////////////////////////////////////////////////////////////////////////////
// Name:        midifunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MIDIFUNCTOR_H__
#define __VRV_MIDIFUNCTOR_H__

#include "functor.h"
#include "functorparams.h"

namespace smf {
class MidiFile;
}

namespace vrv {

//----------------------------------------------------------------------------
// InitOnsetOffsetFunctor
//----------------------------------------------------------------------------

/**
 * This class prepares Note onsets.
 */
class InitOnsetOffsetFunctor : public MutableFunctor {
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
class InitMaxMeasureDurationFunctor : public MutableFunctor {
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
    //
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
class InitTimemapTiesFunctor : public MutableFunctor {
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
    //
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

} // namespace vrv

#endif // __VRV_MIDIFUNCTOR_H__
