/////////////////////////////////////////////////////////////////////////////
// Name:        measureelement.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_MEASURE_ELEMENT_H__
#define __VRV_MEASURE_ELEMENT_H__

#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// MeasureElement
//----------------------------------------------------------------------------

/** 
 * This class is an interface for elements with duration, such as notes and rests.
 * It is not an abstract class but should not be instanciate directly.
 */
class MeasureElement: public DocObject
{
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    MeasureElement();
    MeasureElement(std::string classid);
    virtual ~MeasureElement();
    virtual void Reset();
    ///@}
    
private:
    
public:

private:
    
};
    
} // namespace vrv 

#endif
