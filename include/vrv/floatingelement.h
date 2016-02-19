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
 * This class is an interface for elements appearing within measure.
 * It is not an abstract class but should not be instanciated directly.
 */
class FloatingElement : public DocObject {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
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
    virtual int FillStaffCurrentTimeSpanning(ArrayPtrVoid *params);

    /**
     * See Object::PrepareTimeSpanning
     */
    virtual int PrepareTimeSpanning(ArrayPtrVoid *params);
    
    /**
     * See Object::PrepareTimestamps
     */
    virtual int PrepareTimestamps(ArrayPtrVoid *params);
    
    /**
     * Reset the drawing values before calling PrepareDrawing after changes.
     */
    virtual int ResetDrawing(ArrayPtrVoid *params);
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
