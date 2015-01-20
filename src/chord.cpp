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
LayerElement("chord-"), DrawingListInterface(), ObjectListInterface(),
AttCommon()
{
    Reset();
}

Chord::~Chord()
{
}

void Chord::Reset()
{
    DocObject::Reset();
    DrawingListInterface::Reset();
    ResetCommon();
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
    ListOfObjects::iterator iter = m_list.begin();
    
    while ( iter != m_list.end()) {
        LayerElement *currentElement = dynamic_cast<LayerElement*>(*iter);
        if ( !currentElement ) {
            // remove anything that is not an LayerElement (e.g. Verse, Syl, etc)
            iter = m_list.erase( iter );
        }
        else if ( !currentElement->HasDurationInterface() )
        {
            // remove anything that has not a DurationInterface
            iter = m_list.erase( iter );
        } else {
            // Drop notes that are signaled as grace notes
            Note *n = dynamic_cast<Note*>(currentElement);
            
            if (n) {
                iter++;
                
            } else {
                // if it is a Rest, do not drop
                iter = m_list.erase( iter );
            }
        }
    }
}
    
}