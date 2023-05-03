/////////////////////////////////////////////////////////////////////////////
// Name:        adjuststaffoverlapfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTSTAFFOVERLAPFUNCTOR_H__
#define __VRV_ADJUSTSTAFFOVERLAPFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// AdjustStaffOverlapFunctor
//----------------------------------------------------------------------------

/**
 * This class calculates the overlap of the staff alignments by looking at the overflow bounding boxes.
 */
class AdjustStaffOverlapFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustStaffOverlapFunctor(Doc *doc);
    virtual ~AdjustStaffOverlapFunctor() = default;
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
    // The previous staff alignment
    StaffAlignment *m_previous;
};

} // namespace vrv

#endif // __VRV_ADJUSTSTAFFOVERLAPFUNCTOR_H__
