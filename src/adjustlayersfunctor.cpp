/////////////////////////////////////////////////////////////////////////////
// Name:        adjustlayersfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustlayersfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustLayersFunctor
//----------------------------------------------------------------------------

AdjustLayersFunctor::AdjustLayersFunctor(Doc *doc, const std::vector<int> &staffNs) : DocFunctor(doc)
{
    m_staffNs = staffNs;
    m_currentLayerN = VRV_UNSET;
    m_unison = false;
    m_ignoreDots = true;
    m_stemSameas = false;
    m_accumulatedShift = 0;
}

FunctorCode AdjustLayersFunctor::VisitAlignmentReference(AlignmentReference *alignmentReference)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustLayersFunctor::VisitAlignmentReferenceEnd(AlignmentReference *alignmentReference)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustLayersFunctor::VisitLayerElement(LayerElement *layerElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustLayersFunctor::VisitMeasure(Measure *measure)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustLayersFunctor::VisitScore(Score *score)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
