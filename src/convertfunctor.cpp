/////////////////////////////////////////////////////////////////////////////
// Name:        convertfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "convertfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// ConvertToPageBasedFunctor
//----------------------------------------------------------------------------

ConvertToPageBasedFunctor::ConvertToPageBasedFunctor(Page *page)
{
    m_currentSystem = NULL;
    m_page = page;
}

FunctorCode ConvertToPageBasedFunctor::VisitEditorialElement(EditorialElement *editorialElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToPageBasedFunctor::VisitEditorialElementEnd(EditorialElement *editorialElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToPageBasedFunctor::VisitEnding(Ending *ending)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToPageBasedFunctor::VisitEndingEnd(Ending *ending)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToPageBasedFunctor::VisitMeasure(Measure *measure)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToPageBasedFunctor::VisitMdiv(Mdiv *mdiv)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToPageBasedFunctor::VisitMdivEnd(Mdiv *mdiv)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToPageBasedFunctor::VisitScore(Score *score)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToPageBasedFunctor::VisitScoreEnd(Score *score)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToPageBasedFunctor::VisitScoreDef(ScoreDef *scoreDef)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToPageBasedFunctor::VisitSection(Section *section)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToPageBasedFunctor::VisitSectionEnd(Section *section)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToPageBasedFunctor::VisitSystemElement(SystemElement *systemElement)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
