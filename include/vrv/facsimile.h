/////////////////////////////////////////////////////////////////////////////
// Name:        facsimile.h
// Author:      Juliette Regimbal
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FACSIMILE_H__
#define __VRV_FACSIMILE_H__

#include <cassert>

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

class Facsimile : public Object, public AttTyped {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Facsimile();
    virtual ~Facsimile();
    Object *Clone() const override { return new Facsimile(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Facsimile"; }
    ///@}
    bool IsSupportedChild(Object *object) override;

    Zone *FindZoneByID(const std::string &zoneId);
    const Zone *FindZoneByID(const std::string &zoneId) const;
    int GetMaxX() const;
    int GetMaxY() const;
};

} // namespace vrv
#endif
