/////////////////////////////////////////////////////////////////////////////
// Name:        view_element.cpp
// Author:      Laurent Pugin and Chris Niven
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <iostream>
#include <math.h>

//----------------------------------------------------------------------------

#include "accid.h"
#include "artic.h"
#include "beam.h"
#include "beatrpt.h"
#include "btrem.h"
#include "chord.h"
#include "clef.h"
#include "custos.h"
#include "devicecontext.h"
#include "doc.h"
#include "dot.h"
#include "dynam.h"
#include "elementpart.h"
#include "f.h"
#include "ftrem.h"
#include "functorparams.h"
#include "halfmrpt.h"
#include "keysig.h"
#include "label.h"
#include "labelabbr.h"
#include "layer.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "mrest.h"
#include "mrpt.h"
#include "mrpt2.h"
#include "mspace.h"
#include "multirest.h"
#include "multirpt.h"
#include "neume.h"
#include "note.h"
#include "options.h"
#include "proport.h"
#include "rest.h"
#include "smufl.h"
#include "space.h"
#include "staff.h"
#include "syl.h"
#include "system.h"
#include "textelement.h"
#include "tie.h"
#include "tuplet.h"
#include "verse.h"
#include "vrv.h"
#include "zone.h"

namespace vrv {

void View::DrawLayerElement(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    if (element->HasSameas()) {
        dc->StartGraphic(element, "", element->GetUuid());
        element->SetEmptyBB();
        dc->EndGraphic(element, this);
        return;
    }

    int previousColor = m_currentColour;

    if (element == m_currentElement) {
        m_currentColour = AxRED;
    }
    else {
        m_currentColour = AxNONE;
    }

    if (element->Is(ACCID)) {
        DrawAccid(dc, element, layer, staff, measure);
    }
    else if (element->Is(ARTIC)) {
        DrawArtic(dc, element, layer, staff, measure);
    }
    else if (element->Is(ARTIC_PART)) {
        DrawArticPart(dc, element, layer, staff, measure);
    }
    else if (element->Is(BARLINE)) {
        DrawBarLine(dc, element, layer, staff, measure);
    }
    else if (element->Is(BEAM)) {
        DrawBeam(dc, element, layer, staff, measure);
    }
    else if (element->Is(BEATRPT)) {
        DrawBeatRpt(dc, element, layer, staff, measure);
    }
    else if (element->Is(BTREM)) {
        DrawBTrem(dc, element, layer, staff, measure);
    }
    else if (element->Is(CHORD)) {
        DrawDurationElement(dc, element, layer, staff, measure);
    }
    else if (element->Is(CLEF)) {
        DrawClef(dc, element, layer, staff, measure);
    }
    else if (element->Is(CUSTOS)) {
        DrawCustos(dc, element, layer, staff, measure);
    }
    else if (element->Is(DOT)) {
        DrawDot(dc, element, layer, staff, measure);
    }
    else if (element->Is(DOTS)) {
        DrawDots(dc, element, layer, staff, measure);
    }
    else if (element->Is(FTREM)) {
        DrawFTrem(dc, element, layer, staff, measure);
    }
    else if (element->Is(FLAG)) {
        DrawFlag(dc, element, layer, staff, measure);
    }
    else if (element->Is(GRACEGRP)) {
        DrawGraceGrp(dc, element, layer, staff, measure);
    }
    else if (element->Is(HALFMRPT)) {
        DrawHalfmRpt(dc, element, layer, staff, measure);
    }
    else if (element->Is(KEYSIG)) {
        DrawKeySig(dc, element, layer, staff, measure);
    }
    else if (element->Is(LIGATURE)) {
        DrawLigature(dc, element, layer, staff, measure);
    }
    else if (element->Is(MENSUR)) {
        DrawMensur(dc, element, layer, staff, measure);
    }
    else if (element->Is(METERSIG)) {
        DrawMeterSig(dc, element, layer, staff, measure);
    }
    else if (element->Is(MREST)) {
        DrawMRest(dc, element, layer, staff, measure);
    }
    else if (element->Is(MRPT)) {
        DrawMRpt(dc, element, layer, staff, measure);
    }
    else if (element->Is(MRPT2)) {
        DrawMRpt2(dc, element, layer, staff, measure);
    }
    else if (element->Is(MSPACE)) {
        DrawMSpace(dc, element, layer, staff, measure);
    }
    else if (element->Is(MULTIREST)) {
        DrawMultiRest(dc, element, layer, staff, measure);
    }
    else if (element->Is(MULTIRPT)) {
        DrawMultiRpt(dc, element, layer, staff, measure);
    }
    else if (element->Is(NC)) {
        DrawNc(dc, element, layer, staff, measure);
    }
    else if (element->Is(NOTE)) {
        DrawDurationElement(dc, element, layer, staff, measure);
    }
    else if (element->Is(NEUME)) {
        DrawNeume(dc, element, layer, staff, measure);
    }
    else if (element->Is(PLICA)) {
        DrawPlica(dc, element, layer, staff, measure);
    }
    else if (element->Is(PROPORT)) {
        DrawProport(dc, element, layer, staff, measure);
    }
    else if (element->Is(REST)) {
        DrawDurationElement(dc, element, layer, staff, measure);
    }
    else if (element->Is(SPACE)) {
        DrawSpace(dc, element, layer, staff, measure);
    }
    else if (element->Is(STEM)) {
        DrawStem(dc, element, layer, staff, measure);
    }
    else if (element->Is(SYL)) {
        DrawSyl(dc, element, layer, staff, measure);
    }
    else if (element->Is(SYLLABLE)) {
        DrawSyllable(dc, element, layer, staff, measure);
    }
    else if (element->Is(TUPLET)) {
        DrawTuplet(dc, element, layer, staff, measure);
    }
    else if (element->Is(TUPLET_BRACKET)) {
        dc->StartGraphic(element, "", element->GetUuid());
        dc->EndGraphic(element, this);
        layer->AddToDrawingList(element);
    }
    else if (element->Is(TUPLET_NUM)) {
        dc->StartGraphic(element, "", element->GetUuid());
        dc->EndGraphic(element, this);
        layer->AddToDrawingList(element);
    }
    else if (element->Is(VERSE)) {
        DrawVerse(dc, element, layer, staff, measure);
    }
    else {
        // This should never happen
        LogError("Element '%s' cannot be drawn", element->GetClassName().c_str());
    }

    m_currentColour = previousColor;
}

//----------------------------------------------------------------------------
// View - LayerElement
//----------------------------------------------------------------------------

void View::DrawAccid(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Accid *accid = vrv_cast<Accid *>(element);
    assert(accid);

    // This can happen with accid within note with only accid.ges
    // We still create an graphic in the output
    if (!accid->HasAccid()) {
        dc->StartGraphic(element, "", element->GetUuid());
        accid->SetEmptyBB();
        dc->EndGraphic(element, this);
        return;
    }

    dc->StartGraphic(element, "", element->GetUuid());

    /************** editorial accidental **************/

    std::wstring accidStr = accid->GetSymbolStr();

    int x = accid->GetDrawingX();
    int y = accid->GetDrawingY();

    if ((accid->GetFunc() == accidLog_FUNC_edit) && (!accid->HasEnclose())) {
        y = staff->GetDrawingY();
        // look at the note position and adjust it if necessary
        Note *note = dynamic_cast<Note *>(accid->GetFirstAncestor(NOTE, MAX_ACCID_DEPTH));
        if (note) {
            // Check if the note is on the top line or above (add a unit for the note head half size)
            if (note->GetDrawingY() >= y) y = note->GetDrawingY() + m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
            // Check if the top of the stem is above
            if ((note->GetDrawingStemDir() == STEMDIRECTION_up) && (note->GetDrawingStemEnd(note).y > y))
                y = note->GetDrawingStemEnd(note).y;
            // Increase the x position of the accid
            x += note->GetDrawingRadius(m_doc);
        }
        TextExtend extend;
        dc->SetFont(m_doc->GetDrawingSmuflFont(staff->m_drawingStaffSize, accid->GetDrawingCueSize()));
        dc->GetSmuflTextExtent(accid->GetSymbolStr(), &extend);
        dc->ResetFont();
        y += extend.m_descent + m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    }

    DrawSmuflString(
        dc, x, y, accidStr, HORIZONTALALIGNMENT_center, staff->m_drawingStaffSize, accid->GetDrawingCueSize(), true);

    dc->EndGraphic(element, this);
}

void View::DrawArtic(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Artic *artic = vrv_cast<Artic *>(element);
    assert(artic);

    /************** draw the artic **************/

    dc->StartGraphic(element, "", element->GetUuid());

    DrawLayerChildren(dc, artic, layer, staff, measure);

    dc->EndGraphic(element, this);
}

void View::DrawArticPart(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    ArticPart *articPart = vrv_cast<ArticPart *>(element);
    assert(articPart);

    /************** draw the artic **************/

    int x = articPart->GetDrawingX();
    // HARDCODED value, we double the default margin for now - should go in styling
    int yShift = 2 * m_doc->GetTopMargin(articPart->GetClassId()) * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    int direction = (articPart->GetPlace() == STAFFREL_above) ? 1 : -1;

    int y = articPart->GetDrawingY();

    int xCorr;
    int baselineCorr;

    bool drawingCueSize = true;

    dc->StartGraphic(element, "", element->GetUuid());

    dc->SetFont(m_doc->GetDrawingSmuflFont(staff->m_drawingStaffSize, drawingCueSize));

    std::vector<data_ARTICULATION>::iterator articIter;
    std::vector<data_ARTICULATION> articList = articPart->GetArtic();
    for (articIter = articList.begin(); articIter != articList.end(); ++articIter) {

        wchar_t code = Artic::GetSmuflCode(*articIter, articPart->GetPlace());

        // Skip it if we do not have it in the font (for now - we should log / document this somewhere)
        if (code == 0) {
            articPart->SetEmptyBB();
            continue;
        }

        if (articPart->GetType() == ARTIC_PART_INSIDE) {
            // If we are above the top of the  staff, just pile them up
            if ((articPart->GetPlace() == STAFFREL_above) && (y > staff->GetDrawingY())) y += yShift;
            // If we are below the bottom, just pile the down
            else if ((articPart->GetPlace() == STAFFREL_below)
                && (y < staff->GetDrawingY() - m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize)))
                y -= yShift;
            // Otherwise make it fit the staff space
            else {
                y = GetNearestInterStaffPosition(y, staff, articPart->GetPlace());
                if (IsOnStaffLine(y, staff)) y += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * direction;
            }
        }
        // Artic part outside just need to be piled up or down
        else
            y += yShift * direction;

        // The correction for centering the glyph
        xCorr = m_doc->GetGlyphWidth(code, staff->m_drawingStaffSize, drawingCueSize) / 2;
        // The position of the next glyph (and for correcting the baseline if necessary
        int glyphHeight = m_doc->GetGlyphHeight(code, staff->m_drawingStaffSize, drawingCueSize);

        // Center the glyh if necessary
        if (Artic::IsCentered(*articIter)) {
            y += (articPart->GetPlace() == STAFFREL_above) ? -(glyphHeight / 2) : (glyphHeight / 2);
        }

        // Adjust the baseline for glyph above the baseline in SMuFL
        baselineCorr = 0;
        if (Artic::VerticalCorr(code, articPart->GetPlace())) baselineCorr = glyphHeight;

        DrawSmuflCode(dc, x - xCorr, y - baselineCorr, code, staff->m_drawingStaffSize, drawingCueSize);

        // Adjusting the y position for the next artic
        y += glyphHeight * direction;
    }

