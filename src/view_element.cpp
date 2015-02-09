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

#include "att_comparison.h"
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

int View::s_drawingLigX[2], View::s_drawingLigY[2];	// pour garder coord. des ligatures    
bool View::s_drawingLigObliqua = false;	// marque le 1e passage pour une oblique

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
    
    // Here we set the appropriate x value to be used for drawing
    // With Raw documents, we use m_drawingXRel that is calculated by the layout algorithm
    // With Transcription documents, we use the m_xAbs
    if ( element->m_xAbs == VRV_UNSET ) {
        assert( m_doc->GetType() == Raw );
        element->SetDrawingX( element->GetXRel() + measure->GetDrawingX() );
        element->SetDrawingY( staff->GetDrawingY() );
    }
    else
    {
        assert( m_doc->GetType() == Transcription );
        element->SetDrawingX( element->m_xAbs );
        element->SetDrawingY( staff->GetDrawingY() );
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

    DurationInterface *durElement = dynamic_cast<DurationInterface*>(element);
	if ( !durElement )
		return;
        
	if (dynamic_cast<Chord*>(element))
    {
        dc->StartGraphic( element, "", element->GetUuid() );
        DrawChord(dc, element, layer, staff, measure);
        dc->EndGraphic(element, this );
    }
    else if (dynamic_cast<Note*>(element))
    {
        Note *note = dynamic_cast<Note*>(element);
        int oct = note->GetOct() - 4;
        
        element->SetDrawingY( element->GetDrawingY() + CalculatePitchPosY( staff, note->GetPname(), layer->GetClefOffset( element ), oct ) );
        dc->StartGraphic( element, "", element->GetUuid() );
        DrawNote(dc, element, layer, staff, measure);
        dc->EndGraphic(element, this );
	}
    else if (dynamic_cast<Rest*>(element)) {
        Rest *rest = dynamic_cast<Rest*>(element);
        int oct = rest->GetOloc() - 4;
        
        // Automatically calculate rest position, if so requested
        if (rest->GetPloc() == PITCHNAME_NONE)
            element->SetDrawingY( element->GetDrawingY() + CalculateRestPosY( staff, rest->GetDur()) );
        else
            element->SetDrawingY( element->GetDrawingY() + CalculatePitchPosY( staff, rest->GetPloc(), layer->GetClefOffset( element ), oct) );
		
        dc->StartGraphic( element, "", element->GetUuid() );
        DrawRest( dc, element, layer, staff, measure );
        dc->EndGraphic(element, this );
	}
	
	return;
}

