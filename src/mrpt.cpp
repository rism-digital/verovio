/////////////////////////////////////////////////////////////////////////////
// Name:        mrpt.cpp
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "mrpt.h"

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
// MRpt
//----------------------------------------------------------------------------

MRpt::MRpt() : LayerElement("mrpt-"), AttColor()
{
    RegisterAttClass(ATT_COLOR);

    Reset();
}

MRpt::~MRpt() {}

void MRpt::Reset()
{
    LayerElement::Reset();
    ResetColor();

    m_drawingMeasureCount = 0;
}

//----------------------------------------------------------------------------
// MRpt functor methods
//----------------------------------------------------------------------------

int MRpt::GenerateMIDI(FunctorParams *functorParams)
{
    // GenerateMIDIParams *params = vrv_params_cast<GenerateMIDIParams *>(functorParams);
    // assert(params);

    LogWarning("MRpt produces empty MIDI output");

    return FUNCTOR_CONTINUE;
}

int MRpt::PrepareRpt(FunctorParams *functorParams)
{
    PrepareRptParams *params = vrv_params_cast<PrepareRptParams *>(functorParams);
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
