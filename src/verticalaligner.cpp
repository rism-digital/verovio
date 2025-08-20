/////////////////////////////////////////////////////////////////////////////
// Name:        verticalaligner.cpp
// Author:      Laurent Pugin
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "verticalaligner.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <math.h>
#include <utility>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "doc.h"
#include "floatingobject.h"
#include "functor.h"
#include "scoredef.h"
#include "slur.h"
#include "smufl.h"
#include "staff.h"
#include "staffdef.h"
#include "staffgrp.h"
#include "system.h"
#include "tie.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// SystemAligner
//----------------------------------------------------------------------------

SystemAligner::SystemAligner() : Object(SYSTEM_ALIGNER)
{
    this->Reset();
}

SystemAligner::~SystemAligner() {}

void SystemAligner::Reset()
{
    Object::Reset();
    m_spacingTypes.clear();
    m_system = NULL;

    m_bottomAlignment = new StaffAlignment();
    m_bottomAlignment->SetStaff(NULL, NULL, this->GetAboveSpacingType(NULL));
    m_bottomAlignment->SetParentSystem(this->GetSystem());
    this->AddChild(m_bottomAlignment);
}

bool SystemAligner::IsSupportedChild(ClassId classId)
{
    // Nothing to check here
    return true;
}

StaffAlignment *SystemAligner::GetStaffAlignment(int idx, Staff *staff, const Doc *doc)
{
    ArrayOfObjects &children = this->GetChildrenForModification();

    // The last one is always the bottomAlignment
    assert(m_bottomAlignment);
    // remove it temporarily
    children.pop_back();

    if (idx < this->GetChildCount()) {
        children.push_back(m_bottomAlignment);
        return dynamic_cast<StaffAlignment *>(this->GetChild(idx));
    }
    // check that we are searching for the next one (not a gap)
    assert(idx == this->GetChildCount());
    // LogDebug("Creating staff alignment");

    // This is the first time we are looking for it (e.g., first staff)
    // We create the StaffAlignment
    StaffAlignment *alignment = new StaffAlignment();
    alignment->SetStaff(staff, doc, this->GetAboveSpacingType(staff));
    alignment->SetParentSystem(this->GetSystem());
    this->AddChild(alignment);

    // put back the bottomAlignment
    children.push_back(m_bottomAlignment);

    return alignment;
}

void SystemAligner::ReorderBy(const std::vector<int> &staffNs)
{
    std::vector<int> order = staffNs;
    // First check that staffNs are unique
    std::sort(order.begin(), order.end());
    order.erase(std::unique(order.begin(), order.end()), order.end());
    // If not, we should return because the re-ordering below will corrupt the data
    // Returning will keep the order as it is
    if (order.size() != staffNs.size()) return;

    ArrayOfObjects &children = this->GetChildrenForModification();

    // Since we have a bottom alignment, the number is +1
    // The children list can be smaller with optimized systems
    if (children.size() > staffNs.size() + 1) return;

    ListOfObjects orderedAlignments;
    for (auto staffN : staffNs) {
        StaffAlignment *alignment = this->GetStaffAlignmentForStaffN(staffN);
        // This happens with condensed systems where some alignment for staffN are not there
        if (!alignment) continue;
        orderedAlignments.push_back(alignment);
    }
    int i = 0;
    // Since the number of staffAlignment is the same and they are unique, we can
    // blindly replace them in the StaffAligner children
    for (auto alignment : orderedAlignments) {
        children.at(i) = alignment;
        ++i;
    }
}

StaffAlignment *SystemAligner::GetStaffAlignmentForStaffN(int staffN)
{
    return const_cast<StaffAlignment *>(std::as_const(*this).GetStaffAlignmentForStaffN(staffN));
}

const StaffAlignment *SystemAligner::GetStaffAlignmentForStaffN(int staffN) const
{
    const StaffAlignment *alignment = NULL;
    for (int i = 0; i < this->GetChildCount(); ++i) {
        alignment = vrv_cast<const StaffAlignment *>(this->GetChild(i));
        assert(alignment);

        if ((alignment->GetStaff()) && (alignment->GetStaff()->GetN() == staffN)) return alignment;
    }
    // LogDebug("Staff alignment for staff %d not found", staffN);
    return NULL;
}

