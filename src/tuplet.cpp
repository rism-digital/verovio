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

Tuplet::Tuplet()
    : LayerElement("tuplet-"), ObjectListInterface(), AttDurationRatio(), AttNumberplacement(), AttTupletVis()
{
    RegisterAttClass(ATT_DURATIONRATIO);
    RegisterAttClass(ATT_NUMBERPLACEMENT);
    RegisterAttClass(ATT_TUPLETVIS);

    Reset();
}

Tuplet::~Tuplet()
{
}

void Tuplet::Reset()
{
    LayerElement::Reset();
    ResetDurationRatio();
    ResetNumberplacement();
    ResetTupletVis();
}

void Tuplet::AddLayerElement(LayerElement *element)
{
    // if (!element->HasInterface(INTERFACE_DURATION)()) {
    //    return;
    //}

    element->SetParent(this);
    m_children.push_back(element);
    Modify();
}

void Tuplet::FilterList(ListOfObjects *childList)
{
    // We want to keep only notes and rest
    // Eventually, we also need to filter out grace notes properly (e.g., with sub-beams)
    ListOfObjects::iterator iter = childList->begin();

    while (iter != childList->end()) {
        if (!(*iter)->IsLayerElement() || !(*iter)->HasInterface(INTERFACE_DURATION)) {
            iter = childList->erase(iter);
        }
        else {
            iter++;
        }
    }
}

} // namespace vrv
