/////////////////////////////////////////////////////////////////////////////
// Name:        musrc_element.cpp
// Author:      Laurent Pugin and Chris Niven
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "view.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <sstream>
#include <typeinfo>

//----------------------------------------------------------------------------

#include "vrv.h"
#include "app.h"
#include "barline.h"
#include "beam.h"
#include "clef.h"
#include "doc.h"
#include "keysig.h"
#include "layerelement.h"
#include "leipzigbbox.h"
#include "measure.h"
#include "mensur.h"
#include "mrest.h"
#include "multirest.h"
#include "note.h"
#include "rest.h"
#include "symbol.h"
#include "staff.h"
#include "system.h"
#include "tie.h"
#include "tuplet.h"

namespace vrv {

//----------------------------------------------------------------------------
// View - LayerElement
//----------------------------------------------------------------------------

int View::s_drawingLigX[2], View::s_drawingLigY[2];	// pour garder coord. des ligatures    
bool View::s_drawingLigObliqua = false;	// marque le 1e passage pour une oblique

void View::DrawElement( DeviceContext *dc, LayerElement *element, Layer *layer, Measure *measure, Staff *staff )
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
    // With Raw documents, we use m_xRel that is calculated by the layout algorithm
    // With Transcription documents, we use the m_xAbs
    if ( element->m_xAbs == VRV_UNSET ) {
        assert( m_doc->GetType() == Raw );
        //element->m_xDrawing = element->m_xRel + measure->m_xDrawing;
        //element->m_xDrawing = element->m_xRel + measure->GetXRel();
        element->m_xDrawing = element->GetXRel() + measure->m_xDrawing;
    }
    else
    {
        assert( m_doc->GetType() == Transcription );
        element->m_xDrawing = element->m_xAbs;
    }
    
    if (dynamic_cast<Barline*>(element)) {
        DrawBarline(dc, element, layer, staff);
    }
    else if (dynamic_cast<Beam*>(element)) {
        DrawBeamElement(dc, element, layer, measure, staff);
    }
    else if (dynamic_cast<Clef*>(element)) {
        DrawClef(dc, element, layer, staff);
    }
    else if (dynamic_cast<KeySignature*>(element)) {
        DrawKeySig(dc, element, layer, staff);
    }
    else if (dynamic_cast<Mensur*>(element)) {
        DrawMensur(dc, element, layer, staff);
    }
    else if (dynamic_cast<MRest*>(element)) {
        DrawMRest(dc, element, layer, staff, measure);
    }
    else if (dynamic_cast<MultiRest*>(element)) {
        DrawMultiRest(dc, element, layer, staff, measure);
    }
    else if (dynamic_cast<Note*>(element)) {
        DrawDurationElement(dc, element, layer, staff);
    }
    else if (dynamic_cast<Rest*>(element)) {
        DrawDurationElement(dc, element, layer, staff);
    }
    else if (dynamic_cast<Symbol*>(element)) {
        DrawSymbol(dc, element, layer, staff);
    }
    else if (dynamic_cast<Tie*>(element)) {
        DrawTie(dc, element, layer, staff, measure);
    } 
    else if (dynamic_cast<Tuplet*>(element)) {
        DrawTupletElement(dc, element, layer, measure, staff);
    }
    else if (dynamic_cast<LayerApp*>(element)) {
        DrawLayerApp(dc, element, layer, measure, staff);
    }
    
    m_currentColour = previousColor;
}

void View::DrawDurationElement( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"

    DurationInterface *durElement = dynamic_cast<DurationInterface*>(element);
	if ( !durElement )
		return;
        
	
    if (dynamic_cast<Note*>(element)) 
    {
        Note *note = dynamic_cast<Note*>(element);
        int oct = note->m_oct - 4;
        
        //if ( !m_lyricMode && BelongsToTheNote( m_currentElement ) ) // the current element is a lyric that belongs to the note we are drawing
        //    m_currentColour = AxCYAN;
            
        dc->StartGraphic( element, "note", element->GetUuid() );
        element->m_yRel = CalculatePitchPosY( staff, note->m_pname, layer->GetClefOffset( element ), oct );
        
        if (!note->m_chord) // && (!pelement->ElemInvisible || illumine))
        {	
            DrawNote(dc, element, layer, staff);
        //else 
        //{
            //	note->accord(dc, staff);
        }
        dc->EndGraphic(element, this );
	}
    else if (dynamic_cast<Rest*>(element)) {
        Rest *rest = dynamic_cast<Rest*>(element);
        int oct = rest->m_oct - 4;

        dc->StartGraphic( element, "rest", element->GetUuid() );
        //if (!transp_sil)
		//	pnote->code = getSilencePitch (pelement);
        
        // Automatically calculate rest position, if so requested
        if (rest->m_pname == REST_AUTO)
            element->m_yRel = CalculateRestPosY( staff, rest->m_dur);
        else
            element->m_yRel = CalculatePitchPosY( staff, rest->m_pname, layer->GetClefOffset( element ), oct);
		
        DrawRest( dc, element, layer, staff );
        dc->EndGraphic(element, this );
	}
    
	/* 
	for ( int i = 0; i < (int)m_lyrics.GetCount(); i++ ){
		Symbol1 *lyric = &m_lyrics[i];
		
		if ( lyric != NULL )
		{
            if ( lyric == m_currentElement )
                m_currentColour = AxRED;
            else if ( (this == m_currentElement) || BelongsToTheNote( m_currentElement ) )
                m_currentColour = AxCYAN;

			putlyric(dc, lyric->m_xDrawing + staff->m_xDrawing, staff->m_yDrawing + lyric->dec_y , 
						  lyric->m_debord_str, staff->staffSize, ( lyric == m_currentElement && m_inputLyric ) );
		}		
	}
    */ // ax2 lyrics
	
	return;
}

void View::DrawBeamElement(DeviceContext *dc, LayerElement *element, Layer *layer, Measure *measure, Staff *staff) {
    
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    
    Beam *beam = dynamic_cast
    <Beam*>(element);

    dc->StartGraphic( element, "beam", element->GetUuid() );
    
    for (unsigned int i = 0; i < beam->m_children.size(); i++) {
        if ( dynamic_cast<LayerElement*>(beam->m_children[i]) ) {
            LayerElement *element = dynamic_cast<LayerElement*>(beam->m_children[i]);
            DrawElement(dc, element, layer, measure, staff);
        }
    }
    
    // Add to the list of postponed element 
    layer->AddToDrawingList( beam );
    
    dc->EndGraphic(element, this );
}

void View::DrawTupletElement(DeviceContext *dc, LayerElement *element, Layer *layer, Measure *measure, Staff *staff) {
    
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    
    Tuplet *tuplet = dynamic_cast<Tuplet*>(element);
    
    dc->StartGraphic( element, "tuplet", element->GetUuid() );
    
    // Draw the inner elements
    for (unsigned int i = 0; i < tuplet->m_children.size(); i++) {
        if ( dynamic_cast<LayerElement*>(tuplet->m_children[i]) ) {
            LayerElement *element = dynamic_cast<LayerElement*>(tuplet->m_children[i]);
            DrawElement(dc, element, layer, measure, staff);
        }
    }
    
    // Add to the list of postponed element
    layer->AddToDrawingList( tuplet );
    
    dc->EndGraphic(element, this );
}

// dessine la note en a,b+by. Calcule et dessine lignes addit. avec by=m_yDrawing
// b = decalage en fonction oct., clef, a partir du curseur; by = pos. curs.
// Accords: note doit connaitre le x non modifie par accord(), la fin de 
// l'accord (ptr_n->fchord), la valeur y extreme opposee au sommet de la
// queue: le ptr *testchord extern peut garder le x et l'y.


