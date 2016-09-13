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

Ligature::Ligature()
    : LayerElement("ligature-")
    , ObjectListInterface()
    , StemmedDrawingInterface()
    , DurationInterface()
    , AttCommon()
    , AttStems()
    , AttStemsCmn()
    , AttTiepresent()
{
    RegisterInterface(DurationInterface::GetAttClasses(), DurationInterface::IsInterface());
    RegisterAttClass(ATT_COMMON);
    RegisterAttClass(ATT_STEMS);
    RegisterAttClass(ATT_STEMSCMN);
    RegisterAttClass(ATT_TIEPRESENT);

    Reset();

    m_drawingStemDir = STEMDIRECTION_NONE;
}

Ligature::~Ligature()
{
    ClearClusters();
}

void Ligature::Reset()
{
    LayerElement::Reset();
    StemmedDrawingInterface::Reset();
    DurationInterface::Reset();
    ResetCommon();
    ResetStems();
    ResetStemsCmn();
    ResetTiepresent();

    ClearClusters();
}

void Ligature::ClearClusters()
{
}

void Ligature::AddChild(Object *child)
{
    if (child->Is() == NOTE) {
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

bool compare_pitchL(Object *first, Object *second)
{
    Note *n1 = dynamic_cast<Note *>(first);
    Note *n2 = dynamic_cast<Note *>(second);
    assert(n1 && n2);
    return (n1->GetDiatonicPitch() < n2->GetDiatonicPitch());
}

void Ligature::FilterList(ListOfObjects *childList)
{
    // Retain only note children of ligatures
    ListOfObjects::iterator iter = childList->begin();

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
                iter++;
            }
            else {
                // if it is not a note, drop it
                iter = childList->erase(iter);
            }
        }
    }

    childList->sort(compare_pitchL);

    iter = childList->begin();

    this->ClearClusters();

    Note *curNote, *lastNote = dynamic_cast<Note *>(*iter);
    assert(lastNote);
    int curPitch, lastPitch = lastNote->GetDiatonicPitch();

    iter++;

    while (iter != childList->end()) {
        curNote = dynamic_cast<Note *>(*iter);
        assert(curNote);
        curPitch = curNote->GetDiatonicPitch();

        if (curPitch - lastPitch == 1) {
        }

        lastNote = curNote;
        lastPitch = curPitch;

        iter++;
    }
}

void Ligature::ResetAccidList()
{
    ListOfObjects *childList = this->GetList(this); // make sure it's initialized
    for (ListOfObjects::reverse_iterator it = childList->rbegin(); it != childList->rend(); it++) {
        Note *note = dynamic_cast<Note *>(*it);
        assert(note);
        if (note->m_drawingAccid != NULL) {
        }
    }
}

int Ligature::PositionInLigature(Note *note)
{
    int size = (int)this->GetList(this)->size();
    int position = this->GetListIndex(note);
    assert(position != -1);
    // this is the middle (only if odd)
    if ((size % 2) && (position == (size - 1) / 2)) return 0;
    if (position < (size / 2)) return -1;
    return 1;
}

void Ligature::SetDrawingStemDir(data_STEMDIRECTION stemDir)
{
    m_drawingStemDir = stemDir;
    ListOfObjects *childList = this->GetList(this); // make sure it's initialized
    for (ListOfObjects::iterator it = childList->begin(); it != childList->end(); it++) {
        Note *note = dynamic_cast<Note *>(*it);
        if (!note) continue;
        note->SetDrawingStemDir(stemDir);
    }
}

void Ligature::SetDrawingStemStart(Point stemStart)
{
    m_drawingStemStart = stemStart;
    ListOfObjects *childList = this->GetList(this); // make sure it's initialized
    for (ListOfObjects::iterator it = childList->begin(); it != childList->end(); it++) {
        Note *note = dynamic_cast<Note *>(*it);
        if (!note) continue;
        note->SetDrawingStemStart(stemStart);
    }
}

void Ligature::SetDrawingStemEnd(Point stemEnd)
{
    m_drawingStemEnd = stemEnd;
    ListOfObjects *childList = this->GetList(this); // make sure it's initialized
    for (ListOfObjects::iterator it = childList->begin(); it != childList->end(); it++) {
        Note *note = dynamic_cast<Note *>(*it);
        if (!note) continue;
        note->SetDrawingStemEnd(stemEnd);
    }
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------
}