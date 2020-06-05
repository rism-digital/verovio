/////////////////////////////////////////////////////////////////////////////
// Name:        facsimile.h
// Author:      Juliette Regimbal
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FACSIMILE_H__
#define __VRV_FACSIMILE_H__

#include <assert.h>

//----------------------------------------------------------------------------

#include "atts_analytical.h"
#include "atts_shared.h"
#include "object.h"
#include "view.h"
#include "vrvdef.h"

namespace vrv {

//----------------------------------------------------------------------------
// Facsimile
//----------------------------------------------------------------------------
/**
 * Implements the facsimile element
 * in MEI
 */

class Facsimile : public Object {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Facsimile();
    virtual ~Facsimile();
    virtual Object *Clone() const { return new Facsimile(*this); }
    virtual void Reset();
    virtual ClassId GetClassId() const { return FACSIMILE; }
    virtual std::string GetClassName() const { return "facsimile"; }
    ///@}
    virtual bool IsSupportedChild(Object *object);

    Zone *FindZoneByUuid(std::string zoneId);
    int GetMaxY();
    int GetMaxX();
};

} // namespace vrv
#endif
