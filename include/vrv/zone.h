/////////////////////////////////////////////////////////////////////////////
// Name:        zone.h
// Author:      Juliette Regimbal
// Created:     05/06/2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ZONE_H__
#define __VRV_ZONE_H__

#include <assert.h>

//----------------------------------------------------------------------------

#include "atts_analytical.h"
#include "atts_shared.h"
#include "object.h"
#include "vrvdef.h"

namespace vrv {

//----------------------------------------------------------------------------
// Zone
//----------------------------------------------------------------------------
/**
 * Implements the zone element
 * in MEI
 */

class Zone : public Object, public AttTyped, public AttCoordinated {
public:
    /**
     * @name Constructors, destructors, reset, and class name methods
     */
    ///@{
    Zone();
    virtual ~Zone();
    virtual Object *Clone() const { return new Zone(*this); }
    virtual void Reset();
    ClassId GetClassId() const { return ZONE; }
    ///@}
    void ShiftByXY(int xDiff, int yDiff);
    int GetLogicalUly();
    int GetLogicalLry();

protected:
    //
private:
    //
};

} // namespace vrv

#endif
