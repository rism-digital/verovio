/////////////////////////////////////////////////////////////////////////////
// Name:        fb.h
// Author:      Rodolfo Zitellini
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FB_H__
#define __VRV_FB_H__

#include "atts_shared.h"
#include "textelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Fb (figured bass)
//----------------------------------------------------------------------------

/**
 * This class models the MEI <fb> element.
 */
class Fb : public Object {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    Fb();
    virtual ~Fb();
    Object *Clone() const override { return new Fb(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "fb"; }
    ///@}

    /**
     * Add an element (f) to an fb.
     * Only supported elements will be actually added to the child list.
     */
    bool IsSupportedChild(ClassId classId) override;

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
