/////////////////////////////////////////////////////////////////////////////
// Name:        abbr.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ABBR_H__
#define __VRV_ABBR_H__

#include "atts_shared.h"
#include "editorial.h"

namespace vrv {

//----------------------------------------------------------------------------
// Abbr
//----------------------------------------------------------------------------

class Abbr : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Abbr();
    virtual ~Abbr();
    Object *Clone() const override { return new Abbr(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "abbr"; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
