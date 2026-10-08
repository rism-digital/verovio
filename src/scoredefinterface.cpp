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
    , AttTextStyle()
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
    this->RegisterInterfaceAttClass(ATT_TEXTSTYLE);

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
    this->ResetTextStyle();
}

void ScoreDefInterface::MergeTextStyles(const ScoreDefInterface *other)
{
    assert(other);

    if (other->HasLyricFam()) this->SetLyricFam(other->GetLyricFam());
    if (other->HasLyricName()) this->SetLyricName(other->GetLyricName());
    if (other->HasLyricStyle()) this->SetLyricStyle(other->GetLyricStyle());
    if (other->HasLyricWeight()) this->SetLyricWeight(other->GetLyricWeight());
    if (other->HasTextFam()) this->SetTextFam(other->GetTextFam());
    if (other->HasTextName()) this->SetTextName(other->GetTextName());
    if (other->HasTextStyle()) this->SetTextStyle(other->GetTextStyle());
    if (other->HasTextWeight()) this->SetTextWeight(other->GetTextWeight());
}

} // namespace vrv
