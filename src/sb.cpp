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
#include "functorparams.h"
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

FunctorCode Sb::Accept(MutableFunctor &functor)
{
    return functor.VisitSb(this);
}

FunctorCode Sb::Accept(ConstFunctor &functor) const
{
    return functor.VisitSb(this);
}

FunctorCode Sb::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitSbEnd(this);
}

FunctorCode Sb::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitSbEnd(this);
}

int Sb::CastOffEncoding(FunctorParams *functorParams)
{
    CastOffEncodingParams *params = vrv_params_cast<CastOffEncodingParams *>(functorParams);
    assert(params);

    // We look if the current system has at least one measure - if yes, we assume that the <sb>
    // is not the one at the beginning of the content (<mdiv>). This is not very robust but at least make it
    // work when rendering a <mdiv> that does not start with a <pb> or a <sb> (which we cannot enforce)
    if (params->m_currentSystem->GetChildCount(MEASURE) > 0) {
        params->m_currentPage->AddChild(params->m_currentSystem);
        params->m_currentSystem = new System();
    }

    MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
