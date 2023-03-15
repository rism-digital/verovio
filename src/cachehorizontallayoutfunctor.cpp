/////////////////////////////////////////////////////////////////////////////
// Name:        cachehorizontallayoutfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "cachehorizontallayoutfunctor.h"

//----------------------------------------------------------------------------

#include "arpeg.h"

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
    arpeg->CacheXRel(m_restore);

    return FUNCTOR_CONTINUE;
}

FunctorCode CacheHorizontalLayoutFunctor::VisitLayerElement(LayerElement *layerElement)
{
    layerElement->CacheXRel(m_restore);
    layerElement->CacheYRel(m_restore);

    return FUNCTOR_CONTINUE;
}

FunctorCode CacheHorizontalLayoutFunctor::VisitMeasure(Measure *measure)
{
    measure->CacheXRel(m_restore);

    if (measure->GetLeftBarLine()) this->VisitBarLine(measure->GetLeftBarLine());
    if (measure->GetRightBarLine()) this->VisitBarLine(measure->GetRightBarLine());

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
