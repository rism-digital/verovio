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
class Rest : public LayerElement, public DurationInterface, public PositionInterface, public AttColor {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Rest();
    virtual ~Rest();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Rest"; }
    virtual ClassId Is() const { return REST; }
    ///@}

    virtual PositionInterface *GetPositionInterface() { return dynamic_cast<PositionInterface *>(this); }

    virtual DurationInterface *GetDurationInterface() { return dynamic_cast<DurationInterface *>(this); }

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
