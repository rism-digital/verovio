/////////////////////////////////////////////////////////////////////////////
// Name:        chord.cpp
// Author:      Andrew Horwitz
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "chord.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <assert.h>
#include <functional>
#include <iostream>
#include <numeric>

//----------------------------------------------------------------------------

#include "artic.h"
#include "comparison.h"
#include "doc.h"
#include "editorial.h"
#include "elementpart.h"
#include "fermata.h"
#include "functorparams.h"
#include "horizontalaligner.h"
#include "layer.h"
#include "note.h"
#include "smufl.h"
#include "staff.h"
#include "verse.h"
#include "verticalaligner.h"
#include "vrv.h"

namespace vrv {

// Helper template function to calculate optiomal dot locations based on note locations in the chord. Takes iterators
// for the begin/end of the range; reverse iterators should be passed if reverse order is specified
template <typename Iterator> std::set<int> CalculateDotLocations(Iterator begin, Iterator end, bool isReverseOrder)
{
    // location adjustment that should be applied when looking for optimal position
    std::vector<int> locAdjust{ 0, 1, -1, -2, 2 };
    if (isReverseOrder) std::transform(locAdjust.begin(), locAdjust.end(), locAdjust.begin(), std::negate<int>());
    std::set<int> dotLocations;
    Iterator prev = begin;
    for (auto iter = begin; iter != end; ++iter) {
        bool result = false;
        for (int adjust : locAdjust) {
            if ((*iter + adjust) % 2 == 0) continue;
            if ((prev != iter) && (*prev == *iter) && (adjust == -2)) continue;
            std::tie(std::ignore, result) = dotLocations.insert(*iter + adjust);
            if (result) break;
        }
        prev = iter;
    }
    return dotLocations;
}

//----------------------------------------------------------------------------
// Chord
//----------------------------------------------------------------------------

static const ClassRegistrar<Chord> s_factory("chord", CHORD);

Chord::Chord()
    : LayerElement("chord-")
    , ObjectListInterface()
    , DrawingListInterface()
    , StemmedDrawingInterface()
    , DurationInterface()
    , AttColor()
    , AttCue()
    , AttGraced()
    , AttStems()
    , AttStemsCmn()
    , AttTiePresent()
    , AttVisibility()
{
    RegisterInterface(DurationInterface::GetAttClasses(), DurationInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_CUE);
    RegisterAttClass(ATT_GRACED);
    RegisterAttClass(ATT_STEMS);
    RegisterAttClass(ATT_STEMSCMN);
    RegisterAttClass(ATT_TIEPRESENT);
    RegisterAttClass(ATT_VISIBILITY);

    Reset();
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
    ResetCue();
    ResetGraced();
    ResetStems();
    ResetStemsCmn();
    ResetTiePresent();
    ResetVisibility();

    ClearClusters();
}

void Chord::ClearClusters() const
{
    std::list<ChordCluster *>::iterator iter;
    for (iter = m_clusters.begin(); iter != m_clusters.end(); ++iter) {
        for (std::vector<Note *>::iterator clIter = (*iter)->begin(); clIter != (*iter)->end(); ++clIter) {
            (*clIter)->SetCluster(NULL, 0);
        }
        delete *iter;
    }
    m_clusters.clear();
}

bool Chord::IsSupportedChild(Object *child)
{
    if (child->Is(ARTIC)) {
        assert(dynamic_cast<Artic *>(child));
    }
    else if (child->Is(DOTS)) {
        assert(dynamic_cast<Dots *>(child));
    }
    else if (child->Is(NOTE)) {
        assert(dynamic_cast<Note *>(child));
    }
    else if (child->Is(STEM)) {
        assert(dynamic_cast<Stem *>(child));
    }
    else if (child->Is(VERSE)) {
        assert(dynamic_cast<Verse *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

void Chord::AddChild(Object *child)
{
    if (!this->IsSupportedChild(child)) {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        return;
    }

    ArrayOfObjects *children = this->GetChildrenForModification();

    child->SetParent(this);
    // Stem are always added by PrepareLayerElementParts (for now) and we want them to be in the front
    // for the drawing order in the SVG output
    if (child->Is({ DOTS, STEM })) {
        children->insert(children->begin(), child);
    }
    else {
        children->push_back(child);
    }
    Modify();
}

void Chord::FilterList(ArrayOfObjects *childList)
{
    // Retain only note children of chords
    ArrayOfObjects::iterator iter = childList->begin();

    while (iter != childList->end()) {
        if ((*iter)->Is(NOTE))
            ++iter;
        else
            iter = childList->erase(iter);
    }

    std::sort(childList->begin(), childList->end(), DiatonicSort());

    if (childList->empty()) {
        LogWarning("Chord '%s' has no child note - a default note is added", this->GetUuid().c_str());
        Note *rescueNote = new Note();
        this->AddChild(rescueNote);
        childList->push_back(rescueNote);
    }

    iter = childList->begin();

    this->ClearClusters();

    Note *curNote, *lastNote = vrv_cast<Note *>(*iter);
    assert(lastNote);
    int curPitch, lastPitch = lastNote->GetDiatonicPitch();
    ChordCluster *curCluster = NULL;

    ++iter;

    Layer *layer1 = NULL;
    Layer *layer2 = NULL;

    while (iter != childList->end()) {
        curNote = vrv_cast<Note *>(*iter);
        assert(curNote);
        curPitch = curNote->GetDiatonicPitch();

        if ((curPitch - lastPitch < 2) && (curNote->GetCrossStaff(layer1) == lastNote->GetCrossStaff(layer2))) {
            if (!lastNote->GetCluster()) {
                curCluster = new ChordCluster();
                m_clusters.push_back(curCluster);
                curCluster->push_back(lastNote);
                lastNote->SetCluster(curCluster, (int)curCluster->size());
            }
            assert(curCluster);
            curCluster->push_back(curNote);
            curNote->SetCluster(curCluster, (int)curCluster->size());
        }

        lastNote = curNote;
        lastPitch = curPitch;

        ++iter;
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

void Chord::GetYExtremes(int &yMax, int &yMin)
{
    const ArrayOfObjects *childList = this->GetList(this); // make sure it's initialized
    assert(childList->size() > 0);

    // The first note is the bottom
    yMin = childList->front()->GetDrawingY();
    // The last note is the top
    yMax = childList->back()->GetDrawingY();
}

int Chord::GetYTop()
{
    const ArrayOfObjects *childList = this->GetList(this); // make sure it's initialized
    assert(childList->size() > 0);

    // The last note is the top
    return childList->back()->GetDrawingY();
}

int Chord::GetYBottom()
{
    const ArrayOfObjects *childList = this->GetList(this); // make sure it's initialized
    assert(childList->size() > 0);

    // The first note is the bottom
    return childList->front()->GetDrawingY();
}

Note *Chord::GetTopNote()
{
    const ArrayOfObjects *childList = this->GetList(this); // make sure it's initialized
    assert(childList->size() > 0);

    Note *topNote = vrv_cast<Note *>(childList->back());
    assert(topNote);
    return topNote;
}

Note *Chord::GetBottomNote()
{
    const ArrayOfObjects *childList = this->GetList(this); // make sure it's initialized
    assert(childList->size() > 0);

    // The first note is the bottom
    Note *bottomNote = vrv_cast<Note *>(childList->front());
    assert(bottomNote);
    return bottomNote;
}

int Chord::GetXMin()
{
    const ArrayOfObjects *childList = this->GetList(this); // make sure it's initialized
    assert(childList->size() > 0);

    int x = -VRV_UNSET;
    ArrayOfObjects::const_iterator iter = childList->begin();
    while (iter != childList->end()) {
        if ((*iter)->GetDrawingX() < x) x = (*iter)->GetDrawingX();
        ++iter;
    }
    return x;
}

int Chord::GetXMax()
{
    const ArrayOfObjects *childList = this->GetList(this); // make sure it's initialized
    assert(childList->size() > 0);

    int x = VRV_UNSET;
    ArrayOfObjects::const_iterator iter = childList->begin();
    while (iter != childList->end()) {
        if ((*iter)->GetDrawingX() > x) x = (*iter)->GetDrawingX();
        ++iter;
    }
    return x;
}

void Chord::GetCrossStaffExtremes(Staff *&staffAbove, Staff *&staffBelow, Layer **layerAbove, Layer **layerBelow)
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
        if (layerBelow) (*layerBelow) = bottomNote->m_crossLayer;
    }

    // The last note is the top
    Note *topNote = this->GetTopNote();
    assert(topNote);
    if (topNote->m_crossStaff && topNote->m_crossLayer) {
        staffAbove = topNote->m_crossStaff;
        if (layerAbove) (*layerAbove) = topNote->m_crossLayer;
    }
}

bool Chord::HasCrossStaff()
{
    if (m_crossStaff) return true;

    Staff *staffAbove = NULL;
    Staff *staffBelow = NULL;

    this->GetCrossStaffExtremes(staffAbove, staffBelow);

    return ((staffAbove != NULL) || (staffBelow != NULL));
}

Point Chord::GetStemUpSE(Doc *doc, int staffSize, bool isCueSize)
{
    Note *bottomNote = this->GetBottomNote();
    assert(bottomNote);
    return bottomNote->GetStemUpSE(doc, staffSize, isCueSize);
}

Point Chord::GetStemDownNW(Doc *doc, int staffSize, bool isCueSize)
{
    Note *topNote = this->GetTopNote();
    assert(topNote);
    return topNote->GetStemDownNW(doc, staffSize, isCueSize);
}

int Chord::CalcStemLenInThirdUnits(Staff *staff, data_STEMDIRECTION stemDir)
{
    assert(staff);

    if (stemDir == STEMDIRECTION_up) {
        Note *topNote = this->GetTopNote();
        assert(topNote);
        return topNote->CalcStemLenInThirdUnits(staff, stemDir);
    }
    else if (stemDir == STEMDIRECTION_down) {
        Note *bottomNote = this->GetBottomNote();
        assert(bottomNote);
        return bottomNote->CalcStemLenInThirdUnits(staff, stemDir);
    }
    else {
        return 0;
    }
}

bool Chord::IsVisible()
{
    if (this->HasVisible()) {
        return this->GetVisible() == BOOLEAN_true;
    }

    // if the chord doesn't have it, see if all the children are invisible
    const ArrayOfObjects *notes = this->GetList(this);
    assert(notes);

    for (auto &iter : *notes) {
        Note *note = vrv_cast<Note *>(iter);
        assert(note);
        if (!note->HasVisible() || note->GetVisible() == BOOLEAN_true) {
            return true;
        }
    }

    return false;
}

bool Chord::HasNoteWithDots()
{
    const ArrayOfObjects *notes = this->GetList(this);
    assert(notes);

    for (auto &iter : *notes) {
        Note *note = vrv_cast<Note *>(iter);
        assert(note);
        if (note->GetDots() > 0) {
            return true;
        }
    }

    return false;
}

void Chord::AdjustOverlappingLayers(
    Doc *doc, const std::vector<LayerElement *> &otherElements, bool areDotsAdjusted, bool &isUnison)
{
    int margin = 0;
    // get positions of other elements
    std::set<int> otherElementLocations;
    for (auto element : otherElements) {
        if (element->Is(NOTE)) {
            Note *note = vrv_cast<Note *>(element);
            assert(note);
            otherElementLocations.insert(note->GetDrawingLoc());
        }
    }
    const ArrayOfObjects *notes = GetList(this);
    assert(notes);
    // get current chord positions
    std::set<int> chordElementLocations;
    for (auto iter : *notes) {
        Note *note = vrv_cast<Note *>(iter);
        assert(note);
        chordElementLocations.insert(note->GetDrawingLoc());
    }
    const int expectedElementsInUnison
        = CountElementsInUnison(chordElementLocations, otherElementLocations, GetDrawingStemDir());
    const bool isLowerPosition = (STEMDIRECTION_down == GetDrawingStemDir() && (otherElementLocations.size() > 0)
        && (*chordElementLocations.begin() >= *otherElementLocations.begin()));
    int actualElementsInUnison = 0;
    // process each note of the chord separately, storing locations in the set
    for (auto iter : *notes) {
        Note *note = vrv_cast<Note *>(iter);
        assert(note);
        auto [overlap, isInUnison] = note->CalcElementHorizontalOverlap(
            doc, otherElements, areDotsAdjusted, true, isLowerPosition, expectedElementsInUnison > 0);
        if (((margin >= 0) && (overlap > margin)) || ((margin <= 0) && (overlap < margin))) {
            margin = overlap;
        }
        if (isInUnison) ++actualElementsInUnison;
    }

    if (expectedElementsInUnison && (expectedElementsInUnison == actualElementsInUnison)) {
        isUnison = true;
    }
    else if (margin) {
        SetDrawingXRel(GetDrawingXRel() + margin);
    }
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Chord::AdjustCrossStaffYPos(FunctorParams *functorParams)
{
    FunctorDocParams *params = vrv_params_cast<FunctorDocParams *>(functorParams);
    assert(params);

    if (!this->HasCrossStaff()) return FUNCTOR_SIBLINGS;

    // For cross staff chords we need to re-calculate the stem because the staff position might have changed
    SetAlignmentPitchPosParams setAlignmentPitchPosParams(params->m_doc);
    Functor setAlignmentPitchPos(&Object::SetAlignmentPitchPos);
    this->Process(&setAlignmentPitchPos, &setAlignmentPitchPosParams);

    CalcStemParams calcStemParams(params->m_doc);
    Functor calcStem(&Object::CalcStem);
    this->Process(&calcStem, &calcStemParams);

    return FUNCTOR_SIBLINGS;
}

int Chord::ConvertMarkupAnalytical(FunctorParams *functorParams)
{
    ConvertMarkupAnalyticalParams *params = vrv_params_cast<ConvertMarkupAnalyticalParams *>(functorParams);
    assert(params);

    assert(!params->m_currentChord);
    params->m_currentChord = this;

    /****** fermata ******/

    if (this->HasFermata()) {
        Fermata *fermata = new Fermata();
        fermata->ConvertFromAnalyticalMarkup(this, this->GetUuid(), params);
    }

    return FUNCTOR_CONTINUE;
}

int Chord::ConvertMarkupAnalyticalEnd(FunctorParams *functorParams)
{
    ConvertMarkupAnalyticalParams *params = vrv_params_cast<ConvertMarkupAnalyticalParams *>(functorParams);
    assert(params);

    if (params->m_permanent) {
        this->ResetTiePresent();
    }

    assert(params->m_currentChord);
    params->m_currentChord = NULL;

    return FUNCTOR_CONTINUE;
}

int Chord::CalcArtic(FunctorParams *functorParams)
{
    CalcArticParams *params = vrv_params_cast<CalcArticParams *>(functorParams);
    assert(params);

    params->m_parent = this;
    params->m_stemDir = this->GetDrawingStemDir();

    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);
    Layer *layer = vrv_cast<Layer *>(this->GetFirstAncestor(LAYER));
    assert(layer);

    params->m_staffAbove = staff;
    params->m_staffBelow = staff;
    params->m_layerAbove = layer;
    params->m_layerBelow = layer;
    params->m_crossStaffAbove = false;
    params->m_crossStaffBelow = false;

    if (m_crossStaff) {
        params->m_staffAbove = m_crossStaff;
        params->m_staffBelow = m_crossStaff;
        params->m_layerAbove = m_crossLayer;
        params->m_layerBelow = m_crossLayer;
        params->m_crossStaffAbove = true;
        params->m_crossStaffBelow = true;
    }
    else {
        this->GetCrossStaffExtremes(
            params->m_staffAbove, params->m_staffBelow, &params->m_layerAbove, &params->m_layerBelow);
        if (params->m_staffAbove) {
            params->m_crossStaffAbove = true;
            params->m_staffBelow = staff;
            params->m_layerBelow = layer;
        }
        else if (params->m_staffBelow) {
            params->m_crossStaffBelow = true;
            params->m_staffAbove = staff;
            params->m_layerAbove = layer;
        }
    }

    return FUNCTOR_CONTINUE;
}

int Chord::AdjustArtic(FunctorParams *functorParams)
{
    AdjustArticParams *params = vrv_params_cast<AdjustArticParams *>(functorParams);
    assert(params);

    params->m_parent = this;
    params->m_articAbove.clear();
    params->m_articBelow.clear();

    return FUNCTOR_CONTINUE;
}

int Chord::CalcStem(FunctorParams *functorParams)
{
    CalcStemParams *params = vrv_params_cast<CalcStemParams *>(functorParams);
    assert(params);

    // Set them to NULL in any case
    params->m_interface = NULL;

    // Stems have been calculated previously in beam or fTrem - siblings because flags do not need to
    // be processed either
    if (this->IsInBeam() || this->IsInFTrem()) {
        return FUNCTOR_SIBLINGS;
    }

    // if the chord isn't visible, carry on
    if (!this->IsVisible() || (this->GetStemVisible() == BOOLEAN_false)) {
        return FUNCTOR_SIBLINGS;
    }

    Stem *stem = this->GetDrawingStem();
    assert(stem);
    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);
    Layer *layer = vrv_cast<Layer *>(this->GetFirstAncestor(LAYER));
    assert(layer);

    if (m_crossStaff) {
        staff = m_crossStaff;
        layer = m_crossLayer;
    }

    // Cache the in params to avoid further lookup
    params->m_staff = staff;
    params->m_layer = layer;
    params->m_interface = this;
    params->m_dur = this->GetActualDur();
    params->m_isGraceNote = this->IsGraceNote();

    /************ Set the direction ************/

    int yMax, yMin;
    this->GetYExtremes(yMax, yMin);
    params->m_chordStemLength = yMin - yMax;

    int staffY = staff->GetDrawingY();
    int staffSize = staff->m_drawingStaffSize;
    params->m_verticalCenter = staffY - params->m_doc->GetDrawingDoubleUnit(staffSize) * 2;

    data_STEMDIRECTION layerStemDir;
    data_STEMDIRECTION stemDir = STEMDIRECTION_NONE;

    if (stem->HasStemDir()) {
        stemDir = stem->GetStemDir();
    }
    else if ((layerStemDir = layer->GetDrawingStemDir(this)) != STEMDIRECTION_NONE) {
        stemDir = layerStemDir;
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
        stem->SetDrawingYRel(yMax - this->GetDrawingY());

    return FUNCTOR_CONTINUE;
}

MapOfNoteLocs Chord::CalcNoteLocations()
{
    const ArrayOfObjects *notes = this->GetList(this);
    assert(notes);

    MapOfNoteLocs noteLocations;
    for (Object *obj : *notes) {
        Note *note = vrv_cast<Note *>(obj);
        assert(note);

        Layer *layer = NULL;
        Staff *staff = note->GetCrossStaff(layer);
        if (!staff) staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
        assert(staff);

        noteLocations[staff].insert(note->GetDrawingLoc());
    }
    return noteLocations;
}

MapOfDotLocs Chord::CalcDotLocations(int layerCount, bool primary)
{
    const bool isUpwardDirection = (this->GetDrawingStemDir() == STEMDIRECTION_up) || (layerCount == 1);
    const bool useReverseOrder = (isUpwardDirection && !primary) || (!isUpwardDirection && primary);
    MapOfNoteLocs noteLocs = this->CalcNoteLocations();
    MapOfDotLocs dotLocs;
    for (const auto &mapEntry : noteLocs) {
        if (useReverseOrder)
            dotLocs[mapEntry.first] = CalculateDotLocations(mapEntry.second.rbegin(), mapEntry.second.rend(), true);
        else
            dotLocs[mapEntry.first] = CalculateDotLocations(mapEntry.second.begin(), mapEntry.second.end(), false);
    }
    return dotLocs;
}

int Chord::CalcDots(FunctorParams *functorParams)
{
    CalcDotsParams *params = vrv_params_cast<CalcDotsParams *>(functorParams);
    assert(params);

    // if the chord isn't visible, stop here
    if (!this->IsVisible()) {
        return FUNCTOR_SIBLINGS;
    }
    // if there aren't dot, stop here but only if no note has a dot
    if (this->GetDots() < 1) {
        if (!this->HasNoteWithDots()) {
            return FUNCTOR_SIBLINGS;
        }
        else {
            return FUNCTOR_CONTINUE;
        }
    }

    Dots *dots = vrv_cast<Dots *>(this->FindDescendantByType(DOTS, 1));
    assert(dots);

    params->m_chordDots = dots;
    params->m_chordDrawingX = this->GetDrawingX();
    params->m_chordStemDir = this->GetDrawingStemDir();

    dots->SetMapOfDotLocs(this->CalcOptimalDotLocations());

    return FUNCTOR_CONTINUE;
}

int Chord::PrepareLayerElementParts(FunctorParams *functorParams)
{
    Stem *currentStem = dynamic_cast<Stem *>(this->FindDescendantByType(STEM, 1));
    Flag *currentFlag = NULL;
    if (currentStem) currentFlag = dynamic_cast<Flag *>(currentStem->GetFirst(FLAG));

    if (!currentStem) {
        currentStem = new Stem();
        this->AddChild(currentStem);
    }
    currentStem->AttGraced::operator=(*this);
    currentStem->AttStems::operator=(*this);
    currentStem->AttStemsCmn::operator=(*this);
    if (this->GetActualDur() < DUR_2 || (this->GetStemVisible() == BOOLEAN_false)) {
        currentStem->IsVirtual(true);
    }

    if ((this->GetActualDur() > DUR_4) && !this->IsInBeam() && !this->IsInFTrem()) {
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
    const ArrayOfObjects *childList = this->GetList(this); // make sure it's initialized
    for (ArrayOfObjects::const_iterator it = childList->begin(); it != childList->end(); ++it) {
        assert((*it)->Is(NOTE));
        Note *note = vrv_cast<Note *>(*it);
        assert(note);
        note->SetDrawingStem(currentStem);
    }

    /************ dots ***********/

    Dots *currentDots = dynamic_cast<Dots *>(this->FindDescendantByType(DOTS, 1));

    if (this->GetDots() > 0) {
        if (!currentDots) {
            currentDots = new Dots();
            this->AddChild(currentDots);
        }
        currentDots->AttAugmentDots::operator=(*this);
    }
    // This will happen only if the duration has changed
    else if (currentDots) {
        if (this->DeleteChild(currentDots)) {
            currentDots = NULL;
        }
    }

    /************ Prepare the drawing cue size ************/

    Functor prepareDrawingCueSize(&Object::PrepareDrawingCueSize);
    this->Process(&prepareDrawingCueSize, NULL);

    return FUNCTOR_CONTINUE;
}

int Chord::CalcOnsetOffsetEnd(FunctorParams *functorParams)
{
    CalcOnsetOffsetParams *params = vrv_params_cast<CalcOnsetOffsetParams *>(functorParams);
    assert(params);

    LayerElement *element = this->ThisOrSameasAsLink();

    double incrementScoreTime = element->GetAlignmentDuration(
        params->m_currentMensur, params->m_currentMeterSig, true, params->m_notationType);
    incrementScoreTime = incrementScoreTime / (DUR_MAX / DURATION_4);
    double realTimeIncrementSeconds = incrementScoreTime * 60.0 / params->m_currentTempo;

    params->m_currentScoreTime += incrementScoreTime;
    params->m_currentRealTimeSeconds += realTimeIncrementSeconds;

    return FUNCTOR_CONTINUE;
}

int Chord::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);

    // We want the list of the ObjectListInterface to be re-generated
    this->Modify();
    return FUNCTOR_CONTINUE;
}

int Chord::JustifyY(FunctorParams *functorParams)
{
    JustifyYParams *params = vrv_params_cast<JustifyYParams *>(functorParams);
    assert(params);
    assert(params->m_justificationSum > 0);

    // Check if chord spreads across several staves
    std::list<Staff *> extremalStaves;
    for (Note *note : { this->GetTopNote(), this->GetBottomNote() }) {
        Layer *layer = NULL;
        Staff *staff = note->GetCrossStaff(layer);
        if (!staff) staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
        assert(staff);
        extremalStaves.push_back(staff);
    }
    assert(extremalStaves.size() == 2);

    const int topStaffN = extremalStaves.front()->GetN();
    const int bottomStaffN = extremalStaves.back()->GetN();
    if (topStaffN < bottomStaffN) {
        // Now calculate the shift due to vertical justification of the involved staves.
        Object *measure = this->GetFirstAncestor(MEASURE);
        if (!measure) return FUNCTOR_CONTINUE;

        ListOfObjects staves;
        ClassIdComparison matchType(STAFF);
        measure->FindAllDescendantByComparison(&staves, &matchType, 1);

        int shift = 0;
        for (Object *staff : staves) {
            Staff *currentStaff = vrv_cast<Staff *>(staff);
            assert(currentStaff);

            if ((currentStaff->GetN() > topStaffN) && (currentStaff->GetN() <= bottomStaffN)) {
                const double staffJustificationFactor
                    = currentStaff->GetAlignment()->GetJustificationFactor(params->m_doc);
                shift += staffJustificationFactor / params->m_justificationSum * params->m_spaceToDistribute;
            }
        }

        // Add the shift to the stem length of the chord.
        Stem *stem = vrv_cast<Stem *>(this->FindDescendantByType(STEM));
        if (!stem) return FUNCTOR_CONTINUE;

        const int stemLen = stem->GetDrawingStemLen();
        if (stem->GetDrawingStemDir() == STEMDIRECTION_up) {
            stem->SetDrawingStemLen(stemLen - shift);
            stem->SetDrawingYRel(stem->GetDrawingYRel() - shift);
        }
        else {
            stem->SetDrawingStemLen(stemLen + shift);
        }
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
