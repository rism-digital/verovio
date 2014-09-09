/////////////////////////////////////////////////////////////////////////////
// Name:        dot.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_DOT_H__
#define __VRV_DOT_H__

#include "layerelement.h"
#include "positioninterface.h"

namespace vrv {
    
class ChildElement;

//----------------------------------------------------------------------------
// Dot
//----------------------------------------------------------------------------

class Dot: public LayerElement, public PositionInterface
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Dot();
    virtual ~Dot();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "Dot"; };
    ///@}
    
protected:

private:
    
public:
    
private:

};

} // namespace vrv    
    
#endif
