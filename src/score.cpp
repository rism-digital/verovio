/////////////////////////////////////////////////////////////////////////////
// Name:        score.cpp
// Author:      Laurent Pugin
// Created:     2026
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "score.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "functor.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Score
//----------------------------------------------------------------------------

static const ClassRegistrar<Score> s_factory("score", SCORE);

Score::Score() : Score(true) {}

Score::Score(bool createScoreDef) : ScoreOrPartElement(SCORE, createScoreDef)
{
    this->Reset();
}

Score::~Score() {}

void Score::Reset()
{
    ScoreOrPartElement::Reset();
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode Score::Accept(Functor &functor)
{
    return functor.VisitScore(this);
}

FunctorCode Score::Accept(ConstFunctor &functor) const
{
    return functor.VisitScore(this);
}

FunctorCode Score::AcceptEnd(Functor &functor)
{
    return functor.VisitScoreEnd(this);
}

FunctorCode Score::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitScoreEnd(this);
}

} // namespace vrv
