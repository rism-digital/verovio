/////////////////////////////////////////////////////////////////////////////
// Name:        musrc_element.cpp
// Author:      Laurent Pugin and Chris Niven
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "view.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <iostream>

//----------------------------------------------------------------------------

#include "accid.h"
#include "beam.h"
#include "chord.h"
#include "custos.h"
#include "devicecontext.h"
#include "doc.h"
#include "dot.h"
#include "keysig.h"
#include "layer.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "mrest.h"
#include "multirest.h"
#include "note.h"
#include "rest.h"
#include "slur.h"
#include "space.h"
#include "smufl.h"
#include "staff.h"
#include "style.h"
#include "syl.h"
#include "system.h"
#include "tie.h"
#include "tuplet.h"
#include "verse.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// View - LayerElement
//----------------------------------------------------------------------------

void View::DrawLayerElement( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    
    int previousColor = m_currentColour;

    if (element == m_currentElement) {
		m_currentColour = AxRED;
    }
    else {
        m_currentColour = AxBLACK;
    }

    if (dynamic_cast<Accid*>(element)) {
        DrawAccid(dc, element, layer, staff, measure);
    }
    else if (dynamic_cast<Barline*>(element)) {
        DrawBarline(dc, element, layer, staff, measure);
    }
    else if (dynamic_cast<Beam*>(element)) {
        DrawBeam(dc, element, layer, staff, measure);
    }
    else if (dynamic_cast<Chord*>(element)) {
        DrawDurationElement(dc, element, layer, staff, measure);
    }
    else if (dynamic_cast<Clef*>(element)) {
        DrawClef(dc, element, layer, staff, measure);
    }
    else if (dynamic_cast<Custos*>(element)) {
        DrawCustos(dc, element, layer, staff, measure);
    }
    else if (dynamic_cast<Dot*>(element)) {
        DrawDot(dc, element, layer, staff, measure);
    }
    else if (dynamic_cast<KeySig*>(element)) {
        DrawKeySig(dc, element, layer, staff, measure);
    }
    else if (dynamic_cast<Mensur*>(element)) {
        DrawMensur(dc, element, layer, staff, measure);
    }
    else if (dynamic_cast<MeterSig*>(element)) {
        DrawMeterSig(dc, element, layer, staff, measure);
    }
    else if (dynamic_cast<MRest*>(element)) {
        DrawMRest(dc, element, layer, staff, measure);
    }
    else if (dynamic_cast<MultiRest*>(element)) {
        DrawMultiRest(dc, element, layer, staff, measure);
    }
    else if (dynamic_cast<Note*>(element)) {
        DrawDurationElement(dc, element, layer, staff, measure);
    }
    else if (dynamic_cast<Rest*>(element)) {
        DrawDurationElement(dc, element, layer, staff, measure);
    }
    else if (dynamic_cast<Slur*>(element)) {
        DrawTie(dc, element, layer, staff, measure);
    }
    else if (dynamic_cast<Space*>(element)) {
        DrawSpace(dc, element, layer, staff, measure);
    }
    else if (dynamic_cast<Syl*>(element)) {
        DrawSyl(dc, element, layer, staff, measure);
    }
    else if (dynamic_cast<Tie*>(element)) {
        DrawTie(dc, element, layer, staff, measure);
    }
    else if (dynamic_cast<Tuplet*>(element)) {
        DrawTuplet(dc, element, layer, staff, measure);
    }
    else if (dynamic_cast<Verse*>(element)) {
        DrawVerse(dc, element, layer, staff, measure);
    }
    
    m_currentColour = previousColor;
}

void View::DrawDurationElement( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    
	if (dynamic_cast<Chord*>(element)) {
        dc->StartGraphic( element, "", element->GetUuid() );
        DrawChord(dc, element, layer, staff, measure);
        dc->EndGraphic(element, this );
    }
    else if (dynamic_cast<Note*>(element)) {
        dc->StartGraphic( element, "", element->GetUuid() );
        DrawNote(dc, element, layer, staff, measure);
        dc->EndGraphic(element, this );
	}
    else if (dynamic_cast<Rest*>(element)) {
        dc->StartGraphic( element, "", element->GetUuid() );
        DrawRest( dc, element, layer, staff, measure );
        dc->EndGraphic(element, this );
	}
	
	return;
}

void View::DrawTuplet(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure) {
    
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    
    Tuplet *tuplet = dynamic_cast<Tuplet*>(element);
    
    dc->StartGraphic( element, "", element->GetUuid() );
    
    // Draw the inner elements
    DrawLayerChildren(dc, tuplet, layer, staff, measure);
    
    // Add to the list of postponed element
    layer->AddToDrawingList( tuplet );
    
    dc->EndGraphic(element, this );
}

// dessine la note en a,b+by. Calcule et dessine lignes addit. avec by=m_drawingY
// b = decalage en fonction oct., clef, a partir du curseur; by = pos. curs.
// Accords: note doit connaitre le x non modifie par accord(), la fin de 
// l'accord (ptr_n->fchord), la valeur y extreme opposee au sommet de la
// queue: le ptr *testchord extern peut garder le x et l'y.
    
void View::DrawNote ( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<Note*>(element)); // Element must be a Note"
    
    Note *note = dynamic_cast<Note*>(element);
    
    if (note->IsMensural()){
        DrawMensuralNote(dc, element, layer, staff, measure);
        return;
    }
    
    if (note->m_crossStaff) staff = note->m_crossStaff;
    
    Chord *inChord = note->IsChordTone();
    
    bool inBeam = false;
    
    // Get the immediate parent of the note
    // to see if beamed or not
    Beam *beam_parent = dynamic_cast<Beam*>(note->GetFirstParent(&typeid(Beam)));
    
    // This note is beamed and cue sized
    if (beam_parent != NULL) {
        if (note->m_cueSize == true) {
            // Get the Parent of the parent
            // we want to see if we are in a group of
            // beamed cue notes
            if (beam_parent->GetListIndex(note) > -1) {
                inBeam = true;
            }
                        
        }
        else {
            // the note is just in a beam
            inBeam = true;
        }
    }
    
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

	xStem = inChord ? inChord->GetDrawingX() : element->GetDrawingX();
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
    
    //if the note is clustered, calculations are different
    if (note->m_cluster) {
        if (note->m_drawingStemDir == STEMDIRECTION_down) {
            //stem down/even cluster = noteheads start on left (incorrect side)
            if (note->m_cluster->size() % 2 == 0) {
                flippedNotehead = (note->m_clusterPosition % 2 != 0);
            }
            //else they start on normal side
            else {
                flippedNotehead = (note->m_clusterPosition % 2 == 0);
            }
            
            //if stem goes down, move ledger start to the left and expand it a full radius
            if(!(note->IsClusterExtreme() && IsOnStaffLine(noteY, staff))) {
                xLedger -= radius;
                doubleLengthLedger = true;
            }
        }
        else {
            //flipped noteheads start on normal side no matter what
            flippedNotehead = (note->m_clusterPosition % 2 == 0);
            
            //if stem goes up, move ledger start to the right and expand it a full radius
            if(!(note->IsClusterExtreme() && IsOnStaffLine(noteY, staff))) {
                xLedger += radius;
                doubleLengthLedger = true;
            }
        }
        
        //positions notehead
        if (!flippedNotehead) {
            xNote = xStem - radius;
        }
        else {
            //if we have a flipped notehead, we need to be in a chord
            assert(inChord);
            if (note->m_drawingStemDir == STEMDIRECTION_up) {
                xNote = xStem + radius - m_doc->m_style->m_stemWidth;
            }
            else if (note->m_drawingStemDir == STEMDIRECTION_down) {
                xNote = xStem - radius * 3 + m_doc->m_style->m_stemWidth;
            }
            else {
                xNote = xStem - radius;
            }
        }
    }
    else
    {
        xNote = xStem - radius;
    }

    /************** Noteheads: **************/
    
    if (drawingDur < DUR_1) {
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
        if ((note->GetColored()==BOOLEAN_true) || drawingDur == DUR_2) {
			fontNo = SMUFL_E0A3_noteheadHalf;
        }
        else {
			fontNo = SMUFL_E0A4_noteheadBlack;
        }
        
		DrawSmuflCode( dc, xNote, noteY, fontNo,  staff->staffSize, note->m_cueSize );

		if (!(inBeam && drawingDur > DUR_4) && !inChord) {
            DrawStem(dc, note, staff, note->m_drawingStemDir, radius, xStem, noteY);
        }

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
 
        //if this is in a chord, we don't want to draw it yet, but we want to keep track of the maxima
        if (inChord) {
            if (inChord->m_drawingLedgerLines.count(staff)==0) {
                std::vector<char> legerLines;
                legerLines.resize(4);
                inChord->m_drawingLedgerLines[staff] = legerLines;
            }
            int idx = doubleLengthLedger + aboveStaff * 2; // 2x2 array
            std::vector<char> *legerLines = &inChord->m_drawingLedgerLines[staff];
            (*legerLines)[idx] = ledgermax(numLines, (*legerLines)[idx]);
        }
        //we do want to go ahead and draw if it's not in a chord
        else {
            DrawLedgerLines(dc, note, staff, aboveStaff, false, 0, numLines);
        }
    }
    
    /************** Accidentals/dots/peripherals: **************/
    
	if (note->GetAccid() != ACCIDENTAL_EXPLICIT_NONE) {
		xAccid = xNote - 1.5 * m_doc->m_drawingAccidWidth[staffSize][note->m_cueSize];

        if (note->m_drawingAccid == NULL)
        {
            note->m_drawingAccid = new Accid();
            note->m_drawingAccid->SetOloc(note->GetOct());
            note->m_drawingAccid->SetPloc(note->GetPname());
            note->m_drawingAccid->SetAccid(note->GetAccid());
            note->m_drawingAccid->m_cueSize = note->m_cueSize;
        }
        
        note->m_drawingAccid->SetDrawingX( xAccid );
        note->m_drawingAccid->SetDrawingY( noteY );
        
        //postpone drawing the accidental until later if it's in a chord
        if (!inChord) DrawAccid( dc, note->m_drawingAccid, layer, staff, measure ); // ax2
	}
	
    if (note->GetDots() && !inChord) {
        int xDot;
        if (note->GetActualDur() < DUR_2 || (note->GetActualDur() > DUR_8 && !inBeam && (note->m_drawingStemDir == STEMDIRECTION_up)))
            xDot = xStem + m_doc->m_drawingUnit[staffSize]*7/2;
        else
            xDot = xStem + m_doc->m_drawingUnit[staffSize]*5/2;
        
		DrawDots( dc, xDot, noteY, note->GetDots(), staff );
	}
    
    if (note->GetDrawingTieAttr()) {
        System *system = dynamic_cast<System*>(measure->GetFirstParent(&typeid(System)));
        // create a placeholder for the tie attribute that will be drawn from the system
        dc->StartGraphic(note->GetDrawingTieAttr(), "", note->GetDrawingTieAttr()->GetUuid().c_str());
        dc->EndGraphic(note->GetDrawingTieAttr(), this);
        if (system) system->AddToDrawingList(note->GetDrawingTieAttr());
    }

    // This will draw lyrics, accid, etc.
    DrawLayerChildren(dc, note, layer, staff, measure);

    if (note->GetFermata() != PLACE_NONE) {
        DrawFermata(dc, element, staff);
    }

    if (note->m_embellishment == EMB_TRILL) {
        DrawTrill(dc, element, staff);
    }
    
	return;
}
    
