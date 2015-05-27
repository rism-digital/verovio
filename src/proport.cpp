/////////////////////////////////////////////////////////////////////////////
// Name:        proport.cpp
// Author:      Donald Byrd
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "proport.h"

namespace vrv {

//----------------------------------------------------------------------------
// Proport
//----------------------------------------------------------------------------

Proport::Proport():
	LayerElement("prop-"),
    AttDurationRatio()
{
    Reset();
}

    
/* ??DO I NEED THIS?
Proport::Proport( MeterSigAttr *meterSigAttr ):
    LayerElement("msig-")
{
    Proport();
    //this->SetCount(meterSigAttr->GetMeterCount());
    //this->SetSym(meterSigAttr->GetMeterSym());
    //this->SetUnit(meterSigAttr->GetMeterUnit());
}
 */


Proport::~Proport()
{
}
    
void Proport::Reset()
{
    LayerElement::Reset();
    ResetDurationRatio();
}

} // namespace vrv
