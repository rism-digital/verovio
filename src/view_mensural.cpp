/////////////////////////////////////////////////////////////////////////////
// Name:        view_mensural.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"
#include "vrv.h"

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
#include "proport.h"
#include "rest.h"
#include "smufl.h"
#include "staff.h"
#include "style.h"

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

    int noteY = element->GetDrawingY();
    int xLedger, xNote, xStem;
    int drawingDur;
    int staffY = staff->GetDrawingY();
    wchar_t charCode;
    int verticalCenter = 0;
    bool mensural_black = (staff->m_drawingNotationType == NOTATIONTYPE_mensural_black);

    xStem = element->GetDrawingX();
    xLedger = xStem;

    drawingDur = note->GetDrawingDur();

    int radius = m_doc->GetGlyphWidth(SMUFL_E93C_mensuralNoteheadMinimaWhite, staff->m_drawingStaffSize, false) / 2;

    if (drawingDur > DUR_1) {
        // ledge = m_doc->GetDrawingLedgerLineLength(pseudoStaffSize, false);
        if (mensural_black) radius *= TEMP_MINIMA_WIDTH_FACTOR;
    }
    else {
        // ledge = m_doc->GetDrawingLedgerLineLength(pseudoStaffSize, false);
        radius += radius / 3;
    }

    /************** Stem/notehead direction: **************/

    Stem *stem = note->GetDrawingStem();

    verticalCenter = staffY - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * 2;
    if (stem && stem->HasStemDir()) {
        note->SetDrawingStemDir(stem->GetStemDir());
    }
    else if (layer->GetDrawingStemDir() != STEMDIRECTION_NONE) {
        note->SetDrawingStemDir(layer->GetDrawingStemDir());
    }
    else {
        if (drawingDur < DUR_1)
            note->SetDrawingStemDir(STEMDIRECTION_down);
        else if (drawingDur == DUR_2)
            note->SetDrawingStemDir(STEMDIRECTION_up);
        else
            note->SetDrawingStemDir((noteY >= verticalCenter) ? STEMDIRECTION_down : STEMDIRECTION_up);
    }

    xNote = xStem - radius;

    /************** Noteheads: **************/

    // Ligature, maxima,longa, and brevis
    if ((note->GetLig() != noteLogMensural_LIG_NONE) && (drawingDur <= DUR_1)) {
        DrawLigatureNote(dc, element, layer, staff);
    }
    else if (drawingDur < DUR_1) {
        DrawMaximaToBrevis(dc, noteY, element, layer, staff);
    }
    // Semibrevis
    else if (drawingDur == DUR_1) {
        if (mensural_black) {
            int sbStaffSize
                = 0.8 * staff->m_drawingStaffSize; // FIXME: should be pseudoStaffSize, but that's too small; why??
            // LogDebug("<DrawDiamond SB: pseudoStaffSize=%d m_drawing=%d sb=%d 2*sb=%d (int)(1.2*sb)=%d",
            DrawDiamond(dc, xNote, noteY, 2 * sbStaffSize, (int)(1.2 * sbStaffSize), !note->GetColored(), 20);
        }
        else {
            if (note->GetColored())
                charCode = SMUFL_E938_mensuralNoteheadSemibrevisBlack;
            else
                charCode = SMUFL_E939_mensuralNoteheadSemibrevisVoid;

            DrawSmuflCode(dc, xNote, noteY, charCode, staff->m_drawingStaffSize, false);
        }
    }
    // Shorter values
    else {
        if (mensural_black) {
            // SMuFL 1.20 doesn't have a codepoint for the "colored" semibrevis and minima head in black
            // mensural notation. But an unfilled (void) narrow diamond is fine, so we draw one.
            int sbStaffSize
                = 0.8 * staff->m_drawingStaffSize; // FIXME: should be pseudoStaffSize, but that's too small; why??
            DrawDiamond(dc, xNote, noteY, 2 * sbStaffSize, (int)(TEMP_MINIMA_WIDTH_FACTOR * 2 * sbStaffSize),
                !note->GetColored(), 20);
        }
        else {
            if (note->GetColored()) {
                if (drawingDur == DUR_2)
                    charCode = SMUFL_E93D_mensuralNoteheadSemiminimaWhite;
                else
                    charCode = SMUFL_E93C_mensuralNoteheadMinimaWhite;
            }
            else {
                if (drawingDur == DUR_2)
                    charCode = SMUFL_E93C_mensuralNoteheadMinimaWhite;
                else
                    charCode = SMUFL_E93D_mensuralNoteheadSemiminimaWhite;
            }
            DrawSmuflCode(dc, xNote, noteY, charCode, staff->m_drawingStaffSize, false);
        }

        DrawMensuralStem(dc, note, staff, note->GetDrawingStemDir(), radius, xStem, noteY);
    }

    /************** Augmentation dots **************/

    if (note->GetDots()) {
        int mensDrawingUnit = (int)(TEMP_MNOTEHEAD_SIZE_FACTOR * m_doc->GetDrawingUnit(staff->m_drawingStaffSize));
        int xDot;
        if (note->GetDur() < DUR_2 || (note->GetDur() > DUR_8 && (note->GetDrawingStemDir() == STEMDIRECTION_up)))
            xDot = xStem + mensDrawingUnit * 7 / 2;
        else
            xDot = xStem + mensDrawingUnit * 5 / 2;

        DrawDotsPart(dc, xDot, noteY, note->GetDots(), staff);
    }

    /************** accidental **************/

    Accid *accid = note->GetDrawingAccid();
    if (accid) {
        int xAccid = 0;
        if (accid->GetFunc() != accidLog_FUNC_edit) {
            xAccid -= 1.5 * m_doc->GetGlyphWidth(SMUFL_E262_accidentalSharp, staff->m_drawingStaffSize, false);
        }

        accid->SetDrawingXRel(xAccid);
        accid->SetDrawingYRel(note->GetDrawingYRel());
    }

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

    bool drawingCueSize = rest->IsCueSize();
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
        default:
            charCode = 0; // This should never happen
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

    int x;

    if ((mensur->GetSign() == MENSURATIONSIGN_O) || (mensur->GetTempus() == TEMPUS_3)) {
        DrawMensurCircle(dc, element->GetDrawingX(), staff->GetDrawingY(), staff);
    }
    else if (((mensur->GetSign() == MENSURATIONSIGN_C) && (mensur->GetOrient() != ORIENTATION_reversed))
        || (mensur->GetTempus() == TEMPUS_2)) {
        DrawMensurHalfCircle(dc, element->GetDrawingX(), staff->GetDrawingY(), staff);
    }
    else if (mensur->GetSign() == MENSURATIONSIGN_C && mensur->GetOrient() == ORIENTATION_reversed) {
        DrawMensurReversedHalfCircle(dc, element->GetDrawingX(), staff->GetDrawingY(), staff);
    }
    // we handle only one single slash
    if (mensur->HasSlash()) {
        DrawMensurSlash(dc, element->GetDrawingX(), staff->GetDrawingY(), staff);
    }
    // we handle only one single dot
    if (mensur->HasDot() || (mensur->GetProlatio() == PROLATIO_3)) {
        DrawMensurDot(dc, element->GetDrawingX(), staff->GetDrawingY(), staff);
    }

    if (mensur->HasNum()) {
        x = element->GetDrawingX();
        if (mensur->GetSign() || mensur->HasTempus()) {
            x += m_doc->GetDrawingUnit(staff->m_drawingStaffSize)
                * 5; // step forward because we have a sign or a meter symbol
        }
        int numbase = mensur->HasNumbase() ? mensur->GetNumbase() : 0;
        DrawProportFigures(dc, x, staff->GetDrawingY(), mensur->GetNum(), numbase, staff);
    }

    dc->EndGraphic(element, this);
}

