/////////////////////////////////////////////////////////////////////////////
// Name:        gracegrp.h
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_GRACEGRP_H__
#define __VRV_GRACEGRP_H__

#include "atts_cmn.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// GraceGrp
//----------------------------------------------------------------------------

class GraceGrp : public LayerElement, public AttColor, public AttGraced, public AttGraceGrpLog {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    GraceGrp();
    virtual ~GraceGrp();
    Object *Clone() const override { return new GraceGrp(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "graceGrp"; }
    ///@}

    /**
     * Add childElement to a element.
     */
    bool IsSupportedChild(ClassId classId) override;

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

#endif
