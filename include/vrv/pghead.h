/////////////////////////////////////////////////////////////////////////////
// Name:        pghead.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PGHEAD_H__
#define __VRV_PGHEAD_H__

#include "floatingobject.h"

namespace vrv {


//----------------------------------------------------------------------------
// PgHead
//----------------------------------------------------------------------------

/**
 * This class represents an MEI pgHead. 
 */
class PgHead : public FloatingObject {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    PgHead();
    virtual ~PgHead();
    virtual void Reset();
    virtual std::string GetClassName() const { return "PgHead"; }
    virtual ClassId GetClassId() const { return PGHEAD; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    virtual void AddChild(Object *object);
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
