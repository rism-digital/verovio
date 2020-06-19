/////////////////////////////////////////////////////////////////////////////
// Name:        fb.h
// Author:      Rodolfo Zitellini
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FB_H__
#define __VRV_FB_H__

#include "atts_shared.h"
#include "textelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Fb (figured bass)
//----------------------------------------------------------------------------

/**
 * This class models the MEI <fb> element.
 */
class Fb : public Object {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    Fb();
    virtual ~Fb();
    virtual Object *Clone() const { return new Fb(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Fb"; }
    virtual ClassId GetClassId() const { return FB; }
    ///@}

    /**
     * Add an element (f) to an fb.
     * Only supported elements will be actually added to the child list.
     */
    virtual bool IsSupportedChild(Object *object);

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
