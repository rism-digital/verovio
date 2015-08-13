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
    Init();
}
    
MeterSig::MeterSig( MeterSigAttr *meterSigAttr ):
    LayerElement("msig-"),
    AttMeterSigLog()
{
    Init();
    
    this->SetCount(meterSigAttr->GetMeterCount());
    this->SetSym(meterSigAttr->GetMeterSym());
    this->SetUnit(meterSigAttr->GetMeterUnit());
}
    
void MeterSig::Init()
{
    RegisterAttClass(ATT_METERSIGLOG);
    Reset();
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
    RegisterAttClass(ATT_METERSIGDEFAULTLOG);
    RegisterAttClass(ATT_METERSIGDEFAULTVIS);
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
