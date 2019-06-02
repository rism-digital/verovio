/////////////////////////////////////////////////////////////////////////////
// Name:        tabgrp.h
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TABGRP_H__
#define __VRV_TABGRP_H__

#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// TabGrp
//----------------------------------------------------------------------------

/**
 * This class models the MEI <tabGrp> element.
 */
class TabGrp : public LayerElement, public AttNNumberLike {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    TabGrp();
    virtual ~TabGrp();
    virtual void Reset();
    virtual std::string GetClassName() const { return "TabGrp"; };
    virtual ClassId GetClassId() const { return TABGRP; };
    ///@}

    /**
     * Add an element to a element.
     */
    virtual void AddChild(Object *object);

protected:
    //
private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif
