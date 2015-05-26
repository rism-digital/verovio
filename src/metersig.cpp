/////////////////////////////////////////////////////////////////////////////
// Name:        metersig.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "metersig.h"

namespace vrv {

//----------------------------------------------------------------------------
// MeterSig
//----------------------------------------------------------------------------

MeterSig::MeterSig():
	LayerElement("msig-"),
    AttMeterSigLog()
{
    Reset();
}
    
MeterSig::MeterSig( MeterSigAttr *meterSigAttr ):
    LayerElement("msig-"),
    AttMeterSigLog()
{
    Reset();
    this->SetCount(meterSigAttr->GetMeterCount());
    this->SetSym(meterSigAttr->GetMeterSym());
    this->SetUnit(meterSigAttr->GetMeterUnit());
}


MeterSig::~MeterSig()
{
}
    
void MeterSig::Reset()
{
    LayerElement::Reset();
    ResetMeterSigLog();
}

//----------------------------------------------------------------------------
// MeterSigAttr
//----------------------------------------------------------------------------

MeterSigAttr::MeterSigAttr():
    Object(),
    AttMeterSigDefaultLog(),
    AttMeterSigDefaultVis()
{
    Reset();
}


MeterSigAttr::~MeterSigAttr()
{
}

void MeterSigAttr::Reset()
{
    Object::Reset();
    ResetMeterSigDefaultLog();
    ResetMeterSigDefaultVis();
}

} // namespace vrv
