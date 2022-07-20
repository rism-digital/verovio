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

int Pb::CastOffEncoding(FunctorParams *functorParams)
{
    CastOffEncodingParams *params = vrv_params_cast<CastOffEncodingParams *>(functorParams);
    assert(params);

    // We look if the current system has a pb or at least one measure - if yes, we assume that the <pb>
    // is not the one at the beginning of the content. This is not very robust but at least make it
    // work when rendering a <mdiv> that does not start with a <pb> (which we cannot force)
    if (params->m_currentSystem->GetChildCount(PB) > 0 || params->m_currentSystem->GetChildCount(MEASURE) > 0) {
        params->m_currentPage->AddChild(params->m_currentSystem);
        params->m_currentSystem = new System();
        if (params->m_usePages) {
            params->m_currentPage = new Page();
            assert(params->m_doc->GetPages());
            params->m_doc->GetPages()->AddChild(params->m_currentPage);
        }
    }

    MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
