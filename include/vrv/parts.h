/////////////////////////////////////////////////////////////////////////////
// Name:        parts.h
// Author:      Laurent Pugin
// Created:     2018/02/15
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PARTS_H__
#define __VRV_PARTS_H__

#include "atts_shared.h"
#include "pageelement.h"
#include "pagemilestone.h"

namespace vrv {

//----------------------------------------------------------------------------
// Parts
//----------------------------------------------------------------------------

/**
 * This class represents a <parts> MEI.
 */
class Parts : public PageElement, public PageMilestoneInterface, public AttLabelled, public AttNNumberLike {

public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Parts();
    virtual ~Parts();
    Object *Clone() const override { return new Parts(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "parts"; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    bool IsSupportedChild(ClassId classId) override;
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
    //
private:
    //
};

} // namespace vrv

#endif