void View::DrawStem( DeviceContext *dc, LayerElement *object, Staff *staff, data_STEMDIRECTION dir, int radius, int xn, int originY, int heightY)
{
    int staffSize = staff->staffSize;
    int staffY = staff->GetDrawingY();
    int baseStem, totalFlagStemHeight, flagStemHeight, nbFlags;
    int drawingDur = dynamic_cast<DurationInterface*>(object)->GetActualDur();
    int verticalCenter = staffY - m_doc->m_drawingDoubleUnit[staffSize]*2;
    
    baseStem = object->m_cueSize ? ( m_doc->m_drawingUnit[staffSize]*5) : ( m_doc->m_drawingUnit[staffSize]*7);
    flagStemHeight = object->m_cueSize ?  m_doc->m_drawingUnit[staffSize] :  m_doc->m_drawingDoubleUnit[staffSize];
    nbFlags = drawingDur - DUR_8;
    totalFlagStemHeight = flagStemHeight * (nbFlags * 2 - 1) / 2;
    
    if (dir == STEMDIRECTION_down) {
        // flip all lengths
        baseStem = -baseStem;
        totalFlagStemHeight = -totalFlagStemHeight;
        radius = -radius;
        heightY = -heightY;
    }
    
    // If we have flags, add them to the height
    int y1 = originY;
    int y2 = ((drawingDur>DUR_8) ? (y1 + baseStem + totalFlagStemHeight) : (y1 + baseStem)) + heightY;
    int x2 = xn + radius;
    
    if ((dir == STEMDIRECTION_up) && (y2 < verticalCenter) ) {
        y2 = verticalCenter;
    }
    else if ((dir == STEMDIRECTION_down) && (y2 > verticalCenter) ) {
        y2 = verticalCenter;
    }
    
    // shorten the stem at its connection with the note head
    int stemY1 = (dir == STEMDIRECTION_up) ? y1 + m_doc->m_drawingUnit[staffSize] / 4 : y1 - m_doc->m_drawingUnit[staffSize] / 4;
    int stemY2 = y2;
    if (drawingDur > DUR_4) {
        // if we have flags, shorten the stem to make sure we have a nice overlap with the flag glyph
        stemY2 = (dir == STEMDIRECTION_up) ? y2 - m_doc->m_drawingUnit[staffSize] : y2 + m_doc->m_drawingUnit[staffSize];
    }
    
    // draw the stems and the flags
    if (dir == STEMDIRECTION_up) {
        DrawFullRectangle( dc, x2 - m_doc->m_style->m_stemWidth, stemY1, x2, stemY2);
        
        object->m_drawingStemStart.x = object->m_drawingStemEnd.x = x2 - (m_doc->m_style->m_stemWidth / 2);
        object->m_drawingStemStart.y = y1;
        object->m_drawingStemEnd.y = y2;
        object->m_drawingStemDir = STEMDIRECTION_up;
        
        if (drawingDur > DUR_4) {
            DrawSmuflCode( dc, x2 - m_doc->m_style->m_stemWidth, y2, SMUFL_E240_flag8thUp, staff->staffSize, object->m_cueSize );
            for (int i=0; i < nbFlags; i++)
                DrawSmuflCode( dc, x2 - m_doc->m_style->m_stemWidth, y2 - (i + 1) * flagStemHeight, SMUFL_E240_flag8thUp,  staff->staffSize, object->m_cueSize );
        }
    }
    else {
        DrawFullRectangle( dc, x2, stemY1, x2 + m_doc->m_style->m_stemWidth, stemY2);
        
        object->m_drawingStemStart.x = object->m_drawingStemEnd.x = x2 - (m_doc->m_style->m_stemWidth / 2);
        object->m_drawingStemStart.y = y1;
        object->m_drawingStemEnd.y = y2;
        object->m_drawingStemDir = STEMDIRECTION_down;
        
        if (drawingDur > DUR_4) {
            DrawSmuflCode( dc, x2 , y2, SMUFL_E241_flag8thDown , staff->staffSize, object->m_cueSize );
            for (int i=0; i < nbFlags; i++)
                DrawSmuflCode( dc, x2, y2 + (i + 1) * flagStemHeight, SMUFL_E241_flag8thDown,  staff->staffSize,
                              object->m_cueSize );
        }
    }
    if (object->m_cueSize && dynamic_cast<Note*>(object)->m_acciaccatura) {
        DrawAcciaccaturaSlash(dc, object);
    }
    
}

//skips "skip" lines before drawing "n" ledger lines
void View::DrawLedgerLines ( DeviceContext *dc, LayerElement *element, Staff *staff, bool aboveStaff, bool doubleLength, int skip, int n)
{
    //various variables
    int ledgerY;
    int staffY = staff->GetDrawingY();
    int staffSize = staff->staffSize;
    int betweenLines = m_doc->m_drawingDoubleUnit[staffSize];
    int ledge = m_doc->m_drawingLedgerLine[staffSize][element->m_cueSize];
    int noteDiameter = m_doc->m_drawingNoteRadius[staffSize][element->m_cueSize] * 2;
    
    //prep start and end positions of ledger line depending on stem direction and doubleLength
    int xLedgerStart, xLedgerEnd;
    if (doubleLength) {
        if (dynamic_cast<Chord*>(element)->GetDrawingStemDir() == STEMDIRECTION_down) {
            xLedgerStart = element->GetDrawingX() - ledge - noteDiameter;
            xLedgerEnd = element->GetDrawingX() + ledge;
        }
        else {
            xLedgerStart = element->GetDrawingX() - ledge;
            xLedgerEnd = element->GetDrawingX() + ledge + noteDiameter;
        }
    }
    else {
        xLedgerStart = element->GetDrawingX() - ledge;
        xLedgerEnd = element->GetDrawingX() + ledge;
    }
    
    //prep initial Y position
    if (aboveStaff) {
        ledgerY = (skip * betweenLines) + staffY;
    }
    else {
        ledgerY = staffY - m_doc->m_drawingStaffSize[staffSize] - (skip * betweenLines);
        betweenLines = -m_doc->m_drawingDoubleUnit[staffSize];
    }
    
    //add one line's distance to get it off the edge of the staff
    ledgerY += betweenLines;
    
    dc->SetPen( m_currentColour, ToDeviceContextX( m_doc->m_style->m_staffLineWidth ), AxSOLID );
    dc->SetBrush(m_currentColour , AxTRANSPARENT );
    
    //draw the lines
    for (int i = 0; i < n; i++)
    {
        dc->DrawLine( ToDeviceContextX(xLedgerStart) , ToDeviceContextY ( ledgerY ) , ToDeviceContextX(xLedgerEnd) , ToDeviceContextY ( ledgerY ) );
        ledgerY += betweenLines;
    }
    
    dc->ResetPen();
    dc->ResetBrush();
}
    
