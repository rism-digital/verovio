/////////////////////////////////////////////////////////////////////////////
// Name:        textdirinterface.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TEXT_DIR_INTERFACE_H__
#define __VRV_TEXT_DIR_INTERFACE_H__

#include "atts_shared.h"
#include "interface.h"
#include "vrvdef.h"

namespace vrv {

//----------------------------------------------------------------------------
// TextDirInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for elements implementing a text directive, such
 * as <dir>, <tempo>, or <dynam>
 * It is not an abstract class but should not be instanciated directly.
 */
class TextDirInterface : public Interface, public AttPlacementRelStaff {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    TextDirInterface();
    virtual ~TextDirInterface();
    void Reset() override;
    InterfaceId IsInterface() const override { return INTERFACE_TEXT_DIR; }
    ///@}

    /**
     * Return the number of lines in the text object by counting <lb> children
     */
    int GetNumberOfLines(const Object *object) const;

    /**
     * Check whether one of the children has hAlign attribute set to `alignment` value
     */
    bool AreChildrenAlignedTo(const Object *object, data_HORIZONTALALIGNMENT alignment) const;

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
