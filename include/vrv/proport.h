/////////////////////////////////////////////////////////////////////////////
// Name:        proport.h
// Author:      Donald Byrd
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PROPORT_H__
#define __VRV_PROPORT_H__

#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Proport
//----------------------------------------------------------------------------

/**
 * This class models the MEI <proport> element.
 */
class Proport : public LayerElement, public AttDurationRatio {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes.
     */
    ///@{
    Proport();
    virtual ~Proport();
    Object *Clone() const override { return new Proport(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Proport"; }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