void View::DrawNote ( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff)
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<Note*>(element)); // Element must be a Note"
    
    Note *note = dynamic_cast<Note*>(element);
    
    bool inBeam = (note->GetFirstParent( &typeid( Beam ) ) != NULL );
    
	int staffSize = staff->staffSize;

	//	int horphyspoint=h_pnt;
	int b = element->m_yRel;
	int up=0, i, valdec, fontNo, ledge, queueCentre;
	int x1, x2, y2, espac7, decval, vertical;
	int formval = 0;	// pour permettre dessiner colorations avec dÇcalage de val
	int rayon, milieu = 0;

	int xn = element->m_xDrawing, xl = element->m_xDrawing;
	int bby = staff->m_yDrawing;  // bby= y sommet portee
	int ynn = element->m_yRel + staff->m_yDrawing;
	static int ynn_chrd;

	xn += note->m_hOffset;
	//val=note->m_dur;
	formval = (note->m_colored && note->m_dur > DUR_1) ? (note->m_dur+1) : note->m_dur;
	queueCentre = 0;


	rayon = m_doc->m_rendNoteRadius[staffSize][note->m_cueSize];

	if (note->m_dur > DUR_1 || (note->m_dur == DUR_1 && staff->notAnc))	// annuler provisoirement la modif. des lignes addit.
		ledge = m_doc->m_rendLedgerLine[staffSize][note->m_cueSize];
	else
	{	
        ledge= m_doc->m_rendLedgerLine[staffSize][2];
		rayon += rayon/3;
	}

	x1 = xn-rayon;	// position d'appel du caractäre et de la queue gauche
    xl = xn;

	// permettre d'inverser le cotÇ de la tete de note avec flag lat
	/*if (this->lat && !this->chord)
	{	if (this->queue)
		{	x1 = xn + rayon;
			xl = xn + rayon * 2;
		}
		else
		{	x1 = xn - rayon * 3;
			xl = xn - rayon * 2;
		}
	}*/ // ax2 - not support of lat

	DrawLedgerLines( dc, ynn, bby, xl, ledge, staffSize);	// dessin lignes additionnelles

	if (note->m_dur == DUR_LG || note->m_dur == DUR_BR || ((note->m_lig) && note->m_dur == DUR_1))	// dessin carrees
	{
		DrawLigature ( dc, ynn, element, layer, staff);
 	}
	else if (note->m_dur==DUR_1)
	{	
        if ( is_in (note->m_headshape, LOSANGEVIDE, OPTIONLIBRE))
			fontNo = LEIPZIG_OFFSET_NOTE_HEAD+note->m_headshape;
		else if (note->m_colored) // && !note->m_ligObliqua) // in WG, use of obliq for coloration? 
			fontNo = LEIPZIG_HEAD_WHOLE_FILLED;
		else
			fontNo = LEIPZIG_HEAD_WHOLE;

		DrawLeipzigFont( dc, x1, ynn, fontNo, staff, note->m_cueSize );
		decval = ynn;
	}
	else
	{	
        if ( is_in (note->m_headshape, LOSANGEVIDE, OPTIONLIBRE))
			fontNo = LEIPZIG_OFFSET_NOTE_HEAD+note->m_headshape;

		else if (note->m_colored || formval == DUR_2)
			fontNo = LEIPZIG_HEAD_HALF;
		else
			fontNo = LEIPZIG_HEAD_QUARTER;

		DrawLeipzigFont( dc,x1, ynn, fontNo, staff, note->m_cueSize );

		milieu = bby - m_doc->m_rendInterl[staffSize]*2;

// test ligne mediane pour direction queues: notation mesuree, milieu queue haut
		if (staff->notAnc)
			milieu +=  m_doc->m_rendHalfInterl[staffSize];

		if (note->m_chord) { /*** && this == testchord)***/
			ynn_chrd = ynn;
        }
		if (inBeam && formval > DUR_4)
        {
            // no stem
		}
		else if (note->m_headshape != SANSQUEUE && (!note->m_chord || (note->m_chord==CHORD_TERMINAL))) {	
            if (note->m_chord==CHORD_TERMINAL) {	
				/***up = testchord->obj.not.haste;
				xn = testchord->m_xDrawing;***/
			}
			else {
				//***up = this->q_auto ? ((ynn < milieu)? ON :OFF):this->queue;
				// ENZ
				up = (ynn > milieu) ? ON : OFF;
            }
			
			// ENZ
			if ( note->m_stemDir != 0 ) {
				up = (up == ON) ? OFF : ON;
            }
			
			espac7 = note->m_cueSize ? ( m_doc->m_rendHalfInterl[staffSize]*5) : ( m_doc->m_rendHalfInterl[staffSize]*7);
			vertical = note->m_cueSize ?  m_doc->m_rendHalfInterl[staffSize] :  m_doc->m_rendInterl[staffSize];
			decval = vertical * (valdec = formval-DUR_8);
			
			/***if (this->existDebord)	// queue longueur manuelle
				traiteQueue (&espac7, this);***/

			// diminuer le rayon de la moitie de l'epaisseur du trait de queue
			rayon -= (m_doc->m_env.m_stemWidth) / 2;

			if (!up) {	// si queue vers le bas (a gauche)
				espac7 = -espac7;
				decval = -decval;
				rayon = -rayon;
			}

			y2 = ((formval>DUR_8) ? (ynn + espac7 + decval) : (ynn + espac7));

			if ((note->m_chord==CHORD_INITIAL) || (note->m_chord==CHORD_MEDIAL)) {
				ynn = ynn_chrd;
            }

			/***if (this->q_auto)
			{	this->queue = (up > 0);
				if ( (y2 >= milieu && ynn > milieu) || (y2 <= milieu && ynn < milieu) )
				// note et queue du meme cote par rapport au centre de la portee
				{	y2 = milieu;
					queueCentre = 1;
				}
			}***/

			if (staff->notAnc)
				rayon = 0;
			x2 = xn + rayon;

			if (up)
			{
				if (formval > DUR_8 && !queueCentre)
				// Le 24 Septembre 1993. Correction esthetique pour rapprocher tailles 
				//   des DUR_8 et DUR_16 (longeur de queues trop inegales).
					y2 -= m_doc->m_rendHalfInterl[staffSize];
				decval = y2;
				if (staff->notAnc)
					v_bline ( dc,y2,(int)(ynn + m_doc->m_rendHalfInterl[staffSize]),x2, m_doc->m_env.m_stemWidth );//queue en descendant
				else
					v_bline ( dc,y2,(int)(ynn+ m_doc->m_rendVerticalUnit2[staffSize]),x2 - (m_doc->m_env.m_stemWidth / 2), m_doc->m_env.m_stemWidth );//queue en descendant
                
                element->m_stem_start.x = element->m_stem_end.x = x2 - (m_doc->m_env.m_stemWidth / 2);
                element->m_stem_end.y = y2;
                element->m_stem_start.y = (int)(ynn+ m_doc->m_rendVerticalUnit2[staffSize]);
                element->m_drawn_stem_dir = true;
                
				if (formval > DUR_4)
				{
                    y2 += m_doc->m_env.m_stemWidth / 2; // ENZO correction empirique...
					DrawLeipzigFont( dc,x2,y2,LEIPZIG_STEM_FLAG_UP, staff, note->m_cueSize );
					for (i=0; i < valdec; i++)
						DrawLeipzigFont( dc,x2,y2-=vertical,LEIPZIG_STEM_FLAG_UP, staff, note->m_cueSize );
				}
			}
			else
			{                
				if (formval > DUR_8 && !queueCentre)
				// Le 24 Septembre 1993. Correction esthetique pour rapprocher tailles 
				//   des DUR_8 et DUR_16 (longeur de queues trop inegales).
					y2 += m_doc->m_rendHalfInterl[staffSize];
				decval = y2;

				if (staff->notAnc)
					v_bline ( dc,y2,ynn- m_doc->m_rendHalfInterl[staffSize],x2 - (m_doc->m_env.m_stemWidth / 2), m_doc->m_env.m_stemWidth );//queue en descendant
				else
					v_bline ( dc,y2,(int)(ynn- m_doc->m_rendVerticalUnit2[staffSize]),x2 - (m_doc->m_env.m_stemWidth / 2), m_doc->m_env.m_stemWidth );	// queue en montant

                element->m_stem_start.x = element->m_stem_end.x = x2 - (m_doc->m_env.m_stemWidth / 2);
                element->m_stem_start.y = (int)(ynn- m_doc->m_rendVerticalUnit2[staffSize]);
                element->m_stem_end.y = y2;
                element->m_drawn_stem_dir = false;
                
				// ENZ
				// decalage du crochet vers la gauche
				// changement dans la fonte Leipzig 4.3 ‡ cause de problemes d'affichage
				// en deÁ‡ de 0 avec la notation ancienne
				// dans la fonte les crochets ont ete decales de 164 vers la droite
				int cr_offset = m_doc->m_rendNoteRadius[staffSize][note->m_cueSize]  + (m_doc->m_env.m_stemWidth / 2);
				if (formval > DUR_4)
				{
                    y2 -= m_doc->m_env.m_stemWidth / 2; // ENZO correction empirique...
					DrawLeipzigFont( dc,x2 - cr_offset,y2,LEIPZIG_STEM_FLAG_DOWN , staff, note->m_cueSize );
					for (i=0; i < valdec; i++)
						DrawLeipzigFont( dc,x2  - cr_offset,y2+=vertical,LEIPZIG_STEM_FLAG_DOWN, staff, 
									 note->m_cueSize );
				}
			}
            if (note->m_cueSize && note->m_acciaccatura)
                DrawAcciaccaturaSlash(dc, element);
		}	// fin de dessin queues et crochets

	}
    
	DrawLedgerLines( dc, ynn, bby, xl, ledge, staffSize);

	if (note->m_slur[0])
	{	
        if (note->m_slur[0]==SLUR_TERMINAL)
		{	/***liaison = OFF;	// pour tests de beam...
			liais_note(hdc, this);***/
		}
		/***if (this->dliai && !attente)	premierLie = this;***/
	}
    
	if (note->m_accid) // && !this->accInvis) // ax2 no support invisible accidental yet
	{
		if (note->m_chord)
            {}/***x1 = x_acc_chrd (this,0);***/
		else
			x1 -= 1.5 * m_doc->m_rendAccidWidth[staffSize][note->m_cueSize];
		Symbol accid( SYMBOL_ACCID );
        accid.m_oct = note->m_oct;
        accid.m_pname = note->m_pname;
		accid.m_accid = note->m_accid;
        accid.m_xDrawing = x1;
        DrawSymbol( dc, &accid, layer, staff, element ); // ax2
	}
	if (note->m_chord)
	{	
        /***x2 = testchord->m_xDrawing + m_doc->m_rendStep2;
		if (this->haste)
		{	if (this->lat || (this->ptr_fe && this->ptr_fe->type==NOTE && this->ptr_fe->obj.not.lat)
				|| (this->ptr_pe && element->m_xDrawing==this->ptr_pe->m_xDrawing && this->ptr_pe->type==NOTE && this->ptr_pe->obj.not.lat
					&& this->dec_y - this->ptr_pe->dec_y < m_doc->m_rendInterl[staffSize]
					&& 0 != ((int)b % (int)m_doc->m_rendInterl[staffSize]))
				)
				x2 += m_doc->m_rendNoteRadius[staffSize][dimin] * 2;
		}*///
	}
	else
	{	if (note->m_dur < DUR_2 || (note->m_dur > DUR_8 && !inBeam && up))
			x2 = xn + m_doc->m_rendStep1*7/2;
		else
			x2 = xn + m_doc->m_rendStep1*5/2;

		//if (this->lat) // ax2 - no support of note head flip
        //    x2 += rayon*2;
	}

	if (note->m_dots) // && (!this->pointInvisible)) // ax2 - no support of invisible dots yet
	{
		DrawDots( dc,x2,b, 0, note->m_dots, staff );
	}
