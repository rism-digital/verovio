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
}

void LinkingInterface::SetNextLink(Object *next)
{
    assert(!m_next);
    m_next = next;
}

void LinkingInterface::SetUuidStr()
{
    if (this->HasNext()) {
        m_nextUuid = ExtractUuidFragment(this->GetNext());
    }
}

Measure *LinkingInterface::GetNextMeasure()
{
    if (!m_next) return NULL;
    return dynamic_cast<Measure *>(this->m_next->GetFirstParent(MEASURE));
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

int LinkingInterface::InterfacePrepareLinking(FunctorParams *functorParams, Object *object)
{
    PrepareLinkingParams *params = dynamic_cast<PrepareLinkingParams *>(functorParams);
    assert(params);

    // This should not happen?
    if (params->m_fillList == false) {
        return FUNCTOR_CONTINUE;
    }

    this->SetUuidStr();
    
    if (!m_nextUuid.empty()) {
        params->m_nextUuidPairs.push_back(std::make_pair(this, m_nextUuid));
    }
    
    return FUNCTOR_CONTINUE;
}
    
/*
int TimePointInterface::InterfacePrepareTimestamps(FunctorParams *functorParams, Object *object)
{
    PrepareTimestampsParams *params = dynamic_cast<PrepareTimestampsParams *>(functorParams);
    assert(params);

    // First we check if the object has already a mapped @startid (it should not)
    if (this->HasStart()) {
        if (this->HasTstamp())
            LogWarning("%s with @xml:id %s has both a @startid and an @tstamp; @tstamp is ignored",
                object->GetClassName().c_str(), object->GetUuid().c_str());
        return FUNCTOR_CONTINUE;
    }
    else if (!HasTstamp()) {
        return FUNCTOR_CONTINUE; // This file is quite likely invalid?
    }

    // We set -1 to the data_MEASUREBEAT for @tstamp
    params->m_tstamps.push_back(std::make_pair(object, data_MEASUREBEAT(-1, this->GetTstamp())));

    return FUNCTOR_CONTINUE;
}
*/

int LinkingInterface::InterfaceResetDrawing(FunctorParams *functorParams, Object *object)
{
    m_next = NULL;
    m_nextUuid = "";
    return FUNCTOR_CONTINUE;
}

/*
int TimePointInterface::InterfacePrepareTimePointing(FunctorParams *functorParams, Object *object)
{
    PrepareTimePointingParams *params = dynamic_cast<PrepareTimePointingParams *>(functorParams);
    assert(params);

    if (!this->HasStartid()) return FUNCTOR_CONTINUE;

    this->SetUuidStr();
    params->m_timePointingInterfaces.push_back(std::make_pair(this, object->GetClassId()));

    return FUNCTOR_CONTINUE;
}

int TimeSpanningInterface::InterfacePrepareTimeSpanning(FunctorParams *functorParams, Object *object)
{
    PrepareTimeSpanningParams *params = dynamic_cast<PrepareTimeSpanningParams *>(functorParams);
    assert(params);

    if (!this->HasStartid() && !this->HasEndid()) {
        return FUNCTOR_CONTINUE;
    }

    if (params->m_fillList == false) {
        return FUNCTOR_CONTINUE;
    }

    this->SetUuidStr();
    params->m_timeSpanningInterfaces.push_back(std::make_pair(this, object->GetClassId()));

    return FUNCTOR_CONTINUE;
}

int TimeSpanningInterface::InterfacePrepareTimestamps(FunctorParams *functorParams, Object *object)
{
    PrepareTimestampsParams *params = dynamic_cast<PrepareTimestampsParams *>(functorParams);
    assert(params);

    // First we check if the object has already a mapped @endid (it should not)
    if (this->HasEndid()) {
        if (this->HasTstamp2())
            LogWarning("%s with @xml:id %s has both a @endid and an @tstamp2; @tstamp2 is ignored",
                object->GetClassName().c_str(), object->GetUuid().c_str());
        if (this->GetStartid() == this->GetEndid()) {
            LogWarning("%s with @xml:id %s will not get rendered as it has identical values in @startid and @endid",
                object->GetClassName().c_str(), object->GetUuid().c_str());
        }
        return TimePointInterface::InterfacePrepareTimestamps(functorParams, object);
    }
    else if (!HasTstamp2()) {
        // We won't be able to do anything, just try to prepare the tstamp (start)
        return TimePointInterface::InterfacePrepareTimestamps(functorParams, object);
    }

    // We can now add the pair to our stack
    params->m_tstamps.push_back(std::make_pair(object, data_MEASUREBEAT(this->GetTstamp2())));

    return TimePointInterface::InterfacePrepareTimestamps(params, object);
}

int TimeSpanningInterface::InterfaceFillStaffCurrentTimeSpanning(FunctorParams *functorParams, Object *object)
{
    FillStaffCurrentTimeSpanningParams *params = dynamic_cast<FillStaffCurrentTimeSpanningParams *>(functorParams);
    assert(params);

    if (this->IsSpanningMeasures()) {
        params->m_timeSpanningElements.push_back(object);
    }
    return FUNCTOR_CONTINUE;
}

int TimeSpanningInterface::InterfaceResetDrawing(FunctorParams *functorParams, Object *object)
{
    m_end = NULL;
    m_endUuid = "";
    // Special case where we have interface inheritance
    return TimePointInterface::InterfaceResetDrawing(functorParams, object);
}
*/

} // namespace vrv
