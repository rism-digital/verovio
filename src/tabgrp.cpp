/////////////////////////////////////////////////////////////////////////////
// Name:        tabgrp.cpp
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tabgrp.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "functorparams.h"
#include "note.h"
#include "tabdursym.h"

namespace vrv {

//----------------------------------------------------------------------------
// TabGrp
//----------------------------------------------------------------------------

static const ClassRegistrar<TabGrp> s_factory("tabGrp", TABGRP);

TabGrp::TabGrp() : LayerElement(TABGRP, "tabgrp-"), DurationInterface()
{
    RegisterInterface(DurationInterface::GetAttClasses(), DurationInterface::IsInterface());

    Reset();
}

TabGrp::~TabGrp() {}

void TabGrp::Reset()
{
    LayerElement::Reset();
    DurationInterface::Reset();
}

bool TabGrp::IsSupportedChild(Object *child)
{
    if (child->Is(NOTE)) {
        assert(dynamic_cast<Note *>(child));
    }
    else if (child->Is(TABDURSYM)) {
        assert(dynamic_cast<TabDurSym *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

int TabGrp::CalcOnsetOffsetEnd(FunctorParams *functorParams)
{
    CalcOnsetOffsetParams *params = vrv_params_cast<CalcOnsetOffsetParams *>(functorParams);
    assert(params);

    LayerElement *element = this->ThisOrSameasAsLink();

    double incrementScoreTime = element->GetAlignmentDuration(
        params->m_currentMensur, params->m_currentMeterSig, true, params->m_notationType);
    incrementScoreTime = incrementScoreTime / (DUR_MAX / DURATION_4);
    double realTimeIncrementSeconds = incrementScoreTime * 60.0 / params->m_currentTempo;

    params->m_currentScoreTime += incrementScoreTime;
    params->m_currentRealTimeSeconds += realTimeIncrementSeconds;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
