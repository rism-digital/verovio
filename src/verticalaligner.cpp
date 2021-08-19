/////////////////////////////////////////////////////////////////////////////
// Name:        verticalaligner.cpp
// Author:      Laurent Pugin
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "verticalaligner.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>
#include <utility>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "doc.h"
#include "floatingobject.h"
#include "functorparams.h"
#include "scoredef.h"
#include "slur.h"
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
    Reset();
}

SystemAligner::~SystemAligner() {}

void SystemAligner::Reset()
{
    Object::Reset();
    m_spacingTypes.clear();
    m_system = NULL;
    m_bottomAlignment = NULL;
    m_bottomAlignment = GetStaffAlignment(0, NULL, NULL);
}

StaffAlignment *SystemAligner::GetStaffAlignment(int idx, Staff *staff, Doc *doc)
{
    ArrayOfObjects *children = this->GetChildrenForModification();

    // The last one is always the bottomAlignment (unless if not created)
    if (m_bottomAlignment) {
        // remove it temporarily
        children->pop_back();
    }

    if (idx < GetChildCount()) {
        children->push_back(m_bottomAlignment);
        return dynamic_cast<StaffAlignment *>(GetChildren()->at(idx));
    }
    // check that we are searching for the next one (not a gap)
    assert(idx == GetChildCount());
    // LogDebug("Creating staff alignment");

    // This is the first time we are looking for it (e.g., first staff)
    // We create the StaffAlignment
    StaffAlignment *alignment = new StaffAlignment();
    alignment->SetStaff(staff, doc, GetAboveSpacingType(staff));
    alignment->SetParent(this);
    alignment->SetParentSystem(GetSystem());
    children->push_back(alignment);

    if (m_bottomAlignment) {
        children->push_back(m_bottomAlignment);
    }

    return alignment;
}

StaffAlignment *SystemAligner::GetStaffAlignmentForStaffN(int staffN) const
{
    StaffAlignment *alignment = NULL;
    for (int i = 0; i < this->GetChildCount(); ++i) {
        alignment = vrv_cast<StaffAlignment *>(GetChildren()->at(i));
        assert(alignment);

        if ((alignment->GetStaff()) && (alignment->GetStaff()->GetN() == staffN)) return alignment;
    }
    LogDebug("Staff alignment for staff %d not found", staffN);
    return NULL;
}

System *SystemAligner::GetSystem()
{
    if (m_system == NULL) {
        m_system = dynamic_cast<System *>(GetFirstAncestor(SYSTEM));
    }
    return m_system;
}

void SystemAligner::FindAllPositionerPointingTo(ArrayOfFloatingPositioners *positioners, FloatingObject *object)
{
    assert(positioners);

    positioners->clear();

    StaffAlignment *alignment = NULL;
    for (const auto child : *this->GetChildren()) {
        alignment = vrv_cast<StaffAlignment *>(child);
        assert(alignment);
        FloatingPositioner *positioner = alignment->GetCorrespFloatingPositioner(object);
        if (positioner && (positioner->GetObject() == object)) {
            positioners->push_back(positioner);
        }
    }
}

void SystemAligner::FindAllIntersectionPoints(
    SegmentedLine &line, BoundingBox &boundingBox, const std::vector<ClassId> &classIds, int margin)
{
    StaffAlignment *alignment = NULL;
    for (const auto child : *this->GetChildren()) {
        alignment = vrv_cast<StaffAlignment *>(child);
        assert(alignment);
        alignment->FindAllIntersectionPoints(line, boundingBox, classIds, margin);
    }
}

int SystemAligner::GetOverflowAbove(const Doc *) const
{
    if (!GetChildCount() || GetChild(0) == m_bottomAlignment) return 0;

    StaffAlignment *alignment = vrv_cast<StaffAlignment *>(GetChild(0));
    assert(alignment);
    return alignment->GetOverflowAbove();
}

int SystemAligner::GetOverflowBelow(const Doc *doc) const
{
    if (!GetChildCount() || GetChild(0) == m_bottomAlignment) return 0;

    StaffAlignment *alignment = vrv_cast<StaffAlignment *>(GetChild(GetChildCount() - 2));
    assert(alignment);
    return alignment->GetOverflowBelow() + doc->GetBottomMargin(STAFF) * doc->GetDrawingUnit(alignment->GetStaffSize());
}

