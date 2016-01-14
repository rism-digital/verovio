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
#include "layer.h"
#include "mensur.h"
#include "note.h"
#include "proport.h"
#include "smufl.h"
#include "staff.h"
#include "style.h"

namespace vrv {

int View::s_drawingLigX[2], View::s_drawingLigY[2]; // pour garder coord. des ligatures
bool View::s_drawingLigObliqua = false; // marque le 1e passage pour une oblique

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

    int staffSize = staff->m_drawingStaffSize;
    // Mensural noteheads are quite a bit smaller than CMN noteheads; use _pseudoStaffSize_ to force this.
    int pseudoStaffSize = (int)(MNOTEHEAD_SIZE_FACTOR * staff->m_drawingStaffSize);
    int noteY = element->GetDrawingY();
    int xLedger, xNote, xStem;
    int drawingDur;
    int staffY = staff->GetDrawingY();
    wchar_t charCode;
    int ledge;
    int verticalCenter = 0;

    xStem = element->GetDrawingX();
    xLedger = xStem;

    drawingDur = note->GetDrawingDur();

    int radius = m_doc->GetGlyphWidth(SMUFL_E93C_mensuralNoteheadMinimaWhite, pseudoStaffSize, false) / 2;

    if (drawingDur > DUR_1) {
        ledge = m_doc->GetDrawingLedgerLineLength(pseudoStaffSize, false);
    }
    else {
        ledge = m_doc->GetDrawingLedgerLineLength(pseudoStaffSize, false);
        radius += radius / 3;
    }

    /************** Stem/notehead direction: **************/

    verticalCenter = staffY - m_doc->GetDrawingDoubleUnit(staffSize) * 2;
    if (note->HasStemDir()) {
        note->SetDrawingStemDir(note->GetStemDir());
    }
    else if (layer->GetDrawingStemDir() != STEMDIRECTION_NONE) {
        note->SetDrawingStemDir(layer->GetDrawingStemDir());
    }
    else {
        note->SetDrawingStemDir((noteY >= verticalCenter) ? STEMDIRECTION_down : STEMDIRECTION_up);
    }

    xNote = xStem - radius;

    /************** Noteheads: **************/

    // Ligature, maxima,longa, brevis, and semibrevis
    if ((note->GetLig() != noteLogMensural_LIG_NONE) && (drawingDur <= DUR_1)) {
        DrawLigature(dc, noteY, element, layer, staff);
    }
    else if (drawingDur < DUR_1) {
        DrawMaximaToBrevis(dc, noteY, element, layer, staff);
    }
    else if (drawingDur == DUR_1) {
        if (note->GetColored())
            charCode = SMUFL_E938_mensuralNoteheadSemibrevisBlack;
        else
            charCode = SMUFL_E939_mensuralNoteheadSemibrevisVoid;

        DrawSmuflCode(dc, xNote, noteY, charCode, pseudoStaffSize, false);
    }
    // Shorter values
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

        DrawSmuflCode(dc, xNote, noteY, charCode, pseudoStaffSize, false);

        DrawStem(dc, note, staff, true, note->GetDrawingStemDir(), radius, xStem, noteY);
    }

    /************** Ledger lines: **************/

    int staffTop = staffY + m_doc->GetDrawingUnit(staffSize);
    int staffBot = staffY - m_doc->GetDrawingStaffSize(staffSize) - m_doc->GetDrawingUnit(staffSize);

    // if the note is not in the staff
    if (!is_in(noteY, staffTop, staffBot)) {
        int distance, highestNewLine, numLines;
        bool aboveStaff = (noteY > staffTop);

        distance = (aboveStaff ? (noteY - staffY) : staffY - m_doc->GetDrawingStaffSize(staffSize) - noteY);
        highestNewLine
            = ((distance % m_doc->GetDrawingDoubleUnit(staffSize) > 0) ? (distance - m_doc->GetDrawingUnit(staffSize))
                                                                       : distance);
        numLines = highestNewLine / m_doc->GetDrawingDoubleUnit(staffSize);

        DrawLedgerLines(dc, note, staff, aboveStaff, false, 0, numLines);
    }

    /************** dots **************/

    if (note->GetDots()) {
        int mensDrawingUnit = (int)(MNOTEHEAD_SIZE_FACTOR * m_doc->GetDrawingUnit(staffSize));
        int xDot;
        if (note->GetDur() < DUR_2 || (note->GetDur() > DUR_8 && (note->GetDrawingStemDir() == STEMDIRECTION_up)))
            xDot = xStem + mensDrawingUnit * 7 / 2;
        else
            xDot = xStem + mensDrawingUnit * 5 / 2;

        DrawDots(dc, xDot, noteY, note->GetDots(), staff);
    }
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

