/////////////////////////////////////////////////////////////////////////////
// Name:        calcdots.cpp
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "calcdots.h"

//----------------------------------------------------------------------------

#include "chord.h"
#include "elementpart.h"
#include "note.h"
#include "rest.h"

namespace vrv {

//----------------------------------------------------------------------------
// CalcDots
//----------------------------------------------------------------------------

CalcDots::CalcDots(Doc *doc) : DocFunctor(doc)
{
    m_chordDots = NULL;
    m_chordDrawingX = 0;
    m_chordStemDir = STEMDIRECTION_NONE;
}

FunctorCode CalcDots::VisitChord(Chord *chord)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CalcDots::VisitNote(Note *note)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CalcDots::VisitRest(Rest *rest)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
