/////////////////////////////////////////////////////////////////////////////
// Name:        rpt.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "rpt.h"

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

namespace vrv {

//----------------------------------------------------------------------------
// BeatRpt
//----------------------------------------------------------------------------

BeatRpt::BeatRpt() : LayerElement("beatrpt-"), AttColor(), AttBeatRptVis()
{
    RegisterAttClass(ATT_BEATRPTVIS);
    RegisterAttClass(ATT_COLOR);
    Reset();
}

BeatRpt::~BeatRpt()
{
}

void BeatRpt::Reset()
{
    LayerElement::Reset();
    ResetBeatRptVis();
    ResetColor();
}

double BeatRpt::GetBeatRptAlignmentDuration(int meterUnit) const
{
    return DUR_MAX / meterUnit;
}

//----------------------------------------------------------------------------
// BTrem
//----------------------------------------------------------------------------

BTrem::BTrem() : LayerElement("btrem-"), AttTremmeasured()
{
    RegisterAttClass(ATT_TREMMEASURED);

    Reset();
}

BTrem::~BTrem()
{
}

void BTrem::Reset()
{
    LayerElement::Reset();
    ResetTremmeasured();
}

void BTrem::AddChild(Object *child)
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
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}
//----------------------------------------------------------------------------
// FTrem
//----------------------------------------------------------------------------

FTrem::FTrem() : LayerElement("ftrem-"), ObjectListInterface(), AttSlashcount(), AttTremmeasured()
{
    RegisterAttClass(ATT_SLASHCOUNT);
    RegisterAttClass(ATT_TREMMEASURED);

    Reset();
}

FTrem::~FTrem()
{
    ClearCoords();
}

void FTrem::Reset()
{
    LayerElement::Reset();
    ResetSlashcount();
    ResetTremmeasured();
}

void FTrem::AddChild(Object *child)
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
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

void FTrem::FilterList(ListOfObjects *childList)
{
    ListOfObjects::iterator iter = childList->begin();

    while (iter != childList->end()) {
        if (!(*iter)->Is(NOTE) && !(*iter)->Is(CHORD)) {
            // remove anything that is not an LayerElement (e.g. Verse, Syl, etc)
            iter = childList->erase(iter);
            continue;
        }
        // also remove notes within chords
        if ((*iter)->Is(NOTE)) {
            Note *note = dynamic_cast<Note *>(*iter);
            assert(note);
            if (note->IsChordTone()) {
                iter = childList->erase(iter);
                continue;
            }
        }
        iter++;
    }

    InitCoords(childList);
}

void FTrem::InitCoords(ListOfObjects *childList)
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

    this->m_drawingParams.m_changingDur = false;
    this->m_drawingParams.m_beamHasChord = false;
    this->m_drawingParams.m_hasMultipleStemDir = false;
    this->m_drawingParams.m_cueSize = false;
    // adjust beam->m_drawingParams.m_shortestDur depending on the number of slashes
    this->m_drawingParams.m_shortestDur = std::max(DUR_8, DUR_1 + this->GetSlash());
    this->m_drawingParams.m_stemDir = STEMDIRECTION_NONE;

    if (firstElement->m_element->Is(CHORD)) {
        this->m_drawingParams.m_beamHasChord = true;
    }
    if (secondElement->m_element->Is(CHORD)) {
        this->m_drawingParams.m_beamHasChord = true;
    }

    // For now look at the stemDir only on the first note
    assert(dynamic_cast<AttStems *>(firstElement->m_element));
    this->m_drawingParams.m_stemDir = (dynamic_cast<AttStems *>(firstElement->m_element))->GetStemDir();

    // We look only at the first note for checking if cue-sized. Somehow arbitrarily
    this->m_drawingParams.m_cueSize = firstElement->m_element->IsCueSize();
}

void FTrem::ClearCoords()
{
    ArrayOfBeamElementCoords::iterator iter;
    for (iter = m_beamElementCoords.begin(); iter != m_beamElementCoords.end(); ++iter) {
        delete *iter;
    }
    m_beamElementCoords.clear();
}

//----------------------------------------------------------------------------
// MRpt
//----------------------------------------------------------------------------

MRpt::MRpt() : LayerElement("mrpt-")
{
    Reset();
}

MRpt::~MRpt()
{
}

void MRpt::Reset()
{
    LayerElement::Reset();

    m_drawingMeasureCount = 0;
}

//----------------------------------------------------------------------------
// MRpt2
//----------------------------------------------------------------------------

MRpt2::MRpt2() : LayerElement("mrpt2-")
{
    Reset();
}

MRpt2::~MRpt2()
{
}

void MRpt2::Reset()
{
    LayerElement::Reset();
}

//----------------------------------------------------------------------------
// MultiRpt
//----------------------------------------------------------------------------

MultiRpt::MultiRpt() : LayerElement("multirpt-"), AttNumbered()
{
    RegisterAttClass(ATT_NUMBERED);
    Reset();
}

MultiRpt::~MultiRpt()
{
}

void MultiRpt::Reset()
{
    LayerElement::Reset();
    ResetNumbered();
}

//----------------------------------------------------------------------------
// MRpt functor methods
//----------------------------------------------------------------------------

int MRpt::PrepareRpt(FunctorParams *functorParams)
{
    PrepareRptParams *params = dynamic_cast<PrepareRptParams *>(functorParams);
    assert(params);

    // If multiNumber is not true, nothing needs to be done
    if (params->m_multiNumber != BOOLEAN_true) {
        return FUNCTOR_CONTINUE;
    }

    // If this is the first one, number has to be 2
    if (params->m_currentMRpt == NULL) {
        this->m_drawingMeasureCount = 2;
    }
    // Otherwise increment it
    else {
        this->m_drawingMeasureCount = params->m_currentMRpt->m_drawingMeasureCount + 1;
    }
    params->m_currentMRpt = this;
    return FUNCTOR_CONTINUE;
}

int FTrem::CalcStem(FunctorParams *functorParams)
{
    CalcStemParams *params = dynamic_cast<CalcStemParams *>(functorParams);
    assert(params);

    ListOfObjects *fTremChildren = this->GetList(this);

    // Should we assert this at the beginning?
    if (fTremChildren->empty()) {
        return FUNCTOR_CONTINUE;
    }
    const ArrayOfBeamElementCoords *beamElementCoords = this->GetElementCoords();

    assert(beamElementCoords->size() == 2);

    int elementCount = 2;

    Layer *layer = dynamic_cast<Layer *>(this->GetFirstParent(LAYER));
    assert(layer);
    Staff *staff = dynamic_cast<Staff *>(layer->GetFirstParent(STAFF));
    assert(staff);

    this->m_drawingParams.CalcBeam(layer, staff, params->m_doc, beamElementCoords, elementCount);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
