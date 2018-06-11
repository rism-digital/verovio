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
    virtual void Reset();
    virtual ClassId GetClassId() const { return FACSIMILE; }
    virtual std::string GetClassName() const { return "facsimile"; }
    ///@}
    virtual void AddChild(Object *object);
};

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
private:
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
    
    void AddZone(Zone *zone) { m_zones.emplace(zone->GetUuid(), zone); }
    std::map<std::string, Zone*>::iterator GetZoneBegin() { return m_zones.begin(); }
    std::map<std::string, Zone*>::iterator GetZoneEnd() { return m_zones.end(); } 
protected:
    std::map<std::string, Zone*> m_zones;
private:
};

}
#endif
