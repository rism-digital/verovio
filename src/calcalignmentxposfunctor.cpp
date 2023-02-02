/////////////////////////////////////////////////////////////////////////////
// Name:        calcalignmentxposfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "calcalignmentxposfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// CalcAlignmentXPosFunctor
//----------------------------------------------------------------------------

CalcAlignmentXPosFunctor::CalcAlignmentXPosFunctor(Doc *doc) : DocFunctor(doc)
{
    m_previousTime = 0.0;
    m_previousXRel = 0;
    m_longestActualDur = 0;
    m_estimatedJustificationRatio = 1.0;
    m_lastNonTimestamp = NULL;
    m_measureAligner = NULL;
}

FunctorCode CalcAlignmentXPosFunctor::VisitAlignment(Alignment *alignment)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CalcAlignmentXPosFunctor::VisitMeasure(Measure *measure)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CalcAlignmentXPosFunctor::VisitMeasureAligner(MeasureAligner *measureAligner)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CalcAlignmentXPosFunctor::VisitSystem(System *system)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
