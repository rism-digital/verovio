/////////////////////////////////////////////////////////////////////////////
// Name:        adjustsylspacingfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustsylspacingfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustSylSpacingFunctor
//----------------------------------------------------------------------------

AdjustSylSpacingFunctor::AdjustSylSpacingFunctor(Doc *doc) : DocFunctor(doc)
{
    m_previousVerse = NULL;
    m_lastSyl = NULL;
    m_previousMeasure = NULL;
    m_currentLabelAbbr = NULL;
    m_freeSpace = 0;
    m_staffSize = 100;
}

FunctorCode AdjustSylSpacingFunctor::VisitMeasureEnd(Measure *measure)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustSylSpacingFunctor::VisitStaff(Staff *staff)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustSylSpacingFunctor::VisitSystem(System *system)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustSylSpacingFunctor::VisitSystemEnd(System *system)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustSylSpacingFunctor::VisitVerse(Verse *verse)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