System *SystemAligner::GetSystem()
{
    if (m_system == NULL) {
        m_system = vrv_cast<System *>(this->GetFirstAncestor(SYSTEM));
    }
    return m_system;
}

void SystemAligner::FindAllPositionerPointingTo(ArrayOfFloatingPositioners *positioners, const FloatingObject *object)
{
    assert(positioners);

    positioners->clear();

    StaffAlignment *alignment = NULL;
    for (const auto child : this->GetChildren()) {
        alignment = vrv_cast<StaffAlignment *>(child);
        assert(alignment);
        FloatingPositioner *positioner = alignment->GetCorrespFloatingPositioner(object);
        if (positioner && (positioner->GetObject() == object)) {
            positioners->push_back(positioner);
        }
    }
}

void SystemAligner::FindAllIntersectionPoints(
    SegmentedLine &line, const BoundingBox &boundingBox, const std::vector<ClassId> &classIds, int margin) const
{
    const StaffAlignment *alignment = NULL;
    for (const auto child : this->GetChildren()) {
        alignment = vrv_cast<const StaffAlignment *>(child);
        assert(alignment);
        alignment->FindAllIntersectionPoints(line, boundingBox, classIds, margin);
    }
}

int SystemAligner::GetOverflowAbove(const Doc *, bool scoreDefClef) const
{
    if (!this->GetChildCount() || this->GetChild(0) == m_bottomAlignment) return 0;

    const StaffAlignment *alignment = vrv_cast<const StaffAlignment *>(this->GetChild(0));
    assert(alignment);
    int overflowAbove = scoreDefClef ? alignment->GetScoreDefClefOverflowAbove() : alignment->GetOverflowAbove();
    return overflowAbove;
}

int SystemAligner::GetOverflowBelow(const Doc *doc, bool scoreDefClef) const
{
    if (!this->GetChildCount() || this->GetChild(0) == m_bottomAlignment) return 0;

    const StaffAlignment *alignment = vrv_cast<const StaffAlignment *>(this->GetChild(this->GetChildCount() - 2));
    assert(alignment);
    int overflowBelow = scoreDefClef ? alignment->GetScoreDefClefOverflowBelow() : alignment->GetOverflowBelow();
    return overflowBelow;
}

double SystemAligner::GetJustificationSum(const Doc *doc) const
{
    assert(doc);

    double justificationSum = 0.;
    for (const auto child : this->GetChildren()) {
        const StaffAlignment *alignment = dynamic_cast<const StaffAlignment *>(child);
        justificationSum += alignment ? alignment->GetJustificationFactor(doc) : 0.;
    }

    return justificationSum;
}
void SystemAligner::SetSpacing(const ScoreDef *scoreDef)
{
    assert(scoreDef);

    m_spacingTypes.clear();

    const ListOfConstObjects &childList = scoreDef->GetList();
    for (const Object *object : childList) {
        // It should be staffDef only, but double check.
        if (!object->Is(STAFFDEF)) continue;
        const StaffDef *staffDef = vrv_cast<const StaffDef *>(object);
        assert(staffDef);

        m_spacingTypes[staffDef->GetN()] = CalculateSpacingAbove(staffDef);
    }
}

SystemAligner::SpacingType SystemAligner::GetAboveSpacingType(const Staff *staff)
{
    if (!staff) return SpacingType::None;

    if (m_spacingTypes.empty()) {
        const System *system = vrv_cast<const System *>(staff->GetFirstAncestor(SYSTEM));
        const ScoreDef *scoreDef = system ? system->GetDrawingScoreDef() : NULL;
        this->SetSpacing(scoreDef);
    }

    auto iter = m_spacingTypes.find(staff->GetN());
    if (iter == m_spacingTypes.end()) {
        LogWarning("No spacing type found matching @n=%d for '<%s>'", staff->GetN(), staff->GetID().c_str());
        return SpacingType::None;
    }

    return iter->second;
}

