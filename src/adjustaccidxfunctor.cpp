/////////////////////////////////////////////////////////////////////////////
// Name:        adjustaccidxfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustaccidxfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustAccidXFunctor
//----------------------------------------------------------------------------

AdjustAccidXFunctor::AdjustAccidXFunctor(Doc *doc) : DocFunctor(doc)
{
    m_currentMeasure = NULL;
}

FunctorCode AdjustAccidXFunctor::VisitAlignment(Alignment *alignment)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustAccidXFunctor::VisitAlignmentReference(AlignmentReference *alignmentReference)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustAccidXFunctor::VisitMeasure(Measure *measure)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
