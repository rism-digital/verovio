/////////////////////////////////////////////////////////////////////////////
// Name:        fig.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FIG_H__
#define __VRV_FIG_H__

#include "areaposinterface.h"
#include "textelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Fig (figure)
//----------------------------------------------------------------------------

/**
 * This class models the MEI <fig> element.
 */
class Fig : public TextElement, public AreaPosInterface {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    Fig();
    virtual ~Fig();
    Object *Clone() const override { return new Fig(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Fig"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    AreaPosInterface *GetAreaPosInterface() override { return dynamic_cast<AreaPosInterface *>(this); }
    const AreaPosInterface *GetAreaPosInterface() const override
    {
        return dynamic_cast<const AreaPosInterface *>(this);
    }
    ///@}

    /**
     * Add an element (svg) to an fig.
     * Only supported elements will be actually added to the child list.
     */
    bool IsSupportedChild(Object *object) override;

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
