/////////////////////////////////////////////////////////////////////////////
// Name:        adjustfloatingpositionerfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustfloatingpositionerfunctor.h"

//----------------------------------------------------------------------------

#include <map>
#include <set>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "dir.h"
#include "doc.h"
#include "measure.h"
#include "scoredef.h"
#include "staff.h"
#include "staffdef.h"
#include "system.h"
#include "timeinterface.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustFloatingPositionersFunctor
//----------------------------------------------------------------------------

const std::vector<std::pair<ClassId, data_STAFFITEM>> AdjustFloatingPositionersFunctor::s_defaultStaffItemSteps
    = { { LV, STAFFITEM_lv }, { TIE, STAFFITEM_tie }, { SLUR, STAFFITEM_NONE }, { PHRASE, STAFFITEM_NONE },
          { ACCID_FLOATING, STAFFITEM_accid }, { MORDENT, STAFFITEM_mordent }, { TURN, STAFFITEM_turn },
          { TRILL, STAFFITEM_trill }, { ORNAM, STAFFITEM_ornam }, { FING, STAFFITEM_fing }, { DYNAM, STAFFITEM_dynam },
          { HAIRPIN, STAFFITEM_hairpin }, { BRACKETSPAN, STAFFITEM_bracketSpan }, { OCTAVE, STAFFITEM_octave },
          { BREATH, STAFFITEM_breath }, { FERMATA, STAFFITEM_fermata }, { DIR, STAFFITEM_NONE },
          { CPMARK, STAFFITEM_cpMark }, { REPEATMARK, STAFFITEM_NONE }, { TEMPO, STAFFITEM_tempo },
          { PEDAL, STAFFITEM_pedal }, { HARM, STAFFITEM_harm }, { ENDING, STAFFITEM_NONE }, { REH, STAFFITEM_reh },
          { CAESURA, STAFFITEM_NONE }, { ANNOTSCORE, STAFFITEM_annot } };

AdjustFloatingPositionersFunctor::AdjustFloatingPositionersFunctor(Doc *doc) : DocFunctor(doc)
{
    m_classId = OBJECT;
    m_inBetween = false;
    m_place = STAFFREL_NONE;
    m_includeWithin = false;
    m_positioners = NULL;
    m_keepGroupPosition = false;
    m_movePositionerGroups = true;
    m_detectExplicitGroups = false;
    m_hasExplicitGroups = false;
    m_useStaffItemOrder = false;
}

FunctorCode AdjustFloatingPositionersFunctor::VisitStaffAlignment(StaffAlignment *staffAlignment)
{
    if (m_detectExplicitGroups) {
        // Explicit groups need the grouping path only when they combine classes that are not already aligned by the
        // legacy processing, i.e., anything else than dynam and hairpin, dir, or pedal groups
        std::map<std::pair<data_STAFFREL, int>, int> groupBuckets;
        for (const FloatingPositioner *positioner : staffAlignment->GetFloatingPositioners()) {
            assert(positioner->GetObject());
            const int groupId = positioner->GetObject()->GetDrawingGrpId();
            if (groupId >= 0) continue;

            int bucket = 0;
            if (positioner->GetObject()->IsAnyOf(std::array{ DYNAM, HAIRPIN })) bucket = 1;
            if (positioner->GetObject()->Is(DIR)) bucket = 2;
            if (positioner->GetObject()->Is(PEDAL)) bucket = 3;

            const std::pair<data_STAFFREL, int> key = { positioner->GetDrawingPlace(), groupId };
            std::map<std::pair<data_STAFFREL, int>, int>::const_iterator iter = groupBuckets.find(key);
            if (iter == groupBuckets.end()) {
                groupBuckets[key] = bucket;
            }
            else if ((bucket == 0) || (iter->second != bucket)) {
                m_hasExplicitGroups = true;
                break;
            }
        }
        return FUNCTOR_SIBLINGS;
    }
    if (m_useStaffItemOrder) return this->AdjustStaffItemOrder(staffAlignment);
    return this->AdjustCurrentPositioners(staffAlignment);
}