void View::DrawRest ( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{	
    assert(layer); // Pointer to layer cannot be NULL
    assert(staff); // Pointer to staff cannot be NULL
    assert(dynamic_cast<Rest*>(element)); // Element must be a Rest
        
    Rest *rest = dynamic_cast<Rest*>(element);

	int drawingDur = rest->GetActualDur();
	int x = element->GetDrawingX();
    int y = element->GetDrawingY();
    
    // Temporary fix for rest within tuplet because drawing tuplet requires m_drawingStemXXX to be set
    element->m_drawingStemStart.x = element->m_drawingStemEnd.x = element->GetDrawingX() - (m_doc->m_style->m_stemWidth / 2);
    element->m_drawingStemEnd.y = element->GetDrawingY();
    element->m_drawingStemStart.y = element->GetDrawingY();

    if (drawingDur > DUR_2)
    {
		x -= m_doc->m_drawingNoteRadius[staff->staffSize][rest->m_cueSize];
    }

    switch (drawingDur)
    {	
        case DUR_LG: DrawLongRest ( dc, x, y, staff); break;
        case DUR_BR: DrawBreveRest( dc, x, y, staff); break;
        case DUR_1:
        case DUR_2: DrawWholeRest ( dc, x, y, drawingDur, rest->GetDots(), rest->m_cueSize, staff); break;
        default: DrawQuarterRest( dc, x, y, drawingDur, rest->GetDots(), rest->m_cueSize, staff);
    }
    
    if(rest->GetFermata() != PLACE_NONE) {
        DrawFermata(dc, element, staff);
    }
    
	return;
}
    
void View::DrawMRest(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<MRest*>(element)); // Element must be a MultiRest"
    
    //MRest *mrest = dynamic_cast<MRest*>(element);
    
    dc->StartGraphic( element, "", element->GetUuid() );
    
    //LogMessage("Measure %d - X %d - RightX %d;", measure->GetIdx(), element->m_drawingX, measure->GetRightBarlineX() );
    
    int x = element->GetDrawingX();
    int xCentered = x + (measure->GetDrawingX() + measure->GetRightBarlineX() - x)  / 2;
    int y = element->GetDrawingY();
    
    // move it down according to the number of line in the staff
    y -= staff->m_drawingLines / 2 * m_doc->m_drawingDoubleUnit[staff->staffSize] - m_doc->m_drawingDoubleUnit[staff->staffSize];
    
    DrawWholeRest ( dc, xCentered, y, DUR_1, 0, false, staff );
    
    dc->EndGraphic(element, this);
    
    return;
    
}

/** This function draws multi-measure rests **/
void View::DrawMultiRest(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{	
    int x1, x2, y1, y2, length;

    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<MultiRest*>(element)); // Element must be a Symbol"
    
    MultiRest *multirest = dynamic_cast<MultiRest*>(element);
    
    dc->StartGraphic( element, "", element->GetUuid() );
    
    int x = element->GetDrawingX();
    
    // We do not support more than three chars
    if (multirest->GetNum() > 999)
        multirest->SetNum(999);
    
    // This is 1/2 the length of th black rectangle
	length = (m_doc->m_drawingUnit[staff->staffSize] * 5);
    
    // Position centered in third line
    // it would be m_drawingDoubleUnit * 6.5, or m_drawingDoubleUnit / 2 * 13
	y1 = staff->GetDrawingY() - (m_doc->m_drawingDoubleUnit[staff->staffSize] / 2) * 5;
    y2 = y1 + m_doc->m_drawingDoubleUnit[staff->staffSize];
	
    // a is the central point, claculate x and x2
    x1 = x - length;
    x2 = x + length;
    
    // Draw the base rect
    // make it 8 pixels smaller than the interline space
    // these are the two 4 substracted and added
	DrawFullRectangle(dc, x1, y2 - 4, x2, y1 + 4);
    
    //Draw the to lines at beginning and end
    // make it 8 pixesl longers, and 4 pixels width
    int border = m_doc->m_drawingUnit[staff->staffSize];
    DrawVerticalLine(dc, y1 - border, y2 + border, x1, m_doc->m_style->m_stemWidth * 2);
    DrawVerticalLine(dc, y1 - border, y2 + border, x2, m_doc->m_style->m_stemWidth * 2);
    
    // Draw the text above
    int w, h;
    int start_offset = 0; // offset from x to center text
    
    // convert to string
    std::wstring wtext = IntToTimeSigFigures(multirest->GetNum());
    
    dc->SetFont(&m_doc->m_drawingSmuflFonts[staff->staffSize][0]);
    dc->GetSmuflTextExtent( wtext, &w, &h);
    start_offset = (x2 - x1 - w) /  2; // calculate offset to center text
    DrawSmuflString(dc, x1 + start_offset, staff->GetDrawingY() + 5, wtext, false);
    dc->ResetFont();
    
    dc->EndGraphic(element, this);
    
    return;

}

void View::DrawLongRest ( DeviceContext *dc, int x, int y, Staff *staff)
{
    int x1, x2, y1, y2;
    
    y1 = y;
	x1 = x;
	x2 = x + (m_doc->m_drawingUnit[staff->staffSize] * 2 / 3);
    
    // look if on line or between line
	if ( (y - staff->GetDrawingY()) % m_doc->m_drawingDoubleUnit[staff->staffSize])
		y1 -= m_doc->m_drawingUnit[staff->staffSize];
    
	y2 = y1 + m_doc->m_drawingDoubleUnit[staff->staffSize]*2;
	DrawFullRectangle( dc, x1, y2, x2, y1);
	return;
}


void View::DrawBreveRest ( DeviceContext *dc, int x, int y, Staff *staff)
{	
    int x1, x2, y1, y2;
    y1 = y;
	x1 = x;
	x2 = x + (m_doc->m_drawingUnit[staff->staffSize] * 2 / 3);
    
    // look if one line or between line
	if ( (y - staff->GetDrawingY()) % m_doc->m_drawingDoubleUnit[staff->staffSize])
		y1 -= m_doc->m_drawingUnit[staff->staffSize];
    
	y2 = y1 + m_doc->m_drawingDoubleUnit[staff->staffSize];
	DrawFullRectangle( dc, x1, y2, x2, y1);
	
    // lines
    x1 = x - m_doc->m_drawingUnit[staff->staffSize];
    x2 = x + m_doc->m_drawingUnit[staff->staffSize];

	DrawHorizontalLine ( dc, x1, x2, y2, 1);
	DrawHorizontalLine ( dc, x1, x2, y1, 1);
	return;
}

void View::DrawWholeRest ( DeviceContext *dc, int x, int y, int valeur, unsigned char dots, unsigned int smaller, Staff *staff)
{
    int x1, x2, y1, y2, vertic;
    y1 = y;
    vertic = m_doc->m_drawingUnit[staff->staffSize];
	
    int off;
	float foff;

	if (staff->notAnc)
		foff = (m_doc->m_drawingUnit[staff->staffSize] *1 / 3);
	else
		foff = (m_doc->m_drawingLedgerLine[staff->staffSize][2] * 2) / 3; // i.e., la moitie de la ronde

	if (smaller)
		foff *= (int)( (float)m_doc->m_drawingGraceRatio[0] / (float)m_doc->m_drawingGraceRatio[1] );
	off = (int)foff;

	x1 = x - off;
	x2 = x + off;

	if (valeur == DUR_1)
		vertic = -vertic;

    // look if one line or between line
	if ( (y - staff->GetDrawingY()) % m_doc->m_drawingDoubleUnit[staff->staffSize])
	{
		if (valeur == DUR_2)
			y1 -= vertic;
		else
			y1 += vertic;
	}

	y2 = y1 + vertic;
	DrawFullRectangle ( dc, x1, y1, x2, y2);

	off /= 2;
	x1 -= off;
	x2 += off;

    // legder line
	if (y > (int)staff->GetDrawingY()  || y < staff->GetDrawingY() - m_doc->m_drawingStaffSize[staff->staffSize])
		DrawHorizontalLine ( dc, x1, x2, y1, m_doc->m_style->m_staffLineWidth);

	if (dots)
		DrawDots( dc, (x2 + m_doc->m_drawingUnit[staff->staffSize]), y2, dots, staff);
}


void View::DrawQuarterRest ( DeviceContext *dc, int x, int y, int valeur, unsigned char dots, unsigned int smaller, Staff *staff)
{
    int y2 = y + m_doc->m_drawingDoubleUnit[staff->staffSize];
	DrawSmuflCode( dc, x, y2, SMUFL_E4E5_restQuarter + (valeur-DUR_4), staff->staffSize, smaller );
    
	if (dots)
	{	if (valeur < DUR_16)
			y += m_doc->m_drawingDoubleUnit[staff->staffSize];
		DrawDots( dc, (x + 2 * m_doc->m_drawingDoubleUnit[staff->staffSize]), y, dots, staff);
	}
	return;
}

bool View::IsOnStaffLine ( int y, Staff *staff)
{
    return ((y - staff->GetDrawingY()) % m_doc->m_drawingDoubleUnit[staff->staffSize] == 0 );
}

