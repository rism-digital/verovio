/////////////////////////////////////////////////////////////////////////////
// Name:        mrpt.h
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MRPT_H__
#define __VRV_MRPT_H__

#include "atts_cmn.h"
#include "atts_shared.h"
#include "atts_visual.h"
#include "beam.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// MRpt (measure repeat)
//----------------------------------------------------------------------------

/**
 * This class models the MEI <mRpt> element.
 */
class MRpt : public LayerElement, public AttColor, public AttNumbered, public AttNumberPlacement {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    MRpt();
    virtual ~MRpt();
    Object *Clone() const override { return new MRpt(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "mRpt"; }
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

private:
    //
public:
    /** measure count */
    int m_drawingMeasureCount;

private:
};

} // namespace vrv

#endif
