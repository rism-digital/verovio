/////////////////////////////////////////////////////////////////////////////
// Name:        transposefunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "transposefunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// TransposeFunctor
//----------------------------------------------------------------------------

TransposeFunctor::TransposeFunctor(Doc *doc, Transposer *transposer) : DocFunctor(doc)
{
    m_transposer = transposer;
    m_transposeToSoundingPitch = false;
}

FunctorCode TransposeFunctor::VisitHarm(Harm *harm)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode TransposeFunctor::VisitKeySig(KeySig *keySig)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode TransposeFunctor::VisitMdiv(Mdiv *mdiv)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode TransposeFunctor::VisitNote(Note *note)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode TransposeFunctor::VisitPageMilestone(PageMilestoneEnd *pageMilestoneEnd)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode TransposeFunctor::VisitRest(Rest *rest)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode TransposeFunctor::VisitScore(Score *score)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode TransposeFunctor::VisitScoreDef(ScoreDef *scoreDef)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode TransposeFunctor::VisitScoreDefEnd(ScoreDef *scoreDef)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode TransposeFunctor::VisitStaff(Staff *staff)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode TransposeFunctor::VisitStaffDef(StaffDef *staffDef)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode TransposeFunctor::VisitSystem(System *system)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
