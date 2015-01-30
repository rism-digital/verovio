/////////////////////////////////////////////////////////////////////////////
// Name:        syl.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "syl.h"

//----------------------------------------------------------------------------


//----------------------------------------------------------------------------

#include "note.h"
#include "verse.h"
#include "staff.h"

namespace vrv {

//----------------------------------------------------------------------------
// Syl
//----------------------------------------------------------------------------

Syl::Syl():
    LayerElement("syl-"), TimeSpanningInterface(),
    AttTypography(),
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
    TimeSpanningInterface::Reset();
    ResetTypography();
    ResetSylLog();
    
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
    
    this->SetStart( dynamic_cast<LayerElement*>( this->GetFirstParent( &typeid(Note), MAX_NOTE_DEPTH ) ) );
    
    // At this stage currentSyl is actually the previous one that is ending here
    if ((*currentSyl)) {
        // The previous syl was an initial or median -> The note we just parsed is the end
        if (((*currentSyl)->GetWordpos() == WORDPOS_i) || ((*currentSyl)->GetWordpos() == WORDPOS_m)) {
            (*currentSyl)->SetEnd(*lastNote);
        }
        // The previous syl was a underscore -> the previous but one was the end
        else if ((*currentSyl)->GetCon() == CON_u) {
            (*currentSyl)->SetEnd(*lastButOneNote);
        }
    }
    
    // Now decide what to do with the starting syl and check if it has a forward connector
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
    
    return FUNCTOR_CONTINUE;
}
    
int Syl::FillStaffCurrentLyrics( ArrayPtrVoid params )
{
    // param 0: the current Syl
    Syl **currentSyl = static_cast<Syl**>(params[0]);
    
    // Since the lastNote was set in Syl::PrepareLyircs previously
    // we can rely on it to check if the syl has a forward connector
    if (this->GetEnd() ) {
        if ( this->GetStart()->GetFirstParent( &typeid(Staff) ) != this->GetEnd()->GetFirstParent( &typeid(Staff) ) ) {
            (*currentSyl) = this;
        }
    }
    else {
        (*currentSyl) = NULL;
    }
    
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
