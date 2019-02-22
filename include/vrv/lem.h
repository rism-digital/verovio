/////////////////////////////////////////////////////////////////////////////
// Name:        lem.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_LEM_H__
#define __VRV_LEM_H__

#include "atts_shared.h"
#include "editorial.h"

namespace vrv {

//----------------------------------------------------------------------------
// Lem
//----------------------------------------------------------------------------

class Lem : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Lem();
    virtual ~Lem();
    virtual Object *Clone() const { return new Lem(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Lem"; }
    virtual ClassId GetClassId() const { return LEM; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
