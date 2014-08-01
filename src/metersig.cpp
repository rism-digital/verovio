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
	LayerElement("msig-"), AttMeterSigLog()
{
    Reset();
}


MeterSig::~MeterSig()
{
}
    
void MeterSig::Reset()
{
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

} // namespace vrv
