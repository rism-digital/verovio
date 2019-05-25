/////////////////////////////////////////////////////////////////////////////
// Name:        expand.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_EXPAND_H__
#define __VRV_EXPAND_H__

#include "atts_shared.h"
#include "editorial.h"

namespace vrv {

//----------------------------------------------------------------------------
// Expan
//----------------------------------------------------------------------------

class Expan : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Expan();
    virtual ~Expan();
    virtual Object *Clone() const { return new Expan(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Expan"; }
    virtual ClassId GetClassId() const { return EXPAN; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
