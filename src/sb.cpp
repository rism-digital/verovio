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

#include "doc.h"
#include "editorial.h"
#include "functorparams.h"
#include "measure.h"
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
        // Get the last measure of the currentSystem
        Measure *measure
            = dynamic_cast<Measure *>(params->m_currentSystem->GetChild(params->m_currentSystem->GetChildCount() - 1));
        if (measure != NULL) {
            int measureRightX = measure->GetDrawingX() + measure->GetWidth() - params->m_shift;
            // LogDebug("ratio: %f\n", (float)measureRightX / (float)params->m_systemWidth);
            double smartSbThresh = params->m_doc->GetOptions()->m_breaksSmartSb.GetValue();
            if (measureRightX > params->m_systemWidth * smartSbThresh) {
                // Use this system break.
                params->m_currentSystem = new System();
                params->m_page->AddChild(params->m_currentSystem);
                params->m_shift += measureRightX;
            }
        }
    }
    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