/*
	if (this->stacc && (!this->rel || !this->queue_lig))
	{
		if (!this->chord || ((!this->queue_lig && this == testchord) || (this->queue_lig && this->fchord )))
		{
			if (val > DUR_BR)
			{	if  (!this->queue_lig)
				{	if ((this->queue && !this->chord) || (this->chord && this->haste))
					{	b -= m_doc->m_rendInterl[staffSize];
						decval = -m_doc->m_rendInterl[staffSize];
					}
					else
					{	b += m_doc->m_rendInterl[staffSize];
						decval = 0;
					}

				}
				else	// tous les cas inversÇs par queue_lig
				{	b = decval-staff->m_yDrawing;
	
					if ((!this->queue && !this->chord) || (this->chord && !this->haste))
					{	b -= m_doc->m_rendInterl[staffSize];
						decval = -1;
						if (val <= DUR_1)
							decval = -m_doc->m_rendInterl[staffSize];

					}
					else
					{	b += m_doc->m_rendHalfInterl[staffSize];
						decval = 0;
						if (val <= DUR_1)
							b += m_doc->m_rendHalfInterl[staffSize];
					}
				}

			}
			else
				return;
			putStacc (hdc,xn,b,decval,this->typStac);
		}
	}
*/
	
	//temp debug code
//	m_currentColour = wxCYAN;
//	rect_plein2(dc, element->m_xDrawing - 3, ynn - 3, element->m_xDrawing + 3, ynn + 3);
//	LogDebug("xrel: %d, ynn: %d", element->m_xDrawing, ynn);
//	m_currentColour = wxBLACK;
	//temp debug code
    
    
    // Add the ties to the postponed drawing list
    if ( note->GetTieAttrInitial() ) {
        // normally, we add the tie from the terminal note,
        // however, when the notes are not on the same system (or page),
        // we need to draw them twice. For this reason, we look if the
        // parent system is the same or not. If not, we also add to the list
        // the tie from the inital note
        Note *noteTerminal = note->GetTieAttrInitial()->GetSecondNote();
        if ( noteTerminal ) {
            System *parentSystem1 = dynamic_cast<System*>( note->GetFirstParent( &typeid(System) ) );
            System *parentSystem2 = dynamic_cast<System*>( noteTerminal->GetFirstParent( &typeid(System) ) );
            if ( (parentSystem1 != parentSystem2) && parentSystem1 ) {
                layer->AddToDrawingList( note->GetTieAttrInitial() );
            }
        }
    }
    if ( note->GetTieAttrTerminal() ) {
        layer->AddToDrawingList( note->GetTieAttrTerminal() );
    }

	

    if (note->m_fermata)
        DrawFermata(dc, element, staff);

    if (note->m_embellishment == EMB_TRILL)
        DrawTrill(dc, element, staff);
    
	return;
}


void View::DrawRest ( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff )
{	
    assert(layer); // Pointer to layer cannot be NULL
    assert(staff); // Pointer to staff cannot be NULL
    assert(dynamic_cast<Rest*>(element)); // Element must be a Rest
        
    Rest *rest = dynamic_cast<Rest*>(element);

	int formval = rest->m_dur;
	int a = element->m_xDrawing + rest->m_hOffset;
    int b = element->m_yRel;

	//unsigned char dot = this->point;
	/*if (inv_val && (!this->oblique && formval > DUR_1 || this->oblique && formval > DUR_2))
		formval += 1;*/ // ax2 - no support of inv_val for rest - need to use @dur.ges

	if (rest->m_dur == VALSilSpec) // WG multi-rest? 
    {
		formval = DUR_1;
    }
	else if (formval > DUR_2)
    {
		a -= m_doc->m_rendNoteRadius[staff->staffSize][rest->m_cueSize];
    }

	if (formval == DUR_BR || formval == DUR_2) 
    {
		b -= 0; //m_doc->m_rendInterl[staff->staffSize]; // LP position des silences
    }

	if (formval == DUR_1)
	{	
        if (staff->portNbLine == 1) {
		// silences sur portee a une seule ligne
			b += m_doc->m_rendInterl[staff->staffSize];
		}
        else
        {
			//b += m_doc->m_rendInterl[staff->staffSize]*2; 
			b -= 0; //m_doc->m_rendInterl[staff->staffSize]*2;// LP positions des silences
        }
	}

	if (rest->m_dur == VALSilSpec && rest->m_multimeasure_dur > 1) // LP: not sure what is actually does...
    {
        LogError("Tried to set multi meausure in rest\n");
    }
	else
	{	
        switch (formval)
		{	
            case DUR_LG: DrawLongRest ( dc, a, b, staff); break;
			case DUR_BR: DrawBreveRest( dc, a, b, staff); break;
			case DUR_1:
			case DUR_2: DrawWholeRest ( dc, a, b, formval, rest->m_dots, rest->m_cueSize, staff); break;
			//case CUSTOS: s_nr ( dc, a, b - m_doc->m_rendHalfInterl[staff->staffSize] + 1, '#' - LEIPZIG_REST_QUARTER + DUR_4 , staff); break; // Now in Symbol
			default: DrawQuarterRest( dc, a, b, formval, rest->m_dots, rest->m_cueSize, staff);
		}
	}
    
    if(rest->m_fermata)
        DrawFermata(dc, element, staff);
    
	return;
}