void View::PrepareChordDots ( DeviceContext *dc, Chord *chord, int x, int y, unsigned char dots, Staff *staff )
{
    std::list<int> *dotsList = &chord->m_dots;
    int fullUnit = m_doc->m_drawingUnit[staff->staffSize];
    int doubleUnit = fullUnit * 2;
    
    //if it's on a staff line to start with, we need to compensate here and add a full unit like DrawDots would
    if (IsOnStaffLine(y, staff)) {
        //defaults to the space above the staffline first
        //if that position is not on the list already, we're good to go
        if(std::find(dotsList->begin(), dotsList->end(), y + fullUnit) == dotsList->end()) y += fullUnit;
        
        //if it is on the list, we should try the spot a doubleUnit below
        else if(std::find(dotsList->begin(), dotsList->end(), y - fullUnit) == dotsList->end()) y -= fullUnit;
        
        //otherwise, any other space looks weird so let's not draw it
        else return;
    }
    //similar if it's not on a staff line
    else {
        //see if the optimal place exists already
        if(std::find(dotsList->begin(), dotsList->end(), y) == dotsList->end()) {}
        //if it does, then look up a space first
        else if(std::find(dotsList->begin(), dotsList->end(), y + doubleUnit) == dotsList->end()) y += doubleUnit;
        //then look down a space
        else if(std::find(dotsList->begin(), dotsList->end(), y - doubleUnit) == dotsList->end()) y -= doubleUnit;
        //otherwise let's not draw it
        else return;
    }
    
    //finally, make sure it's not outside the acceptable extremes of the chord
    int yMax, yMin;
    chord->GetYExtremes(&yMax, &yMin);
    if (y > (yMax + fullUnit)) return;
    if (y < (yMin - fullUnit)) return;
    
    //if it's not, add it to the dots list and go back to DrawChord
    dotsList->push_back(y);
    return;
}

void View::DrawDots ( DeviceContext *dc, int x, int y, unsigned char dots, Staff *staff)
{
	int i;
    if ( IsOnStaffLine(y, staff) ) {
        y += m_doc->m_drawingUnit[staff->staffSize];
    }
	for (i = 0; i < dots; i++) {
        DrawDot ( dc, x, y );
		x += std::max (6, 2 * m_doc->m_drawingUnit[staff->staffSize]);
	}
	return;
}

void View::DrawBarline( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(staff->m_parent); // Pointer to system cannot be NULL"
    assert(dynamic_cast<Barline*>(element)); // Element must be a Barline"
    
    Barline *barLine = dynamic_cast<Barline*>(element);
    int x = element->GetDrawingX();
    
    dc->StartGraphic( element, "", element->GetUuid() );
    
    if (barLine->m_partialBarline)
    {
        DrawPartialBarline ( dc, dynamic_cast<System*>( staff->m_parent ), x, staff);
    }
    else
    {
        int y = staff->GetDrawingY();
        DrawBarline( dc, y, y - m_doc->m_drawingStaffSize[staff->staffSize], barLine );
    }
    
    dc->EndGraphic(element, this );
}
    
void View::DrawChord( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    Chord* chord = dynamic_cast<Chord*>(element);
    
    int staffSize = staff->staffSize;
    int staffY = staff->GetDrawingY();
	int verticalCenter = staffY - m_doc->m_drawingDoubleUnit[staffSize]*2;
    int radius = m_doc->m_drawingNoteRadius[staffSize][chord->m_cueSize];
    int fullUnit = m_doc->m_drawingUnit[staffSize];
    
    bool inBeam = false;
    // Get the immadiate parent of the note
    // to see if beamed or not
    Beam *beam_parent = dynamic_cast<Beam*>(chord->GetFirstParent(&typeid(Beam)));
    
    // This note is beamed and cue sized
    if (beam_parent != NULL) {
        if (chord->m_cueSize == true) {
            // Get the Parent of the parent
            // we want to see if we are in a group of
            // beamed cue notes
            if (beam_parent->GetListIndex(chord) > -1) {
                inBeam = true;
            }
        }
        else {
            // the note is just in a beam
            inBeam = true;
        }
    }
    
    /************ Ledger line reset ************/
    
    //if there are double-length lines, we only need to draw single-length after they've been drawn
    chord->m_drawingLedgerLines.clear();
    
    /************ Stems ************/
    
    int drawingDur = chord->GetDur();
    
    //(unless we're in a beam)
    if (!(inBeam && drawingDur > DUR_4)) {
        int yMax, yMin;
        chord->GetYExtremes(&yMax, &yMin);
        
        if ( chord->HasStemDir() ) {
            chord->SetDrawingStemDir(chord->GetStemDir());
        }
        else if ( layer->GetDrawingStemDir() != STEMDIRECTION_NONE) {
            chord->SetDrawingStemDir(layer->GetDrawingStemDir());
        }
        else {
            chord->SetDrawingStemDir(yMax - verticalCenter >= verticalCenter - yMin ? STEMDIRECTION_down : STEMDIRECTION_up);
        }
        
        int beamX = chord->GetDrawingX();
        int originY = ( chord->GetDrawingStemDir() == STEMDIRECTION_down ? yMax : yMin );
        int heightY = yMax - yMin;
        
        DrawStem(dc, chord, staff, chord->GetDrawingStemDir(), radius, beamX, originY, heightY);
    }
    
    /************ Draw children (notes) ************/
    
    DrawLayerChildren(dc, chord, layer, staff, measure);
    
    /************ Dots ************/
    
    chord->m_dots.clear();
    
    if (chord->GetDots()) {
        int numDots = chord->GetDots();
        
        //Set the x value...
        int dotsX;
        
        //make sure all the dots are at the same X position
        if (chord->GetDur() < DUR_2 || (chord->GetDur() > DUR_8 && !inBeam && (chord->GetDrawingStemDir() == STEMDIRECTION_up))) {
            dotsX = chord->GetDrawingX() + (fullUnit * 7/2);
        }
        else {
            dotsX = chord->GetDrawingX() + (fullUnit * 5/2);
        }
        
        // Notes in clusters: If the stem points up and we have a note on the (incorrect) right side of the stem, add a note diameter to the dot positioning to avoid overlapping.
        if ((chord->GetDrawingStemDir() == STEMDIRECTION_up) && (chord->m_clusters.size() > 0)) dotsX += (radius * 2);
        
        //Prep where the dots will go by preventing overlaps and using space efficiently
        for (ListOfObjects::reverse_iterator rit = chord->GetList(chord)->rbegin(); rit != chord->GetList(chord)->rend(); rit++)
        {
            Note *note = dynamic_cast<Note*>(*rit);
            PrepareChordDots(dc, chord, dotsX, note->GetDrawingY(), numDots, staff);
        }
        
        //And then draw them
        std::list<int> *dotsList = &chord->m_dots;
        for (std::list<int>::iterator it=dotsList->begin(); it != dotsList->end(); ++it)
            DrawDots(dc, dotsX, *it, numDots, staff);
    }
    
    /************ Accidentals ************/
    
    //navigate through list of notes, starting with outside and working in

    chord->ResetAccidList();
    std::vector<Note*> noteList = chord->m_accidList;
    int size = (int)noteList.size();
    if (size > 0)
    {
        //set the default x position
        int xAccid = chord->GetDrawingX() - (radius * 2);

        //if chord is a down-stemmed non-cluster, it needs one more note diameter of space
        if ((chord->GetDrawingStemDir() == STEMDIRECTION_down) && (chord->m_clusters.size() > 0))
        {
            xAccid -= (radius * 2);
        }

        int fwIdx, idx, bwIdx;

        //reset the boolean 2d vector
        chord->ResetAccidSpace(fullUnit);
        std::vector<int> accidClusterStarts;
        
        //iterate through the list of notes with accidentals
        for(idx = 0; idx < size; idx++)
        {
            Accid *curAccid = noteList[idx]->m_drawingAccid;

            //if the note does not need to be moved, save a new cluster start position
            if (CalculateAccidX(staff, curAccid, chord, false))
            {
                accidClusterStarts.push_back(idx);
            }
        }
        
        chord->ResetAccidSpace(fullUnit);
        int accidSize = (int)accidClusterStarts.size();
        
        //for each note that conflicts
        for(idx = 0; idx < accidSize; idx++)
        {
            //set fwIdx to the start of the cluster
            fwIdx = accidClusterStarts[idx];
            //if this is the last cluster, set bwIdx to the last note in the chord
            if (idx == accidSize - 1) bwIdx = size - 1;
            //otherwise, set bwIdx to one before the beginning of the next cluster
            else bwIdx = accidClusterStarts[idx + 1] - 1;
            
            //if it's even, this will catch the overlap; if it's odd, there's an if in the middle there
            while (fwIdx <= bwIdx)
            {
                Accid *accidFwd = noteList[fwIdx]->m_drawingAccid;
                Accid *accidBwd = noteList[bwIdx]->m_drawingAccid;
                
                //if the top note has an accidental, draw it and update prevAccid
                accidFwd->SetDrawingX(xAccid);
                CalculateAccidX(staff, accidFwd, chord, true);
                DrawAccid(dc, accidFwd, layer, staff, measure);
                //same, except with an extra check that we're not doing the same note twice
                if (fwIdx != bwIdx) {
                    accidBwd->SetDrawingX(xAccid);
                    CalculateAccidX(staff, accidBwd, chord, true);
                    DrawAccid(dc, accidBwd, layer, staff, measure);
                    bwIdx--;
                }
                
                fwIdx++;
            }
            fwIdx = idx;
        }
    }
    
    /************ Ledger lines ************/

    dc->SetPen( m_currentColour, ToDeviceContextX( m_doc->m_style->m_staffLineWidth ), AxSOLID );
    dc->SetBrush(m_currentColour , AxTRANSPARENT );
    
    MapOfLedgerLineFlags::iterator iter;
    for(iter = chord->m_drawingLedgerLines.begin(); iter != chord->m_drawingLedgerLines.end(); iter++) {
        
        std::vector<char> *legerLines = &(*iter).second;
        
        //if there are double-length lines, we only need to draw single-length after they've been drawn
        (*legerLines)[0] -= (*legerLines)[1];
        (*legerLines)[2] -= (*legerLines)[3];
        
        //double-length lines below the staff
        DrawLedgerLines(dc, chord, (*iter).first, false, true, 0, (*legerLines)[1]);
        
        //remainder single-length lines below the staff
        DrawLedgerLines(dc, chord, (*iter).first, false, false, (*legerLines)[1], (*legerLines)[0]);
        
        //double-length lines above the staff
        DrawLedgerLines(dc, chord, (*iter).first, true, true, 0, (*legerLines)[3]);
        
        //remainder single-length lines above the staff
        DrawLedgerLines(dc, chord, (*iter).first, true, false, (*legerLines)[3], (*legerLines)[2]);

    }
    
    dc->ResetPen();
    dc->ResetBrush();
}

