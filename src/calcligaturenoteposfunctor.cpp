/////////////////////////////////////////////////////////////////////////////
// Name:        calcligaturenoteposfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "calcligaturenoteposfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// CalcLigatureNotePosFunctor
//----------------------------------------------------------------------------

CalcLigatureNotePosFunctor::CalcLigatureNotePosFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode CalcLigatureNotePosFunctor::VisitLigature(Ligature *ligature)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
