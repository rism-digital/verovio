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

FTrem::FTrem() : LayerElement(FTREM), BeamDrawingInterface(), AttFTremVis(), AttTremMeasured()
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

bool FTrem::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ CHORD, CLEF, NOTE };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else if (Object::IsEditorialElement(classId)) {
        return true;
    }
    else {
        return false;
    }
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
