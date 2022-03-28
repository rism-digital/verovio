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
    std::string GetClassName() const override { return "Tuplet"; }
    ///@}

    /**
     * Add an element (a note or a rest) to a tuplet.
     * Only Note or Rest elements will be actually added to the beam.
     */
    bool IsSupportedChild(Object *object) override;

    /**
     * Overwritten method for tuplet
     */
    void AddChild(Object *object) override;

    /**
     * @name Setter and getter for darwing elements and position
     */
    ///@{
    LayerElement *GetDrawingLeft() { return m_drawingLeft; }
    void SetDrawingLeft(LayerElement *drawingLeft) { m_drawingLeft = drawingLeft; }
    LayerElement *GetDrawingRight() { return m_drawingRight; }
    void SetDrawingRight(LayerElement *drawingRight) { m_drawingRight = drawingRight; }
    data_STAFFREL_basic GetDrawingBracketPos() { return m_drawingBracketPos; }
    data_STAFFREL_basic GetDrawingNumPos() { return m_drawingNumPos; }
    ///@}

    /**
     * @name Getter for the beam with which the bracket and / or the num is aligned.
     */
    ///@{
    Beam *GetBracketAlignedBeam() { return m_bracketAlignedBeam; }
    Beam *GetNumAlignedBeam() { return m_numAlignedBeam; }
    ///@}

    /**
     * Calculate the position of the bracket and the num looking at the stem direction or at the encoded values (if
     * any). Called in View::DrawTuplet the first time it is called (and not trough a dedicated CalcTuplet functor)
     */
    void CalcDrawingBracketAndNumPos(bool tupletNumHead);

    /**
     * Return the maximum and minimum X positions of the notes in the tuplets.
     * Look at flipped noteheads in chords.
     */
    void GetDrawingLeftRightXRel(int &XRelLeft, int &XRelRight, Doc *doc) const;

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::PrepareLayerElementParts
     */
    int PrepareLayerElementParts(FunctorParams *functorParams) override;

    /**
     * See Object::AdjustTupletsX
     */
    int AdjustTupletsX(FunctorParams *functorParams) override;

    /**
     * See Object::AdjustTupletsY
     */
    int AdjustTupletsY(FunctorParams *functorParams) override;

    /**
     * See Object::ResetHorizontalAlignment
     */
    int ResetHorizontalAlignment(FunctorParams *functorParams) override;

    /**
     * See Object::ResetData
     */
    int ResetData(FunctorParams *functorParams) override;

protected:
    /**
     * Filter the flat list and keep only Note elements.
     */
    void FilterList(ArrayOfConstObjects &childList) override;

private:
    /**
     * Adjust tuplet relative positioning based on possible overlaps
     */
    void AdjustTupletBracketY(Doc *doc, Staff *staff, int staffSize);

    /**
     * Adjust tuplet relative positioning based on possible overlaps
     */
    void AdjustTupletNumY(Doc *doc, Staff *staff, int staffSize);

    /**
     * Calculate corresponding cross-staff for the tuplet number if necessary. In case when tuplet is completely
     * cross-staff nothing will be done, as tuplet number should share staff with tuplet in that case
     */
    void CalculateTupletNumCrossStaff(LayerElement *layerElement);

    /**
     * Check whether tuplet number has valid postioning staffwise
     */
    bool HasValidTupletNumPosition(Staff *preferredStaff, Staff *otherStaff);

public:
    //
private:
    /**
     * The first Chord / Note / Rest in the tuplet.
     * Set in Tuplet::GetDrawingLeftRightXRel from Tuplet::AdjustTupletsX.
     */
    LayerElement *m_drawingLeft;
    /**
     * The last Chord / Note / Rest in the tuplet.
     * Set in Tuplet::GetDrawingLeftRightXRel from Tuplet::AdjustTupletsX.
     */
    LayerElement *m_drawingRight;
    /** The calcultated drawing position of the bracket set in Tuplet::CalcDrawingBracketAndNumPos  */
    data_STAFFREL_basic m_drawingBracketPos;
    /** The calcultated drawing position of the num set in Tuplet::CalcDrawingBracketAndNumPos  */
    data_STAFFREL_basic m_drawingNumPos;
    /** The beam with which the bracket aligns (in any) set in Tuplet::AdjustTupletsX */
    Beam *m_bracketAlignedBeam;
    /** The beam with which the num aligns (in any) set in Tuplet::AdjustTupletsX */
    Beam *m_numAlignedBeam;
};

} // namespace vrv

#endif
