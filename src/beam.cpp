/////////////////////////////////////////////////////////////////////////////
// Name:        beam.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "beam.h"

//----------------------------------------------------------------------------

#include "note.h"

namespace vrv {

//----------------------------------------------------------------------------
// Beam
//----------------------------------------------------------------------------

Beam::Beam():
    LayerElement("beam-"), ObjectListInterface()
{
}


Beam::~Beam()
{
}

void Beam::AddElement(LayerElement *element) {
   
    element->SetParent( this );
    m_children.push_back(element);
    Modify();
}

void Beam::FilterList()
{
    bool firstNoteGrace = false;
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
                // if we are at the beginning of the beam
                // and the note is cueSize
                // assume all the beam is of grace notes
                if (m_list.begin() == iter) {
                  if (n->m_cueSize)
                      firstNoteGrace = true;
                }
                
                // if the first note in beam was NOT a grace
                // we have grace notes embedded in a beam
                // drop them
                if ( !firstNoteGrace && n->m_cueSize == true)
                    iter = m_list.erase( iter );
                else
                    iter++;
                
            } else {
                // if it is a Rest, do not drop
                iter++;
            }
        }
    }
    
}

} // namespace vrv

