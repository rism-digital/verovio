/////////////////////////////////////////////////////////////////////////////
// Name:        castofffunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "castofffunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// CastOffSystemsFunctor
//----------------------------------------------------------------------------

CastOffSystemsFunctor::CastOffSystemsFunctor(Page *page, Doc *doc, bool smart) : DocFunctor(doc)
{
    m_page = page;
    m_contentSystem = NULL;
    m_currentSystem = NULL;
    m_leftoverSystem = NULL;
    m_shift = 0;
    m_systemWidth = 0;
    m_currentScoreDefWidth = 0;
    m_smart = smart;
}

FunctorCode CastOffSystemsFunctor::VisitEditorialElement(EditorialElement *editorialElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CastOffSystemsFunctor::VisitEnding(Ending *ending)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CastOffSystemsFunctor::VisitMeasure(Measure *measure)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CastOffSystemsFunctor::VisitPageElement(PageElement *pageElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CastOffSystemsFunctor::VisitPageMilestone(PageMilestoneEnd *pageMilestoneEnd)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CastOffSystemsFunctor::VisitSb(Sb *sb)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CastOffSystemsFunctor::VisitScoreDef(ScoreDef *scoreDef)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CastOffSystemsFunctor::VisitSystem(System *system)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CastOffSystemsFunctor::VisitSystemEnd(System *system)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CastOffSystemsFunctor::VisitSystemElement(SystemElement *systemElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CastOffSystemsFunctor::VisitSystemMilestone(SystemMilestoneEnd *systemMilestoneEnd)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
