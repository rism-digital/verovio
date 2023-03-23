/////////////////////////////////////////////////////////////////////////////
// Name:        calcarticfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "calcarticfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// CalcArticFunctor
//----------------------------------------------------------------------------

CalcArticFunctor::CalcArticFunctor(Doc *doc) : DocFunctor(doc)
{
    m_parent = NULL;
    m_staffAbove = NULL;
    m_staffBelow = NULL;
    m_layerAbove = NULL;
    m_layerBelow = NULL;
    m_crossStaffAbove = false;
    m_crossStaffBelow = false;
}

FunctorCode CalcArticFunctor::VisitArtic(Artic *artic)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CalcArticFunctor::VisitChord(Chord *chord)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CalcArticFunctor::VisitNote(Note *note)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
