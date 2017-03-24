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

#include "artic.h"
#include "doc.h"
#include "editorial.h"
#include "elementpart.h"
#include "functorparams.h"
#include "layer.h"
#include "note.h"
#include "smufl.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Chord
//----------------------------------------------------------------------------

Chord::Chord()
    : LayerElement("chord-")
    , ObjectListInterface()
    , DrawingListInterface()
    , StemmedDrawingInterface()
    , DurationInterface()
    , AttColor()
    , AttGraced()
    , AttRelativesize()
    , AttStems()
    , AttStemsCmn()
    , AttTiepresent()
    , AttVisibility()
{
    RegisterInterface(DurationInterface::GetAttClasses(), DurationInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_GRACED);
    RegisterAttClass(ATT_RELATIVESIZE);
    RegisterAttClass(ATT_STEMS);
    RegisterAttClass(ATT_STEMSCMN);
    RegisterAttClass(ATT_TIEPRESENT);
    RegisterAttClass(ATT_VISIBILITY);

    Reset();

    m_drawingLedgerLines.clear();
}

Chord::~Chord()
{
    ClearClusters();
}

void Chord::Reset()
{
    LayerElement::Reset();
    DrawingListInterface::Reset();
    StemmedDrawingInterface::Reset();
    DurationInterface::Reset();
    ResetColor();
    ResetGraced();
    ResetRelativesize();
    ResetStems();
    ResetStemsCmn();
    ResetTiepresent();
    ResetVisibility();

    ClearClusters();
}

void Chord::ClearClusters() const
{
    std::list<ChordCluster *>::iterator iter;
    for (iter = m_clusters.begin(); iter != m_clusters.end(); ++iter) {
        for (std::vector<Note *>::iterator clIter = (*iter)->begin(); clIter != (*iter)->end(); ++clIter) {
            (*clIter)->m_cluster = NULL;
            (*clIter)->m_clusterPosition = 0;
        }
        delete *iter;
    }
    m_clusters.clear();
}

