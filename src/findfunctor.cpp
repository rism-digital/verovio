/////////////////////////////////////////////////////////////////////////////
// Name:        findfunctor.cpp
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "findfunctor.h"

//----------------------------------------------------------------------------

#include "comparison.h"
#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// FindAllByComparisonFunctor
//----------------------------------------------------------------------------

FindAllByComparisonFunctor::FindAllByComparisonFunctor(Comparison *comparison, ListOfObjects *elements)
    : MutableFunctor()
{
    m_comparison = comparison;
    m_elements = elements;
    m_continueDepthSearchForMatches = true;
}

void FindAllByComparisonFunctor::SetContinueDepthSearchForMatches(bool continueDepthSearchForMatches)
{
    m_continueDepthSearchForMatches = continueDepthSearchForMatches;
}

FunctorCode FindAllByComparisonFunctor::VisitObject(Object *object)
{
    // evaluate by applying the Comparison operator()
    if ((*m_comparison)(object)) {
        m_elements->push_back(object);
        if (!m_continueDepthSearchForMatches) {
            return FUNCTOR_SIBLINGS;
        }
    }
    // continue until the end
    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// FindAllConstByComparisonFunctor
//----------------------------------------------------------------------------

FindAllConstByComparisonFunctor::FindAllConstByComparisonFunctor(Comparison *comparison, ListOfConstObjects *elements)
    : ConstFunctor()
{
    m_comparison = comparison;
    m_elements = elements;
    m_continueDepthSearchForMatches = true;
}

void FindAllConstByComparisonFunctor::SetContinueDepthSearchForMatches(bool continueDepthSearchForMatches)
{
    m_continueDepthSearchForMatches = continueDepthSearchForMatches;
}

FunctorCode FindAllConstByComparisonFunctor::VisitObject(const Object *object)
{
    // evaluate by applying the Comparison operator()
    if ((*m_comparison)(object)) {
        m_elements->push_back(object);
        if (!m_continueDepthSearchForMatches) {
            return FUNCTOR_SIBLINGS;
        }
    }
    // continue until the end
    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// FindAllBetweenFunctor
//----------------------------------------------------------------------------

FindAllBetweenFunctor::FindAllBetweenFunctor(
    Comparison *comparison, ListOfConstObjects *elements, const Object *start, const Object *end)
    : ConstFunctor()
{
    m_comparison = comparison;
    m_elements = elements;
    m_start = start;
    m_end = end;
}

FunctorCode FindAllBetweenFunctor::VisitObject(const Object *object)
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
        m_elements->push_back(object);
    }

    // We have reached the end of the range
    if (m_end == object) {
        return FUNCTOR_STOP;
    }

    // continue until the end
    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// FindByComparisonFunctor
//----------------------------------------------------------------------------

FindByComparisonFunctor::FindByComparisonFunctor(Comparison *comparison) : ConstFunctor()
{
    m_comparison = comparison;
    m_element = NULL;
}

FunctorCode FindByComparisonFunctor::VisitObject(const Object *object)
{
    if (m_element) {
        // this should not happen, but just in case
        return FUNCTOR_STOP;
    }

    // evaluate by applying the Comparison operator()
    if ((*m_comparison)(object)) {
        m_element = object;
        return FUNCTOR_STOP;
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// FindByIDFunctor
//----------------------------------------------------------------------------

FindByIDFunctor::FindByIDFunctor(const std::string &id)
{
    m_id = id;
    m_element = NULL;
}

FunctorCode FindByIDFunctor::VisitObject(const Object *object)
{
    if (m_element) {
        // this should not happen, but just in case
        return FUNCTOR_STOP;
    }

    if (m_id == object->GetID()) {
        m_element = object;
        return FUNCTOR_STOP;
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
