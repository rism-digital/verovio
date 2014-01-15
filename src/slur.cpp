/////////////////////////////////////////////////////////////////////////////
// Name:        slur.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "slur.h"

namespace vrv {

//----------------------------------------------------------------------------
// Slur
//----------------------------------------------------------------------------

Slur::Slur():
LayerElement("slur-")
{
}


Slur::~Slur()
{
}

void Slur::AddNote(LayerElement *element) {
    
    if (!element->HasDurationInterface()) {
        return;
    }

    m_children.push_back(element);
    Modify();
    
}

} // namespace vrv