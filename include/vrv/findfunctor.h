/////////////////////////////////////////////////////////////////////////////
// Name:        findfunctor.h
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FINDFUNCTOR_H__
#define __VRV_FINDFUNCTOR_H__

#include "functor.h"

namespace vrv {

class Comparison;
class Object;

//----------------------------------------------------------------------------
// FindAllByComparisonFunctor
//----------------------------------------------------------------------------

/**
 * This class finds all elements in the tree by comparison
 */
class FindAllByComparisonFunctor : public MutableFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    FindAllByComparisonFunctor(Comparison *comparison, ListOfObjects *elements);
    virtual ~FindAllByComparisonFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Configure depth search
     */
    void SetContinueDepthSearchForMatches(bool continueDepthSearchForMatches);

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitObject(Object *object) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The comparison
    Comparison *m_comparison;
    // True if search should be continued for matches
    bool m_continueDepthSearchForMatches;
    // The element list where the search result is appended
    ListOfObjects *m_elements;
};

//----------------------------------------------------------------------------
// FindAllConstByComparisonFunctor
//----------------------------------------------------------------------------

/**
 * This class finds all elements in the tree by comparison
 */
class FindAllConstByComparisonFunctor : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    FindAllConstByComparisonFunctor(Comparison *comparison, ListOfConstObjects *elements);
    virtual ~FindAllConstByComparisonFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Configure depth search
     */
    void SetContinueDepthSearchForMatches(bool continueDepthSearchForMatches);

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitObject(const Object *object) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The comparison
    Comparison *m_comparison;
    // True if search should be continued for matches
    bool m_continueDepthSearchForMatches;
    // The element list where the search result is appended
    ListOfConstObjects *m_elements;
};

//----------------------------------------------------------------------------
// FindAllBetweenFunctor
//----------------------------------------------------------------------------

/**
 * This class finds all elements in the tree by comparison between start and end objects
 */
class FindAllBetweenFunctor : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    FindAllBetweenFunctor(Comparison *comparison, ListOfConstObjects *elements, const Object *start, const Object *end);
    virtual ~FindAllBetweenFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitObject(const Object *object) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The comparison
    Comparison *m_comparison;
    // The start and end object
    const Object *m_start, *m_end;
    // The element list where the search result is appended
    ListOfConstObjects *m_elements;
};

//----------------------------------------------------------------------------
// FindByComparisonFunctor
//----------------------------------------------------------------------------

/**
 * This class finds an element in the tree by comparison
 */
class FindByComparisonFunctor : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    FindByComparisonFunctor(Comparison *comparison);
    virtual ~FindByComparisonFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Retrieve the search result
     */
    const Object *GetElement() const { return m_element; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitObject(const Object *object) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The comparison
    Comparison *m_comparison;
    // The search result
    const Object *m_element;
};

//----------------------------------------------------------------------------
// FindByIDFunctor
//----------------------------------------------------------------------------

/**
 * This class finds an element with a specified id.
 */
class FindByIDFunctor : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    FindByIDFunctor(const std::string &id);
    virtual ~FindByIDFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Retrieve the search result
     */
    const Object *GetElement() const { return m_element; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitObject(const Object *object) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The id we are looking for
    std::string m_id;
    // The search result
    const Object *m_element;
};

//----------------------------------------------------------------------------
// FindNextChildByComparisonFunctor
//----------------------------------------------------------------------------

/**
 * This class finds the next child matching the comparison object.
 */
class FindNextChildByComparisonFunctor : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    FindNextChildByComparisonFunctor(const Comparison *comparison, const Object *start);
    virtual ~FindNextChildByComparisonFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Retrieve the search result
     */
    const Object *GetElement() const { return m_element; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitObject(const Object *object) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The comparison
    const Comparison *m_comparison;
    // The object to start with
    const Object *m_start;
    // The search result
    const Object *m_element;
};

} // namespace vrv

#endif // __VRV_FINDFUNCTOR_H__
