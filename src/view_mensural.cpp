/////////////////////////////////////////////////////////////////////////////
// Name:        musrc_mensural.cpp
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
#include "smufl.h"
#include "staff.h"
#include "style.h"

namespace vrv {    

int View::s_drawingLigX[2], View::s_drawingLigY[2];	// pour garder coord. des ligatures
bool View::s_drawingLigObliqua = false;	// marque le 1e passage pour une oblique

//----------------------------------------------------------------------------
// View - Mensural
//----------------------------------------------------------------------------

void View::DrawMensuralNote ( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<Note*>(element)); // Element must be a Note"
    
    Note *note = dynamic_cast<Note*>(element);
    
    int staffSize = staff->staffSize;
    int noteY = element->GetDrawingY();
    int xLedger, xNote, xStem;
    int drawingDur;
    int staffY = staff->GetDrawingY();
    wchar_t fontNo;
    int ledge;
    int verticalCenter = 0;

    xStem = element->GetDrawingX();
    xLedger = xStem;
    
    drawingDur = note->GetDrawingDur();
    
    int radius = m_doc->m_drawingNoteRadius[staffSize][note->m_cueSize];
    
    if (drawingDur > DUR_1 || (drawingDur == DUR_1 && staff->notAnc)) {	// annuler provisoirement la modif. des lignes addit.
        ledge = m_doc->m_drawingLedgerLine[staffSize][note->m_cueSize];
    }
    else {
        ledge = m_doc->m_drawingLedgerLine[staffSize][note->m_cueSize];
        radius += radius/3;
    }
    
    /************** Stem/notehead direction: **************/
    
    verticalCenter = staffY - m_doc->m_drawingDoubleUnit[staffSize]*2;
    if ( note->HasDrawingStemDir() ) {
        note->m_drawingStemDir = note->GetDrawingStemDir();
    }
    else if ( layer->GetDrawingStemDir() != STEMDIRECTION_NONE) {
        note->m_drawingStemDir = layer->GetDrawingStemDir();
    }
    else {
        note->m_drawingStemDir = (noteY >= verticalCenter) ? STEMDIRECTION_down : STEMDIRECTION_up;
    }
    
    xNote = xStem - radius;
    
    /************** Noteheads: **************/
    
    // Long, breve and ligatures
    if ((note->GetLig()!=LIGATURE_NONE) && (drawingDur <= DUR_1)) {
        DrawLigature ( dc, noteY, element, layer, staff);
    }
    else if (drawingDur < DUR_1) {
        DrawMaximaToBrevis( dc, noteY, element, layer, staff);
    }
    // Whole notes
    else if (drawingDur == DUR_1) {
        if (note->GetColored()==BOOLEAN_true)
            fontNo = SMUFL_E0FA_noteheadWholeFilled;
        else
            fontNo = SMUFL_E0A2_noteheadWhole;
        
        DrawSmuflCode( dc, xNote, noteY, fontNo, staff->staffSize, note->m_cueSize );
    }
    // Other values
    else {
        if (note->GetColored()==BOOLEAN_true) {
            if (drawingDur == DUR_2) fontNo = SMUFL_E0A4_noteheadBlack;
            else fontNo = SMUFL_E0A3_noteheadHalf;
        }
        else {
            if (drawingDur > DUR_2) fontNo = SMUFL_E0A4_noteheadBlack;
            else fontNo = SMUFL_E0A3_noteheadHalf;
        }

        DrawSmuflCode( dc, xNote, noteY, fontNo,  staff->staffSize, note->m_cueSize );
        
        DrawStem(dc, note, staff, note->m_drawingStemDir, radius, xStem, noteY);
    }
    
    /************** Ledger lines: **************/
    
    int staffTop = staffY + m_doc->m_drawingUnit[staffSize];
    int staffBot = staffY - m_doc->m_drawingStaffSize[staffSize] - m_doc->m_drawingUnit[staffSize];
    
