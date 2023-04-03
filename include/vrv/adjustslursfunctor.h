/////////////////////////////////////////////////////////////////////////////
// Name:        adjustslursfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTSLURSFUNCTOR_H__
#define __VRV_ADJUSTSLURSFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// AdjustSlursFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the position of slurs.
 */
class AdjustSlursFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustSlursFunctor(Doc *doc);
    virtual ~AdjustSlursFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Check existence of cross-staff slurs
     */
    bool HasCrossStaffSlurs() const { return m_crossStaffSlurs; }

    /*
     * Functor interface
     */
    ///@{
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
    // Indicates that there is at least one cross-staff slur
    bool m_crossStaffSlurs;
};

} // namespace vrv

#endif // __VRV_ADJUSTSLURSFUNCTOR_H__
