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
    std::string GetClassName() const override { return "MRest"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    PositionInterface *GetPositionInterface() override { return dynamic_cast<PositionInterface *>(this); }
    const PositionInterface *GetPositionInterface() const override
    {
        return dynamic_cast<const PositionInterface *>(this);
    }
    ///@}

    /**
     * Get the vertical location for mRest considering other layer elements
     */
    int GetOptimalLayerLocation(Staff *staff, Layer *layer, int defaultLocation);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ConvertMarkupAnalytical
     */
    int ConvertMarkupAnalytical(FunctorParams *functorParams) override;

    /**
     * See Object::ResetDrawing
     */
    int ResetDrawing(FunctorParams *functorParams) override;

    /**
     * See Object::ResetHorizontalAlignment
     */
    int ResetHorizontalAlignment(FunctorParams *functorParams) override;

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
