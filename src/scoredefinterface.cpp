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
    , AttLyricStyle()
    , AttMeasureNumbers()
    , AttMensuralLog()
    , AttMensuralShared()
    , AttMensuralVis()
    , AttMeterSigDefaultLog()
    , AttMeterSigDefaultVis()
    , AttMidiTempo()
    , AttMultinumMeasures()
{
    RegisterInterfaceAttClass(ATT_LYRICSTYLE);
    RegisterInterfaceAttClass(ATT_MEASURENUMBERS);
    RegisterInterfaceAttClass(ATT_MENSURALLOG);
    RegisterInterfaceAttClass(ATT_MENSURALSHARED);
    RegisterInterfaceAttClass(ATT_MENSURALVIS);
    RegisterInterfaceAttClass(ATT_METERSIGDEFAULTLOG);
    RegisterInterfaceAttClass(ATT_METERSIGDEFAULTVIS);
    RegisterInterfaceAttClass(ATT_MIDITEMPO);
    RegisterInterfaceAttClass(ATT_MULTINUMMEASURES);

    Reset();
}

ScoreDefInterface::~ScoreDefInterface() {}

void ScoreDefInterface::Reset()
{
    ResetLyricStyle();
    ResetMeasureNumbers();
    ResetMensuralLog();
    ResetMensuralShared();
    ResetMensuralVis();
    ResetMeterSigDefaultLog();
    ResetMeterSigDefaultVis();
    ResetMidiTempo();
    ResetMultinumMeasures();
}

} // namespace vrv