FunctorCode AdjustFloatingPositionersFunctor::AdjustCurrentPositioners(StaffAlignment *staffAlignment)
{
    const int staffSize = staffAlignment->GetStaffSize();
    const int drawingUnit = m_doc->GetDrawingUnit(staffSize);

    if (!m_positioners) staffAlignment->SortPositioners();

    if (m_classId == SYL) {
        const bool verseCollapse = m_doc->GetOptions()->m_lyricVerseCollapse.GetValue();
        if (staffAlignment->GetVerseCount(verseCollapse) > 0) {
            Staff *staff = staffAlignment->GetStaff();
            FontInfo lyricFont = m_doc->GetDrawingTextFont(staff->m_drawingStaffSize, staff->m_drawingStaffDef, true);
            int verseHeight = m_doc->GetTextGlyphHeight(L'I', &lyricFont, false)
                - m_doc->GetTextGlyphDescender(L'q', &lyricFont, false);
            verseHeight *= m_doc->GetOptions()->m_lyricHeightFactor.GetValue();
            if (staffAlignment->GetVerseCountAbove(verseCollapse)) {
                int margin = m_doc->GetTopMargin(SYL) * drawingUnit;
                int minMargin = std::max((int)(m_doc->GetOptions()->m_lyricTopMinMargin.GetValue() * drawingUnit),
                    staffAlignment->GetOverflowAbove());
                staffAlignment->SetOverflowAbove(
                    minMargin + staffAlignment->GetVerseCountAbove(verseCollapse) * (verseHeight + margin));
                // For now just clear the overflowBelow, which avoids the overlap to be calculated. We could also keep
                // them and check if they are some lyrics in order to know if the overlap needs to be calculated or not.
                staffAlignment->ClearBBoxesAbove();
            }
            if (staffAlignment->GetVerseCountBelow(verseCollapse)) {
                int margin = m_doc->GetBottomMargin(SYL) * drawingUnit;
                int minMargin = std::max((int)(m_doc->GetOptions()->m_lyricTopMinMargin.GetValue() * drawingUnit),
                    staffAlignment->GetOverflowBelow());
                staffAlignment->SetOverflowBelow(
                    minMargin + staffAlignment->GetVerseCountBelow(verseCollapse) * (verseHeight + margin));
                // For now just clear the overflowBelow, which avoids the overlap to be calculated. We could also keep
                // them and check if there are some lyrics in order to know if the overlap needs to be calculated or
                // not.
                staffAlignment->ClearBBoxesBelow();
            }
        }
        return FUNCTOR_SIBLINGS;
    }

    const ArrayOfFloatingPositioners &positioners
        = m_positioners ? *m_positioners : staffAlignment->GetFloatingPositioners();
    for (FloatingPositioner *positioner : positioners) {
        assert(positioner->GetObject());
        if ((m_classId != OBJECT) && !positioner->GetObject()->Is(m_classId)) continue;

        if (m_place != STAFFREL_NONE) {
            if ((positioner->GetDrawingPlace() != m_place)
                && !(m_includeWithin && (positioner->GetDrawingPlace() == STAFFREL_within))) {
                continue;
            }
        }
        else if (m_inBetween) {
            if (positioner->GetDrawingPlace() != STAFFREL_between) continue;
        }
        else {
            if (positioner->GetDrawingPlace() == STAFFREL_between) continue;
        }

        // Skip if no content bounding box is available
        if (!positioner->HasContentBB()) continue;

        // for slurs and ties we do not need to adjust them, only add them to the overflow boxes if required
        if ((m_classId == LV) || (m_classId == PHRASE) || (m_classId == SLUR) || (m_classId == TIE)) {

            assert(positioner->Is(FLOATING_CURVE_POSITIONER));
            FloatingCurvePositioner *curve = vrv_cast<FloatingCurvePositioner *>(positioner);
            assert(curve);

            bool skipAbove = false;
            bool skipBelow = false;

            if (positioner->GetObject()->IsAnyOf(std::array{ LV, PHRASE, SLUR, TIE })) {
                TimeSpanningInterface *interface = positioner->GetObject()->GetTimeSpanningInterface();
                assert(interface);
                interface->GetCrossStaffOverflows(staffAlignment, curve->GetDir(), skipAbove, skipBelow);
            }

            int overflowAbove = 0;
            if (!skipAbove) overflowAbove = staffAlignment->CalcOverflowAbove(positioner);
            if (overflowAbove > m_doc->GetDrawingStaffLineWidth(staffSize) / 2) {
                staffAlignment->SetOverflowAbove(overflowAbove);
                staffAlignment->AddBBoxAbove(positioner);
            }

            int overflowBelow = 0;
            if (!skipBelow) overflowBelow = staffAlignment->CalcOverflowBelow(positioner);
            if (overflowBelow > m_doc->GetDrawingStaffLineWidth(staffSize) / 2) {
                staffAlignment->SetOverflowBelow(overflowBelow);
                staffAlignment->AddBBoxBelow(positioner);
            }

            int spaceAbove = 0;
            int spaceBelow = 0;
            std::tie(spaceAbove, spaceBelow) = curve->CalcRequestedStaffSpace(staffAlignment);
            staffAlignment->SetRequestedSpaceAbove(spaceAbove);
            staffAlignment->SetRequestedSpaceBelow(spaceBelow);

            continue;
        }

        const data_STAFFREL place = positioner->GetDrawingPlace();
        ArrayOfBoundingBoxes &overflowBoxes = (place == STAFFREL_above)
            ? staffAlignment->GetBBoxesAboveForModification()
            : staffAlignment->GetBBoxesBelowForModification();

        const int groupIndex = (m_keepGroupPosition) ? this->GetPositionerGroupIndex(positioner) : VRV_UNSET;
        if (groupIndex != VRV_UNSET) {
            if (!m_movePositionerGroups) {
                overflowBoxes.push_back(positioner);
                if (place == STAFFREL_above) {
                    staffAlignment->SetOverflowAbove(staffAlignment->CalcOverflowAbove(positioner));
                }
                else {
                    staffAlignment->SetOverflowBelow(staffAlignment->CalcOverflowBelow(positioner));
                }
                continue;
            }
            for (BoundingBox *bbox : overflowBoxes) {
                if (!positioner->HasHorizontalOverlapWith(bbox, drawingUnit)) continue;
                const FloatingPositioner *other = NULL;
                if (bbox->IsAnyOf(std::array{ FLOATING_POSITIONER, FLOATING_CURVE_POSITIONER })) {
                    other = vrv_cast<const FloatingPositioner *>(bbox);
                    assert(other);
                }
                // The members of a group do not push each other
                if (other && (this->GetPositionerGroupIndex(other) == groupIndex)) continue;
                const int previousYRel = positioner->GetDrawingYRel();
                if (other) MovePositionerPastBoundingBox(positioner, other);
                positioner->CalcDrawingYRel(m_doc, staffAlignment, bbox);
                if (positioner->GetDrawingYRel() == previousYRel) continue;

                // Move the other members of the group by the same amount
                for (FloatingPositioner *member : m_positionerGroups.at(groupIndex)) {
                    if (member == positioner) continue;
                    member->SetDrawingYRel(member->GetDrawingYRel() + positioner->GetDrawingYRel() - previousYRel);
                }
            }

            overflowBoxes.push_back(positioner);
            if (place == STAFFREL_above) {
                staffAlignment->SetOverflowAbove(staffAlignment->CalcOverflowAbove(positioner));
            }
            else {
                staffAlignment->SetOverflowBelow(staffAlignment->CalcOverflowBelow(positioner));
            }
            continue;
        }

        // This sets the default position (without considering any overflowing box)
        positioner->CalcDrawingYRel(m_doc, staffAlignment, NULL);

        // Handle within placement (ignore collisions for certain classes)
        if (place == STAFFREL_within) {
            if (m_classId == CPMARK) continue;
            if (m_classId == DIR) continue;
            if (m_classId == HAIRPIN) continue;
        }

        // Find all the overflowing elements from the staff that overlap horizontally
        for (BoundingBox *bbox : overflowBoxes) {
            if (positioner->HasHorizontalOverlapWith(bbox, drawingUnit)) {
                if (m_keepGroupPosition
                    && bbox->IsAnyOf(std::array{ FLOATING_POSITIONER, FLOATING_CURVE_POSITIONER })) {
                    const FloatingPositioner *grouped = vrv_cast<const FloatingPositioner *>(bbox);
                    assert(grouped);
                    if (grouped->GetObject() && (grouped->GetObject()->GetDrawingGrpId() != 0)) {
                        MovePositionerPastBoundingBox(positioner, grouped);
                    }
                }
                // update the yRel accordingly
                positioner->CalcDrawingYRel(m_doc, staffAlignment, bbox);
            }
        }

        // Vertically align extender elements across systems
        positioner->AdjustExtenders();

        //  Now update the staffAlignment max overflow (above or below) and add the positioner to the list of
        //  overflowing elements
        if (place == STAFFREL_above) {
            int overflowAbove = staffAlignment->CalcOverflowAbove(positioner);
            overflowBoxes.push_back(positioner);
            staffAlignment->SetOverflowAbove(overflowAbove);
        }
        // below (or between)
        else {
            int overflowBelow = staffAlignment->CalcOverflowBelow(positioner);
            overflowBoxes.push_back(positioner);
            staffAlignment->SetOverflowBelow(overflowBelow);
        }
    }

    return FUNCTOR_SIBLINGS;
}

