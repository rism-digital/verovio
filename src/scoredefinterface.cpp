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
    , AttCleffingVis()
    , AttKeySigDefaultAnl()
    , AttKeySigDefaultLog()
    , AttKeySigDefaultVis()
    , AttLyricStyle()
    , AttMeasureNumbers()
    , AttMensuralLog()
    , AttMensuralShared()
    , AttMeterSigDefaultLog()
    , AttMeterSigDefaultVis()
    , AttMidiTempo()
    , AttMultinumMeasures()
{
    RegisterInterfaceAttClass(ATT_CLEFFINGLOG);
    RegisterInterfaceAttClass(ATT_CLEFFINGVIS);
    RegisterInterfaceAttClass(ATT_KEYSIGDEFAULTANL);
    RegisterInterfaceAttClass(ATT_KEYSIGDEFAULTLOG);
    RegisterInterfaceAttClass(ATT_KEYSIGDEFAULTVIS);
    RegisterInterfaceAttClass(ATT_LYRICSTYLE);
    RegisterInterfaceAttClass(ATT_MEASURENUMBERS);
    RegisterInterfaceAttClass(ATT_MENSURALLOG);
    RegisterInterfaceAttClass(ATT_MENSURALSHARED);
    RegisterInterfaceAttClass(ATT_METERSIGDEFAULTLOG);
    RegisterInterfaceAttClass(ATT_METERSIGDEFAULTVIS);
    RegisterInterfaceAttClass(ATT_MIDITEMPO);
    RegisterInterfaceAttClass(ATT_MULTINUMMEASURES);

    Reset();
}

ScoreDefInterface::~ScoreDefInterface() {}

void ScoreDefInterface::Reset()
{
    ResetCleffingLog();
    ResetCleffingVis();
    ResetKeySigDefaultAnl();
    ResetKeySigDefaultLog();
    ResetKeySigDefaultVis();
    ResetLyricStyle();
    ResetMeasureNumbers();
    ResetMensuralLog();
    ResetMensuralShared();
    ResetMeterSigDefaultLog();
    ResetMeterSigDefaultVis();
    ResetMidiTempo();
    ResetMultinumMeasures();
}

} // namespace vrv
