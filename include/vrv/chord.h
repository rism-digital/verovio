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

class Dots;
class StaffAlignment;

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
              public AttCue,
              public AttGraced,
              public AttStems,
              public AttStemsCmn,
              public AttTiePresent,
              public AttVisibility {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Chord();
    virtual ~Chord();
    virtual Object *Clone() const { return new Chord(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Chord"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual DurationInterface *GetDurationInterface() { return dynamic_cast<DurationInterface *>(this); }
    virtual StemmedDrawingInterface *GetStemmedDrawingInterface()
    {
        return dynamic_cast<StemmedDrawingInterface *>(this);
    }
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    /**
     * Add an element (only note supported) to a chord.
     */
    virtual bool IsSupportedChild(Object *object);

    /**
     * Overwritten method for chord
     */
    virtual void AddChild(Object *object);

    /**
     * Return the maximum and minimum Y positions of the notes in the chord
     */
    void GetYExtremes(int &yMax, int &yMin);

    /**
     * Return the top or bottom note or their Y position
     */
    ///@{
    Note *GetTopNote();
    Note *GetBottomNote();
    int GetYTop();
    int GetYBottom();
    ///@}

    /**
     * Return min or max note X position
     */
    ///@{
    int GetXMin();
    int GetXMax();
    ///@}

    /**
     * Return the cross staff above or below (if  any).
     */
    void GetCrossStaffExtremes(
        Staff *&staffAbove, Staff *&staffBelow, Layer **layerAbove = NULL, Layer **layerBelow = NULL);

    /**
     * Return true if the chord has some cross staff notes.
     */
    bool HasCrossStaff();

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
     * Get the stem up / stem down attachment point.
     * If necessary look at the glyph anchor (if any).
     */
    ///@{
    virtual Point GetStemUpSE(Doc *doc, int staffSize, bool isCueSize);
    virtual Point GetStemDownNW(Doc *doc, int staffSize, bool isCueSize);
    virtual int CalcStemLenInThirdUnits(Staff *staff, data_STEMDIRECTION stemDir);
    ///@}

    /**
     * Check if the chord or one of its children is visible
     */
    bool IsVisible();

    /**
     * Return true if the chord has at least one note with a @dots > 0
     */
    bool HasNoteWithDots();

    /**
     * Helper to adjust overlapping layers for chords
     * Returns the shift of the adjustment
     */
    virtual int AdjustOverlappingLayers(
        Doc *doc, const std::vector<LayerElement *> &otherElements, bool areDotsAdjusted, bool &isUnison);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::AdjustCrossStaffYPos
     */
    virtual int AdjustCrossStaffYPos(FunctorParams *functorParams);

    /**
     * See Object::AdjustArtic
     */
    virtual int AdjustArtic(FunctorParams *functorParams);

    /**
     * See Object::ConvertMarkupAnalytical
     */
    ///@{
    virtual int ConvertMarkupAnalytical(FunctorParams *functorParams);
    virtual int ConvertMarkupAnalyticalEnd(FunctorParams *functorParams);
    ///@}

    /**
     * See Object::CalcArtic
     */
    virtual int CalcArtic(FunctorParams *functorParams);

    /**
     * See Object::CalcStem
     */
    virtual int CalcStem(FunctorParams *functorParams);

    /**
     * See Object::CalcDots
     */
    virtual int CalcDots(FunctorParams *functorParams);

    /**
     * See Object::PrepareLayerElementParts
     */
    virtual int PrepareLayerElementParts(FunctorParams *functorParams);

    /**
     * See Object::CalcOnsetOffsetEnd
     */
    virtual int CalcOnsetOffsetEnd(FunctorParams *functorParams);

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * See Object::AdjustCrossStaffContent
     */
    virtual int AdjustCrossStaffContent(FunctorParams *functorParams);

protected:
    /**
     * The note locations w.r.t. each staff
     */
    virtual MapOfNoteLocs CalcNoteLocations();

    /**
     * The dot locations w.r.t. each staff
     * Since dots for notes on staff lines can be shifted upwards or downwards, there are two choices: primary and
     * secondary
     */
    virtual MapOfDotLocs CalcDotLocations(int layerCount, bool primary);

    /**
     * Clear the m_clusters vector and delete all the objects.
     */
    void ClearClusters() const;

    /**
     * Filter the flat list and keep only Note elements.
     */
    virtual void FilterList(ArrayOfObjects *childlist);

public:
    mutable std::list<ChordCluster *> m_clusters;

    /**
     * Positions of dots in the chord to avoid overlapping
     */
    std::list<int> m_dots;
};

} // namespace vrv

#endif
