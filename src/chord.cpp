/////////////////////////////////////////////////////////////////////////////
// Name:        chord.cpp
// Author:      Andrew Horwitz
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "chord.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <iostream>

//----------------------------------------------------------------------------

#include "note.h"

namespace vrv {
    
//----------------------------------------------------------------------------
// Chord
//----------------------------------------------------------------------------

Chord::Chord( ):
LayerElement("chord-"), ObjectListInterface(), DurationInterface(),
    AttCommon(),
    AttStemmed(),
    AttTiepresent()
{
    Reset();
    m_drawingStemDir = STEMDIRECTION_NONE;
    m_drawingLedgerLines.clear();
    
    //test[NULL][0][0] = 0;
}

Chord::~Chord()
{
    ClearClusters();
}

void Chord::Reset()
{
    ClearClusters();
    DocObject::Reset();
    DurationInterface::Reset();
    ResetCommon();
    ResetStemmed();
    ResetTiepresent();
}
    
void Chord::ClearClusters()
{
    std::list<ChordCluster*>::iterator iter;
    for (iter = m_clusters.begin(); iter != m_clusters.end(); ++iter)
    {
        ChordCluster *cluster = dynamic_cast<ChordCluster*>(*iter);
        for (std::vector<Note*>::iterator clIter = cluster->begin(); clIter != cluster->end(); ++clIter)
        {
            Note *note = dynamic_cast<Note*>(*clIter);
            note->m_cluster = NULL;
            note->m_clusterPosition = 0;
        }
        delete *iter;
    }
    m_clusters.clear();
}
    
void Chord::AddLayerElement(vrv::LayerElement *element)
{
    assert( dynamic_cast<Note*>(element) );
    element->SetParent( this );
    m_children.push_back(element);
    Modify();
}
    
bool compare_pitch (Object *first, Object *second)
{
    Note *n1 = dynamic_cast<Note*>(first);
    Note *n2 = dynamic_cast<Note*>(second);
    return ( n1->GetDiatonicPitch() < n2->GetDiatonicPitch() );
}

void Chord::FilterList( ListOfObjects *childList )
{
    // Retain only note children of chords
    ListOfObjects::iterator iter = childList->begin();
    
    while ( iter != childList->end()) {
        LayerElement *currentElement = dynamic_cast<LayerElement*>(*iter);
        if ( !currentElement ) {
            // remove anything that is not an LayerElement
            iter = childList->erase( iter );
        }
        else if ( !currentElement->HasDurationInterface() )
        {
            iter = childList->erase( iter );
        }
        else /*if ( dynamic_cast<EditorialElement*>(currentElement))
        {
            Object* object = currentElement->GetFirstChild(&typeid(Note));
            if (dynamic_cast<Note*>(object))
            {
                iter++;
            }
        }
        else */{
            Note *n = dynamic_cast<Note*>(currentElement);
            
            if (n) {
                iter++;
            } else {
                // if it is not a note, drop it
                iter = childList->erase( iter );
            }
        }
    }
    
    childList->sort(compare_pitch);
    
    iter = childList->begin();
    
    this->ClearClusters();
    
    Note *curNote, *lastNote = dynamic_cast<Note*>(*iter);
    int curPitch, lastPitch = lastNote->GetDiatonicPitch();
    ChordCluster* curCluster = NULL;
    
    iter++;
    
    while ( iter != childList->end()) {
        curNote = dynamic_cast<Note*>(*iter);
        curPitch = curNote->GetDiatonicPitch();
        
        if (curPitch - lastPitch == 1) {
            if(!lastNote->m_cluster)
            {
                curCluster = new ChordCluster();
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

void Chord::ResetAccidList()
{
    m_accidList.clear();
    ListOfObjects* childList = this->GetList(this); //make sure it's initialized
    for (ListOfObjects::reverse_iterator it = childList->rbegin(); it != childList->rend(); it++) {
        Note *note = dynamic_cast<Note*>(*it);
        if (note->m_drawingAccid != NULL) {
            m_accidList.push_back(note);
        }
    }
}
    
/**
 * Creates a 2D grid of width (# of accidentals + 1) * 4 and of height (highest accid - lowest accid) / (half a drawing unit)
 */
void Chord::ResetAccidSpace(int fullUnit)
{
    m_accidSpace.clear();
    m_accidSpaceBot = 0;
    m_accidSpaceTop = 0;
    
    //if there are no accidentals in the chord, we don't need to plan for any
    if (m_accidList.size() == 0) return;
    
    //dimensional units, other variables
    int halfUnit = fullUnit / 2;
    int idx, setIdx;
    
    /*
     * Prepare for the situation where every accidental conflicts horizontally:
     *    -Assume each accidental to be 2 drawing units wide, drawn to 1/2-unit detail (ACCID_WIDTH should be represented in half-units)
     *    -Prepare each line to account for one extra accidental so we can guarantee the grid has enough space
     *    -Set m_accidSpaceLeft to be used for asserts during drawing
     */
    int accidLineLength = (int)m_accidList.size() * ACCID_WIDTH;
    
    /*
     * Each accidental's Y position will be its vertical center; set the grid extremes to account for that
     * Resize m_accidSpace to be as tall as is possibly necessary; must accomodate every accidental stacked vertically.
     */
    int accidHeight = ACCID_HEIGHT * halfUnit;
    int yMax, yMin;
    this->GetYExtremes(&yMax, &yMin);
    m_accidSpaceTop = yMax + (accidHeight / 2);
    m_accidSpaceBot = yMin - (accidHeight / 2);
    int height = (m_accidSpaceTop - m_accidSpaceBot) / halfUnit;
    m_accidSpace.resize(height);
    
    //Resize each row in m_accidSpace to be the proper length; set all the bools to false
    std::vector<bool> *accidLine;
    for(idx = 0; idx < m_accidSpace.size(); idx++)
    {
        accidLine = &m_accidSpace.at(idx);
        accidLine->resize(accidLineLength);
        for(setIdx = 0; setIdx < accidLineLength; setIdx++) accidLine->at(setIdx) = false;
    }
}
    
void Chord::GetYExtremes(int *yMax, int *yMin)
{
    bool passed = false;
    int y1;
    ListOfObjects* childList = this->GetList(this); //make sure it's initialized
    for (ListOfObjects::iterator it = childList->begin(); it != childList->end(); it++) {
        Note *note = dynamic_cast<Note*>(*it);
        if (!note) continue;
        y1 = note->GetDrawingY();
        if (!passed) {
            *yMax = y1;
            *yMin = y1;
            passed = true;
        }
        else {
            if (y1 > *yMax) *yMax = y1;
            else if (y1 < *yMin) *yMin = y1;
        }
    }
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Chord::PrepareTieAttr( ArrayPtrVoid params )
{
    // param 0: std::vector<Note*>* that holds the current notes with open ties (unused)
    // param 1: Chord** currentChord for the current chord if in a chord
    Chord **currentChord = static_cast<Chord**>(params[1]);
    
    assert(!(*currentChord));
    (*currentChord) = this;

    return FUNCTOR_CONTINUE;
}

int Chord::PrepareTieAttrEnd( ArrayPtrVoid params )
{
    // param 0: std::vector<Note*>* that holds the current notes with open ties (unused)
    // param 1: Chord** currentChord for the current chord if in a chord
    Chord **currentChord = static_cast<Chord**>(params[1]);
    
    assert((*currentChord));
    (*currentChord) = NULL;
    
    return FUNCTOR_CONTINUE;
}
    
}