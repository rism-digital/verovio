/////////////////////////////////////////////////////////////////////////////
// Name:        adjustarticfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustarticfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "elementpart.h"
#include "staff.h"
#include "stem.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustArticFunctor
//----------------------------------------------------------------------------

AdjustArticFunctor::AdjustArticFunctor(Doc *doc) : DocFunctor(doc)
{
    m_parent = NULL;
}

FunctorCode AdjustArticFunctor::VisitArtic(Artic *artic)
{
    if (!m_parent) return FUNCTOR_CONTINUE;

    int yIn, yOut, yRel;

    Staff *staff = artic->GetAncestorStaff(RESOLVE_CROSS_STAFF);
    const Beam *beam = artic->GetAncestorBeam();
    const int staffHeight = m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * (staff->m_drawingLines - 1);

    const Stem *stem = vrv_cast<const Stem *>(m_parent->FindDescendantByType(STEM));
    const Flag *flag = vrv_cast<const Flag *>(m_parent->FindDescendantByType(FLAG));
    // Avoid artic to be in ledger lines
    if (artic->GetDrawingPlace() == STAFFREL_above) {
        int yAboveStem = m_parent->GetDrawingTop(m_doc, staff->m_drawingStaffSize, false) - staff->GetDrawingY();
        if (flag && stem && (stem->GetDrawingStemDir() == STEMDIRECTION_up))
            yAboveStem += flag->GetStemUpSE(m_doc, staff->m_drawingStaffSize, false).y;
        yIn = std::max(yAboveStem, -staffHeight);
        yOut = std::max(yIn, 0);
    }
    else {
        const bool isStemDown = stem && (stem->GetDrawingStemDir() == STEMDIRECTION_down);
        int yBelowStem = m_parent->GetDrawingBottom(m_doc, staff->m_drawingStaffSize, false) - staff->GetDrawingY();
        if (flag && isStemDown) yBelowStem += flag->GetStemDownNW(m_doc, staff->m_drawingStaffSize, false).y;
        yIn = std::min(yBelowStem, 0);
        if (beam && beam->m_crossStaffContent && (beam->m_drawingPlace == BEAMPLACE_mixed) && isStemDown) {
            yIn -= beam->m_beamWidthBlack;
        }
        yOut = std::min(yIn, -staffHeight);
    }

    yRel = artic->IsInsideArtic() ? yIn : yOut;
    artic->SetDrawingYRel(yRel);

    // Adjust according to the position of a previous artic
    Artic *previous = NULL;
    if ((artic->GetDrawingPlace() == STAFFREL_above) && !m_articAbove.empty()) {
        previous = m_articAbove.back();
        int inTop = previous->GetContentTop();
        int outBottom = artic->GetContentBottom();
        if (inTop > outBottom) {
            artic->SetDrawingYRel(artic->GetDrawingYRel() + inTop - outBottom);
        }
    }
    if ((artic->GetDrawingPlace() == STAFFREL_below) && !m_articBelow.empty()) {
        previous = m_articBelow.back();
        int inBottom = previous->GetContentBottom();
        int outTop = artic->GetContentTop();
        if (inBottom < outTop) {
            artic->SetDrawingYRel(artic->GetDrawingYRel() - outTop + inBottom);
        }
    }

    // Add spacing
    const int unit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    const int spacingTop = m_doc->GetTopMargin(ARTIC) * unit;
    const int spacingBottom = m_doc->GetBottomMargin(ARTIC) * unit;
    const int direction = (artic->GetDrawingPlace() == STAFFREL_above) ? 1 : -1;
    int y = artic->GetDrawingY();
    int yShift = 0;

    if (artic->IsInsideArtic()) {
        // If we are above the top of the staff, just pile them up
        if ((artic->GetDrawingPlace() == STAFFREL_above) && (y > staff->GetDrawingY())) {
            yShift += spacingBottom;
        }
        // If we are below the bottom, just pile them down
        else if ((artic->GetDrawingPlace() == STAFFREL_below) && (y < staff->GetDrawingY() - staffHeight)) {
            if (y > staff->GetDrawingY() - staffHeight - unit) {
                yShift = (staff->GetDrawingY() - staffHeight - unit) - y;
                if (std::abs(yShift) < spacingTop) yShift = -spacingTop;
            }
            else {
                yShift -= spacingTop;
            }
        }
        // Otherwise make it fit in the staff space
        else {
            yShift = staff->GetNearestInterStaffPosition(y, m_doc, artic->GetDrawingPlace()) - y;
            if (staff->IsOnStaffLine(y + yShift, m_doc)) yShift += unit * direction;
        }
    }
    // Artic part outside just need to be piled up or down
    else {
        const int spacing = (direction > 0) ? spacingBottom : spacingTop;
        yShift += spacing * direction;
    }
    artic->SetDrawingYRel(artic->GetDrawingYRel() + yShift);

    // Add it to the list of previous artics - actually keeping only the last one could be sufficient?
    if (artic->GetDrawingPlace() == STAFFREL_above) {
        m_articAbove.push_back(artic);
    }
    else {
        m_articBelow.push_back(artic);
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode AdjustArticFunctor::VisitChord(Chord *chord)
{
    m_parent = chord;
    m_articAbove.clear();
    m_articBelow.clear();

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustArticFunctor::VisitNote(Note *note)
{
    if (note->IsChordTone()) return FUNCTOR_CONTINUE;

    m_parent = note;
    m_articAbove.clear();
    m_articBelow.clear();

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// AdjustArticWithSlursFunctor
//----------------------------------------------------------------------------

AdjustArticWithSlursFunctor::AdjustArticWithSlursFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode AdjustArticWithSlursFunctor::VisitArtic(Artic *artic)
{
    if (artic->m_startSlurPositioners.empty() && artic->m_endSlurPositioners.empty()) return FUNCTOR_CONTINUE;

    for (FloatingCurvePositioner *curve : artic->m_endSlurPositioners) {
        int shift = artic->Intersects(curve, CONTENT, m_doc->GetDrawingUnit(100));
        if (shift != 0) {
            artic->SetDrawingYRel(artic->GetDrawingYRel() + shift);
        }
    }

    for (FloatingCurvePositioner *curve : artic->m_startSlurPositioners) {
        int shift = artic->Intersects(curve, CONTENT, m_doc->GetDrawingUnit(100));
        if (shift != 0) {
            artic->SetDrawingYRel(artic->GetDrawingYRel() + shift);
        }
    }

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
