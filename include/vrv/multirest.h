/////////////////////////////////////////////////////////////////////////////
// Name:        multirest.h
// Author:      Rodolfo Zitellini
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_MULTIREST_H__
#define __VRV_MULTIREST_H__

#include "atts_cmn.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// MultiRest
//----------------------------------------------------------------------------

/** 
 * This class models the MEI <multiRest> element. 
 */
class MultiRest: public LayerElement,
    public AttNumbered
{
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    MultiRest();
    //MultiRest(int number);
    virtual ~MultiRest();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "MultiRest"; };
    virtual ClassId Is() { return MULTI_REST; };
    ///@}
    
private:
    
public:

private:
    
};

} // namespace vrv    

#endif
