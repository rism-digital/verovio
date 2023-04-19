/////////////////////////////////////////////////////////////////////////////
// Name:        savefunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "savefunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// SaveFunctor
//----------------------------------------------------------------------------

SaveFunctor::SaveFunctor(Output *output, bool basic)
{
    m_output = output;
    m_basic = basic;
}

FunctorCode SaveFunctor::VisitEditorialElement(EditorialElement *editorialElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode SaveFunctor::VisitEditorialElementEnd(EditorialElement *editorialElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode SaveFunctor::VisitMdiv(Mdiv *mdiv)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode SaveFunctor::VisitMdivEnd(Mdiv *mdiv)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode SaveFunctor::VisitMeasure(Measure *measure)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode SaveFunctor::VisitMeasureEnd(Measure *measure)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode SaveFunctor::VisitMNum(MNum *mNum)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode SaveFunctor::VisitMNumEnd(MNum *mNum)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode SaveFunctor::VisitObject(Object *object)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode SaveFunctor::VisitObjectEnd(Object *object)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode SaveFunctor::VisitRunningElement(RunningElement *runningElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode SaveFunctor::VisitRunningElementEnd(RunningElement *runningElement)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