void AdjustFloatingPositionersFunctor::ProcessClass(
    StaffAlignment *staffAlignment, ClassId classId, data_STAFFREL place)
{
    m_classId = classId;
    m_inBetween = (place == STAFFREL_between);
    m_place = place;
    this->AdjustCurrentPositioners(staffAlignment);
}

void AdjustFloatingPositionersFunctor::ProcessPositioners(StaffAlignment *staffAlignment,
    const ArrayOfFloatingPositioners &positioners, data_STAFFREL place, bool keepGroupPosition)
{
    m_keepGroupPosition = keepGroupPosition;
    // Process the positioners one by one, each with the class-specific handling of its object
    for (FloatingPositioner *positioner : positioners) {
        assert(positioner->GetObject());
        const ArrayOfFloatingPositioners current = { positioner };
        m_positioners = &current;
        this->ProcessClass(staffAlignment, positioner->GetObject()->GetClassId(), place);
    }
    m_positioners = NULL;
    m_keepGroupPosition = false;
}

bool AdjustFloatingPositionersFunctor::HasExplicitPositionerGroup(
    StaffAlignment *staffAlignment, data_STAFFREL place) const
{
    for (const FloatingPositioner *positioner : staffAlignment->GetFloatingPositioners()) {
        assert(positioner->GetObject());
        if (positioner->GetDrawingPlace() != place) continue;
        if (positioner->GetObject()->GetDrawingGrpId() < 0) return true;
    }
    return false;
}

void AdjustFloatingPositionersFunctor::BuildPositionerGroups(StaffAlignment *staffAlignment, data_STAFFREL place)
{
    m_positionerGroups.clear();
    m_positionerGroupIndices.clear();

    // Explicit groups (negative ids) are shared across classes. Automatic groups (positive ids) are only unique within
    // the classes aligned together, which is given by the bucket. As with AdjustFloatingPositionerGrpsFunctor, only the
    // positioners of the placement are grouped, never the ones within the staff.
    std::map<std::pair<int, int>, ArrayOfFloatingPositioners> groups;
    for (FloatingPositioner *positioner : staffAlignment->GetFloatingPositioners()) {
        assert(positioner->GetObject());
        if (positioner->GetDrawingPlace() != place) continue;
        const int groupId = positioner->GetObject()->GetDrawingGrpId();
        if (groupId == 0) continue;
        const int bucket = (groupId < 0) ? 0 : GetAutomaticGroupBucket(positioner);
        groups[{ bucket, groupId }].push_back(positioner);
    }

    for (const std::pair<const std::pair<int, int>, ArrayOfFloatingPositioners> &group : groups) {
        if (group.second.size() < 2) continue;
        for (const FloatingPositioner *positioner : group.second) {
            m_positionerGroupIndices[positioner] = (int)m_positionerGroups.size();
        }
        m_positionerGroups.push_back(group.second);
    }
}

int AdjustFloatingPositionersFunctor::GetPositionerGroupIndex(const FloatingPositioner *positioner) const
{
    std::map<const FloatingPositioner *, int>::const_iterator iter = m_positionerGroupIndices.find(positioner);
    return (iter != m_positionerGroupIndices.end()) ? iter->second : VRV_UNSET;
}

void AdjustFloatingPositionersFunctor::StabilizePositionerGroups(StaffAlignment *staffAlignment, data_STAFFREL place,
    const ArrayOfBoundingBoxes &baseBoxes, const ArrayOfFloatingPositioners &positioners)
{
    this->AdjustPositionerGroups(staffAlignment, place);
    this->BuildPositionerGroups(staffAlignment, place);

    ArrayOfBoundingBoxes &overflowBoxes = (place == STAFFREL_above) ? staffAlignment->GetBBoxesAboveForModification()
                                                                    : staffAlignment->GetBBoxesBelowForModification();

    // Moving a group because of a collision can affect positioners processed before, so we process them again until
    // the groups do not move anymore
    const int maxPasses = (m_positionerGroups.empty()) ? 1 : 2;
    bool stable = false;
    for (int pass = 0; pass < maxPasses; ++pass) {
        std::vector<int> previousYRels;
        for (const ArrayOfFloatingPositioners &group : m_positionerGroups) {
            for (const FloatingPositioner *positioner : group) {
                previousYRels.push_back(positioner->GetDrawingYRel());
            }
        }

        overflowBoxes = baseBoxes;
        this->ProcessPositioners(staffAlignment, positioners, place, true);

        stable = true;
        int index = 0;
        for (const ArrayOfFloatingPositioners &group : m_positionerGroups) {
            for (const FloatingPositioner *positioner : group) {
                if (positioner->GetDrawingYRel() != previousYRels.at(index)) stable = false;
                ++index;
            }
        }
        if (stable) break;
    }

    // Conflicting category orders can create a cycle between shared groups. Preserve the group positions selected by
    // the last pass and rebuild the remaining positioners around them without moving a group again.
    if (!stable) {
        m_movePositionerGroups = false;
        overflowBoxes = baseBoxes;
        this->ProcessPositioners(staffAlignment, positioners, place, true);
        m_movePositionerGroups = true;
    }

    m_positionerGroups.clear();
    m_positionerGroupIndices.clear();
}

