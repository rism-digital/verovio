/////////////////////////////////////////////////////////////////////////////
// Name:        mrest.h
// Author:      Rodolfo Zitellini
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MREST_H__
#define __VRV_MREST_H__

#include "atts_shared.h"
#include "layerelement.h"
#include "positioninterface.h"

namespace vrv {

class Layer;
class Staff;

//----------------------------------------------------------------------------
// MRest
//----------------------------------------------------------------------------

/**
 * This class models the MEI <mRest> element.
 */
class MRest : public LayerElement,
              public PositionInterface,
              public AttColor,
              public AttCue,
              public AttFermataPresent,
              public AttVisibility {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    MRest();
    virtual ~MRest();
    Object *Clone() const override { return new MRest(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "mRest"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    PositionInterface *GetPositionInterface() override { return vrv_cast<PositionInterface *>(this); }
    const PositionInterface *GetPositionInterface() const override { return vrv_cast<const PositionInterface *>(this); }
    ///@}

    /**
     * Get the vertical location for mRest considering other layer elements
     */
    int GetOptimalLayerLocation(const Layer *layer, int defaultLocation) const;

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
