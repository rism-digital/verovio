/////////////////////////////////////////////////////////////////////////////
// Name:        adjustfloatingpositionerfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustfloatingpositionerfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "staff.h"
#include "system.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustFloatingPositionersFunctor
//----------------------------------------------------------------------------

AdjustFloatingPositionersFunctor::AdjustFloatingPositionersFunctor(Doc *doc) : DocFunctor(doc)
{
    m_classId = OBJECT;
    m_inBetween = false;
}

FunctorCode AdjustFloatingPositionersFunctor::VisitStaffAlignment(StaffAlignment *staffAlignment)
{
    const int staffSize = staffAlignment->GetStaffSize();
    const int drawingUnit = m_doc->GetDrawingUnit(staffSize);

    staffAlignment->SortPositioners();

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

    for (FloatingPositioner *positioner : staffAlignment->GetFloatingPositioners()) {
        assert(positioner->GetObject());
        if (!m_inBetween && !positioner->GetObject()->Is(m_classId)) continue;

        if (m_inBetween) {
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

            if (positioner->GetObject()->Is({ LV, PHRASE, SLUR, TIE })) {
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

        // This sets the default position (without considering any overflowing box)
        positioner->CalcDrawingYRel(m_doc, staffAlignment, NULL);

        const data_STAFFREL place = positioner->GetDrawingPlace();
        ArrayOfBoundingBoxes &overflowBoxes = (place == STAFFREL_above)
            ? staffAlignment->GetBBoxesAboveForModification()
            : staffAlignment->GetBBoxesBelowForModification();

        // Handle within placement (ignore collisions for certain classes)
        if (place == STAFFREL_within) {
            if (m_classId == CPMARK) continue;
            if (m_classId == DIR) continue;
            if (m_classId == HAIRPIN) continue;
        }

        // Find all the overflowing elements from the staff that overlap horizontally
        for (BoundingBox *bbox : overflowBoxes) {
            if (positioner->HasHorizontalOverlapWith(bbox, drawingUnit)) {
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

FunctorCode AdjustFloatingPositionersFunctor::VisitSystem(System *system)
{
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
}

FunctorCode AdjustFloatingPositionerGrpsFunctor::VisitStaffAlignment(StaffAlignment *staffAlignment)
{
    ArrayOfFloatingPositioners positioners;
    // make a temporary copy of positioners with desired classId and a drawing grpId
    const ArrayOfFloatingPositioners &allPositioners = staffAlignment->GetFloatingPositioners();
    std::copy_if(allPositioners.begin(), allPositioners.end(), std::back_inserter(positioners),
        [this](FloatingPositioner *positioner) {
            assert(positioner->GetObject());
            // search in the desired classIds
            return ((std::find(m_classIds.begin(), m_classIds.end(), positioner->GetObject()->GetClassId())
                        != m_classIds.end())
                && (positioner->GetObject()->GetDrawingGrpId() != 0) && (positioner->GetDrawingPlace() == m_place)
                && !positioner->HasEmptyBB());
        });

    if (positioners.empty()) {
        return FUNCTOR_SIBLINGS;
    }

    // A vector storing a pair with the grpId and the min or max YRel
    ArrayOfIntPairs grpIdYRel;

    for (FloatingPositioner *positioner : positioners) {
        int currentGrpId = positioner->GetObject()->GetDrawingGrpId();
        // Look if we already have a pair for this grpId
        auto iter = std::find_if(grpIdYRel.begin(), grpIdYRel.end(),
            [currentGrpId](std::pair<int, int> &pair) { return (pair.first == currentGrpId); });
        // if not, then just add a new pair with the YRel of the current positioner
        if (iter == grpIdYRel.end()) {
            grpIdYRel.push_back({ currentGrpId, positioner->GetDrawingYRel() });
        }
        // else, adjust the min or max YRel of the pair if necessary
        else {
            if (m_place == STAFFREL_above) {
                if (positioner->GetDrawingYRel() < (*iter).second) (*iter).second = positioner->GetDrawingYRel();
            }
            else {
                if (positioner->GetDrawingYRel() > (*iter).second) (*iter).second = positioner->GetDrawingYRel();
            }
        }
    }

    if (std::find(m_classIds.begin(), m_classIds.end(), HARM) != m_classIds.end()) {
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
        if (!positioner->GetObject()->Is({ CPMARK, DIR, DYNAM, HAIRPIN, TEMPO })) continue;

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
