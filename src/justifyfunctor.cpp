/////////////////////////////////////////////////////////////////////////////
// Name:        justifyfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "justifyfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// JustifyXFunctor
//----------------------------------------------------------------------------

JustifyXFunctor::JustifyXFunctor(Doc *doc) : DocFunctor(doc)
{
    m_measureXRel = 0;
    m_justifiableRatio = 1.0;
    m_leftBarLineX = 0;
    m_rightBarLineX = 0;
    m_systemFullWidth = 0;
    m_applySectionRestartShift = false;
}

FunctorCode JustifyXFunctor::VisitAlignment(Alignment *alignment)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode JustifyXFunctor::VisitMeasure(Measure *measure)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode JustifyXFunctor::VisitMeasureAligner(MeasureAligner *measureAligner)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode JustifyXFunctor::VisitScoreDef(ScoreDef *scoreDef)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode JustifyXFunctor::VisitSection(Section *section)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode JustifyXFunctor::VisitSystem(System *system)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
