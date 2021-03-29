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
                  public AttWidth {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    MultiRest();
    virtual ~MultiRest();
    virtual Object *Clone() const { return new MultiRest(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "MultiRest"; }
    virtual ClassId GetClassId() const { return MULTIREST; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
