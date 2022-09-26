/////////////////////////////////////////////////////////////////////////////
// Name:        chord.cpp
// Author:      Andrew Horwitz
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "chord.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <cassert>
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
#include "gracegrp.h"
#include "horizontalaligner.h"
#include "layer.h"
#include "note.h"
#include "smufl.h"
#include "staff.h"
#include "stem.h"
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
    : LayerElement(CHORD, "chord-")
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
    this->RegisterInterface(DurationInterface::GetAttClasses(), DurationInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_CUE);
    this->RegisterAttClass(ATT_GRACED);
    this->RegisterAttClass(ATT_STEMS);
    this->RegisterAttClass(ATT_STEMSCMN);
    this->RegisterAttClass(ATT_TIEPRESENT);
    this->RegisterAttClass(ATT_VISIBILITY);

    this->Reset();
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
    this->ResetColor();
    this->ResetCue();
    this->ResetGraced();
    this->ResetStems();
    this->ResetStemsCmn();
    this->ResetTiePresent();
    this->ResetVisibility();

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

void Chord::CalculateClusters()
{
    this->ClearClusters();

    const ListOfObjects &childList = this->GetList(this);
    ListOfObjects::const_iterator iter = childList.begin();

    Note *curNote, *lastNote = vrv_cast<Note *>(*iter);
    assert(lastNote);
    int lastPitch = lastNote->GetDiatonicPitch();
    ChordCluster *curCluster = NULL;

    ++iter;

    Layer *layer1 = NULL;
    Layer *layer2 = NULL;

    while (iter != childList.end()) {
        curNote = vrv_cast<Note *>(*iter);
        assert(curNote);
        const int curPitch = curNote->GetDiatonicPitch();

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

    ArrayOfObjects &children = this->GetChildrenForModification();

    child->SetParent(this);
    // Stem are always added by PrepareLayerElementParts (for now) and we want them to be in the front
    // for the drawing order in the SVG output
    if (child->Is({ DOTS, STEM })) {
        children.insert(children.begin(), child);
    }
    else {
        children.push_back(child);
    }
    Modify();
}

void Chord::FilterList(ListOfConstObjects &childList) const
{
    // Retain only note children of chords
    ListOfConstObjects::iterator iter = childList.begin();

    while (iter != childList.end()) {
        if ((*iter)->Is(NOTE))
            ++iter;
        else
            iter = childList.erase(iter);
    }

    childList.sort(DiatonicSort());
}

int Chord::PositionInChord(const Note *note) const
{
    const int size = this->GetListSize(this);
    int position = this->GetListIndex(note);
    assert(position != -1);
    // this is the middle (only if odd)
    if ((size % 2) && (position == (size - 1) / 2)) return 0;
    if (position < (size / 2)) return -1;
    return 1;
}

void Chord::GetYExtremes(int &yMax, int &yMin) const
{
    // The first note is the bottom
    yMin = this->GetListFront(this)->GetDrawingY();
    // The last note is the top
    yMax = this->GetListBack(this)->GetDrawingY();
}

int Chord::GetYTop() const
{
    // The last note is the top
    return this->GetListBack(this)->GetDrawingY();
}

int Chord::GetYBottom() const
{
    // The first note is the bottom
    return this->GetListFront(this)->GetDrawingY();
}

Note *Chord::GetTopNote()
{
    return const_cast<Note *>(std::as_const(*this).GetTopNote());
}

const Note *Chord::GetTopNote() const
{
    const Note *topNote = vrv_cast<const Note *>(this->GetListBack(this));
    assert(topNote);
    return topNote;
}

Note *Chord::GetBottomNote()
{
    return const_cast<Note *>(std::as_const(*this).GetBottomNote());
}

const Note *Chord::GetBottomNote() const
{
    // The first note is the bottom
    const Note *bottomNote = vrv_cast<const Note *>(this->GetListFront(this));
    assert(bottomNote);
    return bottomNote;
}

int Chord::GetXMin() const
{
    const ListOfConstObjects &childList = this->GetList(this); // make sure it's initialized
    assert(childList.size() > 0);

    int x = -VRV_UNSET;
    ListOfConstObjects::const_iterator iter = childList.begin();
    while (iter != childList.end()) {
        if ((*iter)->GetDrawingX() < x) x = (*iter)->GetDrawingX();
        ++iter;
    }
    return x;
}

int Chord::GetXMax() const
{
    const ListOfConstObjects &childList = this->GetList(this); // make sure it's initialized
    assert(childList.size() > 0);

    int x = VRV_UNSET;
    ListOfConstObjects::const_iterator iter = childList.begin();
    while (iter != childList.end()) {
        if ((*iter)->GetDrawingX() > x) x = (*iter)->GetDrawingX();
        ++iter;
    }
    return x;
}

void Chord::GetCrossStaffExtremes(Staff *&staffAbove, Staff *&staffBelow, Layer **layerAbove, Layer **layerBelow)
{
    const Staff *staffAboveRef = NULL;
    const Staff *staffBelowRef = NULL;
    const Layer *layerAboveRef = NULL;
    const Layer *layerBelowRef = NULL;
    std::as_const(*this).GetCrossStaffExtremes(staffAboveRef, staffBelowRef, &layerAboveRef, &layerBelowRef);
    staffAbove = const_cast<Staff *>(staffAboveRef);
    staffBelow = const_cast<Staff *>(staffBelowRef);
    if (layerAbove) *layerAbove = const_cast<Layer *>(layerAboveRef);
    if (layerBelow) *layerBelow = const_cast<Layer *>(layerBelowRef);
}

void Chord::GetCrossStaffExtremes(
    const Staff *&staffAbove, const Staff *&staffBelow, const Layer **layerAbove, const Layer **layerBelow) const
{
    staffAbove = NULL;
    staffBelow = NULL;

    // We assume that we have a cross-staff chord we cannot have further cross-staffed notes
    if (m_crossStaff) return;

    // The first note is the bottom
    const Note *bottomNote = this->GetBottomNote();
    assert(bottomNote);
    if (bottomNote->m_crossStaff && bottomNote->m_crossLayer) {
        staffBelow = bottomNote->m_crossStaff;
        if (layerBelow) (*layerBelow) = bottomNote->m_crossLayer;
    }

    // The last note is the top
    const Note *topNote = this->GetTopNote();
    assert(topNote);
    if (topNote->m_crossStaff && topNote->m_crossLayer) {
        staffAbove = topNote->m_crossStaff;
        if (layerAbove) (*layerAbove) = topNote->m_crossLayer;
    }
}

bool Chord::HasCrossStaff() const
{
    if (m_crossStaff) return true;

    const Staff *staffAbove = NULL;
    const Staff *staffBelow = NULL;

    this->GetCrossStaffExtremes(staffAbove, staffBelow);

    return ((staffAbove != NULL) || (staffBelow != NULL));
}

Point Chord::GetStemUpSE(const Doc *doc, int staffSize, bool isCueSize) const
{
    const Note *bottomNote = this->GetBottomNote();
    assert(bottomNote);
    return bottomNote->GetStemUpSE(doc, staffSize, isCueSize);
}

Point Chord::GetStemDownNW(const Doc *doc, int staffSize, bool isCueSize) const
{
    const Note *topNote = this->GetTopNote();
    assert(topNote);
    return topNote->GetStemDownNW(doc, staffSize, isCueSize);
}

data_STEMDIRECTION Chord::CalcStemDirection(int verticalCenter) const
{
    const ListOfConstObjects &childList = this->GetList(this);
    ListOfConstObjects topNotes, bottomNotes;

    // split notes into two vectors - notes above vertical center and below
    std::partition_copy(childList.begin(), childList.end(), std::back_inserter(topNotes),
        std::back_inserter(bottomNotes),
        [verticalCenter](const Object *note) { return note->GetDrawingY() > verticalCenter; });

    auto bottomIter = bottomNotes.begin();
    auto topIter = topNotes.rbegin();
    for (; bottomIter != bottomNotes.end() && topIter != topNotes.rend(); ++bottomIter, ++topIter) {
        const int bottomY = (*bottomIter)->GetDrawingY();
        const int topY = (*topIter)->GetDrawingY();
        const int middlePoint = (topY + bottomY) / 2;

        // if notes are equidistant - proceed to the next pair of notes
        if (middlePoint == verticalCenter) {
            continue;
        }
        // otherwise return corresponding stem direction
        else if (middlePoint > verticalCenter) {
            return STEMDIRECTION_down;
        }
        else if (middlePoint < verticalCenter) {
            return STEMDIRECTION_up;
        }
    }

    // if there are still unprocessed notes left on the bottom that are not on the center - stem direction should be up
    if ((bottomIter != bottomNotes.end()) && ((*bottomIter)->GetDrawingY() != verticalCenter)) {
        return STEMDIRECTION_up;
    }
    // otherwise place it down
    return STEMDIRECTION_down;
}

int Chord::CalcStemLenInThirdUnits(const Staff *staff, data_STEMDIRECTION stemDir) const
{
    assert(staff);

    if (stemDir == STEMDIRECTION_up) {
        const Note *topNote = this->GetTopNote();
        assert(topNote);
        return topNote->CalcStemLenInThirdUnits(staff, stemDir);
    }
    else if (stemDir == STEMDIRECTION_down) {
        const Note *bottomNote = this->GetBottomNote();
        assert(bottomNote);
        return bottomNote->CalcStemLenInThirdUnits(staff, stemDir);
    }
    else {
        return 0;
    }
}

bool Chord::IsVisible() const
{
    if (this->HasVisible()) {
        return this->GetVisible() == BOOLEAN_true;
    }

    // if the chord doesn't have it, see if all the children are invisible
    const ListOfConstObjects &notes = this->GetList(this);

    for (auto &iter : notes) {
        const Note *note = vrv_cast<const Note *>(iter);
        assert(note);
        if (!note->HasVisible() || note->GetVisible() == BOOLEAN_true) {
            return true;
        }
    }

    return false;
}

bool Chord::HasAdjacentNotesInStaff(const Staff *staff) const
{
    assert(staff);
    MapOfNoteLocs locations = this->CalcNoteLocations();

    if (locations[staff].empty() || locations[staff].size() == 1) return false;

    std::vector<int> diff;
    diff.resize(locations[staff].size());
    // Find difference between adjacent notes in the chord. Since locations[staff] is multiset, elements are ordered and
    // represent position of notes in chord. This way we can find whether there are notes with diatonic step difference
    // of 1.
    std::adjacent_difference(locations[staff].begin(), locations[staff].end(), diff.begin());

    return (diff.end() != std::find(std::next(diff.begin()), diff.end(), 1));
}

bool Chord::HasNoteWithDots() const
{
    const ListOfConstObjects &notes = this->GetList(this);

    return std::any_of(notes.cbegin(), notes.cend(), [](const Object *object) {
        const Note *note = vrv_cast<const Note *>(object);
        assert(note);
        return (note->GetDots() > 0);
    });
}

int Chord::AdjustOverlappingLayers(const Doc *doc, const std::vector<LayerElement *> &otherElements,
    bool areDotsAdjusted, bool &isUnison, bool &stemSameas)
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
    const ListOfObjects &notes = this->GetList(this);
    // get current chord positions
    std::set<int> chordElementLocations;
    for (const auto iter : notes) {
        Note *note = vrv_cast<Note *>(iter);
        assert(note);
        chordElementLocations.insert(note->GetDrawingLoc());
    }

    std::vector<int> locationsInUnison
        = this->GetElementsInUnison(chordElementLocations, otherElementLocations, this->GetDrawingStemDir());

    const int expectedElementsInUnison = (int)locationsInUnison.size();
    const bool isLowerPosition = (STEMDIRECTION_down == this->GetDrawingStemDir() && (otherElementLocations.size() > 0)
        && (*chordElementLocations.begin() >= *otherElementLocations.begin()));
    int actualElementsInUnison = 0;

    // process each note of the chord separately, storing locations in the set
    for (auto iter : notes) {
        Note *note = vrv_cast<Note *>(iter);
        assert(note);
        auto [overlap, isInUnison] = note->CalcElementHorizontalOverlap(
            doc, otherElements, areDotsAdjusted, true, isLowerPosition, expectedElementsInUnison > 0);
        if (((margin >= 0) && (overlap > margin)) || ((margin <= 0) && (overlap < margin))) {
            margin = overlap;
        }
        if (isInUnison) ++actualElementsInUnison;
    }

    // if there are accidentals that are aligned for the layer separately, we need to have additional margin for them
    int accidMargin = 0;
    for (const auto iter : otherElements) {
        if (!iter->Is(NOTE)) continue;
        Note *note = vrv_cast<Note *>(iter);
        Accid *accid = vrv_cast<Accid *>(note->FindDescendantByType(ACCID));
        if (accid && accid->IsAlignedWithSameLayer()) {
            accidMargin += accid->GetContentRight() - accid->GetContentLeft();
        }
    }
    if (accidMargin) {
        // add padding for the accidentals (1.5 unit)
        accidMargin += 1.5 * doc->GetDrawingUnit(100);
    }

    if (expectedElementsInUnison && (expectedElementsInUnison == actualElementsInUnison)) {
        isUnison = true;
    }
    else if (margin) {
        // adjust margin by accidental margin
        margin -= accidMargin;
        this->SetDrawingXRel(this->GetDrawingXRel() + margin);
        return margin;
    }
    return 0;
}

