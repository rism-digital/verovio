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

 // to keep coords. of ligatures
int View::s_drawLig1stX, View::s_drawLig1stYT, View::s_drawLig1stYB;
bool View::s_drawingLigObliqua = false; // to keep track of which part of an oblique

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

    // Mensural noteheads are usually somewhat smaller than CMN noteheads for the same size
    // staff; use _pseudoStaffSize_ to force this for fonts that don't consider that fact.
    int pseudoStaffSize = (int)(TEMP_MNOTEHEAD_SIZE_FACTOR * staff->m_drawingStaffSize);

    int noteY = element->GetDrawingY();
    int xNote, xStem;
    int drawingDur;
    int staffY = staff->GetDrawingY();
    wchar_t charCode;
    int verticalCenter = 0;
    bool mensural_black = (staff->m_drawingNotationType == NOTATIONTYPE_mensural_black);

    xStem = element->GetDrawingX();

    drawingDur = note->GetDrawingDur();

    int radius = m_doc->GetGlyphWidth(SMUFL_E93C_mensuralNoteheadMinimaWhite, pseudoStaffSize, false) / 2;

    if (drawingDur > DUR_1) {
        if (mensural_black) radius *= TEMP_MINIMA_WIDTH_FACTOR;
    }
    else {
        radius += radius / 3;
    }

    /************** Stem/notehead direction: **************/

    data_STEMDIRECTION layerStemDir;
    data_STEMDIRECTION stemDir = STEMDIRECTION_NONE;

    verticalCenter = staffY - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * 2;
    // In mensural notation, stem direction is controlled entirely by duration; it has nothing
    // to do with the note's vertical position on the staff.
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
            stemDir = (noteY > verticalCenter) ? STEMDIRECTION_down : STEMDIRECTION_up;
    }

    xNote = xStem - radius;

    /************** Noteheads: **************/

    // Ligature, maxima,longa, and brevis
    if (note->IsInLigature()) {
        DrawLigatureNote(dc, element, layer, staff);
    }
    else if (drawingDur < DUR_1) {
        DrawMaximaToBrevis(dc, noteY, element, layer, staff);
    }
    // Semibrevis
    else if (drawingDur == DUR_1) {
        if (mensural_black) {
            int sbStaffSize = 0.8 * staff->m_drawingStaffSize;    // FIXME: should be pseudoStaffSize, but that's too small; why??
            DrawDiamond(dc, xNote, noteY, 2 * sbStaffSize, (int)(1.2 * sbStaffSize), !note->GetColored(), 20);
        }
        else {
            // Maybe we can add this to Note::GetMensuralSmuflNoteHead?
            if (note->GetColored())
                charCode = SMUFL_E938_mensuralNoteheadSemibrevisBlack;
            else
                charCode = SMUFL_E939_mensuralNoteheadSemibrevisVoid;

            DrawSmuflCode(dc, xNote, noteY, charCode, pseudoStaffSize, false);
        }
    }
    // Shorter values
    else {
        if (mensural_black) {
            // SMuFL 1.20 doesn't have a codepoint for the "colored" semibrevis and minima head in black
            // mensural notation. But an unfilled (void) narrow diamond is fine, so we draw one.
            int sbStaffSize = 0.8 * staff->m_drawingStaffSize;  // FIXME: should be pseudoStaffSize, but that's too small; why??
            DrawDiamond(dc, xNote, noteY, 2 * sbStaffSize, (int)(TEMP_MINIMA_WIDTH_FACTOR * 2 * sbStaffSize),
                !note->GetColored(), 20);
        }
        else {
            DrawSmuflCode(dc, xNote, noteY, note->GetMensuralSmuflNoteHead(), pseudoStaffSize, false);
        }

        DrawMensuralStem(dc, note, staff, stemDir, radius, xStem, noteY);
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
    bool drawingCueSize = object->GetDrawingCueSize();
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
        white notation can require three or even four flags, so we use only the one flag
        chars.

        In black notation, the semiminima gets one flag; in white notation, it gets none.
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
    interface->SetDrawingStemDir(dir);
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

    // Mensural noteheads are usually quite a bit smaller than CMN noteheads for the same size
    // staff; use _pseudoStaffSize_ to force this for fonts that don't consider that fact.
    int pseudoStaffSize = (int)(TEMP_MNOTEHEAD_SIZE_FACTOR * staff->m_drawingStaffSize);
    int xLeft, xRight, yTop, yBottom, y3, y4;
    // int yy2, y5; // unused
    int verticalCenter, up, height;
    bool mensural_black = (staff->m_drawingNotationType == NOTATIONTYPE_mensural_black);
    bool fillNotehead = (mensural_black || note->GetColored()) && !(mensural_black && note->GetColored());
    height = m_doc->GetDrawingBeamWidth(pseudoStaffSize, false) / 2;

    // Calculate bounding box for notehead and draw it
    xLeft = element->GetDrawingX();
    xRight = xLeft + 2 * m_doc->GetDrawingBrevisWidth(pseudoStaffSize);
    if (note->GetActualDur() == DUR_MX) {
        // Maxima is twice the width of brevis
        xRight += 2* m_doc->GetDrawingBrevisWidth(pseudoStaffSize);
    }
    yTop = y + m_doc->GetDrawingUnit(pseudoStaffSize);
    yBottom = y - m_doc->GetDrawingUnit(pseudoStaffSize);

    y3 = yTop;
    y4 = yBottom;
    if (!mensural_black) {
        y3 += (int)m_doc->GetDrawingUnit(pseudoStaffSize) / 2; // partie d'encadrement qui depasse
        y4 -= (int)m_doc->GetDrawingUnit(pseudoStaffSize) / 2;
    }

    if (!fillNotehead) {
        // Draw the top and bottom lines
        DrawObliquePolygon(dc, xLeft, yTop, xRight, yTop, -height);
        DrawObliquePolygon(dc, xLeft, yBottom, xRight, yBottom, height);
    }
    else {
        DrawFilledRectangle(dc, xLeft, yTop, xRight, yBottom);
    }

    DrawVerticalLine(dc, y3, y4, xLeft, m_doc->GetDrawingStemWidth(pseudoStaffSize)); // corset lateral
    DrawVerticalLine(dc, y3, y4, xRight, m_doc->GetDrawingStemWidth(pseudoStaffSize));

    // Calculate position of stem and draw it
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
            y3 = yTop - m_doc->GetDrawingUnit(pseudoStaffSize) * 8;
            yBottom = yTop;
        }
        else {
            y3 = yTop + m_doc->GetDrawingUnit(pseudoStaffSize) * 6;
            yBottom = yTop;
        }
        DrawVerticalLine(dc, yBottom, y3, xRight, m_doc->GetDrawingStemWidth(pseudoStaffSize));
    }

    return;
}