    //if the note is not in the staff
    if (!is_in(noteY,staffTop,staffBot))
    {
        int distance, highestNewLine, numLines;
        bool aboveStaff = (noteY > staffTop);
        
        distance = (aboveStaff ? (noteY - staffY) : staffY - m_doc->m_drawingStaffSize[staffSize] - noteY);
        highestNewLine = ((distance % m_doc->m_drawingDoubleUnit[staffSize] > 0) ? (distance - m_doc->m_drawingUnit[staffSize]) : distance);
        numLines = highestNewLine / m_doc->m_drawingDoubleUnit[staffSize];
        
        DrawLedgerLines(dc, note, staff, aboveStaff, false, 0, numLines);

    }
    
    /************** dots **************/
    
    if (note->GetDots()) {
        int xDot;
        if (note->GetDur() < DUR_2 || (note->GetDur() > DUR_8 && (note->m_drawingStemDir == STEMDIRECTION_up)))
            xDot = xStem + m_doc->m_drawingUnit[staffSize]*7/2;
        else
            xDot = xStem + m_doc->m_drawingUnit[staffSize]*5/2;
        
        DrawDots( dc, xDot, noteY, note->GetDots(), staff );
    }
}


void View::DrawMensur( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<Mensur*>(element)); // Element must be a Mensur"
    
    Mensur *mensur = dynamic_cast<Mensur*>(element);
    
    dc->StartGraphic( element, "", element->GetUuid() );
    
    int x;
    
    if ((mensur->GetSign()==MENSURATIONSIGN_O) || (mensur->GetTempus() == TEMPUS_3))
    {
        DrawMensurCircle ( dc, element->GetDrawingX(), staff->GetDrawingY(), staff);
    }
    else if (((mensur->GetSign()==MENSURATIONSIGN_C) && (mensur->GetOrient()!=ORIENTATION_reversed))
             || (mensur->GetTempus() == TEMPUS_2))
    {
        DrawMensurHalfCircle ( dc, element->GetDrawingX(), staff->GetDrawingY(), staff);
    }
    else if (mensur->GetSign()==MENSURATIONSIGN_C && mensur->GetOrient()==ORIENTATION_reversed)
    {
        DrawMensurReversedHalfCircle ( dc, element->GetDrawingX(), staff->GetDrawingY(), staff);
    }
    if (mensur->HasSlash()) // we handle only one single slash
    {
        DrawMensurSlash ( dc, element->GetDrawingX(), staff->GetDrawingY(), staff);
    }
    if (mensur->HasDot() || (mensur->GetProlatio() == PROLATIO_3)) // we handle only one single dot
    {
        DrawMensurDot (dc, element->GetDrawingX(), staff->GetDrawingY(), staff);
    }
    
    if (mensur->HasNum())
    {
        x = element->GetDrawingX();
        if (mensur->GetSign() || mensur->HasTempus())
        {
            x += m_doc->m_drawingUnit[staff->staffSize] * 5; // step forward because we have a sign or a meter symbol
        }
        int numbase = mensur->HasNumbase() ? mensur->GetNumbase() : 0;
        DrawMeterSigFigures ( dc, x, staff->GetDrawingY(), mensur->GetNum(), numbase, staff);
    }
    
    dc->EndGraphic(element, this ); //RZ
    
}


void View::DrawMensurCircle( DeviceContext *dc, int x, int yy, Staff *staff )
{
    assert( dc ); // DC cannot be NULL
    
    int y =  ToDeviceContextY (yy - m_doc->m_drawingDoubleUnit[ staff->staffSize ] * 2);
    int r = ToDeviceContextX( m_doc->m_drawingDoubleUnit[ staff->staffSize ]);
    
    dc->SetPen( m_currentColour, m_doc->m_style->m_staffLineWidth, AxSOLID );
    dc->SetBrush( m_currentColour, AxTRANSPARENT );
    
    dc->DrawCircle( ToDeviceContextX(x), y, r );
    
    dc->ResetPen();
    dc->ResetBrush();
}

