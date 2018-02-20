/////////////////////////////////////////////////////////////////////////////
// Name:        areaposinterface.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_AREA_POS_INTERFACE_H__
#define __VRV_AREA_POS_INTERFACE_H__

#include "atts_shared.h"
#include "vrvdef.h"

namespace vrv {

//----------------------------------------------------------------------------
// AreaPosInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for elements implementing vertically and horizontally
 * positionned elements, such * as <rend> and <fig>
 * It is not an abstract class but should not be instanciated directly.
 */
class AreaPosInterface : public Interface, public AttHorizontalAlign, public AttVerticalAlign {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    AreaPosInterface();
    virtual ~AreaPosInterface();
    virtual void Reset();
    virtual InterfaceId IsInterface() { return INTERFACE_AREA_POS; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
