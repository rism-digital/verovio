/////////////////////////////////////////////////////////////////////////////
// Name:        adjusttupletsxfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjusttupletsxfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustTupletsXFunctor
//----------------------------------------------------------------------------

AdjustTupletsXFunctor::AdjustTupletsXFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode AdjustTupletsXFunctor::VisitTuplet(Tuplet *tuplet)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
