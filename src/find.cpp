/////////////////////////////////////////////////////////////////////////////
// Name:        findallbycomparison.cpp
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "find.h"

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

//----------------------------------------------------------------------------
// FindAllBetween
//----------------------------------------------------------------------------

FindAllBetween::FindAllBetween(Comparison *comparison, const Object *start, const Object *end) : ConstFunctor()
{
    m_comparison = comparison;
    m_start = start;
    m_end = end;
}

FunctorCode FindAllBetween::VisitObject(const Object *object)
{
    // We are reaching the start of the range
    if (m_start == object) {
        // Setting the start to NULL indicates that we are in the range
        m_start = NULL;
    }
    // We have not reached the start yet
    else if (m_start) {
        return FUNCTOR_CONTINUE;
    }

    // evaluate by applying the Comparison operator()
    if ((*m_comparison)(object)) {
        m_elements.push_back(object);
    }

    // We have reached the end of the range
    if (m_end == object) {
        return FUNCTOR_STOP;
    }

    // continue until the end
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
