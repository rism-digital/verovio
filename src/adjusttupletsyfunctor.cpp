/////////////////////////////////////////////////////////////////////////////
// Name:        adjusttupletsyfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjusttupletsyfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustTupletsYFunctor
//----------------------------------------------------------------------------

AdjustTupletsYFunctor::AdjustTupletsYFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode AdjustTupletsYFunctor::VisitTuplet(Tuplet *tuplet)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
