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
#include "functorparams.h"
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

int SystemMilestoneEnd::PrepareMilestones(FunctorParams *functorParams)
{
    PrepareMilestonesParams *params = vrv_params_cast<PrepareMilestonesParams *>(functorParams);
    assert(params);

    // We set its pointer to the last measure we have encountered - this can be NULL in case no measure exists before
    // the end milestone
    // This can happen with a editorial container around a scoreDef at the beginning
    this->SetMeasure(params->m_lastMeasure);

    // Endings are also set as Measure::m_drawingEnding for all measures in between - when we reach the end milestone of
    // an ending, we need to set the m_currentEnding to NULL
    if (params->m_currentEnding && this->GetStart()->Is(ENDING)) {
        params->m_currentEnding = NULL;
        // With ending we need the drawing measure - this will crash with en empty ending at the beginning of a score...
        assert(m_drawingMeasure);
    }

    return FUNCTOR_CONTINUE;
}

int SystemMilestoneEnd::ResetData(FunctorParams *functorParams)
{
    FloatingObject::ResetData(functorParams);

    this->SetMeasure(NULL);

    return FUNCTOR_CONTINUE;
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

int SystemMilestoneEnd::PrepareFloatingGrps(FunctorParams *functorParams)
{
    PrepareFloatingGrpsParams *params = vrv_params_cast<PrepareFloatingGrpsParams *>(functorParams);
    assert(params);

    assert(this->GetStart());

    // We are reaching the end of an ending - put it to the param and it will be grouped with the next one if there is
    // not measure in between
    if (this->GetStart()->Is(ENDING)) {
        params->m_previousEnding = vrv_cast<Ending *>(this->GetStart());
        assert(params->m_previousEnding);
        // This is the end of the first ending - generate a grpId
        if (params->m_previousEnding->GetDrawingGrpId() == 0) {
            params->m_previousEnding->SetDrawingGrpObject(params->m_previousEnding);
        }
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

int SystemMilestoneInterface::InterfacePrepareMilestones(FunctorParams *functorParams)
{
    PrepareMilestonesParams *params = vrv_params_cast<PrepareMilestonesParams *>(functorParams);
    assert(params);

    // We have to be in a milestone start element
    assert(m_end);

    params->m_startMilestones.push_back(this);

    return FUNCTOR_CONTINUE;
}

int SystemMilestoneInterface::InterfaceResetData(FunctorParams *functorParams)
{
    m_drawingMeasure = NULL;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