void View::DrawLedgerLines( DeviceContext *dc, int y_n, int y_p, int xn, unsigned int smaller, int staffSize)
{
	int yn, ynt, yh, yb, test, v_decal = m_doc->m_rendInterl[staffSize];
	int dist, xng, xnd;
	register int i;


	yh = y_p + m_doc->m_rendHalfInterl[staffSize];
    yb = y_p - m_doc->m_rendStaffSize[staffSize]- m_doc->m_rendHalfInterl[staffSize];

	if (!is_in(y_n,yh,yb))                           // note hors-portee?
	{
		xng = xn - smaller;
		xnd = xn + smaller;

		dist = ((y_n > yh) ? (y_n - y_p) : y_p - m_doc->m_rendStaffSize[staffSize] - y_n);
  		ynt = ((dist % m_doc->m_rendInterl[staffSize] > 0) ? (dist - m_doc->m_rendHalfInterl[staffSize]) : dist);
		test = ynt/ m_doc->m_rendInterl[staffSize];
		if (y_n > yh)
		{	yn = ynt + y_p;
			v_decal = - m_doc->m_rendInterl[staffSize];
		}
		else
			yn = y_p - m_doc->m_rendStaffSize[staffSize] - ynt;

		//hPen = (HPEN)SelectObject (hdc, CreatePen (PS_SOLID, _param.EpLignesPORTEE+1, workColor2));
		//xng = toZoom(xng);
		//xnd = toZoom(xnd);

        dc->SetPen( m_currentColour, ToRendererX( m_doc->m_env.m_staffLineWidth ), AxSOLID );
        dc->SetBrush(m_currentColour , AxTRANSPARENT );

		for (i = 0; i < test; i++)
		{
			dc->DrawLine( ToRendererX(xng) , ToRendererY ( yn ) , ToRendererX(xnd) , ToRendererY ( yn ) );
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
    
    MRest *mrest = dynamic_cast<MRest*>(element);
    
    dc->StartGraphic( element, "mrest", element->GetUuid() );
    
    int a = element->m_xDrawing + mrest->m_hOffset + measure->GetXRelRight() / 2;
    int b = element->m_yRel;
    
    // move it down according to the number of line in the staff
    b -= staff->portNbLine / 2 * m_doc->m_rendInterl[staff->staffSize];
    
    DrawWholeRest ( dc, a, b, DUR_1, 0, false, staff);
    
    dc->EndGraphic(element, this);
    
    return;
    
}

/** This function draws multi-measure rests
 **/
#define NUMBER_REDUCTION 5
void View::DrawMultiRest(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{	
    int x, x2, y, y2, length;

    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<MultiRest*>(element)); // Element must be a Symbol"
    
    MultiRest *multirest = dynamic_cast<MultiRest*>(element);
    
    dc->StartGraphic( element, "multirest", element->GetUuid() );
    
    int a = element->m_xDrawing + multirest->m_hOffset;
    
    // We do not support more than three chars
    if (multirest->GetNumber() > 999)
        multirest->SetNumber(999);
    
    // This is 1/2 the length of th black rectangle
	length = (m_doc->m_rendStep1 * 5);
    
    // Position centered in third line
    // it would be m_rendInterl * 6.5, or m_rendInterl / 2 * 13
	y = staff->m_yDrawing - (m_doc->m_rendInterl[staff->staffSize] / 2) * 5;
    y2 = y + m_doc->m_rendInterl[staff->staffSize];
	
    // a is the central point, claculate x and x2
    x = a - length;
    x2 = a + length;
    
    // Draw the base rect
    // make it 8 pixels smaller than the interline space
    // these are the two 4 substracted and added
	rect_plein2(dc, x, y2 - 4, x2, y + 4);
    
    //Draw the to lines at beginning and end
    // make it 8 pixesl longers, and 4 pixels width
    v_bline(dc, y - 4, y2 + 4, x, 4);
    v_bline(dc, y - 4, y2 + 4, x2, 4);
    
    // Draw the text above
    int w, h;
    unsigned int start_offset = 0; // offset from x to center text
    
    // convert to string
    std::stringstream text;
    text << multirest->GetNumber();
    
    dc->GetTextExtent( text.str(), &w, &h);
    start_offset = (x2 - x - w) / 2; // calculate offset to center text
    
    putstring(dc, x + start_offset, staff->m_yDrawing + 5, text.str(), false);
    
    dc->EndGraphic(element, this);
    
    return;

}

void View::DrawLongRest ( DeviceContext *dc, int a, int b, Staff *staff)

{	int x, x2, y = b + staff->m_yDrawing, y2;

	x = a; //- m_doc->m_rendStep1/3; 
	x2 = a+ (m_doc->m_rendStep1 *2 / 3); // LP
	if (b % m_doc->m_rendInterl[staff->staffSize])
		y -= m_doc->m_rendHalfInterl[staff->staffSize];
	y2 = y + m_doc->m_rendInterl[staff->staffSize]*2;
	rect_plein2( dc,x,y2,x2,y);
	return;
}


void View::DrawBreveRest ( DeviceContext *dc, int a, int b, Staff *staff)

{	int x, x2, y = b + staff->m_yDrawing, y2;

	x = a; //- m_doc->m_rendStep1/3; 
	x2 = a+ (m_doc->m_rendStep1 *2 / 3); // LP

	if (b % m_doc->m_rendInterl[staff->staffSize])
		y -= m_doc->m_rendHalfInterl[staff->staffSize];
	y2 = y + m_doc->m_rendInterl[staff->staffSize];
	rect_plein2 ( dc,x,y2,x2,y);
	x = a - m_doc->m_rendStep1; x2 = a + m_doc->m_rendStep1;

	h_bline ( dc, x,x2,y2,1);
	h_bline ( dc, x,x2,y, 1);
	return;
}

void View::DrawWholeRest ( DeviceContext *dc, int a, int b, int valeur, unsigned char dots, unsigned int smaller, Staff *staff)

{	int x, x2, y = b + staff->m_yDrawing, y2, vertic = m_doc->m_rendHalfInterl[staff->staffSize];
	int off;
	float foff;

	if (staff->notAnc)
		foff = (m_doc->m_rendStep1 *1 / 3);
	else
		foff = (m_doc->m_rendLedgerLine[staff->staffSize][2] * 2) / 3; // i.e., la moitie de la ronde

	if (smaller)
		foff *= (int)( (float)m_doc->m_rendGraceRatio[0] / (float)m_doc->m_rendGraceRatio[1] );
	off = (int)foff;

	x = a - off;
	x2 = a + off;

	if (valeur == DUR_1)
		vertic = -vertic;

	if (b % m_doc->m_rendInterl[staff->staffSize])
	{
		if (valeur == DUR_2)
			y -= vertic;
		else
			y += vertic;
	}

	y2 = y + vertic;
	rect_plein2 ( dc, x,y,x2,y2);

	off /= 2;
	x -= off;
	x2 += off;

	if (y > (int)staff->m_yDrawing  || y < (int)staff->m_yDrawing - m_doc->m_rendStaffSize[staff->staffSize])
		h_bline ( dc, x, x2, y, m_doc->m_env.m_staffLineWidth);

	if (dots)
		DrawDots ( dc,(x2 + m_doc->m_rendStep1), y2, -(int)staff->m_yDrawing, dots, staff);
}


void View::DrawQuarterRest ( DeviceContext *dc, int a, int b, int valeur, unsigned char dots, unsigned int smaller, Staff *staff)
{
	int _intrl = m_doc->m_rendInterl[staff->staffSize];

	DrawLeipzigFont( dc, a, (b + staff->m_yDrawing), LEIPZIG_REST_QUARTER + (valeur-DUR_4), staff, smaller );

	//DrawLeipzigFont( dc, a, (b + staff->m_yDrawing - m_doc->m_rendHalfInterl[staff->staffSize]), '#', staff, note->m_cueSize);

	if (dots)
	{	if (valeur >= DUR_16)
			_intrl = 0;
		DrawDots ( dc, (a+ m_doc->m_rendStep2), b, _intrl, dots, staff);
	}
	return;
}


void View::DrawDots ( DeviceContext *dc, int x1, int y1, int offy, unsigned char dots, Staff *staff )

{
	y1 += offy + staff->m_yDrawing;
    if ((y1 % (int)m_doc->m_rendInterl[staff->staffSize]) == 0) {
        y1 += m_doc->m_rendHalfInterl[staff->staffSize];
    }
    
	int i;
	for (i = 0; i < dots; i++) {
		DoDrawDot ( dc, x1, y1 );
		x1 += std::max (6, 2 * m_doc->m_rendStep1);
	}
	return;
}



void View::CalculateLigaturePosX ( LayerElement *element, Layer *layer, Staff *staff)
{
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
        element->m_xDrawing = previous->m_xDrawing + m_doc->m_rendBrevisWidth[staff->staffSize] * 2;
	}
    return;
}

void View::DrawLigature ( DeviceContext *dc, int y, LayerElement *element, Layer *layer, Staff *staff )
{	
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<Note*>(element)); // Element must be a Note"
    
    Note *note = dynamic_cast<Note*>(element);
    

	int xn, x1, x2, yy2, y1, y2, y3, y4, y5;
	int milieu, up, epaisseur;

	epaisseur = std::max (2, m_doc->m_rendBeamWidth[staff->staffSize]/2);
	xn = element->m_xDrawing;
	
	if ((note->m_lig==LIG_MEDIAL) || (note->m_lig==LIG_TERMINAL))
    {
		CalculateLigaturePosX ( element, layer, staff );
    }
	else {
		xn = element->m_xDrawing + note->m_hOffset;
    }


	// calcul des dimensions du rectangle
	x1 = xn - m_doc->m_rendBrevisWidth[staff->staffSize]; x2 = xn +  m_doc->m_rendBrevisWidth[staff->staffSize];
	y1 = y + m_doc->m_rendHalfInterl[staff->staffSize]; 
	y2 = y - m_doc->m_rendHalfInterl[staff->staffSize]; 
	y3 = (int)(y1 + m_doc->m_rendVerticalUnit1[staff->staffSize]);	// partie d'encadrement qui depasse
	y4 = (int)(y2 - m_doc->m_rendVerticalUnit1[staff->staffSize]);	

	if (!note->m_ligObliqua && (!View::s_drawingLigObliqua))	// notes rectangulaires, y c. en ligature
	{
		if ( !note->m_colored)
		{				//	double base des carrees
			hGrosseligne ( dc, x1,  y1,  x2,  y1, -epaisseur );
			hGrosseligne ( dc, x1,  y2,  x2,  y2, epaisseur );
		}
		else
			rect_plein2( dc,x1,y1,x2,y2);	// dessine val carree pleine // ENZ correction de x2

		v_bline ( dc, y3, y4, x1, m_doc->m_env.m_stemWidth );	// corset lateral
		v_bline ( dc, y3, y4, x2, m_doc->m_env.m_stemWidth );
	}
	else			// traitement des obliques
	{
		if (!View::s_drawingLigObliqua)	// 1e passage: ligne verticale initiale
		{
			v_bline (dc,y3,y4,x1, m_doc->m_env.m_stemWidth );
			View::s_drawingLigObliqua = true;
			//oblique = OFF;
//			if (val == DUR_1)	// queue gauche haut si DUR_1
//				queue_lig = ON;
		}
		else	// 2e passage: lignes obl. et verticale finale
		{
			x1 -=  m_doc->m_rendBrevisWidth[staff->staffSize]*2;	// avance auto

			y1 = *View::s_drawingLigY - m_doc->m_rendHalfInterl[staff->staffSize];	// ligat_y contient y original
			yy2 = y2;
			y5 = y1+ m_doc->m_rendInterl[staff->staffSize]; y2 += m_doc->m_rendInterl[staff->staffSize];	// on monte d'un INTERL

			if (note->m_colored)
				hGrosseligne ( dc,  x1,  y1,  x2,  yy2, m_doc->m_rendInterl[staff->staffSize]);
			else
			{	hGrosseligne ( dc,  x1,  y1,  x2,  yy2, 5);
				hGrosseligne ( dc,  x1,  y5,  x2,  y2, -5);
			}
			v_bline ( dc,y3,y4,x2,m_doc->m_env.m_stemWidth);	//cloture verticale

			View::s_drawingLigObliqua = false;
//			queue_lig = OFF;	//desamorce alg.queue DUR_BR

		}
	}

	if (note->m_lig)	// memoriser positions d'une note a l'autre; relier notes par barres
	{	
        *(View::s_drawingLigX+1) = x2; *(View::s_drawingLigY+1) = y;	// relie notes ligaturees par barres verticales
		//if (in(x1,(*View::s_drawingLigX)-2,(*View::s_drawingLigX)+2) || (this->fligat && this->lat && !Note1::marq_obl))
			// les dernieres conditions pour permettre ligature verticale ancienne
		//	v_bline (dc, *ligat_y, y1, (this->fligat && this->lat) ? x2: x1, m_doc->m_parameters.m_stemWidth); // ax2 - drawing vertical lines missing
		*View::s_drawingLigX = *(View::s_drawingLigX + 1);
		*View::s_drawingLigY = *(View::s_drawingLigY + 1);
	}

	
	y3 = y2 - m_doc->m_rendHalfInterl[staff->staffSize]*6;

	if (note->m_lig)
	{	
        if (note->m_dur == DUR_BR) //  && this->queue_lig)	// queue gauche bas: DUR_BR initiale descendante // ax2 - no support of queue_lig (see WG corrigeLigature)
		{
            v_bline ( dc, y2, y3, x1, m_doc->m_env.m_stemWidth );
        }
		else if (note->m_dur == DUR_LG) // && !this->queue_lig) // DUR_LG en ligature, queue droite bas // ax2 - no support of queue_lig
		{
            v_bline (dc, y2, y3, x2, m_doc->m_env.m_stemWidth );
        }
		else if (note->m_dur == DUR_1) // && this->queue_lig )	// queue gauche haut // ax2 - no support of queue_lig
		{	
            y2 = y1 + m_doc->m_rendHalfInterl[staff->staffSize]*6;
			v_bline ( dc, y1, y2, x1, m_doc->m_env.m_stemWidth );
		} 
	}
	else if (note->m_dur == DUR_LG)		// DUR_LG isolee: queue comme notes normales
	{	
		milieu = staff->m_yDrawing - m_doc->m_rendInterl[staff->staffSize]*6;
		//***up = this->q_auto ? ((y < milieu)? ON :OFF):this->queue;
		// ENZ
		up = (y < milieu) ? ON : OFF;
		// ENZ
		if ( !note->m_stemDir == 0 )
			up = (up == ON) ? OFF : ON;
			
		if (up)
		{	
            y3 = y1 + m_doc->m_rendHalfInterl[staff->staffSize]*6;
			y2 = y1;
		}
		v_bline ( dc, y2,y3,x2, m_doc->m_env.m_stemWidth );
	}

	return;
}

