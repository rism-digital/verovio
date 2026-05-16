/////////////////////////////////////////////////////////////////////////////
// Name:        corr.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CORR_H__
#define __VRV_CORR_H__

#include "atts_shared.h"
#include "editorial.h"

namespace vrv {

//----------------------------------------------------------------------------
// Corr
//----------------------------------------------------------------------------

class Corr : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Corr();
    virtual ~Corr();
    Object *Clone() const override { return new Corr(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "corr"; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