void View::DrawMensurCircle(DeviceContext *dc, int x, int yy, Staff *staff)
{
    assert(dc);
    assert(staff);

    int y = ToDeviceContextY(yy - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * MSIGN_STAFFLINES_BELOW_TOP);
    int r = ToDeviceContextX(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize));
    r = (int)(MSIGN_CIRCLE_DIAM / 2.0 * r);

    int lineWidth = m_doc->GetDrawingStaffLineWidth(staff->m_drawingStaffSize);
    lineWidth = lineWidth * 1.0;
    dc->SetPen(m_currentColour, lineWidth, AxSOLID);
    dc->SetPen(m_currentColour, m_doc->GetDrawingStaffLineWidth(staff->m_drawingStaffSize), AxSOLID);
    dc->SetBrush(m_currentColour, AxTRANSPARENT);

    dc->DrawCircle(ToDeviceContextX(x), y, r);

    dc->ResetPen();
    dc->ResetBrush();
}

void View::DrawMensurHalfCircle(DeviceContext *dc, int x, int yy, Staff *staff)
{
    assert(dc);
    assert(staff);

    dc->SetPen(m_currentColour, m_doc->GetDrawingStaffLineWidth(staff->m_drawingStaffSize), AxSOLID);
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

    // Mensural noteheads are usually quite a bit smaller than CMN noteheads; use
    //  _pseudoStaffSize_ to force this.
    int pseudoStaffSize = (int)(MNOTEHEAD_SIZE_FACTOR * staff->m_drawingStaffSize);
    int xn, x1, x2, y1, y2, y3, y4;
    // int yy2, y5; // unused
    int verticalCenter, up, height;
    bool mensural_black = (staff->m_drawingNotationType==NOTATIONTYPE_mensural_black);
    bool fillNotehead = (mensural_black || note->GetColored()) &&
                        !(mensural_black && note->GetColored());
    height = m_doc->GetDrawingBeamWidth(pseudoStaffSize, false) / 2;
    xn = element->GetDrawingX();

    // Calculate size of the rectangle
    x1 = xn - m_doc->GetDrawingBrevisWidth(pseudoStaffSize);
    x2 = xn + m_doc->GetDrawingBrevisWidth(pseudoStaffSize);
    if (note->GetActualDur() == DUR_MX) {
        // Maxima is three times the width of brevis
        x1 -= m_doc->GetDrawingBrevisWidth(pseudoStaffSize);
        x2 += m_doc->GetDrawingBrevisWidth(pseudoStaffSize);
    }
    y1 = y + m_doc->GetDrawingUnit(pseudoStaffSize);
    y2 = y - m_doc->GetDrawingUnit(pseudoStaffSize);
    y3 = (int)(y1 + m_doc->GetDrawingUnit(pseudoStaffSize) / 2); // partie d'encadrement qui depasse
    y4 = (int)(y2 - m_doc->GetDrawingUnit(pseudoStaffSize) / 2);

    if (!fillNotehead) {
        //	double base des carrees
        DrawObliquePolygon(dc, x1, y1, x2, y1, -height);
        DrawObliquePolygon(dc, x1, y2, x2, y2, height);
    }
    else {
        DrawFullRectangle(dc, x1, y1, x2, y2);
    }

    DrawVerticalLine(dc, y3, y4, x1, m_doc->GetDrawingStemWidth(pseudoStaffSize)); // corset lateral
    DrawVerticalLine(dc, y3, y4, x2, m_doc->GetDrawingStemWidth(pseudoStaffSize));

    // stem
    if (note->GetActualDur() < DUR_BR) {
        verticalCenter = staff->GetDrawingY() - m_doc->GetDrawingDoubleUnit(pseudoStaffSize) * 2;
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
            y3 = y1 - m_doc->GetDrawingUnit(pseudoStaffSize) * 8;
            y2 = y1;
        }
        else {
            y3 = y1 + m_doc->GetDrawingUnit(pseudoStaffSize) * 6;
            y2 = y1;
        }
        DrawVerticalLine(dc, y2, y3, x2, m_doc->GetDrawingStemWidth(pseudoStaffSize));
    }

    return;
}