double SystemAligner::GetJustificationSum(const Doc *doc) const
{
    assert(doc);

    double justificationSum = 0.;
    for (const auto child : *this->GetChildren()) {
        StaffAlignment *alignment = dynamic_cast<StaffAlignment *>(child);
        justificationSum += alignment ? alignment->GetJustificationFactor(doc) : 0.;
    }

    return justificationSum;
}
void SystemAligner::SetSpacing(ScoreDef *scoreDef)
{
    assert(scoreDef);

    m_spacingTypes.clear();

    const ArrayOfObjects *childList = scoreDef->GetList(scoreDef);
    for (auto iter = childList->begin(); iter != childList->end(); ++iter) {
        // It should be staffDef only, but double check.
        if (!(*iter)->Is(STAFFDEF)) continue;
        StaffDef *staffDef = vrv_cast<StaffDef *>(*iter);
        assert(staffDef);

        m_spacingTypes[staffDef->GetN()] = CalculateSpacingAbove(staffDef);
    }
}

SystemAligner::SpacingType SystemAligner::GetAboveSpacingType(Staff *staff)
{
    if (!staff) return SpacingType::None;

    if (m_spacingTypes.empty()) {
        System *system = dynamic_cast<System *>(staff->GetFirstAncestor(SYSTEM));
        ScoreDef *scoreDef = system ? system->GetDrawingScoreDef() : NULL;
        SetSpacing(scoreDef);
    }

    auto iter = m_spacingTypes.find(staff->GetN());
    if (iter == m_spacingTypes.end()) {
        LogWarning("No spacing type found matching @n=%d for '<%s>'", staff->GetN(), staff->GetUuid().c_str());
        return SpacingType::None;
    }

    return iter->second;
}

