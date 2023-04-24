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

} // namespace vrv

#endif // __VRV_MIDIFUNCTOR_H__
