/////////////////////////////////////////////////////////////////////////////
// Name:        hairpin.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "hairpin.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "verticalaligner.h"

namespace vrv {

//----------------------------------------------------------------------------
// Hairpin
//----------------------------------------------------------------------------

Hairpin::Hairpin() : ControlElement("hairpin-"), TimeSpanningInterface(), AttColor(), AttHairpinLog(), AttPlacement(), AttVerticalAlignment()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_HAIRPINLOG);
    RegisterAttClass(ATT_PLACEMENT);
    RegisterAttClass(ATT_VERTICALALIGNMENT);

    Reset();
}

Hairpin::~Hairpin() {}

void Hairpin::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    ResetColor();
    ResetHairpinLog();
    ResetPlacement();
    AttVerticalAlignment::ResetVerticalAlignment();
}

//----------------------------------------------------------------------------
// Hairpin functor methods
//----------------------------------------------------------------------------

int Hairpin::PrepareFloatingGrps(FunctorParams *functorParams)
{
    // PrepareFloatingGrpsParams *params = dynamic_cast<PrepareFloatingGrpsParams *>(functorParams);
    // assert(params);

    if (this->HasVgrp()) {
        this->SetDrawingGrpId(-this->GetVgrp());
    }

    return FUNCTOR_CONTINUE;
}
    
} // namespace vrv