void View::DrawClef( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<Clef*>(element)); // Element must be a Clef"
    
    Clef *clef = dynamic_cast<Clef*>(element);

    dc->StartGraphic( element, "", element->GetUuid() );
	
	int b = staff->GetDrawingY();
	int a = element->GetDrawingX();
    int sym = SMUFL_E050_gClef;	//sSOL, position d'ordre des cles sol fa ut in fonts

    /*  poser sym=no de position sSOL dans la fonte
     *	au depart; ne faire operation sur b qu'une fois pour cas semblables,
     *  et au palier commun superieur, incrementer sym, sans break.
     */
	switch(clef->GetClefId())
	{
		case C1 :
            sym = SMUFL_E05C_cClef;
            b -= m_doc->m_drawingStaffSize[ staff->staffSize ];
            break;
		case G1 :
            b -= m_doc->m_drawingStaffSize[ staff->staffSize ]; 
            break;
		case G2_8va :
            sym = SMUFL_E053_gClef8va;
            b -= m_doc->m_drawingDoubleUnit[ staff->staffSize ]*3;
            break;
        case G2_8vb :
            sym = SMUFL_E052_gClef8vb;
            b -= m_doc->m_drawingDoubleUnit[ staff->staffSize ]*3;
            break;
		case C2 :
            sym = SMUFL_E05C_cClef;
            b -= m_doc->m_drawingDoubleUnit[ staff->staffSize ]*3;
            break;
		case G2 :
            b -= m_doc->m_drawingDoubleUnit[ staff->staffSize ]*3; 
            break;
		case F3 :
            sym = SMUFL_E062_fClef;
            b -= m_doc->m_drawingDoubleUnit[ staff->staffSize ]*2;
            break;
		case C3 :
            sym = SMUFL_E05C_cClef;
            b -= m_doc->m_drawingDoubleUnit[ staff->staffSize ]*2;
            break;
		case F5 :
            sym =SMUFL_E062_fClef;
            break;
		case F4 :
            sym = SMUFL_E062_fClef;
            b -= m_doc->m_drawingDoubleUnit[ staff->staffSize ];
            break;
		case C4 :
            sym = SMUFL_E05C_cClef;
            b -= m_doc->m_drawingDoubleUnit[ staff->staffSize ];
            break;
		case C5 :
            sym = SMUFL_E05C_cClef;
            break;
		case perc :
            b -= m_doc->m_drawingDoubleUnit[ staff->staffSize ]*2;
            // FIXME
            sym = SMUFL_E05C_cClef;
            break;
		default: 
            break;
	}

    bool cueSize = clef->m_cueSize;
    // force cue size for intermediate clefs
    if (clef->GetFirstParent(&typeid(Layer))) cueSize = true;
    
    if (!cueSize)
        a -= m_doc->m_drawingUnit[staff->staffSize] * 2;
    
    DrawSmuflCode ( dc, a, b, sym, staff->staffSize, cueSize  );
   
    dc->EndGraphic(element, this );
}

void View::DrawMeterSigFigures( DeviceContext *dc, int x, int y, int num, int numBase, Staff *staff)
{
    assert( dc ); // DC cannot be NULL
    
    int ynum, yden;
    std::wstring wtext;
    
    if (numBase)
    {
        ynum = y - (m_doc->m_drawingUnit[staff->staffSize]*2);
        yden = ynum - (m_doc->m_drawingDoubleUnit[staff->staffSize]*2);
    }
    else
        ynum = y - (m_doc->m_drawingUnit[staff->staffSize]*4);
    
    if (numBase > 9 || num > 9)	{
        x += m_doc->m_drawingUnit[staff->staffSize] * 2;
    }
    
    dc->SetFont( &m_doc->m_drawingSmuflFonts[staff->staffSize][0] );
    
    wtext = IntToTimeSigFigures(num);
    DrawSmuflString ( dc, x, ynum, wtext, 1, staff->staffSize);	// '1' = centrer
    
    if (numBase)
    {
        wtext = IntToTimeSigFigures(numBase);
        DrawSmuflString ( dc, x, yden, wtext, 1, staff->staffSize);	// '1' = centrer
    }
    
    dc->ResetFont();
    
    return;
}
    
void View::DrawMeterSig( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<MeterSig*>(element)); // Element must be a Mensur"
    
    MeterSig *meterSig = dynamic_cast<MeterSig*>(element);
    
    dc->StartGraphic( element, "", element->GetUuid() );
    
    int y = staff->GetDrawingY() - (m_doc->m_drawingUnit[ staff->staffSize ]*4);
    int x = element->GetDrawingX();
    
    if ( meterSig->GetSym() == METERSIGN_common ) {
        DrawSmuflCode( dc, element->GetDrawingX(), y, SMUFL_E08A_timeSigCommon, staff->staffSize, false);
        x += m_doc->m_drawingUnit[staff->staffSize] * 5; // step forward because we have a symbol
    }
    else if ( meterSig->GetSym() == METERSIGN_cut ) {
        DrawSmuflCode( dc, element->GetDrawingX(), y, SMUFL_E08B_timeSigCutCommon, staff->staffSize, false);
        x += m_doc->m_drawingUnit[staff->staffSize] * 5; // step forward because we have a symbol
    }
    else if (meterSig->GetCount())
    {	
        DrawMeterSigFigures ( dc, x, staff->GetDrawingY(), meterSig->GetCount(), meterSig->GetUnit(), staff);
    }
    
    dc->EndGraphic(element, this );
    
}
    
