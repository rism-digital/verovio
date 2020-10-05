/////////////////////////////////////////////////////////////////////////////
// Name:        mensur.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "mensur.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "scoredefinterface.h"
#include "vrv.h"

namespace vrv {

int Mensur::s_num = 3;
int Mensur::s_numBase = 2;

//----------------------------------------------------------------------------
// Mensur
//----------------------------------------------------------------------------

Mensur::Mensur()
    : LayerElement("mensur-")
    , AttColor()
    , AttCue()
    , AttDurationRatio()
    , AttMensuralShared()
    , AttMensurVis()
    , AttSlashCount()
    , AttStaffLoc()
{
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_CUE);
    RegisterAttClass(ATT_DURATIONRATIO);
    RegisterAttClass(ATT_MENSURALSHARED);
    RegisterAttClass(ATT_MENSURVIS);
    RegisterAttClass(ATT_SLASHCOUNT);
    RegisterAttClass(ATT_STAFFLOC);

    Reset();
}

Mensur::~Mensur() {}

void Mensur::Reset()
{
    LayerElement::Reset();
    ResetColor();
    ResetCue();
    ResetDurationRatio();
    ResetMensuralShared();
    ResetMensurVis();
    ResetSlashCount();
    ResetStaffLoc();
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Mensur::LayerCountInTimeSpan(FunctorParams *functorParams)
{
    LayerCountInTimeSpanParams *params = vrv_params_cast<LayerCountInTimeSpanParams *>(functorParams);
    assert(params);

    params->m_mensur = this;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
