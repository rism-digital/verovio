/////////////////////////////////////////////////////////////////////////////
// Name:        damage.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_DAMAGE_H__
#define __VRV_DAMAGE_H__

#include "atts_shared.h"
#include "editorial.h"

namespace vrv {

//----------------------------------------------------------------------------
// Damage
//----------------------------------------------------------------------------

class Damage : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Damage();
    virtual ~Damage();
    virtual Object *Clone() const { return new Damage(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Damage"; }
    virtual ClassId GetClassId() const { return DAMAGE; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
