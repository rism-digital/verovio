/////////////////////////////////////////////////////////////////////////////
// Name:        note.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "note.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "slur.h"
#include "tie.h"
#include "verse.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Note
//----------------------------------------------------------------------------

Note::Note():
	LayerElement("note-"), DurationInterface(), PitchInterface(),
    AttColoration(),
    AttNoteLogMensural(),
    AttStemmed()
{
    Reset();
}


Note::~Note()
{
    // This deletes the Tie and Slur objects if necessary
    ResetTieAttrInitial();
}
    
void Note::Reset()
{
    LayerElement::Reset();
    DurationInterface::Reset();
    PitchInterface::Reset();
    
    ResetColoration();
    ResetNoteLogMensural();
    ResetStemmed();
    
    // TO BE REMOVED
    m_acciaccatura = false;
    m_embellishment = EMB_NONE;
    // tie pointers
    m_tieAttrInitial = NULL;
    m_tieAttrTerminal = NULL;
    
    m_drawingStemDir = STEMDIRECTION_NONE;
    d_stemLen = 0;
}

bool Note::operator==( Object& other )
{
    Note *otherNote = dynamic_cast<Note*>( &other );
    if ( !otherNote ) {
        return false;
    }
    if ( this->m_colored != otherNote->m_colored ) {
        return false;
    }
    if ( this->m_lig != otherNote->m_lig ) {
        return false;
    }
    // slur need value by value comparison
    //if ( this->m_slur != otherNote->m_slur ) {
    //    return false;
    //}
    if ( this->m_stemDir != otherNote->m_stemDir ) {
        return false;
    }
    if ( this->m_stemLen != otherNote->m_stemLen ) {
        return false;
    }
    if ( this->m_acciaccatura != otherNote->m_acciaccatura ) {
        return false;
    }
    if ( this->m_embellishment != otherNote->m_embellishment ) {
        return false;
    }
    if ( !this->HasIdenticalPitchInterface( otherNote ) ) {
        return false;
    }
    if ( !this->HasIdenticalDurationInterface( otherNote ) ) {
        return false;
    }
    
    return true;
}
    
void Note::AddLayerElement(vrv::LayerElement *element)
{
    assert( dynamic_cast<Verse*>(element) || dynamic_cast<EditorialElement*>(element) );
    element->SetParent( this );
    m_children.push_back(element);
    Modify();
}

void Note::SetValue( int value, int flag )
{
    DurationInterface::SetDur( value );
    
	// remove ligature flag for  inadequate values	
	if ( ( value < DUR_BR ) || ( value > DUR_1 ) ) {
        this->SetLig(LIGATURE_NONE);
    }

	this->SetColored(BOOLEAN_NONE);
    
	// remove qauto flag for silences and inadequate values	
	if ( ( value > DUR_LG ) && ( value < DUR_2 ) ) {
		this->SetStemDir(STEMDIRECTION_NONE);
        this->SetStemLen(0);
    }    
}

void Note::SetTieAttrInitial()
{
    if ( m_tieAttrInitial ) {
        LogWarning("Initial tie attribute already set for note '%s", this->GetUuid().c_str() );
        return;
    }
    m_tieAttrInitial = new Tie();
    m_tieAttrInitial->SetStart( this );
}

void Note::SetTieAttrTerminal( Note *previousNote )
{
    if ( m_tieAttrTerminal ) {
        LogWarning("Terminal tie attribute already set for note '%s", this->GetUuid().c_str() );
        return;
    }
    if ( !previousNote || !previousNote->GetTieAttrInitial() ) {
        LogWarning("No previous note or previous note without intial or median attribute for note '%s", this->GetUuid().c_str() );
        return;
    }
    m_tieAttrTerminal = previousNote->GetTieAttrInitial();
    m_tieAttrTerminal->SetEnd( this );
}

void Note::ResetTieAttrInitial( )
{
    if ( m_tieAttrInitial ) {
        // Deleting the Tie object will also reset the m_tieAttrTerminal of the second note
        delete m_tieAttrInitial;
        m_tieAttrInitial = NULL;
    }
}
    
//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Note::PrepareLyrics( ArrayPtrVoid params )
{
    // param 0: the current Syl (unused)
    // param 1: the last Note
    // param 2: the last but one Note
    Note **lastNote = static_cast<Note**>(params[1]);
    Note **lastButOneNote = static_cast<Note**>(params[2]);
    
    (*lastButOneNote) = (*lastNote);
    (*lastNote) = this;
    
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