void Chord::AddChild(Object *child)
{
    if (child->Is(ARTIC)) {
        assert(dynamic_cast<Artic *>(child));
    }
    else if (child->Is(NOTE)) {
        assert(dynamic_cast<Note *>(child));
    }
    else if (child->Is(NOTEHEAD)) {
        assert(dynamic_cast<NoteHead *>(child));
    }
    else if (child->Is(STEM)) {
        assert(dynamic_cast<Stem *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    // Stem are always added by PrepareLayerElementParts (for now) and we want them to be in the front
    // for the drawing order in the SVG output
    if (child->Is(STEM))
        m_children.insert(m_children.begin(), child);
    else
        m_children.push_back(child);
    Modify();
}

void Chord::FilterList(ListOfObjects *childList)
{
    // Retain only note children of chords
    ListOfObjects::iterator iter = childList->begin();

    while (iter != childList->end()) {
        if ((*iter)->Is(NOTE))
            iter++;
        else
            iter = childList->erase(iter);
    }

    std::sort(childList->begin(), childList->end(), DiatonicSort());

    iter = childList->begin();

    this->ClearClusters();

    Note *curNote, *lastNote = dynamic_cast<Note *>(*iter);
    assert(lastNote);
    int curPitch, lastPitch = lastNote->GetDiatonicPitch();
    ChordCluster *curCluster = NULL;

    iter++;

    while (iter != childList->end()) {
        curNote = dynamic_cast<Note *>(*iter);
        assert(curNote);
        curPitch = curNote->GetDiatonicPitch();

        if ((curPitch - lastPitch < 2) && (curNote->GetCrossStaff() == lastNote->GetCrossStaff())) {
            if (!lastNote->m_cluster) {
                curCluster = new ChordCluster();
                m_clusters.push_back(curCluster);
                curCluster->push_back(lastNote);
                lastNote->m_cluster = curCluster;
                lastNote->m_clusterPosition = (int)curCluster->size();
            }
            assert(curCluster);
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
    ListOfObjects *childList = this->GetList(this); // make sure it's initialized
    for (ListOfObjects::reverse_iterator it = childList->rbegin(); it != childList->rend(); it++) {
        Note *note = dynamic_cast<Note *>(*it);
        assert(note);
        Accid *accid = note->GetDrawingAccid();
        if (accid && accid->HasAccid()) {
            m_accidList.push_back(note);
        }
    }
}

int Chord::PositionInChord(Note *note)
{
    int size = (int)this->GetList(this)->size();
    int position = this->GetListIndex(note);
    assert(position != -1);
    // this is the middle (only if odd)
    if ((size % 2) && (position == (size - 1) / 2)) return 0;
    if (position < (size / 2)) return -1;
    return 1;
}

/**
 * Creates a 2D grid of width (# of accidentals + 1) * 4 and of height (highest accid - lowest accid) / (half a drawing
 * unit)
 */
void Chord::ResetAccidSpace(int fullUnit)
{
    m_accidSpace.clear();
    m_accidSpaceBot = 0;
    m_accidSpaceTop = 0;

    // if there are no accidentals in the chord, we don't need to plan for any
    if (m_accidList.size() == 0) return;

    // dimensional units, other variables
    int halfUnit = fullUnit / 2;
    int idx, setIdx;

    /*
     * Prepare for the situation where every accidental conflicts horizontally:
     *    -Assume each accidental to be 2 drawing units wide, drawn to 1/2-unit detail (ACCID_WIDTH should be
     * represented in half-units)
     *    -Prepare each line to account for one extra accidental so we can guarantee the grid has enough space
     *    -Set m_accidSpaceLeft to be used for asserts during drawing
     */
    int accidLineLength = (int)m_accidList.size() * ACCID_WIDTH;

    /*
     * Each accidental's Y position will be its vertical center; set the grid extremes to account for that
     * Resize m_accidSpace to be as tall as possibly necessary; must accomodate every accidental stacked vertically.
     */
    int accidHeight = ACCID_HEIGHT * halfUnit;
    int yMax = 0, yMin = 0;
    this->GetYExtremes(yMax, yMin);
    m_accidSpaceTop = yMax + (accidHeight / 2);
    m_accidSpaceBot = yMin - (accidHeight / 2);
    int height = (m_accidSpaceTop - m_accidSpaceBot) / halfUnit;
    assert(height >= 0);
    m_accidSpace.resize(height);

    // Resize each row in m_accidSpace to be the proper length; set all the bools to false
    std::vector<bool> *accidLine;
    for (idx = 0; idx < (int)m_accidSpace.size(); idx++) {
        accidLine = &m_accidSpace.at(idx);
        accidLine->resize(accidLineLength);
        for (setIdx = 0; setIdx < accidLineLength; setIdx++) accidLine->at(setIdx) = false;
    }
}

void Chord::GetYExtremes(int &yMax, int &yMin)
{
    ListOfObjects *childList = this->GetList(this); // make sure it's initialized
    assert(childList->size() > 0);

    // The first note is the bottom
    yMin = childList->front()->GetDrawingY();
    // The last note is the top
    yMax = childList->back()->GetDrawingY();
}

int Chord::GetYTop()
{
    ListOfObjects *childList = this->GetList(this); // make sure it's initialized
    assert(childList->size() > 0);

    // The last note is the top
    return childList->back()->GetDrawingY();
}

int Chord::GetYBottom()
{
    ListOfObjects *childList = this->GetList(this); // make sure it's initialized
    assert(childList->size() > 0);

    // The first note is the bottom
    return childList->front()->GetDrawingY();
}

Note *Chord::GetTopNote()
{
    ListOfObjects *childList = this->GetList(this); // make sure it's initialized
    assert(childList->size() > 0);

    Note *topNote = dynamic_cast<Note *>(childList->back());
    assert(topNote);
    return topNote;
}

Note *Chord::GetBottomNote()
{
    ListOfObjects *childList = this->GetList(this); // make sure it's initialized
    assert(childList->size() > 0);

    // The first note is the bottom
    Note *bottomNote = dynamic_cast<Note *>(childList->front());
    assert(bottomNote);
    return bottomNote;
}

void Chord::GetCrossStaffExtremes(Staff *&staffAbove, Staff *&staffBelow)
{
    staffAbove = NULL;
    staffBelow = NULL;

    // We assume that we have a cross-staff chord we cannot have further cross-staffed notes
    if (m_crossStaff) return;

    // The first note is the bottom
    Note *bottomNote = this->GetBottomNote();
    assert(bottomNote);
    if (bottomNote->m_crossStaff && bottomNote->m_crossLayer) {
        staffBelow = bottomNote->m_crossStaff;
    }

    // The last note is the top
    Note *topNote = this->GetTopNote();
    assert(topNote);
    if (topNote->m_crossStaff && topNote->m_crossLayer) {
        staffAbove = topNote->m_crossStaff;
    }
}

bool Chord::HasCrossStaff()
{
    Staff *staffAbove = NULL;
    Staff *staffBelow = NULL;

    this->GetCrossStaffExtremes(staffAbove, staffBelow);

    return (staffAbove != staffBelow);
}

Point Chord::GetStemUpSE(Doc *doc, int staffSize, bool graceSize)
{
    Note *bottomNote = this->GetBottomNote();
    assert(bottomNote);
    return bottomNote->GetStemUpSE(doc, staffSize, graceSize);
}

Point Chord::GetStemDownNW(Doc *doc, int staffSize, bool graceSize)
{
    Note *topNote = this->GetTopNote();
    assert(topNote);
    return topNote->GetStemDownNW(doc, staffSize, graceSize);
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Chord::CalcStem(FunctorParams *functorParams)
{
    CalcStemParams *params = dynamic_cast<CalcStemParams *>(functorParams);
    assert(params);

    // Set them to NULL in any case
    params->m_interface = NULL;

    // Stems have been calculated previously in Beam or FTrem - siblings becasue flags do not need to
    // be processed either
    if (this->IsInBeam() || this->IsInFTrem()) {
        return FUNCTOR_SIBLINGS;
    }

    // No stem
    if (this->GetActualDur() < DUR_2) {
        LogDebug("Duratin is longer than halfnote, there should be no stem");
        return FUNCTOR_SIBLINGS;
    }

    Stem *stem = this->GetDrawingStem();
    assert(stem);
    Staff *staff = dynamic_cast<Staff *>(this->GetFirstParent(STAFF));
    assert(staff);
    Layer *layer = dynamic_cast<Layer *>(this->GetFirstParent(LAYER));
    assert(layer);

    if (this->m_crossStaff) staff = this->m_crossStaff;

    // Cache the in params to avoid further lookup
    params->m_staff = staff;
    params->m_layer = layer;
    params->m_interface = this;
    params->m_dur = this->GetActualDur();

    /************ Set the direction ************/

    int yMax, yMin;
    this->GetYExtremes(yMax, yMin);
    params->m_chordStemLength = yMin - yMax;

    int staffY = staff->GetDrawingY();
    int staffSize = staff->m_drawingStaffSize;
    params->m_verticalCenter = staffY - params->m_doc->GetDrawingDoubleUnit(staffSize) * 2;

    data_STEMDIRECTION stemDir = STEMDIRECTION_NONE;

    if (stem->HasStemDir()) {
        stemDir = stem->GetStemDir();
    }
    else if (layer->GetDrawingStemDir() != STEMDIRECTION_NONE) {
        stemDir = layer->GetDrawingStemDir();
    }
    else {
        stemDir = (yMax - params->m_verticalCenter >= params->m_verticalCenter - yMin) ? STEMDIRECTION_down
                                                                                       : STEMDIRECTION_up;
    }

    this->SetDrawingStemDir(stemDir);

    // Position the stem to the bottom note when up
    if (stemDir == STEMDIRECTION_up) stem->SetDrawingYRel(yMin - this->GetDrawingY());
    // And to the top note when down
    else
        stem->SetDrawingYRel(0);

    return FUNCTOR_CONTINUE;
}

int Chord::PrepareLayerElementParts(FunctorParams *functorParams)
{
    Stem *currentStem = dynamic_cast<Stem *>(this->FindChildByType(STEM));
    Flag *currentFlag = NULL;
    if (currentStem) currentFlag = dynamic_cast<Flag *>(currentStem->FindChildByType(FLAG));

    if (this->GetDur() > DUR_1) {
        if (!currentStem) {
            currentStem = new Stem();
            this->AddChild(currentStem);
        }
        currentStem->AttStems::operator=(*this);
        currentStem->AttStemsCmn::operator=(*this);
    }
    // This will happen only if the duration has changed
    else if (currentStem) {
        if (this->DeleteChild(currentStem)) {
            currentStem = NULL;
            // The currentFlag (if any) will have been deleted above
            currentFlag = NULL;
        }
    }

    if ((this->GetDur() > DUR_4) && !this->IsInBeam()) {
        // We should have a stem at this stage
        assert(currentStem);
        if (!currentFlag) {
            currentFlag = new Flag();
            currentStem->AddChild(currentFlag);
        }
    }
    // This will happen only if the duration has changed (no flag required anymore)
    else if (currentFlag) {
        assert(currentStem);
        if (currentStem->DeleteChild(currentFlag)) currentFlag = NULL;
    }

    SetDrawingStem(currentStem);

    // Also set the drawing stem object (or NULL) to all child notes
    ListOfObjects *childList = this->GetList(this); // make sure it's initialized
    for (ListOfObjects::iterator it = childList->begin(); it != childList->end(); it++) {
        assert((*it)->Is(NOTE));
        Note *note = dynamic_cast<Note *>(*it);
        assert(note);
        note->SetDrawingStem(currentStem);
    }

    return FUNCTOR_CONTINUE;
};

int Chord::PrepareTieAttr(FunctorParams *functorParams)
{
    PrepareTieAttrParams *params = dynamic_cast<PrepareTieAttrParams *>(functorParams);
    assert(params);

    assert(!params->m_currentChord);
    params->m_currentChord = this;

    return FUNCTOR_CONTINUE;
}

int Chord::PrepareTieAttrEnd(FunctorParams *functorParams)
{
    PrepareTieAttrParams *params = dynamic_cast<PrepareTieAttrParams *>(functorParams);
    assert(params);

    assert(params->m_currentChord);
    params->m_currentChord = NULL;

    return FUNCTOR_CONTINUE;
}
}