void View::DrawMensurHalfCircle( DeviceContext *dc, int x, int yy, Staff *staff )
{
    assert( dc ); // DC cannot be NULL
    
    dc->SetPen( m_currentColour, m_doc->m_style->m_staffLineWidth, AxSOLID );
    dc->SetBrush( m_currentColour, AxTRANSPARENT );
    
    int y =  ToDeviceContextY (yy - m_doc->m_drawingDoubleUnit[ staff->staffSize ]);
    int r = ToDeviceContextX( m_doc->m_drawingDoubleUnit[ staff->staffSize ]);
    
    x = ToDeviceContextX (x);
    x -= 3*r/3;
    
    dc->DrawEllipticArc( x, y, 2*r, 2*r, 45, 315 );
    
    dc->ResetPen();
    dc->ResetBrush();
    
    return;
}

void View::DrawMensurReversedHalfCircle( DeviceContext *dc, int x, int yy, Staff *staff )
{
    assert( dc ); // DC cannot be NULL
    
    dc->SetPen( m_currentColour, m_doc->m_style->m_staffLineWidth, AxSOLID );
    dc->SetBrush( m_currentColour, AxTRANSPARENT );
    
    int y =  ToDeviceContextY (yy - m_doc->m_drawingDoubleUnit[ staff->staffSize ]);
    int r = ToDeviceContextX( m_doc->m_drawingDoubleUnit[ staff->staffSize ] );
    
    // needs to be fixed
    x = ToDeviceContextX (x);
    x -= 4*r/3;
    
    dc->DrawEllipticArc( x, y, 2*r, 2*r, 225, 135 );
    
    dc->ResetPen();
    dc->ResetBrush();
    
    return;
}

void View::DrawMensurDot ( DeviceContext *dc, int x, int yy, Staff *staff )
{
    assert( dc ); // DC cannot be NULL
    
    int y =  ToDeviceContextY (yy - m_doc->m_drawingDoubleUnit[ staff->staffSize ] * 2);
    int r = m_doc->m_drawingUnit[staff->staffSize] * 2 / 3;
    
    dc->SetPen( m_currentColour, 1, AxSOLID );
    dc->SetBrush( m_currentColour, AxSOLID );
    
    dc->DrawCircle( ToDeviceContextX(x) , y, r );
    
    dc->ResetPen();
    dc->ResetBrush();
    
    return;
}	


void View::DrawMensurSlash ( DeviceContext *dc, int a, int yy, Staff *staff )
{	
    assert( dc ); // DC cannot be NULL
    
    int y1 = yy;
    int y2 = y1 - m_doc->m_drawingStaffSize[ staff->staffSize ];
    
    DrawVerticalLine ( dc, y1, y2, a, m_doc->m_style->m_staffLineWidth);
    return;
}
    
void View::CalculateLigaturePosX ( LayerElement *element, Layer *layer, Staff *staff)
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
     element->SetDrawingX( previous->GetDrawingX() + m_doc->m_drawingBrevisWidth[staff->staffSize] * 2 );
     }
     */
    return;
}

void View::DrawMaximaToBrevis( DeviceContext *dc, int y, LayerElement *element, Layer *layer, Staff *staff )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<Note*>(element)); // Element must be a Note"
    
    
    Note *note = dynamic_cast<Note*>(element);
    
    int xn, x1, x2, y1, y2, y3, y4;
    // int yy2, y5; // unused
    int verticalCenter, up, height;
    
    height = m_doc->m_drawingBeamWidth[staff->staffSize]/2 ;
    xn = element->GetDrawingX();
    
    // calcul des dimensions du rectangle
    x1 = xn - m_doc->m_drawingBrevisWidth[staff->staffSize];
    x2 = xn +  m_doc->m_drawingBrevisWidth[staff->staffSize];
    if (note->GetActualDur() == DUR_MX) {
        x1 -= m_doc->m_drawingBrevisWidth[staff->staffSize];
        x2 += m_doc->m_drawingBrevisWidth[staff->staffSize];
    }
    y1 = y + m_doc->m_drawingUnit[staff->staffSize];
    y2 = y - m_doc->m_drawingUnit[staff->staffSize];
    y3 = (int)(y1 + m_doc->m_drawingUnit[staff->staffSize]/2);	// partie d'encadrement qui depasse
    y4 = (int)(y2 - m_doc->m_drawingUnit[staff->staffSize]/2);
    
    if (note->GetColored()!=BOOLEAN_true) {
        //	double base des carrees
        DrawObliquePolygon ( dc, x1,  y1,  x2,  y1, -height );
        DrawObliquePolygon ( dc, x1,  y2,  x2,  y2, height );
    }
    else {
        DrawFullRectangle( dc,x1,y1,x2,y2);
    }
    
    DrawVerticalLine ( dc, y3, y4, x1, m_doc->m_style->m_stemWidth );	// corset lateral
    DrawVerticalLine ( dc, y3, y4, x2, m_doc->m_style->m_stemWidth );

    // stem
    if (note->GetActualDur() < DUR_BR)
    {
        verticalCenter = staff->GetDrawingY() - m_doc->m_drawingDoubleUnit[staff->staffSize]*2;
        up = (y < verticalCenter) ? true : false;
        if ( note->m_drawingStemDir != STEMDIRECTION_NONE ) {
            if ( note->m_drawingStemDir == STEMDIRECTION_up) {
                up = true;
            }
            else {
                up = false;
            }
        }
        
        if (!up) {
            y3 = y1 - m_doc->m_drawingUnit[staff->staffSize]*8;
            y2 = y1;
        }
        else {
            y3 = y1 + m_doc->m_drawingUnit[staff->staffSize]*6;
            y2 = y1;
        }
        DrawVerticalLine ( dc, y2,y3,x2, m_doc->m_style->m_stemWidth );
    }
    
    return;
}
    
