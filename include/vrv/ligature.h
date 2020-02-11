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

class Ligature : public LayerElement, public ObjectListInterface, public AttLigatureLog {
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
    virtual void AddChild(Object *object);

    /**
     * Return information about the note's position in the ligature
     * Assume that the note is in the ligature and does not check for that.
     */
    int PositionInLigature(Note *note);

    /**
     * @name Return the first and last note in the ligature (NULL if empty)
     */
    ///@{
    Note *GetFirstNote();
    Note *GetLastNote();
    ///@}

    //----------//
    // Functors //
    //----------//

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
};

} // namespace vrv

#endif
