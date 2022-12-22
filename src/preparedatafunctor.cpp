/////////////////////////////////////////////////////////////////////////////
// Name:        preparedatafunctor.cpp
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "preparedatafunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// PrepareDataInitializationFunctor
//----------------------------------------------------------------------------

PrepareDataInitializationFunctor::PrepareDataInitializationFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode PrepareDataInitializationFunctor::VisitChord(Chord *chord)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareDataInitializationFunctor::VisitKeySig(KeySig *keySig)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareDataInitializationFunctor::VisitRunningElement(RunningElement *runningElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode PrepareDataInitializationFunctor::VisitScore(Score *score)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
