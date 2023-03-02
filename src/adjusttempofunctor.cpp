/////////////////////////////////////////////////////////////////////////////
// Name:        adjusttempofunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjusttempofunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustTempoFunctor
//----------------------------------------------------------------------------

AdjustTempoFunctor::AdjustTempoFunctor(Doc *doc) : DocFunctor(doc)
{
    m_systemAligner = NULL;
}

FunctorCode AdjustTempoFunctor::VisitSystem(System *system)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustTempoFunctor::VisitTempo(Tempo *tempo)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
