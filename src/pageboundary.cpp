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

    /*
    // Since the functor returns FUNCTOR_SIBLINGS we should never go lower than the system children
    assert(dynamic_cast<System *>(this->GetParent()));

    // Special case where we use the Relinquish method.
    // We want to move the measure to the currentSystem. However, we cannot use DetachChild
    // from the content System because this screws up the iterator. Relinquish gives up
    // the ownership of the Measure - the contentSystem will be deleted afterwards.
    PageElementEnd *endBoundary = dynamic_cast<PageElementEnd *>(params->m_contentSystem->Relinquish(this->GetIdx()));
    // End boundaries are not added to the pending objects because we do not want them to be placed at the beginning of
    // the next system but only if the pending object array it empty (otherwise it will mess up the MEI tree)
    if (params->m_pendingObjects.empty())
        params->m_currentSystem->AddChild(endBoundary);
    else
        params->m_pendingObjects.push_back(endBoundary);
     */

    return FUNCTOR_SIBLINGS;
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

} // namespace vrv
