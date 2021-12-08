/////////////////////////////////////////////////////////////////////////////
// Name:        pagems.cpp
// Author:      Laurent Pugin
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pagems.h"

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
// PageMsEnd
//----------------------------------------------------------------------------

PageMsEnd::PageMsEnd(Object *start) : PageElement(PAGE_MS_END, "page-ms-end-")
{
    Reset();
    m_start = start;
    m_startClassName = start->GetClassName();
}

PageMsEnd::~PageMsEnd() {}

void PageMsEnd::Reset()
{
    m_start = NULL;
}

//----------------------------------------------------------------------------
// PageMsInterface
//----------------------------------------------------------------------------

PageMsInterface::PageMsInterface()
{
    Reset();
}

PageMsInterface::~PageMsInterface() {}

void PageMsInterface::Reset()
{
    m_end = NULL;
    // m_drawingMeasure = NULL;
}

void PageMsInterface::SetEnd(PageMsEnd *end)
{
    assert(!m_end);
    m_end = end;
}

void PageMsInterface::ConvertToPageBasedBoundary(Object *object, Object *parent)
{
    assert(object);
    assert(parent);

    // Then add a PageMsEnd
    PageMsEnd *pageMsEnd = new PageMsEnd(object);
    this->SetEnd(pageMsEnd);
    parent->AddChild(pageMsEnd);

    // Also clear the relinquished children
    object->ClearRelinquishedChildren();
}

//----------------------------------------------------------------------------
// PageMsEnd functor methods
//----------------------------------------------------------------------------

int PageMsEnd::CastOffSystems(FunctorParams *functorParams)
{
    CastOffSystemsParams *params = vrv_params_cast<CastOffSystemsParams *>(functorParams);
    assert(params);

    assert(params->m_page);
    this->MoveItselfTo(params->m_page);

    return FUNCTOR_SIBLINGS;
}

int PageMsEnd::CastOffPages(FunctorParams *functorParams)
{
    CastOffPagesParams *params = vrv_params_cast<CastOffPagesParams *>(functorParams);
    assert(params);

    assert(params->m_currentPage);

    PageMsEnd *endBoundary = dynamic_cast<PageMsEnd *>(params->m_contentPage->Relinquish(this->GetIdx()));
    // End boundaries can be added to the page only if the pending list is empty
    // Otherwise we are going to mess up the order
    if (params->m_pendingPageElements.empty()) {
        params->m_currentPage->AddChild(endBoundary);
    }
    else {
        params->m_pendingPageElements.push_back(endBoundary);
    }

    return FUNCTOR_SIBLINGS;
}

int PageMsEnd::CastOffEncoding(FunctorParams *functorParams)
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

int PageMsEnd::UnCastOff(FunctorParams *functorParams)
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
