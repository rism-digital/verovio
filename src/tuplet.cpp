/////////////////////////////////////////////////////////////////////////////
// Name:        tuplet.cpp
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
    Reset();

}

Tuplet::Tuplet(int num, int numbase):
    LayerElement("tuplet-"), ObjectListInterface()
{
    Reset();
    m_num = num;
    m_numbase = numbase;
}

Tuplet::~Tuplet()
{
}

void Tuplet::Reset()
{
    LayerElement::Reset();
}

void Tuplet::AddLayerElement(LayerElement *element) {
    
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
