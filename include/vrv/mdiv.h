/////////////////////////////////////////////////////////////////////////////
// Name:        mdiv.h
// Author:      Laurent Pugin
// Created:     2018/02/15
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MDIV_H__
#define __VRV_MDIV_H__

#include "atts_shared.h"
#include "drawinginterface.h"
#include "pageelement.h"
#include "pagemilestone.h"

namespace vrv {

//----------------------------------------------------------------------------
// Mdiv
//----------------------------------------------------------------------------

/**
 * This class represent a <mdiv> in page-based MEI.
 */
class Mdiv : public PageElement,
             public VisibilityDrawingInterface,
             public PageMilestoneInterface,
             public AttLabelled,
             public AttNNumberLike {

public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Mdiv();
    virtual ~Mdiv();
    Object *Clone() const override { return new Mdiv(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "mdiv"; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    bool IsSupportedChild(ClassId classId) override;
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    VisibilityDrawingInterface *GetVisibilityDrawingInterface() override
    {
        return vrv_cast<VisibilityDrawingInterface *>(this);
    }
    const VisibilityDrawingInterface *GetVisibilityDrawingInterface() const override
    {
        return vrv_cast<const VisibilityDrawingInterface *>(this);
    }
    ///@}

    /**
     * Make the mdiv and its parent (recursively) visible
     */
    void MakeVisible();

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
