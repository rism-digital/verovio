/////////////////////////////////////////////////////////////////////////////
// Name:        sb.cpp
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "sb.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "functorparams.h"
#include "page.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Sb
//----------------------------------------------------------------------------

Sb::Sb() : SystemElement("sb-"), AttNNumberLike()
{
    RegisterAttClass(ATT_NNUMBERLIKE);

    Reset();
}

Sb::~Sb() {}

void Sb::Reset()
{
    SystemElement::Reset();
    ResetNNumberLike();
}

//----------------------------------------------------------------------------
// Sb functor methods
//----------------------------------------------------------------------------

int Sb::CastOffEncoding(FunctorParams *functorParams)
{
    CastOffEncodingParams *params = vrv_params_cast<CastOffEncodingParams *>(functorParams);
    assert(params);

    params->m_currentSystem = new System();
    params->m_currentPage->AddChild(params->m_currentSystem);

    MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

int Sb::CastOffSystems(FunctorParams *functorParams)
{
    CastOffSystemsParams *params = vrv_params_cast<CastOffSystemsParams *>(functorParams);
    assert(params);
    if (params->m_smart) {
        // TODO: Break the layout if this system is almost full.
    }
}

} // namespace vrv
