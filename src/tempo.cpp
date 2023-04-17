/////////////////////////////////////////////////////////////////////////////
// Name:        tempo.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tempo.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "editorial.h"
#include "functor.h"
#include "functorparams.h"
#include "measure.h"
#include "staff.h"
#include "system.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Tempo
//----------------------------------------------------------------------------

static const ClassRegistrar<Tempo> s_factory("tempo", TEMPO);

Tempo::Tempo()
    : ControlElement(TEMPO, "tempo-")
    , TextDirInterface()
    , TimeSpanningInterface()
    , AttExtender()
    , AttLang()
    , AttMidiTempo()
    , AttMmTempo()
{
    this->RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_EXTENDER);
    this->RegisterAttClass(ATT_LANG);
    this->RegisterAttClass(ATT_MIDITEMPO);
    this->RegisterAttClass(ATT_MMTEMPO);

    this->Reset();
}

Tempo::~Tempo() {}

void Tempo::Reset()
{
    ControlElement::Reset();
    TextDirInterface::Reset();
    TimeSpanningInterface::Reset();
    this->ResetExtender();
    this->ResetLang();
    this->ResetMidiTempo();
    this->ResetMmTempo();
}

bool Tempo::IsSupportedChild(Object *child)
{
    if (child->Is({ LB, REND, SYMBOL, TEXT })) {
        assert(dynamic_cast<TextElement *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

int Tempo::GetDrawingXRelativeToStaff(int staffN) const
{
    int m_relativeX = 0;
    if (m_drawingXRels.find(staffN) != m_drawingXRels.end()) {
        m_relativeX = m_drawingXRels.at(staffN);
    }

    return this->GetStart()->GetDrawingX() + m_relativeX;
}

FunctorCode Tempo::Accept(MutableFunctor &functor)
{
    return functor.VisitTempo(this);
}

FunctorCode Tempo::Accept(ConstFunctor &functor) const
{
    return functor.VisitTempo(this);
}

FunctorCode Tempo::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitTempoEnd(this);
}

FunctorCode Tempo::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitTempoEnd(this);
}

int Tempo::InitMaxMeasureDuration(FunctorParams *functorParams)
{
    InitMaxMeasureDurationParams *params = vrv_params_cast<InitMaxMeasureDurationParams *>(functorParams);
    assert(params);

    if (this->HasMidiBpm()) {
        params->m_currentTempo = this->GetMidiBpm();
    }
    else if (this->HasMm()) {
        params->m_currentTempo = Tempo::CalcTempo(this);
    }

    return FUNCTOR_CONTINUE;
}

double Tempo::CalcTempo(const AttMmTempo *attMmTempo)
{
    double tempo = MIDI_TEMPO;

    double mm = attMmTempo->GetMm();
    double mmUnit = 4;

    if (attMmTempo->HasMmUnit() && (attMmTempo->GetMmUnit() > DURATION_breve)) {
        mmUnit = pow(2, (int)attMmTempo->GetMmUnit() - 2);
    }
    if (attMmTempo->HasMmDots()) {
        double dotsUnit = 0.0;
        for (int d = 0; d < attMmTempo->GetMmDots(); d++) {
            dotsUnit += mmUnit / 4.0 / pow(2, d);
        }
        mmUnit -= dotsUnit;
    }
    if (mmUnit > 0) tempo = mm * 4.0 / mmUnit;

    return tempo;
}

} // namespace vrv
