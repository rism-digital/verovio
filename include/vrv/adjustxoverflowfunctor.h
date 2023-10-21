/////////////////////////////////////////////////////////////////////////////
// Name:        adjustxoverflowfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTXOVERFLOWFUNCTOR_H__
#define __VRV_ADJUSTXOVERFLOWFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// AdjustXOverflowFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the X position of right barlines in order to make sure there is no text content
 * overflowing.
 */
class AdjustXOverflowFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustXOverflowFunctor(int margin);
    virtual ~AdjustXOverflowFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Getter and setter for various properties
     */
    ///@{
    void SetCurrentSystem(System *system) { m_currentSystem = system; }
    void SetLastMeasure(Measure *measure) { m_lastMeasure = measure; }
    FloatingPositioner *GetWidestPositioner() const { return m_currentWidest; }
    ///@}

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitControlElement(ControlElement *controlElement) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitSystem(System *system) override;
    FunctorCode VisitSystemEnd(System *system) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The current system
    System *m_currentSystem;
    // The last measure
    Measure *m_lastMeasure;
    // The current widest control event
    FloatingPositioner *m_currentWidest;
    // The margin
    int m_margin;
};

} // namespace vrv

#endif // __VRV_ADJUSTXOVERFLOWFUNCTOR_H__
