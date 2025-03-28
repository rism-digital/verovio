/////////////////////////////////////////////////////////////////////////////
// Name:        expan.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_EXPAN_H__
#define __VRV_EXPAN_H__

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
    Object *Clone() const override { return new Expan(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "expan"; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
