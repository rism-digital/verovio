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

#include "comparison.h"
#include "functorparams.h"
#include "layerelement.h"
#include "measure.h"
#include "staff.h"
#include "verticalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// TimePointInterface
//----------------------------------------------------------------------------

TimePointInterface::TimePointInterface() : Interface(), AttStaffIdent(), AttStartId(), AttTimestampLogical()
{
    RegisterInterfaceAttClass(ATT_STAFFIDENT);
    RegisterInterfaceAttClass(ATT_STARTID);
    RegisterInterfaceAttClass(ATT_TIMESTAMPLOGICAL);

    Reset();
}

TimePointInterface::~TimePointInterface() {}

void TimePointInterface::Reset()
{
    ResetStaffIdent();
    ResetStartId();
    ResetTimestampLogical();

    m_start = NULL;
    m_startUuid = "";
}

void TimePointInterface::SetStart(LayerElement *start)
{
    assert(!m_start);
    m_start = start;
}

bool TimePointInterface::SetStartOnly(LayerElement *element)
{
    // LogDebug("%s - %s - %s", element->GetUuid().c_str(), m_startUuid.c_str(), m_endUuid.c_str() );
    if (!m_start && !m_startUuid.empty() && (element->GetUuid() == m_startUuid)) {
        this->SetStart(element);
        return true;
    }
    return false;
}

void TimePointInterface::AddStaff(int n)
{
    xsdPositiveInteger_List staves = this->GetStaff();
    if (std::find(staves.begin(), staves.end(), n) == staves.end()) {
        staves.push_back(n);
        this->SetStaff(staves);
    }
}

void TimePointInterface::SetUuidStr()
{
    if (this->HasStartid()) {
        m_startUuid = ExtractUuidFragment(this->GetStartid());
    }
}

Measure *TimePointInterface::GetStartMeasure()
{
    if (!m_start) return NULL;
    return dynamic_cast<Measure *>(this->m_start->GetFirstAncestor(MEASURE));
}

bool TimePointInterface::IsOnStaff(int n)
{
    if (this->HasStaff()) {
        std::vector<int> staffList = this->GetStaff();
        std::vector<int>::iterator iter;
        for (iter = staffList.begin(); iter != staffList.end(); ++iter) {
            if (*iter == n) return true;
        }
        return false;
    }
    else if (m_start) {
        Staff *staff = dynamic_cast<Staff *>(m_start->GetFirstAncestor(STAFF));
        if (staff && (staff->GetN() == n)) return true;
    }
    return false;
}

std::vector<Staff *> TimePointInterface::GetTstampStaves(Measure *measure, Object *object)
{
    assert(measure);
    assert(object);

    std::vector<Staff *> staves;
    std::vector<int>::iterator iter;
    std::vector<int> staffList;
    if (this->HasStaff()) {
        bool isInBetween = false;
        // limit between support to some elements?
        if (object->Is({ DYNAM, DIR, HAIRPIN, TEMPO })) {
            AttPlacement *att = dynamic_cast<AttPlacement *>(object);
            assert(att);
            isInBetween = (att->GetPlace() == STAFFREL_between);
        }
        if (isInBetween) {
            assert(this->GetStaff().size() > 0);
            // With @place="between" we use only the first staff value
            staffList.push_back(this->GetStaff().front());
        }
        else {
            staffList = this->GetStaff();
        }
    }
    else if (m_start && !m_start->Is(TIMESTAMP_ATTR)) {
        Staff *staff = dynamic_cast<Staff *>(m_start->GetFirstAncestor(STAFF));
        if (staff) staffList.push_back(staff->GetN());
    }
    else if (measure->GetChildCount(STAFF) == 1) {
        // If we have no @staff or startid but only one staff child assume it is the first one (@n1 is assumed)
        staffList.push_back(1);
    }
    for (iter = staffList.begin(); iter != staffList.end(); ++iter) {
        AttNIntegerComparison comparison(STAFF, *iter);
        Staff *staff = dynamic_cast<Staff *>(measure->FindDescendantByComparison(&comparison, 1));
        if (!staff) {
            // LogDebug("Staff with @n '%d' not found in measure '%s'", *iter, measure->GetUuid().c_str());
            continue;
        }
        if (!staff->DrawingIsVisible()) {
            continue;
        }
        staves.push_back(staff);
    }
    return staves;
}

//----------------------------------------------------------------------------
// TimeSpanningInterface
//----------------------------------------------------------------------------

TimeSpanningInterface::TimeSpanningInterface() : TimePointInterface(), AttStartEndId(), AttTimestamp2Logical()
{
    RegisterInterfaceAttClass(ATT_STARTENDID);
    RegisterInterfaceAttClass(ATT_TIMESTAMP2LOGICAL);

    Reset();
}

TimeSpanningInterface::~TimeSpanningInterface() {}

void TimeSpanningInterface::Reset()
{
    TimePointInterface::Reset();
    ResetStartEndId();
    ResetTimestamp2Logical();

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
        m_endUuid = ExtractUuidFragment(this->GetEndid());
    }
}