std::list<const Note *> Chord::GetAdjacentNotesList(const Staff *staff, int loc) const
{
    const ListOfConstObjects &notes = this->GetList(this);

    std::list<const Note *> adjacentNotes;
    for (const Object *obj : notes) {
        const Note *note = vrv_cast<const Note *>(obj);
        assert(note);

        const Staff *noteStaff = note->GetAncestorStaff(RESOLVE_CROSS_STAFF);
        if (noteStaff != staff) continue;

        const int locDiff = note->GetDrawingLoc() - loc;
        if ((std::abs(locDiff) <= 2) && (locDiff != 0)) {
            adjacentNotes.push_back(note);
        }
    }
    return adjacentNotes;
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
    CalcAlignmentPitchPosParams calcAlignmentPitchPosParams(params->m_doc);
    Functor calcAlignmentPitchPos(&Object::CalcAlignmentPitchPos);
    this->Process(&calcAlignmentPitchPos, &calcAlignmentPitchPosParams);

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
        fermata->ConvertFromAnalyticalMarkup(this, this->GetID(), params);
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

    Staff *staff = this->GetAncestorStaff();
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
    if (this->IsInBeam() || this->GetAncestorFTrem()) {
        return FUNCTOR_SIBLINGS;
    }

    // if the chord isn't visible, carry on
    if (!this->IsVisible() || (this->GetStemVisible() == BOOLEAN_false)) {
        return FUNCTOR_SIBLINGS;
    }

    Stem *stem = this->GetDrawingStem();
    assert(stem);
    Staff *staff = this->GetAncestorStaff();
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
    params->m_dur = this->GetNoteOrChordDur(this);
    params->m_isGraceNote = this->IsGraceNote();
    params->m_isStemSameasSecondary = false;

    /************ Set the direction ************/

    int yMax, yMin;
    this->GetYExtremes(yMax, yMin);
    params->m_chordStemLength = yMin - yMax;

    int staffY = staff->GetDrawingY();
    int staffSize = staff->m_drawingStaffSize;
    params->m_verticalCenter = staffY - params->m_doc->GetDrawingDoubleUnit(staffSize) * 2;

    data_STEMDIRECTION layerStemDir;
    data_STEMDIRECTION stemDir = STEMDIRECTION_NONE;

    if (stem->HasDir()) {
        stemDir = stem->GetDir();
    }
    else if ((layerStemDir = layer->GetDrawingStemDir(this)) != STEMDIRECTION_NONE) {
        stemDir = layerStemDir;
    }
    else {
        stemDir = this->CalcStemDirection(params->m_verticalCenter);
    }

    this->SetDrawingStemDir(stemDir);

    // Position the stem to the bottom note when up
    if (stemDir == STEMDIRECTION_up) stem->SetDrawingYRel(yMin - this->GetDrawingY());
    // And to the top note when down
    else
        stem->SetDrawingYRel(yMax - this->GetDrawingY());

    return FUNCTOR_CONTINUE;
}

