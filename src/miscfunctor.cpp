/////////////////////////////////////////////////////////////////////////////
// Name:        miscfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "miscfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// ApplyPPUFactorFunctor
//----------------------------------------------------------------------------

ApplyPPUFactorFunctor::ApplyPPUFactorFunctor()
{
    m_page = NULL;
}

FunctorCode ApplyPPUFactorFunctor::VisitLayerElement(LayerElement *layerElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ApplyPPUFactorFunctor::VisitMeasure(Measure *measure)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ApplyPPUFactorFunctor::VisitPage(Page *page)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ApplyPPUFactorFunctor::VisitStaff(Staff *staff)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ApplyPPUFactorFunctor::VisitSystem(System *system)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
