/////////////////////////////////////////////////////////////////////////////
// Name:        view_mensural.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "devicecontext.h"
#include "doc.h"
#include "elementpart.h"
#include "layer.h"
#include "ligature.h"
#include "mensur.h"
#include "note.h"
#include "options.h"
#include "proport.h"
#include "rest.h"
#include "smufl.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

int View::s_drawingLigX[2], View::s_drawingLigY[2]; // to keep coords. of ligatures
bool View::s_drawingLigObliqua = false; // mark the first pass for an oblique

//----------------------------------------------------------------------------
// View - Mensural
//----------------------------------------------------------------------------

void View::DrawMensuralNote(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Note *note = dynamic_cast<Note *>(element);
    assert(note);

    int yNote = element->GetDrawingY();
    int xNote = element->GetDrawingX();
    int drawingDur = note->GetDrawingDur();
    int radius = note->GetDrawingRadius(m_doc);
    int staffY = staff->GetDrawingY();
    int verticalCenter = 0;

    /************** Stem/notehead direction: **************/

    data_STEMDIRECTION layerStemDir;
    data_STEMDIRECTION stemDir = STEMDIRECTION_NONE;

    verticalCenter = staffY - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * 2;
    if (note->HasStemDir()) {
        stemDir = note->GetStemDir();
    }
    else if ((layerStemDir = layer->GetDrawingStemDir(note)) != STEMDIRECTION_NONE) {
        stemDir = layerStemDir;
    }
    else {
        if (drawingDur < DUR_1)
            stemDir = STEMDIRECTION_down;
        else
            stemDir = (yNote > verticalCenter) ? STEMDIRECTION_down : STEMDIRECTION_up;
    }

    /************** Noteheads: **************/

    // Ligature, maxima,longa, and brevis
    if (note->IsInLigature()) {
        DrawLigatureNote(dc, element, layer, staff);
    }
    else if (drawingDur < DUR_1) {
        DrawMaximaToBrevis(dc, yNote, element, layer, staff);
    }
    // Semibrevis and shorter
    else {
        wchar_t code = note->GetMensuralSmuflNoteHead();
        DrawSmuflCode(dc, xNote, yNote, code, staff->m_drawingStaffSize, false);
        // For semibrevis with stem in black notation, encoded with an explicit stem direction
        if (((drawingDur > DUR_1) || (note->GetStemDir() != STEMDIRECTION_NONE))
            && note->GetStemVisible() != BOOLEAN_false) {
            DrawMensuralStem(dc, note, staff, stemDir, radius, xNote, yNote);
        }
    }

    /************ Draw children (verse / syl) ************/

    DrawLayerChildren(dc, note, layer, staff, measure);
}

void View::DrawMensuralRest(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    wchar_t charCode;

    Rest *rest = dynamic_cast<Rest *>(element);
    assert(rest);

    bool drawingCueSize = rest->GetDrawingCueSize();
    int drawingDur = rest->GetActualDur();
    int x = element->GetDrawingX();
    int y = element->GetDrawingY();

    switch (drawingDur) {
        case DUR_MX: charCode = SMUFL_E9F0_mensuralRestMaxima; break;
        case DUR_LG: charCode = SMUFL_E9F2_mensuralRestLongaImperfecta; break;
        case DUR_BR: charCode = SMUFL_E9F3_mensuralRestBrevis; break;
        case DUR_1: charCode = SMUFL_E9F4_mensuralRestSemibrevis; break;
        case DUR_2: charCode = SMUFL_E9F5_mensuralRestMinima; break;
        case DUR_4: charCode = SMUFL_E9F6_mensuralRestSemiminima; break;
        case DUR_8: charCode = SMUFL_E9F7_mensuralRestFusa; break;
        case DUR_16: charCode = SMUFL_E9F8_mensuralRestSemifusa; break;
        default: charCode = 0; // This should never happen
    }
    DrawSmuflCode(dc, x, y, charCode, staff->m_drawingStaffSize, drawingCueSize);
}

