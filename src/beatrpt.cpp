/////////////////////////////////////////////////////////////////////////////
// Name:        beatrpt.cpp
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "beatrpt.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <math.h>

//----------------------------------------------------------------------------

#include "chord.h"
#include "editorial.h"
#include "functor.h"
#include "functorparams.h"
#include "layer.h"
#include "note.h"
#include "staff.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "MidiFile.h"

namespace vrv {

//----------------------------------------------------------------------------
// BeatRpt
//----------------------------------------------------------------------------

static const ClassRegistrar<BeatRpt> s_factory("beatRpt", BEATRPT);

BeatRpt::BeatRpt() : LayerElement(BEATRPT, "beatrpt-"), AttColor(), AttBeatRptLog(), AttBeatRptVis()
{
    this->RegisterAttClass(ATT_BEATRPTLOG);
    this->RegisterAttClass(ATT_BEATRPTVIS);
    this->RegisterAttClass(ATT_COLOR);
    this->Reset();
}

BeatRpt::~BeatRpt() {}

void BeatRpt::Reset()
{
    LayerElement::Reset();
    this->ResetBeatRptLog();
    this->ResetBeatRptVis();
    this->ResetColor();

    m_scoreTimeOnset = 0.0;
}

double BeatRpt::GetBeatRptAlignmentDuration(int meterUnit) const
{
    double dur = DUR_MAX / meterUnit;
    if (this->HasBeatdef()) dur *= this->GetBeatdef();
    return dur;
}

void BeatRpt::SetScoreTimeOnset(double scoreTime)
{
    m_scoreTimeOnset = scoreTime;
}

double BeatRpt::GetScoreTimeOnset() const
{
    return m_scoreTimeOnset;
}

//----------------------------------------------------------------------------
// BeatRpt functor methods
//----------------------------------------------------------------------------

FunctorCode BeatRpt::Accept(MutableFunctor &functor)
{
    return functor.VisitBeatRpt(this);
}

FunctorCode BeatRpt::Accept(ConstFunctor &functor) const
{
    return functor.VisitBeatRpt(this);
}

FunctorCode BeatRpt::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitBeatRptEnd(this);
}

FunctorCode BeatRpt::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitBeatRptEnd(this);
}

int BeatRpt::GenerateMIDI(FunctorParams *functorParams)
{
    GenerateMIDIParams *params = vrv_params_cast<GenerateMIDIParams *>(functorParams);
    assert(params);

    // Sameas not taken into account for now
    double beatLength = this->GetAlignmentDuration() / (DUR_MAX / DURATION_4);
    double starttime = params->m_totalTime + this->GetScoreTimeOnset();
    int tpq = params->m_midiFile->getTPQ();

    // filter last beat and copy all notes
    smf::MidiEvent event;
    int eventcount = params->m_midiFile->getEventCount(params->m_midiTrack);
    for (int i = 0; i < eventcount; ++i) {
        event = params->m_midiFile->getEvent(params->m_midiTrack, i);
        if (event.tick > starttime * tpq)
            break;
        else if (event.tick >= (starttime - beatLength) * tpq) {
            if (((event[0] & 0xf0) == 0x80) || ((event[0] & 0xf0) == 0x90)) {
                params->m_midiFile->addEvent(params->m_midiTrack, event.tick + beatLength * tpq, event);
            }
        }
    }

    for (int i = 0; i < beatLength * tpq; ++i) {
        // LogWarning("%i", i);
        // smf::MidiEvent event = params->m_midiFile->getEvent(params->m_midiTrack, starttime * tpq);
        // event.clearVariables();
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
