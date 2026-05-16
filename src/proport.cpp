/////////////////////////////////////////////////////////////////////////////
// Name:        proport.cpp
// Author:      Donald Byrd
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "proport.h"

//----------------------------------------------------------------------------

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// Proport
//----------------------------------------------------------------------------

static const ClassRegistrar<Proport> s_factory("proport", PROPORT);

Proport::Proport() : LayerElement(PROPORT), AttDurationRatio()
{
    this->RegisterAttClass(ATT_DURATIONRATIO);

    this->Reset();
}

Proport::~Proport() {}

void Proport::Reset()
{
    LayerElement::Reset();
    this->ResetDurationRatio();

    m_cumulatedNum = VRV_UNSET;
    m_cumulatedNumbase = VRV_UNSET;
}

int Proport::GetCumulatedNum() const
{
    return (m_cumulatedNum != VRV_UNSET) ? m_cumulatedNum : this->GetNum();
}

int Proport::GetCumulatedNumbase() const
{
    return (m_cumulatedNumbase != VRV_UNSET) ? m_cumulatedNumbase : this->GetNumbase();
}

void Proport::Cumulate(const Proport *proport)
{
    // Reset type proportion - do not cumulate
    if (this->GetType() == "reset") return;
    // Potential reset (tempo change) in CMME - do not cumulate
    if (this->GetType() == "reset?") return;

    // Unset values are not cumulated
    if (proport->HasNum() && this->HasNum()) {
        m_cumulatedNum = this->GetNum() * proport->GetCumulatedNum();
    }
    if (proport->HasNumbase() && this->HasNumbase()) {
        m_cumulatedNumbase = this->GetNumbase() * proport->GetCumulatedNumbase();
    }
    if ((m_cumulatedNum != VRV_UNSET) && (m_cumulatedNumbase != VRV_UNSET)) {
        Fraction::Reduce(m_cumulatedNum, m_cumulatedNumbase);
    }
}

void Proport::ResetCumulate()
{
    m_cumulatedNum = VRV_UNSET;
    m_cumulatedNumbase = VRV_UNSET;
}

FunctorCode Proport::Accept(Functor &functor)
{
    return functor.VisitProport(this);
}

FunctorCode Proport::Accept(ConstFunctor &functor) const
{
    return functor.VisitProport(this);
}

FunctorCode Proport::AcceptEnd(Functor &functor)
{
    return functor.VisitProportEnd(this);
}

FunctorCode Proport::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitProportEnd(this);
}

} // namespace vrv
