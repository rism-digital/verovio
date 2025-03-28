/////////////////////////////////////////////////////////////////////////////
// Name:        hairpin.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_HAIRPIN_H__
#define __VRV_HAIRPIN_H__

#include "atts_cmn.h"
#include "atts_visual.h"
#include "controlelement.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Hairpin
//----------------------------------------------------------------------------

/**
 * This class models the MEI <hairpin> element.
 */
class Hairpin : public ControlElement,
                public TimeSpanningInterface,
                public AttHairpinLog,
                public AttHairpinVis,
                public AttLineRendBase,
                public AttPlacementRelStaff,
                public AttVerticalGroup {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Hairpin();
    virtual ~Hairpin();
    Object *Clone() const override { return new Hairpin(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "hairpin"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    TimePointInterface *GetTimePointInterface() override { return vrv_cast<TimePointInterface *>(this); }
    const TimePointInterface *GetTimePointInterface() const override
    {
        return vrv_cast<const TimePointInterface *>(this);
    }
    TimeSpanningInterface *GetTimeSpanningInterface() override { return vrv_cast<TimeSpanningInterface *>(this); }
    const TimeSpanningInterface *GetTimeSpanningInterface() const override
    {
        return vrv_cast<const TimeSpanningInterface *>(this);
    }
    ///@}

    /**
     * @name Getter, setter and checker for the drawing length
     */
    ///@{
    int GetDrawingLength() const { return m_drawingLength; }
    void SetDrawingLength(int length) { m_drawingLength = length; }
    bool HasDrawingLength() const { return (m_drawingLength > 0); }
    ///@}

    int CalcHeight(const Doc *doc, int staffSize, char spanningType, const FloatingPositioner *leftHairpin,
        const FloatingPositioner *rightHaipin) const;

    /**
     * @name Setter and getter for left and right links
     */
    ///@{
    void SetLeftLink(ControlElement *leftLink);
    ControlElement *GetLeftLink() { return m_leftLink; }
    const ControlElement *GetLeftLink() const { return m_leftLink; }
    void SetRightLink(ControlElement *rightLink);
    ControlElement *GetRightLink() { return m_rightLink; }
    const ControlElement *GetRightLink() const { return m_rightLink; }
    ///@}

    /**
     * Get left/right adjustments that needs to be done to the hairpin with set coordinates (leftX, rightX) for it not
     * to overlap with parent measure's barlines
     */
    std::pair<int, int> GetBarlineOverlapAdjustment(int doubleUnit, int leftX, int rightX, int spanningType) const;

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
    //
private:
    //
public:
    //
private:
    /**
     * A pointer to the possible left link of the Hairpin.
     * This is either another Hairpin or a Dynam that ends / appears at the same position.
     */
    ControlElement *m_leftLink;

    /**
     * A pointer to the possible right link of the Hairpin.
     * This is either another Hairpin or a Dynam that starts / appears at the same position.
     */
    ControlElement *m_rightLink;

    /**
     * The drawing length (uninterrupted) stored for matching height of linked hairpins
     */
    int m_drawingLength;
};

} // namespace vrv

#endif
