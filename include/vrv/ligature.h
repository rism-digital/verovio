/////////////////////////////////////////////////////////////////////////////
// Name:        ligature.h
// Author:      Donald Byrd
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_LIGATURE_H__
#define __VRV_LIGATURE_H__

#include "atts_mensural.h"
#include "drawinginterface.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Ligature
//----------------------------------------------------------------------------

/**
 * This class represents a collection of notes in the same layer with successive
 * onset times, as used in mensural notation.
 * A ligature is contained in a layer.
 * It contains notes.
 */

class Ligature : public LayerElement, public ObjectListInterface, public AttLigatureVis {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Ligature();
    virtual ~Ligature();
    virtual Object *Clone() const { return new Ligature(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Ligature"; }
    virtual ClassId GetClassId() const { return LIGATURE; }
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    /**
     * Add children (notes or editorial markup)
     */
    virtual bool IsSupportedChild(Object *object);

    /**
     * @name Return shape information about the note ligature
     */
    ///@{
    int GetDrawingNoteShape(Note *note);
    int GetDrawingPreviousNoteShape(Note *note);
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::CalcLigatureNotePos
     */
    virtual int CalcLigatureNotePos(FunctorParams *functorParams);

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

protected:
    /**
     * Clear the m_clusters vector and delete all the objects.
     */
    void ClearClusters();

    /**
     * Filter the flat list and keep only Note elements.
     */
    virtual void FilterList(ArrayOfObjects *childlist);

public:
    /**
     *
     */
    std::vector<int> m_drawingShapes;
};

} // namespace vrv

#endif
