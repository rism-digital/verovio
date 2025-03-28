/////////////////////////////////////////////////////////////////////////////
// Name:        staffdef.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_STAFFDEF_H__
#define __VRV_STAFFDEF_H__

#include "atts_mei.h"
#include "atts_midi.h"
#include "atts_shared.h"
#include "atts_stringtab.h"
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
                 public AttStaffDefVis,
                 public AttStaffDefVisTablature,
                 public AttTimeBase,
                 public AttTransposition {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    StaffDef();
    virtual ~StaffDef();
    Object *Clone() const override { return new StaffDef(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "staffDef"; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    bool IsSupportedChild(ClassId classId) override;

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
     * Return true if the label has layerDef with a label
     */
    bool HasLayerDefWithLabel() const;

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

private:
    //
public:
    //
private:
    /**
     * A flag indicating is the staffDef is visible or not.
     * staffDef is not visible when scoreDef is optimized with dynamic rendering and
     * when the corresponding staves contain only mRest elements.
     * By default the value is OPTIMIZATION_NONE
     */
    VisibilityOptimization m_drawingVisibility;
};

} // namespace vrv

#endif
