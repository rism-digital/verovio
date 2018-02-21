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

#include "dynam.h"
#include "functorparams.h"
#include "verticalaligner.h"

namespace vrv {

//----------------------------------------------------------------------------
// Hairpin
//----------------------------------------------------------------------------

Hairpin::Hairpin()
    : ControlElement("hairpin-")
    , TimeSpanningInterface()
    , AttColor()
    , AttHairpinLog()
    , AttPlacement()
    , AttVerticalAlignment()
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

    m_rightLink = NULL;
    m_leftLink = NULL;
}

//----------------------------------------------------------------------------
// Hairpin functor methods
//----------------------------------------------------------------------------

int Hairpin::PrepareFloatingGrps(FunctorParams *functorParams)
{
    PrepareFloatingGrpsParams *params = dynamic_cast<PrepareFloatingGrpsParams *>(functorParams);
    assert(params);

    if (this->HasVgrp()) {
        this->SetDrawingGrpId(-this->GetVgrp());
    }

    // Only try to link them if start and end are resolved
    if (!this->GetStart() || !this->GetEnd()) return FUNCTOR_CONTINUE;

    params->m_hairpins.push_back(this);

    for (auto &dynam : params->m_dynams) {
        if (dynam->GetStart() == this->GetStart() && (dynam->GetStaff() == this->GetStaff())) {
            this->m_leftLink = dynam;
        }
    }

    for (auto &hairpin : params->m_hairpins) {
        if (hairpin->GetEnd() == this->GetStart() && (hairpin->GetStaff() == this->GetStaff())) {
            if (!this->m_leftLink) this->m_leftLink = hairpin;
        }
        if (hairpin->GetEnd() == this->GetStart() && (hairpin->GetStaff() == this->GetStaff())) {
            if (!this->m_rightLink) this->m_rightLink = hairpin;
        }
    }

    return FUNCTOR_CONTINUE;
}

int Hairpin::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    ControlElement::ResetDrawing(functorParams);

    m_rightLink = NULL;
    m_leftLink = NULL;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
