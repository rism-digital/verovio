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
    // Nothing to calculate if note is not part of the chord
    if (!note->IsChordTone()) return FUNCTOR_SIBLINGS;

    Staff *staff = note->GetAncestorStaff(RESOLVE_CROSS_STAFF);
    const int staffSize = staff->m_drawingStaffSize;

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

} // namespace vrv