/* This function draws any flags as well as the stem. */

void View::DrawMensuralStem(DeviceContext *dc, LayerElement *object, Staff *staff, data_STEMDIRECTION dir, int radius,
    int xn, int originY, int heightY)
{
    assert(object->GetDurationInterface());

    int staffSize = staff->m_drawingStaffSize;
    int staffY = staff->GetDrawingY();
    int baseStem, totalFlagStemHeight, flagStemHeight, nbFlags;
    int drawingDur = (object->GetDurationInterface())->GetActualDur();
    bool drawingCueSize = object->IsCueSize();
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
    int x2;
    if (drawingDur < DUR_BR)
        x2 = xn + radius;
    else
        x2 = xn;

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
    if (dir == STEMDIRECTION_up) {

        if (nbFlags > 0) {
            for (int i = 0; i < nbFlags; i++)
                DrawSmuflCode(dc, x2 - halfStemWidth, stemY1 - i * flagStemHeight,
                    SMUFL_E949_mensuralCombStemUpFlagSemiminima, staff->m_drawingStaffSize, drawingCueSize);
        }
        else
            DrawFilledRectangle(dc, x2 - halfStemWidth, stemY1, x2 + halfStemWidth, stemY2);
    }
    else {
        if (nbFlags > 0) {
            for (int i = 0; i < nbFlags; i++)
                DrawSmuflCode(dc, x2 - halfStemWidth, stemY1 + i * flagStemHeight,
                    SMUFL_E94A_mensuralCombStemDownFlagSemiminima, staff->m_drawingStaffSize, drawingCueSize);
        }
        else
            DrawFilledRectangle(dc, x2 - halfStemWidth, stemY1, x2 + halfStemWidth, stemY2);
    }

    // Store the start and end values
    StemmedDrawingInterface *interface = object->GetStemmedDrawingInterface();
    assert(interface);
    // assert(false);
    // interface->SetDrawingStemStart(Point(x2 - (m_doc->GetDrawingStemWidth(staffSize) / 2), y1));
    // interface->SetDrawingStemEnd(Point(x2 - (m_doc->GetDrawingStemWidth(staffSize) / 2), y2));
    interface->SetDrawingStemDir(dir);

    // cast to note is check when setting drawingCueSize value
    if (drawingCueSize && ((dynamic_cast<Note *>(object))->GetGrace() == GRACE_unacc)) {
        DrawAcciaccaturaSlash(dc, object);
    }
}

