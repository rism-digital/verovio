/////////////////////////////////////////////////////////////////////////////
// Name:        label.h
// Author:      Laurent Pugin
// Created:     19/06/2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_LABEL_H__
#define __VRV_LABEL_H__

#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// Label
//----------------------------------------------------------------------------

/**
 * This class models the MEI <label> element.
 */
class Label : public Object, public TextListInterface {

public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Label();
    virtual ~Label();
    Object *Clone() const override { return new Label(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Label"; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    bool IsSupportedChild(Object *object) override;
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
