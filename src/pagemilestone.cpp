/////////////////////////////////////////////////////////////////////////////
// Name:        pagemilestone.cpp
// Author:      Laurent Pugin
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pagemilestone.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "ending.h"
#include "functorparams.h"
#include "page.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// PageMilestoneEnd
//----------------------------------------------------------------------------

PageMilestoneEnd::PageMilestoneEnd(Object *start) : PageElement(PAGE_MILESTONE_END, "page-milestone-end-")
{
    this->Reset();
    m_start = start;
    m_startClassName = start->GetClassName();
}

PageMilestoneEnd::~PageMilestoneEnd() {}

void PageMilestoneEnd::Reset()
{
    m_start = NULL;
}

//----------------------------------------------------------------------------
// PageMilestoneInterface
//----------------------------------------------------------------------------

PageMilestoneInterface::PageMilestoneInterface()
{
    this->Reset();
}

PageMilestoneInterface::~PageMilestoneInterface() {}

void PageMilestoneInterface::Reset()
{
    m_end = NULL;
    // m_drawingMeasure = NULL;
}

void PageMilestoneInterface::SetEnd(PageMilestoneEnd *end)
{
    assert(!m_end);
    m_end = end;
}

void PageMilestoneInterface::ConvertToPageBasedMilestone(Object *object, Object *parent)
{
    assert(object);
    assert(parent);

    // Then add a PageMilestoneEnd
    PageMilestoneEnd *pageMilestoneEnd = new PageMilestoneEnd(object);
    this->SetEnd(pageMilestoneEnd);
    parent->AddChild(pageMilestoneEnd);

    // Also clear the relinquished children
    object->ClearRelinquishedChildren();
}

//----------------------------------------------------------------------------
// PageMilestoneEnd functor methods
//----------------------------------------------------------------------------

int PageMilestoneEnd::CastOffSystems(FunctorParams *functorParams)
{
    CastOffSystemsParams *params = vrv_params_cast<CastOffSystemsParams *>(functorParams);
    assert(params);

    assert(params->m_page);
    this->MoveItselfTo(params->m_page);

    return FUNCTOR_SIBLINGS;
}

int PageMilestoneEnd::CastOffPages(FunctorParams *functorParams)
{
    CastOffPagesParams *params = vrv_params_cast<CastOffPagesParams *>(functorParams);
    assert(params);

    assert(params->m_currentPage);

    PageMilestoneEnd *endMilestone
        = dynamic_cast<PageMilestoneEnd *>(params->m_contentPage->Relinquish(this->GetIdx()));
    // End milestones can be added to the page only if the pending list is empty
    // Otherwise we are going to mess up the order
    if (params->m_pendingPageElements.empty()) {
        params->m_currentPage->AddChild(endMilestone);
    }
    else {
        params->m_pendingPageElements.push_back(endMilestone);
    }

    return FUNCTOR_SIBLINGS;
}

int PageMilestoneEnd::CastOffEncoding(FunctorParams *functorParams)
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

int PageMilestoneEnd::CastOffToSelection(FunctorParams *functorParams)
{
    CastOffToSelectionParams *params = vrv_params_cast<CastOffToSelectionParams *>(functorParams);
    assert(params);

    assert(params->m_page);
    this->MoveItselfTo(params->m_page);

    return FUNCTOR_SIBLINGS;
}

int PageMilestoneEnd::UnCastOff(FunctorParams *functorParams)
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

int PageMilestoneEnd::Transpose(FunctorParams *functorParams)
{
    TransposeParams *params = vrv_params_cast<TransposeParams *>(functorParams);
    assert(params);

    if (this->m_start && this->m_start->Is(MDIV)) {
        params->m_currentMdivIDs.pop_back();
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

} // namespace vrv