    dc->EndGraphic(element, this);
}

void View::DrawBarLine(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    BarLine *barLine = vrv_cast<BarLine *>(element);
    assert(barLine);

    if (barLine->GetForm() == BARRENDITION_invis) {
        barLine->SetEmptyBB();
        return;
    }

    dc->StartGraphic(element, "", element->GetUuid());

    int yTop = staff->GetDrawingY();
    int yBottom = yTop - (staff->m_drawingLines - 1) * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);

    int offset = (yTop == yBottom) ? m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) : 0;

    DrawBarLine(dc, yTop + offset, yBottom - offset, barLine);
    if (barLine->HasRepetitionDots()) {
        DrawBarLineDots(dc, staff, barLine);
    }

    dc->EndGraphic(element, this);
}

void View::DrawBeatRpt(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    BeatRpt *beatRpt = vrv_cast<BeatRpt *>(element);
    assert(beatRpt);

    dc->StartGraphic(element, "", element->GetUuid());

    int x = element->GetDrawingX();
    int xSymbol = x;
    int y = element->GetDrawingY();
    y -= staff->m_drawingLines / 2 * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);

    if (beatRpt->GetSlash() == BEATRPT_REND_mixed) {
        DrawSmuflCode(dc, xSymbol, y, SMUFL_E501_repeat2Bars, staff->m_drawingStaffSize, false);
    }
    else {
        DrawSmuflCode(dc, xSymbol, y, SMUFL_E101_noteheadSlashHorizontalEnds, staff->m_drawingStaffSize, false);
        int additionalSlash = beatRpt->GetSlash() - BEATRPT_REND_1;
        int halfWidth
            = m_doc->GetGlyphWidth(SMUFL_E101_noteheadSlashHorizontalEnds, staff->m_drawingStaffSize, false) / 2;
        int i;
        for (i = 0; i < additionalSlash; ++i) {
            xSymbol += halfWidth;
            DrawSmuflCode(dc, xSymbol, y, SMUFL_E101_noteheadSlashHorizontalEnds, staff->m_drawingStaffSize, false);
        }
    }

    dc->EndGraphic(element, this);
}