bool View::CalculateAccidX(Staff *staff, Accid *accid, Chord *chord, bool adjustHorizontally)
{
    std::vector< std::vector<bool> > *accidSpace = &chord->m_accidSpace;
    
    //global drawing variables
    int fullUnit = m_doc->m_drawingUnit[staff->staffSize];
    int halfUnit = fullUnit / 2;
    int accidHeight = ACCID_HEIGHT * halfUnit;
    
    //drawing variables for the chord
    int xLength = (int)accidSpace->front().size();
    int yHeight = (int)accidSpace->size() - 1;
    int listTop = chord->m_accidSpaceTop;
    int listBot = chord->m_accidSpaceBot;
    
    //drawing variables for the accidental
    int type = accid->GetAccid();
    int centerY = accid->GetDrawingY();
    int topY = centerY + (accidHeight / 2);
    int bottomY = centerY - (accidHeight / 2);
    
    //difference between left end and right end of the accidental
    int accidWidthDiff = ACCID_WIDTH - 1;
    //difference between top and bottom of the accidental
    int accidHeightDiff = ACCID_HEIGHT - 1;
    //drawing variables for the accidental in accidSpace units
    int accidTop = std::max(0, listTop - topY) / halfUnit;
    int accidBot, alignmentThreshold;
    //the left side of the accidental; gets incremented to avoid conflicts
    int currentX = accidWidthDiff;
    
    //another way of calculating accidBot
    assert(((int)accidSpace->size() - 1) - ((std::max(0, bottomY - listBot)) / halfUnit) == accidTop + accidHeightDiff);
    
    // store it for asserts
    int accidSpaceSize = (int)accidSpace->size();
    assert(accidTop >= 0);
    assert(accidTop < accidSpaceSize);
    
    /*
     * Make sure all four corners of the accidental are not on an already-taken spot.
     * The top right corner of a flat can overlap something else; make sure that the bordering sections do not overlap.
     * Move the accidental one half-unit left until it doesn't overlap.
     */
    if (type == ACCIDENTAL_EXPLICIT_f) {
        alignmentThreshold = 2;
        accidBot = accidTop + (accidHeightDiff * FLAT_BOTTOM_HEIGHT_MULTIPLIER);
        assert(accidBot < accidSpaceSize);
        while (currentX < xLength) {
            if (accidSpace->at(accidTop + (ACCID_HEIGHT * FLAT_CORNER_HEIGHT_IGNORE))[currentX - accidWidthDiff]) currentX += 1;
            // just in case
            else if (currentX - accidWidthDiff + (ACCID_WIDTH * FLAT_CORNER_WIDTH_IGNORE) >= xLength ) break;
            else if (accidSpace->at(accidTop)[currentX - accidWidthDiff + (ACCID_WIDTH * FLAT_CORNER_WIDTH_IGNORE)]) currentX += 1;
            else if (accidSpace->at(accidBot)[currentX - accidWidthDiff]) currentX += 1;
            else if (accidSpace->at(accidTop)[currentX]) currentX += 1;
            else if (accidSpace->at(accidBot)[currentX]) currentX += 1;
            else break;
        };
    }
    else if (type == ACCIDENTAL_EXPLICIT_n) {
        alignmentThreshold = 1;
        accidBot = accidTop + accidHeightDiff;
        assert(accidBot < accidSpaceSize);
        //Midpoint needs to be checked for non-flats as there's a chance that a natural/sharp could completely overlap a flat
        int accidMid = accidTop + (accidBot - accidTop) / 2;
        while (currentX < xLength) {
            if (accidSpace->at(accidTop + (ACCID_HEIGHT * NATURAL_CORNER_HEIGHT_IGNORE))[currentX - accidWidthDiff]) currentX += 1;
            // just in case
            else if (currentX - accidWidthDiff + (ACCID_WIDTH * NATURAL_CORNER_WIDTH_IGNORE) >= xLength ) break;
            else if (accidSpace->at(accidTop)[currentX - accidWidthDiff + (ACCID_WIDTH * NATURAL_CORNER_WIDTH_IGNORE)]) currentX += 1;
            else if (accidSpace->at(accidMid)[currentX - accidWidthDiff]) currentX += 1;
            else if (accidSpace->at(accidBot)[currentX - accidWidthDiff]) currentX += 1;
            else if (accidSpace->at(accidTop)[currentX]) currentX += 1;
            else if (accidSpace->at(accidMid)[currentX]) currentX += 1;
            else if (accidSpace->at(accidBot)[currentX]) currentX += 1;
            else break;
        };
    }
    else if (type == ACCIDENTAL_EXPLICIT_s) {
        accidBot = accidTop + accidHeightDiff;
        alignmentThreshold = 1;
        //Midpoint needs to be checked for non-flats as there's a chance that a natural/sharp could completely overlap a flat
        int accidMid = accidTop + (accidBot - accidTop) / 2;
        while (currentX < xLength) {
            if (accidSpace->at(accidTop)[currentX - accidWidthDiff]) currentX += 1;
            else if (accidSpace->at(accidMid)[currentX - accidWidthDiff]) currentX += 1;
            else if (accidSpace->at(accidBot)[currentX - accidWidthDiff]) currentX += 1;
            else if (accidSpace->at(accidTop)[currentX]) currentX += 1;
            else if (accidSpace->at(accidMid)[currentX]) currentX += 1;
            else if (accidSpace->at(accidBot)[currentX]) currentX += 1;
            else break;
        };
    }
    else {
        accidBot = accidTop + accidHeightDiff;
        alignmentThreshold = 1;
        assert(accidBot < accidSpaceSize);
        //Midpoint needs to be checked for non-flats as there's a chance that a natural/sharp could completely overlap a flat
        int accidMid = accidTop + (accidBot - accidTop) / 2;
        assert(accidMid < accidSpaceSize);
        while (currentX < xLength) {
            if (accidSpace->at(accidTop)[currentX - accidWidthDiff]) currentX += 1;
            else if (accidSpace->at(accidMid)[currentX - accidWidthDiff]) currentX += 1;
            else if (accidSpace->at(accidBot)[currentX - accidWidthDiff]) currentX += 1;
            else if (accidSpace->at(accidTop)[currentX]) currentX += 1;
            else if (accidSpace->at(accidMid)[currentX]) currentX += 1;
            else if (accidSpace->at(accidBot)[currentX]) currentX += 1;
            else break;
        };
    }
    
    //If the accidental is lined up with the one above it, move it left by a halfunit to avoid visual confusion
    //This doesn't need to be done with accidentals that are as far left or up as possible
    if ((currentX < xLength - 1) && (accidTop > 1))
    {
        int yComp = accidTop - alignmentThreshold;
        assert(yComp < accidSpaceSize);
        assert(yComp >= 0);
        if((accidSpace->at(yComp)[currentX + 1] == false) && (accidSpace->at(yComp)[currentX] == true)) currentX += 1;
    }
    
    //If the accidental is lined up with the one below it, move it left by a halfunit to avoid visual confusion
    //This doesn't need to be done with accidentals that are as far left or down as possible
    if ((currentX < xLength - 1) && (accidBot < (yHeight - 1)) && accidBot > 1)
    {
        int yComp = accidBot;
        assert(yComp < accidSpaceSize);
        assert(yComp >= 0);
        if((accidSpace->at(yComp)[currentX + 1] == false) && (accidSpace->at(yComp)[currentX] == true)) currentX += 1;
    }

    //Just to make sure.
    assert(currentX <= xLength);
    
    //If we need to move the accidental horizontally, move it by currentX half-units.
    if (adjustHorizontally)
    {
        int xShift = currentX * halfUnit;
        accid->SetDrawingX(accid->GetDrawingX() - xShift);
        
        //mark the spaces as taken (true in accidSpace)
        for(int xIdx = currentX; xIdx > currentX - ACCID_WIDTH; xIdx--)
        {
            for(int yIdx = accidTop; yIdx < accidBot + 1; yIdx++)
            {
                accidSpace->at(yIdx).at(xIdx) = true;
            }
        }
    }
    //Otherwise, just mark its vertical position so we can see if there are any vertical conflicts
    else
    {
        for(int xIdx = 0; xIdx < ACCID_WIDTH; xIdx++) //x from 0 to 4, base position
        {
            for(int yIdx = accidTop; yIdx < accidBot + 1; yIdx++)
            {
                accidSpace->at(yIdx).at(xIdx) = true;
            }
        }
    }
    
    //For debugging; leaving this in temporarily
//    for (int vIdx = 0; vIdx < accidSpace->size(); vIdx++)
//    {
//        std::cout << "|";
//        std::vector<bool> thisRow = accidSpace->at(vIdx);
//        for (int hIdx = (int)thisRow.size() - 1; hIdx >= 0; hIdx --)
//        {
//            std::cout << thisRow.at(hIdx) << "|";
//        }
//        std::cout << std::endl;
//    }
//    std::cout << std::endl;
    
    //Regardless of whether or not we moved it, return true if there was a conflict and currentX would have been moved
    return (currentX - accidWidthDiff == 0);
}

void View::DrawAccid( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure, Accid *prevAccid )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<Accid*>(element)); // Element must be a Symbol"
    
    Accid *accid = dynamic_cast<Accid*>(element);
    dc->StartGraphic( element, "", element->GetUuid() );

    // Parent will be NULL if we are drawing a note @accid (see DrawNote) - the y value is already set
    if ( accid->m_parent ) {
        accid->SetDrawingY( accid->GetDrawingY() + CalculatePitchPosY( staff, accid->GetPloc(), layer->GetClefOffset( accid ), accid->GetOloc()) );
    }
    
    //Get the offset
    int x = accid->GetDrawingX();
    int y = accid->GetDrawingY();
    
    int symc = SMUFL_E261_accidentalNatural;
    switch (accid->GetAccid())
    {
        case ACCIDENTAL_EXPLICIT_n :  symc = SMUFL_E261_accidentalNatural; break;
        
        /* The ACCID_DOUBLE_SHARP definition is used in two ways:
         * 1) Antique notation (notAnc == true): it displays the two
         *    stacked sharps used in key signatures;
         * 2) Modern notation: is display a double flat
         *    this poses no particular rendering problems;
         * Case 1) on the other hand requires that the sharp
         * is printed two times, the second time shifting it up
         * (hence no break in the case statement)
         * Same thing applies to LEIPZIG_ACCID_FLAT, but in this
         * case it is used ONLY ad stacked flats (no double flat
         * glyph).
         */
        case ACCIDENTAL_EXPLICIT_ss :
            // This needs to be fixed because it is the old Wolfgang way to handle
            // mensural notation
            if (staff->notAnc) {
                    symc = SMUFL_E262_accidentalSharp;
                    DrawSmuflCode ( dc, x, y, symc,  staff->staffSize, accid->m_cueSize );
                    y += 7*m_doc->m_drawingUnit[staff->staffSize]; // LP
            } else {
                symc = SMUFL_E263_accidentalDoubleSharp;
                break;
            }
        case ACCIDENTAL_EXPLICIT_s : symc = SMUFL_E262_accidentalSharp; break;
        case ACCIDENTAL_EXPLICIT_ff : symc = SMUFL_E264_accidentalDoubleFlat;
        case ACCIDENTAL_EXPLICIT_f : symc = SMUFL_E260_accidentalFlat; break;
        case ACCIDENTAL_EXPLICIT_su : symc = SMUFL_E268_accidentalNaturalSharp; break; // Not sure this is correct...
        case ACCIDENTAL_EXPLICIT_fu : symc= SMUFL_E267_accidentalNaturalFlat; break; // Same
        default : break;
    }
    
    DrawSmuflCode ( dc, x, y, symc, staff->staffSize, accid->m_cueSize );

    dc->EndGraphic(element, this );
}

void View::DrawSpace(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure) {
    
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    
    dc->StartGraphic( element, "", element->GetUuid() );
    dc->DrawPlaceholder( ToDeviceContextX( element->GetDrawingX() ), ToDeviceContextY( element->GetDrawingY() ) );
    dc->EndGraphic(element, this );
}

void View::DrawCustos( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<Custos*>(element)); // Element must be a Symbol"
    
    Custos *custos = dynamic_cast<Custos*>(element);
    dc->StartGraphic( element, "", element->GetUuid() );
    
    element->SetDrawingY( element->GetDrawingY() + CalculatePitchPosY( staff, custos->GetPloc(), layer->GetClefOffset( element ), custos->GetOloc()) );

    int x = element->GetDrawingX();
    int y = element->GetDrawingY();
    
    y -= m_doc->m_drawingUnit[staff->staffSize] - m_doc->m_drawingUnit[staff->staffSize]/4;  // LP - correction in 2.0.0
    
    // HARDCODED (smufl code wrong)
    DrawSmuflCode( dc, x, y, 35,  staff->staffSize, custos->m_cueSize );
    
    dc->EndGraphic(element, this ); //RZ

}