int Chord::CalcChordNoteHeads(FunctorParams *functorParams)
{
    CalcChordNoteHeadsParams *params = vrv_params_cast<CalcChordNoteHeadsParams *>(functorParams);
    assert(params);

    Staff *staff = this->GetAncestorStaff(RESOLVE_CROSS_STAFF);

    params->m_diameter = 0;
    if (this->GetDrawingStemDir() == STEMDIRECTION_up) {
        if (this->IsInBeam()) {
            params->m_diameter = 2 * this->GetDrawingRadius(params->m_doc);
        }
        else {
            const Note *bottomNote = this->GetBottomNote();
            const char32_t code = bottomNote->GetNoteheadGlyph(this->GetActualDur());
            params->m_diameter = params->m_doc->GetGlyphWidth(
                code, staff->m_drawingStaffSize, this->GetDrawingCueSize() ? bottomNote->GetDrawingCueSize() : false);
        }
    }

    return FUNCTOR_CONTINUE;
}

MapOfNoteLocs Chord::CalcNoteLocations(NotePredicate predicate) const
{
    const ListOfConstObjects &notes = this->GetList(this);

    MapOfNoteLocs noteLocations;
    for (const Object *obj : notes) {
        const Note *note = vrv_cast<const Note *>(obj);
        assert(note);

        if (predicate && !predicate(note)) continue;

        const Staff *staff = note->GetAncestorStaff(RESOLVE_CROSS_STAFF);

        noteLocations[staff].insert(note->GetDrawingLoc());
    }
    return noteLocations;
}

