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
    Object *Clone() const override { return new Damage(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "damage"; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
