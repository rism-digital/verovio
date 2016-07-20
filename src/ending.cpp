/////////////////////////////////////////////////////////////////////////////
// Name:        ending.h
// Author:      Laurent Pugin
// Created:     14/07/2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "ending.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// EndingBoundary
//----------------------------------------------------------------------------

EndingBoundary::EndingBoundary(EndingBoundary *startBoundary) : FloatingElement("ending-"), AttCommon()
{
    RegisterAttClass(ATT_COMMON);

    m_startBoundary = startBoundary;
    if (m_startBoundary) {
        m_startBoundary->SetEndBoundary(this);
    }
    else {
        m_endBoundary = NULL;
    }

    Reset();
}

EndingBoundary::~EndingBoundary()
{
}

void EndingBoundary::Reset()
{
    FloatingElement::Reset();
    ResetCommon();

    m_drawingMeasure = NULL;
}

//----------------------------------------------------------------------------
// EndingBoundary functor methods
//----------------------------------------------------------------------------

int EndingBoundary::PrepareEndings(FunctorParams *functorParams)
{
    PrepareEndingsParams *params = dynamic_cast<PrepareEndingsParams *>(functorParams);
    assert(params);

    // This is an end boundary - we need to its pointer to the last measure we have encountered
    if (this->IsEndBoundary()) {
        if (params->m_lastMeasure == NULL) {
            LogWarning("A measure cannot be set to the end ending boundary");
        }
        this->SetMeasure(params->m_lastMeasure);
        // We are done
        params->m_lastMeasure = NULL;
        params->m_currentEnding = NULL;
    }
    else if (this->IsStartBoundary()) {
        params->m_currentEnding = this;
    }

    return FUNCTOR_CONTINUE;
}

int EndingBoundary::ResetDrawing(FunctorParams *functorParams)
{
    m_drawingMeasure = NULL;

    return FUNCTOR_CONTINUE;
};

int EndingBoundary::CastOffSystems(FunctorParams *functorParams)
{
    CastOffSystemsParams *params = dynamic_cast<CastOffSystemsParams *>(functorParams);
    assert(params);

    // Since the functor returns FUNCTOR_SIBLINGS we should never go lower than the system children
    assert(dynamic_cast<System *>(this->m_parent));

    // Special case where we use the Relinquish method.
    // We want to move the measure to the currentSystem. However, we cannot use DetachChild
    // from the content System because this screws up the iterator. Relinquish gives up
    // the ownership of the Measure - the contentSystem will be deleted afterwards.
    EndingBoundary *ending = dynamic_cast<EndingBoundary *>(params->m_contentSystem->Relinquish(this->GetIdx()));
    params->m_currentSystem->AddEnding(ending);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv