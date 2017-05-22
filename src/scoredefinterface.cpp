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

ScoreDefInterface::ScoreDefInterface()
    : Interface()
    , AttCleffingLog()
    , AttKeySigDefaultAnl()
    , AttKeySigDefaultLog()
    , AttKeySigDefaultVis()
    , AttLyricstyle()
    , AttMensuralLog()
    , AttMensuralShared()
    , AttMeterSigDefaultLog()
    , AttMeterSigDefaultVis()
    , AttMiditempo()
    , AttMultinummeasures()
{
    RegisterInterfaceAttClass(ATT_CLEFFINGLOG);
    RegisterInterfaceAttClass(ATT_KEYSIGDEFAULTANL);
    RegisterInterfaceAttClass(ATT_KEYSIGDEFAULTLOG);
    RegisterInterfaceAttClass(ATT_KEYSIGDEFAULTVIS);
    RegisterInterfaceAttClass(ATT_LYRICSTYLE);
    RegisterInterfaceAttClass(ATT_MENSURALLOG);
    RegisterInterfaceAttClass(ATT_MENSURALSHARED);
    RegisterInterfaceAttClass(ATT_METERSIGDEFAULTLOG);
    RegisterInterfaceAttClass(ATT_METERSIGDEFAULTVIS);
    RegisterInterfaceAttClass(ATT_MIDITEMPO);
    RegisterInterfaceAttClass(ATT_MULTINUMMEASURES);

    Reset();
}

ScoreDefInterface::~ScoreDefInterface()
{
}

void ScoreDefInterface::Reset()
{
    ResetCleffingLog();
    ResetKeySigDefaultAnl();
    ResetKeySigDefaultLog();
    ResetKeySigDefaultVis();
    ResetLyricstyle();
    ResetMensuralLog();
    ResetMensuralShared();
    ResetMeterSigDefaultLog();
    ResetMeterSigDefaultVis();
    ResetMiditempo();
    ResetMultinummeasures();
}

} // namespace vrv
