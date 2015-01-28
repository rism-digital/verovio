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
LayerElement("chord-"), DrawingListInterface(), ObjectListInterface(),    DurationInterface(),
    AttColoration(),
    AttCommon(),
    AttStemmed()
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
    DrawingListInterface::Reset();
    ResetCommon();
    ResetStemmed();
    ResetColoration();
}
    
void Chord::AddElement(vrv::LayerElement *element)
{
    assert( dynamic_cast<Note*>(element) );
    element->SetParent( this );
    m_children.push_back(element);
    Modify();
}

void Chord::FilterList()
{
    // We want to keep only notes and rest
    // Eventually, we also need to filter out grace notes properly (e.g., with sub-beams)
    ListOfObjects* childList = this->GetList(this);
    ListOfObjects::iterator iter = childList->begin();
    
    while ( iter != childList->end()) {
        LayerElement *currentElement = dynamic_cast<LayerElement*>(*iter);
        if ( !currentElement ) {
            // remove anything that is not an LayerElement (e.g. Verse, Syl, etc)
            iter = childList->erase( iter );
        }
        else if ( !currentElement->HasDurationInterface() )
        {
            // remove anything that has not a DurationInterface
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
}
    
void Chord::GetYExtremes(int initial, int *yMax, int *yMin)
{
    *yMax = initial;
    *yMin = initial;
    int y1;
    
    ListOfObjects* childList = this->GetList(this); //make sure it's initialized
    for (ListOfObjects::iterator it = childList->begin(); it != childList->end(); it++) {
        Note *note = dynamic_cast<Note*>(*it);
        y1 = note->GetDrawingY();
        if (y1 > *yMax) *yMax = y1;
        else if (y1 < *yMin) *yMin = y1;
    }
}
    
}