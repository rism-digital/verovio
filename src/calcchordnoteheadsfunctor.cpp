/////////////////////////////////////////////////////////////////////////////
// Name:        calcchordnoteheadsfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "calcchordnoteheadsfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "staff.h"
#include "tabdursym.h"
#include "tabgrp.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// CalcChordNoteHeadsFunctor
//----------------------------------------------------------------------------

CalcChordNoteHeadsFunctor::CalcChordNoteHeadsFunctor(Doc *doc) : DocFunctor(doc)
{
    m_diameter = 0;
    m_alignmentType = 0;
}

FunctorCode CalcChordNoteHeadsFunctor::VisitChord(Chord *chord)
{
    Staff *staff = chord->GetAncestorStaff(RESOLVE_CROSS_STAFF);

    m_diameter = 0;
    if (chord->GetDrawingStemDir() == STEMDIRECTION_up) {
        if (chord->IsInBeam()) {
            m_diameter = 2 * chord->GetDrawingRadius(m_doc);
        }
        else {
            const Note *bottomNote = chord->GetBottomNote();
            const char32_t code = bottomNote->GetNoteheadGlyph(chord->GetActualDur());
            m_diameter = m_doc->GetGlyphWidth(
                code, staff->m_drawingStaffSize, chord->GetDrawingCueSize() ? bottomNote->GetDrawingCueSize() : false);
        }
        m_alignmentType = chord->GetAlignment()->GetType();
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode CalcChordNoteHeadsFunctor::VisitNote(Note *note)
{
    Staff *staff = note->GetAncestorStaff(RESOLVE_CROSS_STAFF);
    const int staffSize = staff->m_drawingStaffSize;

    // Note in tab staff-like needs to be adjusted back from the parent TabGrp
    if (staff->IsTabStaffLike()) {
        const int staffNotationSize = staff->GetDrawingStaffNotationSize();
        const int width = m_doc->GetGlyphWidth(SMUFL_E0A4_noteheadBlack, staffNotationSize, false) / 2;
        note->SetDrawingXRel(-width);
        return FUNCTOR_SIBLINGS;
    }

    // Nothing to calculate if note is not part of the chord
    if (!note->IsChordTone()) return FUNCTOR_SIBLINGS;

    const int diameter = 2 * note->GetDrawingRadius(m_doc);
    int noteheadShift = 0;
    if ((note->GetDrawingStemDir() == STEMDIRECTION_up) && m_diameter) {
        noteheadShift = m_diameter - diameter;
    }

    // Nothing to do for notes that are not in a note group and without base diameter for the chord
    const ChordNoteGroup *noteGroup = note->GetNoteGroup();
    if ((!m_diameter || (m_alignmentType != note->GetAlignment()->GetType())) && !noteGroup) return FUNCTOR_SIBLINGS;

    /************** notehead direction **************/

    bool flippedNotehead = false;

    // if the note is in a note group, calculations are different
    if (noteGroup) {
        const int noteGroupPosition = note->GetNoteGroupPosition();
        if (note->GetDrawingStemDir() == STEMDIRECTION_down) {
            // stem down/even note group = noteheads start on left (incorrect side)
            if (noteGroup->size() % 2 == 0) {
                flippedNotehead = (noteGroupPosition % 2 != 0);
            }
            // else they start on normal side
            else {
                flippedNotehead = (noteGroupPosition % 2 == 0);
            }
        }
        else {
            // flipped noteheads start on normal side no matter what
            flippedNotehead = (noteGroupPosition % 2 == 0);
        }
    }

    // positions notehead
    if (flippedNotehead) {
        if (note->GetDrawingStemDir() == STEMDIRECTION_up) {
            note->SetDrawingXRel(diameter - m_doc->GetDrawingStemWidth(staffSize));
        }
        else {
            note->SetDrawingXRel(-diameter + m_doc->GetDrawingStemWidth(staffSize));
        }
    }
    note->SetDrawingXRel(note->GetDrawingXRel() + noteheadShift);

    note->SetFlippedNotehead(flippedNotehead);

    return FUNCTOR_SIBLINGS;
}

FunctorCode CalcChordNoteHeadsFunctor::VisitTabDurSym(TabDurSym *tabDurSym)
{
    Staff *staff = tabDurSym->GetAncestorStaff(RESOLVE_CROSS_STAFF);
    TabGrp *tabGrp = vrv_cast<TabGrp *>(tabDurSym->GetFirstAncestor(TABGRP));
    assert(tabGrp);

    // adjust vertical position for tabDurSym@tab.line, tabDurSym@vo and tablature type
    // tabDurSym@tab.line takes priority over tabDurSym@vo
    if (!staff->IsTabGuitar()) {
        if (tabDurSym->HasTabLine()) {
            const int yAdjust = (tabDurSym->GetTabLine() - staff->m_drawingLines) * 2;
            tabDurSym->SetDrawingYRel(yAdjust * m_doc->GetDrawingUnit(staff->m_drawingStaffSize));
        }
        else {
            int yAdjust = 1; // margin between staff line and rhythm sign, in half lines

            // position rhythm sign according to tablature type
            if (staff->IsTabLuteFrench() || staff->IsTabLuteGerman()) {
                yAdjust = 2;
            }
            else if (staff->IsTabLuteItalian() && staff->m_drawingLines >= 6) {
                yAdjust = 3; //  allow for >= 7 course Italian tablature
            }
            else if (staff->IsTabStaffLike()) {
                yAdjust = 4; // clear A5 on treble clef

                // raise rhythm sign above ledger lines for B5 and above on treble clef
                if (!tabGrp->HasEmptyList()) {
                    const Note *topNote = tabGrp->GetTopNote();
                    assert(topNote);
                    int linesAbove = 0;
                    int linesBelow = 0;
                    if (topNote->HasLedgerLines(linesAbove, linesBelow, staff) && linesAbove > 0) {
                        yAdjust += topNote->GetDrawingYRel() / m_doc->GetDrawingUnit(staff->m_drawingStaffSize) - 2;
                    }
                }
            }

            // adjust for tabDurSym@vo
            if (tabDurSym->HasVo() && tabDurSym->GetVo().GetType() == MEASUREMENTTYPE_vu) {
                yAdjust += std::round(tabDurSym->GetVo().GetVu());
            }

            tabDurSym->SetDrawingYRel(yAdjust * m_doc->GetDrawingUnit(staff->m_drawingStaffSize));
        }
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode CalcChordNoteHeadsFunctor::VisitTabGrp(TabGrp *tabGrp)
{
    Staff *staff = tabGrp->GetAncestorStaff(RESOLVE_CROSS_STAFF);
    const int staffSize = staff->GetDrawingStaffNotationSize();
    const int width = m_doc->GetGlyphWidth(SMUFL_E0A4_noteheadBlack, staffSize, false) / 2;

    tabGrp->SetDrawingXRel(width);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
