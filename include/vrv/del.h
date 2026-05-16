/////////////////////////////////////////////////////////////////////////////
// Name:        del.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_DEL_H__
#define __VRV_DEL_H__

#include "atts_shared.h"
#include "editorial.h"

namespace vrv {

//----------------------------------------------------------------------------
// Del
//----------------------------------------------------------------------------

class Del : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Del();
    virtual ~Del();
    Object *Clone() const override { return new Del(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "del"; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
