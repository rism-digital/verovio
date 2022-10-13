/////////////////////////////////////////////////////////////////////////////
// Name:        findlayerelementsfunctor.cpp
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "findlayerelementsfunctor.h"

//----------------------------------------------------------------------------

#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// LayerCountInTimeSpanFunctor
//----------------------------------------------------------------------------

LayerCountInTimeSpanFunctor::LayerCountInTimeSpanFunctor(const MeterSig *meterSig, const Mensur *mensur)
    : ConstFunctor()
{
    m_time = 0.0;
    m_duration = 0.0;
    m_meterSig = meterSig;
    m_mensur = mensur;
}

FunctorCode LayerCountInTimeSpanFunctor::VisitLayerElement(const LayerElement *layerElement)
{
    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    // For mRest we do not look at the time span
    if (layerElement->Is(MREST)) {
        // Add the layerN to the list of layers occuring in this time frame
        m_layers.insert(layerElement->GetAlignmentLayerN());

        return FUNCTOR_SIBLINGS;
    }

    if (!layerElement->GetDurationInterface() || layerElement->Is(MSPACE) || layerElement->Is(SPACE)
        || layerElement->HasSameasLink())
        return FUNCTOR_CONTINUE;
    if (layerElement->Is(NOTE) && layerElement->GetParent()->Is(CHORD)) return FUNCTOR_CONTINUE;

    double duration = layerElement->GetAlignmentDuration(m_mensur, m_meterSig);
    double time = layerElement->GetAlignment()->GetTime();

    // The event is starting after the end of the element
    if (time + duration <= m_time) {
        return FUNCTOR_CONTINUE;
    }
    // The element is starting after the event end - we can stop here
    else if (time >= m_time + m_duration) {
        return FUNCTOR_STOP;
    }

    // Add the layerN to the list of layers occuring in this time frame
    m_layers.insert(layerElement->GetAlignmentLayerN());

    // Not need to recurse for chords? Not quite sure about it.
    return (layerElement->Is(CHORD)) ? FUNCTOR_SIBLINGS : FUNCTOR_CONTINUE;
}

FunctorCode LayerCountInTimeSpanFunctor::VisitMensur(const Mensur *mensur)
{
    m_mensur = mensur;

    return FUNCTOR_CONTINUE;
}

FunctorCode LayerCountInTimeSpanFunctor::VisitMeterSig(const MeterSig *meterSig)
{
    m_meterSig = meterSig;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
