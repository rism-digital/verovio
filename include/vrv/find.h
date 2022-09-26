/////////////////////////////////////////////////////////////////////////////
// Name:        find.h
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FIND_H__
#define __VRV_FIND_H__

#include "functor.h"

namespace vrv {

class Comparison;
class Object;

//----------------------------------------------------------------------------
// FindAllByComparison
//----------------------------------------------------------------------------

/**
 * This class finds all elements in the tree by comparison
 */
class FindAllByComparison : public MutableFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    FindAllByComparison(Comparison *comparison, ListOfObjects *elements);
    virtual ~FindAllByComparison() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() override { return false; }

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
// FindAllConstByComparison
//----------------------------------------------------------------------------

/**
 * This class finds all elements in the tree by comparison
 */
class FindAllConstByComparison : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    FindAllConstByComparison(Comparison *comparison, ListOfConstObjects *elements);
    virtual ~FindAllConstByComparison() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() override { return false; }

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
// FindAllBetween
//----------------------------------------------------------------------------

/**
 * This class finds all elements in the tree by comparison between start and end objects
 */
class FindAllBetween : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    FindAllBetween(Comparison *comparison, ListOfConstObjects *elements, const Object *start, const Object *end);
    virtual ~FindAllBetween() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() override { return false; }

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
// FindByComparison
//----------------------------------------------------------------------------

/**
 * This class finds an element in the tree by comparison
 */
class FindByComparison : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    FindByComparison(Comparison *comparison);
    virtual ~FindByComparison() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() override { return false; }

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
// FindByID
//----------------------------------------------------------------------------

/**
 * This class finds an element with a specified id.
 */
class FindByID : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    FindByID(const std::string &id);
    virtual ~FindByID() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() override { return false; }

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

} // namespace vrv

#endif // __VRV_FIND_H__
