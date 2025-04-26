/////////////////////////////////////////////////////////////////////////////
// Name:        editfunctor.h
// Author:      Laurent Pugin
// Created:     2025
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_EDITFUNCTOR_H__
#define __VRV_EDITFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// SectionContextFunctor
//----------------------------------------------------------------------------

/**
 * This build a tree of EditorTreeObject representing the original score-based structure.
 */
class SectionContextFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    SectionContextFunctor(Object *object);
    virtual ~SectionContextFunctor() = default;

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    FunctorCode VisitObject(Object *object) override;
    FunctorCode VisitObjectEnd(Object *object) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    /** The current object in the tree */
    Object *m_current;
};

//----------------------------------------------------------------------------
// ScoreTreeFunctor
//----------------------------------------------------------------------------

/**
 * This build a tree of EditorTreeObject representing the original score-based structure.
 */
class ScoreTreeFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ScoreTreeFunctor(Object *object, bool scoreContext);
    virtual ~ScoreTreeFunctor() = default;

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    FunctorCode VisitObject(Object *object) override;
    FunctorCode VisitObjectEnd(Object *object) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    /** The current object in the tree */
    Object *m_current;
    /** A flag for building a score context or a section context */
    bool m_scoreContext;
};

} // namespace vrv

#endif // __VRV_SAVEFUNCTOR_H__
