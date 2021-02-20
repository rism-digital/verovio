/////////////////////////////////////////////////////////////////////////////
// Name:        metersig.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "metersig.h"
//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "scoredefinterface.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// MeterSig
//----------------------------------------------------------------------------

MeterSig::MeterSig() : LayerElement("msig-"), AttMeterSigLog(), AttMeterSigVis()
{
    RegisterAttClass(ATT_METERSIGLOG);
    RegisterAttClass(ATT_METERSIGVIS);

    Reset();
}

MeterSig::~MeterSig() {}

void MeterSig::Reset()
{
    LayerElement::Reset();
    ResetMeterSigLog();
    ResetMeterSigVis();
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int MeterSig::LayerCountInTimeSpan(FunctorParams *functorParams)
{
    LayerCountInTimeSpanParams *params = vrv_params_cast<LayerCountInTimeSpanParams *>(functorParams);
    assert(params);

    params->m_meterSig = this;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
