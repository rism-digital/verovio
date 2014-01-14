/////////////////////////////////////////////////////////////////////////////
// Name:        musslur.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "tuplet.h"

namespace vrv {

//----------------------------------------------------------------------------
// Tuplet
//----------------------------------------------------------------------------

Tuplet::Tuplet():
LayerElement("tuplet-"), ObjectListInterface()
{
    m_num = 1;
    m_numbase = 1;
}


Tuplet::~Tuplet()
{
}

void Tuplet::AddElement(LayerElement *element) {
    
    //if (!element->HasDurationInterface()) {
    //    return;
    //}
    
    element->SetParent( this );
    m_children.push_back(element);
    Modify();
}

void Tuplet::FilterList()
{
    // We want to keep only notes and rest
    // Eventually, we also need to filter out grace notes properly (e.g., with sub-beams)
    ListOfObjects::iterator iter = m_list.begin();
    
    while ( iter != m_list.end()) {
        LayerElement *currentElement = dynamic_cast<LayerElement*>(*iter);
        if ( currentElement && !currentElement->HasDurationInterface() )
        {
            iter = m_list.erase( iter );
        } else {
            iter++;
        }
    }
    
}

} // namespace vrv
