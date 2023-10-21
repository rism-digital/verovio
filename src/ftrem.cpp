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
#include "functor.h"
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
    this->GetList();

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

FunctorCode FTrem::Accept(Functor &functor)
{
    return functor.VisitFTrem(this);
}

FunctorCode FTrem::Accept(ConstFunctor &functor) const
{
    return functor.VisitFTrem(this);
}

FunctorCode FTrem::AcceptEnd(Functor &functor)
{
    return functor.VisitFTremEnd(this);
}

FunctorCode FTrem::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitFTremEnd(this);
}

} // namespace vrv
