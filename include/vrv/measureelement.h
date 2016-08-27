/////////////////////////////////////////////////////////////////////////////
// Name:        measureelement.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MEASURE_ELEMENT_H__
#define __VRV_MEASURE_ELEMENT_H__

#include "devicecontextbase.h"
#include "floatingobject.h"

namespace vrv {

//----------------------------------------------------------------------------
// MeasureElement
//----------------------------------------------------------------------------

/**
 * This class represents elements appearing within a measure.
 * It is not an abstract class but should not be instanciated directly.
 */
class MeasureElement : public FloatingObject {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    MeasureElement();
    MeasureElement(std::string classid);
    virtual ~MeasureElement();
    virtual void Reset();
    virtual ClassId Is() const { return MEASURE_ELEMENT; }
    ///@}

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif
