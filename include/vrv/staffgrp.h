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
#include "grpsym.h"
#include "object.h"
#include "staffdef.h"

namespace vrv {

class Label;
class LabelAbbr;

//----------------------------------------------------------------------------
// StaffGrp
//----------------------------------------------------------------------------

/**
 * This class represents a MEI staffGrp.
 * It contains StaffDef objects.
 */
class StaffGrp : public Object,
                 public ObjectListInterface,
                 public AttBarring,
                 public AttBasic,
                 public AttLabelled,
                 public AttNNumberLike,
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
    Object *Clone() const override { return new StaffGrp(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "staffGrp"; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    bool IsSupportedChild(ClassId classId) override;
    ///@}

    /**
     * Return an order for the given ClassId.
     */
    int GetInsertOrderFor(ClassId classId) const override;

    /**
     * @name Setter and getter of the drawing visible flag
     */
    ///@{
    VisibilityOptimization GetDrawingVisibility() const { return m_drawingVisibility; }
    void SetDrawingVisibility(VisibilityOptimization drawingIsVisible) { m_drawingVisibility = drawingIsVisible; }
    ///@}

    /**
     * Get first and last staffDef of the staff group without visibility optimization set to hidden
     */
    ///@{
    std::pair<StaffDef *, StaffDef *> GetFirstLastStaffDef();
    std::pair<const StaffDef *, const StaffDef *> GetFirstLastStaffDef() const;
    ///@}

    /**
     * Return the maximum staff size in the staffGrp (100 if empty)
     */
    int GetMaxStaffSize() const;

    /**
     * @name Setter and getter of the group symbol
     */
    ///@{
    void SetGroupSymbol(GrpSym *grpSym);
    GrpSym *GetGroupSymbol() { return m_groupSymbol; }
    const GrpSym *GetGroupSymbol() const { return m_groupSymbol; }
    ///@}

    /**
     * @name Methods for checking the presence of label and labelAbbr information and getting them.
     */
    ///@{
    bool HasLabelInfo() const;
    bool HasLabelAbbrInfo() const;

    ///@}

    /**
     * @name Get a copy of the label and labelAbbr.
     * These methods create new objects (heap) that will need to be deleted.
     */
    ///@{
    Label *GetLabel();
    const Label *GetLabel() const;
    Label *GetLabelCopy() const;
    LabelAbbr *GetLabelAbbr();
    const LabelAbbr *GetLabelAbbr() const;
    LabelAbbr *GetLabelAbbrCopy() const;
    ///@}

    /**
     * Set visibility of the group and all of its nested children to SHOW
     */
    void SetEverythingVisible();

    //----------//
    // Functors //
    //----------//

    /**
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

protected:
    /**
     * Filter the flat list and keep only StaffDef elements.
     */
    void FilterList(ListOfConstObjects &childList) const override;

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

    GrpSym *m_groupSymbol;
};

} // namespace vrv

#endif