void AdjustFloatingPositionersFunctor::AdjustPositionerGroups(StaffAlignment *staffAlignment, data_STAFFREL place)
{
    AdjustFloatingPositionerGrpsFunctor adjustGroups(m_doc);
    adjustGroups.SetPlace(place);

    // Explicit MEI @vgrp values are shared across all supported floating-object classes.
    adjustGroups.SetGroupType(-1);
    adjustGroups.SetClassIDs({});
    adjustGroups.VisitStaffAlignment(staffAlignment);

    // Automatically generated groups retain their class-specific legacy behavior.
    adjustGroups.SetGroupType(1);

    if ((place == STAFFREL_above) || (place == STAFFREL_below)) {
        adjustGroups.SetClassIDs({ DYNAM, HAIRPIN });
        adjustGroups.VisitStaffAlignment(staffAlignment);
        adjustGroups.SetClassIDs({ DIR });
        adjustGroups.VisitStaffAlignment(staffAlignment);
        adjustGroups.SetClassIDs({ PEDAL });
        adjustGroups.VisitStaffAlignment(staffAlignment);
        adjustGroups.SetClassIDs({ HARM });
        adjustGroups.VisitStaffAlignment(staffAlignment);
        adjustGroups.SetClassIDs({ ENDING });
        adjustGroups.VisitStaffAlignment(staffAlignment);
    }
    else if (place == STAFFREL_between) {
        adjustGroups.SetClassIDs({ DYNAM });
        adjustGroups.VisitStaffAlignment(staffAlignment);
    }
}

bool AdjustFloatingPositionersFunctor::HasStaffItemOrder(StaffAlignment *staffAlignment, data_STAFFREL place) const
{
    const System *system = staffAlignment->GetParentSystem();
    const Staff *staff = staffAlignment->GetStaff();
    if (!system || !staff) return false;

    const ListOfConstObjects measures = system->FindAllDescendantsByType(MEASURE);
    for (const Object *object : measures) {
        const Measure *measure = vrv_cast<const Measure *>(object);
        assert(measure);
        const ScoreDef *scoreDef = GetDrawingScoreDef(measure, staff->GetN(), system->GetDrawingScoreDef());
        if (!scoreDef) continue;
        if (!scoreDef->GetStaffItemOrder(staff->GetN(), place).empty()) return true;
    }
    return false;
}

ArrayOfFloatingPositioners AdjustFloatingPositionersFunctor::GetOrderedPositioners(
    StaffAlignment *staffAlignment, data_STAFFREL place, bool includeWithin, bool useStaffItemOrder) const
{
    ArrayOfFloatingPositioners orderedPositioners;
    const System *system = staffAlignment->GetParentSystem();
    const Staff *staff = staffAlignment->GetStaff();
    if (!system || !staff) return orderedPositioners;

    // The rank of each positioner, i.e., the index of its step in the order applying to it
    std::vector<std::pair<int, FloatingPositioner *>> rankedPositioners;
    // The steps of each measure, shared by all the positioners of the measure
    std::map<const Measure *, std::vector<std::pair<ClassId, data_STAFFITEM>>> measureSteps;

    for (FloatingPositioner *positioner : staffAlignment->GetFloatingPositioners()) {
        const data_STAFFREL drawingPlace = positioner->GetDrawingPlace();
        if ((drawingPlace != place) && !(includeWithin && (drawingPlace == STAFFREL_within))) continue;

        // Without an order, the positioners in between keep the encoding order of the legacy processing
        if (!useStaffItemOrder && (place == STAFFREL_between)) {
            rankedPositioners.push_back({ 0, positioner });
            continue;
        }

        const std::vector<std::pair<ClassId, data_STAFFITEM>> *steps = &s_defaultStaffItemSteps;
        // Positioners within the staff are not covered by the orders and keep the legacy class order
        if (useStaffItemOrder && (drawingPlace != STAFFREL_within)) {
            const Measure *measure = GetPositionerMeasure(positioner, system);
            if (!measureSteps.contains(measure)) {
                const ScoreDef *scoreDef = GetDrawingScoreDef(measure, staff->GetN(), system->GetDrawingScoreDef());
                const data_STAFFITEM_List order
                    = (scoreDef) ? scoreDef->GetStaffItemOrder(staff->GetN(), place) : data_STAFFITEM_List();
                measureSteps[measure] = GetStaffItemSteps(order);
            }
            steps = &measureSteps.at(measure);
        }

        for (int rank = 0; rank < (int)steps->size(); ++rank) {
            if (!MatchesStaffItemStep(positioner, steps->at(rank))) continue;
            rankedPositioners.push_back({ rank, positioner });
            break;
        }
    }

    // As with the legacy processing, a rank is processed for the whole system before the next one. Within a rank, the
    // stable sort preserves the order of the positioners in the staff alignment.
    std::stable_sort(rankedPositioners.begin(), rankedPositioners.end(),
        [](const std::pair<int, FloatingPositioner *> &left, const std::pair<int, FloatingPositioner *> &right) {
            return (left.first < right.first);
        });

    for (const std::pair<int, FloatingPositioner *> &rankedPositioner : rankedPositioners) {
        orderedPositioners.push_back(rankedPositioner.second);
    }
    return orderedPositioners;
}

void AdjustFloatingPositionersFunctor::AdjustLegacyPlace(
    StaffAlignment *staffAlignment, data_STAFFREL place, bool includeWithin)
{
    ArrayOfBoundingBoxes &overflowBoxes = (place == STAFFREL_above) ? staffAlignment->GetBBoxesAboveForModification()
                                                                    : staffAlignment->GetBBoxesBelowForModification();
    const ArrayOfBoundingBoxes baseBoxes = overflowBoxes;

    m_includeWithin = includeWithin;
    AdjustFloatingPositionerGrpsFunctor adjustGroups(m_doc);
    adjustGroups.SetGroupType(1);
    adjustGroups.SetPlace(place);

    // Same order and automatic group alignment as in VisitSystem
    for (const std::pair<ClassId, data_STAFFITEM> &step : s_defaultStaffItemSteps) {
        this->ProcessClass(staffAlignment, step.first, place);
        switch (step.first) {
            case HAIRPIN: adjustGroups.SetClassIDs({ DYNAM, HAIRPIN }); break;
            case DIR:
            case PEDAL:
            case HARM:
            case ENDING: adjustGroups.SetClassIDs({ step.first }); break;
            default: continue;
        }
        adjustGroups.VisitStaffAlignment(staffAlignment);
    }

    if (this->HasExplicitPositionerGroup(staffAlignment, place)) {
        const ArrayOfFloatingPositioners positioners
            = this->GetOrderedPositioners(staffAlignment, place, includeWithin, false);
        this->StabilizePositionerGroups(staffAlignment, place, baseBoxes, positioners);
    }
    m_includeWithin = false;
}

