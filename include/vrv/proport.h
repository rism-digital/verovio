/////////////////////////////////////////////////////////////////////////////
// Name:        proport.h
// Author:      Donald Byrd
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_PROPORT_H__
#define __VRV_PROPORT_H__

#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Proport
//----------------------------------------------------------------------------

/** 
 * This class models the MEI <proport> element. 
 */
class Proport: public LayerElement,
    public AttDurationRatio
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes.
     */
    ///@{
    Proport();
    virtual ~Proport();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "Proport"; };
    virtual ClassId Is() { return PROPORT; };
    virtual Object* Clone() { return new Proport(*this); };
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() { return true; };
    
private:
    
public:

private:
    
};
    

} // namespace vrv

#endif
