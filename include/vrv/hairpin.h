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
                public AttColor,
                public AttHairpinLog,
                public AttHairpinVis,
                public AttPlacement,
                public AttVerticalGroup {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Hairpin();
    virtual ~Hairpin();
    virtual Object *Clone() const { return new Hairpin(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Hairpin"; }
    virtual ClassId GetClassId() const { return HAIRPIN; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    virtual TimeSpanningInterface *GetTimeSpanningInterface() { return dynamic_cast<TimeSpanningInterface *>(this); }
    ///@}

    /**
     * @name Getter, setter and checker for the drawing length
     */
    ///@{
    int GetDrawingLength() const { return m_drawingLength; }
    void SetDrawingLength(int length) { m_drawingLength = length; }
    bool HasDrawingLength() const { return (m_drawingLength > 0); }
    ///@}

    int CalcHeight(
        Doc *doc, int staffSize, char spanningType, FloatingPositioner *leftHairpin, FloatingPositioner *rightHaipin);

    /**
     * @name Setter and getter for left and right links
     */
    ///@{
    void SetLeftLink(ControlElement *leftLink);
    ControlElement *GetLeftLink() { return m_leftLink; }
    void SetRightLink(ControlElement *rightLink);
    ControlElement *GetRightLink() { return m_rightLink; }
    ///@}

    /**
     * Get left/right adjustments that needs to be done to the hairpin with set coordinates (leftX, rightX) for it not
     * to overlap with parent measure's barlines
     */
    std::pair<int, int> GetBarlineOverlapAdjustment(int doubleUnit, int leftX, int rightX);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::PrepareFloatingGrps
     */
    virtual int PrepareFloatingGrps(FunctorParams *functoParams);

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

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
