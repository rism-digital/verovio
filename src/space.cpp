/////////////////////////////////////////////////////////////////////////////
// Name:        space.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "space.h"
//----------------------------------------------------------------------------

#include <algorithm>
#include <cassert>
#include <math.h>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "functor.h"
#include "horizontalaligner.h"
#include "rest.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Space
//----------------------------------------------------------------------------

static const ClassRegistrar<Space> s_factory("space", SPACE);

Space::Space() : LayerElement(SPACE), DurationInterface()
{
    this->RegisterInterface(DurationInterface::GetAttClasses(), DurationInterface::IsInterface());

    this->Reset();
}

Space::~Space() {}

void Space::Reset()
{
    LayerElement::Reset();
    DurationInterface::Reset();
}

bool Space::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ REFRAIN, REST, VERSE };

    return (std::find(supported.begin(), supported.end(), classId) != supported.end());
}

void Space::InitShowSpace()
{
    // Remove only the placeholder rest from a previous call and keep lyrics
    IsAttributeComparison isAttribute(REST);
    this->DeleteChildrenByComparison(&isAttribute);
    Rest *rest = new Rest();
    rest->IsAttribute(true);
    rest->SetDur(this->GetDur());
    rest->SetDots(this->GetDots());
    if (!this->AddChild(rest)) delete rest;
}

bool Space::HasLyrics() const
{
    ClassIdsComparison lyricElements({ REFRAIN, VERSE });
    return (this->FindDescendantByComparison(&lyricElements, 1) != NULL);
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

FunctorCode Space::Accept(Functor &functor)
{
    return functor.VisitSpace(this);
}

FunctorCode Space::Accept(ConstFunctor &functor) const
{
    return functor.VisitSpace(this);
}

FunctorCode Space::AcceptEnd(Functor &functor)
{
    return functor.VisitSpaceEnd(this);
}

FunctorCode Space::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitSpaceEnd(this);
}

} // namespace vrv
