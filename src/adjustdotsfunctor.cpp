/////////////////////////////////////////////////////////////////////////////
// Name:        adjustdotsfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustdotsfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustDotsFunctor
//----------------------------------------------------------------------------

AdjustDotsFunctor::AdjustDotsFunctor(Doc *doc, const std::vector<int> &staffNs) : DocFunctor(doc)
{
    m_staffNs = staffNs;
}

FunctorCode AdjustDotsFunctor::VisitAlignmentEnd(Alignment *alignment)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustDotsFunctor::VisitLayerElement(LayerElement *layerElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustDotsFunctor::VisitMeasure(Measure *measure)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustDotsFunctor::VisitScore(Score *score)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
