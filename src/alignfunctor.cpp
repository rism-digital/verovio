/////////////////////////////////////////////////////////////////////////////
// Name:        alignfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "alignfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AlignHorizontallyFunctor
//----------------------------------------------------------------------------

AlignHorizontallyFunctor::AlignHorizontallyFunctor(Doc *doc) : DocFunctor(doc)
{
    m_measureAligner = NULL;
    m_time = 0.0;
    m_currentMensur = NULL;
    m_currentMeterSig = NULL;
    m_notationType = NOTATIONTYPE_cmn;
    m_scoreDefRole = SCOREDEF_NONE;
    m_isFirstMeasure = false;
    m_hasMultipleLayer = false;
}

FunctorCode AlignHorizontallyFunctor::VisitLayer(Layer *layer)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AlignHorizontallyFunctor::VisitLayerEnd(Layer *layer)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AlignHorizontallyFunctor::VisitLayerElement(LayerElement *layerElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AlignHorizontallyFunctor::VisitMeasure(Measure *measure)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AlignHorizontallyFunctor::VisitMeasureEnd(Measure *measure)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AlignHorizontallyFunctor::VisitMeterSigGrp(MeterSigGrp *meterSigGrp)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AlignHorizontallyFunctor::VisitStaff(Staff *staff)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AlignHorizontallyFunctor::VisitSystem(System *system)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
