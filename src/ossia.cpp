/////////////////////////////////////////////////////////////////////////////
// Name:        ossia.cpp
// Author:      Klaus Rettinghaus
// Created:     2025
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "ossia.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <cassert>
#include <math.h>

//----------------------------------------------------------------------------

#include "functor.h"
#include "object.h"
#include "staff.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Ossia
//----------------------------------------------------------------------------

static const ClassRegistrar<Ossia> s_factory("ossia", OSSIA);

Ossia::Ossia() : Object(OSSIA), AttTyped()
{
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

Ossia::~Ossia()
{
    // We need to delete own objects
    this->Reset();
}

void Ossia::CloneReset()
{
    Object::CloneReset();
}

void Ossia::Reset()
{
    Object::Reset();
    this->ResetTyped();
}

bool Ossia::IsSupportedChild(ClassId classId)
{
    // Include pseudo classId for check from the object factory
    static const std::vector<ClassId> supported{ STAFF, FACTORY_OSTAFF };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else if (Object::IsEditorialElement(classId)) {
        return true;
    }
    else {
        return false;
    }
}

void Ossia::GetStavesAbove(MapOfOssiaStaffNs &map) const
{
    ListOfConstObjects staves = this->FindAllDescendantsByType(STAFF);

    ListOfConstObjects stavesReversed;
    stavesReversed.resize(staves.size());
    std::reverse_copy(staves.begin(), staves.end(), stavesReversed.begin());

    this->GetStaves(map, stavesReversed);
}

void Ossia::GetStavesBelow(MapOfOssiaStaffNs &map) const
{
    ListOfConstObjects staves = this->FindAllDescendantsByType(STAFF);

    this->GetStaves(map, staves);
}

void Ossia::GetStaves(MapOfOssiaStaffNs &map, ListOfConstObjects &staves) const
{
    int staffN = VRV_UNSET;
    for (auto it = staves.begin(); it != staves.end(); ++it) {
        const Staff *staff = vrv_cast<const Staff *>(*it);
        assert(staff);
        if (!staff->IsOssia()) {
            staffN = staff->GetN();
            continue;
        }
        if (staffN != VRV_UNSET) {
            std::list<int> &ossias = map[staffN];
            int ossiaN = staff->GetN();
            auto found = std::find(ossias.begin(), ossias.end(), ossiaN);
            // add only if not already present
            if (found == ossias.end()) ossias.push_back(ossiaN);
        }
    }
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode Ossia::Accept(Functor &functor)
{
    return functor.VisitOssia(this);
}

FunctorCode Ossia::Accept(ConstFunctor &functor) const
{
    return functor.VisitOssia(this);
}

FunctorCode Ossia::AcceptEnd(Functor &functor)
{
    return functor.VisitOssiaEnd(this);
}

FunctorCode Ossia::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitOssiaEnd(this);
}

} // namespace vrv