void View::DrawDot( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<Dot*>(element)); // Element must be a Symbol"
    
    Dot *dot = dynamic_cast<Dot*>(element);
    dc->StartGraphic( element, "", element->GetUuid() );
    
    element->SetDrawingY( element->GetDrawingY() + CalculatePitchPosY( staff, dot->GetPloc(), layer->GetClefOffset( element ), dot->GetOloc()) );
    
    int x = element->GetDrawingX();
    int y = element->GetDrawingY();
    
    // Use the note to which the points to for position
    if ( dot->m_drawingNote ) {
        x = dot->m_drawingNote->GetDrawingX() + m_doc->m_drawingUnit[staff->staffSize]*7/2;
        y = dot->m_drawingNote->GetDrawingY();
    }

    DrawDots( dc, x, y, 1, staff );
    
    dc->EndGraphic(element, this );

}
    
int View::GetSylY( Syl *syl, Staff *staff )
{
    assert( syl && staff );
    
    int y = syl->GetStart()->GetDrawingY();
    if (staff->GetAlignment() ) {
        y = staff->GetDrawingY() + staff->GetAlignment()->GetMaxHeight() - syl->m_drawingVerse * m_doc->m_drawingUnit[staff->staffSize] * 4;
    }
    return y;
}
    
void View::DrawSyl( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    Syl *syl = dynamic_cast<Syl*>(element);
    
    if ( !syl->GetStart() ) {
        LogDebug("Syl parent note was not found");
        return;
    }
    
    // move the position back - to be updated HARDCODED also see View::DrawSylConnector and View::DrawSylConnectorLines
    syl->SetDrawingX( syl->GetStart()->GetDrawingX() - m_doc->m_drawingUnit[staff->staffSize] * 2 );
    syl->SetDrawingY( GetSylY(syl, staff) );
    
    dc->StartGraphic( syl, "", syl->GetUuid() );
    
    dc->SetBrush( m_currentColour, AxSOLID );
    
    FontInfo currentFont;
    if (syl->HasFontstyle()) {
        currentFont = m_doc->m_drawingLyricFonts[ staff->staffSize ];
        currentFont.SetStyle(syl->GetFontstyle());
        dc->SetFont(&currentFont);
    }
    else {
        dc->SetFont( &m_doc->m_drawingLyricFonts[ staff->staffSize ] );
    }
    
    DrawLyricString(dc, syl->GetDrawingX(), syl->GetDrawingY(), syl->GetText().c_str(), staff->staffSize );
    
    dc->ResetFont();
    dc->ResetBrush();
    
    if (syl->GetStart() && syl->GetEnd()) {
        System *currentSystem = dynamic_cast<System*>( measure->GetFirstParent( &typeid(System) ) );
        // Postpone the drawing of the syl to the end of the system; this will call DrawSylConnector
        // that will look if the last note is in the same system (or not) and draw the connectors accordingly
        if (currentSystem) {
            currentSystem->AddToDrawingList(syl);
        }
    }
    
    dc->EndGraphic(syl, this );

}

void View::DrawSylConnector( DeviceContext *dc, Syl *syl, int x1, int x2, Staff *staff, char spanningType, DocObject *graphic )
{
    assert( syl->GetStart() && syl->GetEnd());
    if ( !syl->GetStart() || !syl->GetEnd()) return;
    
    int y = GetSylY(syl, staff);
    int w, h;
    
    // The both correspond to the current system, which means no system break in-between (simple case)
    if ( spanningType ==  SPANNING_START_END ) {
        dc->SetFont( &m_doc->m_drawingLyricFonts[ staff->staffSize ] );
        dc->GetTextExtent(syl->GetText(), &w, &h);
        dc->ResetFont();
        // x position of the syl is two units back
        x1 += w - m_doc->m_drawingUnit[staff->staffSize] * 2;
    }
    // Only the first parent is the same, this means that the syl is "open" at the end of the system
    else  if ( spanningType ==  SPANNING_START) {
        dc->SetFont( &m_doc->m_drawingLyricFonts[ staff->staffSize ] );
        dc->GetTextExtent(syl->GetText(), &w, &h);
        dc->ResetFont();
        // idem
        x1 += w - m_doc->m_drawingUnit[staff->staffSize] * 2;
        
    }
    // We are in the system of the last note - draw the connector from the beginning of the system
    else if ( spanningType ==  SPANNING_END ) {
        // nothing to adjust
    }
    // Rare case where neither the first note and the last note are in the current system - draw the connector throughout the system
    else {
        // nothing to adjust
    }
    
    if ( graphic ) dc->ResumeGraphic(graphic, graphic->GetUuid());
    else dc->StartGraphic(syl, "spanning-connector", "");
    dc->DeactivateGraphic();
    DrawSylConnectorLines( dc, x1, x2, y, syl, staff);
    dc->ReactivateGraphic();
    if ( graphic ) dc->EndResumedGraphic(graphic, this);
    else dc->EndGraphic(syl, this);
    
}

void View::DrawSylConnectorLines( DeviceContext *dc, int x1, int x2, int y, Syl *syl, Staff *staff )
{
    if (syl->GetCon() == CON_d) {
        
        y += m_doc->m_drawingUnit[staff->staffSize] * 2 / 3;
        // x position of the syl is two units back
        x2 -= 2 * (int)m_doc->m_drawingUnit[staff->staffSize];
        
        //if ( x1 > x2 ) {
        //    DrawFullRectangle(dc, x1, y + 2* m_doc->m_style->m_barlineWidth, x2, y + 3 * m_doc->m_style->m_barlineWidth);
        //    LogDebug("x1 > x2 (%d %d)", x1, x2 );
        //}
        
        // the length of the dash and the space between them - can be made a parameter
        int dashLength = m_doc->m_drawingUnit[staff->staffSize] * 4 / 3;
        int dashSpace = m_doc->m_drawingStaffSize[staff->staffSize] * 5 / 3;
        int halfDashLength = dashLength / 2;
        
        int dist = x2 - x1;
        int nbDashes = dist / dashSpace;
        
        int margin = dist / 2;
        // at least one dash
        if (nbDashes < 2) {
            nbDashes = 1;
        }
        else {
            margin = (dist - ((nbDashes - 1) * dashSpace)) / 2;
        }
        margin -= dashLength / 2;
        int i, x;
        for (i = 0; i < nbDashes; i++) {
            x = x1 + margin + (i *  dashSpace);
            DrawFullRectangle(dc, x - halfDashLength, y, x + halfDashLength, y + m_doc->m_style->m_barlineWidth);
        }
        
    }
    else if (syl->GetCon() == CON_u) {
        x1 += (int)m_doc->m_drawingUnit[staff->staffSize] / 2;
        DrawFullRectangle(dc, x1, y, x2, y + m_doc->m_style->m_barlineWidth);
    }
    
}
    
void View::DrawVerse( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    Verse *verse = dynamic_cast<Verse*>(element);
    
    dc->StartGraphic( verse, "", verse->GetUuid() );
    
    DrawLayerChildren(dc, verse, layer, staff, measure);
    
    dc->EndGraphic( verse, this );
}

    
void View::DrawKeySig( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"

    KeySig *ks = dynamic_cast<KeySig*>(element);
    
    if (ks->GetAlterationNumber()==0) {
        return;
    }
    
    int symb;
    int x, y;
    
    Clef *c = layer->GetClef(element);
    if (!c) {
        return;
    }
    
    dc->StartGraphic( element, "", element->GetUuid() );
    
    for (int i = 0; i < ks->GetAlterationNumber(); i++) {
        
        // HARDCODED
        x = element->GetDrawingX() + (m_doc->m_drawingAccidWidth[staff->staffSize][0] * 1.2) * i;
        y = staff->GetDrawingY() + CalculatePitchPosY( staff, ks->GetAlterationAt(i), layer->GetClefOffset( element ), ks->GetOctave(ks->GetAlterationAt(i), c->GetClefId()));;
        
        if (ks->GetAlteration() == ACCID_FLAT)
            symb = SMUFL_E260_accidentalFlat;
        else
            symb = SMUFL_E262_accidentalSharp;
        
        DrawSmuflCode ( dc, x, y, symb,  staff->staffSize, false );
    }
    
    dc->EndGraphic(element, this ); //RZ
    
}

