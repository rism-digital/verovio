/////////////////////////////////////////////////////////////////////////////
// Name:        adjustgracexposfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustgracexposfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustGraceXPosFunctor
//----------------------------------------------------------------------------

AdjustGraceXPosFunctor::AdjustGraceXPosFunctor(Doc *doc, const std::vector<int> &staffNs) : DocFunctor(doc)
{
    m_staffNs = staffNs;
    m_graceMaxPos = 0;
    m_graceUpcomingMaxPos = -VRV_UNSET;
    m_graceCumulatedXShift = 0;
    m_isGraceAlignment = false;
    m_rightDefaultAlignment = NULL;
}

FunctorCode AdjustGraceXPosFunctor::VisitAlignment(Alignment *alignment)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustGraceXPosFunctor::VisitAlignmentEnd(Alignment *alignment)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustGraceXPosFunctor::VisitAlignmentReference(AlignmentReference *alignmentReference)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustGraceXPosFunctor::VisitLayerElement(LayerElement *layerElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustGraceXPosFunctor::VisitMeasure(Measure *measure)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustGraceXPosFunctor::VisitScore(Score *score)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
