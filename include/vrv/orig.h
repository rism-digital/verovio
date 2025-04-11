/////////////////////////////////////////////////////////////////////////////
// Name:        orig.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ORIG_H__
#define __VRV_ORIG_H__

#include "atts_shared.h"
#include "editorial.h"

namespace vrv {

//----------------------------------------------------------------------------
// Orig
//----------------------------------------------------------------------------

class Orig : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Orig();
    virtual ~Orig();
    Object *Clone() const override { return new Orig(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "orig"; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