void View::DrawBTrem(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    BTrem *bTrem = vrv_cast<BTrem *>(element);
    assert(bTrem);

    data_STEMDIRECTION stemDir = STEMDIRECTION_NONE;
    data_STEMMODIFIER stemMod;
    int drawingDur = 0;
    LayerElement *childElement = NULL;
    Note *childNote = NULL;
    Point stemPoint;
    bool drawingCueSize = false;
    int x, y;

    Chord *childChord = dynamic_cast<Chord *>(bTrem->FindDescendantByType(CHORD));
    // Get from the chord or note child
    if (childChord) {
        drawingDur = childChord->GetDur();
        childElement = childChord;
    }
    else {
        childNote = dynamic_cast<Note *>(bTrem->FindDescendantByType(NOTE));
        if (childNote) {
            drawingDur = childNote->GetDur();
            childElement = childNote;
        }
    }

    if (!childElement) {
        bTrem->SetEmptyBB();
        return;
    }

    dc->StartGraphic(element, "", element->GetUuid());

    DrawLayerChildren(dc, bTrem, layer, staff, measure);

    // Get stem values from the chord or note child
    if (childChord) {
        Stem *stem = childChord->GetDrawingStem();
        stemDir = childChord->GetDrawingStemDir();
        stemMod = stem ? stem->GetStemMod() : STEMMODIFIER_NONE;
        stemPoint = childChord->GetDrawingStemStart(childChord);
    }
    else {
        Stem *stem = childNote->GetDrawingStem();
        drawingCueSize = childNote->GetDrawingCueSize();
        stemDir = childNote->GetDrawingStemDir();
        stemMod = stem ? stem->GetStemMod() : STEMMODIFIER_NONE;
        stemPoint = childNote->GetDrawingStemStart(childNote);
    }

    // conlude from logical attributes
    if (bTrem->HasUnitdur() && (stemMod == STEMMODIFIER_NONE)) {
        int slashDur = bTrem->GetUnitdur() - drawingDur;
        if (drawingDur < DUR_4) slashDur = bTrem->GetUnitdur() - DUR_4;
        switch (slashDur) {
            case (0): stemMod = STEMMODIFIER_NONE; break;
            case (1): stemMod = STEMMODIFIER_1slash; break;
            case (2): stemMod = STEMMODIFIER_2slash; break;
            case (3): stemMod = STEMMODIFIER_3slash; break;
            case (4): stemMod = STEMMODIFIER_4slash; break;
            case (5): stemMod = STEMMODIFIER_5slash; break;
            case (6): stemMod = STEMMODIFIER_6slash; break;
            default: break;
        }
    }

    int beamWidthBlack = m_doc->GetDrawingBeamWidth(staff->m_drawingStaffSize, drawingCueSize);
    int beamWidthWhite = m_doc->GetDrawingBeamWhiteWidth(staff->m_drawingStaffSize, drawingCueSize);
    int width = m_doc->GetGlyphWidth(SMUFL_E0A3_noteheadHalf, staff->m_drawingStaffSize, drawingCueSize);
    int height = beamWidthBlack * 2 / 3;
    int step = beamWidthBlack + beamWidthWhite;

    if (stemDir == STEMDIRECTION_up) {
        if (drawingDur > DUR_1) {
            // Since we are adding the slashing on the stem, ignore artic
            y = childElement->GetDrawingTop(m_doc, staff->m_drawingStaffSize, false)
                - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2;
            x = stemPoint.x;
        }
        else {
            // Take into account artic (not likely, though)
            y = childElement->GetDrawingTop(m_doc, staff->m_drawingStaffSize)
                + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) + (stemMod - 2) * step;
            x = childElement->GetDrawingX() + childElement->GetDrawingRadius(m_doc);
        }
        if (drawingDur > DUR_4) {
            Flag *flag = NULL;
            flag = dynamic_cast<Flag *>(childElement->FindDescendantByType(FLAG));
            if (flag) y -= (drawingDur > DUR_8) ? 2 * step : step;
        }
        step = -step;
    }
    else {
        if (drawingDur > DUR_1) {
            // Idem as above
            y = childElement->GetDrawingBottom(m_doc, staff->m_drawingStaffSize, false)
                + m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
            x = stemPoint.x;
        }
        else {
            y = childElement->GetDrawingBottom(m_doc, staff->m_drawingStaffSize)
                - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2 - (stemMod - 2) * step;
            x = childElement->GetDrawingX() + childElement->GetDrawingRadius(m_doc);
        }
        if (drawingDur > DUR_4) {
            Flag *flag = NULL;
            flag = dynamic_cast<Flag *>(childElement->FindDescendantByType(FLAG));
            if (flag) y += (drawingDur > DUR_8) ? 2 * step : step;
        }
    }

    Beam *beam = childElement->IsInBeam();
    if (beam) {
        int beamStep = (drawingDur - DUR_8) * (beamWidthBlack + beamWidthWhite) + beamWidthWhite;
        y += (stemDir == STEMDIRECTION_down) ? beamStep : -beamStep;
    }

    // by default draw 3 slashes (e.g., for a temolo on a whole note)
    if ((stemMod == STEMMODIFIER_NONE) && (drawingDur < DUR_2)) stemMod = STEMMODIFIER_3slash;
    if (stemMod == STEMMODIFIER_z) {
        if (stemDir == STEMDIRECTION_down) y += m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        DrawSmuflCode(dc, x, y, SMUFL_E22A_buzzRoll, staff->m_drawingStaffSize, false);
    }
    else if (stemMod != STEMMODIFIER_sprech) {
        for (int s = 1; s < stemMod; ++s) {
            DrawObliquePolygon(dc, x - width / 2, y - height / 2, x + width / 2, y + height / 2, beamWidthBlack);
            y += step;
        }
    }

    dc->EndGraphic(element, this);
}

void View::DrawChord(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Chord *chord = vrv_cast<Chord *>(element);
    assert(chord);

    if (chord->m_crossStaff) staff = chord->m_crossStaff;

    chord->ResetDrawingList();

    /************ Draw children (notes, accidentals, etc) ************/

    DrawLayerChildren(dc, chord, layer, staff, measure);
}

void View::DrawClef(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Clef *clef = vrv_cast<Clef *>(element);
    assert(clef);
    int x, y;
    if (m_doc->GetType() == Facs && clef->HasFacs()) {
        y = ToLogicalY(staff->GetDrawingY());
        x = clef->GetDrawingX();
    }
    else {
        y = staff->GetDrawingY();
        x = element->GetDrawingX();
    }
    int sym = 0;
    bool isMensural = Att::IsMensuralType(staff->m_drawingNotationType);
    bool isNeume = staff->m_drawingNotationType == NOTATIONTYPE_neume;

    // cmn clefs
    int shapeOctaveDis = Clef::ClefId(clef->GetShape(), 0, clef->GetDis(), clef->GetDisPlace());
    // G clef
    if (shapeOctaveDis == Clef::ClefId(CLEFSHAPE_G, 0, OCTAVE_DIS_NONE, STAFFREL_basic_NONE))
        sym = SMUFL_E050_gClef;
    else if (shapeOctaveDis == Clef::ClefId(CLEFSHAPE_G, 0, OCTAVE_DIS_8, STAFFREL_basic_below))
        sym = SMUFL_E052_gClef8vb;
    else if (shapeOctaveDis == Clef::ClefId(CLEFSHAPE_G, 0, OCTAVE_DIS_15, STAFFREL_basic_below))
        sym = SMUFL_E051_gClef15mb;
    else if (shapeOctaveDis == Clef::ClefId(CLEFSHAPE_G, 0, OCTAVE_DIS_8, STAFFREL_basic_above))
        sym = SMUFL_E053_gClef8va;
    else if (shapeOctaveDis == Clef::ClefId(CLEFSHAPE_G, 0, OCTAVE_DIS_15, STAFFREL_basic_above))
        sym = SMUFL_E054_gClef15ma;
    // C clef
    else if (shapeOctaveDis == Clef::ClefId(CLEFSHAPE_C, 0, OCTAVE_DIS_NONE, STAFFREL_basic_NONE))
        sym = SMUFL_E05C_cClef;
    else if (shapeOctaveDis == Clef::ClefId(CLEFSHAPE_C, 0, OCTAVE_DIS_8, STAFFREL_basic_below))
        sym = SMUFL_E05D_cClef8vb;
    else if (clef->GetShape() == CLEFSHAPE_C)
        sym = SMUFL_E05C_cClef;
    // F clef
    else if (shapeOctaveDis == Clef::ClefId(CLEFSHAPE_F, 0, OCTAVE_DIS_NONE, STAFFREL_basic_NONE))
        sym = SMUFL_E062_fClef;
    else if (shapeOctaveDis == Clef::ClefId(CLEFSHAPE_F, 0, OCTAVE_DIS_8, STAFFREL_basic_below))
        sym = SMUFL_E064_fClef8vb;
    else if (shapeOctaveDis == Clef::ClefId(CLEFSHAPE_F, 0, OCTAVE_DIS_15, STAFFREL_basic_below))
        sym = SMUFL_E063_fClef15mb;
    else if (shapeOctaveDis == Clef::ClefId(CLEFSHAPE_F, 0, OCTAVE_DIS_8, STAFFREL_basic_above))
        sym = SMUFL_E065_fClef8va;
    else if (shapeOctaveDis == Clef::ClefId(CLEFSHAPE_F, 0, OCTAVE_DIS_15, STAFFREL_basic_above))
        sym = SMUFL_E066_fClef15ma;
    else if (clef->GetShape() == CLEFSHAPE_F)
        sym = SMUFL_E062_fClef;
    // Perc clef
    else if (clef->GetShape() == CLEFSHAPE_perc)
        sym = SMUFL_E069_unpitchedPercussionClef1;

    // mensural clefs
    if (isMensural) {
        if (staff->m_drawingNotationType == NOTATIONTYPE_mensural_black) {
            if (sym == SMUFL_E050_gClef)
                // G clef doesn't exist in black notation, so should never get here, but just in case.
                sym = SMUFL_E901_mensuralGclefPetrucci;
            else if (sym == SMUFL_E05C_cClef)
                sym = SMUFL_E906_chantCclef;
            else if (sym == SMUFL_E062_fClef)
                sym = SMUFL_E902_chantFclef;
        }
        else {
            if (sym == SMUFL_E050_gClef)
                sym = SMUFL_E901_mensuralGclefPetrucci;
            else if (sym == SMUFL_E05C_cClef)
                sym = SMUFL_E909_mensuralCclefPetrucciPosMiddle;
            else if (sym == SMUFL_E062_fClef)
                sym = SMUFL_E904_mensuralFclefPetrucci;
        }
    }
    // neume clefs
    else if (isNeume) {
        if (clef->GetShape() == CLEFSHAPE_C)
            sym = SMUFL_E906_chantCclef;
        else if (clef->GetShape() == CLEFSHAPE_F)
            sym = SMUFL_E902_chantFclef;
    }

    if (sym == 0) {
        clef->SetEmptyBB();
        return;
    }

    if (clef->HasLine()) {
        y -= m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * (staff->m_drawingLines - clef->GetLine());
        if ((m_doc->GetType() == Facs) && (staff->GetDrawingRotate() != 0)) {
            double deg = staff->GetDrawingRotate();
            int xDiff = x - staff->GetDrawingX();
            y -= int(xDiff * tan(deg * M_PI / 180.0));
        }
    }
    else if (clef->GetShape() == CLEFSHAPE_perc) {
        y -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * (staff->m_drawingLines - 1);
    }
    else {
        // we have no information where to put it
        return;
    }

    bool cueSize = false;
    if (clef->GetAlignment() && (clef->GetAlignment()->GetType() == ALIGNMENT_CLEF)) {
        if (m_doc->GetType() != Transcription && m_doc->GetType() != Facs) {
            cueSize = true;
            // HARDCODED
            x -= m_doc->GetGlyphWidth(sym, staff->m_drawingStaffSize, cueSize) * 1.35;
        }
    }

    dc->StartGraphic(element, "", element->GetUuid());

    DrawSmuflCode(dc, x, y, sym, staff->m_drawingStaffSize, cueSize);

    if ((m_doc->GetType() == Facs) && element->HasFacs()) {
        const int noteHeight = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 2);
        const int noteWidth = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 1.4);

        FacsimileInterface *fi = dynamic_cast<FacsimileInterface *>(element);
        fi->GetZone()->SetUlx(x);
        fi->GetZone()->SetUly(ToDeviceContextY(y));
        fi->GetZone()->SetLrx(x + noteWidth);
        fi->GetZone()->SetLry(ToDeviceContextY(y - noteHeight));
    }

    dc->EndGraphic(element, this);
}

