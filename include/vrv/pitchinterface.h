/////////////////////////////////////////////////////////////////////////////
// Name:        pitchinterface.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PITCH_INTERFACE_H__
#define __VRV_PITCH_INTERFACE_H__

#include "atts_shared.h"

namespace vrv {

//----------------------------------------------------------------------------
// PitchInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for elements with pitch, such as notes and neumes.
 * It is not an abstract class but should not be instanciated directly.
 */
class PitchInterface : public Interface, public AttAccidental, public AttOctave, public AttPitch {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    PitchInterface();
    virtual ~PitchInterface();
    virtual void Reset();
    virtual InterfaceId IsInterface() { return INTERFACE_PITCH; };
    ///@}

    /**
     * Interface comparison operator.
     * Checks if the LayerElement has a PitchInterface and compares attributes
     */
    bool HasIdenticalPitchInterface(PitchInterface *otherPitchInterface);

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
