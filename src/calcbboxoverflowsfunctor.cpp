/////////////////////////////////////////////////////////////////////////////
// Name:        calcbboxoverflowsfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "calcbboxoverflowsfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// CalcBBoxOverflowsFunctor
//----------------------------------------------------------------------------

CalcBBoxOverflowsFunctor::CalcBBoxOverflowsFunctor(Doc *doc) : DocFunctor(doc)
{
    m_staffAlignment = NULL;
}

FunctorCode CalcBBoxOverflowsFunctor::VisitObject(Object *object)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CalcBBoxOverflowsFunctor::VisitObjectEnd(Object *object)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
