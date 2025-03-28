/////////////////////////////////////////////////////////////////////////////
// Name:        lb.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_LB_H__
#define __VRV_LB_H__

#include "atts_shared.h"
#include "textelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Lb
//----------------------------------------------------------------------------

/**
 * This class models the MEI <lb>
 */
class Lb : public TextElement {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    Lb();
    virtual ~Lb();
    Object *Clone() const override { return new Lb(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "lb"; }
    ///@}

    /**
     * Lb is an empty element
     */
    void AddChild(Object *object) override {};

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
