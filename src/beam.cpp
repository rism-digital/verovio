/////////////////////////////////////////////////////////////////////////////
// Name:        musbeam.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "vrv.h"
#include "beam.h"

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
    // We want to keep only notes and rest
    // Eventually, we also need to filter out grace notes properly (e.g., with sub-beams)
    ListOfObjects::iterator iter = m_list.begin();
    
    while ( iter != m_list.end()) {
        LayerElement *currentElement = dynamic_cast<LayerElement*>(*iter);
        if ( currentElement && !currentElement->HasDurationInterface() )
        {
            //LogDebug("KILLED!!! %s", currentElement->MusClassName().c_str() );
            iter = m_list.erase( iter );
        } else {
            iter++;
        }
    }
    
}

} // namespace vrv

