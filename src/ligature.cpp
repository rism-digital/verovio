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

#include "note.h"

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
    m_drawingLedgerLines.clear();
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
    std::list<LigatureCluster *>::iterator iter;
    for (iter = m_clusters.begin(); iter != m_clusters.end(); ++iter) {
        for (std::vector<Note *>::iterator clIter = (*iter)->begin(); clIter != (*iter)->end(); ++clIter) {
            (*clIter)->m_cluster = NULL;
            (*clIter)->m_clusterPosition = 0;
        }
        delete *iter;
    }
    m_clusters.clear();
}

void Ligature::AddLayerElement(LayerElement *element)
{
    assert(dynamic_cast<Note *>(element));
    element->SetParent(this);
    m_children.push_back(element);
    Modify();
}

bool compare_pitch(Object *first, Object *second)
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

    childList->sort(compare_pitch);

    iter = childList->begin();

    this->ClearClusters();

    Note *curNote, *lastNote = dynamic_cast<Note *>(*iter);
    assert(lastNote);
    int curPitch, lastPitch = lastNote->GetDiatonicPitch();
    LigatureCluster *curCluster = NULL;

    iter++;

    while (iter != childList->end()) {
        curNote = dynamic_cast<Note *>(*iter);
        assert(curNote);
        curPitch = curNote->GetDiatonicPitch();

        if (curPitch - lastPitch == 1) {
            if (!lastNote->m_cluster) {
                curCluster = new LigatureCluster();
                m_clusters.push_back(curCluster);
                curCluster->push_back(lastNote);
                lastNote->m_cluster = curCluster;
                lastNote->m_clusterPosition = (int)curCluster->size();
            }
            curCluster->push_back(curNote);
            curNote->m_cluster = curCluster;
            curNote->m_clusterPosition = (int)curCluster->size();
        }

        lastNote = curNote;
        lastPitch = curPitch;

        iter++;
    }
}

void Ligature::ResetAccidList()
{
    m_accidList.clear();
    ListOfObjects *childList = this->GetList(this); // make sure it's initialized
    for (ListOfObjects::reverse_iterator it = childList->rbegin(); it != childList->rend(); it++) {
        Note *note = dynamic_cast<Note *>(*it);
        assert(note);
        if (note->m_drawingAccid != NULL) {
            m_accidList.push_back(note);
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

void Ligature::GetYExtremes(int *yMax, int *yMin)
{
    bool passed = false;
    int y1;
    ListOfObjects *childList = this->GetList(this); // make sure it's initialized
    for (ListOfObjects::iterator it = childList->begin(); it != childList->end(); it++) {
        Note *note = dynamic_cast<Note *>(*it);
        if (!note) continue;
        y1 = note->GetDrawingY();
        if (!passed) {
            *yMax = y1;
            *yMin = y1;
            passed = true;
        }
        else {
            if (y1 > *yMax) {
                *yMax = y1;
            }
            else if (y1 < *yMin) {
                *yMin = y1;
            }
        }
    }
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

int Ligature::PrepareTieAttr(ArrayPtrVoid *params)
{
    // param 0: std::vector<Note*>* that holds the current notes with open ties (unused)
    ???NO SUCH
    // param 1: Ligature** currentLigature for the current ligature if in a ligature
    Ligature **currentLigature = static_cast<Ligature **>((*params).at(1));

    assert(!(*currentLigature));
    (*currentLigature) = this;

    return FUNCTOR_CONTINUE;
}

int Ligature::PrepareTieAttrEnd(ArrayPtrVoid *params)
{
    // param 0: std::vector<Note*>* that holds the current notes with open ties (unused)
    ???NO SUCH
    // param 1: Ligature** currentLigature for the current ligature if in a ligature
    Ligature **currentLigature = static_cast<Ligature **>((*params).at(1));

    assert((*currentLigature));
    (*currentLigature) = NULL;

    return FUNCTOR_CONTINUE;
}
}