SystemAligner::SpacingType SystemAligner::CalculateSpacingAbove(StaffDef *staffDef) const
{
    assert(staffDef);

    SpacingType spacingType = SpacingType::None;
    if (staffDef->GetDrawingVisibility() != OPTIMIZATION_HIDDEN) {
        Object *staffChild = staffDef;
        Object *staffParent = staffChild->GetParent();
        bool notFirstInGroup = false;
        VisibleStaffDefOrGrpObject matchType;
        while (spacingType == SpacingType::None) {
            matchType.Skip(staffParent);
            Object *firstVisible = staffParent->FindDescendantByComparison(&matchType, 1);

            // for first child in staff group parent's symbol should be taken, except
            // when we had a child which not on the first place in group, than take first symbol
            notFirstInGroup = notFirstInGroup || (firstVisible && firstVisible != staffChild);
            if (notFirstInGroup) {
                StaffGrp *staffGrp = dynamic_cast<StaffGrp *>(staffParent);
                if (staffGrp && staffGrp->GetFirst(GRPSYM)) {
                    GrpSym *grpSym = vrv_cast<GrpSym *>(staffGrp->GetFirst(GRPSYM));
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

    m_overflowAbove = 0;
    m_overflowBelow = 0;
    m_staffHeight = 0;
    m_overlap = 0;
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
}

void StaffAlignment::SetStaff(Staff *staff, Doc *doc, SystemAligner::SpacingType spacingType)
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
    System *system = GetParentSystem();
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
        if (m_spacingType != SystemAligner::SpacingType::System) justificationFactor *= GetStaffSize() / 100.0;
    }

    return justificationFactor;
}

int StaffAlignment::CalcOverflowAbove(BoundingBox *box)
{
    if (box->Is(FLOATING_POSITIONER)) {
        FloatingPositioner *positioner = vrv_cast<FloatingPositioner *>(box);
        assert(positioner);
        return positioner->GetContentTop() - this->GetYRel();
    }
    return box->GetSelfTop() - this->GetYRel();
}

int StaffAlignment::CalcOverflowBelow(BoundingBox *box)
{
    if (box->Is(FLOATING_POSITIONER)) {
        FloatingPositioner *positioner = vrv_cast<FloatingPositioner *>(box);
        assert(positioner);
        return -(positioner->GetContentBottom() + m_staffHeight - this->GetYRel());
    }
    return -(box->GetSelfBottom() + m_staffHeight - this->GetYRel());
}

int StaffAlignment::GetMinimumStaffSpacing(const Doc *doc, const AttSpacing *attSpacing) const
{
    const auto &option = doc->GetOptions()->m_spacingStaff;
    int spacing = option.GetValue() * doc->GetDrawingUnit(GetStaffSize());

    if (!option.IsSet() && attSpacing->HasSpacingStaff()) {
        spacing = attSpacing->GetSpacingStaff() * doc->GetDrawingUnit(100);
    }
    return spacing;
}

int StaffAlignment::GetMinimumSpacing(const Doc *doc) const
{
    assert(doc);

    int spacing = 0;
    if (m_staff && m_staff->m_drawingStaffDef) {
        // Default or staffDef spacing
        if (m_staff->m_drawingStaffDef->HasSpacing()) {
            spacing = m_staff->m_drawingStaffDef->GetSpacing() * doc->GetDrawingUnit(100);
        }
        else {
            const AttSpacing *scoreDefSpacing = GetAttSpacing();
            switch (m_spacingType) {
                case SystemAligner::SpacingType::System: {
                    spacing = GetParentSystem()->GetMinimumSystemSpacing(doc);
                    break;
                }
                case SystemAligner::SpacingType::Staff: {
                    spacing = GetMinimumStaffSpacing(doc, scoreDefSpacing);
                    break;
                }
                case SystemAligner::SpacingType::Brace: {
                    const auto &option = doc->GetOptions()->m_spacingBraceGroup;
                    spacing = option.IsSet() ? option.GetValue() * doc->GetDrawingUnit(GetStaffSize())
                                             : GetMinimumStaffSpacing(doc, scoreDefSpacing);
                    break;
                }
                case SystemAligner::SpacingType::Bracket: {
                    const auto &option = doc->GetOptions()->m_spacingBracketGroup;
                    spacing = option.IsSet() ? option.GetValue() * doc->GetDrawingUnit(GetStaffSize())
                                             : GetMinimumStaffSpacing(doc, scoreDefSpacing);
                    break;
                }
                case SystemAligner::SpacingType::None: break;
                default: assert(false);
            }
        }
    }

    return spacing;
}

int StaffAlignment::CalcMinimumRequiredSpacing(const Doc *doc) const
{
    assert(doc);

    Object *parent = GetParent();
    assert(parent);

    StaffAlignment *prevAlignment = dynamic_cast<StaffAlignment *>(parent->GetPrevious(this));

    if (!prevAlignment) {
        return GetOverflowAbove() + GetOverlap();
    }

    int overflowSum = 0;
    const bool verseCollapse = doc->GetOptions()->m_lyricVerseCollapse.GetValue();
    if (prevAlignment->GetVerseCount(verseCollapse) > 0) {
        overflowSum = prevAlignment->GetOverflowBelow() + GetOverflowAbove();
    }
    else {
        // The maximum between the overflow below of the previous staff and the overflow above of the current
        overflowSum = std::max(prevAlignment->GetOverflowBelow(), GetOverflowAbove());
        // add overlap if there any
        overflowSum += GetOverlap();
    }

    // Add a margin
    overflowSum += doc->GetBottomMargin(STAFF) * doc->GetDrawingUnit(GetStaffSize());

    return overflowSum;
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
    }
    positioner->SetObjectXY(objectX, objectY);
    // LogDebug("BB %d", item->second.m_contentBB_x1);
    object->SetCurrentFloatingPositioner(positioner);
}

FloatingPositioner *StaffAlignment::FindFirstFloatingPositioner(ClassId classId)
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

FloatingPositioner *StaffAlignment::GetCorrespFloatingPositioner(FloatingObject *object)
{
    auto item = std::find_if(m_floatingPositioners.begin(), m_floatingPositioners.end(),
        [object](FloatingPositioner *positioner) { return positioner->GetObject() == object; });
    if (item != m_floatingPositioners.end()) {
        return *item;
    }
    return NULL;
}

void StaffAlignment::FindAllIntersectionPoints(
    SegmentedLine &line, BoundingBox &boundingBox, const std::vector<ClassId> &classIds, int margin)
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

            if ((*iter)->GetObject()->Is({ PHRASE, SLUR })) {
                Slur *slur = vrv_cast<Slur *>((*iter)->GetObject());
                assert(slur);
                slur->GetCrossStaffOverflows(this, curve->GetDir(), skipAbove, skipBelow);
            }
            else if ((*iter)->GetObject()->Is({ LV, TIE })) {
                Tie *tie = vrv_cast<Tie *>((*iter)->GetObject());
                assert(tie);
                tie->GetCrossStaffOverflows(this, curve->GetDir(), skipAbove, skipBelow);
            }

            int overflowAbove = 0;
            if (!skipAbove) overflowAbove = this->CalcOverflowAbove((*iter));
            if (overflowAbove > params->m_doc->GetDrawingStaffLineWidth(staffSize) / 2) {
                // LogMessage("%sparams->m_doc top overflow: %d", this->GetUuid().c_str(), overflowAbove);
                this->SetOverflowAbove(overflowAbove);
                m_overflowAboveBBoxes.push_back((*iter));
            }

            int overflowBelow = 0;
            if (!skipBelow) overflowBelow = this->CalcOverflowBelow((*iter));
            if (overflowBelow > params->m_doc->GetDrawingStaffLineWidth(staffSize) / 2) {
                // LogMessage("%s bottom overflow: %d", this->GetUuid().c_str(), overflowBelow);
                this->SetOverflowBelow(overflowBelow);
                m_overflowBelowBBoxes.push_back((*iter));
            }
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
        auto i = overflowBoxes->begin();
        auto end = overflowBoxes->end();
        while (i != end) {
            // find all the overflowing elements from the staff that overlap horizontally (and, in case of extender
            // elements - vertically)
            const int margin = ((*iter)->GetObject()->Is(DYNAM) && GetFirstAncestor(BEAM))
                ? params->m_doc->GetDrawingDoubleUnit(m_staff->m_drawingStaffSize)
                : 0;
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
            grpIdYRel.push_back(std::make_pair(currentGrpId, (*iter)->GetDrawingYRel()));
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

    std::vector<FloatingCurvePositioner *> positioners;
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

        bool adjusted = slur->AdjustSlur(params->m_doc, curve, this->GetStaff());
        if (adjusted) {
            params->m_adjusted = true;
        }
        if (curve->IsCrossStaff()) {
            params->m_crossStaffSlurs = true;
        }
    }

    Staff *staff = GetStaff();
    if (staff) {
        const int slurShift = staff->m_drawingStaffSize / 2;
        for (size_t i = 0; i + 1 < positioners.size(); i++) {
            Slur *firstSlur = vrv_cast<Slur *>(positioners[i]->GetObject());
            for (auto j = i + 1; j < positioners.size(); j++) {
                Slur *secondSlur = vrv_cast<Slur *>(positioners[j]->GetObject());
                Point points1[4], points2[4];
                positioners[i]->GetPoints(points1);
                positioners[j]->GetPoints(points2);
                if (firstSlur->GetStart() == secondSlur->GetStart()) {
                    FloatingCurvePositioner *positioner = positioners[points1[2].x > points2[2].x ? i : j];
                    positioner->MoveFrontVertical(
                        positioner->GetDir() == curvature_CURVEDIR_below ? -slurShift : slurShift);
                }
                else if (firstSlur->GetEnd() == secondSlur->GetEnd()) {
                    FloatingCurvePositioner *positioner = positioners[points1[0].x < points2[0].x ? i : j];
                    positioner->MoveBackVertical(
                        positioner->GetDir() == curvature_CURVEDIR_below ? -slurShift : slurShift);
                }
            }
        }
    }

    return FUNCTOR_SIBLINGS;
}

