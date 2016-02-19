/////////////////////////////////////////////////////////////////////////////
// Name:        mrest.h
// Author:      Rodolfo Zitellini
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_MREST_H__
#define __VRV_MREST_H__

#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// MRest
//----------------------------------------------------------------------------

/** 
 * This class models the MEI <mRest>
 */
class MRest: public LayerElement
{
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    MRest( );
    virtual ~MRest();
    virtual void Reset();
    virtual std::string GetClassName( ){ return "MRest"; }; ;
    virtual ClassId Is() { return MREST; };
    ///@}

private:
    
public:

private:
    
};

} // namespace vrv    

#endif
