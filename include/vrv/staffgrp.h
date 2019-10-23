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

    /**
     * @name Setter and getter of the drawing visible flag
     */
    ///@{
    VisibilityOptimization GetDrawingVisibility() const { return m_drawingVisibility; }
    void SetDrawingVisibility(VisibilityOptimization drawingIsVisible) { m_drawingVisibility = drawingIsVisible; }
    ///@}

    /**
     * Return the maximum staff size in the staffGrp (100 if empty)
     */
    int GetMaxStaffSize();

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::OptimizeScoreDef
     */
    virtual int OptimizeScoreDefEnd(FunctorParams *functorParams);

protected:
    /**
     * Filter the flat list and keep only StaffDef elements.
     */
    virtual void FilterList(ArrayOfObjects *childList);

private:
    //
public:
    //
private:
    /**
     * A flag indicating is the staffGrp is visible or not.
     * staffGrp is not visible when scoreDef is optimized with dynamic rendering and
     * when all its content is not visible.
     * By default the value is OPTIMIZATION_NONE
     */
    VisibilityOptimization m_drawingVisibility;
};

} // namespace vrv

#endif
