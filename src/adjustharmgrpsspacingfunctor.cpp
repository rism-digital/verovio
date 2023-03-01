/////////////////////////////////////////////////////////////////////////////
// Name:        adjustharmgrpsspacingfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustharmgrpsspacingfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustHarmGrpsSpacingFunctor
//----------------------------------------------------------------------------

AdjustHarmGrpsSpacingFunctor::AdjustHarmGrpsSpacingFunctor(Doc *doc) : DocFunctor(doc)
{
    m_currentGrp = 0;
    m_previousHarmPositioner = NULL;
    m_previousHarmStart = NULL;
    m_previousMeasure = NULL;
    m_currentSystem = NULL;
}

FunctorCode AdjustHarmGrpsSpacingFunctor::VisitHarm(Harm *harm)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustHarmGrpsSpacingFunctor::VisitMeasureEnd(Measure *measure)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustHarmGrpsSpacingFunctor::VisitSystem(System *system)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustHarmGrpsSpacingFunctor::VisitSystemEnd(System *system)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