void View::DrawMensur(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Mensur *mensur = dynamic_cast<Mensur *>(element);
    assert(mensur);

    dc->StartGraphic(element, "", element->GetUuid());

    int y = staff->GetDrawingY() - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * (staff->m_drawingLines - 1);
    int x = element->GetDrawingX();
    int perfectRadius = m_doc->GetGlyphWidth(SMUFL_E910_mensuralProlation1, staff->m_drawingStaffSize, false) / 2;
    int code = 0;

    if (mensur->HasLoc()) {
        y = staff->GetDrawingY()
            - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * (2 * staff->m_drawingLines - 2 - mensur->GetLoc());
    }

    if (mensur->HasSign()) {
        if (mensur->GetSign() == MENSURATIONSIGN_O) {
            code = SMUFL_E911_mensuralProlation2;
        }
        else if (mensur->GetSign() == MENSURATIONSIGN_C) {
            if (mensur->GetOrient() == ORIENTATION_reversed) {
                code = SMUFL_E916_mensuralProlation7;
                // additional offset
                // perfectRadius -= 2 * perfectRadius - m_doc->GetGlyphWidth(SMUFL_E916_mensuralProlation7,
                // staff->m_drawingStaffSize, false);
            }
            else {
                code = SMUFL_E915_mensuralProlation6;
            }
        }
    }
    else if (mensur->HasTempus()) {
        if (mensur->GetTempus() == TEMPUS_3) {
            if (mensur->GetProlatio() == PROLATIO_3)
                code = SMUFL_E910_mensuralProlation1;
            else
                code = SMUFL_E911_mensuralProlation2;
        }
        else if (mensur->GetTempus() == TEMPUS_2) {
            if (mensur->GetProlatio() == PROLATIO_3)
                code = SMUFL_E914_mensuralProlation5;
            else
                code = SMUFL_E915_mensuralProlation6;
        }
    }
    DrawSmuflCode(dc, x, y, code, staff->m_drawingStaffSize, false);

    x += perfectRadius;
    // only one slash supported
    if (mensur->HasSlash()) {
        DrawSmuflCode(dc,
            x - m_doc->GetGlyphWidth(SMUFL_E925_mensuralProlationCombiningStroke, staff->m_drawingStaffSize, false) / 2,
            y, SMUFL_E925_mensuralProlationCombiningStroke, staff->m_drawingStaffSize, false);
    }
    if (mensur->GetDot() == BOOLEAN_true) {
        DrawSmuflCode(dc,
            x - m_doc->GetGlyphWidth(SMUFL_E920_mensuralProlationCombiningDot, staff->m_drawingStaffSize, false) / 2, y,
            SMUFL_E920_mensuralProlationCombiningDot, staff->m_drawingStaffSize, false);
    }

    if (mensur->HasNum()) {
        x = element->GetDrawingX();
        if (mensur->HasSign() || mensur->HasTempus()) {
            x += m_doc->GetDrawingUnit(staff->m_drawingStaffSize)
                * 6; // step forward because we have a sign or a meter symbol
        }
        int numbase = mensur->HasNumbase() ? mensur->GetNumbase() : 0;
        DrawProportFigures(dc, x, y, mensur->GetNum(), numbase, staff);
    }

    dc->EndGraphic(element, this);
} // namespace vrv

/* This function draws any flags as well as the stem. */

