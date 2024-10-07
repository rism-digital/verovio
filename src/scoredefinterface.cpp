/////////////////////////////////////////////////////////////////////////////
// Name:        scoredefinterface.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "scoredefinterface.h"

//----------------------------------------------------------------------------

#include <cassert>

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
    , AttBarring()
    , AttDurationDefault()
    , AttLyricStyle()
    , AttMeasureNumbers()
    , AttMidiTempo()
    , AttMmTempo()
    , AttMultinumMeasures()
    , AttOctaveDefault()
    , AttPianoPedals()
    , AttSpacing()
    , AttSystems()
{
    this->RegisterInterfaceAttClass(ATT_BARRING);
    this->RegisterInterfaceAttClass(ATT_DURATIONDEFAULT);
    this->RegisterInterfaceAttClass(ATT_LYRICSTYLE);
    this->RegisterInterfaceAttClass(ATT_MEASURENUMBERS);
    this->RegisterInterfaceAttClass(ATT_MIDITEMPO);
    this->RegisterInterfaceAttClass(ATT_MMTEMPO);
    this->RegisterInterfaceAttClass(ATT_MULTINUMMEASURES);
    this->RegisterInterfaceAttClass(ATT_OCTAVEDEFAULT);
    this->RegisterInterfaceAttClass(ATT_PIANOPEDALS);
    this->RegisterInterfaceAttClass(ATT_SPACING);
    this->RegisterInterfaceAttClass(ATT_SYSTEMS);

    this->Reset();
}

ScoreDefInterface::~ScoreDefInterface() {}

void ScoreDefInterface::Reset()
{
    this->ResetBarring();
    this->ResetDurationDefault();
    this->ResetLyricStyle();
    this->ResetMeasureNumbers();
    this->ResetMidiTempo();
    this->ResetMmTempo();
    this->ResetMultinumMeasures();
    this->ResetOctaveDefault();
    this->ResetPianoPedals();
    this->ResetSpacing();
    this->ResetSystems();
}

} // namespace vrv
