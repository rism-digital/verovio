/////////////////////////////////////////////////////////////////////////////
// Name:        pageboundary.cpp
// Author:      Laurent Pugin
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pageboundary.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "ending.h"
#include "functorparams.h"
#include "page.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// PageElementEnd
//----------------------------------------------------------------------------

PageElementEnd::PageElementEnd(Object *start) : PageElement("page-element-end-")
{
    Reset();
    m_start = start;
    m_startClassName = start->GetClassName();
}

PageElementEnd::~PageElementEnd() {}

void PageElementEnd::Reset()
{
    m_start = NULL;
}

//----------------------------------------------------------------------------
// PageElementStartInterface
//----------------------------------------------------------------------------

PageElementStartInterface::PageElementStartInterface()
{
    Reset();
}

PageElementStartInterface::~PageElementStartInterface() {}

void PageElementStartInterface::Reset()
{
    m_end = NULL;
    // m_drawingMeasure = NULL;
}

void PageElementStartInterface::SetEnd(PageElementEnd *end)
{
    assert(!m_end);
    m_end = end;
}

void PageElementStartInterface::ConvertToPageBasedBoundary(Object *object, Object *parent)
{
    assert(object);
    assert(parent);

    // Then add a PageElementEnd
    PageElementEnd *pageElementEnd = new PageElementEnd(object);
    this->SetEnd(pageElementEnd);
    parent->AddChild(pageElementEnd);

    // Also clear the relinquished children
    object->ClearRelinquishedChildren();
}

//----------------------------------------------------------------------------
// PageElementEnd functor methods
//----------------------------------------------------------------------------

int PageElementEnd::CastOffSystems(FunctorParams *functorParams)
{
    CastOffSystemsParams *params = vrv_params_cast<CastOffSystemsParams *>(functorParams);
    assert(params);

    assert(params->m_page);
    this->MoveItselfTo(params->m_page);

    return FUNCTOR_SIBLINGS;
}

int PageElementEnd::CastOffPages(FunctorParams *functorParams)
{
    CastOffPagesParams *params = vrv_params_cast<CastOffPagesParams *>(functorParams);
    assert(params);

    assert(params->m_currentPage);
    this->MoveItselfTo(params->m_currentPage);

    return FUNCTOR_SIBLINGS;
}

int PageElementEnd::CastOffEncoding(FunctorParams *functorParams)
{
    CastOffEncodingParams *params = vrv_params_cast<CastOffEncodingParams *>(functorParams);
    assert(params);

    if (this->m_start && this->m_start->Is(SCORE)) {
        // This is the end of a score, which means that the current system has to be added
        // to the current page
        assert(params->m_currentSystem);
        params->m_currentPage->AddChild(params->m_currentSystem);
        params->m_currentSystem = NULL;
    }

    MoveItselfTo(params->m_currentPage);

    return FUNCTOR_SIBLINGS;
}

int PageElementEnd::UnCastOff(FunctorParams *functorParams)
{
    UnCastOffParams *params = vrv_params_cast<UnCastOffParams *>(functorParams);
    assert(params);

    if (this->m_start && this->m_start->Is(SCORE)) {
        // This is the end of a score, which means that nothing else should be added to
        // the current system and we set it to NULL;
        assert(params->m_currentSystem);
        params->m_currentSystem = NULL;
    }

    MoveItselfTo(params->m_page);

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

} // namespace vrv
