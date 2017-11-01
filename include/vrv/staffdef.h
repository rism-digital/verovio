/////////////////////////////////////////////////////////////////////////////
// Name:        staffdef.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_STAFFDEF_H__
#define __VRV_STAFFDEF_H__

#include "atts_mei.h"
#include "atts_shared.h"
#include "scoredef.h"

namespace vrv {

//----------------------------------------------------------------------------
// StaffDef
//----------------------------------------------------------------------------

/**
 * This class represents a MEI staffDef.
 */
class StaffDef : public ScoreDefElement,
                 public StaffDefDrawingInterface,
                 public AttDistances,
                 public AttLabelled,
                 public AttNInteger,
                 public AttNotationType,
                 public AttScalable,
                 public AttStaffDefLog,
                 public AttTransposition {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    StaffDef();
    virtual ~StaffDef();
    virtual Object *Clone() const { return new StaffDef(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "StaffDef"; }
    virtual ClassId GetClassId() const { return STAFFDEF; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    virtual void AddChild(Object *object);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ReplaceDrawingValuesInStaffDef
     */
    virtual int ReplaceDrawingValuesInStaffDef(FunctorParams *functorParams);

    /**
     * See Object::SetStaffDefRedrawFlags
     */
    virtual int SetStaffDefRedrawFlags(FunctorParams *functorParams);

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
