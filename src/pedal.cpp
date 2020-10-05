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
#include "smufl.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "MidiFile.h"

namespace vrv {

//----------------------------------------------------------------------------
// Pedal
//----------------------------------------------------------------------------

Pedal::Pedal()
    : ControlElement("pedal-")
    , TimeSpanningInterface()
    , AttColor()
    , AttExtSym()
    , AttPedalLog()
    , AttPedalVis()
    , AttPlacement()
    , AttVerticalGroup()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_EXTSYM);
    RegisterAttClass(ATT_PEDALLOG);
    RegisterAttClass(ATT_PEDALVIS);
    RegisterAttClass(ATT_PLACEMENT);
    RegisterAttClass(ATT_VERTICALGROUP);

    Reset();
}

Pedal::~Pedal() {}

void Pedal::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    ResetColor();
    ResetExtSym();
    ResetPedalLog();
    ResetPedalVis();
    ResetPlacement();
    ResetVerticalGroup();

    m_endsWithBounce = false;
}

wchar_t Pedal::GetPedalGlyph() const
{
    // If there is glyph.num, prioritize it, otherwise check other attributes
    if (HasGlyphNum()) {
        wchar_t code = GetGlyphNum();
        if (NULL != Resources::GetGlyph(code)) return code;
    }

    return (GetFunc() == "sostenuto") ? SMUFL_E659_keyboardPedalSost : SMUFL_E650_keyboardPedalPed;
}

//----------------------------------------------------------------------------
// Pedal functor methods
//----------------------------------------------------------------------------

int Pedal::GenerateMIDI(FunctorParams *functorParams)
{
    GenerateMIDIParams *params = vrv_params_cast<GenerateMIDIParams *>(functorParams);
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
        case pedalLog_DIR_bounce:
            params->m_midiFile->addSustainPedalOff(params->m_midiTrack, (starttime * tpq), params->m_midiChannel);
            params->m_midiFile->addSustainPedalOn(params->m_midiTrack, (starttime * tpq) + 0.1, params->m_midiChannel);
            break;
        default: return FUNCTOR_CONTINUE;
    }

    return FUNCTOR_CONTINUE;
}

int Pedal::PrepareFloatingGrps(FunctorParams *functorParams)
{
    PrepareFloatingGrpsParams *params = vrv_params_cast<PrepareFloatingGrpsParams *>(functorParams);
    assert(params);

    if (this->HasVgrp()) {
        this->SetDrawingGrpId(-this->GetVgrp());
    }

    if (!this->HasDir()) return FUNCTOR_CONTINUE;

    if (params->m_pedalLine && (this->GetDir() != pedalLog_DIR_down)) {
        params->m_pedalLine->SetEnd(this->GetStart());
        if (this->GetDir() == pedalLog_DIR_bounce) {
            params->m_pedalLine->EndsWithBounce(true);
        }
        params->m_pedalLine = NULL;
    }

    if ((this->GetDir() != pedalLog_DIR_up) && (this->GetForm() == pedalVis_FORM_line)) {
        params->m_pedalLine = this;
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
