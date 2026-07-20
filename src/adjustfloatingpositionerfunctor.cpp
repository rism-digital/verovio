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

namespace {

    constexpr size_t STAFF_ITEM_GROUP_MAX_PASSES = 2;

    struct StaffItemStep {
        data_STAFFITEM item;
        ClassId classId;
        bool exactItem = false;
    };

    struct StaffItemPositioner {
        FloatingPositioner *positioner;
        ClassId classId;
        data_STAFFITEM item;
        int measureIndex;
        int stepIndex;
        int positionerIndex;
    };

    const std::vector<StaffItemStep> &GetDefaultStaffItemSteps()
    {
        static const std::vector<StaffItemStep> steps{
            { STAFFITEM_lv, LV },
            { STAFFITEM_tie, TIE },
            { STAFFITEM_NONE, SLUR },
            { STAFFITEM_NONE, PHRASE },
            { STAFFITEM_accid, ACCID_FLOATING },
            { STAFFITEM_mordent, MORDENT },
            { STAFFITEM_turn, TURN },
            { STAFFITEM_trill, TRILL },
            { STAFFITEM_ornam, ORNAM },
            { STAFFITEM_fing, FING },
            { STAFFITEM_dynam, DYNAM },
            { STAFFITEM_hairpin, HAIRPIN },
            { STAFFITEM_bracketSpan, BRACKETSPAN },
            { STAFFITEM_octave, OCTAVE },
            { STAFFITEM_breath, BREATH },
            { STAFFITEM_fermata, FERMATA },
            { STAFFITEM_dir, DIR },
            { STAFFITEM_cpMark, CPMARK },
            { STAFFITEM_NONE, REPEATMARK },
            { STAFFITEM_tempo, TEMPO },
            { STAFFITEM_pedal, PEDAL },
            { STAFFITEM_harm, HARM },
            { STAFFITEM_NONE, ENDING },
            { STAFFITEM_reh, REH },
            { STAFFITEM_NONE, CAESURA },
            { STAFFITEM_annot, ANNOTSCORE },
        };
        return steps;
    }

    std::vector<StaffItemStep> GetOrderedStaffItemSteps(const data_STAFFITEM_List &order)
    {
        const std::vector<StaffItemStep> &defaults = GetDefaultStaffItemSteps();
        if (order.empty()) return defaults;

        std::vector<StaffItemStep> steps;
        std::set<data_STAFFITEM> used;
        for (const data_STAFFITEM item : order) {
            if (!used.insert(item).second) continue;
            if (item == STAFFITEM_stageDir) {
                steps.push_back({ STAFFITEM_stageDir, DIR, true });
            }
            else {
                for (const StaffItemStep &step : defaults) {
                    if (step.item == item) steps.push_back({ step.item, step.classId, true });
                }
            }
        }
        for (const StaffItemStep &step : defaults) {
            if ((step.item == STAFFITEM_dir) && (used.contains(STAFFITEM_dir) || used.contains(STAFFITEM_stageDir))) {
                if (!used.contains(STAFFITEM_dir)) steps.push_back({ STAFFITEM_dir, DIR, true });
                if (!used.contains(STAFFITEM_stageDir)) steps.push_back({ STAFFITEM_stageDir, DIR, true });
            }
            else if ((step.item == STAFFITEM_NONE) || !used.contains(step.item)) {
                steps.push_back(step);
            }
        }
        return steps;
    }

    bool MatchesStaffItemStep(const FloatingPositioner *positioner, const StaffItemStep &step)
    {
        assert(positioner);
        assert(positioner->GetObject());
        if (!positioner->GetObject()->Is(step.classId)) return false;
        if (!step.exactItem) return true;
        if ((step.item != STAFFITEM_dir) && (step.item != STAFFITEM_stageDir)) return true;

        const Dir *dir = vrv_cast<const Dir *>(positioner->GetObject());
        assert(dir);
        return (step.item == STAFFITEM_stageDir) ? dir->IsStageDir() : !dir->IsStageDir();
    }

    const Measure *GetPositionerMeasure(const FloatingPositioner *positioner, const Measure *fallback)
    {
        assert(positioner);
        const Object *object = positioner->GetObject();
        assert(object);

        const Measure *measure = vrv_cast<const Measure *>(object->GetFirstAncestor(MEASURE));
        if (measure) return measure;

        const TimePointInterface *interface = object->GetTimePointInterface();
        if (interface && interface->GetStartMeasure()) return interface->GetStartMeasure();

        measure = vrv_cast<const Measure *>(object->FindDescendantByType(MEASURE));
        return measure ? measure : fallback;
    }

