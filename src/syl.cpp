/////////////////////////////////////////////////////////////////////////////
// Name:        syl.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "syl.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <iostream>

//----------------------------------------------------------------------------

#include "note.h"
#include "verse.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Syl
//----------------------------------------------------------------------------

Syl::Syl():
    LayerElement("syl-"),
    AttSylLog()
{
    Reset();
}

Syl::~Syl()
{
}
    
void Syl::Reset()
{
    LayerElement::Reset();
    ResetSylLog();
    
    m_drawingFirstNote = NULL;
    m_drawingLastNote = NULL;
    m_drawingVerse = 1;
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Syl::PrepareLyrics( ArrayPtrVoid params )
{
    // param 0: the current Syl
    // param 1: the last Note
    // param 2: the last but one Note
    Syl **currentSyl = static_cast<Syl**>(params[0]);
    Note **lastNote = static_cast<Note**>(params[1]);
    Note **lastButOneNote = static_cast<Note**>(params[2]);
    
    Verse *verse = dynamic_cast<Verse*>( this->GetFirstParent( &typeid(Verse), MAX_NOTE_DEPTH ) );
    if ( verse ) {
        m_drawingVerse = std::max(verse->GetN(), 1);
    }
    
    m_drawingFirstNote = dynamic_cast<Note*>( this->GetFirstParent( &typeid(Note), MAX_NOTE_DEPTH ) );
    
    if ((*currentSyl)) {
        if (((*currentSyl)->GetWordpos() == WORDPOS_i) || ((*currentSyl)->GetWordpos() == WORDPOS_m)) {
            (*currentSyl)->m_drawingLastNote = (*lastNote);
        }
        else if ((*currentSyl)->GetCon() == CON_u) {
            (*currentSyl)->m_drawingLastNote = (*lastButOneNote);
        }
    }
    
    if ((this->GetWordpos() == WORDPOS_i) || (this->GetWordpos() == WORDPOS_m)) {
        (*currentSyl) = this;
        return FUNCTOR_CONTINUE;
    }
    else if (this->GetCon() == CON_u) {
        (*currentSyl) = this;
        return FUNCTOR_CONTINUE;
    }
    else {
        (*currentSyl) = NULL;
    }
    
    std::cout << UTF16to8( this->GetText().c_str() ) << std::endl;
    return FUNCTOR_CONTINUE;
}


} // namespace vrv
