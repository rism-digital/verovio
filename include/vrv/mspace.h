/////////////////////////////////////////////////////////////////////////////
// Name:        mspace.h
// Author:      Klaus Rettinghaus
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MSPACE_H__
#define __VRV_MSPACE_H__

#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// MSpace
//----------------------------------------------------------------------------

/**
 * This class models the MEI <space> element.
 */
class MSpace : public LayerElement {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    MSpace();
    virtual ~MSpace();
    Object *Clone() const override { return new MSpace(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "mSpace"; }
    ///@}

    /**
     * Add an element (accid) to a accid.
     * Only supported elements will be actually added to the child list.
     */
    bool IsSupportedChild(ClassId classId) override;

    /** Init the display mspace as mrest when the option is set */
    void InitShowMSpace();

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
};

} // namespace vrv

#endif
