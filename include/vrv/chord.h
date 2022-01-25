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
    Object *Clone() const override { return new Chord(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Chord"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    DurationInterface *GetDurationInterface() override { return dynamic_cast<DurationInterface *>(this); }
    StemmedDrawingInterface *GetStemmedDrawingInterface() override
    {
        return dynamic_cast<StemmedDrawingInterface *>(this);
    }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    /**
     * Add an element (only note supported) to a chord.
     */
    bool IsSupportedChild(Object *object) override;

    /**
     * Overwritten method for chord
     */
    void AddChild(Object *object) override;

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
    Point GetStemUpSE(Doc *doc, int staffSize, bool isCueSize) override;
    Point GetStemDownNW(Doc *doc, int staffSize, bool isCueSize) override;
    int CalcStemLenInThirdUnits(Staff *staff, data_STEMDIRECTION stemDir) override;
    ///@}

    /**
     * Check if the chord or one of its children is visible
     */
    bool IsVisible();

    /**
     * Return true if the chord has two notes with 1 diatonic step difference in the specific staff
     */
    bool HasAdjacentNotesInStaff(Staff *staff);

    /**
     * Return true if the chord has at least one note with a @dots > 0
     */
    bool HasNoteWithDots();

    /**
     * Helper to adjust overlapping layers for chords
     * Returns the shift of the adjustment
     */
    int AdjustOverlappingLayers(Doc *doc, const std::vector<LayerElement *> &otherElements, bool areDotsAdjusted,
        bool &isUnison, bool &stemSameas) override;

    /**
     * Helper to get list of notes that are adjacent to the specified location.
     * Diatonic step difference is take up to 2 points, so HasAdjacentNotesInStaff() needs to be called first, to make
     * sure there actually are adjacent notes.
     */
    std::list<Note *> GetAdjacentNotesList(Staff *staff, int loc);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::AdjustCrossStaffYPos
     */
    int AdjustCrossStaffYPos(FunctorParams *functorParams) override;

    /**
     * See Object::AdjustArtic
     */
    int AdjustArtic(FunctorParams *functorParams) override;

    /**
     * See Object::ConvertMarkupAnalytical
     */
    ///@{
    int ConvertMarkupAnalytical(FunctorParams *functorParams) override;
    int ConvertMarkupAnalyticalEnd(FunctorParams *functorParams) override;
    ///@}

    /**
     * See Object::CalcArtic
     */
    int CalcArtic(FunctorParams *functorParams) override;

    /**
     * See Object::CalcStem
     */
    int CalcStem(FunctorParams *functorParams) override;

    /**
     * See Object::CalcDots
     */
    int CalcDots(FunctorParams *functorParams) override;

    /**
     * See Object::PrepareLayerElementParts
     */
    int PrepareLayerElementParts(FunctorParams *functorParams) override;

    /**
     * See Object::PrepareLyrics
     */
    int PrepareLyrics(FunctorParams *functorParams) override;

    /**
     * See Object::CalcOnsetOffsetEnd
     */
    int CalcOnsetOffsetEnd(FunctorParams *functorParams) override;

    /**
     * See Object::ResetDrawing
     */
    int ResetDrawing(FunctorParams *functorParams) override;

    /**
     * See Object::AdjustCrossStaffContent
     */
    int AdjustCrossStaffContent(FunctorParams *functorParams) override;

    /**
     * See Object::GenerateMIDI
     */
    int GenerateMIDI(FunctorParams *functorParams) override;

protected:
    /**
     * The note locations w.r.t. each staff
     */
    MapOfNoteLocs CalcNoteLocations(NotePredicate predicate = NULL) override;

    /**
     * The dot locations w.r.t. each staff
     * Since dots for notes on staff lines can be shifted upwards or downwards, there are two choices: primary and
     * secondary
     */
    MapOfDotLocs CalcDotLocations(int layerCount, bool primary) override;

    /**
     * Calculate stem direction based on the position of the notes in chord. Notes are compared in pairs starting from
     * the top-/bottommost and moving inward towards the center of the chord
     */
    data_STEMDIRECTION CalcStemDirection(int verticalCenter);

    /**
     * Clear the m_clusters vector and delete all the objects.
     */
    void ClearClusters() const;

    /**
     * Filter the flat list and keep only Note elements.
     */
    void FilterList(ArrayOfObjects *childList) override;

public:
    mutable std::list<ChordCluster *> m_clusters;

    /**
     * Positions of dots in the chord to avoid overlapping
     */
    std::list<int> m_dots;
};

} // namespace vrv

#endif