void View::DrawLigature(DeviceContext *dc, int y, LayerElement *element, Layer *layer, Staff *staff)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);

    Note *note = dynamic_cast<Note *>(element);
    assert(note);

    int xn, x1, x2, y1, y2, y3, y4;
    // int yy2, y5; // unused
    int verticalCenter, up, epaisseur;

    epaisseur = std::max(2, m_doc->GetDrawingBeamWidth(staff->m_drawingStaffSize, false) / 2);
    xn = element->GetDrawingX();

    /*
     if ((note->m_lig==LIG_MEDIAL) || (note->m_lig==LIG_TERMINAL))
     {
     CalculateLigaturePosX (element, layer, staff);
     }
     else
     */ {
        xn = element->GetDrawingX();
    }

    // calcul des dimensions du rectangle
    x1 = xn - m_doc->GetDrawingBrevisWidth(staff->m_drawingStaffSize);
    x2 = xn + m_doc->GetDrawingBrevisWidth(staff->m_drawingStaffSize);
    y1 = y + m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    y2 = y - m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    y3 = (int)(y1 + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2); // partie d'encadrement qui depasse
    y4 = (int)(y2 - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2);

    // if (!note->m_ligObliqua && (!View::s_drawingLigObliqua))	// notes rectangulaires, y c. en ligature
    {
        if (note->GetColored() != BOOLEAN_true) { //	double base des carrees
            DrawObliquePolygon(dc, x1, y1, x2, y1, -epaisseur);
            DrawObliquePolygon(dc, x1, y2, x2, y2, epaisseur);
        }
        else
            DrawFullRectangle(dc, x1, y1, x2, y2); // dessine val carree pleine // ENZ correction de x2

        DrawVerticalLine(dc, y3, y4, x1, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize)); // corset lateral
        DrawVerticalLine(dc, y3, y4, x2, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize));
    }
    /*
     else			// traitement des obliques
     {
     if (!View::s_drawingLigObliqua)	// 1e passage: ligne flagStemHeighte initiale
     {
     DrawVerticalLine (dc,y3,y4,x1, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize));
     View::s_drawingLigObliqua = true;
     //oblique = OFF;
     //			if (val == DUR_1)	// queue gauche haut si DUR_1
     //				queue_lig = ON;
     }
     else	// 2e passage: lignes obl. et flagStemHeighte finale
     {
     x1 -=  m_doc->m_drawingBrevisWidth[staff->m_drawingStaffSize] * 2;	// avance auto

     y1 = *View::s_drawingLigY - m_doc->GetDrawingUnit(staff->m_drawingStaffSize);	// ligat_y contient y original
     yy2 = y2;
     y5 = y1+ m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize); y2 +=
     m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);	// on monte d'un
     INTERL

     if (note->GetColored()==BOOLEAN_true)
     DrawObliquePolygon (dc,  x1,  y1,  x2,  yy2, m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize));
     else
     {	DrawObliquePolygon (dc,  x1,  y1,  x2,  yy2, 5);
     DrawObliquePolygon (dc,  x1,  y5,  x2,  y2, -5);
     }
     DrawVerticalLine (dc,y3,y4,x2,m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize));	//cloture
     flagStemHeighte

     View::s_drawingLigObliqua = false;
     //			queue_lig = OFF;	//desamorce alg.queue DUR_BR

     }
     }

     if (note->m_lig)	// memoriser positions d'une note a l'autre; relier notes par barres
     {
     *(View::s_drawingLigX+1) = x2; *(View::s_drawingLigY+1) = y;	// relie notes ligaturees par barres
     flagStemHeightes
     //if (in(x1,(*View::s_drawingLigX)-2,(*View::s_drawingLigX)+2) || (this->fligat && this->lat && !Note1::marq_obl))
     // les dernieres conditions pour permettre ligature flagStemHeighte ancienne
     //	DrawVerticalLine (dc, *ligat_y, y1, (this->fligat && this->lat) ? x2: x1, m_doc->m_parameters.m_stemWidth); //
     ax2 - drawing flagStemHeight
     lines missing
     *View::s_drawingLigX = *(View::s_drawingLigX + 1);
     *View::s_drawingLigY = *(View::s_drawingLigY + 1);
     }


     y3 = y2 - m_doc->GetDrawingUnit(staff->m_drawingStaffSize)*6;

     if (note->m_lig)
     {
     if (note->m_dur == DUR_BR) //  && this->queue_lig)	// queue gauche bas: DUR_BR initiale descendante // ax2 - no
     support of queue_lig (see WG
     corrigeLigature)
     {
     DrawVerticalLine (dc, y2, y3, x1, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize));
     }
     else if (note->m_dur == DUR_LG) // && !this->queue_lig) // DUR_LG en ligature, queue droite bas // ax2 - no support
     of queue_lig
     {
     DrawVerticalLine (dc, y2, y3, x2, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize));
     }
     else if (note->m_dur == DUR_1) // && this->queue_lig)	// queue gauche haut // ax2 - no support of queue_lig
     {
     y2 = y1 + m_doc->GetDrawingUnit(staff->m_drawingStaffSize)*6;
     DrawVerticalLine (dc, y1, y2, x1, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize));
     }
     }
     else if (note->m_dur == DUR_LG)		// DUR_LG isolee: queue comme notes normales
     */
    if (note->GetActualDur() == DUR_LG) {
        verticalCenter = staff->GetDrawingY() - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * 2;
        // ENZ
        up = (y < verticalCenter) ? ON : OFF;
        // ENZ
        if (note->GetDrawingStemDir() != STEMDIRECTION_NONE) {
            if (note->GetDrawingStemDir() == STEMDIRECTION_up) {
                up = ON;
            }
            else {
                up = OFF;
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

    int ynum, yden;
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
    // DrawFullRectangle(dc,x1,y1,x2,y2);
    DrawPartFullRectangle(dc, x1, y1, x2, y2, 0);

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
