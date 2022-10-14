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
// LayersInTimeSpanFunctor
//----------------------------------------------------------------------------

/**
 * This class collects all layers which appear in the given time / duration
 */
class LayersInTimeSpanFunctor : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    LayersInTimeSpanFunctor(const MeterSig *meterSig, const Mensur *mensur);
    virtual ~LayersInTimeSpanFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() override { return false; }

    /*
     * Set the time and duration of the event
     */
    void SetEvent(double time, double duration);

    /*
     * Retrieve the search result
     */
    std::set<int> GetLayers() const { return m_layers; }

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

//----------------------------------------------------------------------------
// LayerElementsInTimeSpanFunctor
//----------------------------------------------------------------------------

/**
 * This class collects all layer elements which appear in the given time / duration
 */
class LayerElementsInTimeSpanFunctor : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    LayerElementsInTimeSpanFunctor(const MeterSig *meterSig, const Mensur *mensur, const Layer *layer);
    virtual ~LayerElementsInTimeSpanFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() override { return false; }

    /*
     * Set the time and duration of the event
     */
    void SetEvent(double time, double duration);

    /*
     * Consider all layers except the current one
     */
    void ConsiderAllLayersButCurrent() { m_allLayersButCurrent = true; }

    /*
     * Retrieve the search result
     */
    ListOfConstObjects GetElements() const { return m_elements; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLayerElement(const LayerElement *layerElement) override;
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
    // The list of layer elements found
    ListOfConstObjects m_elements;
    // The current meter signature
    const MeterSig *m_meterSig;
    // The current mensur
    const Mensur *m_mensur;
    // The layer to consider
    const Layer *m_layer;
    // ... or to ignore
    bool m_allLayersButCurrent;
};

} // namespace vrv

#endif // __VRV_FINDLAYERELEMENTSFUNCTOR_H__
