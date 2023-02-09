/////////////////////////////////////////////////////////////////////////////
// Name:        calcchordnoteheadsfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "calcchordnoteheadsfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// CalcChordNoteHeadsFunctor
//----------------------------------------------------------------------------

CalcChordNoteHeadsFunctor::CalcChordNoteHeadsFunctor(Doc *doc) : DocFunctor(doc)
{
    m_diameter = 0;
}

FunctorCode CalcChordNoteHeadsFunctor::VisitChord(Chord *chord)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CalcChordNoteHeadsFunctor::VisitNote(Note *note)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