void View::DrawMensuralStem(
    DeviceContext *dc, Note *note, Staff *staff, data_STEMDIRECTION dir, int radius, int xn, int originY, int heightY)
{
    assert(note);

    int staffSize = staff->m_drawingStaffSize;
    int staffY = staff->GetDrawingY();
    int baseStem, totalFlagStemHeight, flagStemHeight, nbFlags;
    int drawingDur = note->GetDrawingDur();
    // Cue size is currently disabled
    bool drawingCueSize = false;
    int verticalCenter = staffY - m_doc->GetDrawingDoubleUnit(staffSize) * 2;
    bool mensural_black = (staff->m_drawingNotationType == NOTATIONTYPE_mensural_black);

    baseStem = m_doc->GetDrawingUnit(staffSize) * STANDARD_STEMLENGTH;
    flagStemHeight = m_doc->GetDrawingDoubleUnit(staffSize);
    if (drawingCueSize) {
        baseStem = m_doc->GetCueSize(baseStem);
        flagStemHeight = m_doc->GetCueSize(flagStemHeight);
    }

    nbFlags = (mensural_black ? drawingDur - DUR_2 : drawingDur - DUR_4);
    totalFlagStemHeight = flagStemHeight * (nbFlags * 2 - 1) / 2;

    /* SMuFL provides combining stem-and-flag characters with one and two flags, but
        at the moment, I'm using only the one flag ones, partly out of concern for
        possible three-flag notes. */

    /* In black notation, the semiminima gets one flag; in white notation, it gets none.
        In both cases, as in CWMN, each shorter duration gets one additional flag. */

    if (dir == STEMDIRECTION_down) {
        // flip all lengths. Exception: in mensural notation, the stem will never be at
        //   left, so leave radius as is.
        baseStem = -baseStem;
        totalFlagStemHeight = -totalFlagStemHeight;
        heightY = -heightY;
    }

    // If we have flags, add them to the height.
    int y1 = originY;
    int y2 = ((nbFlags > 0) ? (y1 + baseStem + totalFlagStemHeight) : (y1 + baseStem)) + heightY;
    int x2 = xn + radius;

    if ((dir == STEMDIRECTION_up) && (y2 < verticalCenter)) {
        y2 = verticalCenter;
    }
    else if ((dir == STEMDIRECTION_down) && (y2 > verticalCenter)) {
        y2 = verticalCenter;
    }

    // shorten the stem at its connection with the note head
    // this will not work if the pseudo size is changed
    int shortening = 0.9 * m_doc->GetDrawingUnit(staffSize);

    // LogDebug("DrawMensuralStem: drawingDur=%d mensural_black=%d nbFlags=%d", drawingDur, mensural_black, nbFlags);
    int stemY1 = (dir == STEMDIRECTION_up) ? y1 + shortening : y1 - shortening;
    int stemY2 = y2;
    if (nbFlags > 0) {
        // if we have flags, shorten the stem to make sure we have a nice overlap with the flag glyph
        int shortener
            = (drawingCueSize) ? m_doc->GetCueSize(m_doc->GetDrawingUnit(staffSize)) : m_doc->GetDrawingUnit(staffSize);
        stemY2 = (dir == STEMDIRECTION_up) ? y2 - shortener : y2 + shortener;
    }

    int halfStemWidth = m_doc->GetDrawingStemWidth(staffSize) / 2;
    // draw the stems and the flags

    dc->StartCustomGraphic("stem");
    if (dir == STEMDIRECTION_up) {

        if (nbFlags > 0) {
            for (int i = 0; i < nbFlags; ++i) {
                DrawSmuflCode(dc, x2 - halfStemWidth, stemY1 - i * flagStemHeight,
                    SMUFL_E949_mensuralCombStemUpFlagSemiminima, staff->m_drawingStaffSize, drawingCueSize);
            }
        }
        else {
            DrawFilledRectangle(dc, x2 - halfStemWidth, stemY1, x2 + halfStemWidth, stemY2);
        }
    }
    else {
        if (nbFlags > 0) {
            for (int i = 0; i < nbFlags; ++i) {
                DrawSmuflCode(dc, x2 - halfStemWidth, stemY1 + i * flagStemHeight,
                    SMUFL_E94A_mensuralCombStemDownFlagSemiminima, staff->m_drawingStaffSize, drawingCueSize);
            }
        }
        else {
            DrawFilledRectangle(dc, x2 - halfStemWidth, stemY1, x2 + halfStemWidth, stemY2);
        }
    }
    dc->EndCustomGraphic();

    // Store the stem direction ?
    note->SetDrawingStemDir(dir);
}

