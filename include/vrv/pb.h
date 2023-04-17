/////////////////////////////////////////////////////////////////////////////
// Name:        pb.h
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PB_H__
#define __VRV_PB_H__

#include "atts_shared.h"
#include "systemelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Pb
//----------------------------------------------------------------------------

/**
 * This class represents a MEI pb in score-based MEI.
 * In page-based MEI, it remains as is as. Actual pages are represented by Page objects.
 */
class Pb : public SystemElement, public AttNNumberLike {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Pb();
    virtual ~Pb();
    Object *Clone() const override { return new Pb(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Pb"; }
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(MutableFunctor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(MutableFunctor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif
