/////////////////////////////////////////////////////////////////////////////
// Name:        rest.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_REST_H__
#define __VRV_REST_H__

#include "durationinterface.h"
#include "layerelement.h"
#include "positioninterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Rest
//----------------------------------------------------------------------------

/**
 * This class models the MEI <rest> element.
 */
class Rest : public LayerElement, public DurationInterface, public PositionInterface {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Rest();
    virtual ~Rest();
    virtual void Reset();
    virtual std::string GetClassName() { return "Rest"; };
    virtual ClassId Is() { return REST; };
    ///@}

    virtual PositionInterface *GetPositionInterface() { return vrv_cast2(PositionInterface *)(this); }

    virtual DurationInterface *GetDurationInterface() { return vrv_cast2(DurationInterface *)(this); }

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() { return true; };

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