void View::DrawTie( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<Tie*>(element) || dynamic_cast<Slur*>(element)); // The element must be a tie
    
    bool up = false;
    LayerElement *note1 = NULL;
    LayerElement *note2 = NULL;
    
    // coordinates for the tie/slur
    int x1, x2, y1, y2;
    up = true;
    data_STEMDIRECTION noteStemDir = STEMDIRECTION_NONE;
    
    Tie *tie = dynamic_cast<Tie*>(element);
    Slur *slur = dynamic_cast<Slur*>(element);
    if ( tie ) {
        note1 = tie->GetStart();
        note2 = tie->GetEnd();
    } else if ( slur ) {
        note1 = slur->GetStart();
        note2 = slur->GetEnd();
    }
    
    if ( !note1 && !note2 ) {
        // no note, obviously nothing to do...
        return;
    }

    System *currentSystem = dynamic_cast<System*>( staff->GetFirstParent( &typeid(System) ) );
    System *parentSystem1 = NULL;
    System *parentSystem2 = NULL;
    
    // In order to know if we are drawing a normal tie or a tie splitted over two systems, we need
    // to look at the parent system of each note.
    if ( note1 ) {
        parentSystem1 = dynamic_cast<System*>( note1->GetFirstParent( &typeid(System) ) );
    }
    if ( note2 ) {
        parentSystem2 = dynamic_cast<System*>( note2->GetFirstParent( &typeid(System) ) );
    }

    // This is the case when the tie is split over two system of two pages.
    // In this case, we are now drawing its beginning to the end of the measure (i.e., the last aligner)
    if ( parentSystem2 && currentSystem && ( parentSystem2 != currentSystem) ) {
        Alignment *nextAlignement = note1->GetAlignment();
        while (nextAlignement) {
            if (nextAlignement->GetType() == ALIGNMENT_MEASURE_END) break;
            nextAlignement = dynamic_cast<Alignment*>(nextAlignement->GetNextSibling());
        }
        if (!nextAlignement) {
            return;
        }
        y1 = y2 = note1->GetDrawingY();
        x1 = note1->GetDrawingX();
        x2 = measure->GetDrawingX() + nextAlignement->GetXRel();
        assert(dynamic_cast<Note*>(note1));
        noteStemDir = dynamic_cast<Note*>(note1)->m_drawingStemDir;
        //DrawTieOrSlurBezier(dc, note1->GetDrawingX(), y - 14, x2, y - 14, true);
    }
    // Now this is the case when the tie is split but we are drawing the end of it
    else if ( parentSystem1 && currentSystem && ( parentSystem1 != currentSystem) ) {
        Alignment *previousAlignement = note2->GetAlignment();
        while (previousAlignement) {
            if (previousAlignement->GetType() != ALIGNMENT_DEFAULT) break;
            previousAlignement = dynamic_cast<Alignment*>(previousAlignement->GetPreviousSibling());
        }
        if (!previousAlignement) {
            return;
        }
        y1 = y2 = note2->GetDrawingY();
        x2 = note2->GetDrawingX();
        x1 = x2 - m_doc->m_drawingDoubleUnit[ staff->staffSize ];
        assert(dynamic_cast<Note*>(note2));
        noteStemDir = dynamic_cast<Note*>(note2)->m_drawingStemDir;
        //DrawTieOrSlurBezier(dc, x1, y - 14, note2->GetDrawingX(), y - 14, true);
    }
    // Finally the normal case
    else {
        assert( note1 && note2 );
        // Copied from DrawNote
        // We could use the stamDir information
        // but then we have to take in account (1) beams (2) stemmed and non stemmed notes tied together
        y1 = note1->GetDrawingY();
        y2 = note2->GetDrawingY();
        x1 = note1->GetDrawingX();
        x2 = note2->GetDrawingX();
        assert(dynamic_cast<Note*>(note1));
        // for now we only look at the first note - needs to be improved
        // m_drawingStemDir it not set properly in beam - needs to be fixed.
        noteStemDir = dynamic_cast<Note*>(note1)->m_drawingStemDir;
        /*
        assert(dynamic_cast<Note*>(note2));
        if (dynamic_cast<Note*>(note2)->m_drawingStemDir != noteStemDir) {
            LogDebug("Diverging stem directions (%d;%d)", noteStemDir, dynamic_cast<Note*>(note2)->m_drawingStemDir);
        }
        */
    }
    
    assert( dynamic_cast<Note*>(note1));
    if (noteStemDir == STEMDIRECTION_up) {
        up = false;
    }
    else if (noteStemDir == STEMDIRECTION_NONE) {
        // no information from the note stem directions, look at the position in the notes
        int center = staff->GetDrawingY() - m_doc->m_drawingDoubleUnit[staff->staffSize] * 2;
        up = (y1 > center) ? true : false;
    }
    
    // FIXME, take in account elements that can be netween notes, eg keys time etc
    // 20 height nice with 70, not nice with 50
    // Also remove HARDCODED values!
    if (up) {
        y1 += m_doc->m_drawingUnit[staff->staffSize] * 1.6;
        y2 += m_doc->m_drawingUnit[staff->staffSize] * 1.6;
    }
    else {
        y1 -= m_doc->m_drawingUnit[staff->staffSize] * 1.6;
        y2 -= m_doc->m_drawingUnit[staff->staffSize] * 1.6;
    }
    
    dc->StartGraphic( element, "", element->GetUuid() );
    dc->DeactivateGraphic();
    DrawTieOrSlurBezier(dc, x1, y1, x2, y2, !up);
    dc->ReactivateGraphic();
    dc->EndGraphic(element, this );

    
}

void View::DrawAcciaccaturaSlash(DeviceContext *dc, LayerElement *element) {
    
    Note *note = dynamic_cast<Note*>(element);
    
    if (note->GetActualDur() < DUR_8)
        return;
    
    dc->SetPen(AxBLACK, 2, AxSOLID);
    dc->SetBrush( AxBLACK, AxSOLID );
    
    // HARDCODED
    if (element->m_drawingStemDir)
        dc->DrawLine(element->m_drawingStemStart.x - 10, ToDeviceContextY(element->m_drawingStemStart.y + 10), element->m_drawingStemStart.x + 20, ToDeviceContextY(element->m_drawingStemStart.y + 40));
    else
        dc->DrawLine(element->m_drawingStemStart.x - 10, ToDeviceContextY(element->m_drawingStemStart.y - 10), element->m_drawingStemStart.x + 20, ToDeviceContextY(element->m_drawingStemStart.y - 40));
    
    dc->ResetPen();
    dc->ResetBrush();
}

/** Draws a fermata
 rest - the fermata is always above the stavv
 note - for breves and semibreves, only above the staff
      - for flagged notes, the fermata is on the side of the notehead
 */
void View::DrawFermata(DeviceContext *dc, LayerElement *element, Staff *staff) {
    int x, y;
    int emb_offset = 0; // if there is and embellishment, offset the note up
    
    // We position the fermata in the same horizontal pos. of th object
    // but it shoud be moved according to half of the fermata size
    x = element->GetDrawingX() - m_doc->m_drawingDoubleUnit[staff->staffSize];
    
    // First case, notes
    if (dynamic_cast<Note*>(element)) {
        Note *note = dynamic_cast<Note*>(element);
        
        /*
        // stem down or semibreve/longa, fermata up!
        if (!element->m_drawingStemDir && (note->m_dur != DUR_1 || note->m_dur != DUR_BR)) {
        */
            // only for up-fermatas, if there is a trill on the same note
            if (note->m_embellishment)
                emb_offset = m_doc->m_drawingDoubleUnit[staff->staffSize];
            
            // check that the notehead is in the staff.
            if ((element->GetDrawingY()) < staff->GetDrawingY())
                // in the staff, set the fermata 20 pixels above the last line (+ embellishment offset)
                y = staff->GetDrawingY() + m_doc->m_drawingUnit[staff->staffSize] + emb_offset;
            else
                // out of the staff, place the trill above the notehead
                y = (element->GetDrawingY()) + m_doc->m_drawingUnit[staff->staffSize] + emb_offset;
            
            // draw the up-fermata
            DrawSmuflCode ( dc, x, y, SMUFL_E4C0_fermataAbove, staff->staffSize, false );
        /*
        } else { // stem up fermata down
            
            // This works as above, only we check that the note head is not
            // UNDER the staff
            if ((element->GetDrawingY()) > (staff->GetDrawingY() - m_doc->m_drawingStaffSize[staff->staffSize]))
                // notehead in staff, set  under
                y = staff->GetDrawingY() - m_doc->m_drawingStaffSize[staff->staffSize] - m_doc->m_drawingDoubleUnit[staff->staffSize];
            else
                // notehead under staff, set under notehead
                y = (element->GetDrawingY()) - m_doc->m_drawingDoubleUnit[staff->staffSize];
            
            DrawSmuflCode ( dc, x, y, LEIPZIG_FERMATA_DOWN, staff, false );
        }
        */
    } else if (dynamic_cast<Rest*>(element)) {
        // this is a rest
        // rests for the moment are always in the staff
        // so just place the fermata above the staff
        y = staff->GetDrawingY() + m_doc->m_drawingDoubleUnit[staff->staffSize];
        DrawSmuflCode ( dc, x, y, SMUFL_E4C0_fermataAbove, staff->staffSize, false );
    }
}

// Draw a trill above the notehead
// This function works as the up-fermata portion of DrawFermata
// if there are many symbols to draw we could make a generalized function
void View::DrawTrill(DeviceContext *dc, LayerElement *element, Staff *staff) {
    int x, y;    

    // It shoud be moved according to half of the trill size
    x = element->GetDrawingX() - m_doc->m_drawingAccidWidth[staff->staffSize][element->m_cueSize];

    // HARDCODED
    if ((element->GetDrawingY()) < staff->GetDrawingY())
        y = staff->GetDrawingY() + m_doc->m_drawingDoubleUnit[staff->staffSize];
    else
        y = (element->GetDrawingY()) + m_doc->m_drawingDoubleUnit[staff->staffSize];
    
    DrawSmuflCode ( dc, x, y, SMUFL_E566_ornamentTrill, staff->staffSize, false );
}

} // namespace vrv    
    
