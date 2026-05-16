/////////////////////////////////////////////////////////////////////////////
// Name:        zone.cpp
// Author:      Juliette Regimbal
// Created:     05/06/2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "zone.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "functor.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Zone
//----------------------------------------------------------------------------

static const ClassRegistrar<Zone> s_factory("zone", ZONE);

Zone::Zone() : Object(ZONE), AttTyped(), AttCoordinated()
{
    this->RegisterAttClass(ATT_TYPED);
    this->RegisterAttClass(ATT_COORDINATED);
    this->RegisterAttClass(ATT_COORDINATEDUL);
    this->Reset();
}

Zone::~Zone() {}

void Zone::Reset()
{
    this->ResetTyped();
    this->ResetCoordinated();
    this->ResetCoordinatedUl();
}

void Zone::ShiftByXY(int xDiff, int yDiff)
{
    this->SetUlx(this->GetUlx() + xDiff);
    this->SetLrx(this->GetLrx() + xDiff);
    this->SetUly(this->GetUly() + yDiff);
    this->SetLry(this->GetLry() + yDiff);
}

int Zone::GetLogicalUly() const
{
    return (this->GetUly());
}

int Zone::GetLogicalLry() const
{
    return (this->GetLry());
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode Zone::Accept(Functor &functor)
{
    return functor.VisitZone(this);
}

FunctorCode Zone::Accept(ConstFunctor &functor) const
{
    return functor.VisitZone(this);
}

FunctorCode Zone::AcceptEnd(Functor &functor)
{
    return functor.VisitZoneEnd(this);
}

FunctorCode Zone::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitZoneEnd(this);
}

} // namespace vrv
