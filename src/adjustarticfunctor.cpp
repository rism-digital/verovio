/////////////////////////////////////////////////////////////////////////////
// Name:        adjustarticfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustarticfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustArticFunctor
//----------------------------------------------------------------------------

AdjustArticFunctor::AdjustArticFunctor(Doc *doc) : DocFunctor(doc)
{
    m_parent = NULL;
}

FunctorCode AdjustArticFunctor::VisitArtic(Artic *artic)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustArticFunctor::VisitChord(Chord *chord)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustArticFunctor::VisitNote(Note *note)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
