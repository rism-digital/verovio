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
    virtual void Reset();
    ClassId GetClassId() const { return ZONE; }
    ///@}
    void ShiftByXY(int xDiff, int yDiff);
    int GetLogicalUly();
    int GetLogicalLry();
private:
};

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
    virtual void Reset();
    virtual ClassId GetClassId() const { return FACSIMILE; }
    virtual std::string GetClassName() const { return "facsimile"; }
    ///@}
    virtual void AddChild(Object *object);

    Zone *FindZoneByUuid(std::string zoneId);
    int GetMaxY();
    int GetMaxX();
};

//----------------------------------------------------------------------------
// Surface 
//----------------------------------------------------------------------------
/**
 * Implements the surface element
 * in MEI
 */

class Surface : public Object, public AttTyped, public AttCoordinated {
public:
    /**
     * @name Constructors, destructors, reset, and class name methods
     */
    ///@{
    Surface();
    virtual ~Surface();
    virtual void Reset();
    ClassId GetClassId() const { return SURFACE; }
    ///@}
    virtual void AddChild(Object *object); 

    int GetMaxX();
    int GetMaxY();
private:
};

}
#endif
