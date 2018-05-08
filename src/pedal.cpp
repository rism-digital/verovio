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
