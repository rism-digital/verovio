/////////////////////////////////////////////////////////////////////////////
// Name:        element.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_ELEMENT_H__
#define __VRV_ELEMENT_H__

#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {
    
class ChildElement;

//----------------------------------------------------------------------------
// Element
//----------------------------------------------------------------------------

class Element: public LayerElement,
    public AttCommon
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Element();
    virtual ~Element();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "Element"; };
    ///@}
    
    /**
     * Add childElement to a element.
     */
    void AddChildElement(Object *childElement);
    
protected:

private:
    
public:
    
private:

};

} // namespace vrv    
    
#endif