void View::DrawMensurCircle(DeviceContext *dc, int x, int yy, Staff *staff)
{
    assert(dc);
    assert(staff);

    int y = ToDeviceContextY(yy - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * MSIGN_STAFFLINES_BELOW_TOP);
    int r = ToDeviceContextX(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize));
    r = (int)(MSIGN_CIRCLE_DIAM / 2.0 * r);

    int lineWidth = m_doc->GetDrawingStaffLineWidth(staff->m_drawingStaffSize);
    dc->SetPen(m_currentColour, lineWidth, AxSOLID);
    dc->SetBrush(m_currentColour, AxTRANSPARENT);

    dc->DrawCircle(ToDeviceContextX(x), y, r);

    dc->ResetPen();
    dc->ResetBrush();
}

void View::DrawMensurHalfCircle(DeviceContext *dc, int x, int yy, Staff *staff)
{
    assert(dc);
    assert(staff);

    int lineWidth = m_doc->GetDrawingStaffLineWidth(staff->m_drawingStaffSize);
    dc->SetPen(m_currentColour, lineWidth, AxSOLID);
    dc->SetBrush(m_currentColour, AxTRANSPARENT);

    /* DrawEllipticArc expects x and y to specify the coordinates of the upper-left corner of the
     rectangle that contains the ellipse; y is not the center of the circle it's an arc of. */
    double halfDistBelowTop = MSIGN_STAFFLINES_BELOW_TOP - (MSIGN_CIRCLE_DIAM / 2.0);
    int y = ToDeviceContextY(yy - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * halfDistBelowTop);
    int r = ToDeviceContextX(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize));
    r = (int)(MSIGN_CIRCLE_DIAM / 2.0 * r);

    x = ToDeviceContextX(x);
    x -= 3 * r / 3;

    dc->DrawEllipticArc(x, y, 2 * r, 2 * r, 45, 315);

    dc->ResetPen();
    dc->ResetBrush();

    return;
}

void View::DrawMensurReversedHalfCircle(DeviceContext *dc, int x, int yy, Staff *staff)
{
    assert(dc);
    assert(staff);

    dc->SetPen(m_currentColour, m_doc->GetDrawingStaffLineWidth(staff->m_drawingStaffSize), AxSOLID);
    dc->SetBrush(m_currentColour, AxTRANSPARENT);

    int y = ToDeviceContextY(yy - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize));
    int r = ToDeviceContextX(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize));

    // needs to be fixed
    x = ToDeviceContextX(x);
    x -= 4 * r / 3;

    dc->DrawEllipticArc(x, y, 2 * r, 2 * r, 225, 135);

    dc->ResetPen();
    dc->ResetBrush();

    return;
}

void View::DrawMensurDot(DeviceContext *dc, int x, int yy, Staff *staff)
{
    assert(dc);
    assert(staff);

    int y = ToDeviceContextY(yy - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * MSIGN_STAFFLINES_BELOW_TOP);
    int r = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * MSIGN_DOT_DIAM;

    dc->SetPen(m_currentColour, 1, AxSOLID);
    dc->SetBrush(m_currentColour, AxSOLID);

    dc->DrawCircle(ToDeviceContextX(x), y, r);

    dc->ResetPen();
    dc->ResetBrush();

    return;
}

