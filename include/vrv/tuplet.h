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
    virtual void Reset();
    virtual std::string GetClassName() const { return "Tuplet"; }
    virtual ClassId GetClassId() const { return TUPLET; }
    ///@}

    /**
     * Add an element (a note or a rest) to a tuplet.
     * Only Note or Rest elements will be actually added to the beam.
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
    void SetDrawingBracketPos(data_STAFFREL_basic drawingBracketPos) { m_drawingBracketPos = drawingBracketPos; }
    ///@}
                   
    Beam *GetAlignedBeam() { return m_alignedBeam; }

    /**
     *
     */
    data_STAFFREL_basic CalcDrawingBracketPos();
                   
    /**
     * Return the maximum and minimum X positions of the notes in the tuplets
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
    virtual void FilterList(ListOfObjects *childList);

private:
    //
public:
    //
private:
    /** */
    LayerElement *m_drawingLeft;
    /** */
    LayerElement *m_drawingRight;
    /** */
    data_STAFFREL_basic m_drawingBracketPos;
    /** */
    Beam *m_alignedBeam;
};

} // namespace vrv

#endif
