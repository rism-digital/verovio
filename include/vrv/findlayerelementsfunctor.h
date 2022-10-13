/////////////////////////////////////////////////////////////////////////////
// Name:        findlayerelementsfunctor.h
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FINDLAYERELEMENTSFUNCTOR_H__
#define __VRV_FINDLAYERELEMENTSFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// LayerCountInTimeSpanFunctor
//----------------------------------------------------------------------------

/**
 * This class collects all layers which appear in the given time / duration
 */
class LayerCountInTimeSpanFunctor : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    LayerCountInTimeSpanFunctor(const MeterSig *meterSig, const Mensur *mensur);
    virtual ~LayerCountInTimeSpanFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLayerElement(const LayerElement *layerElement) override;
    FunctorCode VisitMensur(const Mensur *mensur) override;
    FunctorCode VisitMeterSig(const MeterSig *meterSig) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The time of the event
    double m_time;
    // The duration of the event
    double m_duration;
    // The layers (layerN) found
    std::set<int> m_layers;
    // The current meter signature
    const MeterSig *m_meterSig;
    // The current mensur
    const Mensur *m_mensur;
};

} // namespace vrv

#endif // __VRV_FINDLAYERELEMENTSFUNCTOR_H__
