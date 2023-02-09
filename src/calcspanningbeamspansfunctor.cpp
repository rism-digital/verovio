/////////////////////////////////////////////////////////////////////////////
// Name:        calcspanningbeamspansfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "calcspanningbeamspansfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// CalcSpanningBeamSpansFunctor
//----------------------------------------------------------------------------

CalcSpanningBeamSpansFunctor::CalcSpanningBeamSpansFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode CalcSpanningBeamSpansFunctor::VisitBeamSpan(BeamSpan *beamSpan)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