void View::DrawMensurSlash(DeviceContext *dc, int a, int yy, Staff *staff)
{
    assert(dc);
    assert(staff);

    int y1 = yy;
    int y2 = y1 - m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize);

    DrawVerticalLine(dc, y1, y2, a, m_doc->GetDrawingStaffLineWidth(staff->m_drawingStaffSize));
    return;
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

    int xn, xLeft, xRight, yTop, yBottom, y3, y4;
    // int yy2, y5; // unused
    int verticalCenter, up, height;
    bool mensural_black = (staff->m_drawingNotationType == NOTATIONTYPE_mensural_black);
    bool fillNotehead = (mensural_black || note->GetColored()) && !(mensural_black && note->GetColored());
    height = m_doc->GetDrawingBeamWidth(staff->m_drawingStaffSize, false) / 2;
    xn = element->GetDrawingX();

    // Calculate size of the rectangle
    xLeft = xn - m_doc->GetDrawingBrevisWidth(staff->m_drawingStaffSize);
    xRight = xn + m_doc->GetDrawingBrevisWidth(staff->m_drawingStaffSize);
    if (note->GetActualDur() == DUR_MX) {
        // Maxima is twice the width of brevis
        xLeft -= m_doc->GetDrawingBrevisWidth(staff->m_drawingStaffSize);
        xRight += m_doc->GetDrawingBrevisWidth(staff->m_drawingStaffSize);
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

    DrawVerticalLine(dc, y3, y4, xLeft, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize)); // corset lateral
    DrawVerticalLine(dc, y3, y4, xRight, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize));

    // stem
    if (note->GetActualDur() < DUR_BR) {
        verticalCenter = staff->GetDrawingY() - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * 2;
        up = (y < verticalCenter) ? true : false;
        if (note->GetDrawingStemDir() != STEMDIRECTION_NONE) {
            if (note->GetDrawingStemDir() == STEMDIRECTION_up) {
                up = true;
            }
            else {
                up = false;
            }
        }

        if (!up) {
            y3 = yTop - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 8;
            yBottom = yTop;
        }
        else {
            y3 = yTop + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 6;
            yBottom = yTop;
        }
        DrawVerticalLine(dc, yBottom, y3, xRight, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize));
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
    // dc->SetEmptyBB();    ...really shouldn't be needed.
    dc->EndGraphic(ligature, this);
}

