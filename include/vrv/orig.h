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
    virtual Object *Clone() const { return new Orig(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Orig"; }
    virtual ClassId GetClassId() const { return ORIG; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
