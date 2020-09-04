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

static const ClassRegistrar<Pedal> s_factory("pedal", PEDAL);

Pedal::Pedal()
    : ControlElement("pedal-")
    , TimeSpanningInterface()
    , AttColor()
    , AttExtSym()
    , AttPedalLog()
    , AttPedalVis()
    , AttPlacementRelStaff()
    , AttVerticalGroup()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_EXTSYM);
    RegisterAttClass(ATT_PEDALLOG);
    RegisterAttClass(ATT_PEDALVIS);
    RegisterAttClass(ATT_PLACEMENTRELSTAFF);
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
    ResetPlacementRelStaff();
    ResetVerticalGroup();

    m_endsWithBounce = false;
}

wchar_t Pedal::GetPedalGlyph() const
{
    // If there is glyph.num, prioritize it
    if (HasGlyphNum()) {
        wchar_t code = GetGlyphNum();
        if (NULL != Resources::GetGlyph(code)) return code;
    }
    // If there is glyph.name (second priority)
    else if (HasGlyphName()) {
        wchar_t code = Resources::GetGlyphCode(GetGlyphName());
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

    if (this->GetDir() != pedalLog_DIR_down) {
        auto pairedWithThis =
            std::find_if(params->m_pedalLines.begin(), params->m_pedalLines.end(),[this](const Pedal *p) -> bool {
                return p->GetStaff() == GetStaff();
            });

        if (pairedWithThis != params->m_pedalLines.end()) {
            (*pairedWithThis)->SetEnd(GetStart());

            SetEnd(GetStart());
            SetStart((*pairedWithThis)->GetStart());
            if (this->GetDir() == pedalLog_DIR_bounce) {
                (*pairedWithThis)->EndsWithBounce(true);
            }
            params->m_pedalLines.erase(pairedWithThis);
        } else {
            LogMessage("Pedal %s doesn't have start pair", GetUuid().c_str());
        }
    }

    if ((this->GetDir() != pedalLog_DIR_up) && (this->GetForm() == pedalVis_FORM_line)) {
        params->m_pedalLines.push_back(this);
    }

    return FUNCTOR_CONTINUE;
}

int Pedal::ResolveSpanningPedals(FunctorParams *functorParams) 
{
    if (!this->HasDir() || (this->GetDir() != pedalLog_DIR_up)) return FUNCTOR_SIBLINGS;

    LayerElement *startElement = GetStart();
    LayerElement *endElement = GetEnd();
    if (!startElement || !endElement) return FUNCTOR_SIBLINGS;

    Object *startSystem = startElement->GetFirstAncestor(SYSTEM);
    Object *endSystem = endElement->GetFirstAncestor(SYSTEM);
    assert(startSystem && endSystem);

    const int startSystemIndex = startSystem->GetIdx();
    const int endSystemIndex = endSystem->GetIdx();
    // make sure that start and end are not in the neighboring systems or same system
    if ((startSystem == endSystem) || (startSystemIndex == endSystemIndex - 1)) return FUNCTOR_SIBLINGS;

    Object *pedalPage = startSystem->GetFirstAncestor(PAGE);
    if (!pedalPage) return FUNCTOR_SIBLINGS;

    // add current pedal as child to all systems in between starting and ending system
    for (int i = startSystemIndex + 1; i < endSystemIndex; ++i) {
        Object *system = pedalPage->GetChild(i);
        if (!system->Is(SYSTEM)) continue;

        Object *systemMeasure = system->GetFirst(MEASURE);
        if (!systemMeasure) continue;

        Object *pedal = Clone();

        systemMeasure->AddChild(pedal);
    }

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
