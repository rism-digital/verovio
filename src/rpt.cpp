/////////////////////////////////////////////////////////////////////////////
// Name:        rpt.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "rpt.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "chord.h"
#include "editorial.h"
#include "functorparams.h"
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

BeatRpt::~BeatRpt()
{
}

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

//----------------------------------------------------------------------------
// MRpt
//----------------------------------------------------------------------------

MRpt::MRpt() : LayerElement("mrpt-")
{
    Reset();
}

MRpt::~MRpt()
{
}

void MRpt::Reset()
{
    LayerElement::Reset();

    m_drawingMeasureCount = 0;
}

//----------------------------------------------------------------------------
// MRpt2
//----------------------------------------------------------------------------

MRpt2::MRpt2() : LayerElement("mrpt2-")
{
    Reset();
}

MRpt2::~MRpt2()
{
}

void MRpt2::Reset()
{
    LayerElement::Reset();
}

//----------------------------------------------------------------------------
// MultiRpt
//----------------------------------------------------------------------------

MultiRpt::MultiRpt() : LayerElement("multirpt-"), AttNumbered()
{
    RegisterAttClass(ATT_NUMBERED);
    Reset();
}

MultiRpt::~MultiRpt()
{
}

void MultiRpt::Reset()
{
    LayerElement::Reset();
    ResetNumbered();
}

//----------------------------------------------------------------------------
// MRpt functor methods
//----------------------------------------------------------------------------

int MRpt::PrepareRpt(FunctorParams *functorParams)
{
    PrepareRptParams *params = dynamic_cast<PrepareRptParams *>(functorParams);
    assert(params);

    // If multiNumber is not true, nothing needs to be done
    if (params->m_multiNumber != BOOLEAN_true) {
        return FUNCTOR_CONTINUE;
    }

    // If this is the first one, number has to be 2
    if (params->m_currentMRpt == NULL) {
        this->m_drawingMeasureCount = 2;
    }
    // Otherwise increment it
    else {
        this->m_drawingMeasureCount = params->m_currentMRpt->m_drawingMeasureCount + 1;
    }
    params->m_currentMRpt = this;
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
