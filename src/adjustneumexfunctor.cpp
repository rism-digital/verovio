/////////////////////////////////////////////////////////////////////////////
// Name:        adjustneumexfunctor.cpp
// Author:      Laurent Pugin
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustneumexfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "score.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustNeumeXFunctor
//----------------------------------------------------------------------------

AdjustNeumeXFunctor::AdjustNeumeXFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode AdjustNeumeXFunctor::VisitNeume(Neume *neume)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
