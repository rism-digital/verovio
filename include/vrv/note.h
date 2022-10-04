/////////////////////////////////////////////////////////////////////////////
// Name:        note.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_NOTE_H__
#define __VRV_NOTE_H__

#include <cassert>

//----------------------------------------------------------------------------

#include "accid.h"
#include "atts_externalsymbols.h"
#include "atts_frettab.h"
#include "atts_mensural.h"
#include "atts_midi.h"
#include "atts_shared.h"
#include "beam.h"
#include "chord.h"
#include "durationinterface.h"
#include "layerelement.h"
#include "pitchinterface.h"
#include "transposition.h"

namespace vrv {

class Accid;
class Chord;
class Note;
class PrepareLinkingParams;
class Slur;
class TabGrp;
class Tie;
class TransPitch;
class Verse;

//----------------------------------------------------------------------------
// Note
//----------------------------------------------------------------------------

/**
 * This class models the MEI <note> element.
 */
class Note : public LayerElement,
             public StemmedDrawingInterface,
             public DurationInterface,
             public PitchInterface,
             public PositionInterface,
             public AttColor,
             public AttColoration,
             public AttCue,
             public AttExtSym,
             public AttGraced,
             public AttMidiVelocity,
             public AttNoteGesTab,
             public AttNoteHeads,
             public AttNoteVisMensural,
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
    Note();
    virtual ~Note();
    Object *Clone() const override { return new Note(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Note"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    DurationInterface *GetDurationInterface() override { return vrv_cast<DurationInterface *>(this); }
    const DurationInterface *GetDurationInterface() const override { return vrv_cast<const DurationInterface *>(this); }
    PitchInterface *GetPitchInterface() override { return vrv_cast<PitchInterface *>(this); }
    const PitchInterface *GetPitchInterface() const override { return vrv_cast<const PitchInterface *>(this); }
    PositionInterface *GetPositionInterface() override { return vrv_cast<PositionInterface *>(this); }
    const PositionInterface *GetPositionInterface() const override { return vrv_cast<const PositionInterface *>(this); }
    StemmedDrawingInterface *GetStemmedDrawingInterface() override { return vrv_cast<StemmedDrawingInterface *>(this); }
    const StemmedDrawingInterface *GetStemmedDrawingInterface() const override
    {
        return vrv_cast<const StemmedDrawingInterface *>(this);
    }
    ///@}

    /**
     * Override the method since alignment is required.
     * For notes we want not to align notes within a ligature (except first and last)
     */
    bool HasToBeAligned() const override { return true; }

    /**
     * Add an element (a verse or an accid) to a note.
     * Only Verse and Accid elements will be actually added to the note.
     */
    bool IsSupportedChild(Object *object) override;

    /**
     * Overwritten method for note
     */
    void AddChild(Object *object) override;

    /**
     * Align dots shift for two notes. Should be used for unison notes to align dots positioning
     */
    void AlignDotsShift(const Note *otherNote);

    /**
     * @name Setter and getter for accid attribute and other pointers
     */
    ///@{
    Accid *GetDrawingAccid();
    const Accid *GetDrawingAccid() const;
    ///@}

    /**
     * @name Setter and getter for the drawing staff loc.
     * This is set by the CalcAlignmentPitchPos functor.
     */
    ///@{
    void SetDrawingLoc(int drawingLoc) { m_drawingLoc = drawingLoc; }
    int GetDrawingLoc() const { return m_drawingLoc; }
    ///@}

    /**
     * Check if the note has ledger lines.
     * If staff is passed, use it for getting the staff line number.
     * Otherwise, it will look for the Staff ancestor.
     * Set the value of ledger lines above or below.
     */
    bool HasLedgerLines(int &linesAbove, int &linesBelow, const Staff *staff = NULL) const;

    /**
     * Overriding functions to return information from chord parent if any
     */
    ///@{
    Chord *IsChordTone();
    const Chord *IsChordTone() const;
    int GetDrawingDur() const;
    bool IsClusterExtreme() const; // used to find if it is the highest or lowest note in a cluster
    ///@}

    /**
     * Return the parent TabGrp is the note is part of one.
     */
    ///@{
    TabGrp *IsTabGrpNote();
    const TabGrp *IsTabGrpNote() const;
    ///@}

    /**
     * @name Return the smufl string to use for a note give the notation type
     */
    ///@{
    std::u32string GetTabFretString(data_NOTATIONTYPE notationType) const;
    ///@}

    /**
     * Return true if the note is a unison.
     * If ignoreAccid is set to true then only @pname and @oct are compared.
     */
    bool IsUnisonWith(const Note *note, bool ignoreAccid = false) const;

    /**
     * @name Setter and getter for the chord cluster and the position of the note
     */
    ///@{
    void SetCluster(ChordCluster *cluster, int position);
    ChordCluster *GetCluster() { return m_cluster; }
    ///@}

    /**
     * @name Setter and getter for the flipped note head flag
     */
    ///@{
    void SetFlippedNotehead(bool flippedNotehead) { m_flippedNotehead = flippedNotehead; }
    bool GetFlippedNotehead() const { return m_flippedNotehead; }
    ///@}

    /**
     * Returns a single integer representing pitch and octave.
     */
    int GetDiatonicPitch() const { return this->GetPname() + (int)this->GetOct() * 7; }

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
     * Return the SMuFL code for a mensural note looking at the staff notation type, the coloration and the duration
     */
    char32_t GetMensuralNoteheadGlyph() const;

    /**
     * Return a SMuFL code for the notehead
     */
    char32_t GetNoteheadGlyph(const int duration) const;

    /**
     * Check whether current note is enharmonic with another
     */
    bool IsEnharmonicWith(const Note *note) const;

    /**
     * Check if a note or its parent chord are visible
     */
    bool IsVisible() const;

    /**
     * MIDI pitch
     */
    int GetMIDIPitch(int shift = 0) const;

    /**
     * Get pitch class of the current note
     */
    int GetPitchClass() const;

    /**
     * @name Checker, getter and setter for a note with which the stem is shared
     */
    ///@{
    bool HasStemSameasNote() const { return (m_stemSameas); }
    Note *GetStemSameasNote() { return m_stemSameas; }
    const Note *GetStemSameasNote() const { return m_stemSameas; }
    void SetStemSameasNote(Note *stemSameas) { m_stemSameas = stemSameas; }
    ///@}

    /**
     * Getter for stem sameas role
     */
    StemSameasDrawingRole GetStemSameasRole() const { return m_stemSameasRole; }

    /**
     * Resovle @stem.sameas links by instanciating Note::m_stemSameas (*Note).
     * Called twice from Object::PrepareLinks. Once to fill id / note pairs,
     * and once to resolve the link. The link is bi-directional, which means
     * that both notes have their m_stemSameas pointer instanciated.
     */
    void ResolveStemSameas(PrepareLinkingParams *params);

    /**
     * Calculate the stem direction of the pair of notes.
     * The presence of a StemSameasNote() needs to be check before calling it.
     * Encoded stem direction on the calling note is taken into account.
     * Called from Note::CalcStem
     */
    data_STEMDIRECTION CalcStemDirForSameasNote(int verticalCenter);

    /**
     * Set the Note::m_flippedNotehead flag if one of the two notes needs to be placed on the side.
     * The note X relative position remains untouched because we do not want the stem position to be changed.
     * This is different than with chords. It means the the X position is actually corrected when drawing the note.
     */
    void CalcNoteHeadShiftForSameasNote(Note *stemSameas, data_STEMDIRECTION stemDir);

public:
    //----------------//
    // Static methods //
    //----------------//

    /**
     * Assume that two notes from different layers are given occuring at the same time
     * Returns true if one note has a ledger line that collides (or is quite close) to the other note's stem
     */
    static bool HandleLedgerLineStemCollision(const Doc *doc, const Staff *staff, const Note *note1, const Note *note2);

    /**
     * Get pitch class based on the pitch name
     */
    static int PnameToPclass(data_PITCHNAME pitchName);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::AdjustArtic
     */
    int AdjustArtic(FunctorParams *functorParams) override;

    /**
     * See Object::ConvertMarkupAnalytical
     */
    int ConvertMarkupAnalytical(FunctorParams *functorParams) override;

    /**
     * See Object::CalcArtic
     */
    int CalcArtic(FunctorParams *functorParams) override;

    /**
     * See Object::CalcStem
     */
    int CalcStem(FunctorParams *functorParams) override;

    /**
     * See Object::CalcChordNoteHeads
     */
    int CalcChordNoteHeads(FunctorParams *functorParams) override;

    /**
     * See Object::CalcDots
     */
    int CalcDots(FunctorParams *functorParams) override;

    /**
     * See Object::CalcLedgerLines
     */
    int CalcLedgerLines(FunctorParams *functorParams) override;

    /**
     * See Object::PrepareLayerElementParts
     */
    int PrepareLayerElementParts(FunctorParams *functorParams) override;

    /**
     * See Object::PrepareLyrics
     */
    int PrepareLyrics(FunctorParams *functorParams) override;

    /**
     * See Object::ResetData
     */
    int ResetData(FunctorParams *functorParams) override;

    /**
     * See Object::ResetHorizontalAlignment
     */
    int ResetHorizontalAlignment(FunctorParams *functorParams) override;

    /**
     * See Object::GenerateMIDI
     */
    int GenerateMIDI(FunctorParams *functorParams) override;

    /**
     * See Object::GenerateTimemap
     */
    int GenerateTimemap(FunctorParams *functorParams) override;

    /**
     * See Object::Transpose
     */
    int Transpose(FunctorParams *functorParams) override;

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

private:
    /**
     * Get the pitch difference in semitones of the accidental (implicit or explicit) for this note.
     */
    int GetChromaticAlteration() const;

    TransPitch GetTransPitch() const;

    void UpdateFromTransPitch(const TransPitch &tp, bool hasKeySig);

    /**
     * Return whether dots are overlapping with flag. Take into account flag height, its position as well
     * as position of the note and position of the dots
     */
    bool IsDotOverlappingWithFlag(const Doc *doc, const int staffSize, int dotLocShift) const;

    /**
     * Register deferred notes for MIDI
     */
    void DeferMIDINote(FunctorParams *functorParams, double shift, bool includeChordSiblings);

    /**
     * Create the MIDI output of the grace note sequence stored in params
     */
    void GenerateGraceNoteMIDI(FunctorParams *functorParams, double startTime, int tpq, int channel, int velocity);

public:
    //
private:
    /**
     * The drawing location of the note
     */
    int m_drawingLoc;

    /**
     * A fling indicating if the note head is flipped
     */
    bool m_flippedNotehead;

    /**
     * flags for determining clusters in chord (cluster this belongs to)
     */
    ChordCluster *m_cluster;

    /**
     * Position in the cluster (1-indexed position in said cluster; 0 if does not have position)
     */
    int m_clusterPosition;

    /**
     * A pointer to a note with which the note shares its stem and implementing @stem.sameas.
     * The pointer is bi-directional (both notes point to each other).
     * It is set in Note::ResolveStemSameas
     */
    Note *m_stemSameas;

    /**
     * The role in a stem.sameas situation.
     * Set in Note::ResolveStemSameas and then in Note::CalcStemDirForSameasNote
     * Used to determine if the note is the primary one (normal stem, e.g., with flag)
     * or the secondary one (linking both notes). This depends on the drawing stem direction,
     * which can be encoded but otherwise calculated by CalcStemDirForSameasNote
     */
    StemSameasDrawingRole m_stemSameasRole;
};

//----------------------------------------------------------------------------
// DiatonicSort
//----------------------------------------------------------------------------

/**
 * Unary predicate for sorting notes by diatonic pitch
 */
class DiatonicSort {

public:
    DiatonicSort() {}

    bool operator()(const Object *first, const Object *second) const
    {
        const Note *n1 = dynamic_cast<const Note *>(first);
        const Note *n2 = dynamic_cast<const Note *>(second);
        assert(n1 && n2);
        return (n1->GetDiatonicPitch() < n2->GetDiatonicPitch());
    }
};

//----------------------------------------------------------------------------
// TabCourseSort
//----------------------------------------------------------------------------

/**
 * Unary predicate for sorting notes by course number
 */
class TabCourseSort {

public:
    TabCourseSort() {}

    bool operator()(const Object *first, const Object *second) const
    {
        const Note *n1 = dynamic_cast<const Note *>(first);
        const Note *n2 = dynamic_cast<const Note *>(second);
        assert(n1 && n2);
        return (n1->GetTabCourse() > n2->GetTabCourse());
    }
};

} // namespace vrv

#endif
