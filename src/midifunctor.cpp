/////////////////////////////////////////////////////////////////////////////
// Name:        midifunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "midifunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// InitOnsetOffsetFunctor
//----------------------------------------------------------------------------

InitOnsetOffsetFunctor::InitOnsetOffsetFunctor()
{
    m_currentScoreTime = 0.0;
    m_currentRealTimeSeconds = 0.0;
    m_currentMensur = NULL;
    m_currentMeterSig = NULL;
    m_notationType = NOTATIONTYPE_cmn;
    m_currentTempo = MIDI_TEMPO;
}

FunctorCode InitOnsetOffsetFunctor::VisitChordEnd(Chord *chord)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode InitOnsetOffsetFunctor::VisitLayer(Layer *layer)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode InitOnsetOffsetFunctor::VisitLayerElement(LayerElement *layerElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode InitOnsetOffsetFunctor::VisitMeasure(Measure *measure)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode InitOnsetOffsetFunctor::VisitStaff(Staff *staff)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode InitOnsetOffsetFunctor::VisitTabGrpEnd(TabGrp *tabGrp)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
