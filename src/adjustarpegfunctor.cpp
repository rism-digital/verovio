/////////////////////////////////////////////////////////////////////////////
// Name:        adjustarpegfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustarpegfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustArpegFunctor
//----------------------------------------------------------------------------

AdjustArpegFunctor::AdjustArpegFunctor(Doc *doc) : DocFunctor(doc)
{
    m_measureAligner = NULL;
}

FunctorCode AdjustArpegFunctor::VisitAlignment(Alignment *alignment)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustArpegFunctor::VisitArpeg(Arpeg *arpeg)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustArpegFunctor::VisitMeasureEnd(Measure *measure)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