void View::CalculateLigaturePosX(LayerElement *element, Layer *layer, Staff *staff)
{
    /*
     if (element == NULL)
     {
     return;
     }
     LayerElement *previous = layer->GetPrevious(element);
     if (previous == NULL || !previous->IsNote())
     {
     return;
     }
     Note *previousNote = dynamic_cast<Note*>(previous);
     if (previousNote->m_lig==LIG_TERMINAL)
     {
     return;
     }
     if (previousNote->m_lig && previousNote->m_dur <= DUR_1)
     {
     element->SetDrawingX(previous->GetDrawingX() + m_doc->m_drawingBrevisWidth[staff->m_drawingStaffSize] * 2);
     }
     */
    return;
}

void View::DrawMaximaToBrevis(DeviceContext *dc, int y, LayerElement *element, Layer *layer, Staff *staff)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);

    Note *note = dynamic_cast<Note *>(element);
    assert(note);

    int xLeft, xRight, yTop, yBottom, y3, y4;
    bool mensural_black = (staff->m_drawingNotationType == NOTATIONTYPE_mensural_black);
    bool fillNotehead = (mensural_black || note->GetColored()) && !(mensural_black && note->GetColored());
    int height = m_doc->GetDrawingBeamWidth(staff->m_drawingStaffSize, false) / 2;

    // Calculate size of the rectangle
    xLeft = element->GetDrawingX();
    int width = 2 * note->GetDrawingRadius(m_doc);
    xRight = xLeft + width;
    if (note->GetActualDur() == DUR_MX) {
        // Maxima is twice the width of brevis
        xRight += 2 * width;
    }
    yTop = y + m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    yBottom = y - m_doc->GetDrawingUnit(staff->m_drawingStaffSize);

    y3 = yTop;
    y4 = yBottom;
    if (!mensural_black) {
        y3 += (int)m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2; // partie d'encadrement qui depasse
        y4 -= (int)m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2;
    }

    if (!fillNotehead) {
        //	double the bases of rectangles
        DrawObliquePolygon(dc, xLeft, yTop, xRight, yTop, -height);
        DrawObliquePolygon(dc, xLeft, yBottom, xRight, yBottom, height);
    }
    else {
        DrawFilledRectangle(dc, xLeft, yTop, xRight, yBottom);
    }

    // corset lateral
    DrawFilledRectangle(dc, xLeft - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2, y3,
        xLeft + m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2, y4);
    DrawFilledRectangle(dc, xRight - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2, y3,
        xRight + m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2, y4);

    // stem
    if (note->GetActualDur() < DUR_BR) {
        bool up = false;
        // Mensural notes have no Stem child - rely on the MEI @stem.dir
        if (note->GetStemDir() != STEMDIRECTION_NONE) {
            up = (note->GetStemDir() == STEMDIRECTION_up) ? true : false;
        }
        if (!up) {
            y3 = yTop - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 8;
            yBottom = yTop;
        }
        else {
            y3 = yTop + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 6;
            yBottom = yTop;
        }
        dc->StartCustomGraphic("stem");

        DrawFilledRectangle(dc, xRight - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2, y3,
            xRight + m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2, yBottom);

        dc->EndCustomGraphic();
    }

    return;
}

void View::DrawLigature(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);

    Ligature *ligature = dynamic_cast<Ligature *>(element);
    assert(ligature);

    dc->StartGraphic(ligature, "", ligature->GetUuid());

    // Draw children (notes)
    DrawLayerChildren(dc, ligature, layer, staff, measure);

    dc->EndGraphic(ligature, this);
}

