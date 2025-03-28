/////////////////////////////////////////////////////////////////////////////
// Name:        supplied.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SUPPLIED_H__
#define __VRV_SUPPLIED_H__

#include "atts_shared.h"
#include "editorial.h"

namespace vrv {

//----------------------------------------------------------------------------
// Supplied
//----------------------------------------------------------------------------

class Supplied : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Supplied();
    virtual ~Supplied();
    Object *Clone() const override { return new Supplied(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "supplied"; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
