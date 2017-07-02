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
    Init();
}

MeterSig::MeterSig(const ScoreDefInterface *meterSigAttr) : LayerElement("msig-"), AttMeterSigLog(), AttMeterSigVis()
{
    Init();

    this->SetCount(meterSigAttr->GetMeterCount());
    this->SetForm(meterSigDefaultVisToMeterSigVis(meterSigAttr->GetMeterRend()));
    this->SetSym(meterSigAttr->GetMeterSym());
    this->SetUnit(meterSigAttr->GetMeterUnit());
}

void MeterSig::Init()
{
    RegisterAttClass(ATT_METERSIGLOG);
    RegisterAttClass(ATT_METERSIGVIS);

    Reset();
}

MeterSig::~MeterSig()
{
}

void MeterSig::Reset()
{
    LayerElement::Reset();
    ResetMeterSigLog();
    ResetMeterSigVis();
}

meterSigVis_FORM MeterSig::meterSigDefaultVisToMeterSigVis(meterSigDefaultVis_METERREND rend)
{
    switch (rend) {
        case meterSigDefaultVis_METERREND_NONE: return meterSigVis_FORM_NONE;
        case meterSigDefaultVis_METERREND_num: return meterSigVis_FORM_num;
        case meterSigDefaultVis_METERREND_denomsym: return meterSigVis_FORM_denomsym;
        case meterSigDefaultVis_METERREND_norm: return meterSigVis_FORM_norm;
        case meterSigDefaultVis_METERREND_invis: return meterSigVis_FORM_invis;
        default: return meterSigVis_FORM_NONE;
    }
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int MeterSig::FindSpaceInReferenceAlignments(FunctorParams *functorParams)
{
    FindSpaceInAlignmentParams *params = dynamic_cast<FindSpaceInAlignmentParams *>(functorParams);
    assert(params);

    params->m_meterSig = this;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
