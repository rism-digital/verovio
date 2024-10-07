/////////////////////////////////////////////////////////////////////////////
// Name:        calcligaturorneumeposfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "calcligatureorneumeposfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "ligature.h"
#include "nc.h"
#include "neume.h"
#include "staff.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// CalcLigatureOrNeumePosFunctor
//----------------------------------------------------------------------------

CalcLigatureOrNeumePosFunctor::CalcLigatureOrNeumePosFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode CalcLigatureOrNeumePosFunctor::VisitLigature(Ligature *ligature)
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
        data_DURATION dur1 = previousNote->GetActualDur();
        data_DURATION dur2 = note->GetActualDur();
        // Same treatment for Mx and LG except for positioning, which is done above
        // We still need to avoid oblique, so keep a flag.
        bool isMaxima = false;
        if (dur1 == DURATION_maxima) {
            dur1 = DURATION_long;
            isMaxima = true;
        }
        if (dur2 == DURATION_maxima) dur2 = DURATION_long;

        int diatonicStep = note->GetDiatonicPitch() - previousNote->GetDiatonicPitch();
        bool up = (diatonicStep > 0);
        bool isLastNote = (note == lastNote);

        // L - L
        if ((dur1 == DURATION_long) && (dur2 == DURATION_long)) {
            if (up) {
                ligature->m_drawingShapes.at(n1) = LIGATURE_STEM_RIGHT_DOWN;
                ligature->m_drawingShapes.at(n2) = LIGATURE_STEM_RIGHT_DOWN;
            }
            else {
                // nothing to change
            }
        }
        // L - B
        else if ((dur1 == DURATION_long) && (dur2 == DURATION_breve)) {
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
        else if ((dur1 == DURATION_breve) && (dur2 == DURATION_breve)) {
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
        else if ((dur1 == DURATION_breve) && (dur2 == DURATION_long)) {
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
        else if ((dur1 == DURATION_1) && (dur2 == DURATION_1)) {
            ligature->m_drawingShapes.at(n1) = LIGATURE_STEM_LEFT_UP;
        }
        // SB - L (this should not happen on the first two notes, but this is an encoding problem)
        else if ((dur1 == DURATION_1) && (dur2 == DURATION_long)) {
            if (up) {
                ligature->m_drawingShapes.at(n2) = LIGATURE_STEM_RIGHT_DOWN;
            }
            else {
                // nothing to change
            }
        }
        // SB - B (this should not happen on the first two notes, but this is an encoding problem)
        else if ((dur1 == DURATION_1) && (dur2 == DURATION_breve)) {
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
        if (isLastNote && isMensuralBlack && (dur2 == DURATION_long) && up) {
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

FunctorCode CalcLigatureOrNeumePosFunctor::VisitNeume(Neume *neume)
{
    if (m_doc->GetOptions()->m_neumeAsNote.GetValue()) return FUNCTOR_SIBLINGS;

    ListOfObjects ncs = neume->FindAllDescendantsByType(NC);

    Staff *staff = neume->GetAncestorStaff();
    assert(staff);
    const int staffSize = staff->m_drawingStaffSize;
    const int unit = m_doc->GetDrawingUnit(staffSize);

    int xRel = 0;
    Nc *previousNc = NULL;
    bool previousLig = false;

    for (Object *object : ncs) {

        Nc *nc = vrv_cast<Nc *>(object);
        assert(nc);

        const bool hasLiquescent = (nc->FindDescendantByType(LIQUESCENT));
        const bool hasOriscus = (nc->FindDescendantByType(ORISCUS));
        const bool hasQuilisma = (nc->FindDescendantByType(QUILISMA));

        const int lineWidth = m_doc->GetGlyphWidth(SMUFL_E9BE_chantConnectingLineAsc3rd, staffSize, false);

        // Make sure we have at least one glyph
        nc->m_drawingGlyphs.resize(1);

        int pitchDifference = (previousNc) ? nc->PitchOrLocDifferenceTo(previousNc) : 0;
        bool overlapWithPrevious = (pitchDifference == 0) ? false : true;

        if (hasLiquescent) {
            nc->m_drawingGlyphs.resize(3);
            const int ncWidth = m_doc->GetGlyphWidth(SMUFL_E995_chantAuctumDesc, staffSize, false);
            const int lineWidth = m_doc->GetGlyphWidth(SMUFL_E9BE_chantConnectingLineAsc3rd, staffSize, false);

            if (nc->GetCurve() == curvatureDirection_CURVE_c) {
                nc->m_drawingGlyphs.at(0).m_fontNo = SMUFL_E995_chantAuctumDesc;
                nc->m_drawingGlyphs.at(1).m_fontNo = SMUFL_E9BE_chantConnectingLineAsc3rd;
                nc->m_drawingGlyphs.at(2).m_fontNo = SMUFL_E9BE_chantConnectingLineAsc3rd;
                nc->m_drawingGlyphs.at(2).m_xOffset = ncWidth - lineWidth;
                nc->m_drawingGlyphs.at(1).m_yOffset = -1.75 * unit;
                nc->m_drawingGlyphs.at(2).m_yOffset = -1.9 * unit;
            }
            else if (nc->GetCurve() == curvatureDirection_CURVE_a) {
                nc->m_drawingGlyphs.at(0).m_fontNo = SMUFL_E994_chantAuctumAsc;
                nc->m_drawingGlyphs.at(1).m_fontNo = SMUFL_E9BE_chantConnectingLineAsc3rd;
                nc->m_drawingGlyphs.at(2).m_fontNo = SMUFL_E9BE_chantConnectingLineAsc3rd;
                nc->m_drawingGlyphs.at(2).m_xOffset = ncWidth - lineWidth;
                nc->m_drawingGlyphs.at(1).m_yOffset = 0.5 * unit;
                nc->m_drawingGlyphs.at(2).m_yOffset = 0.75 * unit;
            }
            else {
                nc->m_drawingGlyphs.at(0).m_fontNo = SMUFL_E9A1_chantPunctumDeminutum;
            }
        }
        else if (hasOriscus) {
            nc->m_drawingGlyphs.at(0).m_fontNo = SMUFL_EA2A_medRenOriscusCMN;
        }
        else if (hasQuilisma) {
            nc->m_drawingGlyphs.at(0).m_fontNo = SMUFL_E99B_chantQuilisma;
        }
        else {
            nc->m_drawingGlyphs.at(0).m_fontNo = SMUFL_E990_chantPunctum;

            if (nc->GetLigated() == BOOLEAN_true) {
                // This is the first nc of a ligature
                if (!previousLig) {
                    // Temporarily set a second line glyph
                    nc->m_drawingGlyphs.at(0).m_fontNo = SMUFL_E9B4_chantEntryLineAsc2nd;
                    previousLig = true;
                }
                // This is the second
                else {
                    // No overlap in this case since the second starts at the same position as the first
                    overlapWithPrevious = false;
                    assert(previousNc);
                    previousLig = false;
                    nc->m_drawingGlyphs.at(0).m_yOffset = -pitchDifference * unit;
                    previousNc->m_drawingGlyphs.at(0).m_yOffset = pitchDifference * unit;

                    // set the glyph for both the current and previous nc
                    switch (pitchDifference) {
                        case -1:
                            nc->m_drawingGlyphs.at(0).m_fontNo = SMUFL_E9B9_chantLigaturaDesc2nd;
                            previousNc->m_drawingGlyphs.at(0).m_fontNo = SMUFL_E9B4_chantEntryLineAsc2nd;
                            break;
                        case -2:
                            nc->m_drawingGlyphs.at(0).m_fontNo = SMUFL_E9BA_chantLigaturaDesc3rd;
                            previousNc->m_drawingGlyphs.at(0).m_fontNo = SMUFL_E9B5_chantEntryLineAsc3rd;
                            break;
                        case -3:
                            nc->m_drawingGlyphs.at(0).m_fontNo = SMUFL_E9BB_chantLigaturaDesc4th;
                            previousNc->m_drawingGlyphs.at(0).m_fontNo = SMUFL_E9B6_chantEntryLineAsc4th;
                            break;
                        case -4:
                            nc->m_drawingGlyphs.at(0).m_fontNo = SMUFL_E9BC_chantLigaturaDesc5th;
                            previousNc->m_drawingGlyphs.at(0).m_fontNo = SMUFL_E9B7_chantEntryLineAsc5th;
                            break;
                        default: break;
                    }
                }
            }
            // Check if nc is part of a ligature or is an inclinatum
            else if (nc->HasTilt() && nc->GetTilt() == COMPASSDIRECTION_se) {
                nc->m_drawingGlyphs.at(0).m_fontNo = SMUFL_E991_chantPunctumInclinatum;
                // No overlap with this shape
                overlapWithPrevious = false;
            }
            // If the nc is supposed to be a virga and currently is being rendered as a punctum
            // change it to a virga
            else if (nc->GetTilt() == COMPASSDIRECTION_s
                && nc->m_drawingGlyphs.at(0).m_fontNo == SMUFL_E990_chantPunctum) {
                nc->m_drawingGlyphs.at(0).m_fontNo = SMUFL_E996_chantPunctumVirga;
            }
            else if (nc->GetTilt() == COMPASSDIRECTION_n
                && nc->m_drawingGlyphs.at(0).m_fontNo == SMUFL_E990_chantPunctum) {
                nc->m_drawingGlyphs.at(0).m_fontNo = SMUFL_E997_chantPunctumVirgaReversed;
            }
        }

        // xRel remains unset with facsimile
        if (!m_doc->HasFacsimile()) {
            // If the nc overlaps with the previous, move it back from a line width
            if (overlapWithPrevious) {
                xRel -= lineWidth;
            }

            nc->SetDrawingXRel(xRel);
            // The first glyph set the spacing - unless we are starting a ligature, in which case no spacing should be
            // added between the two nc
            if (!previousLig) {
                xRel += m_doc->GetGlyphWidth(nc->m_drawingGlyphs.at(0).m_fontNo, staffSize, false);
            }
        }

        previousNc = nc;
    }

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
