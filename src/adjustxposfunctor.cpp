/////////////////////////////////////////////////////////////////////////////
// Name:        adjustxposfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustxposfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustXPosFunctor
//----------------------------------------------------------------------------

AdjustXPosFunctor::AdjustXPosFunctor(Doc *doc, const std::vector<int> &staffNs) : DocFunctor(doc)
{
    m_minPos = 0;
    m_upcomingMinPos = VRV_UNSET;
    m_cumulatedXShift = 0;
    m_staffN = 0;
    m_staffNs = staffNs;
    m_staffSize = 100;
    m_rightBarLinesOnly = false;
}

FunctorCode AdjustXPosFunctor::VisitAlignment(Alignment *alignment)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustXPosFunctor::VisitAlignmentEnd(Alignment *alignment)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustXPosFunctor::VisitLayerElement(LayerElement *layerElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustXPosFunctor::VisitMeasure(Measure *measure)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustXPosFunctor::VisitScore(Score *score)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