void AdjustFloatingPositionersFunctor::AdjustOrderedPlace(
    StaffAlignment *staffAlignment, data_STAFFREL place, bool includeWithin)
{
    ArrayOfBoundingBoxes &overflowBoxes = (place == STAFFREL_above) ? staffAlignment->GetBBoxesAboveForModification()
                                                                    : staffAlignment->GetBBoxesBelowForModification();
    const ArrayOfBoundingBoxes baseBoxes = overflowBoxes;
    const ArrayOfFloatingPositioners positioners
        = this->GetOrderedPositioners(staffAlignment, place, includeWithin, true);

    m_includeWithin = includeWithin;
    this->ProcessPositioners(staffAlignment, positioners, place, false);
    this->StabilizePositionerGroups(staffAlignment, place, baseBoxes, positioners);
    m_includeWithin = false;
}

FunctorCode AdjustFloatingPositionersFunctor::AdjustStaffItemOrder(StaffAlignment *staffAlignment)
{
    const System *system = staffAlignment->GetParentSystem();
    const Staff *staff = staffAlignment->GetStaff();
    if (!system || !staff) return FUNCTOR_SIBLINGS;

    staffAlignment->SortPositioners();

    if (this->HasStaffItemOrder(staffAlignment, STAFFREL_above)) {
        this->AdjustOrderedPlace(staffAlignment, STAFFREL_above);
    }
    else {
        this->AdjustLegacyPlace(staffAlignment, STAFFREL_above);
    }

    if (this->HasStaffItemOrder(staffAlignment, STAFFREL_below)) {
        this->AdjustOrderedPlace(staffAlignment, STAFFREL_below, true);
    }
    else {
        this->AdjustLegacyPlace(staffAlignment, STAFFREL_below, true);
    }

    // Lyrics always retain their established processing after above/below/within positioners.
    m_place = STAFFREL_NONE;
    m_includeWithin = false;
    m_inBetween = false;
    m_classId = SYL;
    this->AdjustCurrentPositioners(staffAlignment);

    if (this->HasStaffItemOrder(staffAlignment, STAFFREL_between)) {
        this->AdjustOrderedPlace(staffAlignment, STAFFREL_between);
    }
    else {
        const ArrayOfBoundingBoxes baseBoxes = staffAlignment->GetBBoxesBelow();
        this->ProcessClass(staffAlignment, OBJECT, STAFFREL_between);

        AdjustFloatingPositionerGrpsFunctor adjustGroups(m_doc);
        adjustGroups.SetGroupType(1);
        adjustGroups.SetClassIDs({ DYNAM });
        adjustGroups.SetPlace(STAFFREL_between);
        adjustGroups.VisitStaffAlignment(staffAlignment);

        if (this->HasExplicitPositionerGroup(staffAlignment, STAFFREL_between)) {
            const ArrayOfFloatingPositioners positioners
                = this->GetOrderedPositioners(staffAlignment, STAFFREL_between, false, false);
            this->StabilizePositionerGroups(staffAlignment, STAFFREL_between, baseBoxes, positioners);
        }
    }

    m_place = STAFFREL_NONE;
    m_includeWithin = false;
    m_inBetween = false;
    m_classId = OBJECT;
    return FUNCTOR_SIBLINGS;
}

