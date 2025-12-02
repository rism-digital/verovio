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
 * This builds a tree of EditorTreeObject that represent the original score-based structure.
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
// ScoreContextFunctor
//----------------------------------------------------------------------------

/**
 * This builds a tree of EditorTreeObject that represent the original score-based structure.
 */
class ScoreContextFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ScoreContextFunctor(Object *object);
    virtual ~ScoreContextFunctor() = default;

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
    enum TreeLevel { NOT_IN_SCORE = 0, TO_INCLUDE, INCLUDED };

    /** The current object in the tree */
    Object *m_current;
    /** A flag for building a score context or a section context */
    TreeLevel m_inScoreLevel;
};

} // namespace vrv

#endif // __VRV_SAVEFUNCTOR_H__