int StaffAlignment::AdjustStaffOverlap(FunctorParams *functorParams)
{
    AdjustStaffOverlapParams *params = vrv_params_cast<AdjustStaffOverlapParams *>(functorParams);
    assert(params);

    // This is the bottom alignment (or something is wrong)
    if (!m_staff) return FUNCTOR_STOP;

    if (params->m_previous == NULL) {
        params->m_previous = this;
        return FUNCTOR_SIBLINGS;
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
                int spacing = std::max(params->m_previous->m_overflowBelow, m_overflowAbove);
                if (spacing < (overflowBelow + overflowAbove)) {
                    // LogDebug("Overlap %d", (overflowBelow + overflowAbove) - spacing);
                    this->SetOverlap((overflowBelow + overflowAbove) - spacing);
                }
                i++;
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

    if (m_spacingType != SystemAligner::SpacingType::System) {
        params->m_cumulatedShift += GetMinimumSpacing(params->m_doc);
    }

    SetYRel(-params->m_cumulatedShift);

    params->m_cumulatedShift += m_staffHeight;
    params->m_staffIdx++;

    return FUNCTOR_CONTINUE;
}

int StaffAlignment::AdjustYPos(FunctorParams *functorParams)
{
    AdjustYPosParams *params = vrv_params_cast<AdjustYPosParams *>(functorParams);
    assert(params);

    const int defaultSpacing = GetMinimumSpacing(params->m_doc);
    const int minSpacing = CalcMinimumRequiredSpacing(params->m_doc);

    if (m_spacingType == SystemAligner::SpacingType::System) {
        params->m_cumulatedShift += minSpacing;
    }
    else if (minSpacing > defaultSpacing) {
        params->m_cumulatedShift += minSpacing - defaultSpacing;
    }

    SetYRel(GetYRel() - params->m_cumulatedShift);

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
