/////////////////////////////////////////////////////////////////////////////
// Name:        adjustarticfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTARTICFUNCTOR_H__
#define __VRV_ADJUSTARTICFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// AdjustArticFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the position of articulations.
 */
class AdjustArticFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustArticFunctor(Doc *doc);
    virtual ~AdjustArticFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitArtic(Artic *artic) override;
    FunctorCode VisitChord(Chord *chord) override;
    FunctorCode VisitNote(Note *note) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The list of above / below articulations
    std::list<Artic *> m_articAbove;
    std::list<Artic *> m_articBelow;
    // The parent element to which the articulations refer
    LayerElement *m_parent;
};

//----------------------------------------------------------------------------
// AdjustArticWithSlursFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the position of outside articulations with slur.
 */
class AdjustArticWithSlursFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustArticWithSlursFunctor(Doc *doc);
    virtual ~AdjustArticWithSlursFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitArtic(Artic *artic) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif // __VRV_ADJUSTARTICFUNCTOR_H__
