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
#include "atts_mensural.h"
#include "atts_shared.h"
#include "beam.h"
#include "chord.h"
#include "durationinterface.h"
#include "layerelement.h"
#include "pitchinterface.h"

namespace vrv {

class Accid;
class Chord;
class Slur;
class Tie;
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
             public AttColor,
             public AttColoration,
             public AttGraced,
             public AttNoteLogMensural,
             public AttRelativesize,
             public AttStems,
             public AttStemsCmn,
             public AttTiepresent,
             public AttVisibility {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Note();
    virtual ~Note();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Note"; }
    virtual ClassId GetClassId() const { return NOTE; }
    ///@}

    virtual DurationInterface *GetDurationInterface() { return dynamic_cast<DurationInterface *>(this); }
    virtual PitchInterface *GetPitchInterface() { return dynamic_cast<PitchInterface *>(this); }
    virtual StemmedDrawingInterface *GetStemmedDrawingInterface()
    {
        return dynamic_cast<StemmedDrawingInterface *>(this);
    }

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    /**
     * Add an element (a verse or an accid) to a note.
     * Only Verse and Accid elements will be actually added to the note.
     */
    virtual void AddChild(Object *object);

    /**
     * @name Setter and getter for tie attribute and other pointers
     */
    ///@{
    Accid *GetDrawingAccid();
    void ResetDrawingTieAttr();
    void SetDrawingTieAttr();
    Tie *GetDrawingTieAttr() const { return m_drawingTieAttr; }
    ///@}

    /**
     * Overriding functions to return information from chord parent if any
     */
    ///@{
    Chord *IsChordTone() const;
    int GetDrawingDur();
    bool IsClusterExtreme() const; // used to find if it is the highest or lowest note in a cluster
    ///@}

    /**
     * Returns a single integer representing pitch and octave.
     */
    int GetDiatonicPitch() const { return this->GetPname() + (int)this->GetOct() * 7; }

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::PrepareTieAttr
     */
    virtual int PrepareTieAttr(FunctorParams *functorParams);

    /**
     * See Object::PrepareLyrics
     */
    virtual int PrepareLyrics(FunctorParams *functorParams);

    /**
     * See Object::PreparePointersByLayer
     */
    virtual int PreparePointersByLayer(FunctorParams *functorParams);

    /**
     * See Object::FillStaffCurrentTimeSpanning
     */
    virtual int FillStaffCurrentTimeSpanning(FunctorParams *functorParams);

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

private:
    //
public:
    /** drawing stem length */
    int d_stemLen;

    /** flags for determining clusters in chord **/
    ChordCluster *m_cluster; // cluster this belongs to
    int m_clusterPosition; // 1-indexed position in said cluster; 0 if does not have position

    double m_playingOnset;
    double m_playingOffset;

private:
    /**
     * Tie attributes are represented a pointers to Tie objects.
     * There is one pointer for the initial attribute (TIE_i or TIE_m).
     * The note with the initial attribute owns the Tie object and takes care of deleting it
     */
    Tie *m_drawingTieAttr;
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
