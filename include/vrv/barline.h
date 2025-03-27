/////////////////////////////////////////////////////////////////////////////
// Name:        barline.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_BARLINE_H__
#define __VRV_BARLINE_H__

#include "atts_shared.h"
#include "atts_visual.h"
#include "layerelement.h"

namespace vrv {

class StaffDef;
class StaffGrp;

enum class BarLinePosition { None, Left, Right };

//----------------------------------------------------------------------------
// BarLine
//----------------------------------------------------------------------------

/**
 * This class models the MEI <barLine> element.
 */
class BarLine : public LayerElement,
                public AttBarLineLog,
                public AttBarLineVis,
                public AttColor,
                public AttNNumberLike,
                public AttVisibility {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes.
     */
    ///@{
    BarLine();
    BarLine(ClassId classId);
    virtual ~BarLine();
    Object *Clone() const override { return new BarLine(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "barLine"; }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    /**
     * Use to set the alignment for the Measure BarLine members.
     * This is as special case where we need to add to the measure aligner.
     */
    bool SetAlignment(Alignment *alignment);

    /*
     * Return true if the barLine type requires repetition dots to be drawn.
     */
    bool HasRepetitionDots() const;

    /**
     * @name Get and set the position
     */
    ///@{
    BarLinePosition GetPosition() const { return m_position; }
    void SetPosition(BarLinePosition position) { m_position = position; }
    ///@}

    /**
     * Check if the barline is drawn through
     */
    bool IsDrawnThrough(const StaffGrp *staffGrp) const;

    /**
     * @name Collect AttBarring attributes
     * @return First entry is true if the attribute was found, second entry contains the value
     */
    ///@{
    std::pair<bool, double> GetLengthFromContext(const StaffDef *staffDef) const;
    std::pair<bool, data_BARMETHOD> GetMethodFromContext(const StaffDef *staffDef) const;
    std::pair<bool, int> GetPlaceFromContext(const StaffDef *staffDef) const;
    ///@}

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
    /** The barline position (left/right) */
    BarLinePosition m_position;
};

} // namespace vrv

#endif
