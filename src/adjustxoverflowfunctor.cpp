/////////////////////////////////////////////////////////////////////////////
// Name:        adjustxoverflowfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustxoverflowfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustXOverflowFunctor
//----------------------------------------------------------------------------

AdjustXOverflowFunctor::AdjustXOverflowFunctor(int margin)
{
    m_currentSystem = NULL;
    m_lastMeasure = NULL;
    m_currentWidest = NULL;
    m_margin = margin;
}

FunctorCode AdjustXOverflowFunctor::VisitControlElement(ControlElement *controlElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustXOverflowFunctor::VisitMeasure(Measure *measure)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustXOverflowFunctor::VisitSystem(System *system)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustXOverflowFunctor::VisitSystemEnd(System *system)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
