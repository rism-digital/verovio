/////////////////////////////////////////////////////////////////////////////
// Name:        drawinginterface.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "drawinginterface.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "chord.h"
#include "layerelement.h"
#include "note.h"
#include "object.h"
#include "staff.h"
#include "stem.h"
#include "vrv.h"

namespace vrv {

// helper for determining note direction
data_STEMDIRECTION GetNoteDirection(int leftNoteY, int rightNoteY)
{
    if (leftNoteY == rightNoteY) return STEMDIRECTION_NONE;
    return (leftNoteY < rightNoteY) ? STEMDIRECTION_up : STEMDIRECTION_down;
}

//----------------------------------------------------------------------------
// DrawingListInterface
//----------------------------------------------------------------------------

DrawingListInterface::DrawingListInterface()
{
    this->Reset();
}

DrawingListInterface::~DrawingListInterface() {}

void DrawingListInterface::Reset()
{
    m_drawingList.clear();
}

void DrawingListInterface::AddToDrawingList(Object *object)
{
    if (std::find(m_drawingList.begin(), m_drawingList.end(), object) == m_drawingList.end()) {
        // someName not in name, add it
        m_drawingList.push_back(object);
    }

    /*
    m_drawingList.push_back(object);
    m_drawingList.sort();
    m_drawingList.unique();
     */
}

ArrayOfObjects *DrawingListInterface::GetDrawingList()
{
    return &m_drawingList;
}

void DrawingListInterface::ResetDrawingList()
{
    m_drawingList.clear();
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

FunctorCode DrawingListInterface::InterfaceResetData(ResetDataFunctor &functor)
{
    this->DrawingListInterface::Reset();

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// BeamDrawingInterface
//----------------------------------------------------------------------------

BeamDrawingInterface::BeamDrawingInterface() : ObjectListInterface()
{
    this->Reset();
}

BeamDrawingInterface::~BeamDrawingInterface()
{
    this->ClearCoords();
}

void BeamDrawingInterface::Reset()
{
    m_changingDur = false;
    m_beamHasChord = false;
    m_hasMultipleStemDir = false;
    m_cueSize = false;
    m_fractionSize = 100;
    m_crossStaffContent = NULL;
    m_crossStaffRel = STAFFREL_basic_NONE;
    m_isSpanningElement = false;
    m_shortestDur = DURATION_NONE;
    m_notesStemDir = STEMDIRECTION_NONE;
    m_drawingPlace = BEAMPLACE_NONE;
    m_beamStaff = NULL;

    m_beamWidth = 0;
    m_beamWidthBlack = 0;
    m_beamWidthWhite = 0;

    this->ClearCoords();
}

int BeamDrawingInterface::GetTotalBeamWidth() const
{
    return m_beamWidthBlack + (m_shortestDur - DURATION_8) * m_beamWidth;
}

void BeamDrawingInterface::ClearCoords()
{
    for (BeamElementCoord *coord : m_beamElementCoords) {
        delete coord;
    }
    m_beamElementCoords.clear();
}

void BeamDrawingInterface::InitCoords(const ArrayOfObjects &childList, Staff *staff, data_BEAMPLACE place)
{
    ListOfObjects children(childList.begin(), childList.end());
    this->InitCoords(children, staff, place);
}

void BeamDrawingInterface::InitCoords(const ListOfObjects &childList, Staff *staff, data_BEAMPLACE place)
{
    assert(staff);

    BeamDrawingInterface::Reset();
    this->ClearCoords();

    if (childList.empty()) {
        return;
    }

    m_beamStaff = staff;

    m_beamElementCoords.reserve(childList.size());
    for ([[maybe_unused]] auto child : childList) {
        m_beamElementCoords.push_back(new BeamElementCoord());
    }

    // current point to the first Note in the layed out layer
    LayerElement *current = dynamic_cast<LayerElement *>(childList.front());
    // Beam list should contain only DurationInterface objects
    assert(current->GetDurationInterface());

    data_DURATION lastDur = (current->GetDurationInterface())->GetActualDur();

    /******************************************************************/
    // Populate BeamElementCoord for each element in the beam
    // This could be moved to Beam::InitCoord for optimization because there should be no
    // need for redoing it everytime it is drawn.

    data_STEMDIRECTION currentStemDir;
    Layer *layer = NULL;

    int elementCount = 0;

    ListOfObjects::const_iterator iter = childList.begin();
    do {
        // Beam list should contain only DurationInterface objects
        assert(current->GetDurationInterface());
        const data_DURATION currentDur = (current->GetDurationInterface())->GetActualDur();

        if (current->Is(CHORD)) {
            m_beamHasChord = true;
        }

        m_beamElementCoords.at(elementCount)->m_element = current;
        m_beamElementCoords.at(elementCount)->m_dur = currentDur;

        // Look at beam breaks
        m_beamElementCoords.at(elementCount)->m_breaksec = 0;
        AttBeamSecondary *beamsecondary = dynamic_cast<AttBeamSecondary *>(current);
        if (beamsecondary && beamsecondary->HasBreaksec()) {
            if (!m_changingDur) m_changingDur = true;
            m_beamElementCoords.at(elementCount)->m_breaksec = beamsecondary->GetBreaksec();
        }

        Staff *staff = current->GetCrossStaff(layer);
        if (staff && (staff != m_beamStaff)) {
            m_crossStaffContent = staff;
            m_crossStaffRel = current->GetCrossStaffRel();
        }
        // Check if some beam chord has cross staff content
        else if (current->Is(CHORD)) {
            Chord *chord = vrv_cast<Chord *>(current);
            assert(chord);
            for (Note *note : { chord->GetTopNote(), chord->GetBottomNote() }) {
                if (note->m_crossStaff && (note->m_crossStaff != m_beamStaff)) {
                    m_crossStaffContent = note->m_crossStaff;
                    m_crossStaffRel = note->GetCrossStaffRel();
                }
            }
        }

        // Skip rests and tabGrp
        if (current->Is({ CHORD, NOTE })) {
            // Look at the stemDir to see if we have multiple stem Dir
            if (!m_hasMultipleStemDir) {
                // At this stage, BeamCoord::m_stem is not necessary set, so we need to look at the Note / Chord
                // original value. Example: GetAncestorBeam called in PrepareLayerElementPartsFunctor::VisitNote
                // when reaching the first note of the beam
                currentStemDir = m_beamElementCoords.at(elementCount)->GetStemDir();
                if (currentStemDir != STEMDIRECTION_NONE) {
                    if ((m_notesStemDir != STEMDIRECTION_NONE) && (m_notesStemDir != currentStemDir)) {
                        m_hasMultipleStemDir = true;
                        m_notesStemDir = STEMDIRECTION_NONE;
                    }
                    else {
                        m_notesStemDir = currentStemDir;
                    }
                }
            }
        }
        // Skip rests
        if (current->Is({ CHORD, NOTE, TABGRP })) {
            // keep the shortest dur in the beam
            m_shortestDur = std::max(currentDur, m_shortestDur);
        }

        // check if we have more than duration in the beam
        if (!m_changingDur && currentDur != lastDur) m_changingDur = true;
        lastDur = currentDur;

        elementCount++;

        ++iter;
        if (iter == childList.end()) {
            break;
        }
        current = dynamic_cast<LayerElement *>(*iter);
        if (current == NULL) {
            LogDebug("Error accessing element in Beam list");
            return;
        }

    } while (1);

    // elementCount must be greater than 0 here
    if (elementCount == 0) {
        LogDebug("Beam with no notes of duration > 8 detected. Exiting DrawBeam.");
        return;
    }
}

void BeamDrawingInterface::InitCue(bool beamCue)
{
    if (beamCue) {
        m_cueSize = beamCue;
    }
    else {
        m_cueSize = std::all_of(m_beamElementCoords.begin(), m_beamElementCoords.end(), [](BeamElementCoord *coord) {
            if (!coord->m_element) return false;
            if (coord->m_element->IsGraceNote() || coord->m_element->GetDrawingCueSize()) return true;
            return false;
        });
    }

    return;
}

void BeamDrawingInterface::InitGraceStemDir(bool graceGrp)
{
    if (!graceGrp) {
        graceGrp = std::all_of(m_beamElementCoords.begin(), m_beamElementCoords.end(), [](BeamElementCoord *coord) {
            if (!coord->m_element) return false;
            if (coord->m_element->IsGraceNote()) return true;
            return false;
        });
    }

    // Always set stem direction to up for grace note beam unless stem direction is provided
    if (graceGrp && (m_notesStemDir == STEMDIRECTION_NONE)) {
        m_notesStemDir = STEMDIRECTION_up;
    }
}

bool BeamDrawingInterface::IsHorizontal() const
{
    if (this->IsRepeatedPattern()) {
        return true;
    }

    if (this->HasOneStepHeight()) return true;

    // if (m_drawingPlace == BEAMPLACE_mixed) return true;

    if (m_drawingPlace == BEAMPLACE_NONE) return true;

    std::vector<int> items;
    std::vector<data_BEAMPLACE> directions;
    items.reserve(m_beamElementCoords.size());
    directions.reserve(m_beamElementCoords.size());

    for (BeamElementCoord *coord : m_beamElementCoords) {
        if (!coord->m_stem || !coord->m_closestNote) continue;

        items.push_back(coord->m_closestNote->GetDrawingY());
        directions.push_back(coord->m_beamRelativePlace);
    }
    int itemCount = (int)items.size();

    if (itemCount < 2) return true;

    const int first = items.front();
    const int last = items.back();

    // First note and last note have the same postion
    if (first == last) return true;

    // If drawing place is mixed and is should be drawn horizontal based on mixed rules
    if ((m_drawingPlace == BEAMPLACE_mixed) && (this->IsHorizontalMixedBeam(items, directions))) return true;

    // Detect beam with two pitches only and as step at the beginning or at the end
    const bool firstStep = (first != items.at(1));
    const bool lastStep = (last != items.at(items.size() - 2));
    if ((items.size() > 2) && (firstStep || lastStep)) {
        // Detect concave shapes
        for (int i = 1; i < itemCount - 1; ++i) {
            if (m_drawingPlace == BEAMPLACE_above) {
                if ((items.at(i) >= first) && (items.at(i) >= last)) return true;
            }
            else if (m_drawingPlace == BEAMPLACE_below) {
                if ((items.at(i) <= first) && (items.at(i) <= last)) return true;
            }
        }
        std::vector<int> pitches;
        std::unique_copy(items.begin(), items.end(), std::back_inserter(pitches));
        if (pitches.size() == 2) {
            if (m_drawingPlace == BEAMPLACE_above) {
                // Single note at the beginning as lower first
                if (firstStep && (std::is_sorted(items.begin(), items.end()))) return true;
                // Single note at the end and lower last
                if (lastStep && (std::is_sorted(items.rbegin(), items.rend()))) return true;
            }
            else {
                // Single note at the end and higher last
                if (lastStep && (std::is_sorted(items.begin(), items.end()))) return true;
                // Single note at the beginning and higher first
                if (firstStep && (std::is_sorted(items.rbegin(), items.rend()))) return true;
            }
        }
    }

    return false;
}

bool BeamDrawingInterface::IsHorizontalMixedBeam(
    const std::vector<int> &items, const std::vector<data_BEAMPLACE> &directions) const
{
    // items and directions should be of the same size, otherwise something is wrong
    if (items.size() != directions.size()) return false;

    // calculate how many times stem direction is changed withing the beam
    int directionChanges = 0;
    data_BEAMPLACE previous = directions.front();
    std::for_each(directions.begin(), directions.end(), [&previous, &directionChanges](data_BEAMPLACE current) {
        if (current != previous) {
            ++directionChanges;
            previous = current;
        }
    });
    // if we have a mix of cross-staff elements, going from one staff to another repeatedly, we need to check note
    // directions. Otherwise we can use direction of the outside pitches for beam
    if (directionChanges <= 1) return false;

    int previousTop = VRV_UNSET;
    int previousBottom = VRV_UNSET;
    data_STEMDIRECTION outsidePitchDirection = GetNoteDirection(items.front(), items.back());
    std::map<data_STEMDIRECTION, int> beamDirections{ { STEMDIRECTION_NONE, 0 }, { STEMDIRECTION_up, 0 },
        { STEMDIRECTION_down, 0 } };
    int i = 0;
    for (data_BEAMPLACE direction : directions) {
        if (direction == BEAMPLACE_above) {
            if (previousTop == VRV_UNSET) {
                previousTop = items.at(i);
            }
            else {
                ++beamDirections[GetNoteDirection(previousTop, items.at(i))];
            }
        }
        else if (direction == BEAMPLACE_below) {
            if (previousBottom == VRV_UNSET) {
                previousBottom = items.at(i);
            }
            else {
                ++beamDirections[GetNoteDirection(previousBottom, items.at(i))];
            }
        }
        ++i;
    }
    // if direction of beam outside pitches corresponds to majority of the note directions within the beam, beam
    // can be drawn in that direction. Otherwise horizontal beam should be used
    bool result = std::any_of(
        beamDirections.begin(), beamDirections.end(), [&beamDirections, &outsidePitchDirection](const auto &pair) {
            return (pair.first == outsidePitchDirection) ? false : pair.second > beamDirections[outsidePitchDirection];
        });
    return result;
}

bool BeamDrawingInterface::IsRepeatedPattern() const
{
    if (m_drawingPlace == BEAMPLACE_mixed) return false;

    if (m_drawingPlace == BEAMPLACE_NONE) return false;

    int elementCount = (int)m_beamElementCoords.size();

    // No pattern with at least 4 elements
    if (elementCount < 4) return false;

    std::vector<int> items;
    items.reserve(m_beamElementCoords.size());

    for (BeamElementCoord *coord : m_beamElementCoords) {
        if (!coord->m_stem || !coord->m_closestNote) continue;

        // Could this be an overflow with 32 bits? Not sure why DUR_MAX is used here
        items.push_back(coord->m_closestNote->GetDrawingY() + DUR_MAX * coord->m_dur);
    }
    int itemCount = (int)items.size();

    // No pattern with at least 4 elements or if all elements are the same
    if ((itemCount < 4) || (std::equal(items.begin() + 1, items.end(), items.begin()))) {
        return false;
    }

    // Find all possible dividers for the sequence (without 1 and its size)
    std::vector<int> dividers;
    for (int i = 2; i <= itemCount / 2; ++i) {
        if (itemCount % i == 0) dividers.push_back(i);
    }

    // Correlate a sub-array for each divider until a sequence is found (if any)
    for (int divider : dividers) {
        bool pattern = true;
        std::vector<int>::iterator iter = items.begin();
        std::vector<int> v1 = std::vector<int>(iter, iter + divider);
        for (int j = 1; j < (itemCount / divider); ++j) {
            std::vector<int> v2 = std::vector<int>(iter + j * divider, iter + (j + 1) * divider);
            if (v1 != v2) {
                pattern = false;
                break;
            }
        }
        if (pattern) {
            // LogDebug("Pattern found %d", divider);
            return true;
        }
    }

    return false;
}

bool BeamDrawingInterface::HasOneStepHeight() const
{
    if (m_shortestDur < DURATION_32) return false;

    int top = -128;
    int bottom = 128;
    for (BeamElementCoord *coord : m_beamElementCoords) {
        if (coord->m_closestNote) {
            Note *note = vrv_cast<Note *>(coord->m_closestNote);
            assert(note);
            int loc = note->GetDrawingLoc();
            if (loc > top) top = loc;
            if (loc < bottom) bottom = loc;
        }
    }

    return (abs(top - bottom) <= 1);
}

bool BeamDrawingInterface::IsFirstIn(const LayerElement *element) const
{
    this->GetList();
    const int position = this->GetPosition(element);
    // This method should be called only if the note is part of a fTrem
    assert(position != -1);
    // this is the first one
    return (position == 0);
}

bool BeamDrawingInterface::IsLastIn(const LayerElement *element) const
{
    const int size = this->GetListSize();
    const int position = this->GetPosition(element);
    // This method should be called only if the note is part of a beam
    assert(position != -1);
    // this is the last one
    return (position == size - 1);
}

int BeamDrawingInterface::GetPosition(const LayerElement *element) const
{
    this->GetList();
    int position = this->GetListIndex(element);
    // Check if this is a note in the chord
    if ((position == -1) && (element->Is(NOTE))) {
        const Note *note = vrv_cast<const Note *>(element);
        assert(note);
        const Chord *chord = note->IsChordTone();
        if (chord) position = this->GetListIndex(chord);
    }
    return position;
}

void BeamDrawingInterface::GetBeamOverflow(StaffAlignment *&above, StaffAlignment *&below) const
{
    if (!m_beamStaff || !m_crossStaffContent) return;

    if (m_drawingPlace == BEAMPLACE_mixed) {
        above = NULL;
        below = NULL;
    }
    // Beam below - ignore above and find the appropriate below staff
    else if (m_drawingPlace == BEAMPLACE_below) {
        above = NULL;
        if (m_crossStaffRel == STAFFREL_basic_above) {
            below = m_beamStaff->GetAlignment();
        }
        else {
            below = m_crossStaffContent->GetAlignment();
        }
    }
    // Beam above - ignore below and find the appropriate above staff
    else if (m_drawingPlace == BEAMPLACE_above) {
        below = NULL;
        if (m_crossStaffRel == STAFFREL_basic_below) {
            above = m_beamStaff->GetAlignment();
        }
        else {
            above = m_crossStaffContent->GetAlignment();
        }
    }
}

void BeamDrawingInterface::GetBeamChildOverflow(StaffAlignment *&above, StaffAlignment *&below) const
{
    if (m_beamStaff && m_crossStaffContent) {
        if (m_crossStaffRel == STAFFREL_basic_above) {
            above = m_crossStaffContent->GetAlignment();
            below = m_beamStaff->GetAlignment();
        }
        else {
            above = m_beamStaff->GetAlignment();
            below = m_crossStaffContent->GetAlignment();
        }
    }
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

FunctorCode BeamDrawingInterface::InterfaceResetData(ResetDataFunctor &functor)
{
    this->BeamDrawingInterface::Reset();

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// StaffDefDrawingInterface
//----------------------------------------------------------------------------

StaffDefDrawingInterface::StaffDefDrawingInterface()
{
    this->Reset();
}

StaffDefDrawingInterface::~StaffDefDrawingInterface() {}

void StaffDefDrawingInterface::Reset()
{
    m_currentClef.Reset();
    m_currentKeySig.Reset();
    m_currentMensur.Reset();
    m_currentMeterSig.Reset();
    m_currentMeterSigGrp.Reset();

    m_drawClef = false;
    m_drawKeySig = false;
    m_drawMensur = false;
    m_drawMeterSig = false;
    m_drawMeterSigGrp = false;
}

void StaffDefDrawingInterface::SetCurrentClef(const Clef *clef)
{
    if (clef) {
        m_currentClef = *clef;
        m_currentClef.CloneReset();
    }
}

void StaffDefDrawingInterface::SetCurrentKeySig(const KeySig *keySig)
{
    if (keySig) {
        const bool ignoreCancel
            = (m_currentKeySig.HasNonAttribKeyAccidChildren() || keySig->HasNonAttribKeyAccidChildren());
        const int drawingCancelAccidCount = m_currentKeySig.GetAccidCount();
        const data_ACCIDENTAL_WRITTEN drawingCancelAccidType = m_currentKeySig.GetAccidType();
        m_currentKeySig = *keySig;
        m_currentKeySig.CloneReset();
        if (ignoreCancel) {
            m_currentKeySig.m_skipCancellation = true;
        }
        else {
            m_currentKeySig.m_drawingCancelAccidCount = drawingCancelAccidCount;
            m_currentKeySig.m_drawingCancelAccidType = drawingCancelAccidType;
        }
    }
}

void StaffDefDrawingInterface::SetCurrentMensur(const Mensur *mensur)
{
    if (mensur) {
        m_currentMensur = *mensur;
        m_currentMensur.CloneReset();
    }
}

void StaffDefDrawingInterface::SetCurrentMeterSig(const MeterSig *meterSig)
{
    if (meterSig) {
        m_currentMeterSig = *meterSig;
        m_currentMeterSig.CloneReset();
    }
}

void StaffDefDrawingInterface::SetCurrentMeterSigGrp(const MeterSigGrp *meterSigGrp)
{
    if (meterSigGrp) {
        m_currentMeterSigGrp = *meterSigGrp;
        m_currentMeterSigGrp.CloneReset();
    }
}

bool StaffDefDrawingInterface::DrawMeterSigGrp() const
{
    if (m_drawMeterSigGrp) {
        const int childListSize = m_currentMeterSigGrp.GetListSize();
        if (childListSize > 1) return true;
    }
    return false;
}

void StaffDefDrawingInterface::AlternateCurrentMeterSig(const Measure *measure)
{
    if (MeterSigGrp *meterSigGrp = this->GetCurrentMeterSigGrp();
        meterSigGrp->GetFunc() == meterSigGrpLog_FUNC_alternating) {
        meterSigGrp->SetMeasureBasedCount(measure);
        MeterSig *meter = meterSigGrp->GetSimplifiedMeterSig();
        this->SetCurrentMeterSig(meter);
        delete meter;
    }
}

void StaffDefDrawingInterface::SetCurrentProport(const Proport *proport)
{
    if (proport) {
        m_currentProport = *proport;
        m_currentProport.CloneReset();
    }
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

FunctorCode StaffDefDrawingInterface::InterfaceResetData(ResetDataFunctor &functor)
{
    // ScoreDefSetCurrent expect the interface content to be preserved
    // Since CloneReset call the ResetData functor, this need to be disabled
    // this->StaffDefDrawingInterface::Reset();

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// StemmedDrawingInterface
//----------------------------------------------------------------------------

StemmedDrawingInterface::StemmedDrawingInterface()
{
    this->Reset();
}

StemmedDrawingInterface::~StemmedDrawingInterface() {}

void StemmedDrawingInterface::Reset()
{
    m_drawingStem = NULL;
}

void StemmedDrawingInterface::SetDrawingStem(Stem *stem)
{
    m_drawingStem = stem;
}

void StemmedDrawingInterface::SetDrawingStemDir(data_STEMDIRECTION stemDir)
{
    if (m_drawingStem) m_drawingStem->SetDrawingStemDir(stemDir);
}

data_STEMDIRECTION StemmedDrawingInterface::GetDrawingStemDir() const
{
    if (m_drawingStem) return m_drawingStem->GetDrawingStemDir();
    return STEMDIRECTION_NONE;
}

void StemmedDrawingInterface::SetDrawingStemLen(int stemLen)
{
    if (m_drawingStem) m_drawingStem->SetDrawingStemLen(stemLen);
}

int StemmedDrawingInterface::GetDrawingStemLen() const
{
    if (m_drawingStem) return m_drawingStem->GetDrawingStemLen();
    return 0;
}

int StemmedDrawingInterface::GetDrawingStemModRelY() const
{
    if (m_drawingStem) return m_drawingStem->GetStemModRelY();
    return 0;
}

Point StemmedDrawingInterface::GetDrawingStemStart(const Object *object) const
{
    assert(m_drawingStem || object);
    if (object && !m_drawingStem) {
        assert(this == object->GetStemmedDrawingInterface());
        return Point(object->GetDrawingX(), object->GetDrawingY());
    }
    return Point(m_drawingStem->GetDrawingX(), m_drawingStem->GetDrawingY());
}

Point StemmedDrawingInterface::GetDrawingStemEnd(const Object *object) const
{
    assert(m_drawingStem || object);
    if (object && !m_drawingStem) {
        assert(this == object->GetStemmedDrawingInterface());
        if (!m_drawingStem) {
            // Somehow arbitrary for chord with no stem - stem end is the bottom
            if (object->Is(CHORD)) {
                const Chord *chord = vrv_cast<const Chord *>(object);
                assert(chord);
                return Point(object->GetDrawingX(), chord->GetYBottom());
            }
            return Point(object->GetDrawingX(), object->GetDrawingY());
        }
    }
    return Point(m_drawingStem->GetDrawingX(), m_drawingStem->GetDrawingY() - this->GetDrawingStemLen());
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

FunctorCode StemmedDrawingInterface::InterfaceResetData(ResetDataFunctor &functor)
{
    this->StemmedDrawingInterface::Reset();

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// VisibilityDrawingInterface
//----------------------------------------------------------------------------

VisibilityDrawingInterface::VisibilityDrawingInterface()
{
    this->Reset();
}

VisibilityDrawingInterface::~VisibilityDrawingInterface() {}

void VisibilityDrawingInterface::Reset()
{
    m_visibility = Visible;
}

} // namespace vrv
