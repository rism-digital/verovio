/////////////////////////////////////////////////////////////////////////////
// Name:        interface.h
// Author:      Laurent Pugin
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_INTERFACE_H__
#define __VRV_INTERFACE_H__

#include <string>

//----------------------------------------------------------------------------

#include "vrvdef.h"

namespace vrv {

class Object;

//----------------------------------------------------------------------------
// Interface
//----------------------------------------------------------------------------

/**
 * This is a base class for regrouping MEI att classes.
 * It is not an abstract class but it should not be instanciated directly.
 * The inherited classes should override the InterfaceId method for returning
 * their own InterfaceId.
 */

class Interface {

public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Interface() {}
    virtual ~Interface() {}
    ///@}

    /**
     * Method for registering an MEI att classes in the interface.
     */
    void RegisterInterfaceAttClass(AttClassId attClassId) { m_interfaceAttClasses.push_back(attClassId); }

    /**
     * Method for obtaining a pointer to the attribute class vector of the interface.
     */
    std::vector<AttClassId> *GetAttClasses() { return &m_interfaceAttClasses; }

    /**
     * Virtual reset method.
     * Needs to be overridden in child classes.
     */
    virtual void Reset() {}

    /**
     * Virtual method returning the InterfaceId of the interface.
     * Needs to be overridden in child classes.
     */
    virtual InterfaceId IsInterface() const { return INTERFACE; }

private:
    /**
     * A vector for storing all the MEI att classes grouped in the interface.
     */
    std::vector<AttClassId> m_interfaceAttClasses;
};

} // namespace vrv

#endif
