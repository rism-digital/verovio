/////////////////////////////////////////////////////////////////////////////
// Name:        fermata.cpp
// Author:      Klaus Rettinghaus
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "fermata.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "verticalaligner.h"

namespace vrv {

//----------------------------------------------------------------------------
// Fermata
//----------------------------------------------------------------------------

Fermata::Fermata() : ControlElement("fermata-"), TimePointInterface(), AttColor(), AttFermataVis(), AttPlacement()
{
    RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_FERMATAVIS);
    RegisterAttClass(ATT_PLACEMENT);

    Reset();
}

Fermata::~Fermata() {}

void Fermata::Reset()
{
    ControlElement::Reset();
    TimePointInterface::Reset();
    ResetColor();
    ResetFermataVis();
    ResetPlacement();
}

void Fermata::ConvertFromAnalyticalMarkup(
    AttFermataPresent *fermataPresent, const std::string &uuid, ConvertAnalyticalMarkupParams *params)
{
    this->SetPlace(Att::StaffrelBasicToStaffrel(fermataPresent->GetFermata()));
    if (params->m_permanent) {
        fermataPresent->ResetFermataPresent();
    }
    else {
        this->IsAttribute(true);
    }
    this->SetStartid("#" + uuid);
    params->m_controlEvents.push_back(this);
}

//----------------------------------------------------------------------------
// Fermata functor methods
//----------------------------------------------------------------------------

} // namespace vrv