SystemAligner::SpacingType SystemAligner::CalculateSpacingAbove(const StaffDef *staffDef) const
{
    assert(staffDef);

    SpacingType spacingType = SpacingType::None;
    if (staffDef->GetDrawingVisibility() != OPTIMIZATION_HIDDEN) {
        const Object *staffChild = staffDef;
        const Object *staffParent = staffChild->GetParent();
        bool notFirstInGroup = false;
        VisibleStaffDefOrGrpObject matchType;
        while (spacingType == SpacingType::None) {
            matchType.Skip(staffParent);
            const Object *firstVisible = staffParent->FindDescendantByComparison(&matchType, 1);

            // for first child in staff group parent's symbol should be taken, except
            // when we had a child which not on the first place in group, than take first symbol
            notFirstInGroup = notFirstInGroup || (firstVisible && firstVisible != staffChild);
            if (notFirstInGroup) {
                const StaffGrp *staffGrp = dynamic_cast<const StaffGrp *>(staffParent);
                if (staffGrp && staffGrp->GetFirst(GRPSYM)) {
                    const GrpSym *grpSym = vrv_cast<const GrpSym *>(staffGrp->GetFirst(GRPSYM));
                    assert(grpSym);
                    switch (grpSym->GetSymbol()) {
                        case staffGroupingSym_SYMBOL_brace: spacingType = SpacingType::Brace; break;
                        case staffGroupingSym_SYMBOL_bracket:
                        case staffGroupingSym_SYMBOL_bracketsq: spacingType = SpacingType::Bracket; break;
                        default: spacingType = SpacingType::None;
                    }
                }
            }

            if (spacingType == SpacingType::None) {
                staffChild = staffParent;
                staffParent = staffChild->GetParent();
                if (!staffParent || !staffParent->Is(STAFFGRP)) {
                    spacingType = notFirstInGroup ? SpacingType::Staff : SpacingType::System;
                }
            }
        }
    }

    return spacingType;
}

FunctorCode SystemAligner::Accept(Functor &functor)
{
    return functor.VisitSystemAligner(this);
}

FunctorCode SystemAligner::Accept(ConstFunctor &functor) const
{
    return functor.VisitSystemAligner(this);
}

FunctorCode SystemAligner::AcceptEnd(Functor &functor)
{
    return functor.VisitSystemAlignerEnd(this);
}

FunctorCode SystemAligner::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitSystemAlignerEnd(this);
}

//----------------------------------------------------------------------------
// StaffAlignment
//----------------------------------------------------------------------------

StaffAlignment::StaffAlignment() : Object(STAFF_ALIGNMENT)
{
    m_yRel = 0;
    m_verseAboveNs.clear();
    m_verseBelowNs.clear();
    m_staff = NULL;
    m_floatingPositionersSorted = true;

    m_overflowAbove = 0;
    m_overflowBelow = 0;
    m_staffHeight = 0;
    m_overlap = 0;
    m_requestedSpaceAbove = 0;
    m_requestedSpaceBelow = 0;
    m_requestedSpacing = 0;
    m_scoreDefClefOverflowAbove = 0;
    m_scoreDefClefOverflowBelow = 0;
}

StaffAlignment::~StaffAlignment()
{
    this->ClearPositioners();
}

void StaffAlignment::ClearPositioners()
{
    for (FloatingPositioner *positioner : m_floatingPositioners) {
        delete positioner;
    }
    m_floatingPositioners.clear();

    m_floatingPositionersSorted = true;
}

void StaffAlignment::SortPositioners()
{
    if (!m_floatingPositionersSorted) {
        std::stable_sort(m_floatingPositioners.begin(), m_floatingPositioners.end(),
            [](FloatingPositioner *left, FloatingPositioner *right) {
                if (left->GetObject()->GetClassId() == right->GetObject()->GetClassId()) {
                    if (left->GetDrawingPlace() == right->GetDrawingPlace()) {
                        return left->GetObject()->IsCloserToStaffThan(right->GetObject(), right->GetDrawingPlace());
                    }
                    else {
                        return (left->GetDrawingPlace() < right->GetDrawingPlace());
                    }
                }
                else {
                    return (left->GetObject()->GetClassId() < right->GetObject()->GetClassId());
                }
                return false;
            });

        m_floatingPositionersSorted = true;
    }
}

