/////////////////////////////////////////////////////////////////////////////
// Name:        restore.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_RESTORE_H__
#define __VRV_RESTORE_H__

#include "atts_shared.h"
#include "editorial.h"

namespace vrv {

//----------------------------------------------------------------------------
// Restore
//----------------------------------------------------------------------------

class Restore : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Restore();
    virtual ~Restore();
    virtual Object *Clone() const { return new Restore(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Restore"; }
    virtual ClassId GetClassId() const { return RESTORE; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