    int GetAutomaticGroupBucket(const FloatingPositioner *positioner)
    {
        assert(positioner);
        const Object *object = positioner->GetObject();
        assert(object);
        if (object->IsAnyOf(std::array{ DYNAM, HAIRPIN })) return 1;
        if (object->Is(DIR)) return 2;
        if (object->Is(PEDAL)) return 3;
        if (object->Is(HARM)) return 4;
        if (object->Is(ENDING)) return 5;
        return 100 + object->GetClassId();
    }

    long long GetPositionerGroupKey(const FloatingPositioner *positioner)
    {
        assert(positioner);
        assert(positioner->GetObject());
        const int groupId = positioner->GetObject()->GetDrawingGrpId();
        if (groupId < 0) return groupId;
        return (static_cast<long long>(GetAutomaticGroupBucket(positioner)) << 32) | static_cast<unsigned int>(groupId);
    }

    void MovePositionerPastBoundingBox(FloatingPositioner *positioner, const BoundingBox *bbox)
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

    bool HasStaffItemOrder(const ScoreDef *scoreDef)
    {
        if (!scoreDef) return false;
        if (scoreDef->HasAboveorder() || scoreDef->HasBeloworder() || scoreDef->HasBetweenorder()) return true;

        for (const int staffN : scoreDef->GetStaffNs()) {
            const StaffDef *staffDef = scoreDef->GetStaffDef(staffN);
            if (staffDef && (staffDef->HasAboveorder() || staffDef->HasBeloworder() || staffDef->HasBetweenorder())) {
                return true;
            }
        }
        return false;
    }

    const ScoreDef *GetDrawingScoreDef(const Measure *measure, int staffN, const ScoreDef *fallback)
    {
        if (!measure) return fallback;

        AttNIntegerComparison matchN(STAFF, staffN);
        const Staff *staff = vrv_cast<const Staff *>(measure->FindDescendantByComparison(&matchN, 1));
        if (!staff || !staff->m_drawingStaffDef) return fallback;

        return vrv_cast<const ScoreDef *>(staff->m_drawingStaffDef->GetFirstAncestor(SCOREDEF));
    }

    bool HasSystemStaffItemOrder(const System *system)
    {
        if (!system) return false;
        if (HasStaffItemOrder(system->GetDrawingScoreDef())) return true;

        const ListOfConstObjects staves = system->FindAllDescendantsByType(STAFF);
        for (const Object *object : staves) {
            const Staff *staff = vrv_cast<const Staff *>(object);
            assert(staff);
            if (HasStaffItemOrder(GetDrawingScoreDef(
                    vrv_cast<const Measure *>(staff->GetFirstAncestor(MEASURE)), staff->GetN(), NULL))) {
                return true;
            }
        }
        return false;
    }

    std::vector<StaffItemPositioner> BuildStaffItemPositioners(
        StaffAlignment *staffAlignment, data_STAFFREL place, bool includeWithin, bool useStaffItemOrder)
    {
        std::vector<StaffItemPositioner> result;
        const System *system = staffAlignment->GetParentSystem();
        const Staff *staff = staffAlignment->GetStaff();
        if (!system || !staff) return result;

        const ListOfConstObjects measures = system->FindAllDescendantsByType(MEASURE);
        if (measures.empty()) return result;

        std::map<const Measure *, int> measureIndices;
        int measureIndex = 0;
        for (const Object *object : measures) {
            const Measure *measure = vrv_cast<const Measure *>(object);
            assert(measure);
            measureIndices[measure] = measureIndex++;
        }
        const Measure *fallback = vrv_cast<const Measure *>(measures.front());

        const ArrayOfFloatingPositioners &positioners = staffAlignment->GetFloatingPositioners();
        for (int positionerIndex = 0; positionerIndex < static_cast<int>(positioners.size()); ++positionerIndex) {
            FloatingPositioner *positioner = positioners.at(positionerIndex);
            const data_STAFFREL drawingPlace = positioner->GetDrawingPlace();
            if ((drawingPlace != place) && !(includeWithin && (drawingPlace == STAFFREL_within))) continue;

            const Measure *measure = GetPositionerMeasure(positioner, fallback);
            const ScoreDef *scoreDef = GetDrawingScoreDef(measure, staff->GetN(), system->GetDrawingScoreDef());
            data_STAFFITEM_List order;
            if (useStaffItemOrder && scoreDef && (drawingPlace != STAFFREL_within)) {
                order = scoreDef->GetStaffItemOrder(staff->GetN(), place);
            }
            const std::vector<StaffItemStep> steps = GetOrderedStaffItemSteps(order);

            for (int stepIndex = 0; stepIndex < static_cast<int>(steps.size()); ++stepIndex) {
                const StaffItemStep &step = steps.at(stepIndex);
                if (!MatchesStaffItemStep(positioner, step)) continue;
                const int index = useStaffItemOrder ? measureIndices[measure] : 0;
                result.push_back({ positioner, step.classId, step.exactItem ? step.item : STAFFITEM_NONE, index,
                    stepIndex, positionerIndex });
                break;
            }
        }

        std::stable_sort(
            result.begin(), result.end(), [](const StaffItemPositioner &left, const StaffItemPositioner &right) {
                if (left.measureIndex != right.measureIndex) return left.measureIndex < right.measureIndex;
                if (left.stepIndex != right.stepIndex) return left.stepIndex < right.stepIndex;
                return left.positionerIndex < right.positionerIndex;
            });
        return result;
    }

} // namespace

