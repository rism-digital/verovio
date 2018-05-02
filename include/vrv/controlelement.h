/////////////////////////////////////////////////////////////////////////////
// Name:        controlelement.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CONTROL_ELEMENT_H__
#define __VRV_CONTROL_ELEMENT_H__

#include "atts_shared.h"
#include "devicecontextbase.h"
#include "floatingobject.h"

namespace vrv {

//----------------------------------------------------------------------------
// ControlElement
//----------------------------------------------------------------------------

/**
 * This class represents elements appearing within a measure.
 * It is not an abstract class but should not be instanciated directly.
 */
class ControlElement : public FloatingObject, public AttLabelled, public AttTyped {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    ControlElement();
    ControlElement(std::string classid);
    virtual ~ControlElement();
    virtual void Reset();
    virtual ClassId GetClassId() const { return CONTROL_ELEMENT; }
    ///@}

    /**
     * Check if the ControlElement has a Rend child and return its @halign equivalent (if any)
     */
    data_HORIZONTALALIGNMENT GetChildRendAlignment();

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::AdjustXOverflow
     */
    virtual int AdjustXOverflow(FunctorParams *functorParams);

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif
