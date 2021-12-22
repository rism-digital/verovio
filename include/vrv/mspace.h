/////////////////////////////////////////////////////////////////////////////
// Name:        mspace.h
// Author:      Klaus Rettinghaus
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MSPACE_H__
#define __VRV_MSPACE_H__

#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// MSpace
//----------------------------------------------------------------------------

/**
 * This class models the MEI <space> element.
 */
class MSpace : public LayerElement {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    MSpace();
    virtual ~MSpace();
    Object *Clone() const override { return new MSpace(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "MSpace"; }
    ///@}

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
