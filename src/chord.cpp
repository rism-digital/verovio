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
    AttCommon(),
    AttStemmed(),
    AttColoration()
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
            Note *n = dynamic_cast<Note*>(currentElement);
            
            if (n) {
                iter++;
            } else {
                // if it is not a note, drop it
                iter = m_list.erase( iter );
            }
        }
    }
}
    
yExtremes Chord::GetYExtremes(float initial)
{
    yExtremes yVals;
    yVals.yMax = initial;
    yVals.yMin = initial;
    
    for (int i = 0; i < (int)this->m_children.size(); i++)
    {
        Note *note = dynamic_cast<Note*>(this->m_children[i]);
        int y1 = note->GetDrawingY();
        if (y1 > yVals.yMax) yVals.yMax = y1;
        else if (y1 < yVals.yMin) yVals.yMin = y1;
    }
    
    return yVals;
}
    
}