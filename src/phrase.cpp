/////////////////////////////////////////////////////////////////////////////
// Name:        phrase.cpp
// Author:      Laurent Pugin
// Created:     15/06/2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "phrase.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "functor.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Phrase
//----------------------------------------------------------------------------

static const ClassRegistrar<Phrase> s_factory("phrase", PHRASE);

Phrase::Phrase() : Slur(PHRASE)
{
    this->Reset();
}

Phrase::~Phrase() {}

void Phrase::Reset()
{
    Slur::Reset();
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

FunctorCode Phrase::Accept(Functor &functor)
{
    return functor.VisitPhrase(this);
}

FunctorCode Phrase::Accept(ConstFunctor &functor) const
{
    return functor.VisitPhrase(this);
}

FunctorCode Phrase::AcceptEnd(Functor &functor)
{
    return functor.VisitPhraseEnd(this);
}

FunctorCode Phrase::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitPhraseEnd(this);
}

} // namespace vrv