void StaffAlignment::SetStaff(Staff *staff, const Doc *doc, SystemAligner::SpacingType spacingType)
{
    m_staff = staff;
    m_spacingType = spacingType;
    if (staff && doc) {
        m_staffHeight = (staff->m_drawingLines - 1) * doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
    }
}

void StaffAlignment::SetParentSystem(System *system)
{
    m_system = system;
}

int StaffAlignment::GetStaffSize() const
{
    return m_staff ? m_staff->m_drawingStaffSize : 100;
}

const AttSpacing *StaffAlignment::GetAttSpacing() const
{
    const System *system = this->GetParentSystem();
    assert(system);

    return system->GetDrawingScoreDef();
}

void StaffAlignment::SetYRel(int yRel)
{
    if (yRel < m_yRel) {
        m_yRel = yRel;
    }
}

void StaffAlignment::SetOverflowAbove(int overflowAbove)
{
    if (overflowAbove > m_overflowAbove) {
        m_overflowAbove = overflowAbove;
    }
}

void StaffAlignment::SetRequestedSpaceAbove(int space)
{
    if (space > m_requestedSpaceAbove) {
        m_requestedSpaceAbove = space;
    }
}

void StaffAlignment::SetOverlap(int overlap)
{
    if (overlap > m_overlap) {
        m_overlap = overlap;
    }
}

void StaffAlignment::SetOverflowBelow(int overflowBottom)
{
    if (overflowBottom > m_overflowBelow) {
        m_overflowBelow = overflowBottom;
    }
}

void StaffAlignment::SetRequestedSpaceBelow(int space)
{
    if (space > m_requestedSpaceBelow) {
        m_requestedSpaceBelow = space;
    }
}

void StaffAlignment::AddVerseN(int verseN, data_STAFFREL place)
{
    // if 0, then assume 1;
    verseN = std::max(verseN, 1);
    (place == STAFFREL_above) ? m_verseAboveNs.insert(verseN) : m_verseBelowNs.insert(verseN);
}

int StaffAlignment::GetVerseCount(bool collapse) const
{
    return (this->GetVerseCountAbove(collapse) + this->GetVerseCountBelow(collapse));
}

int StaffAlignment::GetVerseCountAbove(bool collapse) const
{
    if (m_verseAboveNs.empty()) {
        return 0;
    }
    else if (collapse) {
        return (int)m_verseAboveNs.size();
    }
    else {
        return (*m_verseAboveNs.rbegin());
    }
}

int StaffAlignment::GetVerseCountBelow(bool collapse) const
{
    if (m_verseBelowNs.empty()) {
        return 0;
    }
    else if (collapse) {
        return (int)m_verseBelowNs.size();
    }
    else {
        return (*m_verseBelowNs.rbegin());
    }
}

int StaffAlignment::GetVersePositionAbove(int verseN, bool collapse) const
{
    if (m_verseAboveNs.empty()) {
        // Syl in neumatic notation - since verse count will be 0, position is -1
        return -1;
    }
    else if (collapse) {
        auto it = std::find(m_verseAboveNs.begin(), m_verseAboveNs.end(), verseN);
        int pos = (int)std::distance(m_verseAboveNs.begin(), it);
        return pos;
    }
    else {
        return verseN - 1;
    }
}

int StaffAlignment::GetVersePositionBelow(int verseN, bool collapse) const
{
    if (m_verseBelowNs.empty()) {
        // Syl in neumatic notation - since verse count will be 0, position is -1
        return -1;
    }
    else if (collapse) {
        auto it = std::find(m_verseBelowNs.rbegin(), m_verseBelowNs.rend(), verseN);
        int pos = (int)std::distance(m_verseBelowNs.rbegin(), it);
        return pos;
    }
    else {
        return (*m_verseBelowNs.rbegin()) - verseN;
    }
}

