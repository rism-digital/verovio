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
    Object *Clone() const override { return new Ref(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "ref"; }
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
