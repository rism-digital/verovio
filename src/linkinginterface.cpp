/////////////////////////////////////////////////////////////////////////////
// Name:        linkinginterface.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "linkinginterface.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "measure.h"
#include "preparedatafunctor.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// LinkingInterface
//----------------------------------------------------------------------------

LinkingInterface::LinkingInterface() : Interface(), AttLinking()
{
    this->RegisterInterfaceAttClass(ATT_LINKING);

    this->Reset();
}

LinkingInterface::~LinkingInterface() {}

void LinkingInterface::Reset()
{
    this->ResetLinking();

    m_next = NULL;
    m_nextID = "";
    m_sameas = NULL;
    m_sameasID = "";
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

void LinkingInterface::SetIDStr()
{
    if (this->HasNext()) {
        m_nextID = ExtractIDFragment(this->GetNext());
    }
    if (this->HasSameas()) {
        m_sameasID = ExtractIDFragment(this->GetSameas());
    }
}

Measure *LinkingInterface::GetNextMeasure()
{
    return const_cast<Measure *>(std::as_const(*this).GetNextMeasure());
}

const Measure *LinkingInterface::GetNextMeasure() const
{
    if (!m_next) return NULL;
    return vrv_cast<const Measure *>(m_next->GetFirstAncestor(MEASURE));
}

void LinkingInterface::AddBackLink(const Object *object)
{
    const LinkingInterface *linking = object->GetLinkingInterface();
    std::string corresp = "#" + object->GetID();
    if (linking && linking->HasCorresp()) {
        corresp = linking->GetCorresp();
    }
    this->SetCorresp(corresp.c_str());
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

FunctorCode LinkingInterface::InterfacePrepareLinking(PrepareLinkingFunctor &functor, Object *object)
{
    // This should not happen?
    if (functor.IsProcessingData()) {
        return FUNCTOR_CONTINUE;
    }

    this->SetIDStr();

    if (!m_nextID.empty()) {
        functor.InsertNextIDPair(m_nextID, this);
    }
    if (!m_sameasID.empty()) {
        functor.InsertSameasIDPair(m_sameasID, this);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode LinkingInterface::InterfacePrepareStaffCurrentTimeSpanning(
    PrepareStaffCurrentTimeSpanningFunctor &functor, Object *object)
{
    // Only dir and dynam can be spanning with @next (extender)
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

    functor.InsertTimeSpanningElement(object);

    return FUNCTOR_CONTINUE;
}

FunctorCode LinkingInterface::InterfaceResetData(ResetDataFunctor &functor, Object *object)
{
    m_next = NULL;
    m_nextID = "";
    m_sameas = NULL;
    m_sameasID = "";
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
