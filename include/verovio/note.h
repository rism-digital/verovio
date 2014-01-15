/////////////////////////////////////////////////////////////////////////////
// Name:        note.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_NOTE_H__
#define __VRV_NOTE_H__

#include "beam.h"
#include "durationinterface.h"
#include "layerelement.h"
#include "pitchinterface.h"

#define SLUR_INITIAL  (1<<1)
#define SLUR_MEDIAL  (1<<2) 
#define SLUR_TERMINAL  (1<<3) 

#define LIG_INITIAL  (1<<1)
#define LIG_MEDIAL  (1<<2) 
#define LIG_TERMINAL  (1<<3) 

#define CHORD_INITIAL  (1<<1)
#define CHORD_MEDIAL  (1<<2) 
#define CHORD_TERMINAL  (1<<3) 

#define NOTE_MAX_SLURS 6

namespace vrv {

class Tie;

//----------------------------------------------------------------------------
// Note
//----------------------------------------------------------------------------

/** 
 * This class models the MEI <note> element. 
 */


// embellishments
// for the moment only the trill is implemented
#define EMB_NONE 0
#define EMB_TRILL 1
#define EMB_MORDENT 2

class Note: public LayerElement, public DurationInterface, public PitchInterface
{
public:
    // constructors and destructors
    Note();
    virtual ~Note();
    
    virtual std::string GetClassName( ) { return "Note"; };
    
     /** 
      * Set the duration.
      * Because we need to perform additonal check, this method override the LayerElement::SetValue method.
      * The DurationInterface::SetDuration method has to be called explicitly.
      */
    virtual void SetValue( int value, int flag = 0 );
    
    /**
     * Comparison operator. 
     * Check if the LayerElement if a Note and compare attributes
     */
    virtual bool operator==(Object& other);
    
    /**
     * Change the coloration.
     */ 
	virtual void ChangeColoration( );
    
    /**
     * Change the stem direction.
     */
	virtual void ChangeStem( );
    
    /**
     * Set the note into a ligature.
     */
    virtual void SetLigature( );
    
    /**
     * Return the default horizontal spacing of notes.
     */
    virtual int GetHorizontalSpacing( );
    
    /**
     *
     */
    void SetTieAttrInitial( );
    void SetTieAttrTerminal( Note *previousNote );
    Tie *GetTieAttrInitial( ) { return m_tieAttrInitial; };
    Tie *GetTieAttrTerminal( ) { return m_tieAttrTerminal; };
    void ResetTieAttrInitial();
    void ResetTieAttrTerminal() { m_tieAttrTerminal = NULL; };
    
private:
    
protected:
    
    /**
     * @name Tie attributes are represented by pointers to Tie objects.
     * There is one pointer for the initial attribute and one pointer for the end attribute.
     * The Tie objects points back to the notes as it is the case with a MEI tie element.
     * With attributes, the note with the initial attribute own the Tie object and take care of deleting it
     */
    ///@{
    Tie *m_tieAttrInitial;
    Tie *m_tieAttrTerminal;
    ///@}
    
public:
    /** Indicates the articulation (0 == none) */
    unsigned char m_artic;
    /** Indicates if initial, medial or terminal part of a chord */
    unsigned char m_chord;
    /** Indicates if colored (inversed) */
    bool m_colored;
    /** Indicates if intial, medial or terminal part of a ligature */
    unsigned char m_lig;
    /** Indicates the headshape of the note */
    unsigned char m_headshape;
    /** Indicates if the ligature is obliqua (recta otherwise) */
    bool m_ligObliqua;
    /** Indicates if intial, medial or terminal part of a slur for up to 6 nesting/overlaping slur */
    unsigned char m_slur[NOTE_MAX_SLURS];
    /** Indicates the stem direction (0 == auto, -1 down, 1 up) */
    char m_stemDir;
    /** Indicates the stem length (0 == auto) */
    unsigned char m_stemLen;
    /** indicates if the appoggiatura is slashed (i.e. it is an acciaccatura)
     used with cueSize = true */
    bool m_acciaccatura;
    /** embellishment on this note **/
    unsigned int m_embellishment;
    
private:
    
};

} // namespace vrv
    
#endif
