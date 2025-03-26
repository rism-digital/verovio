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
#include "page.h"
#include "pages.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Pb
//----------------------------------------------------------------------------

static const ClassRegistrar<Pb> s_factory("pb", PB);

Pb::Pb() : SystemElement(PB), FacsimileInterface(), AttNNumberLike()
{
    this->RegisterAttClass(ATT_NNUMBERLIKE);
    this->RegisterInterface(FacsimileInterface::GetAttClasses(), FacsimileInterface::IsInterface());

    this->Reset();
}

Pb::~Pb() {}

void Pb::Reset()
{
    SystemElement::Reset();
    FacsimileInterface::Reset();
    this->ResetNNumberLike();
}

//----------------------------------------------------------------------------
// Pb functor methods
//----------------------------------------------------------------------------

FunctorCode Pb::Accept(Functor &functor)
{
    return functor.VisitPb(this);
}

FunctorCode Pb::Accept(ConstFunctor &functor) const
{
    return functor.VisitPb(this);
}

FunctorCode Pb::AcceptEnd(Functor &functor)
{
    return functor.VisitPbEnd(this);
}

FunctorCode Pb::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitPbEnd(this);
}

} // namespace vrv
