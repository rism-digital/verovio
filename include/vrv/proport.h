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
    virtual Object *Clone() const { return new Proport(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Proport"; }
    virtual ClassId GetClassId() const { return PROPORT; }
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
