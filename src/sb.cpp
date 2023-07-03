/////////////////////////////////////////////////////////////////////////////
// Name:        sb.cpp
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "sb.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "doc.h"
#include "editorial.h"
#include "functor.h"
#include "measure.h"
#include "page.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Sb
//----------------------------------------------------------------------------

static const ClassRegistrar<Sb> s_factory("sb", SB);

Sb::Sb() : SystemElement(SB, "sb-"), AttNNumberLike()
{
    this->RegisterAttClass(ATT_NNUMBERLIKE);

    this->Reset();
}

Sb::~Sb() {}

void Sb::Reset()
{
    SystemElement::Reset();
    this->ResetNNumberLike();
}

//----------------------------------------------------------------------------
// Sb functor methods
//----------------------------------------------------------------------------

FunctorCode Sb::Accept(Functor &functor)
{
    return functor.VisitSb(this);
}

FunctorCode Sb::Accept(ConstFunctor &functor) const
{
    return functor.VisitSb(this);
}

FunctorCode Sb::AcceptEnd(Functor &functor)
{
    return functor.VisitSbEnd(this);
}

FunctorCode Sb::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitSbEnd(this);
}

} // namespace vrv
