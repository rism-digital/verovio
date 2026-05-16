/////////////////////////////////////////////////////////////////////////////
// Name:        rdg.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_RDG_H__
#define __VRV_RDG_H__

#include "atts_shared.h"
#include "editorial.h"

namespace vrv {

//----------------------------------------------------------------------------
// Rdg
//----------------------------------------------------------------------------

class Rdg : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Rdg();
    virtual ~Rdg();
    Object *Clone() const override { return new Rdg(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "rdg"; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
