/////////////////////////////////////////////////////////////////////////////
// Name:        pb.cpp
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pb.h"

//----------------------------------------------------------------------------

#include <assert.h>

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

Pb::Pb() : SystemElement("pb-"), AttNNumberLike()
{
    RegisterAttClass(ATT_NNUMBERLIKE);

    Reset();
}

Pb::~Pb() {}

void Pb::Reset()
{
    SystemElement::Reset();
    ResetNNumberLike();
}

//----------------------------------------------------------------------------
// Pb functor methods
//----------------------------------------------------------------------------

int Pb::CastOffEncoding(FunctorParams *functorParams)
{
    CastOffEncodingParams *params = dynamic_cast<CastOffEncodingParams *>(functorParams);
    assert(params);

    if (!params->m_firstPbProcessed) {
        params->m_firstPbProcessed = true;
    }
    else {
        params->m_currentPage = new Page();
        assert(params->m_doc->GetPages());
        params->m_doc->GetPages()->AddChild(params->m_currentPage);
        params->m_currentSystem = new System();
        params->m_currentPage->AddChild(params->m_currentSystem);
    }

    MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
