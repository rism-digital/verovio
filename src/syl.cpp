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
    RegisterAttClass(ATT_TYPOGRAPHY);
    RegisterAttClass(ATT_SYLLOG);
    
    RegisterInterface( TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface() );
    
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

int Syl::PrepareLyrics( ArrayPtrVoid *params )
{
    // param 0: the current Syl
    // param 1: the last Note
    // param 2: the last but one Note
    Syl **currentSyl = static_cast<Syl**>((*params)[0]);
    Note **lastNote = static_cast<Note**>((*params)[1]);
    Note **lastButOneNote = static_cast<Note**>((*params)[2]);
    
    Verse *verse = dynamic_cast<Verse*>( this->GetFirstParent( VERSE, MAX_NOTE_DEPTH ) );
    if ( verse ) {
        m_drawingVerse = std::max(verse->GetN(), 1);
    }
    
    this->SetStart( dynamic_cast<LayerElement*>( this->GetFirstParent( NOTE, MAX_NOTE_DEPTH ) ) );
    
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
    
int Syl::FillStaffCurrentTimeSpanning( ArrayPtrVoid *params )
{
    // Pass it to the pseudo functor of the interface
    return  TimeSpanningInterface::FillStaffCurrentTimeSpanning(params, this);
}
    
int Syl::ResetDarwing( ArrayPtrVoid *params )
{
    // Pass it to the pseudo functor of the interface
    return  TimeSpanningInterface::ResetDrawing(params, this);
};

} // namespace vrv
