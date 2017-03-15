/////////////////////////////////////////////////////////////////////////////
// Name:        chord.h
// Author:      Andrew Horwitz
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CHORD_H__
#define __VRV_CHORD_H__

#include "atts_shared.h"
#include "drawinginterface.h"
#include "durationinterface.h"
#include "layerelement.h"

namespace vrv {

#define ledgermin(a, b) (((a) < (b)) ? (a) : (b))
#define ledgermax(a, b) (((a) > (b)) ? (a) : (b))

//----------------------------------------------------------------------------
// Chord
//----------------------------------------------------------------------------

/**
 * This class represents a collection of notes in the same layer with the
 * same onset time.
 * A chord is contained in a layer.
 * It contains notes.
 */

class Chord : public LayerElement,
              public ObjectListInterface,
              public DrawingListInterface,
              public StemmedDrawingInterface,
              public DurationInterface,
              public AttColor,
              public AttGraced,
              public AttRelativesize,
              public AttTiepresent,
              public AttVisibility {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Chord();
    virtual ~Chord();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Chord"; }
    virtual ClassId GetClassId() const { return CHORD; }
    ///@}

    virtual DurationInterface *GetDurationInterface() { return dynamic_cast<DurationInterface *>(this); }
    virtual StemmedDrawingInterface *GetStemmedDrawingInterface()
    {
        return dynamic_cast<StemmedDrawingInterface *>(this);
    }

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    /**
     * Add an element (only note supported) to a chord.
     */
    virtual void AddChild(Object *object);

    virtual void FilterList(ListOfObjects *childlist);

    void GetYExtremes(int &yMax, int &yMin);
                  
    void GetCrossStaffExtemes(Staff *staffAbove, Staff *staffBelow);

    /**
     * Returns list of notes that have accidentals
     */
    void ResetAccidList();

    /**
     * Return information about a note's position in the chord
     */
    ///@{
    /** Return 0 if the note is the middle note, -1 if below it and 1 if above */
    int PositionInChord(Note *note);
    ///@}

    /**
     * Prepares a 2D grid of booleans to track where accidentals are placed.
     * Further documentation is in chord.cpp comments.
     */
    void ResetAccidSpace(int fullUnit);

    /**
     * @name Set and get stem direction and stem positions
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

    /**
     * See Object::PrepareTieAttr
     */
    virtual int PrepareTieAttr(FunctorParams *functorParams);
    virtual int PrepareTieAttrEnd(FunctorParams *functorParams);

protected:
    /**
     * Clear the m_clusters vector and delete all the objects.
     */
    void ClearClusters() const;

public:
    mutable std::list<ChordCluster *> m_clusters;

    /**
     * Number of ledger lines for the chord where:
     * Staff * is each staff for which the chord has notes and maps to:
     * a four char vector acting as a 2D array (2x2) where:
     * [0][x] is single-length, [1][x] is double-length
     * [x][0] is below staff, [x][1] is above staff
     */
    MapOfLedgerLineFlags m_drawingLedgerLines;

    /**
     * Positions of dots in the chord to avoid overlapping
     */
    std::list<int> m_dots;

    /**
     * Variables related to preventing overlapping in the X dimension for accidentals
     */
    std::vector<Note *> m_accidList;
    std::vector<std::vector<bool> > m_accidSpace;
    int m_accidSpaceTop, m_accidSpaceBot, m_accidSpaceLeft;
};

} // namespace vrv

#endif