void View::DrawCustos(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Custos *custos = vrv_cast<Custos *>(element);
    assert(custos);

    dc->StartGraphic(element, "", element->GetUuid());

    int sym = 0;
    // Select glyph to use for this custos
    switch (staff->m_drawingNotationType) {
        case NOTATIONTYPE_neume:
            sym = SMUFL_EA06_chantCustosStemUpPosMiddle; // chantCustosStemUpPosMiddle
            break;
        default:
            sym = SMUFL_EA02_mensuralCustosUp; // mensuralCustosUp
            break;
    }

    Clef *clef = layer->GetClef(element);
    int staffSize = m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
    int staffLineNumber = staff->m_drawingLines;
    int clefLine = clef->GetLine();

    int x, y;
    if (custos->HasFacs() && m_doc->GetType() == Facs) {
        x = custos->GetDrawingX();
        y = ToLogicalY(staff->GetDrawingY());
    }
    else {
        x = element->GetDrawingX();
        y = element->GetDrawingY();
        // Because SMuFL does not have the origin correpsonding to the pitch as for notes, we need to correct it.
        // This will remain approximate
        y -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    }

    int clefY = y - (staffSize * (staffLineNumber - clefLine));
    int pitchOffset;
    int octaveOffset = (custos->GetOct() - 3) * ((staffSize / 2) * 7);
    int rotateOffset;
    if ((m_doc->GetType() == Facs) && (staff->GetDrawingRotate() != 0)) {
        double deg = staff->GetDrawingRotate();
        int xDiff = x - staff->GetDrawingX();
        rotateOffset = int(xDiff * tan(deg * M_PI / 180.0));
    }
    else {
        rotateOffset = 0;
    }

    if (clef->GetShape() == CLEFSHAPE_C) {
        pitchOffset = (custos->GetPname() - PITCHNAME_c) * (staffSize / 2);
    }
    else if (clef->GetShape() == CLEFSHAPE_F) {
        pitchOffset = (custos->GetPname() - PITCHNAME_f) * (staffSize / 2);
    }
    else if (clef->GetShape() == CLEFSHAPE_G) {
        pitchOffset = (custos->GetPname() - PITCHNAME_g) * (staffSize / 2);
    }
    else {
        // This shouldn't happen
        pitchOffset = 0;
    }

    int actualY = clefY + pitchOffset + octaveOffset - rotateOffset;

    DrawSmuflCode(dc, x, actualY, sym, staff->m_drawingStaffSize, false, true);

    if ((m_doc->GetType() == Facs) && element->HasFacs()) {
        const int noteHeight = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 2);
        const int noteWidth = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 1.4);

        FacsimileInterface *fi = dynamic_cast<FacsimileInterface *>(element);
        fi->GetZone()->SetUlx(x);
        fi->GetZone()->SetUly(ToDeviceContextY(actualY));
        fi->GetZone()->SetLrx(x + noteWidth);
        fi->GetZone()->SetLry(ToDeviceContextY(actualY - noteHeight));
    }

    dc->EndGraphic(element, this);
}

void View::DrawDot(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Dot *dot = vrv_cast<Dot *>(element);
    assert(dot);

    dc->StartGraphic(element, "", element->GetUuid());

    if (dot->m_drawingNote && dot->m_drawingNote->IsInLigature()) {
        this->DrawDotInLigature(dc, element, layer, staff, measure);
    }
    else {
        int x = element->GetDrawingX();
        int y = element->GetDrawingY();

        if (m_doc->GetType() != Transcription) {
            // Use the note to which the points to for position
            if (dot->m_drawingNote && !dot->m_drawingNextElement) {
                x += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 7 / 2;
                y = dot->m_drawingNote->GetDrawingY();
            }
            if (dot->m_drawingNote && dot->m_drawingNextElement) {
                x += ((dot->m_drawingNextElement->GetDrawingX() - dot->m_drawingNote->GetDrawingX()) / 2);
                x += dot->m_drawingNote->GetDrawingRadius(m_doc);
                y = dot->m_drawingNote->GetDrawingY();
            }
        }

        DrawDotsPart(dc, x, y, 1, staff);
    }

    dc->EndGraphic(element, this);
}

void View::DrawDots(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Dots *dots = vrv_cast<Dots *>(element);
    assert(dots);

    dc->StartGraphic(element, "", element->GetUuid());

    MapOfDotLocs::const_iterator iter;
    const MapOfDotLocs *map = dots->GetMapOfDotLocs();
    for (iter = map->begin(); iter != map->end(); ++iter) {
        Staff *dotStaff = (iter->first) ? iter->first : staff;
        int y = dotStaff->GetDrawingY()
            - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * (dotStaff->m_drawingLines - 1);
        int x = dots->GetDrawingX() + m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        const std::list<int> *dotLocs = &iter->second;
        std::list<int>::const_iterator intIter;
        for (intIter = dotLocs->begin(); intIter != dotLocs->end(); ++intIter) {
            DrawDotsPart(
                dc, x, y + (*intIter) * m_doc->GetDrawingUnit(staff->m_drawingStaffSize), dots->GetDots(), dotStaff);
        }
    }

    dc->EndGraphic(element, this);
}

void View::DrawDurationElement(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    if (dynamic_cast<Chord *>(element)) {
        dc->StartGraphic(element, "", element->GetUuid());
        DrawChord(dc, element, layer, staff, measure);
        dc->EndGraphic(element, this);
    }
    else if (dynamic_cast<Note *>(element)) {
        dc->StartGraphic(element, "", element->GetUuid());
        DrawNote(dc, element, layer, staff, measure);
        dc->EndGraphic(element, this);
    }
    else if (dynamic_cast<Rest *>(element)) {
        dc->StartGraphic(element, "", element->GetUuid());
        DrawRest(dc, element, layer, staff, measure);
        dc->EndGraphic(element, this);
    }
}

