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
    // constructors and destructors
    Rest();
    virtual ~Rest();
    
    /**
     * Comparison operator. 
     * Check if the LayerElement if a Rest and compare attributes
     */
    virtual bool operator==(Object& other);
    
    virtual std::string GetClassName( ) { return "Rest"; };
    
private:
    
public:

private:
    
};

} // namespace vrv    

#endif
