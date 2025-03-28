/////////////////////////////////////////////////////////////////////////////
// Name:        mrpt2.h
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MRPT2_H__
#define __VRV_MRPT2_H__

#include "atts_cmn.h"
#include "atts_shared.h"
#include "atts_visual.h"
#include "beam.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// MRpt2 (2-measure repeat)
//----------------------------------------------------------------------------

/**
 * This class models the MEI <mRpt2> element.
 */
class MRpt2 : public LayerElement, public AttColor {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    MRpt2();
    virtual ~MRpt2();
    Object *Clone() const override { return new MRpt2(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "mRpt2"; }
    ///@}

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
    //
private:
};

} // namespace vrv

#endif
