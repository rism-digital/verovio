/////////////////////////////////////////////////////////////////////////////
// Name:        beatrpt.cpp
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "beatrpt.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <math.h>

//----------------------------------------------------------------------------

#include "chord.h"
#include "editorial.h"
#include "functor.h"
#include "layer.h"
#include "note.h"
#include "staff.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "MidiFile.h"

namespace vrv {

//----------------------------------------------------------------------------
// BeatRpt
//----------------------------------------------------------------------------

static const ClassRegistrar<BeatRpt> s_factory("beatRpt", BEATRPT);

BeatRpt::BeatRpt() : LayerElement(BEATRPT, "beatrpt-"), AttColor(), AttBeatRptLog(), AttBeatRptVis()
{
    this->RegisterAttClass(ATT_BEATRPTLOG);
    this->RegisterAttClass(ATT_BEATRPTVIS);
    this->RegisterAttClass(ATT_COLOR);
    this->Reset();
}

BeatRpt::~BeatRpt() {}

void BeatRpt::Reset()
{
    LayerElement::Reset();
    this->ResetBeatRptLog();
    this->ResetBeatRptVis();
    this->ResetColor();

    m_scoreTimeOnset = 0.0;
}

double BeatRpt::GetBeatRptAlignmentDuration(int meterUnit) const
{
    double dur = DUR_MAX / meterUnit;
    if (this->HasBeatdef()) dur *= this->GetBeatdef();
    return dur;
}

void BeatRpt::SetScoreTimeOnset(double scoreTime)
{
    m_scoreTimeOnset = scoreTime;
}

double BeatRpt::GetScoreTimeOnset() const
{
    return m_scoreTimeOnset;
}

//----------------------------------------------------------------------------
// BeatRpt functor methods
//----------------------------------------------------------------------------

FunctorCode BeatRpt::Accept(Functor &functor)
{
    return functor.VisitBeatRpt(this);
}

FunctorCode BeatRpt::Accept(ConstFunctor &functor) const
{
    return functor.VisitBeatRpt(this);
}

FunctorCode BeatRpt::AcceptEnd(Functor &functor)
{
    return functor.VisitBeatRptEnd(this);
}

FunctorCode BeatRpt::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitBeatRptEnd(this);
}

} // namespace vrv
