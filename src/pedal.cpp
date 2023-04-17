/////////////////////////////////////////////////////////////////////////////
// Name:        pedal.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pedal.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "functor.h"
#include "functorparams.h"
#include "horizontalaligner.h"
#include "layerelement.h"
#include "smufl.h"
#include "system.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "MidiFile.h"

namespace vrv {

//----------------------------------------------------------------------------
// Pedal
//----------------------------------------------------------------------------

static const ClassRegistrar<Pedal> s_factory("pedal", PEDAL);

Pedal::Pedal()
    : ControlElement(PEDAL, "pedal-")
    , TimeSpanningInterface()
    , AttColor()
    , AttExtSym()
    , AttPedalLog()
    , AttPedalVis()
    , AttPlacementRelStaff()
    , AttVerticalGroup()
{
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_EXTSYM);
    this->RegisterAttClass(ATT_PEDALLOG);
    this->RegisterAttClass(ATT_PEDALVIS);
    this->RegisterAttClass(ATT_PLACEMENTRELSTAFF);
    this->RegisterAttClass(ATT_VERTICALGROUP);

    this->Reset();
}

Pedal::~Pedal() {}

void Pedal::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    this->ResetColor();
    this->ResetExtSym();
    this->ResetPedalLog();
    this->ResetPedalVis();
    this->ResetPlacementRelStaff();
    this->ResetVerticalGroup();

    m_endsWithBounce = false;
}

char32_t Pedal::GetPedalGlyph() const
{
    const Resources *resources = this->GetDocResources();
    if (!resources) return 0;

    // If there is glyph.num, prioritize it
    if (this->HasGlyphNum()) {
        char32_t code = this->GetGlyphNum();
        if (NULL != resources->GetGlyph(code)) return code;
    }
    // If there is glyph.name (second priority)
    else if (this->HasGlyphName()) {
        char32_t code = resources->GetGlyphCode(this->GetGlyphName());
        if (NULL != resources->GetGlyph(code)) return code;
    }

    return (this->GetFunc() == "sostenuto") ? SMUFL_E659_keyboardPedalSost : SMUFL_E650_keyboardPedalPed;
}

data_PEDALSTYLE Pedal::GetPedalForm(const Doc *doc, const System *system) const
{
    data_PEDALSTYLE style = static_cast<data_PEDALSTYLE>(doc->GetOptions()->m_pedalStyle.GetValue());
    if (style != PEDALSTYLE_NONE) {
        return style;
    }
    else if (this->HasForm()) {
        style = this->GetForm();
    }
    else if (const ScoreDef *scoreDef = system->GetDrawingScoreDef(); scoreDef && scoreDef->HasPedalStyle()) {
        style = scoreDef->GetPedalStyle();
    }

    return style;
}

//----------------------------------------------------------------------------
// Pedal functor methods
//----------------------------------------------------------------------------

FunctorCode Pedal::Accept(MutableFunctor &functor)
{
    return functor.VisitPedal(this);
}

FunctorCode Pedal::Accept(ConstFunctor &functor) const
{
    return functor.VisitPedal(this);
}

FunctorCode Pedal::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitPedalEnd(this);
}

FunctorCode Pedal::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitPedalEnd(this);
}

int Pedal::GenerateMIDI(FunctorParams *functorParams)
{
    GenerateMIDIParams *params = vrv_params_cast<GenerateMIDIParams *>(functorParams);
    assert(params);

    // Sameas not taken into account for now
    if (!this->HasDir()) return FUNCTOR_CONTINUE;

    double pedalTime = this->GetStart()->GetAlignment()->GetTime() * DURATION_4 / DUR_MAX;
    double starttime = params->m_totalTime + pedalTime;
    int tpq = params->m_midiFile->getTPQ();

    // todo: check pedal @func to switch between sustain/soften/damper pedals?
    switch (this->GetDir()) {
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

} // namespace vrv
