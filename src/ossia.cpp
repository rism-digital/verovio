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

    m_drawingLeftBarLine.SetForm(BARRENDITION_single);
    m_drawingStaffGrp.SetParent(this);

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

    this->ResetDrawingStaffGrp();
    this->ResetAlignments();
}

void Ossia::Reset()
{
    Object::Reset();
    this->ResetTyped();

    this->ResetDrawingStaffGrp();
    this->ResetAlignments();
}

void Ossia::ResetDrawingStaffGrp()
{
    m_drawingStaffGrp.Reset();
    m_isFirst = true;
    m_isLast = true;
}

void Ossia::AddDrawingStaffDef(StaffDef *drawingStaffDef)
{
    assert(drawingStaffDef);
    m_drawingStaffGrp.AddChild(drawingStaffDef);
}

bool Ossia::HasShowScoreDef() const
{
    static const std::regex re(R"(\bshow\.scoredef\.(true|false)\b)");
    return (this->HasType() && std::regex_search(this->GetType(), re));
}

data_BOOLEAN Ossia::GetShowScoreDef() const
{
    static const std::regex reTrue(R"(\bshow\.scoredef\.true\b)");
    static const std::regex reFalse(R"(\bshow\.scoredef\.false\b)");
    if (this->HasType() && std::regex_search(this->GetType(), reTrue)) return BOOLEAN_true;
    if (this->HasType() && std::regex_search(this->GetType(), reFalse)) return BOOLEAN_false;
    return BOOLEAN_NONE;
}

bool Ossia::HasShowBarLines() const
{
    static const std::regex re(R"(\bshow\.barlines\.(true|false)\b)");
    return (this->HasType() && std::regex_search(this->GetType(), re));
}

data_BOOLEAN Ossia::GetShowBarLines() const
{
    static const std::regex reTrue(R"(\bshow\.barlines\.true\b)");
    static const std::regex reFalse(R"(\bshow\.barlines\.false\b)");
    if (this->HasType() && std::regex_search(this->GetType(), reTrue)) return BOOLEAN_true;
    if (this->HasType() && std::regex_search(this->GetType(), reFalse)) return BOOLEAN_false;
    return BOOLEAN_NONE;
}

bool Ossia::HasMultipleOStaves() const
{
    int count = 0;
    ListOfConstObjects staves = this->FindAllDescendantsByType(STAFF);
    for (auto it = staves.begin(); it != staves.end(); ++it) {
        const Staff *staff = vrv_cast<const Staff *>(*it);
        assert(staff);
        if (staff->IsOssia() && !staff->IsHidden()) count++;
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
    AttNIntegerComparison staffN(STAFF, ossia->GetNFromOssia());
    const Staff *staff = vrv_cast<const Staff *>(this->FindDescendantByComparison(&staffN));
    if (!staff) {
        LogDebug("Orignal ossia staff %d could not be found", ossia->GetNFromOssia());
    }
    return staff;
}

void Ossia::ResetAlignments()
{
    m_clefAlignment = NULL;
    m_keySigAlignment = NULL;
    m_drawingLeftBarLine.ResetParent();
    m_drawingLeftBarLine.ResetAlignment();
}

int Ossia::GetScoreDefShift() const
{
    // The clef is the further appart
    if (m_clefAlignment) return m_clefAlignment->GetXRel();
    // Other wise the key signature
    if (m_keySigAlignment) return m_keySigAlignment->GetXRel();
    return 0;
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

const Staff *Ossia::GetDrawingTopOStaff() const
{
    // We have only staffDef as children
    const StaffDef *staffDef = vrv_cast<const StaffDef *>(m_drawingStaffGrp.GetFirst());
    AttNIntegerComparison n(STAFF, staffDef->GetN());
    const Staff *staff = vrv_cast<const Staff *>(this->FindDescendantByComparison(&n));
    return (staff && !staff->IsHidden()) ? staff : NULL;
}

const Staff *Ossia::GetDrawingBottopOStaff() const
{
    // We have only staffDef as children
    const StaffDef *staffDef = vrv_cast<const StaffDef *>(m_drawingStaffGrp.GetLast());
    AttNIntegerComparison n(STAFF, staffDef->GetN());
    const Staff *staff = vrv_cast<const Staff *>(this->FindDescendantByComparison(&n));
    return (staff && !staff->IsHidden()) ? staff : NULL;
}

std::vector<int> Ossia::GetOStaffNs() const
{
    ListOfConstObjects staves = this->FindAllDescendantsByType(STAFF);
    std::vector<int> ns;
    for (auto object : staves) {
        const Staff *staff = vrv_cast<const Staff *>(object);
        assert(staff);
        if (staff->IsOssia() && !staff->IsHidden()) ns.push_back(staff->GetN());
    }
    return ns;
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
        if (staff->IsHidden()) continue;
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
