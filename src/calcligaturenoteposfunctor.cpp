/////////////////////////////////////////////////////////////////////////////
// Name:        calcligaturenoteposfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "calcligaturenoteposfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "ligature.h"
#include "staff.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// CalcLigatureNotePosFunctor
//----------------------------------------------------------------------------

CalcLigatureNotePosFunctor::CalcLigatureNotePosFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode CalcLigatureNotePosFunctor::VisitLigature(Ligature *ligature)
{
    if (m_doc->GetOptions()->m_ligatureAsBracket.GetValue()) return FUNCTOR_CONTINUE;

    ligature->m_drawingShapes.clear();

    const ListOfObjects &notes = ligature->GetList();
    Note *lastNote = dynamic_cast<Note *>(notes.back());
    Staff *staff = ligature->GetAncestorStaff();

    if (notes.size() < 2) return FUNCTOR_SIBLINGS;

    Note *previousNote = NULL;
    bool previousUp = false;
    int n1 = 0;
    int n2 = 1;

    bool isMensuralBlack = (staff->m_drawingNotationType == NOTATIONTYPE_mensural_black);
    bool oblique = false;
    if ((notes.size() == 2) && (ligature->GetForm() == LIGATUREFORM_obliqua)) oblique = true;

    // For better clarity, we loop withing the VisitLigature instead of implementing VisitNote.

    for (Object *object : notes) {

        Note *note = vrv_cast<Note *>(object);
        assert(note);

        ligature->m_drawingShapes.push_back(LIGATURE_DEFAULT);

        if (!previousNote) {
            previousNote = note;
            continue;
        }

        // Look at the @lig attribute on the previous note
        if (previousNote->GetLig() == LIGATUREFORM_obliqua) oblique = true;
        int dur1 = previousNote->GetActualDur();
        int dur2 = note->GetActualDur();
        // Same treatment for Mx and LG except for positioning, which is done above
        // We still need to avoid oblique, so keep a flag.
        bool isMaxima = false;
        if (dur1 == DUR_MX) {
            dur1 = DUR_LG;
            isMaxima = true;
        }
        if (dur2 == DUR_MX) dur2 = DUR_LG;

        int diatonicStep = note->GetDiatonicPitch() - previousNote->GetDiatonicPitch();
        bool up = (diatonicStep > 0);
        bool isLastNote = (note == lastNote);

        // L - L
        if ((dur1 == DUR_LG) && (dur2 == DUR_LG)) {
            if (up) {
                ligature->m_drawingShapes.at(n1) = LIGATURE_STEM_RIGHT_DOWN;
                ligature->m_drawingShapes.at(n2) = LIGATURE_STEM_RIGHT_DOWN;
            }
            else {
                // nothing to change
            }
        }
        // L - B
        else if ((dur1 == DUR_LG) && (dur2 == DUR_BR)) {
            if (up) {
                ligature->m_drawingShapes.at(n1) = LIGATURE_STEM_RIGHT_DOWN;
            }
            // automatically set oblique on B, but not with Mx and only at the beginning and end
            else if (!isMaxima && ((n1 == 0) || isLastNote)) {
                ligature->m_drawingShapes.at(n1) = LIGATURE_OBLIQUE;
                // make sure the previous one is not oblique
                if (n1 > 0) {
                    ligature->m_drawingShapes.at(n1 - 1) &= ~LIGATURE_OBLIQUE;
                }
            }
        }
        // B - B
        else if ((dur1 == DUR_BR) && (dur2 == DUR_BR)) {
            if (up) {
                // nothing to change
            }
            // automatically set oblique on B only at the beginning and end
            else if ((n1 == 0) || isLastNote) {
                ligature->m_drawingShapes.at(n1) = LIGATURE_OBLIQUE;
                // make sure the previous one is not oblique
                if (n1 > 0) {
                    ligature->m_drawingShapes.at(n1 - 1) &= ~LIGATURE_OBLIQUE;
                }
                else {
                    ligature->m_drawingShapes.at(n1) |= LIGATURE_STEM_LEFT_DOWN;
                }
            }
        }
        // B - L
        else if ((dur1 == DUR_BR) && (dur2 == DUR_LG)) {
            if (up) {
                ligature->m_drawingShapes.at(n2) = LIGATURE_STEM_RIGHT_DOWN;
            }
            else {
                if (!isLastNote) {
                    ligature->m_drawingShapes.at(n2) = LIGATURE_STEM_RIGHT_DOWN;
                }
                if (n1 == 0) {
                    ligature->m_drawingShapes.at(n1) = LIGATURE_STEM_LEFT_DOWN;
                }
            }
        }
        // SB - SB
        else if ((dur1 == DUR_1) && (dur2 == DUR_1)) {
            ligature->m_drawingShapes.at(n1) = LIGATURE_STEM_LEFT_UP;
        }
        // SB - L (this should not happen on the first two notes, but this is an encoding problem)
        else if ((dur1 == DUR_1) && (dur2 == DUR_LG)) {
            if (up) {
                ligature->m_drawingShapes.at(n2) = LIGATURE_STEM_RIGHT_DOWN;
            }
            else {
                // nothing to change
            }
        }
        // SB - B (this should not happen on the first two notes, but this is an encoding problem)
        else if ((dur1 == DUR_1) && (dur2 == DUR_BR)) {
            if (up) {
                // nothing to change
            }
            // only set the oblique with the SB if the following B is not the start of an oblique
            else if (note->GetLig() != LIGATUREFORM_obliqua) {
                ligature->m_drawingShapes.at(n1) = LIGATURE_OBLIQUE;
                if (n1 > 0) {
                    ligature->m_drawingShapes.at(n1 - 1) &= ~LIGATURE_OBLIQUE;
                }
            }
        }

        // Blindly set the oblique shape without trying to deal with encoding problems
        if (oblique) {
            ligature->m_drawingShapes.at(n1) |= LIGATURE_OBLIQUE;
            if (n1 > 0) {
                ligature->m_drawingShapes.at(n1 - 1) &= ~LIGATURE_OBLIQUE;
            }
        }

        // With mensural black notation, stack longa going up
        if (isLastNote && isMensuralBlack && (dur2 == DUR_LG) && up) {
            // Stack only if at least a third
            int stackThreshold = 1;
            // If the previous was going down, adjust the threshold
            if ((n1 > 0) && !previousUp) {
                // For oblique, stack but only from a fourth, for recta, never stack them
                stackThreshold = (ligature->m_drawingShapes.at(n1 - 1) & LIGATURE_OBLIQUE) ? 2 : -VRV_UNSET;
            }
            if (diatonicStep > stackThreshold) ligature->m_drawingShapes.at(n2) = LIGATURE_STACKED;
        }

        oblique = false;
        previousNote = note;
        previousUp = up;
        ++n1;
        ++n2;
    }

    /**** Set the xRel position for each note ****/

    int previousRight = 0;
    previousNote = NULL;
    n1 = 0;

    for (Object *object : notes) {

        Note *note = vrv_cast<Note *>(object);
        assert(note);

        // previousRight is 0 for the first note
        int width = (note->GetDrawingRadius(m_doc, true) * 2) - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
        // With stacked notes, back-track the position
        if (ligature->m_drawingShapes.at(n1 + 1) & LIGATURE_STACKED) previousRight -= width;
        note->SetDrawingXRel(previousRight);
        previousRight += width;

        if (!previousNote) {
            previousNote = note;
            continue;
        }

        int diatonicStep = note->GetDiatonicPitch() - previousNote->GetDiatonicPitch();

        // For large interval and oblique, adjust the x position to limit the angle
        if ((ligature->m_drawingShapes.at(n1) & LIGATURE_OBLIQUE) && (abs(diatonicStep) > 2)) {
            // angle stays the same from third onward (2 / 3 or a brevis per diatonic step)
            int shift = (abs(diatonicStep) - 2) * width * 2 / 3;
            note->SetDrawingXRel(note->GetDrawingXRel() + shift);
            previousRight += shift;
        }
        previousNote = note;
        ++n1;
    }

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