bool TimeSpanningInterface::SetStartAndEnd(LayerElement *element)
{
    // LogDebug("%s - %s - %s", element->GetUuid().c_str(), m_startUuid.c_str(), m_endUuid.c_str() );
    if (!m_start && !m_startUuid.empty() && (element->GetUuid() == m_startUuid)) {
        this->SetStart(element);
    }
    else if (!m_end && !m_endUuid.empty() && (element->GetUuid() == m_endUuid)) {
        this->SetEnd(element);
    }
    return (m_start && m_end);
}

Measure *TimeSpanningInterface::GetEndMeasure()
{
    if (!m_end) return NULL;
    return dynamic_cast<Measure *>(this->m_end->GetFirstAncestor(MEASURE));
}

bool TimeSpanningInterface::IsSpanningMeasures()
{
    if (!this->HasStartAndEnd()) return false;
    return (this->GetStartMeasure() != this->GetEndMeasure());
}

void TimeSpanningInterface::GetCrossStaffOverflows(
    StaffAlignment *alignment, curvature_CURVEDIR cuvreDir, bool &skipAbove, bool &skipBelow)
{
    assert(alignment);

    if (!this->GetStart() || !this->GetEnd() || !alignment->GetStaff()) return;

    Layer *layer = NULL;

    // If the starting point is a chord we need to select the appropriate extreme staff
    Staff *startStaff = NULL;
    if (this->GetStart()->Is(CHORD)) {
        Chord *chord = vrv_cast<Chord *>(this->GetStart());
        assert(chord);
        Staff *staffAbove = NULL;
        Staff *staffBelow = NULL;
        chord->GetCrossStaffExtremes(staffAbove, staffBelow);
        startStaff = (cuvreDir == curvature_CURVEDIR_above) ? staffAbove : staffBelow;
    }
    else
        startStaff = this->GetStart()->GetCrossStaff(layer);

    // Same for the end point
    Staff *endStaff = NULL;
    if (this->GetEnd()->Is(CHORD)) {
        Chord *chord = vrv_cast<Chord *>(this->GetEnd());
        assert(chord);
        Staff *staffAbove = NULL;
        Staff *staffBelow = NULL;
        chord->GetCrossStaffExtremes(staffAbove, staffBelow);
        endStaff = (cuvreDir == curvature_CURVEDIR_above) ? staffAbove : staffBelow;
    }
    else {
        endStaff = this->GetEnd()->GetCrossStaff(layer);
    }

    // No cross-staff endpoints, check if the slur itself crosses staves
    if (!startStaff) {
        startStaff = dynamic_cast<Staff *>(this->GetStart()->GetFirstAncestor(STAFF));
    }
    if (!endStaff) {
        endStaff = dynamic_cast<Staff *>(this->GetEnd()->GetFirstAncestor(STAFF));
    }

    // This happens with slurs starting or ending with a timestamp
    if (!endStaff) {
        endStaff = startStaff;
    }
    else if (!startStaff) {
        startStaff = endStaff;
    }
    assert(startStaff && endStaff);

    if (startStaff && (startStaff->GetN() < alignment->GetStaff()->GetN())) skipAbove = true;
    if (endStaff && (endStaff->GetN() < alignment->GetStaff()->GetN())) skipAbove = true;

    if (startStaff && (startStaff->GetN() > alignment->GetStaff()->GetN())) skipBelow = true;
    if (endStaff && (endStaff->GetN() > alignment->GetStaff()->GetN())) skipBelow = true;
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

int TimePointInterface::InterfacePrepareTimestamps(FunctorParams *functorParams, Object *object)
{
    PrepareTimestampsParams *params = vrv_params_cast<PrepareTimestampsParams *>(functorParams);
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

int TimePointInterface::InterfaceResetDrawing(FunctorParams *functorParams, Object *object)
{
    m_start = NULL;
    m_startUuid = "";
    return FUNCTOR_CONTINUE;
}

int TimePointInterface::InterfacePrepareTimePointing(FunctorParams *functorParams, Object *object)
{
    PrepareTimePointingParams *params = vrv_params_cast<PrepareTimePointingParams *>(functorParams);
    assert(params);

    if (!this->HasStartid()) return FUNCTOR_CONTINUE;

    this->SetUuidStr();
    params->m_timePointingInterfaces.push_back(std::make_pair(this, object->GetClassId()));

    return FUNCTOR_CONTINUE;
}

int TimeSpanningInterface::InterfacePrepareTimeSpanning(FunctorParams *functorParams, Object *object)
{
    PrepareTimeSpanningParams *params = vrv_params_cast<PrepareTimeSpanningParams *>(functorParams);
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
    PrepareTimestampsParams *params = vrv_params_cast<PrepareTimestampsParams *>(functorParams);
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
    params->m_timeSpanningInterfaces.push_back(std::make_pair(this, object->GetClassId()));
    params->m_tstamps.push_back(std::make_pair(object, data_MEASUREBEAT(this->GetTstamp2())));

    return TimePointInterface::InterfacePrepareTimestamps(params, object);
}

int TimeSpanningInterface::InterfaceFillStaffCurrentTimeSpanning(FunctorParams *functorParams, Object *object)
{
    FillStaffCurrentTimeSpanningParams *params = vrv_params_cast<FillStaffCurrentTimeSpanningParams *>(functorParams);
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

} // namespace vrv
