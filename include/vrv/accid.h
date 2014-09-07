/////////////////////////////////////////////////////////////////////////////
// Name:        accid.h
// Author:      Laurent Pugin
// Created:     201X
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_ACCID_H__
#define __VRV_ACCID_H__

#include "layerelement.h"
#include "positioninterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Accid
//----------------------------------------------------------------------------

class Accid: public LayerElement, public PositionInterface,
    public AttAccidental
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Accid();
    virtual ~Accid();
    void Reset();
    virtual std::string GetClassName( ) { return "Accid"; };
    ///@}
    
protected:

private:
    
public:
    
private:

};

} // namespace vrv    
    
#endif
