/////////////////////////////////////////////////////////////////////////////
// Name:        adjustbeamsfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustbeamsfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustBeamsFunctor
//----------------------------------------------------------------------------

AdjustBeamsFunctor::AdjustBeamsFunctor(Doc *doc) : DocFunctor(doc)
{
    m_beam = NULL;
    m_y1 = 0;
    m_y2 = 0;
    m_x1 = 0;
    m_x2 = 0;
    m_beamSlope = 0.0;
    m_directionBias = 0;
    m_overlapMargin = 0;
    m_isOtherLayer = false;
}

FunctorCode AdjustBeamsFunctor::VisitBeam(Beam *beam)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustBeamsFunctor::VisitBeamEnd(Beam *beam)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustBeamsFunctor::VisitClef(Clef *clef)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustBeamsFunctor::VisitFTrem(FTrem *fTrem)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustBeamsFunctor::VisitFTremEnd(FTrem *fTrem)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustBeamsFunctor::VisitLayerElement(LayerElement *layerElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustBeamsFunctor::VisitRest(Rest *rest)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
