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

//----------------------------------------------------------------------------
// MRest
//----------------------------------------------------------------------------

/**
 * This class models the MEI <mRest>
 */
class MRest : public LayerElement,
              public PositionInterface,
              public AttVisibility,
              public AttFermatapresent,
              public AttRelativesize {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    MRest();
    virtual ~MRest();
    virtual void Reset();
    virtual std::string GetClassName() const { return "MRest"; }
    virtual ClassId GetClassId() const { return MREST; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual PositionInterface *GetPositionInterface() { return dynamic_cast<PositionInterface *>(this); }
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * See Object::ResetHorizontalAlignment
     */
    virtual int ResetHorizontalAlignment(FunctorParams *functorParams);

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
