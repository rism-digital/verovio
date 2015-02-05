/////////////////////////////////////////////////////////////////////////////
// Name:        chord.cpp
// Author:      Andrew Horwitz
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "chord.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <iostream>

//----------------------------------------------------------------------------

#include "note.h"

namespace vrv {
    
//----------------------------------------------------------------------------
// Chord
//----------------------------------------------------------------------------

Chord::Chord( ):
LayerElement("chord-"), ObjectListInterface(), DurationInterface(),
    AttColoration(),
    AttCommon(),
    AttStemmed(),
    AttTiepresent()
{
    Reset();
}

Chord::~Chord()
{
}

void Chord::Reset()
{
    DocObject::Reset();
    DurationInterface::Reset();
    ResetCommon();
    ResetStemmed();
    ResetColoration();
    ResetTiepresent();
}
    
void Chord::AddLayerElement(vrv::LayerElement *element)
{
    assert( dynamic_cast<Note*>(element) );
    element->SetParent( this );
    m_children.push_back(element);
    Modify();
}
    
bool compare_pitch (Object *first, Object *second)
{
    Note *n1 = dynamic_cast<Note*>(first);
    Note *n2 = dynamic_cast<Note*>(second);
    return ( n1->GetDiatonicPitch() < n2->GetDiatonicPitch() );
}

void Chord::FilterList()
{
    // Retain only note children of chords
    ListOfObjects* childList = this->GetList(this);
    ListOfObjects::iterator iter = childList->begin();
    
    while ( iter != childList->end()) {
        LayerElement *currentElement = dynamic_cast<LayerElement*>(*iter);
        if ( !currentElement ) {
            // remove anything that is not an LayerElement
            iter = childList->erase( iter );
        }
        else if ( !currentElement->HasDurationInterface() )
        {
            iter = childList->erase( iter );
        } else {
            Note *n = dynamic_cast<Note*>(currentElement);
            
            if (n) {
                iter++;
            } else {
                // if it is not a note, drop it
                iter = childList->erase( iter );
            }
        }
    }
    
    childList->sort(compare_pitch);
    
    iter = childList->begin();
    
    Note *curNote, *lastNote = dynamic_cast<Note*>(*iter);
    lastNote->m_flippedNotehead = false;
    int curPitch, lastPitch = lastNote->GetDiatonicPitch();
    
    iter++;
    
    while ( iter != childList->end()) {
        curNote = dynamic_cast<Note*>(*iter);
        curPitch = curNote->GetDiatonicPitch();
        
        if (curPitch - lastPitch == 1) {
            curNote->m_flippedNotehead = !lastNote->m_flippedNotehead;
        }
        else {
            curNote->m_flippedNotehead = false;
        }
        
        lastNote = curNote;
        lastPitch = curPitch;
        
        iter++;
    }
}
    
void Chord::GetYExtremes(int *yMax, int *yMin)
{
    bool passed = false;
    int y1;
    ListOfObjects* childList = this->GetList(this); //make sure it's initialized
    for (ListOfObjects::iterator it = childList->begin(); it != childList->end(); it++) {
        Note *note = dynamic_cast<Note*>(*it);
        if (!note) continue;
        y1 = note->GetDrawingY();
        if (!passed) {
            *yMax = y1;
            *yMin = y1;
            passed = true;
        }
        else {
            if (y1 > *yMax) *yMax = y1;
            else if (y1 < *yMin) *yMin = y1;
        }
    }
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Chord::PrepareTieAttr( ArrayPtrVoid params )
{
    // param 0: std::vector<Note*>* that holds the current notes with open ties (unused)
    // param 1: Chord** currentChord for the current chord if in a chord
    Chord **currentChord = static_cast<Chord**>(params[1]);
    
    assert(!(*currentChord));
    (*currentChord) = this;

    return FUNCTOR_CONTINUE;
}

int Chord::PrepareTieAttrEnd( ArrayPtrVoid params )
{
    // param 0: std::vector<Note*>* that holds the current notes with open ties (unused)
    // param 1: Chord** currentChord for the current chord if in a chord
    Chord **currentChord = static_cast<Chord**>(params[1]);
    
    assert((*currentChord));
    (*currentChord) = NULL;
    
    return FUNCTOR_CONTINUE;
}
    
}