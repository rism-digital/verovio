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
#include "functorparams.h"
#include "preparedatafunctor.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// SystemMilestoneEnd
//----------------------------------------------------------------------------

SystemMilestoneEnd::SystemMilestoneEnd(Object *start) : SystemElement(SYSTEM_MILESTONE_END, "system-milestone-end-")
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

FunctorCode SystemMilestoneEnd::Accept(MutableFunctor &functor)
{
    return functor.VisitSystemMilestone(this);
}

FunctorCode SystemMilestoneEnd::Accept(ConstFunctor &functor) const
{
    return functor.VisitSystemMilestone(this);
}

FunctorCode SystemMilestoneEnd::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitSystemMilestoneEnd(this);
}

FunctorCode SystemMilestoneEnd::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitSystemMilestoneEnd(this);
}

int SystemMilestoneEnd::CastOffSystems(FunctorParams *functorParams)
{
    CastOffSystemsParams *params = vrv_params_cast<CastOffSystemsParams *>(functorParams);
    assert(params);

    // Since the functor returns FUNCTOR_SIBLINGS we should never go lower than the system children
    assert(dynamic_cast<System *>(this->GetParent()));

    // Special case where we use the Relinquish method.
    // We want to move the measure to the currentSystem. However, we cannot use DetachChild
    // from the content System because this screws up the iterator. Relinquish gives up
    // the ownership of the Measure - the contentSystem will be deleted afterwards.
    SystemMilestoneEnd *endMilestone
        = dynamic_cast<SystemMilestoneEnd *>(params->m_contentSystem->Relinquish(this->GetIdx()));
    // End milestones are not added to the pending objects because we do not want them to be placed at the beginning of
    // the next system but only if the pending object array it empty (otherwise it will mess up the MEI tree)
    if (params->m_pendingElements.empty()) {
        params->m_currentSystem->AddChild(endMilestone);
    }
    else {
        params->m_pendingElements.push_back(endMilestone);
    }

    return FUNCTOR_SIBLINGS;
}

int SystemMilestoneEnd::CastOffToSelection(FunctorParams *functorParams)
{
    CastOffToSelectionParams *params = vrv_params_cast<CastOffToSelectionParams *>(functorParams);
    assert(params);

    MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_SIBLINGS;
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