//----------------------------------------------------------------------------
// AdjustFloatingPositionersFunctor
//----------------------------------------------------------------------------

AdjustFloatingPositionersFunctor::AdjustFloatingPositionersFunctor(Doc *doc) : DocFunctor(doc)
{
    m_classId = OBJECT;
    m_staffItem = STAFFITEM_NONE;
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
        std::map<std::pair<data_STAFFREL, int>, std::vector<ClassId>> explicitGroups;
        for (const FloatingPositioner *positioner : staffAlignment->GetFloatingPositioners()) {
            assert(positioner->GetObject());
            const int groupId = positioner->GetObject()->GetDrawingGrpId();
            if (groupId < 0) {
                explicitGroups[{ positioner->GetDrawingPlace(), groupId }].push_back(
                    positioner->GetObject()->GetClassId());
            }
        }
        for (const auto &group : explicitGroups) {
            const std::vector<ClassId> &classes = group.second;
            if (classes.size() < 2) continue;

            const bool legacyDynamGroup = std::all_of(classes.begin(), classes.end(),
                [](ClassId classId) { return (classId == DYNAM) || (classId == HAIRPIN); });
            const bool legacyDirGroup
                = std::all_of(classes.begin(), classes.end(), [](ClassId classId) { return classId == DIR; });
            const bool legacyPedalGroup
                = std::all_of(classes.begin(), classes.end(), [](ClassId classId) { return classId == PEDAL; });
            if (!legacyDynamGroup && !legacyDirGroup && !legacyPedalGroup) {
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
            FontInfo *lyricFont = m_doc->GetDrawingLyricFont(staffAlignment->GetStaff()->m_drawingStaffSize);
            int verseHeight = m_doc->GetTextGlyphHeight(L'I', lyricFont, false)
                - m_doc->GetTextGlyphDescender(L'q', lyricFont, false);
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
        if ((m_staffItem == STAFFITEM_stageDir) || (m_staffItem == STAFFITEM_dir)) {
            const Dir *dir = vrv_cast<const Dir *>(positioner->GetObject());
            assert(dir);
            if ((m_staffItem == STAFFITEM_stageDir) != dir->IsStageDir()) continue;
        }

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

        const long long groupKey = GetPositionerGroupKey(positioner);
        const auto positionerGroup = m_positionerGroups.find(groupKey);
        if (m_keepGroupPosition && (positionerGroup != m_positionerGroups.end())) {
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
                const FloatingPositioner *other = dynamic_cast<const FloatingPositioner *>(bbox);
                if (other && other->GetObject() && (other->GetObject()->GetDrawingGrpId() != 0)
                    && (GetPositionerGroupKey(other) == groupKey)) {
                    continue;
                }
                if (!positioner->HasHorizontalOverlapWith(bbox, drawingUnit)) continue;
                const int previousYRel = positioner->GetDrawingYRel();
                if (other) MovePositionerPastBoundingBox(positioner, other);
                positioner->CalcDrawingYRel(m_doc, staffAlignment, bbox);
                if (positioner->GetDrawingYRel() == previousYRel) continue;

                for (FloatingPositioner *member : positionerGroup->second) {
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
                if (m_keepGroupPosition) {
                    const FloatingPositioner *grouped = dynamic_cast<const FloatingPositioner *>(bbox);
                    if (grouped && grouped->GetObject() && (grouped->GetObject()->GetDrawingGrpId() != 0)) {
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
    StaffAlignment *staffAlignment, ClassId classId, data_STAFFREL place, data_STAFFITEM staffItem)
{
    m_classId = classId;
    m_staffItem = staffItem;
    m_inBetween = (place == STAFFREL_between);
    m_place = place;
    this->AdjustCurrentPositioners(staffAlignment);
}

void AdjustFloatingPositionersFunctor::ProcessPositioners(StaffAlignment *staffAlignment,
    const ArrayOfFloatingPositioners &positioners, ClassId classId, data_STAFFREL place, data_STAFFITEM staffItem,
    bool keepGroupPosition)
{
    m_positioners = &positioners;
    m_keepGroupPosition = keepGroupPosition;
    this->ProcessClass(staffAlignment, classId, place, staffItem);
    m_keepGroupPosition = false;
    m_positioners = NULL;
}

bool AdjustFloatingPositionersFunctor::HasExplicitPositionerGroup(
    StaffAlignment *staffAlignment, data_STAFFREL place) const
{
    for (const FloatingPositioner *positioner : staffAlignment->GetFloatingPositioners()) {
        assert(positioner->GetObject());
        if ((positioner->GetDrawingPlace() == place) && (positioner->GetObject()->GetDrawingGrpId() < 0)) return true;
    }
    return false;
}

void AdjustFloatingPositionersFunctor::BuildPositionerGroups(
    StaffAlignment *staffAlignment, data_STAFFREL place, bool includeWithin)
{
    m_positionerGroups.clear();
    for (FloatingPositioner *positioner : staffAlignment->GetFloatingPositioners()) {
        assert(positioner->GetObject());
        if ((positioner->GetDrawingPlace() != place)
            && !(includeWithin && (positioner->GetDrawingPlace() == STAFFREL_within))) {
            continue;
        }
        if (positioner->GetObject()->GetDrawingGrpId() == 0) continue;
        m_positionerGroups[GetPositionerGroupKey(positioner)].push_back(positioner);
    }
    std::erase_if(m_positionerGroups, [](const auto &group) { return group.second.size() < 2; });
}

void AdjustFloatingPositionersFunctor::StabilizePositionerGroups(StaffAlignment *staffAlignment, data_STAFFREL place,
    const ArrayOfBoundingBoxes &baseBoxes, const std::function<void()> &processPositioners)
{
    ArrayOfBoundingBoxes &overflowBoxes = (place == STAFFREL_above) ? staffAlignment->GetBBoxesAboveForModification()
                                                                    : staffAlignment->GetBBoxesBelowForModification();

    bool stable = false;
    const size_t maxPasses = std::min<size_t>(m_positionerGroups.size() + 1, STAFF_ITEM_GROUP_MAX_PASSES);
    for (size_t pass = 0; pass < maxPasses; ++pass) {
        std::map<FloatingPositioner *, int> previousPositions;
        for (const auto &group : m_positionerGroups) {
            for (FloatingPositioner *positioner : group.second) {
                previousPositions[positioner] = positioner->GetDrawingYRel();
            }
        }
        overflowBoxes = baseBoxes;
        processPositioners();
        stable = std::all_of(previousPositions.begin(), previousPositions.end(),
            [](const auto &entry) { return entry.first->GetDrawingYRel() == entry.second; });
        if (stable) break;
    }

    // Conflicting category orders can create a cycle between shared groups. Preserve the group baselines selected by
    // the stable traversal order and rebuild the remaining positioners around them without moving a group again.
    if (!stable) {
        m_movePositionerGroups = false;
        overflowBoxes = baseBoxes;
        processPositioners();
        m_movePositionerGroups = true;
    }
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

    const auto process
        = [this, staffAlignment, place](ClassId classId) { this->ProcessClass(staffAlignment, classId, place); };
    process(LV);
    process(TIE);
    process(SLUR);
    process(PHRASE);
    process(ACCID_FLOATING);
    process(MORDENT);
    process(TURN);
    process(TRILL);
    process(ORNAM);
    process(FING);
    process(DYNAM);
    process(HAIRPIN);

    adjustGroups.SetClassIDs({ DYNAM, HAIRPIN });
    adjustGroups.VisitStaffAlignment(staffAlignment);

    process(BRACKETSPAN);
    process(OCTAVE);
    process(BREATH);
    process(FERMATA);
    process(DIR);

    adjustGroups.SetClassIDs({ DIR });
    adjustGroups.VisitStaffAlignment(staffAlignment);

    process(CPMARK);
    process(REPEATMARK);
    process(TEMPO);
    process(PEDAL);

    adjustGroups.SetClassIDs({ PEDAL });
    adjustGroups.VisitStaffAlignment(staffAlignment);

    process(HARM);
    adjustGroups.SetClassIDs({ HARM });
    adjustGroups.VisitStaffAlignment(staffAlignment);

    process(ENDING);
    adjustGroups.SetClassIDs({ ENDING });
    adjustGroups.VisitStaffAlignment(staffAlignment);

    process(REH);
    process(CAESURA);
    process(ANNOTSCORE);

    if (this->HasExplicitPositionerGroup(staffAlignment, place)) {
        this->AdjustPositionerGroups(staffAlignment, place);
        this->BuildPositionerGroups(staffAlignment, place, includeWithin);
        const std::vector<StaffItemPositioner> positioners
            = BuildStaffItemPositioners(staffAlignment, place, includeWithin, false);

        const auto processPositioners = [this, staffAlignment, place, &positioners]() {
            for (const StaffItemPositioner &entry : positioners) {
                const ArrayOfFloatingPositioners current{ entry.positioner };
                this->ProcessPositioners(staffAlignment, current, entry.classId, place, entry.item, true);
            }
        };
        this->StabilizePositionerGroups(staffAlignment, place, baseBoxes, processPositioners);
        m_positionerGroups.clear();
    }
    m_includeWithin = false;
}

void AdjustFloatingPositionersFunctor::AdjustOrderedPlace(
    StaffAlignment *staffAlignment, data_STAFFREL place, bool includeWithin)
{
    ArrayOfBoundingBoxes &overflowBoxes = (place == STAFFREL_above) ? staffAlignment->GetBBoxesAboveForModification()
                                                                    : staffAlignment->GetBBoxesBelowForModification();
    const ArrayOfBoundingBoxes baseBoxes = overflowBoxes;
    const std::vector<StaffItemPositioner> positioners
        = BuildStaffItemPositioners(staffAlignment, place, includeWithin, true);

    m_includeWithin = includeWithin;
    for (const StaffItemPositioner &entry : positioners) {
        const ArrayOfFloatingPositioners current{ entry.positioner };
        this->ProcessPositioners(staffAlignment, current, entry.classId, place, entry.item, false);
    }

    this->AdjustPositionerGroups(staffAlignment, place);
    this->BuildPositionerGroups(staffAlignment, place, includeWithin);

    const auto processPositioners = [this, staffAlignment, place, &positioners]() {
        for (const StaffItemPositioner &entry : positioners) {
            const ArrayOfFloatingPositioners current{ entry.positioner };
            this->ProcessPositioners(staffAlignment, current, entry.classId, place, entry.item, true);
        }
    };
    this->StabilizePositionerGroups(staffAlignment, place, baseBoxes, processPositioners);
    m_positionerGroups.clear();
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
    m_staffItem = STAFFITEM_NONE;
    this->AdjustCurrentPositioners(staffAlignment);

    if (this->HasStaffItemOrder(staffAlignment, STAFFREL_between)) {
        this->AdjustOrderedPlace(staffAlignment, STAFFREL_between);
    }
    else {
        const ArrayOfBoundingBoxes baseBoxes = staffAlignment->GetBBoxesBelow();
        m_includeWithin = false;
        this->ProcessClass(staffAlignment, OBJECT, STAFFREL_between);

        AdjustFloatingPositionerGrpsFunctor adjustGroups(m_doc);
        adjustGroups.SetGroupType(1);
        adjustGroups.SetClassIDs({ DYNAM });
        adjustGroups.SetPlace(STAFFREL_between);
        adjustGroups.VisitStaffAlignment(staffAlignment);

        if (this->HasExplicitPositionerGroup(staffAlignment, STAFFREL_between)) {
            this->AdjustPositionerGroups(staffAlignment, STAFFREL_between);
            this->BuildPositionerGroups(staffAlignment, STAFFREL_between, false);
            const std::vector<StaffItemPositioner> positioners
                = BuildStaffItemPositioners(staffAlignment, STAFFREL_between, false, false);

            const auto processPositioners = [this, staffAlignment, &positioners]() {
                for (const StaffItemPositioner &entry : positioners) {
                    const ArrayOfFloatingPositioners current{ entry.positioner };
                    this->ProcessPositioners(
                        staffAlignment, current, entry.classId, STAFFREL_between, entry.item, true);
                }
            };
            this->StabilizePositionerGroups(staffAlignment, STAFFREL_between, baseBoxes, processPositioners);
            m_positionerGroups.clear();
        }
    }

    m_place = STAFFREL_NONE;
    m_includeWithin = false;
    m_inBetween = false;
    m_classId = OBJECT;
    m_staffItem = STAFFITEM_NONE;
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
    m_staffItem = STAFFITEM_NONE;
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
            const auto iter = std::find_if(grpIdYRel.begin(), grpIdYRel.end(),
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
