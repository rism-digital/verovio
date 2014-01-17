/////////////////////////////////////////////////////////////////////////////
// Name:        mrest.h
// Author:      Rodolfo Zitellini
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_MREST_H__
#define __VRV_MREST_H__

#include "vrvdef.h"
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
    // constructors and destructors
    MRest( );
    virtual ~MRest();
    
    /**
     * Comparison operator. 
     * Check if the LayerElement if a Clef and compare attributes
     */
    virtual bool operator==(Object& other);
    
    virtual std::string GetClassName( ){ return "MRest"; }; ;

private:
    
public:

private:
    
};

} // namespace vrv    

#endif
