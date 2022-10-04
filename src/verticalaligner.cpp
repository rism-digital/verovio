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
#include "functorparams.h"
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

SystemAligner::SystemAligner() : Object(SYSTEM_ALIGNER), m_bottomAlignment(NULL), m_system(NULL)
{
    this->Reset();
}

SystemAligner::~SystemAligner() {}

void SystemAligner::Reset()
{
    Object::Reset();
    m_spacingTypes.clear();
    m_system = NULL;
    m_bottomAlignment = NULL;
    m_bottomAlignment = this->GetStaffAlignment(0, NULL, NULL);
}

StaffAlignment *SystemAligner::GetStaffAlignment(int idx, Staff *staff, const Doc *doc)
{
    ArrayOfObjects &children = this->GetChildrenForModification();

    // The last one is always the bottomAlignment (unless if not created)
    if (m_bottomAlignment) {
        // remove it temporarily
        children.pop_back();
    }

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
    alignment->SetParent(this);
    alignment->SetParentSystem(this->GetSystem());
    children.push_back(alignment);

    if (m_bottomAlignment) {
        children.push_back(m_bottomAlignment);
    }

    return alignment;
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
    LogDebug("Staff alignment for staff %d not found", staffN);
    return NULL;
}

System *SystemAligner::GetSystem()
{
    if (m_system == NULL) {
        m_system = dynamic_cast<System *>(this->GetFirstAncestor(SYSTEM));
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

    const ListOfConstObjects &childList = scoreDef->GetList(scoreDef);
    for (auto iter = childList.begin(); iter != childList.end(); ++iter) {
        // It should be staffDef only, but double check.
        if (!(*iter)->Is(STAFFDEF)) continue;
        const StaffDef *staffDef = vrv_cast<const StaffDef *>(*iter);
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

//----------------------------------------------------------------------------
// StaffAlignment
//----------------------------------------------------------------------------

StaffAlignment::StaffAlignment() : Object(STAFF_ALIGNMENT)
{
    m_yRel = 0;
    m_verseNs.clear();
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
    ClearPositioners();
}

void StaffAlignment::ClearPositioners()
{
    ArrayOfFloatingPositioners::iterator iter;
    for (iter = m_floatingPositioners.begin(); iter != m_floatingPositioners.end(); ++iter) {
        delete *iter;
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

void StaffAlignment::AddVerseN(int verseN)
{
    // if 0, then assume 1;
    verseN = std::max(verseN, 1);
    m_verseNs.insert(verseN);
}

int StaffAlignment::GetVerseCount(bool collapse) const
{
    if (m_verseNs.empty()) {
        return 0;
    }
    else if (collapse) {
        return (int)m_verseNs.size();
    }
    else {
        return *m_verseNs.rbegin();
    }
}

int StaffAlignment::GetVersePosition(int verseN, bool collapse) const
{
    if (m_verseNs.empty()) {
        return 0;
    }
    else if (collapse) {
        auto it = std::find(m_verseNs.rbegin(), m_verseNs.rend(), verseN);
        int pos = (int)std::distance(m_verseNs.rbegin(), it);
        return pos;
    }
    else {
        return (*m_verseNs.rbegin()) - verseN;
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
    if (box->Is(FLOATING_POSITIONER)) {
        const FloatingPositioner *positioner = vrv_cast<const FloatingPositioner *>(box);
        assert(positioner);
        return positioner->GetContentTop() - this->GetYRel();
    }
    return box->GetSelfTop() - this->GetYRel();
}

int StaffAlignment::CalcOverflowBelow(const BoundingBox *box) const
{
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
        spacing = attSpacing->GetSpacingStaff() * doc->GetDrawingUnit(100);
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
            spacing = m_staff->m_drawingStaffDef->GetSpacing() * doc->GetDrawingUnit(100);
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
    for (auto staffGrp : groups) {
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
            if (staffNs.count(currentN)) {
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

void StaffAlignment::ReAdjustFloatingPositionersGrps(AdjustFloatingPositionerGrpsParams *params,
    const ArrayOfFloatingPositioners &positioners, ArrayOfIntPairs &grpIdYRel)
{
    if (grpIdYRel.empty()) {
        return;
    }

    std::sort(grpIdYRel.begin(), grpIdYRel.end());

    int yRel;
    // The initial next position is the original position of the first group. Nothing will happen for it.
    int nextYRel = grpIdYRel.at(0).second;

    // For each grpId (sorted, see above), loop to find the highest / lowest position to put the next group
    // The move the next group (if not already higher or lower)
    for (auto const &grp : grpIdYRel) {
        // Check if the next group it not already higher or lower.
        if (params->m_place == STAFFREL_above) {
            yRel = (nextYRel < grp.second) ? nextYRel : grp.second;
        }
        else {
            yRel = (nextYRel > grp.second) ? nextYRel : grp.second;
        }
        // Go through all the positioners, but filter by group
        ArrayOfFloatingPositioners::const_iterator iter;
        for (iter = positioners.begin(); iter != positioners.end(); ++iter) {
            int currentGrpId = (*iter)->GetObject()->GetDrawingGrpId();
            // Not the grpId we are processing, skip it.
            if (currentGrpId != grp.first) continue;
            // Set its position
            (*iter)->SetDrawingYRel(yRel);
            // Then find the highest / lowest position for the next group
            if (params->m_place == STAFFREL_above) {
                int iterY = yRel - (*iter)->GetContentY2()
                    - (params->m_doc->GetTopMargin((*iter)->GetObject()->GetClassId())
                        * params->m_doc->GetDrawingUnit(this->GetStaffSize()));
                if (nextYRel > iterY) {
                    nextYRel = iterY;
                }
            }
            else {
                int iterY = yRel + (*iter)->GetContentY2()
                    + (params->m_doc->GetBottomMargin((*iter)->GetObject()->GetClassId())
                        * params->m_doc->GetDrawingUnit(this->GetStaffSize()));
                if (nextYRel < iterY) {
                    nextYRel = iterY;
                }
            }
        }
    }
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int StaffAlignment::AdjustFloatingPositioners(FunctorParams *functorParams)
{
    AdjustFloatingPositionersParams *params = vrv_params_cast<AdjustFloatingPositionersParams *>(functorParams);
    assert(params);

    const int staffSize = this->GetStaffSize();
    const int drawingUnit = params->m_doc->GetDrawingUnit(staffSize);

    this->SortPositioners();

    const bool verseCollapse = params->m_doc->GetOptions()->m_lyricVerseCollapse.GetValue();
    if (params->m_classId == SYL) {
        if (this->GetVerseCount(verseCollapse) > 0) {
            FontInfo *lyricFont = params->m_doc->GetDrawingLyricFont(m_staff->m_drawingStaffSize);
            int descender = params->m_doc->GetTextGlyphDescender(L'q', lyricFont, false);
            int height = params->m_doc->GetTextGlyphHeight(L'I', lyricFont, false);
            int margin = params->m_doc->GetBottomMargin(SYL) * drawingUnit;
            int minMargin = std::max((int)(params->m_doc->GetOptions()->m_lyricTopMinMargin.GetValue() * drawingUnit),
                this->GetOverflowBelow());
            this->SetOverflowBelow(minMargin + this->GetVerseCount(verseCollapse) * (height - descender + margin));
            // For now just clear the overflowBelow, which avoids the overlap to be calculated. We could also keep them
            // and check if they are some lyrics in order to know if the overlap needs to be calculated or not.
            m_overflowBelowBBoxes.clear();
        }
        return FUNCTOR_SIBLINGS;
    }

    ArrayOfFloatingPositioners::iterator iter;
    for (iter = m_floatingPositioners.begin(); iter != m_floatingPositioners.end(); ++iter) {
        assert((*iter)->GetObject());
        if (!params->m_inBetween && !(*iter)->GetObject()->Is(params->m_classId)) continue;

        if (params->m_inBetween) {
            if ((*iter)->GetDrawingPlace() != STAFFREL_between) continue;
        }
        else {
            if ((*iter)->GetDrawingPlace() == STAFFREL_between) continue;
        }

        // Skip if no content bounding box is available
        if (!(*iter)->HasContentBB()) continue;

        // for slurs and ties we do not need to adjust them, only add them to the overflow boxes if required
        if ((params->m_classId == LV) || (params->m_classId == PHRASE) || (params->m_classId == SLUR)
            || (params->m_classId == TIE)) {

            assert((*iter)->Is(FLOATING_CURVE_POSITIONER));
            FloatingCurvePositioner *curve = vrv_cast<FloatingCurvePositioner *>(*iter);
            assert(curve);

            bool skipAbove = false;
            bool skipBelow = false;

            if ((*iter)->GetObject()->Is({ LV, PHRASE, SLUR, TIE })) {
                TimeSpanningInterface *interface = (*iter)->GetObject()->GetTimeSpanningInterface();
                assert(interface);
                interface->GetCrossStaffOverflows(this, curve->GetDir(), skipAbove, skipBelow);
            }

            int overflowAbove = 0;
            if (!skipAbove) overflowAbove = this->CalcOverflowAbove(*iter);
            if (overflowAbove > params->m_doc->GetDrawingStaffLineWidth(staffSize) / 2) {
                // LogMessage("%sparams->m_doc top overflow: %d", this->GetID().c_str(), overflowAbove);
                this->SetOverflowAbove(overflowAbove);
                m_overflowAboveBBoxes.push_back(*iter);
            }

            int overflowBelow = 0;
            if (!skipBelow) overflowBelow = this->CalcOverflowBelow(*iter);
            if (overflowBelow > params->m_doc->GetDrawingStaffLineWidth(staffSize) / 2) {
                // LogMessage("%s bottom overflow: %d", this->GetID().c_str(), overflowBelow);
                this->SetOverflowBelow(overflowBelow);
                m_overflowBelowBBoxes.push_back(*iter);
            }

            int spaceAbove = 0;
            int spaceBelow = 0;
            std::tie(spaceAbove, spaceBelow) = curve->CalcRequestedStaffSpace(this);
            this->SetRequestedSpaceAbove(spaceAbove);
            this->SetRequestedSpaceBelow(spaceBelow);

            continue;
        }

        // This sets the default position (without considering any overflowing box)
        (*iter)->CalcDrawingYRel(params->m_doc, this, NULL);

        ArrayOfBoundingBoxes *overflowBoxes = &m_overflowBelowBBoxes;
        // above?
        data_STAFFREL place = (*iter)->GetDrawingPlace();
        if (place == STAFFREL_above) {
            overflowBoxes = &m_overflowAboveBBoxes;
        }
        // Handle within placement (ignore collisions for certain classes)
        if (place == STAFFREL_within) {
            if (params->m_classId == DIR) continue;
            if (params->m_classId == HAIRPIN) continue;
        }

        auto i = overflowBoxes->begin();
        auto end = overflowBoxes->end();
        while (i != end) {
            // find all the overflowing elements from the staff that overlap horizontally (and, in case of extender
            // elements - vertically)
            LayerElement *element = dynamic_cast<LayerElement *>(*i);
            const bool additionalMargin
                = ((*iter)->GetObject()->Is(DYNAM) && element && element->GetFirstAncestor(BEAM));
            const int margin = additionalMargin ? params->m_doc->GetDrawingDoubleUnit(m_staff->m_drawingStaffSize) : 0;
            i = std::find_if(i, end, [iter, drawingUnit, margin](BoundingBox *elem) {
                if ((*iter)->GetObject()->IsExtenderElement() && !elem->Is(FLOATING_POSITIONER)) {
                    return (*iter)->HorizontalContentOverlap(elem, drawingUnit * 8)
                        || (*iter)->VerticalContentOverlap(elem);
                }
                return (*iter)->HorizontalContentOverlap(elem, margin);
            });
            if (i != end) {
                // update the yRel accordingly
                (*iter)->CalcDrawingYRel(params->m_doc, this, *i);
                i++;
            }
        }
        //  Now update the staffAlignment max overflow (above or below) and add the positioner to the list of
        //  overflowing elements
        if (place == STAFFREL_above) {
            int overflowAbove = this->CalcOverflowAbove((*iter));
            overflowBoxes->push_back((*iter));
            this->SetOverflowAbove(overflowAbove);
        }
        // below (or between)
        else {
            int overflowBelow = this->CalcOverflowBelow((*iter));
            overflowBoxes->push_back((*iter));
            this->SetOverflowBelow(overflowBelow);
        }
    }

    return FUNCTOR_SIBLINGS;
}

int StaffAlignment::AdjustFloatingPositionersBetween(FunctorParams *functorParams)
{
    AdjustFloatingPositionersBetweenParams *params
        = vrv_params_cast<AdjustFloatingPositionersBetweenParams *>(functorParams);
    assert(params);

    // int staffSize = this->GetStaffSize();

    // First staff - nothing to do
    if (params->m_previousStaffPositioners == NULL) {
        params->m_previousStaffPositioners = &m_floatingPositioners;
        params->m_previousStaffAlignment = this;
        return FUNCTOR_SIBLINGS;
    }
    assert(params->m_previousStaffAlignment);

    int dist = params->m_previousStaffAlignment->GetYRel() - this->GetYRel();
    dist -= params->m_previousStaffAlignment->m_staffHeight;
    int centerYRel = dist / 2 + params->m_previousStaffAlignment->m_staffHeight;

    for (auto &positioner : *params->m_previousStaffPositioners) {
        assert(positioner->GetObject());
        if (!positioner->GetObject()->Is({ DIR, DYNAM, HAIRPIN, TEMPO })) continue;

        if (positioner->GetDrawingPlace() != STAFFREL_between) continue;

        // Skip if no content bounding box is available
        if (!positioner->HasContentBB()) continue;

        int diffY = centerYRel - positioner->GetDrawingYRel();

        ArrayOfBoundingBoxes *overflowBoxes = &m_overflowAboveBBoxes;
        auto i = overflowBoxes->begin();
        auto end = overflowBoxes->end();
        bool adjusted = false;
        while (i != end) {

            // find all the overflowing elements from the staff that overlap horizontally
            i = std::find_if(
                i, end, [positioner](BoundingBox *elem) { return positioner->HorizontalContentOverlap(elem); });
            if (i != end) {
                // update the yRel accordingly
                int y = positioner->GetSpaceBelow(params->m_doc, this, *i);
                if (y < diffY) {
                    diffY = y;
                    adjusted = true;
                }
                i++;
            }
        }
        if (!adjusted) {
            positioner->SetDrawingYRel(centerYRel);
        }
        else {
            positioner->SetDrawingYRel(positioner->GetDrawingYRel() + diffY);
        }
    }

    params->m_previousStaffPositioners = &m_floatingPositioners;
    params->m_previousStaffAlignment = this;

    return FUNCTOR_SIBLINGS;
}

int StaffAlignment::AdjustFloatingPositionerGrps(FunctorParams *functorParams)
{
    AdjustFloatingPositionerGrpsParams *params = vrv_params_cast<AdjustFloatingPositionerGrpsParams *>(functorParams);
    assert(params);

    ArrayOfFloatingPositioners positioners;
    // make a temporary copy of positioners with a classId desired and that have a drawing grpId
    std::copy_if(m_floatingPositioners.begin(), m_floatingPositioners.end(), std::back_inserter(positioners),
        [params](FloatingPositioner *positioner) {
            assert(positioner->GetObject());
            // search in the desired classIds
            return (
                (std::find(params->m_classIds.begin(), params->m_classIds.end(), positioner->GetObject()->GetClassId())
                    != params->m_classIds.end())
                && (positioner->GetObject()->GetDrawingGrpId() != 0)
                && (positioner->GetDrawingPlace() == params->m_place) && !positioner->HasEmptyBB());
        });

    if (positioners.empty()) {
        return FUNCTOR_SIBLINGS;
    }

    // A vector for storing a pair with the grpId and the min or max YRel
    ArrayOfIntPairs grpIdYRel;

    ArrayOfFloatingPositioners::iterator iter;
    for (iter = positioners.begin(); iter != positioners.end(); ++iter) {
        int currentGrpId = (*iter)->GetObject()->GetDrawingGrpId();
        // Look if we already have a pair for this grpId
        auto i = std::find_if(grpIdYRel.begin(), grpIdYRel.end(),
            [currentGrpId](std::pair<int, int> &pair) { return (pair.first == currentGrpId); });
        // if not, then just add a new pair with the YRel of the current positioner
        if (i == grpIdYRel.end()) {
            grpIdYRel.push_back({ currentGrpId, (*iter)->GetDrawingYRel() });
        }
        // else, adjust the min or max YRel of the pair if necessary
        else {
            if (params->m_place == STAFFREL_above) {
                if ((*iter)->GetDrawingYRel() < (*i).second) (*i).second = (*iter)->GetDrawingYRel();
            }
            else {
                if ((*iter)->GetDrawingYRel() > (*i).second) (*i).second = (*iter)->GetDrawingYRel();
            }
        }
    }

    if (std::find(params->m_classIds.begin(), params->m_classIds.end(), HARM) != params->m_classIds.end()) {
        // Re-adjust the postion in order to make sure the group remain in the right order
        this->ReAdjustFloatingPositionersGrps(params, positioners, grpIdYRel);
        // The already move them, so the loop below is not necessary.
    }
    else {
        // Now go through all the positioners again and adjust the YRel with the value of the pair
        for (iter = positioners.begin(); iter != positioners.end(); ++iter) {
            int currentGrpId = (*iter)->GetObject()->GetDrawingGrpId();
            auto i = std::find_if(grpIdYRel.begin(), grpIdYRel.end(),
                [currentGrpId](std::pair<int, int> &pair) { return (pair.first == currentGrpId); });
            // We must have found it
            assert(i != grpIdYRel.end());
            (*iter)->SetDrawingYRel((*i).second);
        }
    }

    //  Now update the staffAlignment max overflow (above or below)
    for (iter = positioners.begin(); iter != positioners.end(); ++iter) {
        if (params->m_place == STAFFREL_above) {
            int overflowAbove = this->CalcOverflowAbove((*iter));
            this->SetOverflowAbove(overflowAbove);
        }
        else {
            int overflowBelow = this->CalcOverflowBelow((*iter));
            this->SetOverflowBelow(overflowBelow);
        }
    }

    return FUNCTOR_SIBLINGS;
}

int StaffAlignment::AdjustSlurs(FunctorParams *functorParams)
{
    AdjustSlursParams *params = vrv_params_cast<AdjustSlursParams *>(functorParams);
    assert(params);

    Staff *staff = this->GetStaff();
    if (!staff) return FUNCTOR_CONTINUE;
    const int unit = params->m_doc->GetDrawingUnit(staff->m_drawingStaffSize);

    // Adjust each slur such that spanned elements are avoided
    ArrayOfFloatingCurvePositioners positioners;
    for (FloatingPositioner *positioner : m_floatingPositioners) {
        assert(positioner->GetObject());
        if (!positioner->GetObject()->Is({ PHRASE, SLUR })) continue;
        Slur *slur = vrv_cast<Slur *>(positioner->GetObject());
        assert(slur);

        assert(positioner->Is(FLOATING_CURVE_POSITIONER));
        FloatingCurvePositioner *curve = vrv_cast<FloatingCurvePositioner *>(positioner);
        assert(curve);

        // Skip if no content bounding box is available
        if (!curve->HasContentBB()) continue;
        positioners.push_back(curve);

        slur->AdjustSlur(params->m_doc, curve, unit);

        if (curve->IsCrossStaff()) {
            params->m_crossStaffSlurs = true;
        }
    }

    // Detection of inner slurs
    std::map<FloatingCurvePositioner *, ArrayOfFloatingCurvePositioners> innerCurveMap;
    for (size_t i = 0; i < positioners.size(); ++i) {
        Slur *firstSlur = vrv_cast<Slur *>(positioners[i]->GetObject());
        ArrayOfFloatingCurvePositioners innerCurves;
        for (size_t j = 0; j < positioners.size(); ++j) {
            if (i == j) continue;
            Slur *secondSlur = vrv_cast<Slur *>(positioners[j]->GetObject());
            // Check if second slur is inner slur of first
            if (!positioners[i]->IsCrossStaff() && !positioners[j]->IsCrossStaff()) {
                if (positioners[j]->GetSpanningType() == SPANNING_START_END) {
                    if (firstSlur->HasInnerSlur(secondSlur)) {
                        innerCurves.push_back(positioners[j]);
                        continue;
                    }
                }
            }
            // Adjust positioning of slurs with common start/end
            Point points1[4], points2[4];
            positioners[i]->GetPoints(points1);
            positioners[j]->GetPoints(points2);
            if ((firstSlur->GetEnd() == secondSlur->GetStart())
                && BoundingBox::ArePointsClose(points1[3], points2[0], unit)) {
                positioners[i]->MoveBackHorizontal(-unit / 2);
                positioners[j]->MoveFrontHorizontal(unit / 2);
            }
            if ((firstSlur->GetStart() == secondSlur->GetStart())
                && BoundingBox::ArePointsClose(points1[0], points2[0], unit) && (points1[3].x > points2[3].x)) {
                int diff = points2[0].y - points1[0].y;
                diff += ((positioners[i]->GetDir() == curvature_CURVEDIR_below) ? -unit : unit);
                positioners[i]->MoveFrontVertical(diff);
            }
            if ((firstSlur->GetEnd() == secondSlur->GetEnd())
                && BoundingBox::ArePointsClose(points1[3], points2[3], unit) && (points1[0].x < points2[0].x)) {
                int diff = points2[3].y - points1[3].y;
                diff += ((positioners[i]->GetDir() == curvature_CURVEDIR_below) ? -unit : unit);
                positioners[i]->MoveBackVertical(diff);
            }
        }
        if (!innerCurves.empty()) {
            innerCurveMap[positioners[i]] = innerCurves;
        }
    }

    // Adjust outer slurs w.r.t. inner slurs
    for (const auto &mapEntry : innerCurveMap) {
        Slur *slur = vrv_cast<Slur *>(mapEntry.first->GetObject());
        assert(slur);
        slur->AdjustOuterSlur(params->m_doc, mapEntry.first, mapEntry.second, unit);
    }

    return FUNCTOR_SIBLINGS;
}

int StaffAlignment::AdjustStaffOverlap(FunctorParams *functorParams)
{
    AdjustStaffOverlapParams *params = vrv_params_cast<AdjustStaffOverlapParams *>(functorParams);
    assert(params);

    // This is the first alignment
    if (params->m_previous == NULL) {
        params->m_previous = this;
        return FUNCTOR_SIBLINGS;
    }

    const int spacing = std::max(params->m_previous->m_overflowBelow, m_overflowAbove);

    // Calculate the overlap for scoreDef clefs
    int overflowBelow = params->m_previous->GetScoreDefClefOverflowBelow();
    int overflowAbove = this->GetScoreDefClefOverflowAbove();
    if (spacing < (overflowBelow + overflowAbove)) {
        this->SetOverlap((overflowBelow + overflowAbove) - spacing);
    }

    this->AdjustBracketGroupSpacing(params->m_doc, params->m_previous, spacing);

    // Calculate the requested spacing
    const int currentStaffDistance
        = params->m_previous->GetYRel() - params->m_previous->GetStaffHeight() - this->GetYRel();
    const int requestedSpace = std::max(this->GetRequestedSpaceAbove(), params->m_previous->GetRequestedSpaceBelow());
    if (requestedSpace > 0) {
        this->SetRequestedSpacing(currentStaffDistance + requestedSpace);
    }

    // This is the bottom alignment (or something is wrong) - this is all we need to do
    if (!m_staff) {
        return FUNCTOR_STOP;
    }

    const int staffSize = this->GetStaffSize();
    const int drawingUnit = params->m_doc->GetDrawingUnit(staffSize);

    ArrayOfBoundingBoxes::iterator iter;
    // go through all the elements of the top staff that have an overflow below
    for (iter = params->m_previous->m_overflowBelowBBoxes.begin();
         iter != params->m_previous->m_overflowBelowBBoxes.end(); ++iter) {
        auto i = m_overflowAboveBBoxes.begin();
        auto end = m_overflowAboveBBoxes.end();
        while (i != end) {
            // find all the elements from the bottom staff that have an overflow at the top with an horizontal overlap
            i = std::find_if(i, end, [iter, drawingUnit](BoundingBox *elem) {
                if ((*iter)->Is(FLOATING_POSITIONER)) {
                    FloatingPositioner *fp = vrv_cast<FloatingPositioner *>(*iter);
                    if (fp->GetObject()->Is({ DIR, DYNAM }) && fp->GetObject()->IsExtenderElement()) {
                        return (*iter)->HorizontalContentOverlap(elem, drawingUnit * 4)
                            || (*iter)->VerticalContentOverlap(elem);
                    }
                }
                return (*iter)->HorizontalContentOverlap(elem);
            });
            if (i != end) {
                // calculate the vertical overlap and see if this is more than the expected space
                int overflowBelow = params->m_previous->CalcOverflowBelow(*iter);
                int overflowAbove = this->CalcOverflowAbove(*i);
                int minSpaceBetween = 0;
                if (((*iter)->Is(ARTIC) && ((*i)->Is({ ARTIC, NOTE }))) || ((*iter)->Is(NOTE) && ((*i)->Is(ARTIC)))) {
                    minSpaceBetween = drawingUnit;
                }
                if (spacing < (overflowBelow + overflowAbove + minSpaceBetween)) {
                    // LogDebug("Overlap %d", (overflowBelow + overflowAbove + minSpaceBetween) - spacing);
                    this->SetOverlap((overflowBelow + overflowAbove + minSpaceBetween) - spacing);
                }
                ++i;
            }
        }
    }

    params->m_previous = this;

    return FUNCTOR_SIBLINGS;
}

int StaffAlignment::AlignVerticallyEnd(FunctorParams *functorParams)
{
    AlignVerticallyParams *params = vrv_params_cast<AlignVerticallyParams *>(functorParams);
    assert(params);

    params->m_cumulatedShift += this->GetMinimumSpacing(params->m_doc);

    this->SetYRel(-params->m_cumulatedShift);

    params->m_cumulatedShift += m_staffHeight;
    params->m_staffIdx++;

    return FUNCTOR_CONTINUE;
}

int StaffAlignment::AdjustYPos(FunctorParams *functorParams)
{
    AdjustYPosParams *params = vrv_params_cast<AdjustYPosParams *>(functorParams);
    assert(params);

    const int defaultSpacing = this->GetMinimumSpacing(params->m_doc);
    int minSpacing = this->CalcMinimumRequiredSpacing(params->m_doc);
    minSpacing = std::max(this->GetRequestedSpacing(), minSpacing);

    if (minSpacing > defaultSpacing) {
        params->m_cumulatedShift += minSpacing - defaultSpacing;
    }

    this->SetYRel(this->GetYRel() - params->m_cumulatedShift);

    return FUNCTOR_CONTINUE;
}

int StaffAlignment::JustifyY(FunctorParams *functorParams)
{
    JustifyYParams *params = vrv_params_cast<JustifyYParams *>(functorParams);
    assert(params);
    if (params->m_justificationSum <= 0.0) return FUNCTOR_STOP;
    if (params->m_spaceToDistribute <= 0) return FUNCTOR_STOP;

    // Skip bottom aligner and first staff
    if (m_staff && (m_spacingType != SystemAligner::SpacingType::System)) {
        const int shift
            = this->GetJustificationFactor(params->m_doc) / params->m_justificationSum * params->m_spaceToDistribute;
        params->m_relativeShift += shift;
        params->m_cumulatedShift += shift;

        this->SetYRel(this->GetYRel() - params->m_relativeShift);
    }

    params->m_shiftForStaff[this] = params->m_cumulatedShift;

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