void View::DrawFlag(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Flag *flag = vrv_cast<Flag *>(element);
    assert(flag);

    Stem *stem = vrv_cast<Stem *>(flag->GetFirstAncestor(STEM));
    assert(stem);

    int x = flag->GetDrawingX() - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2;
    int y = flag->GetDrawingY();

    dc->StartGraphic(element, "", element->GetUuid());

    wchar_t code = flag->GetSmuflCode(stem->GetDrawingStemDir());
    DrawSmuflCode(dc, x, y, code, staff->m_drawingStaffSize, flag->GetDrawingCueSize());

    dc->EndGraphic(element, this);
}

void View::DrawGraceGrp(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    dc->StartGraphic(element, "", element->GetUuid());

    // basically nothing to do here
    DrawLayerChildren(dc, element, layer, staff, measure);

    dc->EndGraphic(element, this);
}

void View::DrawHalfmRpt(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    HalfmRpt *halfmRpt = vrv_cast<HalfmRpt *>(element);
    assert(halfmRpt);

    int x = halfmRpt->GetDrawingX();
    x += m_doc->GetGlyphWidth(SMUFL_E500_repeat1Bar, staff->m_drawingStaffSize, false) / 2;

    dc->StartGraphic(element, "", element->GetUuid());

    DrawMRptPart(dc, x, SMUFL_E500_repeat1Bar, 0, false, staff);

    dc->EndGraphic(element, this);
}

void View::DrawKeySig(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    KeySig *keySig = vrv_cast<KeySig *>(element);
    assert(keySig);

    int x, y, i;

    Clef *c = layer->GetClef(element);
    if (!c) {
        keySig->SetEmptyBB();
        return;
    }

    // hidden key signature
    if (keySig->GetVisible() == BOOLEAN_false) {
        keySig->SetEmptyBB();
        return;
    }

    // C major (0) key sig and no cancellation
    else if ((keySig->GetAccidCount() == 0) && (keySig->m_drawingCancelAccidCount == 0)) {
        keySig->SetEmptyBB();
        return;
    }

    // C major (0) key sig and system scoreDef - cancellation (if any) is done at the end of the previous system
    else if ((keySig->GetScoreDefRole() == SCOREDEF_SYSTEM) && (keySig->GetAccidCount() == 0)) {
        keySig->SetEmptyBB();
        return;
    }

    x = element->GetDrawingX();
    // HARDCODED
    int naturalGlyphWidth = m_doc->GetGlyphWidth(SMUFL_E261_accidentalNatural, staff->m_drawingStaffSize, false);
    int step = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    int naturalStep = step * TEMP_KEYSIG_NATURAL_STEP;
    step *= TEMP_KEYSIG_STEP;

    int clefLocOffset = layer->GetClefLocOffset(element);
    int loc;

    // Show cancellation if C major (0)
    // This is not meant to make sense with mixed key signature
    if ((keySig->GetScoreDefRole() != SCOREDEF_SYSTEM) && (keySig->GetAccidCount() == 0)) {
        dc->StartGraphic(element, "", element->GetUuid());

        for (i = 0; i < keySig->m_drawingCancelAccidCount; ++i) {
            data_PITCHNAME pitch = KeySig::GetAccidPnameAt(keySig->m_drawingCancelAccidType, i);
            loc = PitchInterface::CalcLoc(
                pitch, KeySig::GetOctave(keySig->m_drawingCancelAccidType, pitch, c), clefLocOffset);
            y = staff->GetDrawingY() + staff->CalcPitchPosYRel(m_doc, loc);

            DrawSmuflCode(dc, x, y, SMUFL_E261_accidentalNatural, staff->m_drawingStaffSize, false);
            x += naturalGlyphWidth + naturalStep;
        }

        dc->EndGraphic(element, this);
        return;
    }

    dc->StartGraphic(element, "", element->GetUuid());

    // Show cancellation if show cancellation (showchange) is true (false by default)
    // This is not meant to make sense with mixed key signature
    if ((keySig->GetScoreDefRole() != SCOREDEF_SYSTEM) && (keySig->GetSigShowchange() == BOOLEAN_true)) {
        // The type of alteration is different (f/s or f/n or s/n) - cancel all accid in the normal order
        if (keySig->GetAccidType() != keySig->m_drawingCancelAccidType) {
            for (i = 0; i < keySig->m_drawingCancelAccidCount; ++i) {
                data_PITCHNAME pitch = KeySig::GetAccidPnameAt(keySig->m_drawingCancelAccidType, i);
                loc = PitchInterface::CalcLoc(
                    pitch, KeySig::GetOctave(keySig->m_drawingCancelAccidType, pitch, c), clefLocOffset);
                y = staff->GetDrawingY() + staff->CalcPitchPosYRel(m_doc, loc);

                DrawSmuflCode(dc, x, y, SMUFL_E261_accidentalNatural, staff->m_drawingStaffSize, false);
                x += naturalGlyphWidth + naturalStep;
            }
        }
    }

    dc->SetFont(m_doc->GetDrawingSmuflFont(staff->m_drawingStaffSize, false));

    for (i = 0; i < keySig->GetAccidCount(); ++i) {
        // We get the pitch from the keySig (looks for keyAccid children if any)
        data_ACCIDENTAL_WRITTEN accid;
        data_PITCHNAME pname;
        std::wstring accidStr = keySig->GetKeyAccidStrAt(i, accid, pname);

        loc = PitchInterface::CalcLoc(pname, KeySig::GetOctave(accid, pname, c), clefLocOffset);
        y = staff->GetDrawingY() + staff->CalcPitchPosYRel(m_doc, loc);

        DrawSmuflString(dc, x, y, accidStr, HORIZONTALALIGNMENT_left, staff->m_drawingStaffSize, false);
        TextExtend extend;
        dc->GetSmuflTextExtent(accidStr, &extend);
        x += extend.m_width + step;
    }

    dc->ResetFont();

    // Show cancellation if show cancellation (showchange) is true (false by default)
    // This is not meant to make sense with mixed key signature
    if ((keySig->GetScoreDefRole() != SCOREDEF_SYSTEM) && (keySig->GetSigShowchange() == BOOLEAN_true)) {
        // Same time of alteration, but smaller number - cancellation is displayed afterwards
        if ((keySig->GetAccidType() == keySig->m_drawingCancelAccidType)
            && (keySig->GetAccidCount() < keySig->m_drawingCancelAccidCount)) {
            for (i = keySig->GetAccidCount(); i < keySig->m_drawingCancelAccidCount; ++i) {
                data_PITCHNAME pitch = KeySig::GetAccidPnameAt(keySig->m_drawingCancelAccidType, i);
                loc = PitchInterface::CalcLoc(
                    pitch, KeySig::GetOctave(keySig->m_drawingCancelAccidType, pitch, c), clefLocOffset);
                y = staff->GetDrawingY() + staff->CalcPitchPosYRel(m_doc, loc);

                DrawSmuflCode(dc, x, y, SMUFL_E261_accidentalNatural, staff->m_drawingStaffSize, false);
                x += naturalGlyphWidth + naturalStep;
            }
        }
    }

    dc->EndGraphic(element, this);
}

void View::DrawMeterSig(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    MeterSig *meterSig = vrv_cast<MeterSig *>(element);
    assert(meterSig);

    dc->StartGraphic(element, "", element->GetUuid());

    int y = staff->GetDrawingY() - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * (staff->m_drawingLines - 1);
    int x = element->GetDrawingX();

    if (meterSig->GetForm() == METERFORM_invis) {
        // just skip
    }
    else if (meterSig->HasSym()) {
        if (meterSig->GetSym() == METERSIGN_common) {
            DrawSmuflCode(dc, x, y, SMUFL_E08A_timeSigCommon, staff->m_drawingStaffSize, false);
        }
        else if (meterSig->GetSym() == METERSIGN_cut) {
            DrawSmuflCode(dc, x, y, SMUFL_E08B_timeSigCutCommon, staff->m_drawingStaffSize, false);
        }
    }
    else if (meterSig->GetForm() == METERFORM_num) {
        DrawMeterSigFigures(dc, x, y, meterSig->GetCount(), 0, staff);
    }
    else if (meterSig->HasCount()) {
        DrawMeterSigFigures(dc, x, y, meterSig->GetCount(), meterSig->GetUnit(), staff);
    }

    dc->EndGraphic(element, this);
}

