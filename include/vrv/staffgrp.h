/////////////////////////////////////////////////////////////////////////////
// Name:        staffgrp.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_STAFFGRP_H__
#define __VRV_STAFFGRP_H__

#include "atts_mei.h"
#include "atts_shared.h"
#include "atts_visual.h"
#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// StaffGrp
//----------------------------------------------------------------------------

/**
 * This class represents a MEI staffGrp.
 * It contains StaffDef objects.
 */
class StaffGrp : public Object,
                 public ObjectListInterface,
                 public AttBasic,
                 public AttLabelled,
                 public AttStaffGroupingSym,
                 public AttStaffGrpVis,
                 public AttTyped {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    StaffGrp();
    virtual ~StaffGrp();
    virtual Object *Clone() const { return new StaffGrp(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "StaffGrp"; }
    virtual ClassId GetClassId() const { return STAFFGRP; }
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

protected:
    /**
     * Filter the flat list and keep only StaffDef elements.
     */
    virtual void FilterList(ListOfObjects *childList);

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
