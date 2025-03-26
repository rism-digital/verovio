/////////////////////////////////////////////////////////////////////////////
// Name:        add.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADD_H__
#define __VRV_ADD_H__

#include "atts_shared.h"
#include "editorial.h"

namespace vrv {

//----------------------------------------------------------------------------
// Add
//----------------------------------------------------------------------------

class Add : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Add();
    virtual ~Add();
    Object *Clone() const override { return new Add(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "add"; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
