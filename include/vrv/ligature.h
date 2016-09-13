/////////////////////////////////////////////////////////////////////////////
// Name:        ligature.h
// Author:      Donald Byrd
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_LIGATURE_H__
#define __VRV_LIGATURE_H__

#include "atts_shared.h"
#include "drawinginterface.h"
#include "durationinterface.h"
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

class Ligature : public LayerElement,
                 public ObjectListInterface,
                 public StemmedDrawingInterface,
                 public DurationInterface,
                 public AttCommon,
                 public AttStems,
                 public AttStemsCmn,
                 public AttTiepresent {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Ligature();
    virtual ~Ligature();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Ligature"; }
    virtual ClassId Is() const { return LIGATURE; }
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    /**
     * Add an element (only notes are supported) to a ligature.
     */
    virtual void AddChild(Object *object);

    virtual void FilterList(ListOfObjects *childlist);

    /**
     * Returns list of notes that have accidentals
     */
    void ResetAccidList();

    /**
     * Return information about the note's position in the ligature ??
     */
    ///@{
    /** Return 0 if the note is the middle note, -1 if below it and 1 if above */
    int PositionInLigature(Note *note);
    ///@}

    /**
     * Prepares a 2D grid of booleans to track where accidentals are placed. ??
     * Further documentation is in chord.cpp comments. ??
     */
    void ResetAccidSpace(int fullUnit);

    /**
     * @name Set and get the stem direction and stem positions
     * The methods are overriding the interface because we want to apply it to child notes
     */
    ///@{
    virtual void SetDrawingStemDir(data_STEMDIRECTION stemDir);
    virtual void SetDrawingStemStart(Point stemStart);
    virtual void SetDrawingStemEnd(Point stemEnd);
    ///@}

    //----------//
    // Functors //
    //----------//

protected:
    /**
     * Clear the m_clusters vector and delete all the objects.
     */
    void ClearClusters();

public:
};

} // namespace vrv

#endif