double StaffAlignment::GetJustificationFactor(const Doc *doc) const
{
    assert(doc);

    double justificationFactor = 0.;
    if (m_staff) {
        switch (m_spacingType) {
            case SystemAligner::SpacingType::System:
                justificationFactor = doc->GetOptions()->m_justificationSystem.GetValue();
                break;
            case SystemAligner::SpacingType::Staff:
                justificationFactor = doc->GetOptions()->m_justificationStaff.GetValue();
                break;
            case SystemAligner::SpacingType::Brace:
                justificationFactor = doc->GetOptions()->m_justificationBraceGroup.GetValue();
                break;
            case SystemAligner::SpacingType::Bracket:
                justificationFactor = doc->GetOptions()->m_justificationBracketGroup.GetValue();
                break;
            case SystemAligner::SpacingType::None: break;
            default: assert(false);
        }
        if (m_spacingType != SystemAligner::SpacingType::System) justificationFactor *= this->GetStaffSize() / 100.0;
    }

    return justificationFactor;
}

int StaffAlignment::CalcOverflowAbove(const BoundingBox *box) const
{
    if (!box->HasContentVerticalBB()) return 0;
    if (box->Is(FLOATING_POSITIONER)) {
        const FloatingPositioner *positioner = vrv_cast<const FloatingPositioner *>(box);
        assert(positioner);
        return positioner->GetContentTop() - this->GetYRel();
    }
    return box->GetSelfTop() - this->GetYRel();
}

int StaffAlignment::CalcOverflowBelow(const BoundingBox *box) const
{
    if (!box->HasContentVerticalBB()) return 0;
    if (box->Is(FLOATING_POSITIONER)) {
        const FloatingPositioner *positioner = vrv_cast<const FloatingPositioner *>(box);
        assert(positioner);
        return -(positioner->GetContentBottom() + m_staffHeight - this->GetYRel());
    }
    return -(box->GetSelfBottom() + m_staffHeight - this->GetYRel());
}

int StaffAlignment::GetMinimumStaffSpacing(const Doc *doc, const AttSpacing *attSpacing) const
{
    const auto &option = doc->GetOptions()->m_spacingStaff;
    int spacing = option.GetValue() * doc->GetDrawingUnit(this->GetStaffSize());

    if (!option.IsSet() && attSpacing->HasSpacingStaff()) {
        if (attSpacing->GetSpacingStaff().GetType() == MEASUREMENTTYPE_px) {
            spacing = attSpacing->GetSpacingStaff().GetPx();
        }
        else {
            spacing = attSpacing->GetSpacingStaff().GetVu() * doc->GetDrawingUnit(100);
        }
    }
    return spacing;
}

int StaffAlignment::GetMinimumSpacing(const Doc *doc) const
{
    assert(doc);

    int spacing = 0;
    const AttSpacing *scoreDefSpacing = this->GetAttSpacing();

    if (!scoreDefSpacing) return spacing;
    if (m_staff && m_staff->m_drawingStaffDef) {
        // Default or staffDef spacing
        if (m_staff->m_drawingStaffDef->HasSpacing()) {
            if (m_staff->m_drawingStaffDef->GetSpacing().GetType() == MEASUREMENTTYPE_px) {
                spacing = m_staff->m_drawingStaffDef->GetSpacing().GetPx();
            }
            else {
                spacing = m_staff->m_drawingStaffDef->GetSpacing().GetVu() * doc->GetDrawingUnit(100);
            }
        }
        else {
            switch (m_spacingType) {
                case SystemAligner::SpacingType::System: {
                    // Top staff spacing (above) is half of a staff spacing
                    spacing = this->GetMinimumStaffSpacing(doc, scoreDefSpacing) / 2;
                    break;
                }
                case SystemAligner::SpacingType::Staff: {
                    spacing = this->GetMinimumStaffSpacing(doc, scoreDefSpacing);
                    break;
                }
                case SystemAligner::SpacingType::Brace: {
                    const auto &option = doc->GetOptions()->m_spacingBraceGroup;
                    spacing = option.IsSet() ? option.GetValue() * doc->GetDrawingUnit(this->GetStaffSize())
                                             : this->GetMinimumStaffSpacing(doc, scoreDefSpacing);
                    break;
                }
                case SystemAligner::SpacingType::Bracket: {
                    const auto &option = doc->GetOptions()->m_spacingBracketGroup;
                    spacing = option.IsSet() ? option.GetValue() * doc->GetDrawingUnit(this->GetStaffSize())
                                             : this->GetMinimumStaffSpacing(doc, scoreDefSpacing);
                    break;
                }
                case SystemAligner::SpacingType::None: break;
                default: assert(false);
            }
        }
    }
    // This is the bottom aligner - spacing is half of a staff spacing
    else {
        spacing = this->GetMinimumStaffSpacing(doc, scoreDefSpacing) / 2;
    }

    return spacing;
}

