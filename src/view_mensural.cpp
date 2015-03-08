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
    int xLedger, xNote, xAccid, xStem;
    int drawingDur;
    int staffY = staff->GetDrawingY();
    wchar_t fontNo;
    int ledge;
    int verticalCenter = 0;
    bool flippedNotehead = false;
    bool doubleLengthLedger = false;
    
    xStem = element->GetDrawingX();
    xLedger = xStem;
    
    drawingDur = note->GetDrawingDur();
    drawingDur = ((note->GetColored()==BOOLEAN_true) && drawingDur > DUR_1) ? (drawingDur+1) : drawingDur;
    
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
    if (drawingDur == DUR_LG || drawingDur == DUR_BR || ((note->GetLig()!=LIGATURE_NONE) && drawingDur == DUR_1)) {
        DrawLigature ( dc, noteY, element, layer, staff);
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
        if ((note->GetColored()==BOOLEAN_true) || drawingDur == DUR_2) {
            fontNo = SMUFL_E0A3_noteheadHalf;
        }
        else {
            fontNo = SMUFL_E0A4_noteheadBlack;
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
        
        //DrawLedgerLines(dc, note, staff, aboveStaff, false, 0, numLines);
    }
    
    /************** Accidentals/dots/peripherals: **************/
    
    /*
    if (note->GetAccid() != ACCIDENTAL_EXPLICIT_NONE) {
        xAccid = xNote - 1.5 * m_doc->m_drawingAccidWidth[staffSize][note->m_cueSize];
        
        Accid *accid = &note->m_accid;
        accid->SetOloc(note->GetOct());
        accid->SetPloc(note->GetPname());
        accid->SetAccid(note->GetAccid());
        accid->m_cueSize = note->m_cueSize;
        accid->SetDrawingX( xAccid );
        accid->SetDrawingY( noteY );
        
        //postpone drawing the accidental until later if it's in a chord
        DrawAccid( dc, accid, layer, staff, measure, NULL ); // ax2
    }
    */
    
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
        DrawMeterSigFigures ( dc, x, staff->GetDrawingY(), mensur->GetNum(), mensur->GetNumbase(), staff);
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
    
} // namespace vrv    
    