FunctorCode AdjustFloatingPositionersFunctor::VisitSystem(System *system)
{
    m_hasExplicitGroups = false;
    m_detectExplicitGroups = true;
    system->m_systemAligner.Process(*this);
    m_detectExplicitGroups = false;

    if (HasSystemStaffItemOrder(system) || m_hasExplicitGroups) {
        m_useStaffItemOrder = true;
        system->m_systemAligner.Process(*this);
        m_useStaffItemOrder = false;
        return FUNCTOR_SIBLINGS;
    }

    m_place = STAFFREL_NONE;
    m_inBetween = false;

    AdjustFloatingPositionerGrpsFunctor adjustFloatingPositionerGrps(m_doc);

    m_classId = LV;
    system->m_systemAligner.Process(*this);

    m_classId = TIE;
    system->m_systemAligner.Process(*this);

    m_classId = SLUR;
    system->m_systemAligner.Process(*this);

    m_classId = PHRASE;
    system->m_systemAligner.Process(*this);

    m_classId = ACCID_FLOATING;
    system->m_systemAligner.Process(*this);

    m_classId = MORDENT;
    system->m_systemAligner.Process(*this);

    m_classId = TURN;
    system->m_systemAligner.Process(*this);

    m_classId = TRILL;
    system->m_systemAligner.Process(*this);

    m_classId = ORNAM;
    system->m_systemAligner.Process(*this);

    m_classId = FING;
    system->m_systemAligner.Process(*this);

    m_classId = DYNAM;
    system->m_systemAligner.Process(*this);

    m_classId = HAIRPIN;
    system->m_systemAligner.Process(*this);

    adjustFloatingPositionerGrps.SetClassIDs({ DYNAM, HAIRPIN });
    adjustFloatingPositionerGrps.SetPlace(STAFFREL_above);
    system->m_systemAligner.Process(adjustFloatingPositionerGrps);
    adjustFloatingPositionerGrps.SetPlace(STAFFREL_below);
    system->m_systemAligner.Process(adjustFloatingPositionerGrps);

    m_classId = BRACKETSPAN;
    system->m_systemAligner.Process(*this);

    m_classId = OCTAVE;
    system->m_systemAligner.Process(*this);

    m_classId = BREATH;
    system->m_systemAligner.Process(*this);

    m_classId = FERMATA;
    system->m_systemAligner.Process(*this);

    m_classId = DIR;
    system->m_systemAligner.Process(*this);

    adjustFloatingPositionerGrps.SetClassIDs({ DIR });
    adjustFloatingPositionerGrps.SetPlace(STAFFREL_above);
    system->m_systemAligner.Process(adjustFloatingPositionerGrps);
    adjustFloatingPositionerGrps.SetPlace(STAFFREL_below);
    system->m_systemAligner.Process(adjustFloatingPositionerGrps);

    m_classId = CPMARK;
    system->m_systemAligner.Process(*this);

    m_classId = REPEATMARK;
    system->m_systemAligner.Process(*this);

    m_classId = TEMPO;
    system->m_systemAligner.Process(*this);

    m_classId = PEDAL;
    system->m_systemAligner.Process(*this);

    adjustFloatingPositionerGrps.SetClassIDs({ PEDAL });
    adjustFloatingPositionerGrps.SetPlace(STAFFREL_above);
    system->m_systemAligner.Process(adjustFloatingPositionerGrps);
    adjustFloatingPositionerGrps.SetPlace(STAFFREL_below);
    system->m_systemAligner.Process(adjustFloatingPositionerGrps);

    m_classId = HARM;
    system->m_systemAligner.Process(*this);

    adjustFloatingPositionerGrps.SetClassIDs({ HARM });
    adjustFloatingPositionerGrps.SetPlace(STAFFREL_above);
    system->m_systemAligner.Process(adjustFloatingPositionerGrps);
    adjustFloatingPositionerGrps.SetPlace(STAFFREL_below);
    system->m_systemAligner.Process(adjustFloatingPositionerGrps);

    m_classId = ENDING;
    system->m_systemAligner.Process(*this);

    adjustFloatingPositionerGrps.SetClassIDs({ ENDING });
    adjustFloatingPositionerGrps.SetPlace(STAFFREL_above);
    system->m_systemAligner.Process(adjustFloatingPositionerGrps);
    adjustFloatingPositionerGrps.SetPlace(STAFFREL_below);
    system->m_systemAligner.Process(adjustFloatingPositionerGrps);

    m_classId = REH;
    system->m_systemAligner.Process(*this);

    m_classId = CAESURA;
    system->m_systemAligner.Process(*this);

    m_classId = ANNOTSCORE;
    system->m_systemAligner.Process(*this);

    // SYL check if there are some lyrics and make space for them if any
    m_classId = SYL;
    system->m_systemAligner.Process(*this);

    /**** Process elements that need to be put in between ****/

    m_inBetween = true;
    // All of them with no particular processing order.
    // The resulting layout order will correspond to the order in the encoding.
    m_classId = OBJECT;
    system->m_systemAligner.Process(*this);

    adjustFloatingPositionerGrps.SetClassIDs({ DYNAM });
    adjustFloatingPositionerGrps.SetPlace(STAFFREL_between);
    system->m_systemAligner.Process(adjustFloatingPositionerGrps);

    return FUNCTOR_SIBLINGS;
}

std::vector<std::pair<ClassId, data_STAFFITEM>> AdjustFloatingPositionersFunctor::GetStaffItemSteps(
    const data_STAFFITEM_List &order)
{
    if (order.empty()) return s_defaultStaffItemSteps;

    // The listed categories come first, followed by the remaining ones in their default order
    std::vector<std::pair<ClassId, data_STAFFITEM>> steps;
    std::set<data_STAFFITEM> listed;
    for (const data_STAFFITEM item : order) {
        // Duplicates are preserved in the attribute but only the first occurrence is relevant
        if (!listed.insert(item).second) continue;
        if ((item == STAFFITEM_dir) || (item == STAFFITEM_stageDir)) {
            steps.push_back({ DIR, item });
            continue;
        }
        for (const std::pair<ClassId, data_STAFFITEM> &step : s_defaultStaffItemSteps) {
            if (step.second == item) steps.push_back(step);
        }
    }

    for (const std::pair<ClassId, data_STAFFITEM> &step : s_defaultStaffItemSteps) {
        // Dir and stageDir are both rendered by Dir and need to be distinguished only when one of them is listed
        if (step.first == DIR) {
            if (!listed.contains(STAFFITEM_dir)) {
                steps.push_back({ DIR, listed.contains(STAFFITEM_stageDir) ? STAFFITEM_dir : STAFFITEM_NONE });
            }
            else if (!listed.contains(STAFFITEM_stageDir)) {
                steps.push_back({ DIR, STAFFITEM_stageDir });
            }
        }
        else if ((step.second == STAFFITEM_NONE) || !listed.contains(step.second)) {
            steps.push_back(step);
        }
    }
    return steps;
}

bool AdjustFloatingPositionersFunctor::MatchesStaffItemStep(
    const FloatingPositioner *positioner, const std::pair<ClassId, data_STAFFITEM> &step)
{
    assert(positioner);
    assert(positioner->GetObject());

    if (!positioner->GetObject()->Is(step.first)) return false;
    if ((step.second != STAFFITEM_dir) && (step.second != STAFFITEM_stageDir)) return true;

    const Dir *dir = vrv_cast<const Dir *>(positioner->GetObject());
    assert(dir);
    return (dir->IsStageDir() == (step.second == STAFFITEM_stageDir));
}

const Measure *AdjustFloatingPositionersFunctor::GetPositionerMeasure(
    const FloatingPositioner *positioner, const System *system)
{
    assert(positioner);
    assert(positioner->GetObject());
    assert(system);

    const Object *object = positioner->GetObject();
    const Measure *measure = vrv_cast<const Measure *>(object->GetFirstAncestor(MEASURE));
    if (!measure) {
        const TimePointInterface *interface = object->GetTimePointInterface();
        if (interface) measure = interface->GetStartMeasure();
    }
    if (!measure) {
        measure = vrv_cast<const Measure *>(object->FindDescendantByType(MEASURE));
    }

    // An element continued from a previous system uses the order of the first measure of the current system
    if (!measure || (measure->GetFirstAncestor(SYSTEM) != system)) {
        measure = vrv_cast<const Measure *>(system->FindDescendantByType(MEASURE));
    }
    return measure;
}

