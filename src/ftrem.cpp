/////////////////////////////////////////////////////////////////////////////
// Name:        ftrem.cpp
// Author:      Klaus Rettinghaus
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "ftrem.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "chord.h"
#include "editorial.h"
#include "functorparams.h"
#include "layer.h"
#include "note.h"
#include "staff.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "MidiFile.h"

namespace vrv {

//----------------------------------------------------------------------------
// FTrem
//----------------------------------------------------------------------------

FTrem::FTrem() : LayerElement("ftrem-"), ObjectListInterface(), BeamDrawingInterface(), AttFTremVis(), AttTremMeasured()
{
    RegisterAttClass(ATT_FTREMVIS);
    RegisterAttClass(ATT_TREMMEASURED);

    Reset();
}

FTrem::~FTrem() {}

void FTrem::Reset()
{
    LayerElement::Reset();
    BeamDrawingInterface::Reset();
    ResetFTremVis();
    ResetTremMeasured();
}

bool FTrem::IsSupportedChild(Object *child)
{
    if (child->Is(CHORD)) {
        assert(dynamic_cast<Chord *>(child));
    }
    else if (child->Is(CLEF)) {
        assert(dynamic_cast<Clef *>(child));
    }
    else if (child->Is(NOTE)) {
        assert(dynamic_cast<Note *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

const ArrayOfBeamElementCoords *FTrem::GetElementCoords()
{
    this->GetList(this);

    return &m_beamElementCoords;
}

void FTrem::FilterList(ArrayOfObjects *childList)
{
    ArrayOfObjects::iterator iter = childList->begin();

    while (iter != childList->end()) {
        if (!(*iter)->Is(NOTE) && !(*iter)->Is(CHORD)) {
            // remove anything that is not an LayerElement (e.g. Verse, Syl, etc)
            iter = childList->erase(iter);
            continue;
        }
        // also remove notes within chords
        if ((*iter)->Is(NOTE)) {
            Note *note = vrv_cast<Note *>(*iter);
            assert(note);
            if (note->IsChordTone()) {
                iter = childList->erase(iter);
                continue;
            }
        }
        ++iter;
    }

    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);

    InitCoords(childList, staff, BEAMPLACE_NONE);
}

/*
void FTrem::InitCoords(ArrayOfObjects *childList)
{
    ClearCoords();

    if (childList->empty()) {
        return;
    }

    BeamElementCoord *firstElement = new BeamElementCoord;
    BeamElementCoord *secondElement = new BeamElementCoord;

    m_beamElementCoords.push_back(firstElement);
    m_beamElementCoords.push_back(secondElement);

    // current point to the first Note in the layed out layer
    firstElement->m_element = dynamic_cast<LayerElement *>(childList->front());
    // fTrem list should contain only DurationInterface objects
    assert(firstElement->m_element->GetDurationInterface());
    // current point to the first Note in the layed out layer
    secondElement->m_element = dynamic_cast<LayerElement *>(childList->back());
    // fTrem list should contain only DurationInterface objects
    assert(secondElement->m_element->GetDurationInterface());
    // Should we assert this at the beginning?
    if (firstElement->m_element == secondElement->m_element) {
        return;
    }

    this->m_changingDur = false;
    this->m_beamHasChord = false;
    this->m_hasMultipleStemDir = false;
    this->m_cueSize = false;
    // adjust beam->m_drawingParams.m_shortestDur depending on the number of slashes
    this->m_shortestDur = std::max(DUR_8, DUR_1 + this->GetBeams());
    this->m_stemDir = STEMDIRECTION_NONE;

    if (firstElement->m_element->Is(CHORD)) {
        this->m_beamHasChord = true;
    }
    if (secondElement->m_element->Is(CHORD)) {
        this->m_beamHasChord = true;
    }

    // For now look at the stemDir only on the first note
    assert(dynamic_cast<AttStems *>(firstElement->m_element));
    this->m_stemDir = (dynamic_cast<AttStems *>(firstElement->m_element))->GetStemDir();

    // We look only at the first note for checking if cue-sized. Somehow arbitrarily
    this->m_cueSize = firstElement->m_element->GetDrawingCueSize();
 }
 */

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int FTrem::CalcStem(FunctorParams *functorParams)
{
    CalcStemParams *params = vrv_params_cast<CalcStemParams *>(functorParams);
    assert(params);

    const ArrayOfObjects *fTremChildren = this->GetList(this);

    // Should we assert this at the beginning?
    if (fTremChildren->empty()) {
        return FUNCTOR_CONTINUE;
    }

    if (GetElementCoords()->size() != 2) {
        LogError("Stem calculation: <fTrem> element has invalid number of descendants.");
        return FUNCTOR_STOP;
    }

    this->m_beamSegment.InitCoordRefs(this->GetElementCoords());

    Layer *layer = vrv_cast<Layer *>(this->GetFirstAncestor(LAYER));
    assert(layer);
    Staff *staff = vrv_cast<Staff *>(layer->GetFirstAncestor(STAFF));
    assert(staff);

    this->m_beamSegment.CalcBeam(layer, staff, params->m_doc, this);

    return FUNCTOR_CONTINUE;
}

int FTrem::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);

    this->m_beamSegment.Reset();

    // We want the list of the ObjectListInterface to be re-generated
    this->Modify();
    return FUNCTOR_CONTINUE;
}

int FTrem::GenerateMIDI(FunctorParams *functorParams)
{
    // GenerateMIDIParams *params = vrv_params_cast<GenerateMIDIParams *>(functorParams);
    // assert(params);

    FTrem *fTrem = vrv_cast<FTrem *>(this);
    assert(fTrem);

    if (!fTrem->HasUnitdur()) {
        return FUNCTOR_CONTINUE;
    }
    else {
        LogWarning("FTrem produces incorrect MIDI output");
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