void View::DrawBeam(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure) {
    
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    
    Beam *beam = dynamic_cast<Beam*>(element);

    dc->StartGraphic( element, "", element->GetUuid() );
    
    DrawLayerChildren(dc, beam, layer, staff, measure);
    
    // Add to the list of postponed element 
    layer->AddToDrawingList( beam );
    
    dc->EndGraphic(element, this );
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
    int y1 = element->GetDrawingY();
    int xLedger, xNote, xAccid, xStem;
    int drawingDur;
    int staffY = staff->GetDrawingY();
    wchar_t fontNo;
	int ledge;
	int verticalCenter = 0;
    bool flippedNotehead = false;

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
    
    //set stem direction
    verticalCenter = staffY - m_doc->m_drawingDoubleUnit[staffSize]*2;
    if ( note->HasDrawingStemDir() ) {
        note->m_drawingStemDir = note->GetDrawingStemDir();
    }
    else if ( layer->GetDrawingStemDir() != STEMDIRECTION_NONE) {
        note->m_drawingStemDir = layer->GetDrawingStemDir();
    }
    else {
        note->m_drawingStemDir = (y1 >= verticalCenter) ? STEMDIRECTION_down : STEMDIRECTION_up;
    }
    
    //if the note is clustered, calculations are different
    if (note->m_cluster) {
        if (note->m_drawingStemDir == STEMDIRECTION_down) {
            //stem down/even cluster = noteheads start on left (incorrect side)
            if (note->m_cluster->size() % 2 == 0) {
                flippedNotehead = (note->m_clusterPosition % 2 == 0);
            }
            //else they start on normal side
            else {
                flippedNotehead = (note->m_clusterPosition % 2 != 0);
            }
            
            //if stem goes down, move ledger start to the left and expand it a full radius
            xLedger -= radius;
            ledge += radius;
        }
        else {
            //flipped noteheads start on normal side no matter what
            flippedNotehead = (note->m_clusterPosition % 2 != 0);
            
            //if stem goes up, move ledger start to the right and expand it a full radius
            xLedger += radius;
            ledge += radius;
        }
        
        //positions notehead
        if (flippedNotehead) {
            xNote = xStem - radius;
        }
        else {
            if (note->m_drawingStemDir == STEMDIRECTION_up) {
                xNote = xStem + radius;
            }
            else if (note->m_drawingStemDir == STEMDIRECTION_down) {
                xNote = xStem - radius * 3;
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

    // Long, breve and ligatures
	if (drawingDur == DUR_LG || drawingDur == DUR_BR || ((note->GetLig()!=LIGATURE_NONE) && drawingDur == DUR_1)) {
		DrawLigature ( dc, y1, element, layer, staff);
 	}
    // Whole notes
	else if (drawingDur == DUR_1) {
        if (note->GetColored()==BOOLEAN_true)
			fontNo = SMUFL_E0FA_noteheadWholeFilled;
		else
			fontNo = SMUFL_E0A2_noteheadWhole;

		DrawSmuflCode( dc, xNote, y1, fontNo, staff->staffSize, note->m_cueSize );
	}
    // Other values
	else {
        if ((note->GetColored()==BOOLEAN_true) || drawingDur == DUR_2) {
			fontNo = SMUFL_E0A3_noteheadHalf;
        }
        else {
			fontNo = SMUFL_E0A4_noteheadBlack;
        }
        
		DrawSmuflCode( dc, xNote, y1, fontNo,  staff->staffSize, note->m_cueSize );

		if (!(inBeam && drawingDur > DUR_4) && !inChord) {
            DrawStem(dc, note, staff, note->m_drawingStemDir, radius, xStem, y1);
        }

	}
    
	DrawLedgerLines( dc, y1, staffY, xLedger, ledge, staffSize );
    
	if (note->GetAccid() != ACCIDENTAL_EXPLICIT_NONE) {
		xAccid = xNote - 1.5 * m_doc->m_drawingAccidWidth[staffSize][note->m_cueSize];
		
        Accid accid;
        accid.SetOloc(note->GetOct());
        accid.SetPloc(note->GetPname());
		accid.SetAccid(note->GetAccid());
        accid.m_cueSize = note->m_cueSize;
        accid.SetDrawingX( xAccid );
        accid.SetDrawingY( y1 );
        DrawAccid( dc, &accid, layer, staff, measure ); // ax2
	}
	
    if (note->GetDrawingDots() && !inChord) {
        int xDot;
        if (note->GetDur() < DUR_2 || (note->GetDur() > DUR_8 && !inBeam && (note->m_drawingStemDir == STEMDIRECTION_up)))
            xDot = xStem + m_doc->m_drawingUnit[staffSize]*7/2;
        else
            xDot = xStem + m_doc->m_drawingUnit[staffSize]*5/2;
        
		PrepareDots( dc, xDot, y1, note->GetDrawingDots(), staff );
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
    int drawingDur = dynamic_cast<DurationInterface*>(object)->GetDur();
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
        object->m_drawingStemDir = true;
        
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
        object->m_drawingStemDir = false;
        
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



void View::DrawRest ( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{	
    assert(layer); // Pointer to layer cannot be NULL
    assert(staff); // Pointer to staff cannot be NULL
    assert(dynamic_cast<Rest*>(element)); // Element must be a Rest
        
    Rest *rest = dynamic_cast<Rest*>(element);

	int drawingDur = rest->GetDur();
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


void View::DrawLedgerLines( DeviceContext *dc, int y_n, int y_p, int xn, unsigned int smaller, int staffSize)
{
	int yn, ynt, yh, yb, test, v_decal = m_doc->m_drawingDoubleUnit[staffSize];
	int dist, xng, xnd;
	register int i;


	yh = y_p + m_doc->m_drawingUnit[staffSize];
    yb = y_p - m_doc->m_drawingStaffSize[staffSize]- m_doc->m_drawingUnit[staffSize];

	if (!is_in(y_n,yh,yb))                           // note hors-portee?
	{
		xng = xn - smaller;
		xnd = xn + smaller;

		dist = ((y_n > yh) ? (y_n - y_p) : y_p - m_doc->m_drawingStaffSize[staffSize] - y_n);
  		ynt = ((dist % m_doc->m_drawingDoubleUnit[staffSize] > 0) ? (dist - m_doc->m_drawingUnit[staffSize]) : dist);
		test = ynt/ m_doc->m_drawingDoubleUnit[staffSize];
		if (y_n > yh)
		{	yn = ynt + y_p;
			v_decal = - m_doc->m_drawingDoubleUnit[staffSize];
		}
		else
			yn = y_p - m_doc->m_drawingStaffSize[staffSize] - ynt;

		//hPen = (HPEN)SelectObject (hdc, CreatePen (PS_SOLID, _param.EpLignesPORTEE+1, workColor2));
		//xng = toZoom(xng);
		//xnd = toZoom(xnd);

        dc->SetPen( m_currentColour, ToDeviceContextX( m_doc->m_style->m_staffLineWidth ), AxSOLID );
        dc->SetBrush(m_currentColour , AxTRANSPARENT );

		for (i = 0; i < test; i++)
		{
			dc->DrawLine( ToDeviceContextX(xng) , ToDeviceContextY ( yn ) , ToDeviceContextX(xnd) , ToDeviceContextY ( yn ) );
			//h_line ( dc, xng, xnd, yn, _param.EpLignesPORTEE);
			//yh =  toZoom(yn);
			//MoveToEx (hdc, xng, yh, NULL);
			//LineTo (hdc, xnd, yh);

			yn += v_decal;
		}

        dc->ResetPen();
        dc->ResetBrush();
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

/** This function draws multi-measure rests
 **/
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
    int border = m_doc->m_drawingUnit[staff->staffSize] / 2;
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
    
	y2 = y1 + m_doc->m_drawingDoubleUnit[staff->staffSize]*2;
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
		PrepareDots( dc, (x2 + m_doc->m_drawingUnit[staff->staffSize]), y2, dots, staff);
}


void View::DrawQuarterRest ( DeviceContext *dc, int x, int y, int valeur, unsigned char dots, unsigned int smaller, Staff *staff)
{
    int y2 = y + m_doc->m_drawingDoubleUnit[staff->staffSize];
	DrawSmuflCode( dc, x, y2, SMUFL_E4E5_restQuarter + (valeur-DUR_4), staff->staffSize, smaller );
    
	if (dots)
	{	if (valeur < DUR_16)
			y += m_doc->m_drawingDoubleUnit[staff->staffSize];
		PrepareDots( dc, (x + 2 * m_doc->m_drawingDoubleUnit[staff->staffSize]), y, dots, staff);
	}
	return;
}

bool View::IsOnStaffLine ( int y, Staff *staff)
{
    return ((y - staff->GetDrawingY()) % m_doc->m_drawingDoubleUnit[staff->staffSize] == 0 );
}

void View::PrepareDots ( DeviceContext *dc, int x, int y, unsigned char dots, Staff *staff )
{
    if ( IsOnStaffLine(y, staff) ) {
        y += m_doc->m_drawingUnit[staff->staffSize];
    }
    DrawDots(dc, x, y, dots, staff);
}

void View::DrawDots ( DeviceContext *dc, int x, int y, unsigned char dots, Staff *staff)
{
	int i;
	for (i = 0; i < dots; i++) {
        DrawDot ( dc, x, y );
		x += std::max (6, 2 * m_doc->m_drawingUnit[staff->staffSize]);
	}
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
        //DrawBarline( dc, (System*)staff->m_parent, x,  m_doc->m_style->m_barlineWidth, barLine->m_onStaffOnly, staff);
    }
    

    
    dc->EndGraphic(element, this ); //RZ
}
    
void View::DrawChord( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    Chord* chord = dynamic_cast<Chord*>(element);
    
    int staffSize = staff->staffSize;
    int staffY = staff->GetDrawingY();
	int verticalCenter = staffY - m_doc->m_drawingDoubleUnit[staffSize]*2;
    int radius = m_doc->m_drawingNoteRadius[staffSize][chord->m_cueSize];
    int fullUnit = m_doc->m_drawingUnit[staffSize];
    int doubleUnit = fullUnit * 2;
    
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
    
    DrawLayerChildren(dc, chord, layer, staff, measure);
    
    //Easier to draw dots for the chord at once to make sure we've got one standard X-position for all the dots.
    if (chord->GetDots()) {
        int dots = chord->GetDots();
        int dotsX;
        if (chord->GetDur() < DUR_2 || (chord->GetDur() > DUR_8 && !inBeam && (chord->GetDrawingStemDir() == STEMDIRECTION_up))) {
            dotsX = chord->GetDrawingX() + (fullUnit * 7/2);
        }
        else {
            dotsX = chord->GetDrawingX() + (fullUnit * 5/2);
        }
        
        //Draw dots for notes that are not in clusters...
        for (ListOfObjects::iterator it = chord->GetList(chord)->begin(); it != chord->GetList(chord)->end(); it++)
        {
            Note *note = dynamic_cast<Note*>(*it);
            if (!note->m_cluster) {
                PrepareDots(dc, dotsX, note->GetDrawingY(), dots, staff);
            }
        }
        
        // Notes in clusters: If the stem points up, we have a note on the (incorrect) right side of the stem; add a note diameter to the dot positioning to avoid overlapping.
        if (chord->GetDrawingStemDir() == STEMDIRECTION_up) dotsX += (radius * 2);
        
        for(std::list<ChordCluster*>::iterator cit = chord->m_clusters.begin(); cit != chord->m_clusters.end(); cit++)
        {
            ChordCluster* cluster = *cit;
            int clusterSize = (int)cluster->size();
            
            //find beginning and ending Y-point
            Note* first = *(cluster->begin());
            Note* last = cluster->at(clusterSize - 1);
            int curY = first->GetDrawingY();
            int endY = last->GetDrawingY();
            
            //if either is on a staff line, expand it to the next space
            if (IsOnStaffLine(curY, staff)) curY -= fullUnit;
            if (IsOnStaffLine(endY, staff)) endY += fullUnit;
            //in the case of size = 3, we need one more dot on top for clarity
            else if (clusterSize == 3) endY += doubleUnit;
            
            //draw dots from one point to another
            do {
                DrawDots(dc, dotsX, curY, dots, staff);
                curY += doubleUnit;
            } while (curY <= endY);
        }
    }
    
    //if we're not in a beam, draw the stems
    int drawingDur = chord->GetDur();
    drawingDur = ((chord->GetColored()==BOOLEAN_true) && drawingDur > DUR_1) ? (drawingDur + 1) : drawingDur;
    
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

	a -= m_doc->m_drawingUnit[staff->staffSize] * 2;
	if (clef->m_cueSize)
		a += m_doc->m_drawingUnit[staff->staffSize];

	DrawSmuflCode ( dc, a, b, sym, staff->staffSize, clef->m_cueSize  );
   
    dc->EndGraphic(element, this ); //RZ
}

void View::DrawMensur( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<Mensur*>(element)); // Element must be a Mensur"

    Mensur *mensur = dynamic_cast<Mensur*>(element);
 
    dc->StartGraphic( element, "", element->GetUuid() );
	
	int x;

    if (mensur->GetSign()==MENSURATIONSIGN_O)
    {	
        DrawMensurCircle ( dc, element->GetDrawingX(), staff->GetDrawingY(), staff);
    }
    else if ((mensur->GetSign()==MENSURATIONSIGN_C) && (mensur->GetOrient()!=ORIENTATION_reversed))
    {	
        DrawMensurHalfCircle ( dc, element->GetDrawingX(), staff->GetDrawingY(), staff);
    }
    else if (mensur->GetSign()==MENSURATIONSIGN_C && mensur->GetOrient()==ORIENTATION_reversed)
    {	
        DrawMensurReversedHalfCircle ( dc, element->GetDrawingX(), staff->GetDrawingY(), staff);
    }
    if (mensur->GetSlash()) // we handle only one single slash
    {	
        DrawMensurSlash ( dc, element->GetDrawingX(), staff->GetDrawingY(), staff);
    }
    if (mensur->GetDot()) // we handle only one single dot
    {	
        DrawMensurDot (dc, element->GetDrawingX(), staff->GetDrawingY(), staff);
    }

	if (mensur->GetNum())
	{	
        x = element->GetDrawingX();
		if (mensur->GetSign())
        {
			x += m_doc->m_drawingUnit[staff->staffSize] * 5; // step forward because we have a sign or a meter symbol
        }
		DrawMensurFigures ( dc, x, staff->GetDrawingY(), mensur->GetNum(), mensur->GetNumbase(), staff);
	}
    
    dc->EndGraphic(element, this ); //RZ

}


void View::DrawMensurCircle( DeviceContext *dc, int x, int yy, Staff *staff )
{
	assert( dc ); // DC cannot be NULL
	
	int y =  ToDeviceContextY (yy - m_doc->m_drawingDoubleUnit[ staff->staffSize ] * 2);
	int r = ToDeviceContextX( m_doc->m_drawingDoubleUnit[ staff->staffSize ]);

	int w = std::max( ToDeviceContextX(4), 2 );

    dc->SetPen( m_currentColour, w, AxSOLID );
    dc->SetBrush( m_currentColour, AxTRANSPARENT );

	dc->DrawCircle( ToDeviceContextX(x), y, r );

    dc->ResetPen();
    dc->ResetBrush();
}	

void View::DrawMensurHalfCircle( DeviceContext *dc, int x, int yy, Staff *staff )
{
	assert( dc ); // DC cannot be NULL

	int w = std::max( ToDeviceContextX(4), 2 );
    dc->SetPen( m_currentColour, w, AxSOLID );
    dc->SetBrush( m_currentColour, AxTRANSPARENT );

	int y =  ToDeviceContextY (yy - m_doc->m_drawingDoubleUnit[ staff->staffSize ]);
	int r = ToDeviceContextX( m_doc->m_drawingDoubleUnit[ staff->staffSize ]);

	x = ToDeviceContextX (x);
	x -= 3*r/3;

	dc->DrawEllipticArc( x, y, 2*r, 2*r, 70, 290 );
		
    dc->ResetPen();
    dc->ResetBrush();

	return;
}	

void View::DrawMensurReversedHalfCircle( DeviceContext *dc, int x, int yy, Staff *staff )
{	
	assert( dc ); // DC cannot be NULL

	int w = std::max( ToDeviceContextX(4), 2 );
    dc->SetPen( m_currentColour, w, AxSOLID );
    dc->SetBrush( m_currentColour, AxTRANSPARENT );

	int y =  ToDeviceContextY (yy - m_doc->m_drawingDoubleUnit[ staff->staffSize ]);
	int r = ToDeviceContextX( m_doc->m_drawingDoubleUnit[ staff->staffSize ] );

    // needs to be fixed
	x = ToDeviceContextX (x);
	x -= 4*r/3;

	dc->DrawEllipticArc( x, y, 2*r, 2*r, 250, 110 );
    
    dc->ResetPen();
    dc->ResetBrush();

	return;
}	

void View::DrawMensurDot ( DeviceContext *dc, int x, int yy, Staff *staff )
{
	assert( dc ); // DC cannot be NULL

	int y =  ToDeviceContextY (yy - m_doc->m_drawingDoubleUnit[ staff->staffSize ] * 2);
	int r = std::max( ToDeviceContextX(4), 2 );
	
    dc->SetPen( m_currentColour, 1, AxSOLID );
    dc->SetBrush( m_currentColour, AxSOLID );

	dc->DrawCircle( ToDeviceContextX(x) -r/2 , y, r );
		
    dc->ResetPen();
    dc->ResetBrush();

	return;
}	


void View::DrawMensurSlash ( DeviceContext *dc, int a, int yy, Staff *staff )
{	
	assert( dc ); // DC cannot be NULL
	
	int y1 = yy;
	int y2 = y1 - m_doc->m_drawingStaffSize[ staff->staffSize ];
	
	DrawVerticalLine ( dc, y1, y2, a, 3);
	return;
}	


void View::DrawMensurFigures( DeviceContext *dc, int x, int y, int num, int numBase, Staff *staff)
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

    if (meterSig->GetCount())
    {	
        DrawMensurFigures ( dc, x, staff->GetDrawingY(), meterSig->GetCount(), meterSig->GetUnit(), staff);
    }
    
    dc->EndGraphic(element, this );
    
}

void View::DrawAccid( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<Accid*>(element)); // Element must be a Symbol"
    
    Accid *accid = dynamic_cast<Accid*>(element);
    dc->StartGraphic( element, "", element->GetUuid() );

    // Parent will be NULL if we are drawing a note @accid (see DrawNote) - the y value is already set
    if ( accid->m_parent ) {
        int oct = accid->GetOloc() - 4;
        element->SetDrawingY( element->GetDrawingY() + CalculatePitchPosY( staff, accid->GetPloc(), layer->GetClefOffset( accid ), oct) );
    }
    
    int x = element->GetDrawingX();
    int y = element->GetDrawingY();
    
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


void View::DrawCustos( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<Custos*>(element)); // Element must be a Symbol"
    
    Custos *custos = dynamic_cast<Custos*>(element);
    dc->StartGraphic( element, "", element->GetUuid() );
    
    int oct = custos->GetOloc() - 4;
    element->SetDrawingY( element->GetDrawingY() + CalculatePitchPosY( staff, custos->GetPloc(), layer->GetClefOffset( element ), oct) );

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
    
    int oct = dot->GetOloc() - 4;
    element->SetDrawingY( element->GetDrawingY() + CalculatePitchPosY( staff, dot->GetPloc(), layer->GetClefOffset( element ), oct) );
    
    int x = element->GetDrawingX();
    int y = element->GetDrawingY();

    DrawDot ( dc, x, y );
    
    dc->EndGraphic(element, this );

}
    
int View::GetSylY( Syl *syl, Staff *staff )
{
    assert( syl && staff );
    
    int y = syl->m_drawingFirstNote->GetDrawingY();
    if (staff->GetAlignment() ) {
        y = staff->GetDrawingY() + staff->GetAlignment()->GetMaxHeight() - syl->m_drawingVerse * m_doc->m_drawingUnit[staff->staffSize] * 4;
    }
    return y;
}
    
void View::DrawSyl( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    Syl *syl = dynamic_cast<Syl*>(element);
    
    if ( !syl->m_drawingFirstNote ) {
        LogDebug("Syl parent note was not found");
        return;
    }
    
    // to be updated
    syl->SetDrawingX( syl->m_drawingFirstNote->GetDrawingX() - m_doc->m_drawingUnit[staff->staffSize] * 2 );
    syl->SetDrawingY( GetSylY(syl, staff) );
    
    dc->StartGraphic( syl, "", syl->GetUuid() );
    
    DrawLyricString(dc, syl->GetDrawingX(), syl->GetDrawingY(), syl->GetText().c_str() );
    
    if (syl->m_drawingFirstNote && syl->m_drawingLastNote) {
        System *currentSystem = dynamic_cast<System*>( measure->GetFirstParent( &typeid(System) ) );
        // Postpone the drawing of the syl to the end of the system; this will call DrawSylConnector
        // that will look if the last note is in the same system (or not) and draw the connectors accordingly
        if (currentSystem) {
            currentSystem->AddToDrawingList(syl);
        }
    }
    
    dc->EndGraphic(syl, this );

}

void View::DrawSylConnector( DeviceContext *dc, Syl *syl, System *system )
{
    assert( syl->m_drawingFirstNote && syl->m_drawingLastNote);
    if ( !syl->m_drawingFirstNote || !syl->m_drawingLastNote) return;
    
    // Get the parent system of the first and last note
    System *parentSystem1 = dynamic_cast<System*>( syl->m_drawingFirstNote->GetFirstParent( &typeid(System) )  );
    System *parentSystem2 = dynamic_cast<System*>( syl->m_drawingLastNote->GetFirstParent( &typeid(System) )  );
    
    // The both correspond to the current system, which means no system break in-between (simple case)
    if (( system == parentSystem1 ) && ( system == parentSystem2 )) {
        // Get the parent staff for calculating the y position
        Staff *staff = dynamic_cast<Staff*>( syl->m_drawingFirstNote->GetFirstParent( &typeid(Staff) ) );
        if ( !Check( staff ) ) return;
        
        int y = GetSylY(syl, staff);
        // x1 is the end of the syl - very approximative, we should use GetTextExtend once implemented
        int x1 = syl->m_drawingFirstNote->GetDrawingX() + ((int)syl->GetText().length()) * m_doc->m_drawingLyricFonts[staff->staffSize].GetPointSize() / 3;
        int x2 = syl->m_drawingLastNote->GetDrawingX();
        
        // In this case we can resume the Syl because is was drawn previouly in the system
        dc->ResumeGraphic(syl, syl->GetUuid());
        dc->DeactivateGraphic();
        DrawSylConnectorLines( dc, x1, x2, y, syl, staff);
        dc->ReactivateGraphic();
        dc->EndResumedGraphic(syl, this);
    }
    // Only the first parent is the same, this means that the syl is "open" at the end of the system
    else if ( system == parentSystem1 ) {
        // We need the last measure of the system for x2
        Measure *last = dynamic_cast<Measure*>( system->FindChildByType( &typeid(Measure), 1, BACKWARD ) );
        if ( !Check( last ) ) return;
        Staff *staff = dynamic_cast<Staff*>( syl->m_drawingFirstNote->GetFirstParent( &typeid(Staff) ) );
        if ( !Check( staff ) ) return;
        
        int y = GetSylY(syl, staff);
        // x1 is the end of the syl - very approximative, we should use GetTextExtend once implemented
        int x1 = syl->m_drawingFirstNote->GetDrawingX() + ((int)syl->GetText().length()) * m_doc->m_drawingLyricFonts[staff->staffSize].GetPointSize() / 3;
        int x2 = last->GetDrawingX() + last->GetRightBarlineX();
        
        // In this case too we can resume the Syl because is was drawn previouly in the system
        dc->ResumeGraphic(syl, syl->GetUuid());
        dc->DeactivateGraphic();
        DrawSylConnectorLines( dc, x1, x2, y, syl, staff);
        dc->ReactivateGraphic();
        dc->EndResumedGraphic(syl, this);
        
    }
    // We are in the system of the last note - draw the connector from the beginning of the system
    else if ( system == parentSystem2 ) {
        // We need the first measure of the system for x1
        Measure *first = dynamic_cast<Measure*>( system->FindChildByType( &typeid(Measure), 1, FORWARD ) );
        if ( !Check( first ) ) return;
        Staff *staff = dynamic_cast<Staff*>( syl->m_drawingLastNote->GetFirstParent( &typeid(Staff) ) );
        if ( !Check( staff ) ) return;
        // Also try to get a first note - we should change this once we have a x position in measure that
        // takes into account the scoreDef
        Note *firstNote = dynamic_cast<Note*>( staff->FindChildByType( &typeid(Note) ) );
        
        int y = GetSylY(syl, staff);
        int x1 = firstNote ? firstNote->GetDrawingX() - 2 * m_doc->m_drawingDoubleUnit[staff->staffSize] : first->GetDrawingX();
        int x2 = syl->m_drawingLastNote->GetDrawingX();
        
        // In this case we can resume the last note because the Syl itself was _not_ drawn previouly in the system
        dc->ResumeGraphic(syl->m_drawingLastNote, syl->m_drawingLastNote->GetUuid());
        dc->DeactivateGraphic();
        DrawSylConnectorLines( dc, x1, x2, y, syl, staff);
        dc->ReactivateGraphic();
        dc->EndResumedGraphic(syl->m_drawingLastNote, this);
    }
    // Rare case where neither the first note and the last note are in the current system - draw the connector throughout the system
    else {
        // We need the first measure of the system for x1
        Measure *first = dynamic_cast<Measure*>( system->FindChildByType( &typeid(Measure), 1, FORWARD ) );
        if ( !Check( first ) ) return;
        // Also try to get a first note - we should change this once we have a x position in measure that
        // takes into account the scoreDef
        Note *firstNote = dynamic_cast<Note*>( first->FindChildByType( &typeid(Note) ) );
        // We need the last measure of the system for x2
        Measure *last = dynamic_cast<Measure*>( system->FindChildByType( &typeid(Measure), 1, BACKWARD ) );
        if ( !Check( last ) ) return;
        // Get the staff of the first note - however, not the staff we need
        Staff *firstStaff = dynamic_cast<Staff*>( syl->m_drawingFirstNote->GetFirstParent( &typeid(Staff) ) );
        if ( !Check( firstStaff ) ) return;
        
        // We need the staff from the current system, i.e., the first measure.
        AttCommonNComparison comparison( &typeid(Staff), firstStaff->GetN() );
        Staff *staff = dynamic_cast<Staff*>(first->FindChildByAttComparison(&comparison, 1));
        if (!staff ) {
            LogDebug("Could not get staff (%d) while drawing staffGrp - View::DrawSylConnector", firstStaff->GetN() );
            return;
        }
        
        int y = GetSylY(syl, staff);
        int x1 = firstNote ? firstNote->GetDrawingX() - 2 * m_doc->m_drawingDoubleUnit[staff->staffSize] : first->GetDrawingX();
        int x2 = last->GetDrawingX() + last->GetRightBarlineX();
        
        // In this case we cannot resume anything
        dc->DeactivateGraphic();
        DrawSylConnectorLines( dc, x1, x2, y, syl, staff);
        dc->ReactivateGraphic();
    }
    
}

void View::DrawSylConnectorLines( DeviceContext *dc, int x1, int x2, int y, Syl *syl, Staff *staff )
{
    if (syl->GetCon() == CON_d) {
        
        y += m_doc->m_drawingUnit[staff->staffSize] * 2 / 3;
        x2 -= 3 * (int)m_doc->m_drawingUnit[staff->staffSize];
        
        // the length of the dash and the space between them - can be made a parameter
        int dashLength = m_doc->m_drawingUnit[staff->staffSize] * 4 / 3;
        int dashSpace = m_doc->m_drawingStaffSize[staff->staffSize] * 5 / 3;
        
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
            DrawFullRectangle(dc, x, y, x + dashLength, y + m_doc->m_style->m_barlineWidth);
        }
        
    }
    else if (syl->GetCon() == CON_u) {
        DrawFullRectangle(dc, x1, y, x2, y + m_doc->m_style->m_barlineWidth);
    }
    
}
    
void View::DrawVerse( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    Verse *verse = dynamic_cast<Verse*>(element);
    
    DrawLayerChildren(dc, verse, layer, staff, measure);
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
    
    if (note->GetDur() < DUR_8)
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
        DrawSmuflCode ( dc, x, y, SMUFL_E4C1_fermataBelow, staff->staffSize, false );
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
    
