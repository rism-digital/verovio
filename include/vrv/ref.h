/////////////////////////////////////////////////////////////////////////////
// Name:        ref.h
// Author:      Laurent Pugin
// Created:     2018/02/21
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_REF_H__
#define __VRV_REF_H__

#include "atts_shared.h"
#include "editorial.h"

namespace vrv {

//----------------------------------------------------------------------------
// Ref
//----------------------------------------------------------------------------

/**
 * This class models the MEI <ref> element.
 */
class Ref : public EditorialElement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Ref();
    virtual ~Ref();
    virtual Object *Clone() const { return new Ref(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Ref"; }
    virtual ClassId GetClassId() const { return REF; }
    ///@}

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
