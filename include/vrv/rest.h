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
class Rest: public LayerElement, public DurationInterface, public PositionInterface
{
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    Rest();
    virtual ~Rest();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "Rest"; };
    ///@}
    
private:
    
public:

private:
    
};

} // namespace vrv    

#endif
