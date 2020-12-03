/////////////////////////////////////////////////////////////////////////////
// Name:        drawinginterface.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "drawinginterface.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "chord.h"
#include "elementpart.h"
#include "layerelement.h"
#include "note.h"
#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// DrawingListInterface
//----------------------------------------------------------------------------

DrawingListInterface::DrawingListInterface()
{
    Reset();
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
// BeamDrawingInterface
//----------------------------------------------------------------------------

BeamDrawingInterface::BeamDrawingInterface()
{
    Reset();
}

BeamDrawingInterface::~BeamDrawingInterface()
{
    ClearCoords();
}

void BeamDrawingInterface::Reset()
{
    m_changingDur = false;
    m_beamHasChord = false;
    m_hasMultipleStemDir = false;
    m_cueSize = false;
    m_hasCrossStaffContent = false;
    m_shortestDur = 0;
    m_notesStemDir = STEMDIRECTION_NONE;
    m_drawingPlace = BEAMPLACE_NONE;
    m_beamStaff = NULL;

    m_beamWidth = 0;
    m_beamWidthBlack = 0;
    m_beamWidthWhite = 0;
}

void BeamDrawingInterface::ClearCoords()
{
    ArrayOfBeamElementCoords::iterator iter;
    for (iter = m_beamElementCoords.begin(); iter != m_beamElementCoords.end(); ++iter) {
        delete *iter;
    }
    m_beamElementCoords.clear();
}

void BeamDrawingInterface::InitCoords(ArrayOfObjects *childList, Staff *staff, data_BEAMPLACE place)
{
    assert(staff);

    ClearCoords();

    if (childList->empty()) {
        return;
    }

    m_beamStaff = staff;

    // duration variables
    int lastDur, currentDur;

    m_beamElementCoords.reserve(childList->size());
    int i;
    for (i = 0; i < (int)childList->size(); ++i) {
        m_beamElementCoords.push_back(new BeamElementCoord());
    }

    // current point to the first Note in the layed out layer
    LayerElement *current = dynamic_cast<LayerElement *>(childList->front());
    // Beam list should contain only DurationInterface objects
    assert(current->GetDurationInterface());

    lastDur = (current->GetDurationInterface())->GetActualDur();

    /******************************************************************/
    // Populate BeamElementCoord for each element in the beam
    // This could be moved to Beam::InitCoord for optimization because there should be no
    // need for redoing it everytime it is drawn.

    data_STEMDIRECTION currentStemDir;
    Layer *layer = NULL;
    Staff *currentStaff = NULL;

    int elementCount = 0;

    ArrayOfObjects::iterator iter = childList->begin();
    do {
        // Beam list should contain only DurationInterface objects
        assert(current->GetDurationInterface());
        currentDur = (current->GetDurationInterface())->GetActualDur();

        if (current->Is(CHORD)) {
            this->m_beamHasChord = true;
        }

        m_beamElementCoords.at(elementCount)->m_element = current;
        m_beamElementCoords.at(elementCount)->m_dur = currentDur;

        // Look at beam breaks
        m_beamElementCoords.at(elementCount)->m_breaksec = 0;
        AttBeamSecondary *beamsecondary = dynamic_cast<AttBeamSecondary *>(current);
        if (beamsecondary && beamsecondary->HasBreaksec()) {
            if (!this->m_changingDur) this->m_changingDur = true;
            m_beamElementCoords.at(elementCount)->m_breaksec = beamsecondary->GetBreaksec();
        }

        Staff *staff = current->GetCrossStaff(layer);
        if (staff != currentStaff) {
            this->m_hasCrossStaffContent = true;
        }
        currentStaff = staff;

        // Skip rests
        if (current->Is({ NOTE, CHORD })) {
            // Look at the stemDir to see if we have multiple stem Dir
            if (!this->m_hasMultipleStemDir) {
                // At this stage, BeamCoord::m_stem is not necssary set, so we need to look at the Note / Chord original
                // value Example: IsInBeam called in Note::PrepareLayerElementParts when reaching the first note of the
                // beam
                currentStemDir = m_beamElementCoords.at(elementCount)->GetStemDir();
                if (currentStemDir != STEMDIRECTION_NONE) {
                    if ((this->m_notesStemDir != STEMDIRECTION_NONE) && (this->m_notesStemDir != currentStemDir)) {
                        this->m_hasMultipleStemDir = true;
                        this->m_notesStemDir = STEMDIRECTION_NONE;
                    }
                    else {
                        this->m_notesStemDir = currentStemDir;
                    }
                }
            }
            // keep the shortest dur in the beam
            this->m_shortestDur = std::max(currentDur, this->m_shortestDur);
        }
        // check if we have more than duration in the beam
        if (!this->m_changingDur && currentDur != lastDur) this->m_changingDur = true;
        lastDur = currentDur;

        elementCount++;

        ++iter;
        if (iter == childList->end()) {
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

    int last = elementCount - 1;

    // We look only at the last note for checking if cue-sized. Somehow arbitrarily
    this->m_cueSize = m_beamElementCoords.at(last)->m_element->GetDrawingCueSize();

    // Always set stem diretion to up for grace note beam unless stem direction is provided
    if (this->m_cueSize && (this->m_notesStemDir == STEMDIRECTION_NONE)) {
        this->m_notesStemDir = STEMDIRECTION_up;
    }
}

bool BeamDrawingInterface::IsHorizontal()
{
    if (this->IsRepeatedPattern()) {
        return true;
    }

    if (HasOneStepHeight()) return true;

    // if (m_drawingPlace == BEAMPLACE_mixed) return true;

    if (m_drawingPlace == BEAMPLACE_NONE) return true;

    int elementCount = (int)m_beamElementCoords.size();

    std::vector<int> items;
    items.reserve(m_beamElementCoords.size());

    int i;
    for (i = 0; i < elementCount; ++i) {
        BeamElementCoord *coord = m_beamElementCoords.at(i);
        if (!coord->m_stem || !coord->m_closestNote) continue;

        items.push_back(coord->m_closestNote->GetDrawingY());
    }
    int itemCount = (int)items.size();

    if (itemCount < 2) return true;

    const int first = items.front();
    const int last = items.back();

    // First note and last note have the same postion
    if (first == last) return true;

    // Detect concave shapes
    for (i = 1; i < itemCount - 1; ++i) {
        if (m_drawingPlace == BEAMPLACE_above) {
            if ((items.at(i) > first) && (items.at(i) > last)) return true;
        }
        else {
            if ((items.at(i) < first) && (items.at(i) < last)) return true;
        }
    }

    // Detect beam with two pitches only and as step at the beginning or at the end
    const bool firstStep = (first != items.at(1));
    const bool lastStep = (last != items.at(items.size() - 2));
    if ((items.size() > 2) && (firstStep || lastStep)) {
        std::vector<int> pitches;
        std::unique_copy(items.begin(), items.end(), std::back_inserter(pitches));
        if (pitches.size() == 2) {
            // if (firstStep)
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

bool BeamDrawingInterface::IsRepeatedPattern()
{
    if (m_drawingPlace == BEAMPLACE_mixed) return false;

    if (m_drawingPlace == BEAMPLACE_NONE) return false;

    int elementCount = (int)m_beamElementCoords.size();

    // No pattern with at least 4 elements
    if (elementCount < 4) return false;

    std::vector<int> items;
    items.reserve(m_beamElementCoords.size());

    int i;
    for (i = 0; i < elementCount; ++i) {
        BeamElementCoord *coord = m_beamElementCoords.at(i);
        if (!coord->m_stem || !coord->m_closestNote) continue;

        // Could this be an overflow with 32 bits?
        items.push_back(coord->m_closestNote->GetDrawingY() * DUR_MAX + coord->m_dur);
    }
    int itemCount = (int)items.size();

    // No pattern with at least 4 elements or if all elements are the same
    if ((itemCount < 4) || (std::equal(items.begin() + 1, items.end(), items.begin()))) {
        return false;
    }

    // Find all possible dividers for the sequence (without 1 and its size)
    std::vector<int> dividers;
    for (i = 2; i <= itemCount / 2; ++i) {
        if (itemCount % i == 0) dividers.push_back(i);
    }

    // Correlate a sub-array for each divider until a sequence is found (if any)
    for (i = 0; i < (int)dividers.size(); ++i) {
        int divider = dividers.at(i);
        int j;
        bool pattern = true;
        std::vector<int>::iterator iter = items.begin();
        std::vector<int> v1 = std::vector<int>(iter, iter + divider);
        for (j = 1; j < (itemCount / divider); ++j) {
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

bool BeamDrawingInterface::HasOneStepHeight()
{
    if (m_shortestDur < DUR_32) return false;

    int top = -128;
    int bottom = 128;
    for (auto coord : m_beamElementCoords) {
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

//----------------------------------------------------------------------------
// StaffDefDrawingInterface
//----------------------------------------------------------------------------

StaffDefDrawingInterface::StaffDefDrawingInterface()
{
    Reset();
}

StaffDefDrawingInterface::~StaffDefDrawingInterface() {}

void StaffDefDrawingInterface::Reset()
{
    m_currentClef.Reset();
    m_currentKeySig.Reset();
    m_currentMensur.Reset();
    m_currentMeterSig.Reset();

    m_drawClef = false;
    m_drawKeySig = false;
    m_drawMensur = false;
    m_drawMeterSig = false;
}

void StaffDefDrawingInterface::SetCurrentClef(Clef const *clef)
{
    if (clef) {
        m_currentClef = *clef;
        m_currentClef.CloneReset();
    }
}

void StaffDefDrawingInterface::SetCurrentKeySig(KeySig const *keySig)
{
    if (keySig) {
        char drawingCancelAccidCount = m_currentKeySig.GetAccidCount();
        data_ACCIDENTAL_WRITTEN drawingCancelAccidType = m_currentKeySig.GetAccidType();
        m_currentKeySig = *keySig;
        m_currentKeySig.CloneReset();
        m_currentKeySig.m_drawingCancelAccidCount = drawingCancelAccidCount;
        m_currentKeySig.m_drawingCancelAccidType = drawingCancelAccidType;
    }
}

void StaffDefDrawingInterface::SetCurrentMensur(Mensur const *mensur)
{
    if (mensur) {
        m_currentMensur = *mensur;
        m_currentMensur.CloneReset();
    }
}

void StaffDefDrawingInterface::SetCurrentMeterSig(MeterSig const *meterSig)
{
    if (meterSig) {
        m_currentMeterSig = *meterSig;
        m_currentMeterSig.CloneReset();
    }
}

//----------------------------------------------------------------------------
// StemmedDrawingInterface
//----------------------------------------------------------------------------

StemmedDrawingInterface::StemmedDrawingInterface()
{
    Reset();
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

data_STEMDIRECTION StemmedDrawingInterface::GetDrawingStemDir()
{
    if (m_drawingStem) return m_drawingStem->GetDrawingStemDir();
    return STEMDIRECTION_NONE;
}

void StemmedDrawingInterface::SetDrawingStemLen(int stemLen)
{
    if (m_drawingStem) m_drawingStem->SetDrawingStemLen(stemLen);
}

int StemmedDrawingInterface::GetDrawingStemLen()
{
    if (m_drawingStem) return m_drawingStem->GetDrawingStemLen();
    return 0;
}

Point StemmedDrawingInterface::GetDrawingStemStart(Object *object)
{
    assert(m_drawingStem || object);
    if (object && !m_drawingStem) {
        assert(this == dynamic_cast<StemmedDrawingInterface *>(object));
        return Point(object->GetDrawingX(), object->GetDrawingY());
    }
    return Point(m_drawingStem->GetDrawingX(), m_drawingStem->GetDrawingY());
}

Point StemmedDrawingInterface::GetDrawingStemEnd(Object *object)
{
    assert(m_drawingStem || object);
    if (object && !m_drawingStem) {
        assert(this == dynamic_cast<StemmedDrawingInterface *>(object));
        if (!m_drawingStem) {
            // Somehow arbitrary for chord - stem end it the bottom with no stem
            if (object->Is(CHORD)) {
                Chord *chord = vrv_cast<Chord *>(object);
                assert(chord);
                return Point(object->GetDrawingX(), chord->GetYBottom());
            }
            return Point(object->GetDrawingX(), object->GetDrawingY());
        }
    }
    return Point(m_drawingStem->GetDrawingX(), m_drawingStem->GetDrawingY() - GetDrawingStemLen());
}

} // namespace vrv
