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
#include "functor.h"
#include "page.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// PageMilestoneEnd
//----------------------------------------------------------------------------

PageMilestoneEnd::PageMilestoneEnd(Object *start) : PageElement(PAGE_MILESTONE_END)
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

FunctorCode PageMilestoneEnd::Accept(Functor &functor)
{
    return functor.VisitPageMilestone(this);
}

FunctorCode PageMilestoneEnd::Accept(ConstFunctor &functor) const
{
    return functor.VisitPageMilestone(this);
}

FunctorCode PageMilestoneEnd::AcceptEnd(Functor &functor)
{
    return functor.VisitPageMilestoneEnd(this);
}

FunctorCode PageMilestoneEnd::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitPageMilestoneEnd(this);
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

} // namespace vrv
