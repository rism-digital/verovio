/////////////////////////////////////////////////////////////////////////////
// Name:        calcalignmentpitchposfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "calcalignmentpitchposfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// CalcAlignmentPitchPosFunctor
//----------------------------------------------------------------------------

CalcAlignmentPitchPosFunctor::CalcAlignmentPitchPosFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode CalcAlignmentPitchPosFunctor::VisitLayerElement(LayerElement *layerElement)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
