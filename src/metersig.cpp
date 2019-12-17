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

meterSigVis_FORM MeterSig::meterSigDefaultVisToMeterSigVis(meterSigDefaultVis_METERFORM form)
{
    switch (form) {
        case meterSigDefaultVis_METERFORM_NONE: return meterSigVis_FORM_NONE;
        case meterSigDefaultVis_METERFORM_num: return meterSigVis_FORM_num;
        case meterSigDefaultVis_METERFORM_denomsym: return meterSigVis_FORM_denomsym;
        case meterSigDefaultVis_METERFORM_norm: return meterSigVis_FORM_norm;
        case meterSigDefaultVis_METERFORM_invis: return meterSigVis_FORM_invis;
        default: return meterSigVis_FORM_NONE;
    }
}

meterSigDefaultVis_METERFORM MeterSig::meterSigVisToMeterSigDefaultVis(meterSigVis_FORM form)
{
    switch (form) {
        case meterSigVis_FORM_NONE: return meterSigDefaultVis_METERFORM_NONE;
        case meterSigVis_FORM_num: return meterSigDefaultVis_METERFORM_num;
        case meterSigVis_FORM_denomsym: return meterSigDefaultVis_METERFORM_denomsym;
        case meterSigVis_FORM_norm: return meterSigDefaultVis_METERFORM_norm;
        case meterSigVis_FORM_invis: return meterSigDefaultVis_METERFORM_invis;
        default: return meterSigDefaultVis_METERFORM_NONE;
    }
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int MeterSig::LayerCountInTimeSpan(FunctorParams *functorParams)
{
    LayerCountInTimeSpanParams *params = dynamic_cast<LayerCountInTimeSpanParams *>(functorParams);
    assert(params);

    params->m_meterSig = this;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
