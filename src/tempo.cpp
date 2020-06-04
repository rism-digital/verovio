/////////////////////////////////////////////////////////////////////////////
// Name:        tempo.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tempo.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "controlelement.h"
#include "editorial.h"
#include "functorparams.h"
#include "horizontalaligner.h"
#include "layerelement.h"
#include "text.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "MidiFile.h"

namespace vrv {

//----------------------------------------------------------------------------
// Tempo
//----------------------------------------------------------------------------

Tempo::Tempo()
    : ControlElement("tempo-"), TextDirInterface(), TimePointInterface(), AttLang(), AttMidiTempo(), AttMmTempo()
{
    RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    RegisterAttClass(ATT_LANG);
    RegisterAttClass(ATT_MIDITEMPO);
    RegisterAttClass(ATT_MMTEMPO);

    Reset();
}

Tempo::~Tempo() {}

void Tempo::Reset()
{
    ControlElement::Reset();
    TextDirInterface::Reset();
    TimePointInterface::Reset();
    ResetLang();
    ResetMidiTempo();
    ResetMmTempo();
}

void Tempo::AddChild(Object *child)
{
    if (child->Is({ REND, TEXT })) {
        assert(dynamic_cast<TextElement *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

//----------------------------------------------------------------------------
// Tempo functor methods
//----------------------------------------------------------------------------

int Tempo::GenerateMIDI(FunctorParams *functorParams)
{
    GenerateMIDIParams *params = dynamic_cast<GenerateMIDIParams *>(functorParams);
    assert(params);
    
    double tempoTime = GetStart()->GetAlignment()->GetTime() * DURATION_4 / DUR_MAX;
    double starttime = params->m_totalTime + tempoTime;
    int tpq = params->m_midiFile->getTPQ();
    
    if (this->HasMidiBpm()) {
        params->m_midiFile->addTempo(0, (starttime * tpq), this->GetMidiBpm());
    }
    else if (this->HasMm()) {
        int mm = this->GetMm();
        int mmUnit = 4;
        if (this->HasMmUnit() && (this->GetMmUnit() > DURATION_breve)) {
            mmUnit = pow(2, (int)this->GetMmUnit() - 2);
        }
        if (this->HasMmDots()) {
            mmUnit = 2 * mmUnit - (mmUnit / pow(2, this->GetMmDots()));
        }
        params->m_midiFile->addTempo(0, (starttime * tpq), int(mm * 4.0 / mmUnit + 0.5));
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