void View::DrawBarline( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(staff->m_parent); // Pointer to system cannot be NULL"
    assert(dynamic_cast<Barline*>(element)); // Element must be a Barline"
    
    Barline *barline = dynamic_cast<Barline*>(element);
    int x = element->m_xDrawing + barline->m_hOffset;

    dc->StartGraphic( element, "barline", element->GetUuid() );
    
    if (barline->m_partialBarline)
    {
        DrawPartialBarline ( dc, (System*)staff->m_parent, x, staff);
    }
    else
    {
        //DrawBarline( dc, (System*)staff->m_parent, x,  m_doc->m_env.m_barlineWidth, barline->m_onStaffOnly, staff);
    }
    

    
    dc->EndGraphic(element, this ); //RZ
}

void View::DrawClef( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<Clef*>(element)); // Element must be a Clef"
    
    Clef *clef = dynamic_cast<Clef*>(element);

    dc->StartGraphic( element, "clef", element->GetUuid() );
	
	int b = staff->m_yDrawing;
	int a = element->m_xDrawing;
    int sym = LEIPZIG_CLEF_G;	//sSOL, position d'ordre des cles sol fa ut in fonts

	if (staff->portNbLine > 5)
		b -= ((staff->portNbLine - 5) * 2) *m_doc->m_rendHalfInterl[ staff->staffSize ]; // LP: I am not sure it works with any number of lines

    /*  poser sym=no de position sSOL dans la fonte
     *	au depart; ne faire operation sur b qu'une fois pour cas semblables,
     *  et au palier commun superieur, incrementer sym, sans break.
     */
	switch(clef->m_clefId)
	{
		case UT1 : 
            sym += 2;
		case SOL1 : 
            b -= m_doc->m_rendStaffSize[ staff->staffSize ]; 
            break;
		case SOLva : 
            sym += 1;
		case UT2 : 
            sym += 2;
		case SOL2 : 
            b -= m_doc->m_rendInterl[ staff->staffSize ]*3; 
            break;
		case FA3 : 
            sym--;
		case UT3 : 
            b -= m_doc->m_rendInterl[ staff->staffSize ]*2; 
            sym += 2; 
            break;
		case FA5 : 
            sym++; 
            break;
		case FA4 : 
            sym--;
		case UT4 : 
            b -= m_doc->m_rendInterl[ staff->staffSize ];
		case UT5 :  
            sym += 2; 
            break;
		case CLEPERC :  
            b -= m_doc->m_rendInterl[ staff->staffSize ]*2;
            sym = LEIPZIG_CLEF_PERC; 
            break;
		default: 
            break;
	}

	a -= m_doc->m_rendStep1*2;
	if (clef->m_cueSize)
		a+= m_doc->m_rendStep1;

	DrawLeipzigFont ( dc, a, b, sym, staff, clef->m_cueSize  );
   
    dc->EndGraphic(element, this ); //RZ
}

