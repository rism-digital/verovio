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
    virtual Object *Clone() const { return new Add(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Add"; }
    virtual ClassId GetClassId() const { return ADD; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
