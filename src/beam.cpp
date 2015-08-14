/////////////////////////////////////////////////////////////////////////////
// Name:        beam.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "beam.h"

//----------------------------------------------------------------------------

#include "assert.h"

//----------------------------------------------------------------------------

#include "note.h"

namespace vrv {

//----------------------------------------------------------------------------
// Beam
//----------------------------------------------------------------------------

Beam::Beam():
    LayerElement("beam-"), ObjectListInterface()
{
    Reset();
    
    m_drawingStemDir = STEMDIRECTION_NONE;
}


Beam::~Beam()
{
}
    
void Beam::Reset()
{
    LayerElement::Reset();
}

void Beam::AddLayerElement(LayerElement *element)
{
   
    element->SetParent( this );
    m_children.push_back(element);
    Modify();
}

void Beam::FilterList( ListOfObjects *childList )
{
    bool firstNoteGrace = false;
    // We want to keep only notes and rest
    // Eventually, we also need to filter out grace notes properly (e.g., with sub-beams)
    ListOfObjects::iterator iter = childList->begin();
    
    while ( iter != childList->end()) {
        if ( !(*iter)->IsLayerElement() ) {
            // remove anything that is not an LayerElement (e.g. Verse, Syl, etc)
            iter = childList->erase( iter );           
        }
        LayerElement *currentElement = dynamic_cast<LayerElement*>(*iter);
        assert( currentElement );
        if ( !currentElement->HasDurationInterface() )
        {
            // remove anything that has not a DurationInterface
            iter = childList->erase( iter );
        } else {
            // Drop notes that are signaled as grace notes
            Note *n = dynamic_cast<Note*>(currentElement);
            
            if (n) {
                // if we are at the beginning of the beam
                // and the note is cueSize
                // assume all the beam is of grace notes
                if (childList->begin() == iter) {
                  if (n->HasGrace())
                      firstNoteGrace = true;
                }
                
                // if the first note in beam was NOT a grace
                // we have grace notes embedded in a beam
                // drop them
                if ( !firstNoteGrace && n->HasGrace() == true)
                    iter = childList->erase( iter );
                else
                    iter++;
                
            } else {
                // if it is a Rest, do not drop
                iter++;
            }
        }
    }
    
    InitCoords( childList );
}
  
void Beam::InitCoords( ListOfObjects *childList )
{
    this->ClearCoords();
    m_beamElementCoords.reserve( childList->size() );
    int i;
    for (i = 0; i < childList->size(); i++ ) {
        m_beamElementCoords.push_back( new BeamElementCoord() );
    }
}
    
void Beam::ClearCoords()
{
    ArrayOfBeamElementCoords::iterator iter;
    for (iter = m_beamElementCoords.begin(); iter != m_beamElementCoords.end(); ++iter)
    {
        delete *iter;
    }
    m_beamElementCoords.clear();
}
    
//----------------------------------------------------------------------------
// BeamElementCoord
//----------------------------------------------------------------------------

BeamElementCoord::~BeamElementCoord()
{
    if (m_element) m_element->m_beamElementCoord = NULL;
}

} // namespace vrv