void View::DrawMensur( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<Mensur*>(element)); // Element must be a Mensur"

    Mensur *mensur = dynamic_cast<Mensur*>(element);
 
    dc->StartGraphic( element, "mensur", element->GetUuid() );
	
	int x, yp;
	float mDen=1.0, mNum=1.0;

	if (mensur->m_meterSymb)
	{	
		yp = staff->m_yDrawing - (m_doc->m_rendHalfInterl[ staff->staffSize ]*6);
		
		unsigned char fontChar = LEIPZIG_METER_SYMB_COMMON;
		switch (mensur->m_meterSymb)
		{	
            case METER_SYMB_COMMON:
				fontChar = LEIPZIG_METER_SYMB_COMMON;
				mNum = 4; mDen = 4; break;
			case METER_SYMB_CUT:
				fontChar = LEIPZIG_METER_SYMB_CUT;
				mNum = 2; mDen = 2; break;
			case METER_SYMB_2:
				fontChar = LEIPZIG_METER_SYMB_2;
				mNum = 2; mDen = 2; break;
			case METER_SYMB_3:
				fontChar = LEIPZIG_METER_SYMB_3;
				mNum = 3; mDen = 2; break;
			case METER_SYMB_2_CUT:
				fontChar = LEIPZIG_METER_SYMB_2_CUT;	
				mNum = 4; mDen = 2; break;
			case METER_SYMB_3_CUT:
				fontChar = LEIPZIG_METER_SYMB_3_CUT;	
				mNum = 6; mDen = 2; break;
            default:
                break;
		}
		if ( dc )
		{	
			DrawLeipzigFont( dc, element->m_xDrawing, yp, fontChar, staff, staff->staffSize);
		}
	}
	else
	{
		if (mensur->m_sign==MENSUR_SIGN_O)
		{	
            mNum = 2; mDen = 2;
			DrawMensurCircle ( dc, element->m_xDrawing, staff->m_yDrawing, staff);
		}
		else if (mensur->m_sign==MENSUR_SIGN_C && !mensur->m_reversed)
		{	
            mNum = 2; mDen = 2;
			DrawMensurHalfCircle ( dc, element->m_xDrawing, staff->m_yDrawing, staff);
		}
		else if (mensur->m_sign==MENSUR_SIGN_C && mensur->m_reversed)
		{	
            mNum = 4; mDen = 2;
			DrawMensurReversedHalfCircle ( dc, element->m_xDrawing, staff->m_yDrawing, staff);
		}
		if (mensur->m_slash) // we handle only one single slash
		{	
            DrawMensurSlash ( dc, element->m_xDrawing, staff->m_yDrawing, staff);
			mDen = 1;
		}
		if (mensur->m_dot) // we handle only one single dot
		{	
            DrawMensurDot (dc, element->m_xDrawing, staff->m_yDrawing, staff);
			mNum = 9; mDen = 4;
		}
	}


	if (mensur->m_num)
	{	
        x = element->m_xDrawing;
		if (mensur->m_sign || mensur->m_meterSymb) 
        {
			x += m_doc->m_rendStep1*5; // step forward because we have a sign or a meter symbol
        }
		DrawMensurFigures ( dc, x, staff->m_yDrawing, mensur->m_num, mensur->m_numBase, staff);
		//mDen = max ( this->durDen, (unsigned short)1); // ax2
		//mNum = max ( this->durNum, (unsigned short)1); // ax2
	}
    /*
	MesVal = mNum / mDen;
	mesureNum = (int)mNum;
	mesureDen = (int)mDen;
    */ // ax2 
    
    dc->EndGraphic(element, this ); //RZ

}


void View::DrawMensurCircle( DeviceContext *dc, int x, int yy, Staff *staff )
{
	assert( dc ); // DC cannot be NULL
	
	int y =  ToRendererY (yy - m_doc->m_rendInterl[ staff->staffSize ] * 2);
	int r = ToRendererX( m_doc->m_rendInterl[ staff->staffSize ]);

	int w = std::max( ToRendererX(4), 2 );

    dc->SetPen( m_currentColour, w, AxSOLID );
    dc->SetBrush( m_currentColour, AxTRANSPARENT );

	dc->DrawCircle( ToRendererX(x), y, r );

    dc->ResetPen();
    dc->ResetBrush();
}	

void View::DrawMensurHalfCircle( DeviceContext *dc, int x, int yy, Staff *staff )
{
	assert( dc ); // DC cannot be NULL

	int w = std::max( ToRendererX(4), 2 );
    dc->SetPen( m_currentColour, w, AxSOLID );
    dc->SetBrush( m_currentColour, AxTRANSPARENT );

	int y =  ToRendererY (yy - m_doc->m_rendInterl[ staff->staffSize ]);
	int r = ToRendererX( m_doc->m_rendInterl[ staff->staffSize ]);

	x = ToRendererX (x);
	x -= 3*r/3;

	dc->DrawEllipticArc( x, y, 2*r, 2*r, 70, 290 );
		
    dc->ResetPen();
    dc->ResetBrush();

	return;
}	

