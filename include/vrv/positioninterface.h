/////////////////////////////////////////////////////////////////////////////
// Name:        positioninterface.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_POSITION_INTERFACE_H__
#define __VRV_POSITION_INTERFACE_H__

#include "atts_shared.h"

namespace vrv {

//----------------------------------------------------------------------------
// PositionInterface
//----------------------------------------------------------------------------

/** 
 * This class is an interface for elements with a position on the staff, such as rests.
 * It is not an abstract class but should not be instanciate directly.
 * For now, the position is handled in a similar way that for PitchInterface, that is with a pitch and octave. 
 */
class PositionInterface: public Interface,
    public AttStafflocPitched
{
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    PositionInterface();
    virtual ~PositionInterface();
    virtual void Reset();
    virtual InterfaceId IsInterface() { return INTERFACE_POSITION; };
    ///@}
    
    /**
     * Inteface comparison operator. 
     * Check if the LayerElement has a PositionInterface and compare attributes
     */
    bool HasIdenticalPositionInterface(PositionInterface  *otherPitchInterface);

private:
    
public:

private:
    
};
    
} // namespace vrv    

#endif
