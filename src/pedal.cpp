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
#include "verticalaligner.h"
#include "vrv.h"
#include "MidiFile.h"
#include "layerelement.h"
#include "horizontalaligner.h"

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

int Pedal::GenerateMIDI(FunctorParams * functorParams)
{
    GenerateMIDIParams *params = dynamic_cast<GenerateMIDIParams *>(functorParams);
    assert(params);

    if (!HasDir()) return FUNCTOR_CONTINUE;

    int velocity;
    int tickOffset = 0;
    switch (GetDir())
    {
    case pedalLog_DIR_down:
        velocity = 126;
        break;
    case pedalLog_DIR_up:
        velocity = 0;

        // if you have a pedal event that ends right as another begins (ex. one in the end of measure 1 and start of measure 2), 
        // they will overlap because they occur at the same midi time
        // so we just add a -1 offset to the ending time to remedy this until a better method is thought of
        tickOffset = -1;
        break;
    default:
        return FUNCTOR_CONTINUE;
    }

    // todo: check pedal @func to switch between sustain/soften/damper pedals?

    double pedalTime = GetStart()->GetAlignment()->GetTime() * DURATION_4 / DUR_MAX;
    double starttime = params->m_totalTime + pedalTime;
    int tpq = params->m_midiFile->getTPQ();

    params->m_midiFile->addSustainPedal(params->m_midiTrack, (starttime * tpq) + tickOffset, params->m_midiChannel, velocity);

    return FUNCTOR_SIBLINGS;
}

int Pedal::PrepareFloatingGrps(FunctorParams *functorParams)
{
    PrepareFloatingGrpsParams *params = dynamic_cast<PrepareFloatingGrpsParams *>(functorParams);
    assert(params);

    if (this->HasVgrp()) {
        this->SetDrawingGrpId(-this->GetVgrp());
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
