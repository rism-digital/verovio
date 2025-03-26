/////////////////////////////////////////////////////////////////////////////
// Name:        multirpt.cpp
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "multirpt.h"

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
// MultiRpt
//----------------------------------------------------------------------------

static const ClassRegistrar<MultiRpt> s_factory("multiRpt", MULTIRPT);

MultiRpt::MultiRpt() : LayerElement(MULTIRPT), AttNumbered()
{
    this->RegisterAttClass(ATT_NUMBERED);
    this->Reset();
}

MultiRpt::~MultiRpt() {}

void MultiRpt::Reset()
{
    LayerElement::Reset();
    this->ResetNumbered();
}

FunctorCode MultiRpt::Accept(Functor &functor)
{
    return functor.VisitMultiRpt(this);
}

FunctorCode MultiRpt::Accept(ConstFunctor &functor) const
{
    return functor.VisitMultiRpt(this);
}

FunctorCode MultiRpt::AcceptEnd(Functor &functor)
{
    return functor.VisitMultiRptEnd(this);
}

FunctorCode MultiRpt::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitMultiRptEnd(this);
}

} // namespace vrv
