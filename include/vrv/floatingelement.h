/////////////////////////////////////////////////////////////////////////////
// Name:        floatingelement.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_FLOATING_ELEMENT_H__
#define __VRV_FLOATING_ELEMENT_H__

#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// FloatingElement
//----------------------------------------------------------------------------

/** 
 * This class is an interface for elements with duration, such as notes and rests.
 * It is not an abstract class but should not be instanciate directly.
 */
class FloatingElement: public DocObject
{
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    FloatingElement();
    FloatingElement(std::string classid);
    virtual ~FloatingElement();
    virtual void Reset();
    virtual ClassId Is() { return FLOATING_ELEMENT; };
    ///@}
    
    //----------//
    // Functors //
    //----------//
    
    /**
     * See Object::FillStaffCurrentTimeSpanning
     */
    virtual int FillStaffCurrentTimeSpanning( ArrayPtrVoid *params );
    
    /**
     * See Object::PrepareTimeSpanning
     */
    virtual int PrepareTimeSpanning( ArrayPtrVoid *params );
    
    /**
     * Reset the drawing values before calling PrepareDrawing after changes.
     */
    virtual int ResetDarwing( ArrayPtrVoid *params );
    ///@}
    
private:
    
public:

private:
    
};
    
} // namespace vrv 

#endif
