/////////////////////////////////////////////////////////////////////////////
// Name:        mrpt2.cpp
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "mrpt2.h"

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

namespace vrv {

//----------------------------------------------------------------------------
// MRpt2
//----------------------------------------------------------------------------

static const ClassRegistrar<MRpt2> s_factory("mRpt2", MRPT2);

MRpt2::MRpt2() : LayerElement(MRPT2), AttColor()
{
    this->RegisterAttClass(ATT_COLOR);

    this->Reset();
}

MRpt2::~MRpt2() {}

void MRpt2::Reset()
{
    LayerElement::Reset();
    this->ResetColor();
}

FunctorCode MRpt2::Accept(Functor &functor)
{
    return functor.VisitMRpt2(this);
}

FunctorCode MRpt2::Accept(ConstFunctor &functor) const
{
    return functor.VisitMRpt2(this);
}

FunctorCode MRpt2::AcceptEnd(Functor &functor)
{
    return functor.VisitMRpt2End(this);
}

FunctorCode MRpt2::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitMRpt2End(this);
}

} // namespace vrv
