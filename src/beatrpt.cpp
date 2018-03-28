/////////////////////////////////////////////////////////////////////////////
// Name:        beatrpt.cpp
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "beatrpt.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "chord.h"
#include "editorial.h"
#include "layer.h"
#include "note.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// BeatRpt
//----------------------------------------------------------------------------

BeatRpt::BeatRpt() : LayerElement("beatrpt-"), AttColor(), AttBeatRptVis()
{
    RegisterAttClass(ATT_BEATRPTVIS);
    RegisterAttClass(ATT_COLOR);
    Reset();
}

BeatRpt::~BeatRpt() {}

void BeatRpt::Reset()
{
    LayerElement::Reset();
    ResetBeatRptVis();
    ResetColor();
}

double BeatRpt::GetBeatRptAlignmentDuration(int meterUnit) const
{
    return DUR_MAX / meterUnit;
}

} // namespace vrv