const ScoreDef *AdjustFloatingPositionersFunctor::GetDrawingScoreDef(
    const Measure *measure, int staffN, const ScoreDef *fallback)
{
    if (!measure) return fallback;

    AttNIntegerComparison matchN(STAFF, staffN);
    const Staff *staff = vrv_cast<const Staff *>(measure->FindDescendantByComparison(&matchN, 1));
    if (!staff || !staff->m_drawingStaffDef) return fallback;

    return vrv_cast<const ScoreDef *>(staff->m_drawingStaffDef->GetFirstAncestor(SCOREDEF));
}

bool AdjustFloatingPositionersFunctor::HasAnyStaffItemOrder(const ScoreDef *scoreDef)
{
    if (!scoreDef) return false;
    if (scoreDef->HasAboveorder() || scoreDef->HasBeloworder() || scoreDef->HasBetweenorder()) return true;

    for (const int staffN : scoreDef->GetStaffNs()) {
        const StaffDef *staffDef = scoreDef->GetStaffDef(staffN);
        if (!staffDef) continue;
        if (staffDef->HasAboveorder() || staffDef->HasBeloworder() || staffDef->HasBetweenorder()) return true;
    }
    return false;
}

bool AdjustFloatingPositionersFunctor::HasSystemStaffItemOrder(const System *system)
{
    assert(system);

    if (HasAnyStaffItemOrder(system->GetDrawingScoreDef())) return true;

    const ListOfConstObjects staves = system->FindAllDescendantsByType(STAFF);
    for (const Object *object : staves) {
        const Staff *staff = vrv_cast<const Staff *>(object);
        assert(staff);
        const Measure *measure = vrv_cast<const Measure *>(staff->GetFirstAncestor(MEASURE));
        if (HasAnyStaffItemOrder(GetDrawingScoreDef(measure, staff->GetN(), NULL))) return true;
    }
    return false;
}

int AdjustFloatingPositionersFunctor::GetAutomaticGroupBucket(const FloatingPositioner *positioner)
{
    assert(positioner);
    assert(positioner->GetObject());

    // The classes aligned together by AdjustFloatingPositionerGrpsFunctor share a bucket
    const Object *object = positioner->GetObject();
    if (object->IsAnyOf(std::array{ DYNAM, HAIRPIN })) return 1;
    if (object->Is(DIR)) return 2;
    if (object->Is(PEDAL)) return 3;
    if (object->Is(HARM)) return 4;
    if (object->Is(ENDING)) return 5;
    return 100 + object->GetClassId();
}

void AdjustFloatingPositionersFunctor::MovePositionerPastBoundingBox(
    FloatingPositioner *positioner, const BoundingBox *bbox)
{
    assert(positioner);
    assert(bbox);

    if (positioner->GetDrawingPlace() == STAFFREL_above) {
        const int shift = bbox->GetContentTop() - positioner->GetContentBottom();
        if (shift > 0) positioner->SetDrawingYRel(positioner->GetDrawingYRel() - shift, true);
    }
    else {
        const int shift = positioner->GetContentTop() - bbox->GetContentBottom();
        if (shift > 0) positioner->SetDrawingYRel(positioner->GetDrawingYRel() + shift, true);
    }
}

//----------------------------------------------------------------------------
// AdjustFloatingPositionerGrpsFunctor
//----------------------------------------------------------------------------

AdjustFloatingPositionerGrpsFunctor::AdjustFloatingPositionerGrpsFunctor(Doc *doc) : DocFunctor(doc)
{
    m_place = STAFFREL_above;
    m_groupType = 0;
}

FunctorCode AdjustFloatingPositionerGrpsFunctor::VisitStaffAlignment(StaffAlignment *staffAlignment)
{
    ArrayOfFloatingPositioners positioners;
    // make a temporary copy of positioners with desired classId and a drawing grpId
    const ArrayOfFloatingPositioners &allPositioners = staffAlignment->GetFloatingPositioners();
    std::copy_if(allPositioners.begin(), allPositioners.end(), std::back_inserter(positioners),
        [this](FloatingPositioner *positioner) {
            assert(positioner->GetObject());
            const int groupId = positioner->GetObject()->GetDrawingGrpId();
            const bool classMatches = m_classIds.empty()
                || (std::find(m_classIds.begin(), m_classIds.end(), positioner->GetObject()->GetClassId())
                    != m_classIds.end());
            const bool groupMatches = (m_groupType == 0) || ((m_groupType < 0) ? (groupId < 0) : (groupId > 0));
            return classMatches && groupMatches && (groupId != 0) && (positioner->GetDrawingPlace() == m_place)
                && !positioner->HasEmptyBB();
        });

    if (positioners.empty()) {
        return FUNCTOR_SIBLINGS;
    }
    // A vector storing a pair with the grpId and the target relative or absolute drawing position
    ArrayOfIntPairs grpIdYRel;

    for (FloatingPositioner *positioner : positioners) {
        int currentGrpId = positioner->GetObject()->GetDrawingGrpId();
        const int currentY = (m_groupType < 0) ? positioner->GetDrawingY() : positioner->GetDrawingYRel();
        // Look if we already have a pair for this grpId
        auto iter = std::find_if(grpIdYRel.begin(), grpIdYRel.end(),
            [currentGrpId](std::pair<int, int> &pair) { return (pair.first == currentGrpId); });
        // if not, then just add a new pair with the YRel of the current positioner
        if (iter == grpIdYRel.end()) {
            grpIdYRel.push_back({ currentGrpId, currentY });
        }
        // else, adjust the min or max YRel of the pair if necessary
        else {
            if ((m_place == STAFFREL_above) != (m_groupType < 0)) {
                if (currentY < (*iter).second) (*iter).second = currentY;
            }
            else {
                if (currentY > (*iter).second) (*iter).second = currentY;
            }
        }
    }

    if (m_groupType < 0) {
        for (FloatingPositioner *positioner : positioners) {
            const int currentGrpId = positioner->GetObject()->GetDrawingGrpId();
            ArrayOfIntPairs::const_iterator iter = std::find_if(grpIdYRel.begin(), grpIdYRel.end(),
                [currentGrpId](const std::pair<int, int> &pair) { return (pair.first == currentGrpId); });
            assert(iter != grpIdYRel.end());
            const int drawingYRel = positioner->GetDrawingYRel() + positioner->GetDrawingY() - iter->second;
            positioner->SetDrawingYRel(drawingYRel, true);
        }
    }
    else if (std::find(m_classIds.begin(), m_classIds.end(), HARM) != m_classIds.end()) {
        // Adjust the position of groups to ensure that any group is positioned further away
        this->AdjustGroupsMonotone(staffAlignment, positioners, grpIdYRel);
        // This already moves them, so the loop below is not necessary.
    }
    else {
        // Now go through all the positioners again and adjust the YRel with the value of the pair
        for (FloatingPositioner *positioner : positioners) {
            int currentGrpId = positioner->GetObject()->GetDrawingGrpId();
            auto iter = std::find_if(grpIdYRel.begin(), grpIdYRel.end(),
                [currentGrpId](std::pair<int, int> &pair) { return (pair.first == currentGrpId); });
            // We must have found it
            assert(iter != grpIdYRel.end());
            positioner->SetDrawingYRel((*iter).second);
        }
    }

    //  Now update the staffAlignment max overflow (above or below)
    for (FloatingPositioner *positioner : positioners) {
        if (m_place == STAFFREL_above) {
            int overflowAbove = staffAlignment->CalcOverflowAbove(positioner);
            staffAlignment->SetOverflowAbove(overflowAbove);
        }
        else {
            int overflowBelow = staffAlignment->CalcOverflowBelow(positioner);
            staffAlignment->SetOverflowBelow(overflowBelow);
        }
    }

    return FUNCTOR_SIBLINGS;
}

