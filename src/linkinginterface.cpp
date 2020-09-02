/////////////////////////////////////////////////////////////////////////////
// Name:        linkinginterface.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "linkinginterface.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "measure.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// LinkingInterface
//----------------------------------------------------------------------------

LinkingInterface::LinkingInterface() : Interface(), AttLinking()
{
    RegisterInterfaceAttClass(ATT_LINKING);

    Reset();
}

LinkingInterface::~LinkingInterface() {}

void LinkingInterface::Reset()
{
    ResetLinking();

    m_next = NULL;
    m_nextUuid = "";
    m_sameas = NULL;
    m_sameasUuid = "";
}

void LinkingInterface::SetNextLink(Object *next)
{
    assert(!m_next);
    m_next = next;
}

void LinkingInterface::SetSameasLink(Object *sameas)
{
    assert(!m_sameas);
    m_sameas = sameas;
}

void LinkingInterface::SetUuidStr()
{
    if (this->HasNext()) {
        m_nextUuid = ExtractUuidFragment(this->GetNext());
    }
    if (this->HasSameas()) {
        m_sameasUuid = ExtractUuidFragment(this->GetSameas());
    }
}

Measure *LinkingInterface::GetNextMeasure()
{
    if (!m_next) return NULL;
    return dynamic_cast<Measure *>(this->m_next->GetFirstAncestor(MEASURE));
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

int LinkingInterface::InterfacePrepareLinking(FunctorParams *functorParams, Object *object)
{
    PrepareLinkingParams *params = vrv_params_cast<PrepareLinkingParams *>(functorParams);
    assert(params);

    // This should not happen?
    if (params->m_fillList == false) {
        return FUNCTOR_CONTINUE;
    }

    this->SetUuidStr();

    if (!m_nextUuid.empty()) {
        params->m_nextUuidPairs.insert({ m_nextUuid, this });
    }
    if (!m_sameasUuid.empty()) {
        params->m_sameasUuidPairs.insert({ m_sameasUuid, this });
    }

    return FUNCTOR_CONTINUE;
}

int LinkingInterface::InterfaceFillStaffCurrentTimeSpanning(FunctorParams *functorParams, Object *object)
{
    FillStaffCurrentTimeSpanningParams *params = vrv_params_cast<FillStaffCurrentTimeSpanningParams *>(functorParams);
    assert(params);

    // Only Dir and Dynam can be spanning with @next (extender)
    if (!object->Is({ DIR, DYNAM })) {
        return FUNCTOR_CONTINUE;
    }

    // Only target control events are supported
    if (!this->GetNextLink() || !this->GetNextLink()->IsControlElement()) {
        return FUNCTOR_CONTINUE;
    }

    // if @extender is available, the explicit "true" is required
    if (object->HasAttClass(ATT_EXTENDER)) {
        AttExtender *att = dynamic_cast<AttExtender *>(object);
        assert(att);
        if (att->GetExtender() != BOOLEAN_true) return FUNCTOR_CONTINUE;
    }

    params->m_timeSpanningElements.push_back(object);

    return FUNCTOR_CONTINUE;
}

int LinkingInterface::InterfaceResetDrawing(FunctorParams *functorParams, Object *object)
{
    m_next = NULL;
    m_nextUuid = "";
    m_sameas = NULL;
    m_sameasUuid = "";
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