void View::DrawLigatureNote(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);

    LogDebug("DrawLigatureNote");
    Note *note = dynamic_cast<Note *>(element);
    assert(note);

    int xn, x1, x2, y, y1, y2, y3, y4;
    // int yy2, y5; // unused
    int verticalCenter, up, epaisseur;

    epaisseur = std::max(2, m_doc->GetDrawingBeamWidth(staff->m_drawingStaffSize, false) / 2);
    xn = element->GetDrawingX();
    y = 99;
    //    LogDebug("DrawLigatureNote: _ligObliqua=%d drawingX=%d y=%d", note->m_ligObliqua, xn, y);
    LogDebug("DrawLigatureNote: drawingX=%d y=%d", xn, y);

    /*
     if ((note->m_lig==LIG_MEDIAL) || (note->m_lig==LIG_TERMINAL))
     {
     CalculateLigaturePosX(element, layer, staff);
     }
     else
     */ {
        xn = element->GetDrawingX();
    }

    // Compute the dimensions of the rectangle
    x1 = xn - m_doc->GetDrawingBrevisWidth(staff->m_drawingStaffSize);
    x2 = xn + m_doc->GetDrawingBrevisWidth(staff->m_drawingStaffSize);
    y1 = y + m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    y2 = y - m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    y3 = (int)(y1 + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2); // part of the frame that overflows
    y4 = (int)(y2 - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2);

    // if (!note->m_ligObliqua && (!View::s_drawingLigObliqua))	// rectangular notes, incl. ligature
    {
        if (note->GetColored() != BOOLEAN_true) { //	double the bases of rectangles
            DrawObliquePolygon(dc, x1, y1, x2, y1, -epaisseur);
            DrawObliquePolygon(dc, x1, y2, x2, y2, epaisseur);
        }
        else
            DrawFilledRectangle(dc, x1, y1, x2, y2); //
        // ENZ correction of x2

        DrawVerticalLine(dc, y3, y4, x1, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize)); // lateral brace
        DrawVerticalLine(dc, y3, y4, x2, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize));
    }
    /*
     else			// handle obliques
     {
     if (!View::s_drawingLigObliqua)	// 1st pass: Initial flagStemHeight
     {
     DrawVerticalLine (dc,y3,y4,x1, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize));
     View::s_drawingLigObliqua = true;
     //oblique = false;
     //			if (val == DUR_1)	// left tail up if DUR_1
     //				queue_lig = true;
     }
     else	// 2nd pass: oblique lines and final flagStemHeighte
     {
     x1 -=  m_doc->m_drawingBrevisWidth[staff->m_drawingStaffSize] * 2;	// auto advance

     y1 = *View::s_drawingLigY - m_doc->GetDrawingUnit(staff->m_drawingStaffSize);	// ligat_y contains original y
     yy2 = y2;
     y5 = y1+ m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize); y2 +=
     m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);            // go up a
     INTERL

     if (note->GetColored()==BOOLEAN_true)
     DrawObliquePolygon (dc,  x1,  y1,  x2,  yy2, m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize));
     else
     {	DrawObliquePolygon (dc,  x1,  y1,  x2,  yy2, 5);
     DrawObliquePolygon (dc,  x1,  y5,  x2,  y2, -5);
     }
     DrawVerticalLine (dc,y3,y4,x2,m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize));	// enclosure
     flagStemHeighte

     View::s_drawingLigObliqua = false;
     //			queue_lig = false;	// ??defuses alg.queue DUR_BR??

     }
     }

     if (note->m_lig)	// remember positions from one note to another; connect notes by bars
     {
     *(View::s_drawingLigX+1) = x2; *(View::s_drawingLigY+1) = y;	// connect ligature beamed notes by bar
     flagStemHeightes
     //if (in(x1,(*View::s_drawingLigX)-2,(*View::s_drawingLigX)+2) || (this->fligat && this->lat && !Note1::marq_obl))
     // the latest conditions to allow previous ligature flagStemHeighte
     //	DrawVerticalLine (dc, *ligat_y, y1, (this->fligat && this->lat) ? x2: x1, m_doc->m_parameters.m_stemWidth); //
     ax2 - drawing flagStemHeight
     lines missing
     *View::s_drawingLigX = *(View::s_drawingLigX + 1);
     *View::s_drawingLigY = *(View::s_drawingLigY + 1);
     }


     y3 = y2 - m_doc->GetDrawingUnit(staff->m_drawingStaffSize)*6;

     if (note->m_lig)
     {
     if (note->m_dur == DUR_BR) //  && this->queue_lig)	// tail left bottom: initial downward DUR_BR // ax2 - no
     support for queue_lig (see WG
     corrigeLigature)
     {
     DrawVerticalLine (dc, y2, y3, x1, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize));
     }
     else if (note->m_dur == DUR_LG) // && !this->queue_lig) // DUR_LG ligature, tail right down // ax2 - no support
     for queue_lig
     {
     DrawVerticalLine (dc, y2, y3, x2, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize));
     }
     else if (note->m_dur == DUR_1) // && this->queue_lig)	// queue gauche haut // ax2 - no support for queue_lig
     {
     y2 = y1 + m_doc->GetDrawingUnit(staff->m_drawingStaffSize)*6;
     DrawVerticalLine (dc, y1, y2, x1, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize));
     }
     }
     else if (note->m_dur == DUR_LG)		// isolated DUR_LG: tail like normal notes
     */
    if (note->GetActualDur() == DUR_LG) {
        verticalCenter = staff->GetDrawingY() - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * 2;
        // ENZ
        up = (y < verticalCenter) ? true : false;
        // ENZ
        if (note->GetDrawingStemDir() != STEMDIRECTION_NONE) {
            if (note->GetDrawingStemDir() == STEMDIRECTION_up) {
                up = true;
            }
            else {
                up = false;
            }
        }

        if (!up) {
            y3 = y1 - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 8;
            y2 = y1;
        }
        else {
            y3 = y1 + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 6;
            y2 = y1;
        }
        DrawVerticalLine(dc, y2, y3, x2, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize));
    }

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
        ynum = y - (m_doc->GetDrawingUnit(textSize) * 2);
        yden = ynum - (m_doc->GetDrawingDoubleUnit(textSize) * 2);
    }
    else
        ynum = y - (m_doc->GetDrawingUnit(textSize) * 4);

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
        DrawProportFigures(dc, x, staff->GetDrawingY(), proport->GetNum(), numbase, staff);
    }

    dc->EndGraphic(element, this);
}

} // namespace vrv
