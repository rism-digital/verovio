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
    LayerElement("msig-")
{
    MeterSig();
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

bool MeterSig::operator==( Object& other )
{
    MeterSig *otherMeterSig = dynamic_cast<MeterSig*>( &other );
    if ( !otherMeterSig ) {
        return false;
    }
    if ( this->GetCount() != otherMeterSig->GetCount() ) {
        return false;
    }
    if ( this->GetSym() != otherMeterSig->GetSym() ) {
        return false;
    }
    if ( this->GetUnit() != otherMeterSig->GetUnit() ) {
        return false;
    }
    return true;
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

bool MeterSigAttr::operator==( Object& other )
{
    MeterSigAttr *otherMeterSigAttr = dynamic_cast<MeterSigAttr*>( &other );
    if ( !otherMeterSigAttr ) {
        return false;
    }
    // we need member comparison here...
    return true;
}


} // namespace vrv