void View::DrawLigatureNote(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);

    Note *note = dynamic_cast<Note *>(element);
    assert(note);

    Ligature *ligature = dynamic_cast<Ligature *>(note->GetFirstAncestor(LIGATURE));
    assert(ligature);

    Note *firstNote = ligature->GetFirstNote();
    assert(firstNote);

    int xLeft, xRight, yTop, yBottom, y3, y4;
    bool mensural_black = (staff->m_drawingNotationType == NOTATIONTYPE_mensural_black);
    bool fillNotehead = (mensural_black || note->GetColored()) && !(mensural_black && note->GetColored());
    int height = m_doc->GetDrawingBeamWidth(staff->m_drawingStaffSize, false) / 2;

    // Calculate size of the rectangle
    xLeft = firstNote->GetDrawingX();
    xLeft += (2 * m_doc->GetDrawingBrevisWidth(staff->m_drawingStaffSize) * ligature->PositionInLigature(note));

    xRight = xLeft + 2 * m_doc->GetDrawingBrevisWidth(staff->m_drawingStaffSize);
    if (note->GetActualDur() == DUR_MX) {
        // Maxima is twice the width of brevis
        xRight += 2 * m_doc->GetDrawingBrevisWidth(staff->m_drawingStaffSize);
    }

    int y = note->GetDrawingY();
    yTop = y + m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    yBottom = y - m_doc->GetDrawingUnit(staff->m_drawingStaffSize);

    y3 = yTop;
    y4 = yBottom;
    if (!mensural_black) {
        y3 += (int)m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2; // partie d'encadrement qui depasse
        y4 -= (int)m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2;
    }

    if (!fillNotehead) {
        //    double the bases of rectangles
        DrawObliquePolygon(dc, xLeft, yTop, xRight, yTop, -height);
        DrawObliquePolygon(dc, xLeft, yBottom, xRight, yBottom, height);
    }
    else {
        DrawFilledRectangle(dc, xLeft, yTop, xRight, yBottom);
    }

    // corset lateral
    DrawFilledRectangle(dc, xLeft - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2, y3,
        xLeft + m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2, y4);
    DrawFilledRectangle(dc, xRight - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2, y3,
        xRight + m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2, y4);

    return;
}

void View::DrawProportFigures(DeviceContext *dc, int x, int y, int num, int numBase, Staff *staff)
{
    assert(dc);
    assert(staff);

    int ynum = 0, yden = 0;
    int textSize = PROPRT_SIZE_FACTOR * staff->m_drawingStaffSize;
    std::wstring wtext;

    if (numBase) {
        ynum = y + m_doc->GetDrawingDoubleUnit(textSize);
        yden = y - m_doc->GetDrawingDoubleUnit(textSize);
    }
    else
        ynum = y;

    if (numBase > 9 || num > 9) {
        x += m_doc->GetDrawingUnit(textSize) * 2;
    }

    dc->SetFont(m_doc->GetDrawingSmuflFont(textSize, false));

    wtext = IntToTimeSigFigures(num);
    DrawSmuflString(dc, x, ynum, wtext, true, textSize); // true = center

    if (numBase) {
        wtext = IntToTimeSigFigures(numBase);
        DrawSmuflString(dc, x, yden, wtext, true, textSize); // true = center
    }

    dc->ResetFont();

    return;
}

void View::DrawProport(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(layer);
    assert(staff);
    assert(dynamic_cast<Proport *>(element)); // Element must be a Proport"

    int x1, x2, y1, y2;

    Proport *proport = dynamic_cast<Proport *>(element);

    dc->StartGraphic(element, "", element->GetUuid());

    int y = staff->GetDrawingY() - (m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 4);
    int x = element->GetDrawingX();

    x1 = x + 120;
    x2 = x1 + 150; // ??TEST: JUST DRAW AN ARBITRARY RECTANGLE
    y1 = y;
    y2 = y + 50 + (50 * proport->GetNum());
    // DrawFilledRectangle(dc,x1,y1,x2,y2);
    DrawPartFilledRectangle(dc, x1, y1, x2, y2, 0);

    if (proport->HasNum()) {
        x = element->GetDrawingX();
        // if (proport->GetSign() || proport->HasTempus())           // ??WHAT SHOULD THIS BE?
        {
            x += m_doc->GetDrawingUnit(staff->m_drawingStaffSize)
                * 5; // step forward because we have a sign or a meter symbol
        }
        int numbase = proport->HasNumbase() ? proport->GetNumbase() : 0;
        DrawProportFigures(dc, x,
            staff->GetDrawingY() - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * (staff->m_drawingLines - 1),
            proport->GetNum(), numbase, staff);
    }

    dc->EndGraphic(element, this);
}

} // namespace vrv
