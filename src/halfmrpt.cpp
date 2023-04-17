/////////////////////////////////////////////////////////////////////////////
// Name:        halfmrpt.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "halfmrpt.h"

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
// HalfmRpt
//----------------------------------------------------------------------------

static const ClassRegistrar<HalfmRpt> s_factory("halfmRpt", HALFMRPT);

HalfmRpt::HalfmRpt() : LayerElement(HALFMRPT, "mrpt-")
{
    this->RegisterAttClass(ATT_COLOR);

    this->Reset();
}

HalfmRpt::~HalfmRpt() {}

void HalfmRpt::Reset()
{
    LayerElement::Reset();
    this->ResetColor();
}

//----------------------------------------------------------------------------
// HalfmRpt functor methods
//----------------------------------------------------------------------------

FunctorCode HalfmRpt::Accept(MutableFunctor &functor)
{
    return functor.VisitHalfmRpt(this);
}

FunctorCode HalfmRpt::Accept(ConstFunctor &functor) const
{
    return functor.VisitHalfmRpt(this);
}

FunctorCode HalfmRpt::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitHalfmRptEnd(this);
}

FunctorCode HalfmRpt::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitHalfmRptEnd(this);
}

int HalfmRpt::GenerateMIDI(FunctorParams *functorParams)
{
    // GenerateMIDIParams *params = vrv_params_cast<GenerateMIDIParams *>(functorParams);
    // assert(params);

    LogWarning("HalfmRpt produces empty MIDI output");

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
