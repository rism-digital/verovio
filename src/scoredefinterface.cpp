/////////////////////////////////////////////////////////////////////////////
// Name:        scoredefinterface.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "scoredefinterface.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "layerelement.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {


//----------------------------------------------------------------------------
// ScoreDefInterface
//----------------------------------------------------------------------------

ScoreDefInterface::ScoreDefInterface(): Interface(),
    AttCleffingLog(),
    AttKeySigDefaultLog(),
    AttKeySigDefaultVis(),
    AttMensuralLog(),
    AttMensuralShared(),
    AttMeterSigDefaultLog(),
    AttMeterSigDefaultVis(),
    AttMultinummeasures()
{
    RegisterInterfaceAttClass(ATT_CLEFFINGLOG);
    RegisterInterfaceAttClass(ATT_KEYSIGDEFAULTLOG);
    RegisterInterfaceAttClass(ATT_KEYSIGDEFAULTVIS);
    RegisterInterfaceAttClass(ATT_MENSURALLOG);
    RegisterInterfaceAttClass(ATT_MENSURALSHARED);
    RegisterInterfaceAttClass(ATT_METERSIGDEFAULTLOG);
    RegisterInterfaceAttClass(ATT_METERSIGDEFAULTVIS);
    RegisterInterfaceAttClass(ATT_MULTINUMMEASURES);
    Reset();
}

ScoreDefInterface::~ScoreDefInterface()
{
}    
    
void ScoreDefInterface::Reset()
{
    ResetCleffingLog();
    ResetKeySigDefaultLog();
    ResetKeySigDefaultVis();
    ResetMensuralLog();
    ResetMensuralShared();
    ResetMeterSigDefaultLog();
    ResetMeterSigDefaultVis();
    ResetMultinummeasures();
}
    
} // namespace vrv
