/////////////////////////////////////////////////////////////////////////////
// Name:        multirest.h
// Author:      Rodolfo Zitellini
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MULTIREST_H__
#define __VRV_MULTIREST_H__

#include "atts_cmn.h"
#include "atts_shared.h"
#include "atts_visual.h"
#include "layerelement.h"
#include "positioninterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// MultiRest
//----------------------------------------------------------------------------

/**
 * This class models the MEI <multiRest> element.
 */
class MultiRest : public LayerElement,
                  public PositionInterface,
                  public AttColor,
                  public AttMultiRestVis,
                  public AttNumbered,
                  public AttNumberPlacement,
                  public AttWidth {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    MultiRest();
    virtual ~MultiRest();
    Object *Clone() const override { return new MultiRest(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "multiRest"; }
    ///@}

    /**
     * True if block style rendering applies
     */
    bool UseBlockStyle(const Doc *doc) const;

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
