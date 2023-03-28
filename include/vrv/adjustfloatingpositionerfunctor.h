/////////////////////////////////////////////////////////////////////////////
// Name:        adjustfloatingpositionerfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTFLOATINGPOSITIONERFUNCTOR_H__
#define __VRV_ADJUSTFLOATINGPOSITIONERFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// AdjustFloatingPositionersFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the position of all floating positioners, staff by staff.
 */
class AdjustFloatingPositionersFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustFloatingPositionersFunctor(Doc *doc);
    virtual ~AdjustFloatingPositionersFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

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
    // The class ID
    ClassId m_classId;
    // Indicates if we are processing floating objects to be put in between the staff
    bool m_inBetween;
};

} // namespace vrv

#endif // __VRV_ADJUSTFLOATINGPOSITIONERFUNCTOR_H__
