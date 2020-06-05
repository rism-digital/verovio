/////////////////////////////////////////////////////////////////////////////
// Name:        surface.h
// Author:      Juliette Regimbal
// Created:     05/06/2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SURFACE_H__
#define __VRV_SURFACE_H__

#include <assert.h>

//----------------------------------------------------------------------------

#include "atts_analytical.h"
#include "atts_shared.h"
#include "object.h"
#include "vrvdef.h"

namespace vrv {

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
    virtual Object *Clone() const { return new Surface(*this); }
    virtual void Reset();
    ClassId GetClassId() const { return SURFACE; }
    ///@}
    virtual bool IsSupportedChild(Object *object);

    int GetMaxX();
    int GetMaxY();

protected:
    //
private:
    //
};

} // namespace vrv

#endif
