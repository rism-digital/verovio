/////////////////////////////////////////////////////////////////////////////
// Name:        systemboundary.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "systemboundary.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "ending.h"
#include "functorparams.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// SystemElementEnd
//----------------------------------------------------------------------------

SystemElementEnd::SystemElementEnd(Object *start) : SystemElement("system-element-end-")
{
    Reset();
    m_start = start;
    m_startClassName = start->GetClassName();
}

SystemElementEnd::~SystemElementEnd() {}

void SystemElementEnd::Reset()
{
    m_start = NULL;
    m_drawingMeasure = NULL;
}

//----------------------------------------------------------------------------
// SystemElementStartInterface
//----------------------------------------------------------------------------

SystemElementStartInterface::SystemElementStartInterface()
{
    Reset();
}

SystemElementStartInterface::~SystemElementStartInterface() {}

void SystemElementStartInterface::Reset()
{
    m_end = NULL;
    m_drawingMeasure = NULL;
}

void SystemElementStartInterface::SetEnd(SystemElementEnd *end)
{
    assert(!m_end);
    m_end = end;
}

void SystemElementStartInterface::ConvertToPageBasedBoundary(Object *object, Object *parent)
{
    assert(object);
    assert(parent);

    // Then add a SystemElementEnd
    SystemElementEnd *systemElementEnd = new SystemElementEnd(object);
    this->SetEnd(systemElementEnd);
    parent->AddChild(systemElementEnd);

    // Also clear the relinquished children
    object->ClearRelinquishedChildren();
}

//----------------------------------------------------------------------------
// SystemElementEnd functor methods
//----------------------------------------------------------------------------

int SystemElementEnd::PrepareBoundaries(FunctorParams *functorParams)
{
    PrepareBoundariesParams *params = vrv_params_cast<PrepareBoundariesParams *>(functorParams);
    assert(params);

    // We set its pointer to the last measure we have encountered - this can be NULL in case no measure exists before
    // the end boundary
    // This can happen with a editorial container around a scoreDef at the beginning
    this->SetMeasure(params->m_lastMeasure);

    // Endings are also set as Measure::m_drawingEnding for all measures in between - when we reach the end boundary of
    // an ending, we need to set the m_currentEnding to NULL
    if (params->m_currentEnding && this->GetStart()->Is(ENDING)) {
        params->m_currentEnding = NULL;
        // With ending we need the drawing measure - this will crash with en empty ending at the beginning of a score...
        assert(m_drawingMeasure);
    }

    return FUNCTOR_CONTINUE;
}

int SystemElementEnd::ResetDrawing(FunctorParams *functorParams)
{
    FloatingObject::ResetDrawing(functorParams);

    this->SetMeasure(NULL);

    return FUNCTOR_CONTINUE;
}

int SystemElementEnd::CastOffSystems(FunctorParams *functorParams)
{
    CastOffSystemsParams *params = vrv_params_cast<CastOffSystemsParams *>(functorParams);
    assert(params);

    // Since the functor returns FUNCTOR_SIBLINGS we should never go lower than the system children
    assert(dynamic_cast<System *>(this->GetParent()));

    // Special case where we use the Relinquish method.
    // We want to move the measure to the currentSystem. However, we cannot use DetachChild
    // from the content System because this screws up the iterator. Relinquish gives up
    // the ownership of the Measure - the contentSystem will be deleted afterwards.
    SystemElementEnd *endBoundary
        = dynamic_cast<SystemElementEnd *>(params->m_contentSystem->Relinquish(this->GetIdx()));
    // End boundaries are not added to the pending objects because we do not want them to be placed at the beginning of
    // the next system but only if the pending object array it empty (otherwise it will mess up the MEI tree)
    if (params->m_pendingElements.empty())
        params->m_currentSystem->AddChild(endBoundary);
    else
        params->m_pendingElements.push_back(endBoundary);

    return FUNCTOR_SIBLINGS;
}

int SystemElementEnd::PrepareFloatingGrps(FunctorParams *functorParams)
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

int SystemElementStartInterface::InterfacePrepareBoundaries(FunctorParams *functorParams)
{
    PrepareBoundariesParams *params = vrv_params_cast<PrepareBoundariesParams *>(functorParams);
    assert(params);

    // We have to be in a boundary start element
    assert(m_end);

    params->m_startBoundaries.push_back(this);

    return FUNCTOR_CONTINUE;
}

int SystemElementStartInterface::InterfaceResetDrawing(FunctorParams *functorParams)
{
    m_drawingMeasure = NULL;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
