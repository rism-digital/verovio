/////////////////////////////////////////////////////////////////////////////
// Name:        adjustfloatingpositionerfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustfloatingpositionerfunctor.h"

//----------------------------------------------------------------------------

#include "functorparams.h"
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

    const bool verseCollapse = m_doc->GetOptions()->m_lyricVerseCollapse.GetValue();
    if (m_classId == SYL) {
        if (staffAlignment->GetVerseCount(verseCollapse) > 0) {
            FontInfo *lyricFont = m_doc->GetDrawingLyricFont(staffAlignment->GetStaff()->m_drawingStaffSize);
            int descender = m_doc->GetTextGlyphDescender(L'q', lyricFont, false);
            int height = m_doc->GetTextGlyphHeight(L'I', lyricFont, false);
            int margin = m_doc->GetBottomMargin(SYL) * drawingUnit;
            int minMargin = std::max((int)(m_doc->GetOptions()->m_lyricTopMinMargin.GetValue() * drawingUnit),
                staffAlignment->GetOverflowBelow());
            staffAlignment->SetOverflowBelow(
                minMargin + staffAlignment->GetVerseCount(verseCollapse) * (height - descender + margin));
            // For now just clear the overflowBelow, which avoids the overlap to be calculated. We could also keep them
            // and check if they are some lyrics in order to know if the overlap needs to be calculated or not.
            staffAlignment->ClearBBoxesBelow();
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
            if (m_classId == DIR) continue;
            if (m_classId == HAIRPIN) continue;
        }

        // Find all the overflowing elements from the staff that overlap horizontally
        for (auto i = overflowBoxes.begin(); i != overflowBoxes.end(); ++i) {
            if (positioner->HasHorizontalOverlapWith(*i, drawingUnit)) {
                // update the yRel accordingly
                positioner->CalcDrawingYRel(m_doc, staffAlignment, *i);
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

    AdjustFloatingPositionerGrpsParams adjustFloatingPositionerGrpsParams(m_doc);
    Functor adjustFloatingPositionerGrps(&Object::AdjustFloatingPositionerGrps);

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

    adjustFloatingPositionerGrpsParams.m_classIds.clear();
    adjustFloatingPositionerGrpsParams.m_classIds.push_back(DYNAM);
    adjustFloatingPositionerGrpsParams.m_classIds.push_back(HAIRPIN);
    adjustFloatingPositionerGrpsParams.m_place = STAFFREL_above;
    system->m_systemAligner.Process(&adjustFloatingPositionerGrps, &adjustFloatingPositionerGrpsParams);
    adjustFloatingPositionerGrpsParams.m_place = STAFFREL_below;
    system->m_systemAligner.Process(&adjustFloatingPositionerGrps, &adjustFloatingPositionerGrpsParams);

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

    adjustFloatingPositionerGrpsParams.m_classIds.clear();
    adjustFloatingPositionerGrpsParams.m_classIds.push_back(DIR);
    adjustFloatingPositionerGrpsParams.m_place = STAFFREL_above;
    system->m_systemAligner.Process(&adjustFloatingPositionerGrps, &adjustFloatingPositionerGrpsParams);
    adjustFloatingPositionerGrpsParams.m_place = STAFFREL_below;
    system->m_systemAligner.Process(&adjustFloatingPositionerGrps, &adjustFloatingPositionerGrpsParams);

    m_classId = TEMPO;
    system->m_systemAligner.Process(*this);

    m_classId = PEDAL;
    system->m_systemAligner.Process(*this);

    adjustFloatingPositionerGrpsParams.m_classIds.clear();
    adjustFloatingPositionerGrpsParams.m_classIds.push_back(PEDAL);
    adjustFloatingPositionerGrpsParams.m_place = STAFFREL_above;
    system->m_systemAligner.Process(&adjustFloatingPositionerGrps, &adjustFloatingPositionerGrpsParams);
    adjustFloatingPositionerGrpsParams.m_place = STAFFREL_below;
    system->m_systemAligner.Process(&adjustFloatingPositionerGrps, &adjustFloatingPositionerGrpsParams);

    m_classId = HARM;
    system->m_systemAligner.Process(*this);

    adjustFloatingPositionerGrpsParams.m_classIds.clear();
    adjustFloatingPositionerGrpsParams.m_classIds.push_back(HARM);
    adjustFloatingPositionerGrpsParams.m_place = STAFFREL_above;
    system->m_systemAligner.Process(&adjustFloatingPositionerGrps, &adjustFloatingPositionerGrpsParams);
    adjustFloatingPositionerGrpsParams.m_place = STAFFREL_below;
    system->m_systemAligner.Process(&adjustFloatingPositionerGrps, &adjustFloatingPositionerGrpsParams);

    m_classId = ENDING;
    system->m_systemAligner.Process(*this);

    adjustFloatingPositionerGrpsParams.m_classIds.clear();
    adjustFloatingPositionerGrpsParams.m_classIds.push_back(ENDING);
    adjustFloatingPositionerGrpsParams.m_place = STAFFREL_above;
    system->m_systemAligner.Process(&adjustFloatingPositionerGrps, &adjustFloatingPositionerGrpsParams);
    adjustFloatingPositionerGrpsParams.m_place = STAFFREL_below;
    system->m_systemAligner.Process(&adjustFloatingPositionerGrps, &adjustFloatingPositionerGrpsParams);

    m_classId = REH;
    system->m_systemAligner.Process(*this);

    m_classId = CAESURA;
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

    adjustFloatingPositionerGrpsParams.m_classIds.clear();
    adjustFloatingPositionerGrpsParams.m_classIds.push_back(DYNAM);
    adjustFloatingPositionerGrpsParams.m_place = STAFFREL_between;
    system->m_systemAligner.Process(&adjustFloatingPositionerGrps, &adjustFloatingPositionerGrpsParams);

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
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
