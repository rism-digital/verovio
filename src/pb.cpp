/////////////////////////////////////////////////////////////////////////////
// Name:        pb.cpp
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pb.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "doc.h"
#include "editorial.h"
#include "functor.h"
#include "functorparams.h"
#include "page.h"
#include "pages.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Pb
//----------------------------------------------------------------------------

static const ClassRegistrar<Pb> s_factory("pb", PB);

Pb::Pb() : SystemElement(PB, "pb-"), AttNNumberLike()
{
    this->RegisterAttClass(ATT_NNUMBERLIKE);

    this->Reset();
}

Pb::~Pb() {}

void Pb::Reset()
{
    SystemElement::Reset();
    this->ResetNNumberLike();
}

//----------------------------------------------------------------------------
// Pb functor methods
//----------------------------------------------------------------------------

FunctorCode Pb::Accept(MutableFunctor &functor)
{
    return functor.VisitPb(this);
}

FunctorCode Pb::Accept(ConstFunctor &functor) const
{
    return functor.VisitPb(this);
}

FunctorCode Pb::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitPbEnd(this);
}

FunctorCode Pb::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitPbEnd(this);
}

} // namespace vrv
