/////////////////////////////////////////////////////////////////////////////
// Name:        mrpt.cpp
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "mrpt.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <math.h>

//----------------------------------------------------------------------------

#include "chord.h"
#include "editorial.h"
#include "functor.h"
#include "functorparams.h"
#include "layer.h"
#include "note.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// MRpt
//----------------------------------------------------------------------------

static const ClassRegistrar<MRpt> s_factory("mRpt", MRPT);

MRpt::MRpt() : LayerElement(MRPT, "mrpt-"), AttColor(), AttNumbered(), AttNumberPlacement()
{
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_NUMBERED);
    this->RegisterAttClass(ATT_NUMBERPLACEMENT);

    this->Reset();
}

MRpt::~MRpt() {}

void MRpt::Reset()
{
    LayerElement::Reset();
    this->ResetColor();
    this->ResetNumbered();
    this->ResetNumberPlacement();

    m_drawingMeasureCount = 0;
}

//----------------------------------------------------------------------------
// MRpt functor methods
//----------------------------------------------------------------------------

FunctorCode MRpt::Accept(MutableFunctor &functor)
{
    return functor.VisitMRpt(this);
}

FunctorCode MRpt::Accept(ConstFunctor &functor) const
{
    return functor.VisitMRpt(this);
}

FunctorCode MRpt::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitMRptEnd(this);
}

FunctorCode MRpt::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitMRptEnd(this);
}

int MRpt::GenerateMIDI(FunctorParams *functorParams)
{
    // GenerateMIDIParams *params = vrv_params_cast<GenerateMIDIParams *>(functorParams);
    // assert(params);

    LogWarning("MRpt produces empty MIDI output");

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
