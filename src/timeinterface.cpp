/////////////////////////////////////////////////////////////////////////////
// Name:        timeinterface.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "timeinterface.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "harm.h"
#include "layerelement.h"
#include "measure.h"
#include "preparedatafunctor.h"
#include "staff.h"
#include "system.h"
#include "verticalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// TimePointInterface
//----------------------------------------------------------------------------

TimePointInterface::TimePointInterface() : Interface(), AttPartIdent(), AttStaffIdent(), AttStartId(), AttTimestampLog()
{
    this->RegisterInterfaceAttClass(ATT_PARTIDENT);
    this->RegisterInterfaceAttClass(ATT_STAFFIDENT);
    this->RegisterInterfaceAttClass(ATT_STARTID);
    this->RegisterInterfaceAttClass(ATT_TIMESTAMPLOG);

    this->Reset();
}

TimePointInterface::~TimePointInterface() {}

void TimePointInterface::Reset()
{
    this->ResetPartIdent();
    this->ResetStaffIdent();
    this->ResetStartId();
    this->ResetTimestampLog();

    m_start = NULL;
    m_startID = "";
}

void TimePointInterface::SetStart(LayerElement *start)
{
    assert(!m_start);
    m_start = start;
}

bool TimePointInterface::SetStartOnly(LayerElement *element)
{
    // LogDebug("%s - %s - %s", element->GetID().c_str(), m_startID.c_str(), m_endID.c_str() );
    if (!m_start && !m_startID.empty() && (element->GetID() == m_startID)) {
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

void TimePointInterface::SetIDStr()
{
    if (this->HasStartid()) {
        m_startID = ExtractIDFragment(this->GetStartid());
    }
}

Measure *TimePointInterface::GetStartMeasure()
{
    return const_cast<Measure *>(std::as_const(*this).GetStartMeasure());
}

const Measure *TimePointInterface::GetStartMeasure() const
{
    if (!m_start) return NULL;
    return vrv_cast<const Measure *>(m_start->GetFirstAncestor(MEASURE));
}

bool TimePointInterface::IsOnStaff(int n) const
{
    if (this->HasStaff()) {
        std::vector<int> staffList = this->GetStaff();
        for (int staffN : staffList) {
            if (staffN == n) return true;
        }
        return false;
    }
    else if (m_start) {
        const Staff *staff = m_start->GetAncestorStaff(ANCESTOR_ONLY, false);
        if (staff && (staff->GetN() == n)) return true;
    }
    return false;
}

std::vector<Staff *> TimePointInterface::GetTstampStaves(const Measure *measure, const Object *object)
{
    std::vector<const Staff *> staves = std::as_const(*this).GetTstampStaves(measure, object);
    std::vector<Staff *> stavesCasted;
    std::transform(staves.begin(), staves.end(), std::back_inserter(stavesCasted),
        [](const Staff *staff) { return const_cast<Staff *>(staff); });
    return stavesCasted;
}

std::vector<const Staff *> TimePointInterface::GetTstampStaves(const Measure *measure, const Object *object) const
{
    assert(measure);
    assert(object);

    std::vector<const Staff *> staves;
    std::vector<int> staffList;

    // For <f> within <harm> without @staff we try to get the @staff from the <harm> ancestor
    if (object->Is(FIGURE) && !this->HasStaff()) {
        const Harm *harm = vrv_cast<const Harm *>(object->GetFirstAncestor(HARM));
        if (harm) {
            staffList = harm->GetStaff();
        }
    }
    // For control elements with `@part="%all`, use the top visible staff indenpendently from the `@staff`
    else if (this->HasPart() && (this->GetPart() == "%all")) {
        const System *system = vrv_cast<const System *>(measure->GetFirstAncestor(SYSTEM));
        assert(system);
        const Staff *staff = system->GetTopVisibleStaff();
        if (staff) staffList.push_back(staff->GetN());
    }
    else if (this->HasStaff()) {
        bool isInBetween = false;
        // limit between support to some elements?
        if (object->Is({ DYNAM, DIR, HAIRPIN, TEMPO })) {
            const AttPlacementRelStaff *att = dynamic_cast<const AttPlacementRelStaff *>(object);
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
    else if (m_start && !m_start->Is({ BARLINE, TIMESTAMP_ATTR })) {
        const Staff *staff = m_start->GetAncestorStaff();
        staffList.push_back(staff->GetN());
    }
    else if (measure->GetChildCount(STAFF) == 1) {
        // If we have no @staff or startid but only one staff child assume it is the first one
        const Staff *staff = vrv_cast<const Staff *>(measure->GetFirst(STAFF));
        staffList.push_back(staff->GetN());
    }

    for (int staffN : staffList) {
        AttNIntegerComparison comparison(STAFF, staffN);
        const Staff *staff = vrv_cast<const Staff *>(measure->FindDescendantByComparison(&comparison, 1));
        if (!staff) {
            // LogDebug("Staff with @n '%d' not found in measure '%s'", *iter, measure->GetID().c_str());
            continue;
        }
        if (!staff->DrawingIsVisible()) {
            continue;
        }
        staves.push_back(staff);
    }
    return staves;
}

bool TimePointInterface::VerifyMeasure(const Object *owner) const
{
    assert(owner);
    if (m_start && (owner->GetFirstAncestor(MEASURE) != this->GetStartMeasure())) {
        LogWarning("%s '%s' is not encoded in the measure of its start '%s'. This may cause improper rendering.",
            owner->GetClassName().c_str(), owner->GetID().c_str(), m_start->GetID().c_str());
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------
// TimeSpanningInterface
//----------------------------------------------------------------------------

TimeSpanningInterface::TimeSpanningInterface() : TimePointInterface(), AttStartEndId(), AttTimestamp2Log()
{
    this->RegisterInterfaceAttClass(ATT_STARTENDID);
    this->RegisterInterfaceAttClass(ATT_TIMESTAMP2LOG);

    this->Reset();
}

TimeSpanningInterface::~TimeSpanningInterface() {}

void TimeSpanningInterface::Reset()
{
    TimePointInterface::Reset();
    this->ResetStartEndId();
    this->ResetTimestamp2Log();

    m_end = NULL;
    m_endID = "";
}

void TimeSpanningInterface::SetEnd(LayerElement *end)
{
    assert(!m_end);
    m_end = end;
}

void TimeSpanningInterface::SetIDStr()
{
    TimePointInterface::SetIDStr();
    if (this->HasEndid()) {
        m_endID = ExtractIDFragment(this->GetEndid());
    }
}

bool TimeSpanningInterface::SetStartAndEnd(LayerElement *element)
{
    // LogDebug("%s - %s - %s", element->GetID().c_str(), m_startID.c_str(), m_endID.c_str() );
    if (!m_start && !m_startID.empty() && (element->GetID() == m_startID)) {
        this->SetStart(element);
    }
    else if (!m_end && !m_endID.empty() && (element->GetID() == m_endID)) {
        this->SetEnd(element);
    }
    return (m_start && m_end);
}

Measure *TimeSpanningInterface::GetEndMeasure()
{
    return const_cast<Measure *>(std::as_const(*this).GetEndMeasure());
}

const Measure *TimeSpanningInterface::GetEndMeasure() const
{
    if (!m_end) return NULL;
    return vrv_cast<const Measure *>(m_end->GetFirstAncestor(MEASURE));
}

bool TimeSpanningInterface::IsSpanningMeasures() const
{
    if (!this->HasStartAndEnd()) return false;
    return (this->GetStartMeasure() != this->GetEndMeasure());
}

bool TimeSpanningInterface::IsOrdered() const
{
    return this->IsOrdered(m_start, m_end);
}

bool TimeSpanningInterface::IsOrdered(const LayerElement *start, const LayerElement *end) const
{
    if (!start || !end) return true;
    const Measure *startMeasure = vrv_cast<const Measure *>(start->GetFirstAncestor(MEASURE));
    const Measure *endMeasure = vrv_cast<const Measure *>(end->GetFirstAncestor(MEASURE));

    if (startMeasure == endMeasure) {
        if (!start->GetAlignment() || !end->GetAlignment()) return true;
        return Object::IsPreOrdered(start->GetAlignment(), end->GetAlignment());
    }
    else {
        return (startMeasure->GetIndex() < endMeasure->GetIndex());
    }
}

void TimeSpanningInterface::GetCrossStaffOverflows(
    StaffAlignment *alignment, curvature_CURVEDIR curveDir, bool &skipAbove, bool &skipBelow)
{
    assert(alignment);

    if (!this->GetStart() || !this->GetEnd() || !alignment->GetStaff()) return;

    // We cannot have cross-staff slurs only with timestamps
    if (this->GetStart()->Is(TIMESTAMP_ATTR) && this->GetEnd()->Is(TIMESTAMP_ATTR)) return;

    Layer *layer = NULL;

    // If the starting point is a chord we need to select the appropriate extreme staff
    Staff *startStaff = NULL;
    if (this->GetStart()->Is(CHORD)) {
        Chord *chord = vrv_cast<Chord *>(this->GetStart());
        assert(chord);
        // First check if the chord itself is cross-staff
        startStaff = chord->GetCrossStaff(layer);
        if (!startStaff) {
            // If not look at its content
            Staff *staffAbove = NULL;
            Staff *staffBelow = NULL;
            chord->GetCrossStaffExtremes(staffAbove, staffBelow);
            startStaff = (curveDir == curvature_CURVEDIR_above) ? staffAbove : staffBelow;
        }
    }
    else {
        startStaff = this->GetStart()->GetCrossStaff(layer);
    }

    // Same for the end point
    Staff *endStaff = NULL;
    if (this->GetEnd()->Is(CHORD)) {
        Chord *chord = vrv_cast<Chord *>(this->GetEnd());
        assert(chord);
        // First check if the chord itself is cross-staff
        endStaff = chord->GetCrossStaff(layer);
        if (!endStaff) {
            // If not look at its content
            Staff *staffAbove = NULL;
            Staff *staffBelow = NULL;
            chord->GetCrossStaffExtremes(staffAbove, staffBelow);
            endStaff = (curveDir == curvature_CURVEDIR_above) ? staffAbove : staffBelow;
        }
    }
    else {
        endStaff = this->GetEnd()->GetCrossStaff(layer);
    }

    // No cross-staff endpoints, check if the slur itself crosses staves
    if (!startStaff) {
        startStaff = this->GetStart()->GetAncestorStaff(ANCESTOR_ONLY, false);
    }
    if (!endStaff) {
        endStaff = this->GetEnd()->GetAncestorStaff(ANCESTOR_ONLY, false);
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

FunctorCode TimePointInterface::InterfacePrepareTimePointing(PrepareTimePointingFunctor &functor, Object *object)
{
    if (!this->HasStartid()) return FUNCTOR_CONTINUE;

    this->SetIDStr();
    functor.InsertInterfaceIDTuple(object->GetClassId(), this);

    return FUNCTOR_CONTINUE;
}

FunctorCode TimePointInterface::InterfacePrepareTimestamps(PrepareTimestampsFunctor &functor, Object *object)
{
    // First we check if the object has already a mapped @startid (it should not)
    if (this->HasStart()) {
        if (this->HasTstamp())
            LogWarning("%s with @xml:id %s has both a @startid and an @tstamp; @tstamp is ignored",
                object->GetClassName().c_str(), object->GetID().c_str());
        return FUNCTOR_CONTINUE;
    }
    else if (!this->HasTstamp()) {
        return FUNCTOR_CONTINUE; // This file is quite likely invalid?
    }

    // We set -1 to the data_MEASUREBEAT for @tstamp
    functor.InsertObjectBeatPair(object, data_MEASUREBEAT(-1, this->GetTstamp()));

    return FUNCTOR_CONTINUE;
}

FunctorCode TimePointInterface::InterfaceResetData(ResetDataFunctor &functor, Object *object)
{
    m_start = NULL;
    m_startID = "";
    return FUNCTOR_CONTINUE;
}

FunctorCode TimeSpanningInterface::InterfacePrepareTimeSpanning(PrepareTimeSpanningFunctor &functor, Object *object)
{
    if (!this->HasStartid() && !this->HasEndid()) {
        return FUNCTOR_CONTINUE;
    }

    if (functor.IsProcessingData()) {
        return FUNCTOR_CONTINUE;
    }

    this->SetIDStr();
    functor.InsertInterfaceOwnerPair(object, this);

    return FUNCTOR_CONTINUE;
}

FunctorCode TimeSpanningInterface::InterfacePrepareTimestamps(PrepareTimestampsFunctor &functor, Object *object)
{
    // First we check if the object has already a mapped @endid (it should not)
    if (this->HasEndid()) {
        if (this->HasTstamp2())
            LogWarning("%s with @xml:id %s has both a @endid and an @tstamp2; @tstamp2 is ignored",
                object->GetClassName().c_str(), object->GetID().c_str());
        if ((this->GetStartid() == this->GetEndid()) && !object->Is(OCTAVE)) {
            LogWarning("%s with @xml:id %s will not get rendered as it has identical values in @startid and @endid",
                object->GetClassName().c_str(), object->GetID().c_str());
        }
        return TimePointInterface::InterfacePrepareTimestamps(functor, object);
    }
    else if (!this->HasTstamp2()) {
        // We won't be able to do anything, just try to prepare the tstamp (start)
        return TimePointInterface::InterfacePrepareTimestamps(functor, object);
    }

    // We can now add the pair to our stack
    functor.InsertInterfaceIDPair(object->GetClassId(), this);
    functor.InsertObjectBeatPair(object, data_MEASUREBEAT(this->GetTstamp2()));

    return TimePointInterface::InterfacePrepareTimestamps(functor, object);
}

FunctorCode TimeSpanningInterface::InterfacePrepareStaffCurrentTimeSpanning(
    PrepareStaffCurrentTimeSpanningFunctor &functor, Object *object)
{
    if (this->IsSpanningMeasures()) {
        functor.InsertTimeSpanningElement(object);
    }
    return FUNCTOR_CONTINUE;
}

FunctorCode TimeSpanningInterface::InterfaceResetData(ResetDataFunctor &functor, Object *object)
{
    m_end = NULL;
    m_endID = "";
    // Special case where we have interface inheritance
    return TimePointInterface::InterfaceResetData(functor, object);
}

} // namespace vrv