void View::DrawMaximaOrLongaRest(DeviceContext *dc, int x, int yStaffTop, LayerElement *element, int loc, Staff *staff)
{
    Rest *rest = dynamic_cast<Rest *>(element);

    dc->StartGraphic(element, "", element->GetUuid());
    
    int drawingDur = rest->GetActualDur();
    int nominalStaffSize = m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize);
    int staffSpace = nominalStaffSize/4;
    int yStaffBottom = yStaffTop - ((staff->m_drawingLines-1)*staffSpace);
    int nSpaces;
    int yLineBottom, yLineTop;
    
    // Set default height, in staff spaces (called "double units" elsewhere in Verovio).
    if (staff->m_drawingLines==5)
        nSpaces = 2;
    else
        // ??Surely we can come up with better defaults for other numbers of staff lines!
        nSpaces = staff->m_drawingLines-1;

    if (rest->HasLoc()) loc = rest->GetLoc();
    if (rest->HasSpaces()) nSpaces = rest->GetSpaces();

    yLineBottom = yStaffBottom+(loc*staffSpace/2);
    
    // The staff's m_drawingStaffSize seems always to be set as if it has 5 lines;
    //   correct for the actual number of lines. On one-line staves, draw rest
    //   from 1/2 space above the line to 1/2 space below.
    
    if (staff->m_drawingLines==1) {
        yLineBottom = yStaffTop - (nominalStaffSize/8);
        DrawRestLines(dc, x, yLineBottom, yLineBottom+staffSpace, drawingDur);
    }
    else {
        yLineTop = yLineBottom+(nSpaces*staffSpace);
        DrawRestLines(dc, x, yLineBottom, yLineTop, drawingDur);
    }
    dc->EndGraphic(element, this);
}


void View::DrawRestLines(DeviceContext *dc, int x, int y_top, int y_bottom, int drawingDur)
{
    assert(dc);
    
#ifdef NOTYET
    int restLineWidth = m_doc->GetDrawingLongMensRestLineWidth(100, false);
#else
    int restLineWidth = 25;
#endif
    if (drawingDur==DUR_MX) {
        // Draw the two lines close enough together that it's obvious they're one symbol.
        int x1 = x - (int)(0.80*restLineWidth);
        int x2 = x + (int)(0.80*restLineWidth);
        DrawVerticalLine(dc, y_top, y_bottom, x1, restLineWidth);
        DrawVerticalLine(dc, y_top, y_bottom, x2, restLineWidth);
    }
    else
        DrawVerticalLine(dc, y_top, y_bottom, x, restLineWidth);
}

    
void View::DrawLigature(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);

    Ligature *ligature = dynamic_cast<Ligature *>(element);
    assert(ligature);
    AttLigatureLog *ligatureLog = dynamic_cast<AttLigatureLog *>(element);
    assert(ligatureLog);
