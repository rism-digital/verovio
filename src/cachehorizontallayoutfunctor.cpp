/////////////////////////////////////////////////////////////////////////////
// Name:        cachehorizontallayoutfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "cachehorizontallayoutfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// CacheHorizontalLayoutFunctor
//----------------------------------------------------------------------------

CacheHorizontalLayoutFunctor::CacheHorizontalLayoutFunctor(Doc *doc) : DocFunctor(doc)
{
    m_restore = false;
}

FunctorCode CacheHorizontalLayoutFunctor::VisitArpeg(Arpeg *arpeg)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CacheHorizontalLayoutFunctor::VisitLayerElement(LayerElement *layerElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CacheHorizontalLayoutFunctor::VisitMeasure(Measure *measure)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