MapOfDotLocs Chord::CalcDotLocations(int layerCount, bool primary) const
{
    const bool isUpwardDirection = (this->GetDrawingStemDir() == STEMDIRECTION_up) || (layerCount == 1);
    const bool useReverseOrder = (isUpwardDirection != primary);
    MapOfNoteLocs noteLocs = this->CalcNoteLocations([](const Note *note) { return !note->HasDots(); });
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
        currentStem->IsAttribute(true);
        this->AddChild(currentStem);
    }
    currentStem->AttGraced::operator=(*this);
    currentStem->FillAttributes(*this);

    int duration = this->GetNoteOrChordDur(this);
    if ((duration < DUR_2) || (this->GetStemVisible() == BOOLEAN_false)) {
        currentStem->IsVirtual(true);
    }

    if ((duration > DUR_4) && !this->IsInBeam() && !this->GetAncestorFTrem()) {
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

    this->SetDrawingStem(currentStem);

    // Calculate chord clusters
    this->CalculateClusters();

    // Also set the drawing stem object (or NULL) to all child notes
    const ListOfObjects &childList = this->GetList(this);
    for (ListOfObjects::const_iterator it = childList.begin(); it != childList.end(); ++it) {
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

    Functor prepareCueSize(&Object::PrepareCueSize);
    this->Process(&prepareCueSize, NULL);

    return FUNCTOR_CONTINUE;
}

int Chord::PrepareLyrics(FunctorParams *functorParams)
{
    PrepareLyricsParams *params = vrv_params_cast<PrepareLyricsParams *>(functorParams);
    assert(params);

    params->m_penultimateNoteOrChord = params->m_lastNoteOrChord;
    params->m_lastNoteOrChord = this;

    return FUNCTOR_CONTINUE;
}

int Chord::InitOnsetOffsetEnd(FunctorParams *functorParams)
{
    InitOnsetOffsetParams *params = vrv_params_cast<InitOnsetOffsetParams *>(functorParams);
    assert(params);

    LayerElement *element = this->ThisOrSameasLink();

    double incrementScoreTime = element->GetAlignmentDuration(
        params->m_currentMensur, params->m_currentMeterSig, true, params->m_notationType);
    incrementScoreTime = incrementScoreTime / (DUR_MAX / DURATION_4);
    double realTimeIncrementSeconds = incrementScoreTime * 60.0 / params->m_currentTempo;

    params->m_currentScoreTime += incrementScoreTime;
    params->m_currentRealTimeSeconds += realTimeIncrementSeconds;

    return FUNCTOR_CONTINUE;
}

int Chord::ResetData(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetData(functorParams);

    // We want the list of the ObjectListInterface to be re-generated
    this->Modify();
    return FUNCTOR_CONTINUE;
}

int Chord::PrepareDataInitialization(FunctorParams *)
{
    if (this->HasEmptyList(this)) {
        LogWarning("Chord '%s' has no child note - a default note is added", this->GetID().c_str());
        Note *rescueNote = new Note();
        this->AddChild(rescueNote);
    }
    this->Modify();

    return FUNCTOR_CONTINUE;
}

int Chord::JustifyYAdjustCrossStaff(FunctorParams *functorParams)
{
    JustifyYAdjustCrossStaffParams *params = vrv_params_cast<JustifyYAdjustCrossStaffParams *>(functorParams);
    assert(params);

    // Check if chord spreads across several staves
    std::map<int, Staff *> extremalStaves;
    for (Note *note : { this->GetTopNote(), this->GetBottomNote() }) {
        Staff *staff = note->GetAncestorStaff(RESOLVE_CROSS_STAFF);
        extremalStaves.insert({ staff->GetN(), staff });
    }
    // get chord parent staff
    Staff *staff = this->GetAncestorStaff(RESOLVE_CROSS_STAFF);
    extremalStaves.insert({ staff->GetN(), staff });

    if (extremalStaves.size() < 2) return FUNCTOR_CONTINUE;

    // Now calculate the shift due to vertical justification
    auto getShift = [params](Staff *staff) {
        StaffAlignment *alignment = staff->GetAlignment();
        if (params->m_shiftForStaff.find(alignment) != params->m_shiftForStaff.end()) {
            return params->m_shiftForStaff.at(alignment);
        }
        return 0;
    };

    const int shift = getShift(extremalStaves.rbegin()->second) - getShift(extremalStaves.begin()->second);

    // Add the shift to the stem length of the chord
    Stem *stem = vrv_cast<Stem *>(this->FindDescendantByType(STEM));
    if (!stem) return FUNCTOR_CONTINUE;

    const int stemLen = stem->GetDrawingStemLen();
    if (stem->GetDrawingStemDir() == STEMDIRECTION_up) {
        stem->SetDrawingStemLen(stemLen - shift);
    }
    else {
        stem->SetDrawingStemLen(stemLen + shift);
    }

    // Reposition the stem
    Staff *rootStaff = (stem->GetDrawingStemDir() == STEMDIRECTION_up) ? extremalStaves.rbegin()->second
                                                                       : extremalStaves.begin()->second;
    stem->SetDrawingYRel(stem->GetDrawingYRel() + getShift(staff) - getShift(rootStaff));

    // Add the shift to the flag position
    Flag *flag = vrv_cast<Flag *>(stem->FindDescendantByType(FLAG));
    if (flag) {
        const int sign = (stem->GetDrawingStemDir() == STEMDIRECTION_up) ? 1 : -1;
        flag->SetDrawingYRel(flag->GetDrawingYRel() + sign * shift);
    }

    return FUNCTOR_CONTINUE;
}

int Chord::GenerateMIDI(FunctorParams *functorParams)
{
    GenerateMIDIParams *params = vrv_params_cast<GenerateMIDIParams *>(functorParams);
    assert(params);

    // Handle grace chords
    if (this->IsGraceNote()) {
        std::set<int> pitches;
        const ListOfObjects &notes = this->GetList(this);
        for (Object *obj : notes) {
            Note *note = vrv_cast<Note *>(obj);
            assert(note);
            pitches.insert(note->GetMIDIPitch(params->m_transSemi));
        }

        double quarterDuration = 0.0;
        const data_DURATION dur = this->GetDur();
        if ((dur >= DURATION_long) && (dur <= DURATION_1024)) {
            quarterDuration = pow(2.0, (DURATION_4 - dur));
        }

        params->m_graceNotes.push_back({ pitches, quarterDuration });

        bool accented = (this->GetGrace() == GRACE_acc);
        GraceGrp *graceGrp = vrv_cast<GraceGrp *>(this->GetFirstAncestor(GRACEGRP));
        if (graceGrp && (graceGrp->GetGrace() == GRACE_acc)) accented = true;
        params->m_accentedGraceNote = accented;

        return FUNCTOR_SIBLINGS;
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
