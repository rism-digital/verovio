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

BeatRpt::BeatRpt() : LayerElement(BEATRPT), AttColor(), AttBeatRptLog(), AttBeatRptVis()
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

    m_scoreTimeOnset = 0;
}

Fraction BeatRpt::GetBeatRptAlignmentDuration(data_DURATION meterUnit) const
{
    Fraction duration(meterUnit);
    if (this->HasBeatdef()) duration = duration * Fraction(this->GetBeatdef() * DUR_MAX, DUR_MAX);
    return duration;
}

void BeatRpt::SetScoreTimeOnset(Fraction scoreTime)
{
    m_scoreTimeOnset = scoreTime;
}

Fraction BeatRpt::GetScoreTimeOnset() const
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
