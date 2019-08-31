/////////////////////////////////////////////////////////////////////////////
// Name:        ligature.cpp
// Author:      Don Byrd
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "ligature.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <iostream>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "note.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Ligature
//----------------------------------------------------------------------------

Ligature::Ligature() : LayerElement("ligature-"), ObjectListInterface(), AttLigatureLog()
{
    RegisterAttClass(ATT_LIGATURELOG);

    Reset();
}

Ligature::~Ligature()
{
    ClearClusters();
}

void Ligature::Reset()
{
    LayerElement::Reset();
    ResetLigatureLog();

    ClearClusters();
}

void Ligature::ClearClusters() {}

void Ligature::AddChild(Object *child)
{
    if (child->Is(NOTE)) {
        assert(dynamic_cast<LayerElement *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }

    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

void Ligature::FilterList(ArrayOfObjects *childList)
{
    // Retain only note children of ligatures
    ArrayOfObjects::iterator iter = childList->begin();

    while (iter != childList->end()) {
        if (!(*iter)->IsLayerElement()) {
            // remove anything that is not an LayerElement
            iter = childList->erase(iter);
            continue;
        }
        LayerElement *currentElement = dynamic_cast<LayerElement *>(*iter);
        assert(currentElement);
        if (!currentElement->HasInterface(INTERFACE_DURATION)) {
            iter = childList->erase(iter);
        }
        else {
            Note *n = dynamic_cast<Note *>(currentElement);

            if (n) {
                ++iter;
            }
            else {
                // if it is not a note, drop it
                iter = childList->erase(iter);
            }
        }
    }

    iter = childList->begin();

    this->ClearClusters();
}

int Ligature::PositionInLigature(Note *note)
{
    this->GetList(this);
    int position = this->GetListIndex(note);
    assert(position != -1);
    return position;
}

Note *Ligature::GetFirstNote()
{
    const ArrayOfObjects *list = this->GetList(this);
    if (list->empty()) {
        return NULL;
    }
    return dynamic_cast<Note *>(list->front());
}

Note *Ligature::GetLastNote()
{
    const ArrayOfObjects *list = this->GetList(this);
    if (list->empty()) {
        return NULL;
    }
    return dynamic_cast<Note *>(list->back());
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Ligature::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);

    // We want the list of the ObjectListInterface to be re-generated
    this->Modify();
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
