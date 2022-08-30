/////////////////////////////////////////////////////////////////////////////
// Name:        findallbycomparison.cpp
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "findallbycomparison.h"

//----------------------------------------------------------------------------

#include "comparison.h"
#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// FindAllByComparison
//----------------------------------------------------------------------------

FindAllByComparison::FindAllByComparison(Comparison *comparison) : ConstFunctor()
{
    m_comparison = comparison;
    m_continueDepthSearchForMatches = true;
}

void FindAllByComparison::SetContinueDepthSearchForMatches(bool continueDepthSearchForMatches)
{
    m_continueDepthSearchForMatches = continueDepthSearchForMatches;
}

FunctorCode FindAllByComparison::VisitObject(const Object *object)
{
    // evaluate by applying the Comparison operator()
    if ((*m_comparison)(object)) {
        m_elements.push_back(object);
        if (!m_continueDepthSearchForMatches) {
            return FUNCTOR_SIBLINGS;
        }
    }
    // continue until the end
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
