/////////////////////////////////////////////////////////////////////////////
// Name:        ftrem.cpp
// Author:      Klaus Rettinghaus
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "ftrem.h"

//----------------------------------------------------------------------------

#include <cassert>
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

static const ClassRegistrar<FTrem> s_factory("fTrem", FTREM);

FTrem::FTrem() : LayerElement(FTREM, "ftrem-"), BeamDrawingInterface(), AttFTremVis(), AttTremMeasured()
{
    this->RegisterAttClass(ATT_FTREMVIS);
    this->RegisterAttClass(ATT_TREMMEASURED);

    this->Reset();
}

FTrem::~FTrem() {}

void FTrem::Reset()
{
    LayerElement::Reset();
    BeamDrawingInterface::Reset();
    this->ResetFTremVis();
    this->ResetTremMeasured();
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

void FTrem::FilterList(ListOfConstObjects &childList) const
{
    ListOfConstObjects::iterator iter = childList.begin();

    while (iter != childList.end()) {
        if (!(*iter)->Is(NOTE) && !(*iter)->Is(CHORD)) {
            // remove anything that is not an LayerElement (e.g. Verse, Syl, etc.)
            iter = childList.erase(iter);
            continue;
        }
        // also remove notes within chords
        if ((*iter)->Is(NOTE)) {
            const Note *note = vrv_cast<const Note *>(*iter);
            assert(note);
            if (note->IsChordTone()) {
                iter = childList.erase(iter);
                continue;
            }
        }
        ++iter;
    }
}

std::pair<int, int> FTrem::GetAdditionalBeamCount() const
{
    return { 0, std::max(this->GetBeams(), this->GetBeamsFloat()) };
}

std::pair<int, int> FTrem::GetFloatingBeamCount() const
{
    return { this->GetBeams(), this->GetBeamsFloat() };
}

void FTrem::SetElementShortening(int shortening)
{
    std::for_each(m_beamSegment.m_beamElementCoordRefs.begin(), m_beamSegment.m_beamElementCoordRefs.end(),
        [shortening](BeamElementCoord *coord) { coord->m_maxShortening = shortening; });
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int FTrem::AdjustBeams(FunctorParams *functorParams)
{
    AdjustBeamParams *params = vrv_params_cast<AdjustBeamParams *>(functorParams);
    assert(params);

    if (this->HasSameas() || !this->GetChildCount() || m_beamSegment.m_beamElementCoordRefs.empty()) {
        return FUNCTOR_CONTINUE;
    }

    if (!params->m_beam) {
        if (m_drawingPlace == BEAMPLACE_mixed) {
            m_beamSegment.RequestStaffSpace(params->m_doc, this);
        }
        else {
            params->m_beam = this;
            params->m_y1 = (*m_beamSegment.m_beamElementCoordRefs.begin())->m_yBeam;
            params->m_y2 = m_beamSegment.m_beamElementCoordRefs.back()->m_yBeam;
            params->m_x1 = m_beamSegment.m_beamElementCoordRefs.front()->m_x;
            params->m_beamSlope = m_beamSegment.m_beamSlope;
            params->m_directionBias = (m_drawingPlace == BEAMPLACE_above) ? 1 : -1;
            params->m_overlapMargin
                = this->CalcLayerOverlap(params->m_doc, params->m_directionBias, params->m_y1, params->m_y2);
        }
        return FUNCTOR_CONTINUE;
    }

    const int leftMargin = (*m_beamSegment.m_beamElementCoordRefs.begin())->m_yBeam - params->m_y1;
    const int rightMargin = m_beamSegment.m_beamElementCoordRefs.back()->m_yBeam - params->m_y2;

    const int overlapMargin = std::max(leftMargin * params->m_directionBias, rightMargin * params->m_directionBias);
    if (overlapMargin >= params->m_overlapMargin) {
        Staff *staff = this->GetAncestorStaff();
        const int staffOffset = params->m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        params->m_overlapMargin = (overlapMargin + staffOffset) * params->m_directionBias;
    }
    return FUNCTOR_SIBLINGS;
}

int FTrem::AdjustBeamsEnd(FunctorParams *functorParams)
{
    AdjustBeamParams *params = vrv_params_cast<AdjustBeamParams *>(functorParams);
    assert(params);

    if (params->m_beam != this) return FUNCTOR_CONTINUE;

    if (m_drawingPlace == BEAMPLACE_mixed) return FUNCTOR_CONTINUE;

    Layer *parentLayer = vrv_cast<Layer *>(this->GetFirstAncestor(LAYER));
    if (parentLayer) {
        // find elements on the other layers for the duration of the current beam
        auto otherLayersElements = parentLayer->GetLayerElementsForTimeSpanOf(this, true);
        if (!otherLayersElements.empty()) {
            // call AdjustBeams separately for each element to find possible overlaps
            params->m_isOtherLayer = true;
            for (const auto element : otherLayersElements) {
                if (!params->m_beam->HorizontalContentOverlap(element)) continue;
                element->AdjustBeams(params);
            }
            params->m_isOtherLayer = false;
        }
    }

    // set overlap margin for each coord in the beam
    if (params->m_overlapMargin) {
        std::for_each(m_beamSegment.m_beamElementCoordRefs.begin(), m_beamSegment.m_beamElementCoordRefs.end(),
            [overlap = params->m_overlapMargin](BeamElementCoord *coord) { coord->m_overlapMargin = overlap; });
    }
    params->m_beam = NULL;
    params->m_overlapMargin = 0;

    return FUNCTOR_CONTINUE;
}

int FTrem::CalcStem(FunctorParams *functorParams)
{
    CalcStemParams *params = vrv_params_cast<CalcStemParams *>(functorParams);
    assert(params);

    const ListOfObjects &fTremChildren = this->GetList(this);

    // Should we assert this at the beginning?
    if (fTremChildren.empty()) {
        return FUNCTOR_CONTINUE;
    }

    Layer *layer = vrv_cast<Layer *>(this->GetFirstAncestor(LAYER));
    assert(layer);
    Staff *staff = vrv_cast<Staff *>(layer->GetFirstAncestor(STAFF));
    assert(staff);

    if (!this->HasCoords()) {
        this->InitCoords(fTremChildren, staff, BEAMPLACE_NONE);
        this->InitCue(false);
    }

    if (this->GetElementCoords()->size() != 2) {
        LogError("Stem calculation: <fTrem> element has invalid number of descendants.");
        return FUNCTOR_CONTINUE;
    }

    m_beamSegment.InitCoordRefs(this->GetElementCoords());

    m_beamSegment.CalcBeam(layer, staff, params->m_doc, this);

    return FUNCTOR_CONTINUE;
}

int FTrem::ResetData(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetData(functorParams);

    m_beamSegment.Reset();

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
