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

    m_isFirst = true;
    this->ResetDrawingStaffDefs();
}

void Ossia::Reset()
{
    Object::Reset();
    this->ResetTyped();

    m_isFirst = true;
    this->ResetDrawingStaffDefs();
}

void Ossia::ResetDrawingStaffDefs()
{
    for (const auto &[key, value] : m_drawingStaffDefs) {
        delete value;
    }
    m_drawingStaffDefs.clear();
}

void Ossia::SetDrawingStaffDef(StaffDef *drawingStaffDef)
{
    assert(drawingStaffDef);
    assert(!m_drawingStaffDefs.contains(drawingStaffDef->GetN()));
    m_drawingStaffDefs[drawingStaffDef->GetN()] = drawingStaffDef;
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

const Staff *Ossia::GetOriginalStaffForOssia(const Staff *ossia) const
{
    AttNIntegerComparison staffN(STAFF, ossia->GetN() - OSSIA_N_OFFSET);
    const Staff *staff = vrv_cast<const Staff *>(this->FindDescendantByComparison(&staffN));
    if (!staff) {
        LogDebug("Orignal ossia staff %d could not be found", ossia->GetN() - OSSIA_N_OFFSET);
    }
    return staff;
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
