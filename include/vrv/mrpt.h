/////////////////////////////////////////////////////////////////////////////
// Name:        mrpt.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_MRPT_H__
#define __VRV_MRPT_H__

#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// MRpt
//----------------------------------------------------------------------------

/** 
 * This class models the MEI <mRpt>
 */
class MRpt: public LayerElement
{
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    MRpt( );
    virtual ~MRpt();
    virtual void Reset();
    virtual std::string GetClassName( ){ return "MRpt"; }; ;
    virtual ClassId Is() { return MRPT; };
    ///@}

private:
    
public:

private:
    
};

} // namespace vrv    

#endif