void View::DrawMRest(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    MRest *mRest = vrv_cast<MRest *>(element);
    assert(mRest);

    dc->StartGraphic(element, "", element->GetUuid());

    mRest->CenterDrawingX();

    const bool drawingCueSize = mRest->GetDrawingCueSize();
    int x = mRest->GetDrawingX();
    int y = (measure->m_measureAligner.GetMaxTime() >= (DUR_MAX * 2))
        ? element->GetDrawingY() - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize)
        : element->GetDrawingY();
    wchar_t rest
        = (measure->m_measureAligner.GetMaxTime() >= (DUR_MAX * 2)) ? SMUFL_E4E2_restDoubleWhole : SMUFL_E4E3_restWhole;

    x -= m_doc->GetGlyphWidth(rest, staff->m_drawingStaffSize, drawingCueSize) / 2;

    DrawSmuflCode(dc, x, y, rest, staff->m_drawingStaffSize, drawingCueSize);

    dc->EndGraphic(element, this);
}

void View::DrawMRpt(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    MRpt *mRpt = vrv_cast<MRpt *>(element);
    assert(mRpt);

    mRpt->CenterDrawingX();

    dc->StartGraphic(element, "", element->GetUuid());

    DrawMRptPart(dc, element->GetDrawingX(), SMUFL_E500_repeat1Bar, mRpt->m_drawingMeasureCount, false, staff);

    dc->EndGraphic(element, this);
}

void View::DrawMRpt2(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    MRpt2 *mRpt2 = vrv_cast<MRpt2 *>(element);
    assert(mRpt2);

    mRpt2->CenterDrawingX();

    dc->StartGraphic(element, "", element->GetUuid());

    DrawMRptPart(dc, element->GetDrawingX(), SMUFL_E501_repeat2Bars, 2, true, staff);

    dc->EndGraphic(element, this);
}

void View::DrawMSpace(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    // MSpace *mSpace = vrv_cast<MSpace *>(element);
    // assert(mSpace);

    dc->StartGraphic(element, "", element->GetUuid());
    // nothing to draw here
    dc->EndGraphic(element, this);
}

void View::DrawMultiRest(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    MultiRest *multiRest = vrv_cast<MultiRest *>(element);
    assert(multiRest);

    multiRest->CenterDrawingX();

    int x1, x2, y1, y2;

    dc->StartGraphic(element, "", element->GetUuid());

    const int measureWidth = measure->GetInnerWidth();
    const int xCentered = multiRest->GetDrawingX();

    // We do not support more than three chars
    const int num = std::min(multiRest->GetNum(), 999);

    // Position centered in staff
    y2 = staff->GetDrawingY() - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * staff->m_drawingLines;
    if (multiRest->HasLoc()) {
        y2 -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * (staff->m_drawingLines - 1 - multiRest->GetLoc());
    }

    if (((num > 4) && !multiRest->HasBlock()) || (num > 15) || (multiRest->GetBlock() == BOOLEAN_true)) {
        // This is 1/2 the length of the black rectangle
        int width = measureWidth - 2 * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
        if (multiRest->HasWidth()) {
            const int fixedWidth = multiRest->AttWidth::GetWidth() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
            width = (width > fixedWidth) ? fixedWidth : width;
        }

        // a is the central point, claculate x and x2
        x1 = xCentered - width / 2;
        x2 = xCentered + width / 2;

        y1 = y2 + m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);

        // bounding box is not relevant for the multi-rest rectangle
        dc->DeactivateGraphicX();

        // Draw the base rect
        DrawFilledRectangle(dc, x1, y1, x2, y2);

        // Draw two lines at beginning and end
        int border = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        DrawFilledRectangle(
            dc, x1, y1 + border, x1 + m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) * 2, y2 - border);
        DrawFilledRectangle(
            dc, x2 - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) * 2, y1 + border, x2, y2 - border);

        dc->ReactivateGraphic();
    }
    else {
        // Position centered in staff
        y2 += m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        y1 = y2 + m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);

        const int lgWidth = m_doc->GetGlyphWidth(SMUFL_E4E1_restLonga, staff->m_drawingStaffSize, false);
        const int brWidth = m_doc->GetGlyphWidth(SMUFL_E4E2_restDoubleWhole, staff->m_drawingStaffSize, false);
        const int sbWidth = m_doc->GetGlyphWidth(SMUFL_E4E3_restWhole, staff->m_drawingStaffSize, false);

        int width = (num / 4) * (lgWidth + m_doc->GetDrawingUnit(staff->m_drawingStaffSize));
        width += ((num % 4) / 2) * (brWidth + m_doc->GetDrawingUnit(staff->m_drawingStaffSize));
        width = (num % 2) ? width + sbWidth : width - m_doc->GetDrawingUnit(staff->m_drawingStaffSize);

        x1 = xCentered - width / 2;

        int count = num;
        while ((count / 4)) {
            DrawSmuflCode(dc, x1, y2, SMUFL_E4E1_restLonga, staff->m_drawingStaffSize, false);
            x1 += lgWidth + m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
            count -= 4;
        }
        while ((count / 2)) {
            DrawSmuflCode(dc, x1, y2, SMUFL_E4E2_restDoubleWhole, staff->m_drawingStaffSize, false);
            x1 += brWidth + m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
            count -= 2;
        }
        if (count) DrawSmuflCode(dc, x1, y1, SMUFL_E4E3_restWhole, staff->m_drawingStaffSize, false);
    }

    // Draw the text above
    dc->SetFont(m_doc->GetDrawingSmuflFont(staff->m_drawingStaffSize, false));
    int y = (staff->GetDrawingY() > y1) ? staff->GetDrawingY() + 3 * m_doc->GetDrawingUnit(staff->m_drawingStaffSize)
                                        : y1 + 3 * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    DrawSmuflString(dc, xCentered, y, IntToTimeSigFigures(num), HORIZONTALALIGNMENT_center);
    dc->ResetFont();

    dc->EndGraphic(element, this);
}

void View::DrawMultiRpt(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    MultiRpt *multiRpt = vrv_cast<MultiRpt *>(element);
    assert(multiRpt);

    multiRpt->CenterDrawingX();

    dc->StartGraphic(element, "", element->GetUuid());

    DrawMRptPart(dc, element->GetDrawingX(), SMUFL_E501_repeat2Bars, multiRpt->GetNum(), true, staff);

    dc->EndGraphic(element, this);

    return;
}

void View::DrawNote(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Note *note = vrv_cast<Note *>(element);
    assert(note);

    if (note->IsMensuralDur()) {
        DrawMensuralNote(dc, element, layer, staff, measure);
        return;
    }

    if (note->m_crossStaff) staff = note->m_crossStaff;

    bool drawingCueSize = note->GetDrawingCueSize();
    int noteY = element->GetDrawingY();
    int noteX = element->GetDrawingX();

    if (!(note->GetHeadVisible() == BOOLEAN_false)) {
        /************** Noteheads: **************/
        int drawingDur = note->GetDrawingDur();
        drawingDur = ((note->GetColored() == BOOLEAN_true) && drawingDur > DUR_1) ? (drawingDur + 1) : drawingDur;

        if (drawingDur < DUR_1) {
            DrawMaximaToBrevis(dc, noteY, element, layer, staff);
        }
        else {
            // Whole notes
            wchar_t fontNo;
            if (note->GetColored() == BOOLEAN_true) {
                fontNo = (drawingDur == DUR_1) ? SMUFL_E0FA_noteheadWholeFilled : SMUFL_E0A3_noteheadHalf;
            }
            else {
                fontNo = note->GetNoteheadGlyph(drawingDur);
            }

            dc->StartCustomGraphic("notehead");

            DrawSmuflCode(dc, noteX, noteY, fontNo, staff->m_drawingStaffSize, drawingCueSize, true);

            // handle notehead enclosure
            if (note->HasHeadMod()) {
                switch (note->GetHeadMod()) {
                    case NOTEHEADMODIFIER_paren: {
                        DrawSmuflCode(dc, noteX - note->GetDrawingRadius(m_doc), noteY, SMUFL_E26A_accidentalParensLeft,
                            staff->m_drawingStaffSize, drawingCueSize, true);
                        DrawSmuflCode(dc, noteX + note->GetDrawingRadius(m_doc) * 2, noteY,
                            SMUFL_E26B_accidentalParensRight, staff->m_drawingStaffSize, drawingCueSize, true);
                        break;
                    }
                    case NOTEHEADMODIFIER_slash:
                    case NOTEHEADMODIFIER_backslash:
                    case NOTEHEADMODIFIER_vline:
                    case NOTEHEADMODIFIER_hline: {
                        // TODO: Handle other headmodifiers whenever they become available
                        // wchar_t glyphCode = note->GetNoteheadModifierGlyph();
                        // int offset = (m_doc->GetGlyphWidth - note->GetDrawingRadius(m_doc) * 2) / 2;
                        // DrawSmuflCode(dc, noteX - offset, noteY, glyphCode, staff->m_drawingStaffSize,
                        // drawingCueSize, true);
                        break;
                    }
                    default: break;
                }
            }

            dc->EndCustomGraphic();
        }
    }

    /************ Draw children (accidentals, etc) ************/

    DrawLayerChildren(dc, note, layer, staff, measure);
}

