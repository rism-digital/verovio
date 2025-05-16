/////////////////////////////////////////////////////////////////////////////
// Name:        tuplet.h
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TUPLET_H__
#define __VRV_TUPLET_H__

#include "atts_cmn.h"
#include "atts_shared.h"
#include "atts_visual.h"
#include "layerelement.h"

namespace vrv {

class Note;
class TupletBracket;

// Helper enum classes
enum class MelodicDirection { None, Up, Down };

//----------------------------------------------------------------------------
// Tuplet
//----------------------------------------------------------------------------

class Tuplet : public LayerElement,
               public ObjectListInterface,
               public AttColor,
               public AttDurationRatio,
               public AttNumberPlacement,
               public AttTupletVis {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Tuplet();
    virtual ~Tuplet();
    Object *Clone() const override { return new Tuplet(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "tuplet"; }
    ///@}

    /**
     * Add an element (a note or a rest) to a tuplet.
     * Only Note or Rest elements will be actually added to the beam.
     */
    bool IsSupportedChild(ClassId classId) override;

    /**
     * Overwritten method for tuplet
     */
    void AddChild(Object *object) override;

    /**
     * @name Setter and getter for drawing elements and position
     */
    ///@{
    LayerElement *GetDrawingLeft() { return m_drawingLeft; }
    const LayerElement *GetDrawingLeft() const { return m_drawingLeft; }
    void SetDrawingLeft(LayerElement *drawingLeft) { m_drawingLeft = drawingLeft; }
    LayerElement *GetDrawingRight() { return m_drawingRight; }
    const LayerElement *GetDrawingRight() const { return m_drawingRight; }
    void SetDrawingRight(LayerElement *drawingRight) { m_drawingRight = drawingRight; }
    data_STAFFREL_basic GetDrawingBracketPos() const { return m_drawingBracketPos; }
    void SetDrawingBracketPos(data_STAFFREL_basic bracketPos) { m_drawingBracketPos = bracketPos; }
    data_STAFFREL_basic GetDrawingNumPos() const { return m_drawingNumPos; }
    void SetDrawingNumPos(data_STAFFREL_basic numPos) { m_drawingNumPos = numPos; }
    ///@}

    /**
     * @name Setter and getter for the beam with which the bracket and / or the num is aligned.
     */
    ///@{
    Beam *GetBracketAlignedBeam() { return m_bracketAlignedBeam; }
    const Beam *GetBracketAlignedBeam() const { return m_bracketAlignedBeam; }
    void SetBracketAlignedBeam(Beam *alignedBeam) { m_bracketAlignedBeam = alignedBeam; }
    Beam *GetNumAlignedBeam() { return m_numAlignedBeam; }
    const Beam *GetNumAlignedBeam() const { return m_numAlignedBeam; }
    void SetNumAlignedBeam(Beam *alignedBeam) { m_numAlignedBeam = alignedBeam; }
    ///@}

    /**
     * @name Getter and setter for the inner slurs.
     */
    ///@{
    const std::set<const FloatingCurvePositioner *> &GetInnerSlurs() const { return m_innerSlurs; }
    void AddInnerSlur(const FloatingCurvePositioner *slur) { m_innerSlurs.insert(slur); }
    void ResetInnerSlurs() { m_innerSlurs.clear(); }
    ///@}

    /**
     * Determine the melodic direction
     */
    MelodicDirection GetMelodicDirection() const;

    /**
     * Calculate the position of the bracket and the num looking at the stem direction or at the encoded values (if
     * any). Called in View::DrawTuplet the first time it is called (and not trough a dedicated CalcTuplet functor)
     */
    void CalcDrawingBracketAndNumPos(bool tupletNumHead);

    /**
     * Return the maximum and minimum X positions of the notes in the tuplets.
     * Look at flipped noteheads in chords.
     */
    void GetDrawingLeftRightXRel(int &xRelLeft, int &xRelRight, const Doc *doc) const;

    /**
     * Calculate corresponding cross-staff for the tuplet number if necessary. In case when tuplet is completely
     * cross-staff nothing will be done, as tuplet number should share staff with tuplet in that case
     */
    void CalculateTupletNumCrossStaff(LayerElement *layerElement);

    /**
     * Check whether tuplet number has valid positioning staffwise
     */
    bool HasValidTupletNumPosition(const Staff *preferredStaff, const Staff *otherStaff) const;

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
     * Filter the flat list and keep only Note elements.
     */
    void FilterList(ListOfConstObjects &childList) const override;

private:
    //
public:
    //
private:
    /**
     * The first Chord / Note / Rest in the tuplet.
     * Set in Tuplet::GetDrawingLeftRightXRel from AdjustTupletsXFunctor.
     */
    LayerElement *m_drawingLeft;
    /**
     * The last Chord / Note / Rest in the tuplet.
     * Set in Tuplet::GetDrawingLeftRightXRel from AdjustTupletsXFunctor.
     */
    LayerElement *m_drawingRight;
    /** The calculated drawing position of the bracket set in Tuplet::CalcDrawingBracketAndNumPos  */
    data_STAFFREL_basic m_drawingBracketPos;
    /** The calculated drawing position of the num set in Tuplet::CalcDrawingBracketAndNumPos  */
    data_STAFFREL_basic m_drawingNumPos;
    /** The beam with which the bracket aligns (if any) set in AdjustTupletsXFunctor */
    Beam *m_bracketAlignedBeam;
    /** The beam with which the num aligns (if any) set in AdjustTupletsXFunctor */
    Beam *m_numAlignedBeam;
    /** The slurs avoided by the tuplet, set during drawing */
    std::set<const FloatingCurvePositioner *> m_innerSlurs;
};

} // namespace vrv

#endif
