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

#include "doc.h"
#include "layer.h"
#include "note.h"
#include "smufl.h"
#include "staff.h"

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
    int xLedger, xNote, xStem;
    int drawingDur;
    int staffY = staff->GetDrawingY();
    wchar_t fontNo;
    int ledge;
    int verticalCenter = 0;

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

} // namespace vrv    
    