void View::DrawLigature ( DeviceContext *dc, int y, LayerElement *element, Layer *layer, Staff *staff )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<Note*>(element)); // Element must be a Note"
    
    
    Note *note = dynamic_cast<Note*>(element);
    
    int xn, x1, x2, y1, y2, y3, y4;
    // int yy2, y5; // unused
    int verticalCenter, up, epaisseur;
    
    epaisseur = std::max (2, m_doc->m_drawingBeamWidth[staff->staffSize]/2);
    xn = element->GetDrawingX();
    
    /*
     if ((note->m_lig==LIG_MEDIAL) || (note->m_lig==LIG_TERMINAL))
     {
     CalculateLigaturePosX ( element, layer, staff );
     }
     else
     */{
         xn = element->GetDrawingX();
     }
    
    // calcul des dimensions du rectangle
    x1 = xn - m_doc->m_drawingBrevisWidth[staff->staffSize]; x2 = xn +  m_doc->m_drawingBrevisWidth[staff->staffSize];
    y1 = y + m_doc->m_drawingUnit[staff->staffSize];
    y2 = y - m_doc->m_drawingUnit[staff->staffSize];
    y3 = (int)(y1 + m_doc->m_drawingUnit[staff->staffSize]/2);	// partie d'encadrement qui depasse
    y4 = (int)(y2 - m_doc->m_drawingUnit[staff->staffSize]/2);
    
    
    //if (!note->m_ligObliqua && (!View::s_drawingLigObliqua))	// notes rectangulaires, y c. en ligature
    {
        if (note->GetColored()!=BOOLEAN_true)
        {				//	double base des carrees
            DrawObliquePolygon ( dc, x1,  y1,  x2,  y1, -epaisseur );
            DrawObliquePolygon ( dc, x1,  y2,  x2,  y2, epaisseur );
        }
        else
            DrawFullRectangle( dc,x1,y1,x2,y2);	// dessine val carree pleine // ENZ correction de x2
        
        DrawVerticalLine ( dc, y3, y4, x1, m_doc->m_style->m_stemWidth );	// corset lateral
        DrawVerticalLine ( dc, y3, y4, x2, m_doc->m_style->m_stemWidth );
    }
    /*
     else			// traitement des obliques
     {
     if (!View::s_drawingLigObliqua)	// 1e passage: ligne flagStemHeighte initiale
     {
     DrawVerticalLine (dc,y3,y4,x1, m_doc->m_style->m_stemWidth );
     View::s_drawingLigObliqua = true;
     //oblique = OFF;
     //			if (val == DUR_1)	// queue gauche haut si DUR_1
     //				queue_lig = ON;
     }
     else	// 2e passage: lignes obl. et flagStemHeighte finale
     {
     x1 -=  m_doc->m_drawingBrevisWidth[staff->staffSize]*2;	// avance auto
     
     y1 = *View::s_drawingLigY - m_doc->m_drawingUnit[staff->staffSize];	// ligat_y contient y original
     yy2 = y2;
     y5 = y1+ m_doc->m_drawingDoubleUnit[staff->staffSize]; y2 += m_doc->m_drawingDoubleUnit[staff->staffSize];	// on monte d'un INTERL
     
     if (note->GetColored()==BOOLEAN_true)
     DrawObliquePolygon ( dc,  x1,  y1,  x2,  yy2, m_doc->m_drawingDoubleUnit[staff->staffSize]);
     else
     {	DrawObliquePolygon ( dc,  x1,  y1,  x2,  yy2, 5);
     DrawObliquePolygon ( dc,  x1,  y5,  x2,  y2, -5);
     }
     DrawVerticalLine ( dc,y3,y4,x2,m_doc->m_style->m_stemWidth);	//cloture flagStemHeighte
     
     View::s_drawingLigObliqua = false;
     //			queue_lig = OFF;	//desamorce alg.queue DUR_BR
     
     }
     }
     
     if (note->m_lig)	// memoriser positions d'une note a l'autre; relier notes par barres
     {
     *(View::s_drawingLigX+1) = x2; *(View::s_drawingLigY+1) = y;	// relie notes ligaturees par barres flagStemHeightes
     //if (in(x1,(*View::s_drawingLigX)-2,(*View::s_drawingLigX)+2) || (this->fligat && this->lat && !Note1::marq_obl))
     // les dernieres conditions pour permettre ligature flagStemHeighte ancienne
     //	DrawVerticalLine (dc, *ligat_y, y1, (this->fligat && this->lat) ? x2: x1, m_doc->m_parameters.m_stemWidth); // ax2 - drawing flagStemHeight lines missing
     *View::s_drawingLigX = *(View::s_drawingLigX + 1);
     *View::s_drawingLigY = *(View::s_drawingLigY + 1);
     }
     
     
     y3 = y2 - m_doc->m_drawingUnit[staff->staffSize]*6;
     
     if (note->m_lig)
     {
     if (note->m_dur == DUR_BR) //  && this->queue_lig)	// queue gauche bas: DUR_BR initiale descendante // ax2 - no support of queue_lig (see WG corrigeLigature)
     {
     DrawVerticalLine ( dc, y2, y3, x1, m_doc->m_style->m_stemWidth );
     }
     else if (note->m_dur == DUR_LG) // && !this->queue_lig) // DUR_LG en ligature, queue droite bas // ax2 - no support of queue_lig
     {
     DrawVerticalLine (dc, y2, y3, x2, m_doc->m_style->m_stemWidth );
     }
     else if (note->m_dur == DUR_1) // && this->queue_lig )	// queue gauche haut // ax2 - no support of queue_lig
     {
     y2 = y1 + m_doc->m_drawingUnit[staff->staffSize]*6;
     DrawVerticalLine ( dc, y1, y2, x1, m_doc->m_style->m_stemWidth );
     }
     }
     else if (note->m_dur == DUR_LG)		// DUR_LG isolee: queue comme notes normales
     */
    if (note->GetActualDur() == DUR_LG)
    {
        verticalCenter = staff->GetDrawingY() - m_doc->m_drawingDoubleUnit[staff->staffSize]*2;
        // ENZ
        up = (y < verticalCenter) ? ON : OFF;
        // ENZ
        if ( note->m_drawingStemDir != STEMDIRECTION_NONE ) {
            if ( note->m_drawingStemDir == STEMDIRECTION_up) {
                up = ON;
            }
            else {
                up = OFF;
            }
        }
        
        if (!up)
        {
            y3 = y1 - m_doc->m_drawingUnit[staff->staffSize]*8;
            y2 = y1;
        }
        else {
            y3 = y1 + m_doc->m_drawingUnit[staff->staffSize]*6;
            y2 = y1;
        }
        DrawVerticalLine ( dc, y2,y3,x2, m_doc->m_style->m_stemWidth );
    }
    
    return;
}

    
    
} // namespace vrv    
    
