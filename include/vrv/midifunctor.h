/////////////////////////////////////////////////////////////////////////////
// Name:        midifunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MIDIFUNCTOR_H__
#define __VRV_MIDIFUNCTOR_H__

#include "functor.h"

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

} // namespace vrv

#endif // __VRV_MIDIFUNCTOR_H__
