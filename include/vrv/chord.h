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
              public AttChordVis,
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
    std::string GetClassName() const override { return "chord"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    DurationInterface *GetDurationInterface() override { return vrv_cast<DurationInterface *>(this); }
    const DurationInterface *GetDurationInterface() const override { return vrv_cast<const DurationInterface *>(this); }
    StemmedDrawingInterface *GetStemmedDrawingInterface() override { return vrv_cast<StemmedDrawingInterface *>(this); }
    const StemmedDrawingInterface *GetStemmedDrawingInterface() const override
    {
        return vrv_cast<const StemmedDrawingInterface *>(this);
    }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    /**
     * Add an element (only note supported) to a chord.
     */
    bool IsSupportedChild(ClassId classId) override;

    /**
     * Overwritten method for chord
     */
    void AddChild(Object *object) override;

    /**
     * Return the maximum and minimum Y positions of the notes in the chord
     */
    void GetYExtremes(int &yMax, int &yMin) const;

    /**
     * Return the top or bottom note or their Y position
     */
    ///@{
    Note *GetTopNote();
    const Note *GetTopNote() const;
    Note *GetBottomNote();
    const Note *GetBottomNote() const;
    int GetYTop() const;
    int GetYBottom() const;
    ///@}

    /**
     * Return min or max note X position
     */
    ///@{
    int GetXMin() const;
    int GetXMax() const;
    ///@}

    /**
     * Return the cross staff above or below (if any).
     */
    ///@{
    void GetCrossStaffExtremes(
        Staff *&staffAbove, Staff *&staffBelow, Layer **layerAbove = NULL, Layer **layerBelow = NULL);
    void GetCrossStaffExtremes(const Staff *&staffAbove, const Staff *&staffBelow, const Layer **layerAbove = NULL,
        const Layer **layerBelow = NULL) const;
    ///@}

    /**
     * Return true if the chord has some cross staff notes.
     */
    bool HasCrossStaff() const override;

    /**
     * Returns list of notes that have accidentals
     */
    void ResetAccidList();

    /**
     * Return information about a note's position in the chord
     */
    ///@{
    /** Return 0 if the note is the middle note, -1 if below it and 1 if above */
    int PositionInChord(const Note *note) const;
    ///@}

    /**
     * Get the stem up / stem down attachment point.
     * If necessary look at the glyph anchor (if any).
     */
    ///@{
    Point GetStemUpSE(const Doc *doc, int staffSize, bool isCueSize) const override;
    Point GetStemDownNW(const Doc *doc, int staffSize, bool isCueSize) const override;
    int CalcStemLenInThirdUnits(const Staff *staff, data_STEMDIRECTION stemDir) const override;
    ///@}

    /**
     * Check if the chord or one of its children is visible
     */
    bool IsVisible() const;

    /**
     * Return true if the chord has two notes with 1 diatonic step difference in the specific staff
     */
    bool HasAdjacentNotesInStaff(const Staff *staff) const;

    /**
     * Return true if the chord has at least one note with a @dots > 0
     */
    bool HasNoteWithDots() const;

    /**
     * Helper to adjust overlapping layers for chords
     * Returns the shift of the adjustment
     */
    int AdjustOverlappingLayers(const Doc *doc, const std::vector<LayerElement *> &otherElements, bool areDotsAdjusted,
        bool &isUnison, bool &stemSameas) override;

    /**
     * Helper to get list of notes that are adjacent to the specified location.
     * Diatonic step difference is take up to 2 points, so HasAdjacentNotesInStaff() needs to be called first, to make
     * sure there actually are adjacent notes.
     */
    std::list<const Note *> GetAdjacentNotesList(const Staff *staff, int loc) const;

    /**
     * Recalculate the m_noteGroups vector
     */
    void CalculateNoteGroups();

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
     * The note locations w.r.t. each staff
     */
    MapOfNoteLocs CalcNoteLocations(NotePredicate predicate = NULL) const override;

    /**
     * The dot locations w.r.t. each staff
     * Since dots for notes on staff lines can be shifted upwards or downwards, there are two choices: primary and
     * secondary
     */
    MapOfDotLocs CalcDotLocations(int layerCount, bool primary) const override;

    /**
     * Clear the m_noteGroups vector and delete all the objects.
     */
    void ClearNoteGroups() const;

    /**
     * Filter the flat list and keep only Note elements.
     */
    void FilterList(ListOfConstObjects &childList) const override;

public:
    //
private:
    /**
     * The list of chord note groups
     */
    mutable std::list<ChordNoteGroup *> m_noteGroups;
};

} // namespace vrv

#endif
