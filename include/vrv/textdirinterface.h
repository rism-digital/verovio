/////////////////////////////////////////////////////////////////////////////
// Name:        textdirinterface.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TEXT_DIR_INTERFACE_H__
#define __VRV_TEXT_DIR_INTERFACE_H__

#include "atts_shared.h"
#include "vrvdef.h"

namespace vrv {

//----------------------------------------------------------------------------
// TextDirInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for elements implementing a text directive, such
 * as <dir>, <tempo>, or <dynam>
 * It is not an abstract class but should not be instanciate directly.
 */
class TextDirInterface : public Interface, public AttCommon, public AttPlacement, public AttStaffident {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    TextDirInterface();
    virtual ~TextDirInterface();
    virtual void Reset();
    virtual InterfaceId IsInterface() { return INTERFACE_TEXT_DIR; };
    ///@}

protected:
private:
public:
private:
};

} // namespace vrv

#endif
