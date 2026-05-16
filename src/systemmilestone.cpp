/////////////////////////////////////////////////////////////////////////////
// Name:        systemmilestone.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "systemmilestone.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "ending.h"
#include "functor.h"
#include "preparedatafunctor.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// SystemMilestoneEnd
//----------------------------------------------------------------------------

SystemMilestoneEnd::SystemMilestoneEnd(Object *start) : SystemElement(SYSTEM_MILESTONE_END)
{
    this->Reset();
    m_start = start;
    m_startClassName = start->GetClassName();
}

SystemMilestoneEnd::~SystemMilestoneEnd() {}

void SystemMilestoneEnd::Reset()
{
    m_start = NULL;
    m_drawingMeasure = NULL;
}

//----------------------------------------------------------------------------
// SystemMilestoneInterface
//----------------------------------------------------------------------------

SystemMilestoneInterface::SystemMilestoneInterface()
{
    this->Reset();
}

SystemMilestoneInterface::~SystemMilestoneInterface() {}

void SystemMilestoneInterface::Reset()
{
    m_end = NULL;
    m_drawingMeasure = NULL;
}

void SystemMilestoneInterface::SetEnd(SystemMilestoneEnd *end)
{
    assert(!m_end);
    m_end = end;
}

void SystemMilestoneInterface::ConvertToPageBasedMilestone(Object *object, Object *parent)
{
    assert(object);
    assert(parent);

    // Then add a SystemMilestoneEnd
    SystemMilestoneEnd *systemMilestoneEnd = new SystemMilestoneEnd(object);
    this->SetEnd(systemMilestoneEnd);
    parent->AddChild(systemMilestoneEnd);

    // Also clear the relinquished children
    object->ClearRelinquishedChildren();
}

//----------------------------------------------------------------------------
// SystemMilestoneEnd functor methods
//----------------------------------------------------------------------------

FunctorCode SystemMilestoneEnd::Accept(Functor &functor)
{
    return functor.VisitSystemMilestone(this);
}

FunctorCode SystemMilestoneEnd::Accept(ConstFunctor &functor) const
{
    return functor.VisitSystemMilestone(this);
}

FunctorCode SystemMilestoneEnd::AcceptEnd(Functor &functor)
{
    return functor.VisitSystemMilestoneEnd(this);
}

FunctorCode SystemMilestoneEnd::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitSystemMilestoneEnd(this);
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

FunctorCode SystemMilestoneInterface::InterfacePrepareMilestones(PrepareMilestonesFunctor &functor)
{
    // We have to be in a milestone start element
    assert(m_end);

    functor.InsertStartMilestone(this);

    return FUNCTOR_CONTINUE;
}

FunctorCode SystemMilestoneInterface::InterfaceResetData(ResetDataFunctor &functor)
{
    m_drawingMeasure = NULL;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
