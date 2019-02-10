/////////////////////////////////////////////////////////////////////////////
// Name:        tie.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tie.h"
#include "note.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Tie
//----------------------------------------------------------------------------

Tie::Tie() : ControlElement("tie-"), TimeSpanningInterface(), AttColor(), AttCurvature()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_CURVATURE);

    Reset();
}

Tie::~Tie() {}

void Tie::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    ResetColor();
    ResetCurvature();
}

//----------------------------------------------------------------------------
// Tie functor methods
//----------------------------------------------------------------------------

int Tie::ResolveMIDITies(FunctorParams *)
{
    Note *note1 = dynamic_cast<Note *>(this->GetStart());
    Note *note2 = dynamic_cast<Note *>(this->GetEnd());

    if (!note1 || !note2) {
        return FUNCTOR_CONTINUE;
    }

    double sttd2 = note2->GetScoreTimeTiedDuration();
    double std2 = note2->GetScoreTimeDuration();

    if (sttd2 > 0.0) {
        note1->SetScoreTimeTiedDuration(sttd2 + std2);
    }
    else {
        note1->SetScoreTimeTiedDuration(std2);
    }
    note2->SetScoreTimeTiedDuration(-1.0);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
