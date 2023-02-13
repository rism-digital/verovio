/////////////////////////////////////////////////////////////////////////////
// Name:        calcslurdirectionfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "calcslurdirectionfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// CalcSlurDirectionFunctor
//----------------------------------------------------------------------------

CalcSlurDirectionFunctor::CalcSlurDirectionFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode CalcSlurDirectionFunctor::VisitLayerElement(LayerElement *layerElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CalcSlurDirectionFunctor::VisitSlur(Slur *slur)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