LogDebug("DrawLigature %u: form is %s", element, ligatureLog->GetForm()==LIGATUREFORM_recta? "recta" : "obliqua");
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

    Note *note = dynamic_cast<Note *>(element);
    assert(note);
    
    Ligature *ligature = dynamic_cast<Ligature*>(note->GetFirstParent(LIGATURE));
    assert(ligature);
    
    AttLigatureLog *ligatureLog = dynamic_cast<AttLigatureLog *>(note->GetParent());
    assert(ligatureLog);
//LogDebug("DrawLigatureNote %u: form is %s", element, ligatureLog->GetForm()==LIGATUREFORM_recta? "recta" : "obliqua");

    Note *firstNote = ligature->GetFirstNote();
    assert(firstNote);
    
    int xLeft, xRight, yTop, yBottom, yTopEdge, yBotEdge;
    int pseudoStaffSize = (int)(TEMP_MNOTEHEAD_SIZE_FACTOR * staff->m_drawingStaffSize);
    bool mensural_black = (staff->m_drawingNotationType == NOTATIONTYPE_mensural_black);
    bool fillNotehead = (mensural_black || note->GetColored()) && !(mensural_black && note->GetColored());
    int height = m_doc->GetDrawingBeamWidth(pseudoStaffSize, false) / 2;
    
    xLeft = firstNote->GetDrawingX();
    xLeft += (2 * m_doc->GetDrawingBrevisWidth(pseudoStaffSize) * ligature->PositionInLigature(note));
    
    int y = note->GetDrawingY();
    yTop = y + m_doc->GetDrawingUnit(pseudoStaffSize);
    yBottom = y - m_doc->GetDrawingUnit(pseudoStaffSize);
    
    if (ligatureLog->GetForm()==LIGATUREFORM_recta) {
        xRight = xLeft + 2 * m_doc->GetDrawingBrevisWidth(pseudoStaffSize);

        // Calculate bounding box for notehead and outer edges of frame, and draw it
        if (note->GetActualDur() == DUR_MX) {
            // Maxima is twice the width of brevis
            xRight += 2* m_doc->GetDrawingBrevisWidth(pseudoStaffSize);
        }
        
        yTopEdge = yTop;
        yBotEdge = yBottom;
        if (!mensural_black) {
            yTopEdge += (int)m_doc->GetDrawingUnit(pseudoStaffSize) / 2; // partie d'encadrement qui depasse
            yBotEdge -= (int)m_doc->GetDrawingUnit(pseudoStaffSize) / 2;
        }
        
        if (!fillNotehead) {
            //    double the bases of rectangles
            DrawObliquePolygon(dc, xLeft, yTop, xRight, yTop, -height);
            DrawObliquePolygon(dc, xLeft, yBottom, xRight, yBottom, height);
        }
        else {
            DrawFilledRectangle(dc, xLeft, yTop, xRight, yBottom);
        }
        
        DrawVerticalLine(dc, yTopEdge, yBotEdge, xLeft, m_doc->GetDrawingStemWidth(pseudoStaffSize)); // corset lateral
        DrawVerticalLine(dc, yTopEdge, yBotEdge, xRight, m_doc->GetDrawingStemWidth(pseudoStaffSize));

        int notePos = ligature->PositionInLigature(note);
        //if (notePos>0) ??;
        //LogDebug("DrawLigatureNote: note #%d y=%d, prev note y=%d", note->GetDrawingY(), ??);
        
    }
    else {
        // We're drawing an obliqua. If the first note, just save its coordinates; if the 2nd, really draw it.
        if (!View::s_drawingLigObliqua)	{
            // First note of oblique ligature: just save its coordinates
            s_drawLig1stX = xLeft;
            s_drawLig1stYT = yTop;
            s_drawLig1stYB = yBottom;
            LogDebug("DrawLigatureNote 1st: xLeft=%d yTop=%d yBottom=%d", xLeft, yTop, yBottom);
            View::s_drawingLigObliqua = true;
        }
        else {
            // Second note of oblique ligature: draw parallelogram with width proportional to range
            int thickness = yTop-yBottom;
            int height = s_drawLig1stYT-yTop;
            int widthFactor = (2 * height) / m_doc->GetDrawingBrevisWidth(pseudoStaffSize);
            xRight = xLeft + widthFactor * m_doc->GetDrawingBrevisWidth(pseudoStaffSize);
            LogDebug("DrawLigatureNote 2nd: parallelogram from %d,%d to %d,%d height=%d", s_drawLig1stX, s_drawLig1stYT, xRight, yBottom, height);
            DrawVSidedParallogram(dc, xRight, yBottom, s_drawLig1stX, s_drawLig1stYT, thickness);
            View::s_drawingLigObliqua = false;
        }
        
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
