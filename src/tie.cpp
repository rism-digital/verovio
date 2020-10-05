/////////////////////////////////////////////////////////////////////////////
// Name:        tie.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tie.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "note.h"
#include "slur.h"

namespace vrv {

//----------------------------------------------------------------------------
// Tie
//----------------------------------------------------------------------------

Tie::Tie() : ControlElement("tie-"), TimeSpanningInterface(), AttColor(), AttCurvature(), AttCurveRend()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_CURVATURE);
    RegisterAttClass(ATT_CURVEREND);

    Reset();
}

Tie::~Tie() {}

void Tie::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    ResetColor();
    ResetCurvature();
    ResetCurveRend();
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

int Tie::FindSpannedLayerElements(FunctorParams *functorParams)
{
    FindSpannedLayerElementsParams *params = vrv_params_cast<FindSpannedLayerElementsParams *>(functorParams);
    assert(params);

    FloatingPositioner *positioner = params->m_slur->GetCorrespFloatingPositioner(this);

    if (!positioner) {
        return FUNCTOR_CONTINUE;
    }

    if (positioner->HasContentBB() && (positioner->GetContentRight() > params->m_minPos)
        && (positioner->GetContentLeft() < params->m_maxPos)) {

        params->m_ties.push_back(positioner);
    }

    return FUNCTOR_CONTINUE;

    /*
    if (!this->Is(params->m_classIds)) {
        return FUNCTOR_CONTINUE;
    }

    if (this->HasContentBB() && (this->GetContentRight() > params->m_minPos)
        && (this->GetContentLeft() < params->m_maxPos)) {

        // We skip the start or end of the slur
        if ((this == params->m_interface->GetStart()) || (this == params->m_interface->GetEnd())) {
            return FUNCTOR_CONTINUE;
        }
        if (params->m_interface->GetStart()->HasChild(this) || this->HasChild(params->m_interface->GetStart())) {
            return FUNCTOR_CONTINUE;
        }
        if (params->m_interface->GetEnd()->HasChild(this) || this->HasChild(params->m_interface->GetEnd())) {
            return FUNCTOR_CONTINUE;
        }

        params->m_elements.push_back(this);
    }
    else if (this->GetDrawingX() > params->m_maxPos) {
        return FUNCTOR_STOP;
    }

    return FUNCTOR_CONTINUE;
    */
}

} // namespace vrv