void AdjustFloatingPositionerGrpsFunctor::AdjustGroupsMonotone(const StaffAlignment *staffAlignment,
    const ArrayOfFloatingPositioners &positioners, ArrayOfIntPairs &grpIdYRel) const
{
    if (grpIdYRel.empty()) {
        return;
    }

    std::sort(grpIdYRel.begin(), grpIdYRel.end());

    int yRel;
    // The initial next position is the original position of the first group. Nothing will happen for it.
    int nextYRel = grpIdYRel.at(0).second;

    // For each grpId (sorted, see above), loop to find the highest / lowest position to put the next group
    // Then move the next group (if not already higher or lower)
    for (const auto &grp : grpIdYRel) {
        // Check if the next group is not already higher or lower.
        if (m_place == STAFFREL_above) {
            yRel = (nextYRel < grp.second) ? nextYRel : grp.second;
        }
        else {
            yRel = (nextYRel > grp.second) ? nextYRel : grp.second;
        }
        // Go through all the positioners, but filter by group
        for (FloatingPositioner *positioner : positioners) {
            int currentGrpId = positioner->GetObject()->GetDrawingGrpId();
            // Not the grpId we are processing, skip it.
            if (currentGrpId != grp.first) continue;
            // Set its position
            positioner->SetDrawingYRel(yRel);
            // Then find the highest / lowest position for the next group
            if (m_place == STAFFREL_above) {
                int positionerY = yRel - positioner->GetContentY2()
                    - (m_doc->GetTopMargin(positioner->GetObject()->GetClassId())
                        * m_doc->GetDrawingUnit(staffAlignment->GetStaffSize()));
                if (nextYRel > positionerY) {
                    nextYRel = positionerY;
                }
            }
            else {
                int positionerY = yRel + positioner->GetContentY2()
                    + (m_doc->GetBottomMargin(positioner->GetObject()->GetClassId())
                        * m_doc->GetDrawingUnit(staffAlignment->GetStaffSize()));
                if (nextYRel < positionerY) {
                    nextYRel = positionerY;
                }
            }
        }
    }
}

//----------------------------------------------------------------------------
// AdjustFloatingPositionersBetweenFunctor
//----------------------------------------------------------------------------

AdjustFloatingPositionersBetweenFunctor::AdjustFloatingPositionersBetweenFunctor(Doc *doc) : DocFunctor(doc)
{
    m_previousStaffAlignment = NULL;
}

FunctorCode AdjustFloatingPositionersBetweenFunctor::VisitStaffAlignment(StaffAlignment *staffAlignment)
{
    // First staff - nothing to do
    if (m_previousStaffAlignment == NULL) {
        m_previousStaffAlignment = staffAlignment;
        return FUNCTOR_SIBLINGS;
    }
    assert(m_previousStaffAlignment);

    int dist = m_previousStaffAlignment->GetYRel() - staffAlignment->GetYRel();
    dist -= m_previousStaffAlignment->GetStaffHeight();
    int centerYRel = dist / 2 + m_previousStaffAlignment->GetStaffHeight();

    for (FloatingPositioner *positioner : m_previousStaffAlignment->GetFloatingPositioners()) {
        assert(positioner->GetObject());
        if (!positioner->GetObject()->IsAnyOf(std::array{ CPMARK, DIR, DYNAM, HAIRPIN, TEMPO })) continue;

        if (positioner->GetDrawingPlace() != STAFFREL_between) continue;

        // Skip if no content bounding box is available
        if (!positioner->HasContentBB()) continue;

        int diffY = centerYRel - positioner->GetDrawingYRel();

        const ArrayOfBoundingBoxes &overflowBoxes = staffAlignment->GetBBoxesAbove();
        auto i = overflowBoxes.begin();
        auto end = overflowBoxes.end();
        while (i != end) {

            // find all the overflowing elements from the staff that overlap horizontally
            i = std::find_if(
                i, end, [positioner](BoundingBox *elem) { return positioner->HorizontalContentOverlap(elem); });
            if (i != end) {
                // update the yRel accordingly
                const int spaceY = positioner->GetSpaceBelow(m_doc, staffAlignment, *i);
                if (spaceY != VRV_UNSET) {
                    diffY = std::min(diffY, spaceY);
                }
                ++i;
            }
        }
        positioner->SetDrawingYRel(positioner->GetDrawingYRel() + diffY);
    }

    m_previousStaffAlignment = staffAlignment;

    return FUNCTOR_SIBLINGS;
}

FunctorCode AdjustFloatingPositionersBetweenFunctor::VisitSystem(System *system)
{
    m_previousStaffAlignment = NULL;
    system->m_systemAligner.Process(*this);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
