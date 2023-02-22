/////////////////////////////////////////////////////////////////////////////
// Name:        adjustclefchangesfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustclefchangesfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustClefChangesFunctor
//----------------------------------------------------------------------------

AdjustClefChangesFunctor::AdjustClefChangesFunctor(Doc *doc) : DocFunctor(doc)
{
    m_aligner = NULL;
}

FunctorCode AdjustClefChangesFunctor::VisitClef(Clef *clef)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustClefChangesFunctor::VisitMeasure(Measure *measure)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
