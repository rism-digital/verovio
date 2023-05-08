/////////////////////////////////////////////////////////////////////////////
// Name:        pghead2.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PGHEAD2_H__
#define __VRV_PGHEAD2_H__

#include "runningelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// PgHead2
//----------------------------------------------------------------------------

/**
 * This class represents an MEI pgHead2.
 */
class PgHead2 : public RunningElement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    PgHead2();
    virtual ~PgHead2();
    void Reset() override;
    std::string GetClassName() const override { return "PgHead2"; }
    ///@}

    /**
     * Overriden to get the appropriate margin
     */
    int GetTotalHeight(const Doc *doc) const override;

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
    //
private:
    //
};

} // namespace vrv

#endif
