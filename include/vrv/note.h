/////////////////////////////////////////////////////////////////////////////
// Name:        note.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_NOTE_H__
#define __VRV_NOTE_H__

#include <assert.h>

//----------------------------------------------------------------------------

#include "accid.h"
#include "atts_externalsymbols.h"
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
class Slur;
class Tie;
class TransPitch;
class Verse;
class Note;
typedef std::vector<Note *> ChordCluster;

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
    virtual Object *Clone() const { return new Note(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Note"; }
    virtual ClassId GetClassId() const { return NOTE; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual DurationInterface *GetDurationInterface() { return dynamic_cast<DurationInterface *>(this); }
    virtual PitchInterface *GetPitchInterface() { return dynamic_cast<PitchInterface *>(this); }
    virtual StemmedDrawingInterface *GetStemmedDrawingInterface()
    {
        return dynamic_cast<StemmedDrawingInterface *>(this);
    }
    ///@}

    /**
     * Override the method since alignment is required.
     * For notes we want not to align notes within a ligature (except first and last)
     */
    virtual bool HasToBeAligned() const { return true; }

    /**
     * Add an element (a verse or an accid) to a note.
     * Only Verse and Accid elements will be actually added to the note.
     */
    virtual bool IsSupportedChild(Object *object);

    /**
     * Overwritten method for note
     */
    virtual void AddChild(Object *object);

    /**
     * @name Setter and getter for tie attribute and other pointers
     */
    ///@{
    Accid *GetDrawingAccid();
    ///@}

    /**
     * @name Setter and getter for the drawing staff loc.
     * This is set by the SetAlignmentPitchPos functor.
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
    bool HasLedgerLines(int &linesAbove, int &linesBelow, Staff *staff = NULL);

    /**
     * Overriding functions to return information from chord parent if any
     */
    ///@{
    Chord *IsChordTone() const;
    int GetDrawingDur() const;
    bool IsClusterExtreme() const; // used to find if it is the highest or lowest note in a cluster
    ///@}

    /**
     * Return true if the note is a unisson.
     * If ignoreAccid is set to true then only @pname and @oct are compared.
     */
    bool IsUnissonWith(Note *note, bool ignoreAccid = false);

    /**
     * @name Setter and getter for the chord cluster and the position of the note
     */
    ///@{
    void SetCluster(ChordCluster *cluster, int position);
    ChordCluster *GetCluster() const { return m_cluster; }
    ///}

    /**
     * @name Setter and getter for the flipped note head flag
     */
    ///@{
    void SetFlippedNotehead(bool flippedNotehead) { m_flippedNotehead = flippedNotehead; }
    bool GetFlippedNotehead() const { return m_flippedNotehead; }
    ///}

    /**
     * Returns a single integer representing pitch and octave.
     */
    int GetDiatonicPitch() const { return this->GetPname() + (int)this->GetOct() * 7; }

    /**
     * Get the stem up / stem down attachment point.
     * If necessary look at the glyph anchor (if any).
     */
    ///@{
    virtual Point GetStemUpSE(Doc *doc, int staffSize, bool isCueSize);
    virtual Point GetStemDownNW(Doc *doc, int staffSize, bool isCueSize);
    virtual int CalcStemLenInThirdUnits(Staff *staff);
    ///@}

    /**
     * Return the SMuFL code for a mensural note looking at the staff notation type, the coloration and the duration
     */
    wchar_t GetMensuralSmuflNoteHead();

    /**
     * Return a SMuFL code for the notehead
     */
    wchar_t GetNoteheadGlyph(const int duration) const;

    /**
     * Check if a note or its parent chord are visible
     */
    bool IsVisible();

    /**
     * Calculate note horizontal overlap with elemenents from another layers. Returns overlapMargin and index of other
     * element if it's in unison with it
     */
    std::pair<int, bool> CalcNoteHorizontalOverlap(Doc *doc, const std::vector<LayerElement *> &otherElements,
        bool isChordElement, bool isLowerElement = false, bool unison = true);

    /**
     * MIDI timing information
     */
    ///@{
    void SetScoreTimeOnset(double scoreTime);
    void SetRealTimeOnsetSeconds(double timeInSeconds);
    void SetScoreTimeOffset(double scoreTime);
    void SetRealTimeOffsetSeconds(double timeInSeconds);
    void SetScoreTimeTiedDuration(double timeInSeconds);
    void SetMIDIPitch(char pitch);
    double GetScoreTimeOnset();
    double GetRealTimeOnsetMilliseconds();
    double GetScoreTimeOffset();
    double GetScoreTimeTiedDuration();
    double GetRealTimeOffsetMilliseconds();
    double GetScoreTimeDuration();
    char GetMIDIPitch();
    ///@}

    /**
     * Helper to adjust overlaping layers for notes
     */
    virtual void AdjustOverlappingLayers(Doc *doc, const std::vector<LayerElement *> &otherElements, bool &isUnison);

public:
    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ConvertMarkupAnalytical
     */
    virtual int ConvertMarkupAnalytical(FunctorParams *functorParams);

    /**
     * See Object::CalcStem
     */
    virtual int CalcStem(FunctorParams *functorParams);

    /**
     * See Object::CalcChordNoteHeads
     */
    virtual int CalcChordNoteHeads(FunctorParams *functorParams);

    /**
     * See Object::CalcDots
     */
    virtual int CalcDots(FunctorParams *functorParams);

    /**
     * See Object::CalcLedgerLines
     */
    virtual int CalcLedgerLines(FunctorParams *functorParams);

    /**
     * See Object::PrepareLayerElementParts
     */
    virtual int PrepareLayerElementParts(FunctorParams *functorParams);

    /**
     * See Object::PrepareLyrics
     */
    virtual int PrepareLyrics(FunctorParams *functorParams);

    /**
     * See Object::PreparePointersByLayer
     */
    virtual int PreparePointersByLayer(FunctorParams *functorParams);

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * See Object::ResetHorizontalAlignment
     */
    virtual int ResetHorizontalAlignment(FunctorParams *functorParams);

    /**
     * See Object::GenerateMIDI
     */
    virtual int GenerateMIDI(FunctorParams *functorParams);

    /**
     * See Object::GenerateTimemap
     */
    virtual int GenerateTimemap(FunctorParams *functorParams);

    /**
     * See Object::Transpose
     */
    virtual int Transpose(FunctorParams *);

private:
    /**
     * Get the pitch difference in semitones of the accidental (implicit or explicit) for this note.
     */
    int GetChromaticAlteration();

    TransPitch GetTransPitch();

    void UpdateFromTransPitch(const TransPitch &tp);

    /**
     * Return whether dots are overlapping with flag. Take into account flag height, its position as well
     * as position of the note and position of the dots
     */
    bool IsDotOverlappingWithFlag(Doc *doc, const int staffSize, bool isDotShifted);

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
     **/
    ChordCluster *m_cluster;

    /**
     * Position in the cluster (1-indexed position in said cluster; 0 if does not have position)
     */
    int m_clusterPosition;

    /**
     * The score-time onset of the note in the measure (duration from the start of measure in
     * quarter notes).
     */
    double m_scoreTimeOnset;

    /**
     * The score-time off-time of the note in the measure (duration from the start of the measure
     * in quarter notes).  This is the duration of the printed note.  If the note is the start of
     * a tied group, the score time of the tied group is this variable plus m_scoreTimeTiedDuration.
     * If this note is a secondary note in a tied group, then this value is the score time end
     * of the printed note, and the m_scoreTimeTiedDuration is -1.0 to indicate that it should not
     * be exported when creating a MIDI file.
     */
    double m_scoreTimeOffset;

    /**
     * The time in milliseconds since the start of the measure element that contains the note.
     */
    double m_realTimeOnsetMilliseconds;

    /**
     * The time in milliseconds since the start of the measure element to end of printed note.
     * The real-time duration of a tied group is not currently tracked (this gets complicated
     * if there is a tempo change during a note sustain, which is currently not supported).
     */
    double m_realTimeOffsetMilliseconds;

    /**
     * If the note is the first in a tied group, then m_scoreTimeTiedDuration contains the
     * score-time duration (in quarter notes) of all tied notes in the group after this note.
     * If the note is a secondary note in a tied group, then this variable is set to -1.0 to
     * indicate that it should not be written to MIDI output.
     */
    double m_scoreTimeTiedDuration;

    /**
     * The MIDI pitch of the note.
     */
    char m_MIDIPitch;
};

//----------------------------------------------------------------------------
// DiatonicSort
//----------------------------------------------------------------------------

/**
 * Unary predicate for comparing object types.
 * This is used for example in std::find_if.
 * See Object::GetFirst or Object::GetNext
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

} // namespace vrv

#endif
