/////////////////////////////////////////////////////////////////////////////
// Name:        note.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_NOTE_H__
#define __VRV_NOTE_H__

#include "atts_mensural.h"
#include "atts_shared.h"
#include "beam.h"
#include "durationinterface.h"
#include "layerelement.h"
#include "pitchinterface.h"

// To be removed
#define CHORD_INITIAL  (1<<1)
#define CHORD_MEDIAL  (1<<2) 
#define CHORD_TERMINAL  (1<<3) 

namespace vrv {

class Slur;
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

class Note: public LayerElement, public DurationInterface, public PitchInterface,
    public AttColoration, public AttNoteLogMensural
{
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    Note();
    virtual ~Note();
    void Reset();
    virtual std::string GetClassName( ) { return "Note"; };
    ///@}
    
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
     * Return the default horizontal spacing of notes.
     */
    virtual int GetHorizontalSpacing( );
    
    /**
     * @name Setters and getters for tie attributes
     */
    ///@{
    void SetTieAttrInitial( );
    void SetTieAttrTerminal( Note *previousNote );
    Tie *GetTieAttrInitial( ) { return m_tieAttrInitial; };
    Tie *GetTieAttrTerminal( ) { return m_tieAttrTerminal; };
    void ResetTieAttrInitial();
    void ResetTieAttrTerminal() { m_tieAttrTerminal = NULL; };
    ///@}
    
    /**
     * @name Setters and getters for slur attributes.
     * Only one attribute is currently supported.
     */
    ///@{
    void SetSlurAttrInitial( );
    void SetSlurAttrTerminal( Note *previousNote );
    Slur *GetSlurAttrInitial( ) { return m_slurAttrInitial; };
    Slur *GetSlurAttrTerminal( ) { return m_slurAttrTerminal; };
    void ResetSlurAttrInitial();
    void ResetSlurAttrTerminal() { m_slurAttrTerminal = NULL; };
    ///@}
    
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
    
    /**
     * @name Slur attributes are represented by pointers to Slur objects.
     * There is one pointer for the initial attribute and one pointer for the end attribute.
     * The Slur objects points back to the notes as it is the case with a MEI slur element.
     * With attributes, the note with the initial attribute own the Slur object and take care of deleting it.
     * Currenly only one single slub attribute (@slur="i1") is supported.
     */
     /* Note: we would need to change this to a Slur vector to support 1-6 slur attributes */
    ///@{
    Slur *m_slurAttrInitial;
    Slur *m_slurAttrTerminal;
    ///@}
    
public:
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