int StaffAlignment::CalcMinimumRequiredSpacing(const Doc *doc) const
{
    assert(doc);

    const Object *parent = this->GetParent();
    assert(parent);

    const StaffAlignment *prevAlignment = dynamic_cast<const StaffAlignment *>(parent->GetPrevious(this));

    if (!prevAlignment) {
        const int maxOverflow = std::max(this->GetOverflowAbove(), this->GetScoreDefClefOverflowAbove());
        return maxOverflow + this->GetOverlap();
    }

    int overflowSum = 0;
    const bool verseCollapse = doc->GetOptions()->m_lyricVerseCollapse.GetValue();
    if (prevAlignment->GetVerseCount(verseCollapse) > 0) {
        overflowSum = prevAlignment->GetOverflowBelow() + this->GetOverflowAbove();
    }
    else {
        // The maximum between the overflow below of the previous staff and the overflow above of the current
        overflowSum = std::max(prevAlignment->GetOverflowBelow(), this->GetOverflowAbove());
        // add overlap if there any
        overflowSum += this->GetOverlap();
    }

    const int unit = doc->GetDrawingUnit(this->GetStaffSize());

    // Add a margin but not for the bottom aligner
    if (m_staff) overflowSum += doc->GetBottomMargin(STAFF) * unit;

    return overflowSum;
}

void StaffAlignment::AdjustBracketGroupSpacing(const Doc *doc, const StaffAlignment *previous, int spacing)
{
    if (!previous) return;

    if (this->IsInBracketGroup(true) && previous->IsInBracketGroup(false)) {
        const int unit = doc->GetDrawingUnit(this->GetStaffSize());
        const int offset = (doc->GetOptions()->m_bracketThickness.GetValue() - 1) * unit / 2;
        const int overflowAbove = doc->GetGlyphHeight(SMUFL_E003_bracketTop, this->GetStaffSize(), false) + offset;
        const int overflowBelow = doc->GetGlyphHeight(SMUFL_E004_bracketBottom, this->GetStaffSize(), false) + offset;
        if (spacing < (overflowAbove + overflowBelow)) {
            const int bracketOverlap = (overflowAbove + overflowBelow) - spacing / 2;
            if (this->GetOverlap() < bracketOverlap) {
                this->SetOverlap(bracketOverlap);
            }
        }
    }
}

bool StaffAlignment::IsInBracketGroup(bool isFirst) const
{
    if (!this->m_staff) return false;

    ScoreDef *scoreDef = this->m_system->GetDrawingScoreDef();
    ListOfObjects groups = scoreDef->FindAllDescendantsByType(STAFFGRP);
    for (Object *staffGrp : groups) {
        // Make sure that there is GrpSym present
        GrpSym *grpSym = vrv_cast<GrpSym *>(staffGrp->GetFirst(GRPSYM));
        if (!grpSym) continue;

        if (grpSym->GetSymbol() == staffGroupingSym_SYMBOL_bracket) {
            std::set<int> staffNs;
            ListOfObjects staffDefs = staffGrp->FindAllDescendantsByType(STAFFDEF);
            std::for_each(staffDefs.begin(), staffDefs.end(), [&staffNs](Object *object) {
                StaffDef *staffDef = vrv_cast<StaffDef *>(object);
                staffNs.emplace(staffDef->GetN());
            });

            const int currentN = this->m_staff->GetN();
            if (staffNs.contains(currentN)) {
                if ((isFirst && (*staffNs.begin() == currentN)) || (!isFirst && (*staffNs.rbegin() == currentN)))
                    return true;
            }
        }
    }

    return false;
}

