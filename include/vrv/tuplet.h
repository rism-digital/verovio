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
    virtual Object *Clone() const { return new Tuplet(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Tuplet"; }
    virtual ClassId GetClassId() const { return TUPLET; }
    ///@}

    /**
     * Add an element (a note or a rest) to a tuplet.
     * Only Note or Rest elements will be actually added to the beam.
     */
    virtual bool IsSupportedChild(Object *object);

    /**
     * Overwritten method for tuplet
     */
    virtual void AddChild(Object *object);

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
    void CalcDrawingBracketAndNumPos();

    /**
     * Return the maximum and minimum X positions of the notes in the tuplets.
     * Look at flipped noteheads in chords.
     */
    void GetDrawingLeftRightXRel(int &XRelLeft, int &XRelRight, Doc *doc);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::PrepareLayerElementParts
     */
    virtual int PrepareLayerElementParts(FunctorParams *functorParams);

    /**
     * See Object::AdjustTupletsX
     */
    virtual int AdjustTupletsX(FunctorParams *);

    /**
     * See Object::AdjustTupletsY
     */
    virtual int AdjustTupletsY(FunctorParams *);

    /**
     * See Object::ResetHorizontalAlignment
     */
    virtual int ResetHorizontalAlignment(FunctorParams *functorParams);

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

protected:
    /**
     * Filter the flat list and keep only Note elements.
     */
    virtual void FilterList(ArrayOfObjects *childList);

private:
    //
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
