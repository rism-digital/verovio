/////////////////////////////////////////////////////////////////////////////
// Name:        svg.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SVG_H__
#define __VRV_SVG_H__

#include "atts_shared.h"
#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// Svg
//----------------------------------------------------------------------------

/**
 * This class models the MEI <fig>
 */
class Svg : public Object {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    Svg();
    virtual ~Svg();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Svg"; }
    virtual ClassId GetClassId() const { return SVG; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
