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
class FindAllByComparisonFunctor : public Functor {
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
    FindNextChildByComparisonFunctor(Comparison *comparison, const Object *start);
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
    Comparison *m_comparison;
    // The object to start with
    const Object *m_start;
    // The search result
    const Object *m_element;
};

//----------------------------------------------------------------------------
// FindPreviousChildByComparisonFunctor
//----------------------------------------------------------------------------

/**
 * This class finds the previous child matching the comparison object.
 */
class FindPreviousChildByComparisonFunctor : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    FindPreviousChildByComparisonFunctor(Comparison *comparison, const Object *start);
    virtual ~FindPreviousChildByComparisonFunctor() = default;
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
    // The object to start with
    const Object *m_start;
    // The search result
    const Object *m_element;
};

//----------------------------------------------------------------------------
// FindExtremeByComparisonFunctor
//----------------------------------------------------------------------------

/**
 * This class finds the last object matching the comparison.
 */
class FindExtremeByComparisonFunctor : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    FindExtremeByComparisonFunctor(Comparison *comparison);
    virtual ~FindExtremeByComparisonFunctor() = default;
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
// FindAllReferencedObjectsFunctor
//----------------------------------------------------------------------------

/**
 * This class finds all objects to which another object refers to.
 * The constructor should take either elements or a listWithAttName pointer set, the other should be NULL.
 */
class FindAllReferencedObjectsFunctor : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    FindAllReferencedObjectsFunctor(SetOfConstObjects *elements, ListOfObjectAttNamePairs *listWithAttName);
    virtual ~FindAllReferencedObjectsFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Include milestone references?
     */
    void IncludeMilestoneReferences(bool included) { m_milestoneReferences = included; }

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
    void AddObject(const Object *object, const std::string &attribute);

public:
    //
private:
    // The set of all matching objects
    SetOfConstObjects *m_elements;
    // The list of pairs of matching objects with the attribute name
    ListOfObjectAttNamePairs *m_listWithAttName;
    // A flag indicating if milestone references should be included as well
    bool m_milestoneReferences;
};

//----------------------------------------------------------------------------
// FindAllReferringObjectsFunctor
//----------------------------------------------------------------------------

/**
 * This class finds all objects referring to a specific object
 */
class FindAllReferringObjectsFunctor : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    FindAllReferringObjectsFunctor(const Object *object, ListOfObjectAttNamePairs *elements);
    virtual ~FindAllReferringObjectsFunctor() = default;
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
    // The object referred to
    const Object *m_object;
    // The set of all objects with the attribute name they are referring through
    ListOfObjectAttNamePairs *m_elements;
};

//----------------------------------------------------------------------------
// FindElementInLayerStaffDefFunctor
//----------------------------------------------------------------------------

/**
 * This class looks for an element with given ID in StaffDef elements (Clef, KeySig, etc.)
 */
class FindElementInLayerStaffDefFunctor : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    FindElementInLayerStaffDefFunctor(const std::string &xmlId);
    virtual ~FindElementInLayerStaffDefFunctor() = default;
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
    FunctorCode VisitLayer(const Layer *layer) override;
    FunctorCode VisitScore(const Score *score) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The search result
    const Object *m_element;
    // ID of the element to be found
    std::string m_id;
};

//----------------------------------------------------------------------------
// AddToFlatListFunctor
//----------------------------------------------------------------------------

/**
 * This class adds elements and its children to a flat list.
 */
class AddToFlatListFunctor : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AddToFlatListFunctor(ListOfConstObjects *flatList);
    virtual ~AddToFlatListFunctor() = default;
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
    // The list of elements
    ListOfConstObjects *m_flatList;
};

} // namespace vrv

#endif // __VRV_FINDFUNCTOR_H__
