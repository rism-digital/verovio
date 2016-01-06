/////////////////////////////////////////////////////////////////////////////
// Name:        note.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_NOTE_H__
#define __VRV_NOTE_H__

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

// embellishments
// for the moment only the trill is implemented
// to be change to MEI Att
#define EMB_NONE 0
#define EMB_TRILL 1
#define EMB_MORDENT 2

class Note : public LayerElement,
             public StemmedDrawingInterface,
             public DurationInterface,
             public PitchInterface,
             public AttColoration,
             public AttGraced,
             public AttNoteLogMensural,
             public AttStems,
             public AttStemsCmn,
             public AttTiepresent {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    Note();
    virtual ~Note();
    virtual void Reset();
    virtual std::string GetClassName() { return "Note"; };
    virtual ClassId Is() { return NOTE; };
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() { return true; };

    /**
     * Add an element (a verse or an accid) to a note.
     * Only Verse and Accid elements will be actually added to the note.
     */
    void AddLayerElement(LayerElement *element);

    /**
     * @name Setter and getter for tie attribute and other pointers
     */
    ///@{
    void ResetDrawingAccid();
    void ResetDrawingTieAttr();
    void SetDrawingTieAttr();
    Tie *GetDrawingTieAttr() { return m_drawingTieAttr; };
    ///@}

    /**
     * @name Setter and getter for the Algnment the grace note is pointing to (NULL by default)
     */
    ///@{
    Alignment *GetGraceAlignment();
    void SetGraceAlignment(Alignment *graceAlignment);
    bool HasGraceAlignment() { return (m_graceAlignment != NULL); };
    void ResetGraceAlignment() { m_graceAlignment = NULL; };
    ///@}

    /**
     * Overriding functions to return information from chord parent if any
     */
    ///@{
    Chord *IsChordTone();
    int GetDrawingDur();
    bool IsClusterExtreme(); // used to find if is the highest or lowest note in a cluster
    ///@}

    /**
     * Returns a single integer representing pitch and octave.
     */
    int GetDiatonicPitch() { return this->GetPname() + (int)this->GetOct() * 7; };

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::PrepareTieAttr
     */
    virtual int PrepareTieAttr(ArrayPtrVoid *params);

    /**
     * Functor for setting wordpos and connector ends
     * The functor is process by staff/layer/verse using an ArrayOfAttComparisons filter.
     */
    virtual int PrepareLyrics(ArrayPtrVoid *params);

    /**
     * See Object::PreparePointersByLayer
     */
    virtual int PreparePointersByLayer(ArrayPtrVoid *params);

    /**
     */
    virtual int FillStaffCurrentTimeSpanning(ArrayPtrVoid *params);

    /**
     * Reset the drawing values before calling PrepareDrawing after changes.
     */
    virtual int ResetDrawing(ArrayPtrVoid *params);

private:
public:
    /** embellishment on this note **/
    unsigned int m_embellishment; // To be changed to Att

    /** drawing stem length */
    int d_stemLen;

    /** flags for determining clusters in chord **/
    ChordCluster *m_cluster; // cluster this belongs to
    int m_clusterPosition; // 1-indexed position in said cluster; 0 if does not have position

    /** other information necessary for notes in chords **/
    Accid *m_drawingAccid;

    /**
     * Flag indicating if the drawing accid is an attribute.
     * If yes, then it is owned by the Note and will be deleted
     */
    bool m_isDrawingAccidAttr;

private:
    /**
     * Tie attributes are represented a pointers to Tie objects.
     * There is one pointer for the initial attribute (TIE_i or TIE_m).
     * The note with the initial attribute owns the Tie object and take care of deleting it
     */
    Tie *m_drawingTieAttr;
    /**
     * An alignment for grace notes
     */
    Alignment *m_graceAlignment;
};

} // namespace vrv

#endif
