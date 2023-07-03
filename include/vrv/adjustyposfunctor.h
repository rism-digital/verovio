/////////////////////////////////////////////////////////////////////////////
// Name:        adjustyposfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTYPOSFUNCTOR_H__
#define __VRV_ADJUSTYPOSFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// AdjustYPosFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the position of the StaffAlignment.
 */
class AdjustYPosFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustYPosFunctor(Doc *doc);
    virtual ~AdjustYPosFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitDiv(Div *div) override;
    FunctorCode VisitStaffAlignment(StaffAlignment *staffAlignment) override;
    FunctorCode VisitSystem(System *system) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The cumulated shift
    int m_cumulatedShift;
};

//----------------------------------------------------------------------------
// AdjustCrossStaffYPosFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the position of cross-staff elements after the adjustment of the staves.
 */
class AdjustCrossStaffYPosFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustCrossStaffYPosFunctor(Doc *doc);
    virtual ~AdjustCrossStaffYPosFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitChord(Chord *chord) override;
    FunctorCode VisitSystem(System *system) override;
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

#endif // __VRV_ADJUSTYPOSFUNCTOR_H__