void StaffAlignment::SetCurrentFloatingPositioner(
    FloatingObject *object, Object *objectX, Object *objectY, char spanningType)
{
    FloatingPositioner *positioner = this->GetCorrespFloatingPositioner(object);
    if (positioner == NULL) {
        if (object->Is({ LV, PHRASE, SLUR, TIE })) {
            positioner = new FloatingCurvePositioner(object, this, spanningType);
            m_floatingPositioners.push_back(positioner);
        }
        else {
            positioner = new FloatingPositioner(object, this, spanningType);
            m_floatingPositioners.push_back(positioner);
        }
        m_floatingPositionersSorted = false;
    }
    positioner->SetObjectXY(objectX, objectY);
    // LogDebug("BB %d", item->second.m_contentBB_x1);
    object->SetCurrentFloatingPositioner(positioner);
}

FloatingPositioner *StaffAlignment::FindFirstFloatingPositioner(ClassId classId)
{
    return const_cast<FloatingPositioner *>(std::as_const(*this).FindFirstFloatingPositioner(classId));
}

const FloatingPositioner *StaffAlignment::FindFirstFloatingPositioner(ClassId classId) const
{
    auto item = std::find_if(m_floatingPositioners.begin(), m_floatingPositioners.end(),
        [classId](FloatingPositioner *positioner) { return positioner->GetObject()->GetClassId() == classId; });
    if (item != m_floatingPositioners.end()) {
        return *item;
    }
    return NULL;
}

ArrayOfFloatingPositioners StaffAlignment::FindAllFloatingPositioners(ClassId classId)
{
    ArrayOfFloatingPositioners positioners;
    std::copy_if(m_floatingPositioners.begin(), m_floatingPositioners.end(), std::back_inserter(positioners),
        [classId](FloatingPositioner *positioner) { return (positioner->GetObject()->GetClassId() == classId); });
    return positioners;
}

FloatingPositioner *StaffAlignment::GetCorrespFloatingPositioner(const FloatingObject *object)
{
    return const_cast<FloatingPositioner *>(std::as_const(*this).GetCorrespFloatingPositioner(object));
}

const FloatingPositioner *StaffAlignment::GetCorrespFloatingPositioner(const FloatingObject *object) const
{
    auto item = std::find_if(m_floatingPositioners.begin(), m_floatingPositioners.end(),
        [object](FloatingPositioner *positioner) { return positioner->GetObject() == object; });
    if (item != m_floatingPositioners.end()) {
        return *item;
    }
    return NULL;
}

void StaffAlignment::FindAllIntersectionPoints(
    SegmentedLine &line, const BoundingBox &boundingBox, const std::vector<ClassId> &classIds, int margin) const
{
    for (const auto positioner : m_floatingPositioners) {
        assert(positioner->GetObject());
        if (!positioner->GetObject()->Is(classIds)) {
            continue;
        }
        if (positioner->HorizontalContentOverlap(&boundingBox, margin / 2)) {
            line.AddGap(positioner->GetContentTop() + margin, positioner->GetContentBottom() - margin);
        }
    }
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

FunctorCode StaffAlignment::Accept(Functor &functor)
{
    return functor.VisitStaffAlignment(this);
}

FunctorCode StaffAlignment::Accept(ConstFunctor &functor) const
{
    return functor.VisitStaffAlignment(this);
}

FunctorCode StaffAlignment::AcceptEnd(Functor &functor)
{
    return functor.VisitStaffAlignmentEnd(this);
}

FunctorCode StaffAlignment::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitStaffAlignmentEnd(this);
}

} // namespace vrv
