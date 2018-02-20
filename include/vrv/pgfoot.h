/////////////////////////////////////////////////////////////////////////////
// Name:        pgfoot.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PGFOOT_H__
#define __VRV_PGFOOT_H__

#include "runningelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// PgFoot
//----------------------------------------------------------------------------

/**
 * This class represents an MEI pgFoot.
 */
class PgFoot : public RunningElement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    PgFoot();
    virtual ~PgFoot();
    virtual void Reset();
    virtual std::string GetClassName() const { return "PgFoot"; }
    virtual ClassId GetClassId() const { return PGFOOT; }
    ///@}

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif
