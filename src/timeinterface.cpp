/////////////////////////////////////////////////////////////////////////////
// Name:        timeinterface.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "timeinterface.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "attcomparison.h"
#include "layerelement.h"
#include "measure.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// TimePointInterface
//----------------------------------------------------------------------------

TimePointInterface::TimePointInterface() : Interface(), AttStaffident(), AttStartid(), AttTimestampMusical()
{
    RegisterInterfaceAttClass(ATT_STAFFIDENT);
    RegisterInterfaceAttClass(ATT_STARTID);
    RegisterInterfaceAttClass(ATT_TIMESTAMPMUSICAL);

    Reset();
}

TimePointInterface::~TimePointInterface()
{
}

void TimePointInterface::Reset()
{
    ResetStaffident();
    ResetStartid();
    ResetTimestampMusical();

    m_start = NULL;
    m_startUuid = "";
}

void TimePointInterface::SetStart(LayerElement *start)
{
    assert(!m_start);
    m_start = start;
}

void TimePointInterface::SetUuidStr()
{
    if (this->HasStartid()) {
        m_startUuid = this->ExtractUuidFragment(this->GetStartid());
    }
}

std::string TimePointInterface::ExtractUuidFragment(std::string refUuid)
{
    size_t pos = refUuid.find_last_of("#");
    if ((pos != std::string::npos) && (pos < refUuid.length() - 1)) {
        refUuid = refUuid.substr(pos + 1);
    }
    return refUuid;
}

Measure *TimePointInterface::GetStartMeasure()
{
    if (!m_start) return NULL;
    return dynamic_cast<Measure *>(this->m_start->GetFirstParent(MEASURE));
}

bool TimePointInterface::IsOnStaff(int n)
{
    if (this->HasStaff()) {
        std::vector<int> staffList = this->GetStaff();
        std::vector<int>::iterator iter;
        for (iter = staffList.begin(); iter != staffList.end(); iter++) {
            if (*iter == n) return true;
        }
        return false;
    }
    else if (m_start) {
        Staff *staff = dynamic_cast<Staff *>(m_start->GetFirstParent(STAFF));
        if (staff && (staff->GetN() == n)) return true;
    }
    return false;
}

std::vector<Staff *> TimePointInterface::GetTstampStaves(Measure *measure)
{
    std::vector<Staff *> staves;
    std::vector<int>::iterator iter;
    std::vector<int> staffList;
    if (this->HasStaff()) {
        staffList = this->GetStaff();
    }
    else if (m_start) {
        Staff *staff = dynamic_cast<Staff *>(m_start->GetFirstParent(STAFF));
        if (staff) staffList.push_back(staff->GetN());
    }
    for (iter = staffList.begin(); iter != staffList.end(); iter++) {
        AttCommonNComparison comparison(STAFF, *iter);
        Staff *staff = dynamic_cast<Staff *>(measure->FindChildByAttComparison(&comparison, 1));
        if (!staff) {
            // LogDebug("Staff with @n '%d' not found in measure '%s'", *iter, measure->GetUuid().c_str());
            continue;
        }
        staves.push_back(staff);
    }
    return staves;
}

//----------------------------------------------------------------------------
// TimeSpanningInterface
//----------------------------------------------------------------------------

TimeSpanningInterface::TimeSpanningInterface() : TimePointInterface(), AttStartendid(), AttTimestamp2Musical()
{
    RegisterInterfaceAttClass(ATT_STARTENDID);
    RegisterInterfaceAttClass(ATT_TIMESTAMP2MUSICAL);

    Reset();
}

TimeSpanningInterface::~TimeSpanningInterface()
{
}

void TimeSpanningInterface::Reset()
{
    TimePointInterface::Reset();
    ResetStartendid();
    ResetTimestamp2Musical();

    m_end = NULL;
    m_endUuid = "";
}

void TimeSpanningInterface::SetEnd(LayerElement *end)
{
    assert(!m_end);
    m_end = end;
}

void TimeSpanningInterface::SetUuidStr()
{
    TimePointInterface::SetUuidStr();
    if (this->HasEndid()) {
        m_endUuid = this->ExtractUuidFragment(this->GetEndid());
    }
}

bool TimeSpanningInterface::SetStartAndEnd(LayerElement *element)
{
    // LogDebug("%s - %s - %s", element->GetUuid().c_str(), m_startUuid.c_str(), m_endUuid.c_str() );
    if (!m_start && (element->GetUuid() == m_startUuid)) {
        this->SetStart(element);
    }
    else if (!m_end && (element->GetUuid() == m_endUuid)) {
        this->SetEnd(element);
    }
    return (m_start && m_end);
}

Measure *TimeSpanningInterface::GetEndMeasure()
{
    if (!m_end) return NULL;
    return dynamic_cast<Measure *>(this->m_end->GetFirstParent(MEASURE));
}

bool TimeSpanningInterface::IsSpanningMeasures()
{
    if (!this->HasStartAndEnd()) return false;
    return (this->GetStartMeasure() != this->GetEndMeasure());
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

int TimePointInterface::InterfacePrepareTimestamps(ArrayPtrVoid *params, DocObject *object)
{
    // param 0: std::vector<DocObject*>* that holds the current elements to match (unused)
    // param 1: ArrayOfDocObjectBeatPairs* that holds the tstamp2 elements for attach to the end measure
    ArrayOfDocObjectBeatPairs *tstamps = static_cast<ArrayOfDocObjectBeatPairs *>((*params).at(1));

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
    tstamps->push_back(std::make_pair(object, data_MEASUREBEAT(-1, this->GetTstamp())));

    return FUNCTOR_CONTINUE;
}

int TimePointInterface::InterfaceResetDrawing(ArrayPtrVoid *params, DocObject *object)
{
    m_start = NULL;
    m_startUuid = "";
    return FUNCTOR_CONTINUE;
}

int TimeSpanningInterface::InterfacePrepareTimeSpanning(ArrayPtrVoid *params, DocObject *object)
{
    // param 0: std::vector<DocObject*>* that holds the current elements to match
    // param 1: bool* fillList for indicating whether the elements have to be stacked or not
    std::vector<DocObject *> *elements = static_cast<std::vector<DocObject *> *>((*params).at(0));
    bool *fillList = static_cast<bool *>((*params).at(1));

    if ((*fillList) == false) {
        return FUNCTOR_CONTINUE;
    }

    this->SetUuidStr();
    elements->push_back(object);

    return FUNCTOR_CONTINUE;
}

int TimeSpanningInterface::InterfacePrepareTimestamps(ArrayPtrVoid *params, DocObject *object)
{
    // param 0: std::vector<DocObject*>* that holds the current elements to match (unused)
    // param 1: ArrayOfDocObjectBeatPairs* that holds the tstamp2 elements for attach to the end measure
    ArrayOfDocObjectBeatPairs *tstamps = static_cast<ArrayOfDocObjectBeatPairs *>((*params).at(1));

    // First we check if the object has already a mapped @endid (it should not)
    if (this->HasEndid()) {
        if (this->HasTstamp2())
            LogWarning("%s with @xml:id %s has both a @endid and an @tstamp2; @tstamp2 is ignored",
                object->GetClassName().c_str(), object->GetUuid().c_str());
        return TimePointInterface::InterfacePrepareTimestamps(params, object);
    }
    else if (!HasTstamp2()) {
        // We won't be able to do anything, just try to prepare the tstamp (start)
        return TimePointInterface::InterfacePrepareTimestamps(params, object);
    }

    // We can now add the pair to our stack
    tstamps->push_back(std::make_pair(object, data_MEASUREBEAT(this->GetTstamp2())));

    return TimePointInterface::InterfacePrepareTimestamps(params, object);
}

int TimeSpanningInterface::InterfaceFillStaffCurrentTimeSpanning(ArrayPtrVoid *params, DocObject *object)
{
    // param 0: std::vector<DocObject * >* of the current running TimeSpanningInterface elements
    std::vector<DocObject *> *elements = static_cast<std::vector<DocObject *> *>((*params).at(0));

    if (this->IsSpanningMeasures()) {
        elements->push_back(object);
    }
    return FUNCTOR_CONTINUE;
}

int TimeSpanningInterface::InterfaceResetDrawing(ArrayPtrVoid *params, DocObject *object)
{
    m_end = NULL;
    m_endUuid = "";
    // Special case where we have interface inheritance
    return TimePointInterface::InterfaceResetDrawing(params, object);
}

} // namespace vrv