void View::DrawRest(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Rest *rest = vrv_cast<Rest *>(element);
    assert(rest);

    if (rest->IsMensuralDur()) {
        DrawMensuralRest(dc, element, layer, staff, measure);
        return;
    }

    if (rest->m_crossStaff) staff = rest->m_crossStaff;

    const bool drawingCueSize = rest->GetDrawingCueSize();
    const int drawingDur = rest->GetActualDur();
    const wchar_t drawingGlyph = rest->GetRestGlyph();

    int x = element->GetDrawingX();
    int y = element->GetDrawingY();

    DrawSmuflCode(dc, x, y, drawingGlyph, staff->m_drawingStaffSize, drawingCueSize);

    // single legder line for half and whole rests
    if ((drawingDur == DUR_1 || drawingDur == DUR_2)
        && (y > (int)staff->GetDrawingY()
            || y < staff->GetDrawingY()
                    - (staff->m_drawingLines - 1) * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize))) {
        const int width = m_doc->GetGlyphWidth(drawingGlyph, staff->m_drawingStaffSize, drawingCueSize);
        int ledgerLineThickness
            = m_doc->GetOptions()->m_ledgerLineThickness.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        int ledgerLineExtension
            = m_doc->GetOptions()->m_ledgerLineExtension.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        if (drawingCueSize) {
            ledgerLineThickness *= m_doc->GetOptions()->m_graceFactor.GetValue();
            ledgerLineExtension *= m_doc->GetOptions()->m_graceFactor.GetValue();
        }

        DrawHorizontalLine(dc, x - ledgerLineExtension, x + width + ledgerLineExtension, y, ledgerLineThickness);
    }

    /************ Draw children (dots) ************/

    DrawLayerChildren(dc, rest, layer, staff, measure);
}

void View::DrawSpace(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    dc->StartGraphic(element, "", element->GetUuid());
    dc->DrawPlaceholder(ToDeviceContextX(element->GetDrawingX()), ToDeviceContextY(element->GetDrawingY()));
    dc->EndGraphic(element, this);
}

void View::DrawStem(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Stem *stem = vrv_cast<Stem *>(element);
    assert(stem);

    // Do not draw virtual (e.g., whole note) stems
    if (stem->IsVirtual()) return;

    dc->StartGraphic(element, "", element->GetUuid());

    DrawFilledRectangle(dc, stem->GetDrawingX() - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2,
        stem->GetDrawingY(), stem->GetDrawingX() + m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2,
        stem->GetDrawingY() - stem->GetDrawingStemLen());

    if (stem->HasStemMod()) {
        if (stem->GetStemMod() == STEMMODIFIER_sprech) {
            int yShift = m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * 2;
            Note *note = vrv_cast<Note *>(stem->GetParent());
            assert(note);
            if ((note->GetDrawingLoc() % 2) != 0) {
                yShift += m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
            }
            yShift *= (stem->GetDrawingStemLen() > 0) ? -1 : 1;
            yShift -= m_doc->GetGlyphHeight(SMUFL_E645_vocalSprechgesang, staff->m_drawingStaffSize, false) / 2;
            DrawSmuflCode(dc, stem->GetDrawingX(), note->GetDrawingY() + yShift, SMUFL_E645_vocalSprechgesang,
                staff->m_drawingStaffSize, false);
        }
    }

    DrawLayerChildren(dc, stem, layer, staff, measure);

    /************ Draw slash ************/

    if ((stem->GetGrace() == GRACE_unacc) && !stem->IsInBeam()) {
        DrawAcciaccaturaSlash(dc, stem, staff);
    }

    dc->EndGraphic(element, this);
}

void View::DrawSyl(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    Syl *syl = vrv_cast<Syl *>(element);
    assert(syl);

    bool isNeume = (staff->m_drawingNotationType == NOTATIONTYPE_neume);

    if (!syl->GetStart() && !isNeume) {
        LogWarning("Parent note for <syl> was not found");
        return;
    }

    syl->SetDrawingYRel(GetSylYRel(syl->m_drawingVerse, staff));

    dc->StartGraphic(syl, "", syl->GetUuid());
    dc->DeactivateGraphicY();

    dc->SetBrush(m_currentColour, AxSOLID);

    FontInfo currentFont = *m_doc->GetDrawingLyricFont(staff->m_drawingStaffSize);
    if (syl->HasFontweight()) {
        currentFont.SetWeight(syl->GetFontweight());
    }
    if (syl->HasFontstyle()) {
        currentFont.SetStyle(syl->GetFontstyle());
    }
    dc->SetFont(&currentFont);

    TextDrawingParams params;
    params.m_x = syl->GetDrawingX();
    params.m_y = syl->GetDrawingY();
    if (m_doc->GetType() == Facs) {
        params.m_width = syl->GetDrawingWidth();
        params.m_height = syl->GetDrawingHeight();
    }
    assert(dc->GetFont());
    params.m_pointSize = dc->GetFont()->GetPointSize();

    dc->StartText(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y));
    DrawTextChildren(dc, syl, params);

    if (syl->GetCon() == sylLog_CON_b) {
        dc->ReactivateGraphic();
        dc->DeactivateGraphic();
        FontInfo vrvTxt;
        vrvTxt.SetFaceName("VerovioText");
        dc->SetFont(&vrvTxt);
        std::wstring str;
        str.push_back(VRV_TEXT_E551);
        dc->DrawText(UTF16to8(str), str);
        dc->ResetFont();
        dc->ReactivateGraphic();
        dc->DeactivateGraphicY();
    }

    dc->EndText();

    dc->ResetFont();
    dc->ResetBrush();

    if (syl->GetStart() && syl->GetEnd()) {
        System *currentSystem = dynamic_cast<System *>(measure->GetFirstAncestor(SYSTEM));
        // Postpone the drawing of the syl to the end of the system; this will call DrawSylConnector
        // that will look if the last note is in the same system (or not) and draw the connectors accordingly
        if (currentSystem) {
            currentSystem->AddToDrawingList(syl);
        }
    }

    dc->ReactivateGraphic();
    dc->EndGraphic(syl, this);
}

