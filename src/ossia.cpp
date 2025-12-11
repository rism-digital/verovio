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
#include <regex>

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
    this->ResetAlignments();
}

void Ossia::Reset()
{
    Object::Reset();
    this->ResetTyped();

    m_isFirst = true;
    this->ResetDrawingStaffDefs();
    this->ResetAlignments();
}

void Ossia::ResetDrawingStaffDefs()
{
    for (const auto staffDef : m_drawingStaffDefs) {
        delete staffDef;
    }
    m_drawingStaffDefs.clear();
}

void Ossia::SetDrawingStaffDef(StaffDef *drawingStaffDef)
{
    assert(drawingStaffDef);
    m_drawingStaffDefs.push_back(drawingStaffDef);
}

bool Ossia::HasShowScoreDef() const
{
    static const std::regex re("show.scoredef.*");
    return (this->HasType() && std::regex_match(this->GetType(), re));
}

data_BOOLEAN Ossia::GetShowScoreDef() const
{
    static const std::regex reTrue("show.scoredef.true");
    static const std::regex reFalse("show.scoredef.false");
    if (this->HasType() && std::regex_match(this->GetType(), reTrue)) return BOOLEAN_true;
    if (this->HasType() && std::regex_match(this->GetType(), reFalse)) return BOOLEAN_false;
    return BOOLEAN_NONE;
}

bool Ossia::HasMultipleOStaves() const
{
    int count = 0;
    ListOfConstObjects staves = this->FindAllDescendantsByType(STAFF);
    for (auto it = staves.begin(); it != staves.end(); ++it) {
        const Staff *staff = vrv_cast<const Staff *>(*it);
        assert(staff);
        if (staff->IsOssia()) count++;
        if (count > 1) return true;
    }
    return false;
}

bool Ossia::DrawScoreDef() const
{
    if (!this->HasShowScoreDef()) return this->HasMultipleOStaves();
    return (this->GetShowScoreDef() == BOOLEAN_true);
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

void Ossia::ResetAlignments()
{
    m_clefAlignment = NULL;
    m_keySigAlignment = NULL;
}

void Ossia::GetStavesAbove(MapOfOssiaStaffNs &map) const
{
    ListOfConstObjects staves = this->FindAllDescendantsByType(STAFF);

    ListOfConstObjects stavesReversed;
    stavesReversed.resize(staves.size());
    std::reverse_copy(staves.begin(), staves.end(), stavesReversed.begin());

    this->GetStaves(map, stavesReversed);
}

int Ossia::GetScoreDefShift() const
{
    // The clef is the further appart
    if (m_clefAlignment) return m_clefAlignment->GetXRel();
    // Other wise the key signature
    if (m_keySigAlignment) return m_keySigAlignment->GetXRel();
    return 0;
}

void Ossia::GetStavesBelow(MapOfOssiaStaffNs &map) const
{
    ListOfConstObjects staves = this->FindAllDescendantsByType(STAFF);

    this->GetStaves(map, staves);
}

const Staff *Ossia::GetTopOStaff() const
{
    if (m_drawingStaffDefs.empty()) return NULL;
    const int staffN = m_drawingStaffDefs.front()->GetN();
    AttNIntegerComparison n(STAFF, staffN);
    return vrv_cast<const Staff *>(this->FindDescendantByComparison(&n));
}

const Staff *Ossia::GetBottopOStaff() const
{
    if (m_drawingStaffDefs.empty()) return NULL;
    const int staffN = m_drawingStaffDefs.back()->GetN();
    AttNIntegerComparison n(STAFF, staffN);
    return vrv_cast<const Staff *>(this->FindDescendantByComparison(&n));
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
