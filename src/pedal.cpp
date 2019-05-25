/////////////////////////////////////////////////////////////////////////////
// Name:        pedal.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pedal.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "horizontalaligner.h"
#include "layerelement.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "MidiFile.h"

namespace vrv {

//----------------------------------------------------------------------------
// Pedal
//----------------------------------------------------------------------------

Pedal::Pedal()
    : ControlElement("pedal-"), TimePointInterface(), AttColor(), AttPedalLog(), AttPlacement(), AttVerticalGroup()
{
    RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_PEDALLOG);
    RegisterAttClass(ATT_PLACEMENT);
    RegisterAttClass(ATT_VERTICALGROUP);

    Reset();
}

Pedal::~Pedal() {}

void Pedal::Reset()
{
    ControlElement::Reset();
    TimePointInterface::Reset();
    ResetColor();
    ResetPedalLog();
    ResetPlacement();
    ResetVerticalGroup();
}

//----------------------------------------------------------------------------
// Pedal functor methods
//----------------------------------------------------------------------------

int Pedal::GenerateMIDI(FunctorParams *functorParams)
{
    GenerateMIDIParams *params = dynamic_cast<GenerateMIDIParams *>(functorParams);
    assert(params);

    // Sameas not taken into account for now
    if (!HasDir()) return FUNCTOR_CONTINUE;

    double pedalTime = GetStart()->GetAlignment()->GetTime() * DURATION_4 / DUR_MAX;
    double starttime = params->m_totalTime + pedalTime;
    int tpq = params->m_midiFile->getTPQ();

    // todo: check pedal @func to switch between sustain/soften/damper pedals?
    switch (GetDir()) {
        case pedalLog_DIR_down:
            params->m_midiFile->addSustainPedalOn(params->m_midiTrack, (starttime * tpq), params->m_midiChannel);
            break;
        case pedalLog_DIR_up:
            params->m_midiFile->addSustainPedalOff(params->m_midiTrack, (starttime * tpq), params->m_midiChannel);
            break;
        default: return FUNCTOR_CONTINUE;
    }

    return FUNCTOR_CONTINUE;
}

int Pedal::PrepareFloatingGrps(FunctorParams *)
{
    if (this->HasVgrp()) {
        this->SetDrawingGrpId(-this->GetVgrp());
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