void View::DrawVerse(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Verse *verse = vrv_cast<Verse *>(element);
    assert(verse);

    Label *label = dynamic_cast<Label *>(verse->FindDescendantByType(LABEL, 1));
    LabelAbbr *labelAbbr = verse->m_drawingLabelAbbr;

    if (label || labelAbbr) {

        std::wstring labelStr;
        Object *graphic = NULL;

        if (label) {
            graphic = label;
            labelStr = label->GetText(label);
        }
        else {
            graphic = labelAbbr;
            labelStr = labelAbbr->GetText(labelAbbr);
        }

        FontInfo labelTxt;
        if (!dc->UseGlobalStyling()) {
            labelTxt.SetFaceName("Times");
        }
        labelTxt.SetPointSize(m_doc->GetDrawingLyricFont(staff->m_drawingStaffSize)->GetPointSize());

        TextDrawingParams params;
        params.m_x = verse->GetDrawingX() - m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        params.m_y = staff->GetDrawingY() + this->GetSylYRel(std::max(1, verse->GetN()), staff);
        params.m_pointSize = labelTxt.GetPointSize();

        dc->SetBrush(m_currentColour, AxSOLID);
        dc->SetFont(&labelTxt);

        dc->StartGraphic(graphic, "", graphic->GetUuid());

        dc->StartText(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), HORIZONTALALIGNMENT_right);
        DrawTextChildren(dc, graphic, params);
        dc->EndText();

        dc->EndGraphic(graphic, this);

        dc->ResetFont();
        dc->ResetBrush();
    }

    dc->StartGraphic(verse, "", verse->GetUuid());

    DrawLayerChildren(dc, verse, layer, staff, measure);

    dc->EndGraphic(verse, this);
}

//----------------------------------------------------------------------------
// Partial drawing methods
//----------------------------------------------------------------------------

void View::DrawAcciaccaturaSlash(DeviceContext *dc, Stem *stem, Staff *staff)
{
    assert(dc);
    assert(stem);
    assert(staff);

    dc->SetPen(AxNONE, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) * 1.2, AxSOLID);
    dc->SetBrush(AxNONE, AxSOLID);

    int positionShift = m_doc->GetCueSize(m_doc->GetDrawingUnit(staff->m_drawingStaffSize));
    int positionShiftX1 = positionShift;
    int positionShiftY1 = positionShift * -4;
    int positionShiftX2 = positionShift * 2;
    int positionShiftY2 = positionShift * -1;
    Point startPoint(stem->GetDrawingX(), stem->GetDrawingY() - stem->GetDrawingStemLen());

    // HARDCODED
    if (stem->GetDrawingStemDir() == STEMDIRECTION_up) {
        dc->DrawLine(ToDeviceContextX(startPoint.x - positionShiftX1), ToDeviceContextY(startPoint.y + positionShiftY1),
            ToDeviceContextX(startPoint.x + positionShiftX2), ToDeviceContextY(startPoint.y + positionShiftY2));
    }
    else {
        dc->DrawLine(ToDeviceContextX(startPoint.x - positionShiftX1), ToDeviceContextY(startPoint.y - positionShiftY1),
            ToDeviceContextX(startPoint.x + positionShiftX2), ToDeviceContextY(startPoint.y - positionShiftY2));
    }

    dc->ResetPen();
    dc->ResetBrush();
}

void View::DrawDotsPart(DeviceContext *dc, int x, int y, unsigned char dots, Staff *staff)
{
    int i;

    if (IsOnStaffLine(y, staff)) {
        y += m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    }
    for (i = 0; i < dots; ++i) {
        DrawDot(dc, x, y, staff->m_drawingStaffSize);
        // HARDCODED
        x += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 1.5;
    }
}

void View::DrawMeterSigFigures(DeviceContext *dc, int x, int y, int num, int den, Staff *staff)
{
    assert(dc);
    assert(staff);

    std::wstring timeSigCombNumerator = IntToTimeSigFigures(num), timeSigCombDenominator;
    if (den) timeSigCombDenominator = IntToTimeSigFigures(den);

    dc->SetFont(m_doc->GetDrawingSmuflFont(staff->m_drawingStaffSize, false));

    std::wstring widthText = (timeSigCombNumerator.length() > timeSigCombDenominator.length()) ? timeSigCombNumerator
                                                                                               : timeSigCombDenominator;

    TextExtend extend;
    dc->GetSmuflTextExtent(widthText, &extend);
    x += (extend.m_width / 2);

    if (den) {
        DrawSmuflString(dc, x, y + m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize), timeSigCombNumerator,
            HORIZONTALALIGNMENT_center, staff->m_drawingStaffSize);
        DrawSmuflString(dc, x, y - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize), timeSigCombDenominator,
            HORIZONTALALIGNMENT_center, staff->m_drawingStaffSize);
    }
    else
        DrawSmuflString(dc, x, y, timeSigCombNumerator, HORIZONTALALIGNMENT_center, staff->m_drawingStaffSize);

    dc->ResetFont();
}

void View::DrawMRptPart(DeviceContext *dc, int xCentered, wchar_t smuflCode, int num, bool line, Staff *staff)
{
    int xSymbol = xCentered - m_doc->GetGlyphWidth(smuflCode, staff->m_drawingStaffSize, false) / 2;
    int y = staff->GetDrawingY();
    int ySymbol = y - staff->m_drawingLines / 2 * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);

    DrawSmuflCode(dc, xSymbol, ySymbol, smuflCode, staff->m_drawingStaffSize, false);

    if (line) {
        DrawVerticalLine(dc, y, y - m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize), xCentered,
            m_doc->GetDrawingBarLineWidth(staff->m_drawingStaffSize));
    }

    if (num > 0) {
        dc->SetFont(m_doc->GetDrawingSmuflFont(staff->m_drawingStaffSize, false));
        // calculate the width of the figures
        TextExtend extend;
        std::wstring figures = IntToTupletFigures(num);
        dc->GetSmuflTextExtent(figures, &extend);
        int y = (staff->GetDrawingY() > ySymbol)
            ? staff->GetDrawingY() + m_doc->GetDrawingUnit(staff->m_drawingStaffSize)
            : ySymbol + 3 * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        dc->DrawMusicText(figures, ToDeviceContextX(xCentered - extend.m_width / 2), ToDeviceContextY(y));
        dc->ResetFont();
    }
}

//----------------------------------------------------------------------------
// Calculation or preparation methods
///----------------------------------------------------------------------------

int View::GetFYRel(F *f, Staff *staff)
{
    assert(f && staff);

    int y = staff->GetDrawingY();

    StaffAlignment *alignment = staff->GetAlignment();
    // Something must be seriously wrong...
    if (!alignment) return y;

    y -= (alignment->GetStaffHeight() + alignment->GetOverflowBelow());

    FloatingPositioner *positioner = alignment->FindFirstFloatingPositioner(HARM);
    // There is no other harm, we use the bottom line.
    if (!positioner) return y;

    y = positioner->GetDrawingY();

    Object *fb = f->GetFirstAncestor(FB);
    assert(fb);
    int line = fb->GetDescendantIndex(f, FIGURE, UNLIMITED_DEPTH);

    if (line > 0) {
        FontInfo *fFont = m_doc->GetDrawingLyricFont(staff->m_drawingStaffSize);
        int lineHeight = m_doc->GetTextLineHeight(fFont, false);
        y -= (line * lineHeight);
    }

    return y;
}

int View::GetSylYRel(int verseN, Staff *staff)
{
    assert(staff);

    int y = 0;
    StaffAlignment *alignment = staff->GetAlignment();
    if (alignment) {
        FontInfo *lyricFont = m_doc->GetDrawingLyricFont(staff->m_drawingStaffSize);
        int descender = -m_doc->GetTextGlyphDescender(L'q', lyricFont, false);
        int height = m_doc->GetTextGlyphHeight(L'I', lyricFont, false);
        int margin = m_doc->GetBottomMargin(SYL) * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);

        y = -alignment->GetStaffHeight() - alignment->GetOverflowBelow()
            + (alignment->GetVerseCount() - verseN) * (height + descender + margin) + (descender);
    }
    return y;
}

bool View::IsOnStaffLine(int y, Staff *staff)
{
    // int y1 = y - staff->GetDrawingY();
    // int y2 = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    // LogDebug("IsOnStaff %d %d", y1, y2);

    return ((y - staff->GetDrawingY()) % (2 * m_doc->GetDrawingUnit(staff->m_drawingStaffSize)) == 0);
}

int View::GetNearestInterStaffPosition(int y, Staff *staff, data_STAFFREL place)
{
    int yPos = y - staff->GetDrawingY();
    int distance = yPos % m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    if (place == STAFFREL_above) {
        if (distance > 0) distance = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) - distance;
        return y - distance + m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    }
    else {
        if (distance < 0) distance = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) + distance;
        return y - distance - m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    }
}

} // namespace vrv