void View::DrawMensurReversedHalfCircle( DeviceContext *dc, int x, int yy, Staff *staff )
{	
	assert( dc ); // DC cannot be NULL

	int w = std::max( ToRendererX(4), 2 );
    dc->SetPen( m_currentColour, w, AxSOLID );
    dc->SetBrush( m_currentColour, AxTRANSPARENT );

	int y =  ToRendererY (yy - m_doc->m_rendInterl[ staff->staffSize ]);
	int r = ToRendererX( m_doc->m_rendInterl[ staff->staffSize ] );

    // needs to be fixed
	x = ToRendererX (x);
	x -= 4*r/3;

	dc->DrawEllipticArc( x, y, 2*r, 2*r, 250, 110 );
    
    dc->ResetPen();
    dc->ResetBrush();

	return;
}	

void View::DrawMensurDot ( DeviceContext *dc, int x, int yy, Staff *staff )
{
	assert( dc ); // DC cannot be NULL

	int y =  ToRendererY (yy - m_doc->m_rendInterl[ staff->staffSize ] * 2);
	int r = std::max( ToRendererX(4), 2 );
	
    dc->SetPen( m_currentColour, 1, AxSOLID );
    dc->SetBrush( m_currentColour, AxSOLID );

	dc->DrawCircle( ToRendererX(x) -r/2 , y, r );
		
    dc->ResetPen();
    dc->ResetBrush();

	return;
}	


void View::DrawMensurSlash ( DeviceContext *dc, int a, int yy, Staff *staff )
{	
	assert( dc ); // DC cannot be NULL
	
	int y1 = yy;
	int y2 = y1 - m_doc->m_rendStaffSize[ staff->staffSize ];
	
	v_bline2 ( dc, y1, y2, a, 3);
	return;
}	


void View::DrawMensurFigures( DeviceContext *dc, int x, int y, int num, int numBase, Staff *staff)
{
	assert( dc ); // DC cannot be NULL	
    	
	int ynum, yden;
	std::string s;

	if (numBase)
	{	
		ynum = y - (m_doc->m_rendHalfInterl[staff->staffSize]*4);
		yden = ynum - (m_doc->m_rendInterl[staff->staffSize]*2);
	}
	else
		ynum = y - (m_doc->m_rendHalfInterl[staff->staffSize]*6);

	if (numBase > 9 || num > 9)	// avancer
		x += m_doc->m_rendStep1*2;

	s = StringFormat("%u",num);
	putstring ( dc, x, ynum, s, 1, staff->staffSize);	// '1' = centrer

	if (numBase)
	{
        s = StringFormat("%u",numBase);
		putstring ( dc, x, yden, s, 1, staff->staffSize);	// '1' = centrer
	}
	return;
}


void View::DrawSymbol( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, LayerElement *parent )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<Symbol*>(element)); // Element must be a Symbol"
    
    // This is used when we add dynamically an element (eg. accidentals before notes)
    // So we can get the clef without adding the new elem in the list
    LayerElement *list_elem = element;
    if (parent) list_elem = parent;
    
    Symbol *symbol = dynamic_cast<Symbol*>(element);
    int oct = symbol->m_oct - 4;
    element->m_yRel = CalculatePitchPosY( staff, symbol->m_pname, layer->GetClefOffset( list_elem ), oct);
    
    if (symbol->m_type==SYMBOL_ACCID) {
        DrawSymbolAccid(dc, element, layer, staff);
    }
    else if (symbol->m_type==SYMBOL_CUSTOS) {
        DrawSymbolCustos(dc, element, layer, staff);
    }
    else if (symbol->m_type==SYMBOL_DOT) {
        DrawSymbolDot(dc, element, layer, staff);
    }
    
}


void View::DrawSymbolAccid( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<Symbol*>(element)); // Element must be a Symbol"
    
    Symbol *accid = dynamic_cast<Symbol*>(element);
    dc->StartGraphic( element, "accid", element->GetUuid() );
    
    int x = element->m_xDrawing + accid->m_hOffset;
    int y = element->m_yRel + staff->m_yDrawing;
    
    int symc;
    switch (accid->m_accid)
    {	case ACCID_NATURAL :  symc = LEIPZIG_ACCID_NATURAL; break;
        //case ACCID_DOUBLE_SHARP : symc = LEIPZIG_ACCID_DOUBLE_SHARP; DrawLeipzigFont ( dc, x, y, symc, staff, accid->m_cueSize );
        // so far, double sharp (and flat) have been used for key signature. This is poor design and should be fixed
        case ACCID_DOUBLE_SHARP : symc = LEIPZIG_ACCID_SHARP; DrawLeipzigFont ( dc, x, y, symc, staff, accid->m_cueSize );    
                    y += 7*m_doc->m_rendHalfInterl[staff->staffSize]; // LP
        case ACCID_SHARP : symc = LEIPZIG_ACCID_SHARP; break;
        case ACCID_DOUBLE_FLAT :  symc = LEIPZIG_ACCID_FLAT; DrawLeipzigFont ( dc, x, y, symc, staff, accid->m_cueSize );
                    y += 7*m_doc->m_rendHalfInterl[staff->staffSize]; // LP
        case ACCID_FLAT :  symc = LEIPZIG_ACCID_FLAT; break;
        case ACCID_QUARTER_SHARP : symc = LEIPZIG_ACCID_QUARTER_SHARP; break;
        case ACCID_QUARTER_FLAT :  symc= LEIPZIG_ACCID_QUARTER_FLAT; break;
    }
    DrawLeipzigFont ( dc, x, y, symc, staff, accid->m_cueSize );

    
    dc->EndGraphic(element, this ); //RZ

}


void View::DrawSymbolCustos( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<Symbol*>(element)); // Element must be a Symbol"
    
    Symbol *custos = dynamic_cast<Symbol*>(element);
    dc->StartGraphic( element, "custos", element->GetUuid() );

    int x = element->m_xDrawing + custos->m_hOffset;
    int y = element->m_yRel + staff->m_yDrawing;
    y -= m_doc->m_rendHalfInterl[staff->staffSize] - m_doc->m_rendVerticalUnit2[staff->staffSize];  // LP - correction in 2.0.0
    
    DrawLeipzigFont( dc, x, y, 35, staff, custos->m_cueSize );
    
    dc->EndGraphic(element, this ); //RZ

}

void View::DrawSymbolDot( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<Symbol*>(element)); // Element must be a Symbol"
    
    Symbol *dot = dynamic_cast<Symbol*>(element);
    dc->StartGraphic( element, "dot", element->GetUuid() );
    
    int x = element->m_xDrawing + dot->m_hOffset;
    int y = element->m_yRel;

    switch (dot->m_dot)
    {	
        case 1 : DoDrawDot( dc, x, y ); x += std::max (6, m_doc->m_rendStep1);
        case 0 : DoDrawDot ( dc, x, y );
    }
    
    dc->EndGraphic(element, this );

}

void View::DrawKeySig( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"

    KeySignature *ks = dynamic_cast<KeySignature*>(element);
    int symb;
    int x, y;
    
    Clef *c = layer->GetClef(element);
    if (!c) {
        return;
    }
    
    dc->StartGraphic( element, "keysig", element->GetUuid() );
    
    for (int i = 0; i < ks->m_num_alter; i++) {
        
        element->m_yRel = CalculatePitchPosY( staff, ks->GetAlterationAt(i), layer->GetClefOffset( element ), ks->GetOctave(ks->GetAlterationAt(i), c->m_clefId));
        
        x = element->m_xDrawing + (m_doc->m_rendAccidWidth[staff->staffSize][0] + 5) * i;
        y = element->m_yRel + staff->m_yDrawing;
        
        if (ks->m_alteration == ACCID_FLAT)
            symb = LEIPZIG_ACCID_FLAT;
        else
            symb = LEIPZIG_ACCID_SHARP;
        
        DrawLeipzigFont ( dc, x, y, symb, staff, false );
    }
    
    dc->EndGraphic(element, this ); //RZ
    
}

