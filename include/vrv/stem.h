/////////////////////////////////////////////////////////////////////////////
// Name:        stem.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_STEM_H__
#define __VRV_STEM_H__

#include "atts_cmn.h"
#include "atts_mensural.h"
#include "atts_visual.h"
#include "layerelement.h"

namespace vrv {

class Flag;

//----------------------------------------------------------------------------
// Stem
//----------------------------------------------------------------------------

/**
 * This class models a stem as a layer element part and as MEI <stem> element.
 */
class Stem : public LayerElement, public AttGraced, public AttStemVis, public AttVisibility {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Stem();
    virtual ~Stem();
    Object *Clone() const override { return new Stem(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "stem"; }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    /**
     * Add an element (only flag supported) to a stem.
     */
    bool IsSupportedChild(ClassId classId) override;

    /**
     * Fill the attributes from the AttStems attribute of the parent note/chord
     */
    void FillAttributes(const AttStems &attSource);

    /**
     * @name Setter and getter for the drawing stem direction, length and modifier
     */
    ///@{
    data_STEMDIRECTION GetDrawingStemDir() const { return m_drawingStemDir; }
    void SetDrawingStemDir(data_STEMDIRECTION drawingStemDir) { m_drawingStemDir = drawingStemDir; }
    int GetDrawingStemLen() const { return m_drawingStemLen; }
    void SetDrawingStemLen(int drawingStemLen) { m_drawingStemLen = drawingStemLen; }
    // Since Stem does not inherit from AttStems we override LayerElement::GetDrawingStemMod()
    data_STEMMODIFIER GetDrawingStemMod() const override { return m_drawingStemMod; }
    bool HasDrawingStemMod() const { return (m_drawingStemMod != STEMMODIFIER_NONE); }
    void SetDrawingStemMod(data_STEMMODIFIER mod) { m_drawingStemMod = mod; }
    int GetDrawingStemAdjust() const { return m_drawingStemAdjust; }
    void SetDrawingStemAdjust(int drawingStemAdjust) { m_drawingStemAdjust = drawingStemAdjust; }
    int GetStemModRelY() const { return m_stemModRelY; }
    ///@}

    /**
     * @name Setter and getter of the virtual flag
     */
    ///@{
    bool IsVirtual() const { return m_isVirtual; }
    void IsVirtual(bool isVirtual) { m_isVirtual = isVirtual; }
    ///@}

    /**
     * Helper to adjust overlapping layers for stems
     */
    int CompareToElementPosition(const Doc *doc, const LayerElement *otherElement, int margin) const;

    /**
     * Helper to calculate stem modifier relative Y rel and required adjustment for stem length
     */
    int CalculateStemModAdjustment(const Doc *doc, const Staff *staff, int flagOffset = 0);

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
    /**
     * Helper to adjust length of stem based on presence of slashes
     */
    int AdjustSlashes(const Doc *doc, const Staff *staff, int flagOffset) const;

    /**
     * Helper to calculate relative position for the stem modifier
     */
    void CalculateStemModRelY(const Doc *doc, const Staff *staff);

public:
    //
private:
    /**
     * The drawing direction of the stem
     */
    data_STEMDIRECTION m_drawingStemDir;
    /**
     * The drawing length of stem
     */
    int m_drawingStemLen;
    /**
     * The drawing modifier of the stem
     */
    data_STEMMODIFIER m_drawingStemMod;
    /**
     * Relative Y position for the stem modifier
     */
    int m_stemModRelY;
    /**
     * The adjustment of the drawing stem length (used with french style of beams)
     */
    int m_drawingStemAdjust;
    /**
     * A flag indicating if a stem if virtual and should never be rendered.
     * Virtual stems are added to whole notes (and longer) for position calculation and
     * for supporting MEI @stem.mod
     */
    bool m_isVirtual;
};

} // namespace vrv

#endif //__VRV_STEM_H__