void View::DrawTie( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(dynamic_cast<Tie*>(element)); // The element must be a tie
    
    bool up = false;
    
    Tie *tie = dynamic_cast<Tie*>(element);
    LayerElement *note1 = tie->GetFirstNote();
    LayerElement *note2 = tie->GetSecondNote();
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
        int y = note1->m_yRel + staff->m_yDrawing;
        int x2 = measure->m_xDrawing + nextAlignement->GetXRel();
        DrawTieBezier(dc, note1->m_xDrawing, y - 14, x2, true);
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
        int y = note2->m_yRel + staff->m_yDrawing;
        //int x1 = measure->m_xDrawing + previousAlignement->GetXRel() + previousAlignement->GetMaxWidth();
        // is it actually better for x1 just to have a fixed value
        int x1 = note2->m_xDrawing - m_doc->m_rendStep2;
        DrawTieBezier(dc, x1, y - 14, note2->m_xDrawing, true);
    }
    // Finally the normal case, but double check we have two notes
    else if ( note1 && note2 ) {
        
        // Copied from DrawNote
        // We could use the stamDir information
        // but then we have to take in account (1) beams (2) stemmed and non stemmed notes tied together
        int ynn = note1->m_yRel + staff->m_yDrawing;
        int bby = staff->m_yDrawing;
        int milieu = bby - m_doc->m_rendInterl[staff->staffSize] * 2;
        
        up = (ynn < milieu) ? true : false;
        
        dc->StartGraphic( element, "tie", element->GetUuid() );
        
        // FIXME, take in account elements that can be netween notes, eg keys time etc
        // 20 height nice with 70, not nice with 50
        if (up)
            DrawTieBezier(dc, note1->m_xDrawing, note1->m_yRel + staff->m_yDrawing - 14, note2->m_xDrawing, true);
        else
            DrawTieBezier(dc, note1->m_xDrawing, note1->m_yRel + staff->m_yDrawing + 14, note2->m_xDrawing, false);

        dc->EndGraphic(element, this );
    }
    
}

void View::DrawAcciaccaturaSlash(DeviceContext *dc, LayerElement *element) {
    
    Note *note = dynamic_cast<Note*>(element);
    
    if (note->m_dur < DUR_8)
        return;
    
    dc->SetPen(AxBLACK, 2, AxSOLID);
    dc->SetBrush( AxBLACK, AxSOLID );
    
    if (element->m_drawn_stem_dir)
        dc->DrawLine(element->m_stem_start.x - 10, ToRendererY(element->m_stem_start.y + 10), element->m_stem_start.x + 20, ToRendererY(element->m_stem_start.y + 40));
    else
        dc->DrawLine(element->m_stem_start.x - 10, ToRendererY(element->m_stem_start.y - 10), element->m_stem_start.x + 20, ToRendererY(element->m_stem_start.y - 40));
    
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
    
    // We position the fermata in the same horizontal pos. of th eobject
    x = element->m_xDrawing;
    
    // First case, notes
    if (dynamic_cast<Note*>(element)) {
        Note *note = dynamic_cast<Note*>(element);
        
        // stem down or semibreve/longa, fermata up!
        if (!element->m_drawn_stem_dir && (note->m_dur != DUR_1 || note->m_dur != DUR_BR)) {
            
            // only for up-fermatas, if there is a trill on the same note
            // add a 35 pixel space so they do not collide
            if (note->m_embellishment)
                emb_offset = 35;
            
            // check that the notehead is in the staff.
            if ((element->m_yRel + staff->m_yDrawing) < staff->m_yAbs)
                // in the staff, set the fermata 20 pixels above the last line (+ embellishment offset)
                y = staff->m_yAbs + 20 + emb_offset;
            else
                // out of the staff, place the trill 20 px above the notehead
                y = (element->m_yRel + staff->m_yDrawing) + 20 + emb_offset;
            
            // draw the up-fermata
            DrawLeipzigFont ( dc, element->m_xDrawing, y, LEIPZIG_FERMATA_UP, staff, false );
        } else { // stem up fermata down
            
            // This works as above, only we check that the note head is not
            // UNDER the staff
            if ((element->m_yRel + staff->m_yDrawing) > (staff->m_yDrawing - m_doc->m_rendStaffSize[staff->staffSize]))
                // notehead in staff, set at 20 px under
                y = staff->m_yDrawing - m_doc->m_rendStaffSize[staff->staffSize] - 20;
            else
                // notehead under staff, set 20 px under notehead
                y = (element->m_yRel + staff->m_yDrawing) - 20;
            
            DrawLeipzigFont ( dc, element->m_xDrawing, y, LEIPZIG_FERMATA_DOWN, staff, false );
        }
    } else if (dynamic_cast<Rest*>(element)) {
        // this is a rest
        // rests for the moment are always in the staff
        // so just place the fermata above the staff + 20 px
        y = staff->m_yAbs + 20;
        DrawLeipzigFont ( dc, element->m_xDrawing, y, LEIPZIG_FERMATA_UP, staff, false );
    }
}

// Draw a trill above the notehead
// This function works as the up-fermata portion of DrawFermata
// if there are many symbols to draw we could make a generalized function
void View::DrawTrill(DeviceContext *dc, LayerElement *element, Staff *staff) {
    int x, y;    
    x = element->m_xDrawing;

    if ((element->m_yRel + staff->m_yDrawing) < staff->m_yAbs)
        y = staff->m_yAbs + 30;
    else
        y = (element->m_yRel + staff->m_yDrawing) + 30;
    
    DrawLeipzigFont ( dc, element->m_xDrawing, y, LEIPZIG_EMB_TRILL, staff, false );
}


void View::DrawLayerApp( DeviceContext *dc, LayerElement *element, Layer *layer, Measure *measure, Staff *staff ){
    
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    assert(staff->m_parent); // Pointer to staff system cannot be NULL"
    
    LayerApp *app = dynamic_cast<LayerApp*>(element);    
    int i;
    for (i = 0; i < app->GetRdgCount(); i++ )
    {
        Layer *rdg = (Layer*)app->m_children[i];
        int j;
        for (j = 0; j < rdg->GetElementCount(); j++ ) {
            LayerElement *lelem = (LayerElement*)rdg->m_children[j];
            if (i == 0) {
                m_currentColour = AxGREEN;
            }
            else {
                m_currentColour = AxBLUE;
            }
            DrawElement(dc, lelem, layer, measure, staff );
            /*
            LayerElement rdgElement(&rdg->m_elements[j] );
            rdgElement.m_layer = element->m_layer;
            rdgElement.SetLayout( m_doc );
            rdgElement.m_xDrawing = element->m_xDrawing;
            DrawElement(dc, &rdgElement, layer, staff );
            rdgElement.m_layer = NULL;
            */
        }
        
        /*
        Staff *appStaff = new Staff( staff->m_n );
        appStaff->m_yDrawing = staff->m_yDrawing + m_doc->m_rendStaffSize[staff->staffSize];
        appStaff->m_system = staff->m_system;
        appStaff->SetLayout( m_doc );
        DrawStaff(dc, appStaff, staff->m_system );
        delete appStaff;
        */
    }
}

} // namespace vrv    
    
/*
 * Lyric code not refactored in ax2
    
void Note1::DeleteLyricFromNote( Symbol1 *lyric )
{
	for ( int i = 0; i < (int)this->m_lyrics.GetCount(); i++ ){
		Symbol1 *tmp = &this->m_lyrics[i];
		if ( lyric == tmp ){
			this->m_lyrics.RemoveAt( i );
			break;
		}
	}
}

Symbol1 *Note1::GetFirstLyric( )
{
	if ( this->m_lyrics.GetCount() == 0 ) 
		return NULL;
	else
		return &this->m_lyrics[0];
}

Symbol1 *Note1::GetLastLyric( )
{
	int num = this->m_lyrics.GetCount(); 
	if ( num == 0 ) 
		return NULL;
	else 
		return &this->m_lyrics[num-1];
}

Symbol1 *Note1::GetLyricNo( int no )
{
	int num = this->m_lyrics.GetCount(); 
	if ( (no < 0) || (num <= no) ) 
		return NULL;
	else 
		return &this->m_lyrics[no];
}

*/
