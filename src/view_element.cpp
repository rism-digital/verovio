/////////////////////////////////////////////////////////////////////////////
// Name:        view_element.cpp
// Author:      Laurent Pugin and Chris Niven
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "view.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <iostream>
#include <math.h>

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
#include "proport.h"
#include "rest.h"
#include "rpt.h"
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

void View::DrawLayerElement( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert( dc );
    assert( element );
    assert( layer );
    assert( staff );
    assert( measure );
    
    int previousColor = m_currentColour;

    if (element == m_currentElement) {
		m_currentColour = AxRED;
    }
    else {
        m_currentColour = AxBLACK;
    }

    if (element->Is() == ACCID) {
        DrawAccid(dc, element, layer, staff, measure);
    }
    else if (element->Is() == BARLINE) {
        DrawBarline(dc, element, layer, staff, measure);
    }
    else if (element->Is() == BEAM) {
        DrawBeam(dc, element, layer, staff, measure);
    }
    else if (element->Is() == BEATRPT) {
        DrawBeatRpt(dc, element, layer, staff, measure);
    }
    else if (element->Is() == BTREM) {
        DrawBTrem(dc, element, layer, staff, measure);
    }
    else if (element->Is() == CHORD) {
        DrawDurationElement(dc, element, layer, staff, measure);
    }
    else if (element->Is() == CLEF) {
        DrawClef(dc, element, layer, staff, measure);
    }
    else if (element->Is() == CUSTOS) {
        DrawCustos(dc, element, layer, staff, measure);
    }
    else if (element->Is() == DOT) {
        DrawDot(dc, element, layer, staff, measure);
    }
    else if (element->Is() == FTREM) {
        DrawFTrem(dc, element, layer, staff, measure);
    }
    else if (element->Is() == KEYSIG) {
        DrawKeySig(dc, element, layer, staff, measure);
    }
    else if (element->Is() == MENSUR) {
        DrawMensur(dc, element, layer, staff, measure);
    }
    else if (element->Is() == METERSIG) {
        DrawMeterSig(dc, element, layer, staff, measure);
    }
    else if (element->Is() == MREST) {
        DrawMRest(dc, element, layer, staff, measure);
    }
    else if (element->Is() == MRPT) {
        DrawMRpt(dc, element, layer, staff, measure);
    }
    else if (element->Is() == MRPT2) {
        DrawMRpt2(dc, element, layer, staff, measure);
    }
    else if (element->Is() == MULTIREST) {
        DrawMultiRest(dc, element, layer, staff, measure);
    }
    else if (element->Is() == MULTIRPT) {
        DrawMultiRpt(dc, element, layer, staff, measure);
    }
    else if (element->Is() == NOTE) {
        DrawDurationElement(dc, element, layer, staff, measure);
    }
    else if (element->Is() == PROPORT) {
        DrawProport(dc, element, layer, staff, measure);
    }
    else if (element->Is() == REST) {
        DrawDurationElement(dc, element, layer, staff, measure);
    }
    else if (element->Is() == SPACE) {
        DrawSpace(dc, element, layer, staff, measure);
    }
    else if (element->Is() == SYL) {
        DrawSyl(dc, element, layer, staff, measure);
    }
    else if (element->Is() == TUPLET) {
        DrawTuplet(dc, element, layer, staff, measure);
    }
    else if (element->Is() == VERSE) {
        DrawVerse(dc, element, layer, staff, measure);
    }
    else {
        // This should never happen
        LogError("Element '%s' cannot be drawn", element->GetClassName().c_str());
    }
    
    m_currentColour = previousColor;
}

//----------------------------------------------------------------------------
// View - LayerElement
//----------------------------------------------------------------------------

void View::DrawAccid( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure, Accid *prevAccid )
{
    assert( dc );
    assert( element );
    assert( layer );
    assert( staff );
    assert( measure );
    
    Accid *accid = dynamic_cast<Accid*>(element);
    assert( accid );
    
    dc->StartGraphic( element, "", element->GetUuid() );

    // Parent will be NULL if we are drawing a note @accid (see DrawNote) - the y value is already set
    if ( accid->m_parent ) {
        accid->SetDrawingY( accid->GetDrawingY() + CalculatePitchPosY( staff, accid->GetPloc(), layer->GetClefOffset( accid ), accid->GetOloc()) );
        accid->m_drawingCueSize = accid->IsCueSize();
    }
    
    /************** editorial accidental **************/
    
    if ( accid->GetFunc() == FUNC_edit ) {
        // position is currently only above the staff
        int y = staff->GetDrawingY();
        // look at the note position and adjust it if necessary
        Note *note = dynamic_cast<Note*>( accid->GetFirstParent( NOTE, MAX_ACCID_DEPTH ) );
        if ( note ) {
            if ( note->GetDrawingY() > y ) {
                y = note->GetDrawingY() + m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
            }
            if ( (note->GetDrawingStemDir() == STEMDIRECTION_up) && (note->GetDrawingStemEnd().y > y )) {
                y = note->GetDrawingStemEnd().y;
            }
            
            // adjust the x position so it is centered
            int radius = m_doc->GetGlyphWidth(SMUFL_E0A3_noteheadHalf, staff->m_drawingStaffSize, accid->m_drawingCueSize);
            if ( note->GetActualDur() < DUR_2 ) radius += radius/3;
            accid->SetDrawingX( accid->GetDrawingX() + radius / 2 );
        }
        accid->SetDrawingY( y + TEMP_STYLE_ACCID_EDIT_SPACE * m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / PARAM_DENOMINATOR );
    }
    
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
                    DrawSmuflCode ( dc, x, y, symc,  staff->m_drawingStaffSize, accid->m_drawingCueSize );
                    y += 7*m_doc->GetDrawingUnit(staff->m_drawingStaffSize); // LP
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
    
    DrawSmuflCode ( dc, x, y, symc, staff->m_drawingStaffSize, accid->m_drawingCueSize );

    dc->EndGraphic(element, this );
}

void View::DrawBarline( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert( dc );
    assert( element );
    assert( layer );
    assert( staff );
    assert( measure );
    
    Barline *barLine = dynamic_cast<Barline*>(element);
    assert( barLine );
    
    if (barLine->GetRend() == BARRENDITION_invis) {
        barLine->SetEmptyBB();
        return;
    }
    
    dc->StartGraphic( element, "", element->GetUuid() );
    
    int y = staff->GetDrawingY();
    DrawBarline( dc, y, y - m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize), barLine );
    
    dc->EndGraphic(element, this );
}
    
void View::DrawBeatRpt(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert( dc );
    assert( element );
    assert( layer );
    assert( staff );
    assert( measure );
    
    BeatRpt *beatRpt = dynamic_cast<BeatRpt*>(element);
    assert( beatRpt );
    
    dc->StartGraphic( element, "", element->GetUuid() );
    
    int x = element->GetDrawingX();
    int xSymbol = x;
    int y = element->GetDrawingY();
    y -= staff->m_drawingLines / 2 * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
    
    if (beatRpt->GetRend() == BEATRPT_REND_mixed) {
        DrawSmuflCode( dc, xSymbol, y, SMUFL_E501_repeat2Bars, staff->m_drawingStaffSize, false );
    }
    else {
        DrawSmuflCode( dc, xSymbol, y, SMUFL_E101_noteheadSlashHorizontalEnds, staff->m_drawingStaffSize, false );
        int additionalSlash = beatRpt->GetRend() - BEATRPT_REND_8;
        int halfWidth = m_doc->GetGlyphWidth(SMUFL_E101_noteheadSlashHorizontalEnds, staff->m_drawingStaffSize, false) / 2;
        int i;
        for (i = 0; i < additionalSlash; i++) {
            xSymbol += halfWidth;
            DrawSmuflCode( dc, xSymbol, y, SMUFL_E101_noteheadSlashHorizontalEnds, staff->m_drawingStaffSize, false );
        }
    }

    dc->EndGraphic(element, this);
}
    
void View::DrawBTrem(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert( dc );
    assert( element );
    assert( layer );
    assert( staff );
    assert( measure );
    
    BTrem *bTrem = dynamic_cast<BTrem*>(element);
    assert( bTrem );
    
    data_STEMDIRECTION stemDir = STEMDIRECTION_NONE;
    data_STEMMODIFIER stemMod;
    int drawingDur;
    LayerElement *childElement = NULL;
    Note *childNote = NULL;
    Chord *childChord = NULL;
    Point stemPoint;
    bool drawingCueSize = false;
    int x, y, yUnused;
    
    childChord = dynamic_cast<Chord*>(bTrem->FindChildByType(CHORD));
    // Get from the chord or note child
    if (childChord) {
        drawingDur = childChord->GetDur();
        childElement = childChord;
    }
    else {
        childNote = dynamic_cast<Note*>(bTrem->FindChildByType(NOTE));
        if (childNote) {
            drawingDur = childNote->GetDur();
            childElement = childNote;
        }
    }
    
    if (!childElement) {
        bTrem->SetEmptyBB();
        return;
    }
    
    dc->StartGraphic( element, "", element->GetUuid() );
    
    DrawLayerChildren(dc, bTrem, layer, staff, measure);
    
    // Get stem values from the chord or note child
    if (childChord) {
        stemDir = childChord->GetDrawingStemDir();
        stemMod = childChord->GetStemMod();
        stemPoint = childChord->GetDrawingStemStart();
    }
    else {
        drawingCueSize = childNote->IsCueSize();
        stemDir = childNote->GetDrawingStemDir();
        stemMod = childNote->GetStemMod();
        stemPoint = childNote->GetDrawingStemStart();
    }
    
    int beamWidthBlack = m_doc->GetDrawingBeamWidth(staff->m_drawingStaffSize, drawingCueSize);
    int beamWidthWhite = m_doc->GetDrawingBeamWhiteWidth(staff->m_drawingStaffSize, drawingCueSize);
    int width = m_doc->GetGlyphWidth(SMUFL_E0A3_noteheadHalf, staff->m_drawingStaffSize, drawingCueSize);
    int height = beamWidthBlack * 7 / 10;
    int step = height + beamWidthWhite;

    if (stemDir == STEMDIRECTION_up) {
        if (drawingDur > DUR_1) {
            y = childElement->GetDrawingTop(m_doc, staff->m_drawingStaffSize) - 3 * height;
            x = stemPoint.x;
        }
        else {
            if (childElement->Is() == NOTE) y = childElement->GetDrawingY();
            else childChord->GetYExtremes(&y, &yUnused);
            y += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 6;
            x = childElement->GetDrawingX();
        }
        step = -step;
    }
    else {
        if (drawingDur > DUR_1) {
            y = childElement->GetDrawingBottom(m_doc, staff->m_drawingStaffSize) + 1 * height;
            x = stemPoint.x + m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
        }
        else {
            if (childElement->Is() == NOTE) y = childElement->GetDrawingY();
            else childChord->GetYExtremes(&yUnused, &y);
            y -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 6;
            x = childElement->GetDrawingX();
        }
    }
    
    Beam *beam = childElement->IsInBeam();
    if (beam) {
        int beamStep = (drawingDur - DUR_8) * (beamWidthBlack + beamWidthWhite) + beamWidthWhite;
        y += (stemDir == STEMDIRECTION_down) ? beamStep : -beamStep;
    }
    
    int s;
    // by default draw 3 slashes (e.g., for a temolo on a whole note)
    if (stemMod == STEMMODIFIER_NONE) stemMod = STEMMODIFIER_3slash;
    for (s = 0; s < stemMod; s++) {
        DrawObliquePolygon (dc, x - width / 2, y, x + width / 2, y + height, height);
        y += step;
    }
    
    dc->EndGraphic(element, this);
}
    
void View::DrawChord( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert( dc );
    assert( element );
    assert( layer );
    assert( staff );
    assert( measure );
    
    Chord* chord = dynamic_cast<Chord*>(element);
    assert( chord );
    
    int staffSize = staff->m_drawingStaffSize;
    int staffY = staff->GetDrawingY();
	int verticalCenter = staffY - m_doc->GetDrawingDoubleUnit(staffSize)*2;
    bool drawingCueSize = chord->IsCueSize();
    int radius = m_doc->GetGlyphWidth(SMUFL_E0A3_noteheadHalf, staffSize, drawingCueSize) / 2;
    int fullUnit = m_doc->GetDrawingUnit(staffSize);
    
    Beam *inBeam = chord->IsInBeam();
    bool inFTrem = chord->IsInFTrem();
    
    /************ Ledger line reset ************/
    
    //if there are double-length lines, we only need to draw single-length after they've been drawn
    chord->m_drawingLedgerLines.clear();
    
    /************ Stems ************/
    
    int drawingDur = chord->GetDur();
    
    // (unless we're in a beam or in an fTrem)
    if (!inBeam && !inFTrem) {
        int yMax, yMin;
        chord->GetYExtremes(&yMax, &yMin);
        
        if (chord->HasStemDir()) {
            chord->SetDrawingStemDir(chord->GetStemDir());
        }
        else if (layer->GetDrawingStemDir() != STEMDIRECTION_NONE) {
            chord->SetDrawingStemDir(layer->GetDrawingStemDir());
        }
        else {
            chord->SetDrawingStemDir(yMax - verticalCenter >= verticalCenter - yMin ? STEMDIRECTION_down : STEMDIRECTION_up);
        }
        
        // (only shorter than whole notes)
        if (drawingDur > DUR_1) {
            int beamX = chord->GetDrawingX();
            int originY = ( chord->GetDrawingStemDir() == STEMDIRECTION_down ? yMax : yMin );
            int heightY = yMax - yMin;
            DrawStem(dc, chord, staff, chord->GetDrawingStemDir(), radius, beamX, originY, heightY);
        }
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
            Accid *curAccid = noteList.at(idx)->m_drawingAccid;

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
            fwIdx = accidClusterStarts.at(idx);
            //if this is the last cluster, set bwIdx to the last note in the chord
            if (idx == accidSize - 1) bwIdx = size - 1;
            //otherwise, set bwIdx to one before the beginning of the next cluster
            else bwIdx = accidClusterStarts.at(idx + 1) - 1;
            
            //if it's even, this will catch the overlap; if it's odd, there's an if in the middle there
            while (fwIdx <= bwIdx)
            {
                Accid *accidFwd = noteList.at(fwIdx)->m_drawingAccid;
                Accid *accidBwd = noteList.at(bwIdx)->m_drawingAccid;
                
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

    dc->SetPen( m_currentColour, ToDeviceContextX( m_doc->GetDrawingStaffLineWidth(staffSize) ), AxSOLID );
    dc->SetBrush(m_currentColour , AxTRANSPARENT );
    
    MapOfLedgerLineFlags::iterator iter;
    for(iter = chord->m_drawingLedgerLines.begin(); iter != chord->m_drawingLedgerLines.end(); iter++) {
        
        std::vector<char> *legerLines = &(*iter).second;
        
        //if there are double-length lines, we only need to draw single-length after they've been drawn
        (*legerLines).at(0) -= (*legerLines).at(1);
        (*legerLines).at(2) -= (*legerLines).at(3);
        
        //double-length lines below the staff
        DrawLedgerLines(dc, chord, (*iter).first, false, true, 0, (*legerLines).at(1));
        
        //remainder single-length lines below the staff
        DrawLedgerLines(dc, chord, (*iter).first, false, false, (*legerLines).at(1), (*legerLines).at(0));
        
        //double-length lines above the staff
        DrawLedgerLines(dc, chord, (*iter).first, true, true, 0, (*legerLines).at(3));
        
        //remainder single-length lines above the staff
        DrawLedgerLines(dc, chord, (*iter).first, true, false, (*legerLines).at(3), (*legerLines).at(2));

    }
    
    dc->ResetPen();
    dc->ResetBrush();
}

void View::DrawClef( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert( dc );
    assert( element );
    assert( layer );
    assert( staff );
    assert( measure );
    
    Clef *clef = dynamic_cast<Clef*>(element);
    assert( clef );

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
            b -= m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize);
            break;
		case G1 :
            b -= m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize);
            break;
		case G2_8va :
            sym = SMUFL_E053_gClef8va;
            b -= m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * 3;
            break;
        case G2_8vb :
            sym = SMUFL_E052_gClef8vb;
            b -= m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * 3;
            break;
		case C2 :
            sym = SMUFL_E05C_cClef;
            b -= m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * 3;
            break;
		case G2 :
            b -= m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * 3;
            break;
		case F3 :
            sym = SMUFL_E062_fClef;
            b -= m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize)*2;
            break;
		case C3 :
            sym = SMUFL_E05C_cClef;
            b -= m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize)*2;
            break;
		case F5 :
            sym =SMUFL_E062_fClef;
            break;
		case F4 :
            sym = SMUFL_E062_fClef;
            b -= m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
            break;
		case C4 :
            sym = SMUFL_E05C_cClef;
            b -= m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
            break;
		case C5 :
            sym = SMUFL_E05C_cClef;
            break;
		case perc :
            b -= m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * 2;
            // FIXME
            sym = SMUFL_E05C_cClef;
            break;
		default: 
            break;
	}

    bool cueSize = false;
    // force cue size for intermediate clefs
    if (clef->GetFirstParent( LAYER )) cueSize = true;
    
    //if (!cueSize)
    //    a -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2;
    
    DrawSmuflCode ( dc, a, b, sym, staff->m_drawingStaffSize, cueSize );
   
    dc->EndGraphic(element, this );
}

void View::DrawCustos( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert( dc );
    assert( element );
    assert( layer );
    assert( staff );
    assert( measure );
    
    Custos *custos = dynamic_cast<Custos*>(element);
    assert( custos );
    
    dc->StartGraphic( element, "", element->GetUuid() );
    
    element->SetDrawingY( element->GetDrawingY() + CalculatePitchPosY( staff, custos->GetPloc(), layer->GetClefOffset( element ), custos->GetOloc()) );

    int x = element->GetDrawingX();
    int y = element->GetDrawingY();
    
    y -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize) - m_doc->GetDrawingUnit(staff->m_drawingStaffSize)/4;  // LP - correction in 2.0.0
    
    // HARDCODED (smufl code wrong)
    DrawSmuflCode( dc, x, y, 35,  staff->m_drawingStaffSize, false );
    
    dc->EndGraphic(element, this );
}

void View::DrawDot( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert( dc );
    assert( element );
    assert( layer );
    assert( staff );
    assert( measure );
    
    Dot *dot = dynamic_cast<Dot*>(element);
    assert( dot );
    
    dc->StartGraphic( element, "", element->GetUuid() );
    
    element->SetDrawingY( element->GetDrawingY() + CalculatePitchPosY( staff, dot->GetPloc(), layer->GetClefOffset( element ), dot->GetOloc()) );
    
    int x = element->GetDrawingX();
    int y = element->GetDrawingY();
    
    // Use the note to which the points to for position
    if ( dot->m_drawingNote ) {
        x = dot->m_drawingNote->GetDrawingX() + m_doc->GetDrawingUnit(staff->m_drawingStaffSize)*7/2;
        y = dot->m_drawingNote->GetDrawingY();
    }

    DrawDots( dc, x, y, 1, staff );
    
    dc->EndGraphic(element, this );
}

void View::DrawDurationElement( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert( dc );
    assert( element );
    assert( layer );
    assert( staff );
    assert( measure );
    
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
}

void View::DrawKeySig( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert( dc );
    assert( element );
    assert( layer );
    assert( staff );
    assert( measure );

    KeySig *keySig = dynamic_cast<KeySig*>(element);
    assert( keySig );
    
    int symb;
    int x, y, i;
    
    Clef *c = layer->GetClef(element);
    if (!c) {
        keySig->SetEmptyBB();
        return;
    }
    
    // hidden key signature
    if (!keySig->m_drawingShow) {
        keySig->SetEmptyBB();
        return;
    }
    
    // C major (0) key sig and no cancellation
    else if ((keySig->GetAlterationNumber() == 0) && (keySig->m_drawingCancelAccidCount == 0)) {
        keySig->SetEmptyBB();
        return;
    }
    
    dc->StartGraphic( element, "", element->GetUuid() );
    
    x = element->GetDrawingX();
    // HARDCODED
    int step = m_doc->GetGlyphWidth(SMUFL_E262_accidentalSharp, staff->m_drawingStaffSize, false) * TEMP_STYLE_KEYSIG_STEP;
    
    // Show cancellation if C major (0) or if any cancellation and show cancellation (showchange) is true (false by default)
    if ( (keySig->GetAlterationNumber() == 0) || (layer->DrawKeySigCancellation() && keySig->m_drawingShowchange) ) {
        // The type of alteration is different (f/s or f/n or s/n) - cancel all accid in the normal order
        if (keySig->GetAlterationType() != keySig->m_drawingCancelAccidType) {
            for (i = 0; i < keySig->m_drawingCancelAccidCount; i++) {
                data_PITCHNAME pitch = KeySig::GetAlterationAt( keySig->m_drawingCancelAccidType, i);
                y = staff->GetDrawingY() + CalculatePitchPosY( staff, pitch, layer->GetClefOffset( element ),
                                            KeySig::GetOctave( keySig->m_drawingCancelAccidType, pitch, c->GetClefId()));;

                DrawSmuflCode ( dc, x, y, SMUFL_E261_accidentalNatural, staff->m_drawingStaffSize, false );   
                x += step;
            }
        }
        // Cancel some of them if more accid before
        else if (keySig->GetAlterationNumber() < keySig->m_drawingCancelAccidCount) {
            for (i = keySig->GetAlterationNumber(); i < keySig->m_drawingCancelAccidCount; i++) {
                data_PITCHNAME pitch = KeySig::GetAlterationAt( keySig->m_drawingCancelAccidType, i);
                y = staff->GetDrawingY() + CalculatePitchPosY( staff, pitch, layer->GetClefOffset( element ),
                                                              KeySig::GetOctave( keySig->m_drawingCancelAccidType, pitch, c->GetClefId()));;
                
                DrawSmuflCode ( dc, x, y, SMUFL_E261_accidentalNatural, staff->m_drawingStaffSize, false );
                x += step;
            }
        }
    }
    
    for (i = 0; i < keySig->GetAlterationNumber(); i++) {
        data_PITCHNAME pitch = KeySig::GetAlterationAt( keySig->GetAlterationType(), i);
        y = staff->GetDrawingY() + CalculatePitchPosY( staff, pitch, layer->GetClefOffset( element ),
                                    KeySig::GetOctave( keySig->GetAlterationType(), pitch, c->GetClefId()));;
        
        if (keySig->GetAlterationType() == ACCIDENTAL_EXPLICIT_f) symb = SMUFL_E260_accidentalFlat;
        else symb = SMUFL_E262_accidentalSharp;
        
        DrawSmuflCode ( dc, x, y, symb,  staff->m_drawingStaffSize, false );
        x += step;
    }
    
    dc->EndGraphic(element, this );
}

void View::DrawMeterSig( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert( dc );
    assert( element );
    assert( layer );
    assert( staff );
    assert( measure );
    
    MeterSig *meterSig = dynamic_cast<MeterSig*>(element);
    assert( meterSig );
    
    dc->StartGraphic( element, "", element->GetUuid() );
    
    int y = staff->GetDrawingY() - (m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 4);
    int x = element->GetDrawingX();
    
    if ( meterSig->GetSym() == METERSIGN_common ) {
        DrawSmuflCode( dc, element->GetDrawingX(), y, SMUFL_E08A_timeSigCommon, staff->m_drawingStaffSize, false);
        x += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 5; // step forward because we have a symbol
    }
    else if ( meterSig->GetSym() == METERSIGN_cut ) {
        DrawSmuflCode( dc, element->GetDrawingX(), y, SMUFL_E08B_timeSigCutCommon, staff->m_drawingStaffSize, false);
        x += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 5; // step forward because we have a symbol
    }
    else if (meterSig->GetCount())
    {	
        DrawMeterSigFigures ( dc, x, staff->GetDrawingY(), meterSig->GetCount(), meterSig->GetUnit(), staff);
    }
    
    dc->EndGraphic(element, this );
}

void View::DrawMRest(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert( dc );
    assert( element );
    assert( layer );
    assert( staff );
    assert( measure );
    
    assert( dynamic_cast<MRest*>(element) ); // Element must be a MultiRest"
    
    dc->StartGraphic( element, "", element->GetUuid() );
    
    int width = measure->GetRightBarlineX() - measure->GetNonJustifiableLeftMargin();
    int xCentered = measure->GetDrawingX() + measure->GetNonJustifiableLeftMargin() + (width / 2);
    int y = element->GetDrawingY();
    
    // move it down according to the number of line in the staff
    y -= staff->m_drawingLines / 2 * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
    
    DrawRestWhole ( dc, xCentered, y, DUR_1, 0, false, staff );
    
    dc->EndGraphic(element, this);
}

void View::DrawMRpt(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert( dc );
    assert( element );
    assert( layer );
    assert( staff );
    assert( measure );
    
    MRpt *mRpt = dynamic_cast<MRpt*>(element);
    assert( mRpt );
    
    dc->StartGraphic( element, "", element->GetUuid() );
    
    DrawMRptPart(dc, element->GetDrawingX(), SMUFL_E500_repeat1Bar, mRpt->m_drawingMeasureCount, false, staff, measure );
    
    dc->EndGraphic(element, this);
}

void View::DrawMRpt2(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert( dc );
    assert( element );
    assert( layer );
    assert( staff );
    assert( measure );
    
    MRpt2 *mRpt2 = dynamic_cast<MRpt2*>(element);
    assert( mRpt2 );
    
    dc->StartGraphic( element, "", element->GetUuid() );
    
    DrawMRptPart(dc, element->GetDrawingX(), SMUFL_E501_repeat2Bars, 2, true, staff, measure );
    
    dc->EndGraphic(element, this);
}

void View::DrawMultiRest(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert( dc );
    assert( element );
    assert( layer );
    assert( staff );
    assert( measure );
    
    MultiRest *multiRest = dynamic_cast<MultiRest*>(element);
    assert( multiRest );
    
    int x1, x2, y1, y2, length;

    dc->StartGraphic( element, "", element->GetUuid() );
    
    int width = measure->GetRightBarlineX() - measure->GetNonJustifiableLeftMargin();
    int xCentered = measure->GetDrawingX() + measure->GetNonJustifiableLeftMargin() + (width / 2);
  
    // We do not support more than three chars
    int num = std::min( multiRest->GetNum(), 999);
    
    if (num > 2 ) {
        // This is 1/2 the length of the black rectangle
        length = width - 2 * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
        
        // a is the central point, claculate x and x2
        x1 = xCentered - length / 2;
        x2 = xCentered + length / 2;
        
        // Position centered in third line
        y1 = staff->GetDrawingY() - (m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 2) * 5;
        y2 = y1 + m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
        
        // Draw the base rect
        // make it 8 pixels smaller than the interline space
        // these are the two 4 substracted and added
        DrawFullRectangle(dc, x1, y2, x2, y1);
        
        // Draw two lines at beginning and end
        // make it 8 pixels longer, and 4 pixels width
        int border = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        DrawVerticalLine(dc, y1 - border, y2 + border, x1, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) * 2);
        DrawVerticalLine(dc, y1 - border, y2 + border, x2, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) * 2);
    }
    else {
        // Draw the base rect
        x1 = xCentered - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 3;
        x2 = xCentered + m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 3 ;
        
        // Position centered in third line
        y1 = staff->GetDrawingY() - (m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 2) * 4;
        y2 = y1 + m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
        DrawFullRectangle(dc, x1, y2 - 4, x2, y1 + 4);
    }
    
    // Draw the text above
    int w, h;
    int start_offset = 0; // offset from x to center text
    
    // convert to string
    std::wstring wtext = IntToTimeSigFigures(num);
    
    dc->SetFont(m_doc->GetDrawingSmuflFont(staff->m_drawingStaffSize, false));
    dc->GetSmuflTextExtent( wtext, &w, &h);
    start_offset = (x2 - x1 - w) /  2; // calculate offset to center text
    DrawSmuflString(dc, x1 + start_offset, staff->GetDrawingY() + 3 * m_doc->GetDrawingUnit(staff->m_drawingStaffSize), wtext, false);
    dc->ResetFont();
    
    dc->EndGraphic(element, this);
}

void View::DrawMultiRpt(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert( dc );
    assert( element );
    assert( layer );
    assert( staff );
    assert( measure );
    
    MultiRpt *multiRpt = dynamic_cast<MultiRpt*>(element);
    assert( multiRpt );
    
    dc->StartGraphic( element, "", element->GetUuid() );
    
    DrawMRptPart(dc, element->GetDrawingX(), SMUFL_E501_repeat2Bars, multiRpt->GetNum(), true, staff, measure );
    
    dc->EndGraphic(element, this);
    
    return;
}

void View::DrawNote ( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert( dc );
    assert( element );
    assert( layer );
    assert( staff );
    assert( measure );
    
    Note *note = dynamic_cast<Note*>(element);
    assert( note );
    
    if (note->IsMensural()){
        DrawMensuralNote(dc, element, layer, staff, measure);
        return;
    }
    
    if (note->m_crossStaff) staff = note->m_crossStaff;
    
    Chord *inChord = note->IsChordTone();
    Beam *inBeam = note->IsInBeam();
    bool inFTrem = note->IsInFTrem();
    bool drawingCueSize = note->HasGrace();
    
	int staffSize = staff->m_drawingStaffSize;
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
    
    int radius = m_doc->GetGlyphWidth(SMUFL_E0A3_noteheadHalf, staffSize, drawingCueSize) / 2;
    
    if (drawingDur > DUR_1 || (drawingDur == DUR_1 && staff->notAnc)) {	// annuler provisoirement la modif. des lignes addit.
		ledge = m_doc->GetDrawingLedgerLineLength(staffSize, drawingCueSize);
	}
    else {
        ledge = m_doc->GetDrawingLedgerLineLength(staffSize, drawingCueSize);
		radius += radius/3;
	}
    
    /************** Stem/notehead direction: **************/
    
    verticalCenter = staffY - m_doc->GetDrawingDoubleUnit(staffSize)*2;
    if (!inChord && !inBeam && !inFTrem) {
        if (note->HasStemDir()) {
            note->SetDrawingStemDir(note->GetStemDir());
        }
        else if ( layer->GetDrawingStemDir() != STEMDIRECTION_NONE) {
            note->SetDrawingStemDir( layer->GetDrawingStemDir() );
        }
        else {
            note->SetDrawingStemDir( (noteY >= verticalCenter) ? STEMDIRECTION_down : STEMDIRECTION_up );
        }
    }
    
    //if the note is clustered, calculations are different
    if (note->m_cluster) {
        if (note->GetDrawingStemDir() == STEMDIRECTION_down) {
            //stem down/even cluster = noteheads start on left (incorrect side)
            if (note->m_cluster->size() % 2 == 0) {
                flippedNotehead = (note->m_clusterPosition % 2 != 0);
            }
            //else they start on normal side
            else {
                flippedNotehead = (note->m_clusterPosition % 2 == 0);
            }
            
            //if stem goes down, move ledger start to the left and expand it to the full radius
            if(!(note->IsClusterExtreme() && IsOnStaffLine(noteY, staff))) {
                xLedger -= radius;
                doubleLengthLedger = true;
            }
        }
        else {
            //flipped noteheads start on normal side no matter what
            flippedNotehead = (note->m_clusterPosition % 2 == 0);
            
            //if stem goes up, move ledger start to the right and expand it to the full radius
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
            if (note->GetDrawingStemDir() == STEMDIRECTION_up) {
                xNote = xStem + radius - m_doc->GetDrawingStemWidth(staffSize);
            }
            else if (note->GetDrawingStemDir() == STEMDIRECTION_down) {
                xNote = xStem - radius * 3 + m_doc->GetDrawingStemWidth(staffSize);
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

		DrawSmuflCode( dc, xNote, noteY, fontNo, staff->m_drawingStaffSize, drawingCueSize );
	}
    // Other values
	else {
        if ((note->GetColored()==BOOLEAN_true) || drawingDur == DUR_2) {
			fontNo = SMUFL_E0A3_noteheadHalf;
        }
        else {
			fontNo = SMUFL_E0A4_noteheadBlack;
        }
        
		DrawSmuflCode( dc, xNote, noteY, fontNo,  staff->m_drawingStaffSize, drawingCueSize );

		if (!(inBeam && drawingDur > DUR_4) && !inFTrem && !inChord) {
            DrawStem(dc, note, staff, note->GetDrawingStemDir(), radius, xStem, noteY);
        }

	}

    /************** Ledger lines: **************/

    int staffTop = staffY + m_doc->GetDrawingUnit(staffSize);
    int staffBot = staffY - m_doc->GetDrawingStaffSize(staffSize) - m_doc->GetDrawingUnit(staffSize);
    
    //if the note is not in the staff
    if (!is_in(noteY,staffTop,staffBot))
    {
        int distance, highestNewLine, numLines;
        
        bool aboveStaff = (noteY > staffTop);
        
        distance = (aboveStaff ? (noteY - staffY) : staffY - m_doc->GetDrawingStaffSize(staffSize) - noteY);
        highestNewLine = ((distance % m_doc->GetDrawingDoubleUnit(staffSize) > 0) ? (distance - m_doc->GetDrawingUnit(staffSize)) : distance);
        numLines = highestNewLine / m_doc->GetDrawingDoubleUnit(staffSize);
 
        //if this is in a chord, we don't want to draw it yet, but we want to keep track of the maxima
        if (inChord) {
            if (inChord->m_drawingLedgerLines.count(staff)==0) {
                std::vector<char> legerLines;
                legerLines.resize(4);
                inChord->m_drawingLedgerLines[staff] = legerLines;
            }
            int idx = doubleLengthLedger + aboveStaff * 2; // 2x2 array
            std::vector<char> *legerLines = &inChord->m_drawingLedgerLines.at(staff);
            (*legerLines).at(idx) = ledgermax(numLines, (*legerLines).at(idx));
        }
        //we do want to go ahead and draw if it's not in a chord
        else {
            DrawLedgerLines(dc, note, staff, aboveStaff, false, 0, numLines);
        }
    }
    
    /************** Accidentals/dots/peripherals: **************/
    
	if (note->m_drawingAccid) {
        xAccid = xNote;
        if (note->m_drawingAccid->GetFunc() != FUNC_edit) {
            xAccid -= 1.5 * m_doc->GetGlyphWidth(SMUFL_E262_accidentalSharp, staffSize, drawingCueSize);
        }
        
        note->m_drawingAccid->SetDrawingX( xAccid );
        note->m_drawingAccid->SetDrawingY( noteY );
        
        //postpone drawing the accidental until later if it's in a chord or if it is not an attribute
        if (!inChord && note->m_isDrawingAccidAttr) DrawAccid( dc, note->m_drawingAccid, layer, staff, measure );
	}
	
    if (note->GetDots() && !inChord) {
        int xDot;
        if (note->GetActualDur() < DUR_2 || (note->GetActualDur() > DUR_8 && !inBeam && (note->GetDrawingStemDir() == STEMDIRECTION_up)))
            xDot = xStem + m_doc->GetDrawingUnit(staffSize)*7/2;
        else
            xDot = xStem + m_doc->GetDrawingUnit(staffSize)*5/2;
        
		DrawDots( dc, xDot, noteY, note->GetDots(), staff );
	}
    
    if (note->GetDrawingTieAttr()) {
        System *system = dynamic_cast<System*>(measure->GetFirstParent( SYSTEM ) );
        // create a placeholder for the tie attribute that will be drawn from the system
        dc->StartGraphic(note->GetDrawingTieAttr(), "", note->GetDrawingTieAttr()->GetUuid().c_str());
        dc->EndGraphic(note->GetDrawingTieAttr(), this);
        if (system) system->AddToDrawingList(note->GetDrawingTieAttr());
    }

    // This will draw lyrics, accid, etc. (but only if not in chord)
    if (!inChord) DrawLayerChildren(dc, note, layer, staff, measure);

    if (note->GetFermata() != PLACE_NONE) {
        DrawFermata(dc, element, layer, staff);
    }

    if (note->m_embellishment == EMB_TRILL) {
        DrawTrill(dc, element, staff);
    }
}

void View::DrawRest ( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert( dc );
    assert( element );
    assert( layer );
    assert( staff );
    assert( measure );
        
    Rest *rest = dynamic_cast<Rest*>(element);
    assert( rest );

    bool drawingCueSize = rest->IsCueSize();
	int drawingDur = rest->GetActualDur();
	int x = element->GetDrawingX();
    int y = element->GetDrawingY();
    
    // Temporary fix for rest within tuplet because drawing tuplet requires m_drawingStemXXX to be set
    //element->m_drawingStemStart.x = element->m_drawingStemEnd.x = element->GetDrawingX() - (m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2);
    //element->m_drawingStemEnd.y = element->GetDrawingY();
    //element->m_drawingStemStart.y = element->GetDrawingY();

    if (drawingDur > DUR_2)
    {
		//x -= m_doc->GetGlyphWidth(SMUFL_E0A3_noteheadHalf, staff->m_drawingStaffSize, drawingCueSize);
    }

    switch (drawingDur)
    {	
        case DUR_LG: DrawRestLong( dc, x, y, staff); break;
        case DUR_BR: DrawRestBreve( dc, x, y, staff); break;
        case DUR_1:
        case DUR_2: DrawRestWhole( dc, x, y, drawingDur, rest->GetDots(), drawingCueSize, staff); break;
        default: DrawRestQuarter( dc, x, y, drawingDur, rest->GetDots(), drawingCueSize, staff);
    }
    
    if(rest->GetFermata() != PLACE_NONE) {
        DrawFermata(dc, element, layer, staff);
    }
}

void View::DrawSpace(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert( dc );
    assert( element );
    assert( layer );
    assert( staff );
    assert( measure );
    
    dc->StartGraphic( element, "", element->GetUuid() );
    dc->DrawPlaceholder( ToDeviceContextX( element->GetDrawingX() ), ToDeviceContextY( element->GetDrawingY() ) );
    dc->EndGraphic(element, this );
}

void View::DrawSyl( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    Syl *syl = dynamic_cast<Syl*>(element);
    assert( syl );
    
    if ( !syl->GetStart() ) {
        LogDebug("Syl parent note was not found");
        return;
    }
    
    // move the position back - to be updated HARDCODED also see View::DrawSylConnector and View::DrawSylConnectorLines
    syl->SetDrawingX( syl->GetStart()->GetDrawingX() - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2 );
    syl->SetDrawingY( GetSylY(syl, staff) );
    
    dc->StartGraphic( syl, "", syl->GetUuid() );
    
    dc->SetBrush( m_currentColour, AxSOLID );
    
    FontInfo currentFont;
    if (syl->HasFontstyle()) {
        currentFont = *m_doc->GetDrawingLyricFont(staff->m_drawingStaffSize);
        currentFont.SetStyle(syl->GetFontstyle());
        dc->SetFont(&currentFont);
    }
    else {
        dc->SetFont( m_doc->GetDrawingLyricFont(staff->m_drawingStaffSize) );
    }
    
    DrawLyricString(dc, syl->GetDrawingX(), syl->GetDrawingY(), syl->GetText().c_str(), staff->m_drawingStaffSize );
    
    dc->ResetFont();
    dc->ResetBrush();
    
    if (syl->GetStart() && syl->GetEnd()) {
        System *currentSystem = dynamic_cast<System*>( measure->GetFirstParent( SYSTEM ) );
        // Postpone the drawing of the syl to the end of the system; this will call DrawSylConnector
        // that will look if the last note is in the same system (or not) and draw the connectors accordingly
        if (currentSystem) {
            currentSystem->AddToDrawingList(syl);
        }
    }
    
    dc->EndGraphic(syl, this );
}

void View::DrawTuplet(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert( dc );
    assert( element );
    assert( layer );
    assert( staff );
    assert( measure );
    
    Tuplet *tuplet = dynamic_cast<Tuplet*>(element);
    assert( tuplet );
    
    dc->StartGraphic( element, "", element->GetUuid() );
    
    // Draw the inner elements
    DrawLayerChildren(dc, tuplet, layer, staff, measure);
    
    // Add to the list of postponed element
    layer->AddToDrawingList( tuplet );
    
    dc->EndGraphic(element, this );
}

void View::DrawVerse( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert( dc );
    assert( element );
    assert( layer );
    assert( staff );
    assert( measure );
    
    Verse *verse = dynamic_cast<Verse*>(element);
    assert( verse );
    
    dc->StartGraphic( verse, "", verse->GetUuid() );
    
    DrawLayerChildren(dc, verse, layer, staff, measure);
    
    dc->EndGraphic( verse, this );
}

//----------------------------------------------------------------------------
// Partial drawing methods
//----------------------------------------------------------------------------


void View::DrawAcciaccaturaSlash(DeviceContext *dc, LayerElement *element)
{
    Note *note = dynamic_cast<Note*>(element);
    assert( note );

    Staff *staff = dynamic_cast<Staff*>( note->GetFirstParent( STAFF ) );
    assert( staff );
    
    if (note->GetActualDur() < DUR_8)
        return;
    
    dc->SetPen(AxBLACK, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize), AxSOLID);
    dc->SetBrush( AxBLACK, AxSOLID );
    
    int positionShift = m_doc->GetGraceSize(m_doc->GetDrawingUnit(staff->m_drawingStaffSize));
    int positionShiftX1 = positionShift * 3 / 2;
    int positionShiftY1 = positionShift * 2;
    int positionShiftX2 = positionShift * 3;
    int positionShiftY2 = positionShift * 6;
    Point startPoint = note->GetDrawingStemStart();
    
    // HARDCODED
    if (note->GetDrawingStemDir() == STEMDIRECTION_up) {
        dc->DrawLine(
                     ToDeviceContextX(startPoint.x - positionShiftX1 ),
                     ToDeviceContextY(startPoint.y + positionShiftY1),
                     ToDeviceContextX(startPoint.x + positionShiftX2),
                     ToDeviceContextY(startPoint.y + positionShiftY2));
    } else {
        dc->DrawLine(ToDeviceContextX(startPoint.x - positionShiftX1),
                     ToDeviceContextY(startPoint.y - positionShiftY1),
                     ToDeviceContextX(startPoint.x + positionShiftX2),
                     ToDeviceContextY(startPoint.y - positionShiftY2));
    }
    
    dc->ResetPen();
    dc->ResetBrush();
}

void View::DrawDots( DeviceContext *dc, int x, int y, unsigned char dots, Staff *staff)
{
	int i;
    if ( IsOnStaffLine(y, staff) ) {
        y += m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    }
	for (i = 0; i < dots; i++) {
        DrawDot ( dc, x, y, staff->m_drawingStaffSize );
		x += m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
	}
}

void View::DrawFermata(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff)
{
    int x, y;
    int emb_offset = 0; // if there is and embellishment, offset the note up
    
    // We move the fermata position of half of the fermata size
    x = element->GetDrawingX() - m_doc->GetGlyphWidth(SMUFL_E4C0_fermataAbove, staff->m_drawingStaffSize, false) / 2;
    
    data_PLACE place = PLACE_above;
    if (layer->GetDrawingStemDir() != STEMDIRECTION_NONE) {
        place = (layer->GetDrawingStemDir() == STEMDIRECTION_up) ? PLACE_above : PLACE_above;
    }
    
    // First case, notes
    if ((element->Is() == NOTE) || (element->Is() == CHORD)) {
        // To be fixed once m_embellishment is removed
        if (element->Is() == NOTE)  {
            Note *note = dynamic_cast<Note*>(element);
            assert( note );
            if (note->m_embellishment) {
                emb_offset = m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
            }
        }
        
        if (place == PLACE_above) {
            // check if the notehead is in the staff.
            if (element->GetDrawingTop(m_doc, staff->m_drawingStaffSize) < staff->GetDrawingY()) {
                // in the staff, set the fermata 20 pixels above the last line (+ embellishment offset)
                y = staff->GetDrawingY() + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) + emb_offset;
            }
            else {
                // out of the staff, place the trill above the notehead
                y = element->GetDrawingTop(m_doc, staff->m_drawingStaffSize) + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) + emb_offset;
            }
            // draw the up-fermata - need cue size support
            DrawSmuflCode ( dc, x, y, SMUFL_E4C0_fermataAbove, staff->m_drawingStaffSize, false );
        }
        else {
            // This works as above, only we check that the note head is not
            if (element->GetDrawingBottom(m_doc, staff->m_drawingStaffSize) > (staff->GetDrawingY() - m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize))) {
                // notehead in staff, set  under
                y = staff->GetDrawingY() - m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize) - m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
            }
            else {
                // notehead under staff, set under notehead
                y = element->GetDrawingBottom(m_doc, staff->m_drawingStaffSize) - m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
            }
            // draw the down-fermata - need cue size support
            DrawSmuflCode ( dc, x, y, SMUFL_E4C1_fermataBelow, staff->m_drawingStaffSize, false );
        }
    } else if (element->Is() == REST) {
        if (place == PLACE_above) {
            y = staff->GetDrawingY() + m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
            DrawSmuflCode ( dc, x, y, SMUFL_E4C0_fermataAbove, staff->m_drawingStaffSize, false );
        }
        else {
            y = staff->GetDrawingY() - m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize) - m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
            DrawSmuflCode ( dc, x, y, SMUFL_E4C1_fermataBelow, staff->m_drawingStaffSize, false );
        }
    }
}

//skips "skip" lines before drawing "n" ledger lines
void View::DrawLedgerLines( DeviceContext *dc, LayerElement *element, Staff *staff, bool aboveStaff, bool doubleLength, int skip, int n)
{
    //various variables
    int ledgerY;
    int staffY = staff->GetDrawingY();
    int staffSize = staff->m_drawingStaffSize;
    int betweenLines = m_doc->GetDrawingDoubleUnit(staffSize);
    bool drawingCueSize = element->IsCueSize();
    int ledge = m_doc->GetDrawingLedgerLineLength(staffSize, drawingCueSize);
    int noteDiameter = m_doc->GetGlyphWidth(SMUFL_E0A3_noteheadHalf, staffSize, drawingCueSize);
    
    //prep start and end positions of ledger line depending on stem direction and doubleLength
    int xLedgerStart, xLedgerEnd;
    if (doubleLength) {
        Chord *chord = dynamic_cast<Chord*>(element);
        assert( chord );
        if (chord->GetDrawingStemDir() == STEMDIRECTION_down) {
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
        ledgerY = staffY - m_doc->GetDrawingStaffSize(staffSize) - (skip * betweenLines);
        betweenLines = -m_doc->GetDrawingDoubleUnit(staffSize);
    }
    
    //add one line's distance to get it off the edge of the staff
    ledgerY += betweenLines;
    
    dc->SetPen( m_currentColour, ToDeviceContextX( m_doc->GetDrawingStaffLineWidth(staffSize) ), AxSOLID );
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

void View::DrawMeterSigFigures( DeviceContext *dc, int x, int y, int num, int numBase, Staff *staff)
{
    assert( dc );
    assert( staff );
    
    int ynum, yden;
    std::wstring wtext;
    
    if (numBase)
    {
        ynum = y - (m_doc->GetDrawingUnit(staff->m_drawingStaffSize)*2);
        yden = ynum - (m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize)*2);
    }
    else
        ynum = y - (m_doc->GetDrawingUnit(staff->m_drawingStaffSize)*4);
    
    if (numBase > 9 || num > 9)	{
        x += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2;
    }
    
    dc->SetFont(m_doc->GetDrawingSmuflFont(staff->m_drawingStaffSize, false));
    
    wtext = IntToTimeSigFigures(num);
    DrawSmuflString ( dc, x, ynum, wtext, true, staff->m_drawingStaffSize);	// true = center
    
    if (numBase)
    {
        wtext = IntToTimeSigFigures(numBase);
        DrawSmuflString ( dc, x, yden, wtext, true, staff->m_drawingStaffSize);	// '1' = center
    }
    
    dc->ResetFont();
}

void View::DrawMRptPart(DeviceContext *dc, int x, wchar_t smuflCode, int num, bool line, Staff *staff, Measure *measure )
{
    int width = measure->GetRightBarlineX() - measure->GetNonJustifiableLeftMargin();
    int xCentered = measure->GetDrawingX() + measure->GetNonJustifiableLeftMargin()  + (width / 2);
    
    int xSymbol = xCentered - m_doc->GetGlyphWidth(smuflCode, staff->m_drawingStaffSize, false) / 2;
    int y = staff->GetDrawingY();
    int ySymbol = y - staff->m_drawingLines / 2 * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
    
    DrawSmuflCode( dc, xSymbol, ySymbol, smuflCode, staff->m_drawingStaffSize, false );
    
    if (line) {
        DrawVerticalLine( dc, y, y - m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize),
                     xCentered, m_doc->GetDrawingBarLineWidth(staff->m_drawingStaffSize));
    }
    
    if (num > 0) {
        dc->SetFont( m_doc->GetDrawingSmuflFont( staff->m_drawingStaffSize, false) );
        // calculate the width of the figures
        int txt_length = 0;
        int txt_height = 0;
        std::wstring figures = IntToTupletFigures(num);
        dc->GetSmuflTextExtent(figures, &txt_length, &txt_height);
        dc->DrawMusicText( figures, ToDeviceContextX(xCentered - txt_length / 2),
                          ToDeviceContextY(staff->GetDrawingY() + m_doc->GetDrawingUnit(staff->m_drawingStaffSize)) );
        dc->ResetFont();
    }
}

void View::DrawRestBreve( DeviceContext *dc, int x, int y, Staff *staff)
{
    int x1, x2, y1, y2;
    y1 = y;
	x1 = x;
	x2 = x + (m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2 / 3);
    
    // look if one line or between line
	if ( (y - staff->GetDrawingY()) % m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize))
		y1 -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    
	y2 = y1 + m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
	DrawFullRectangle( dc, x1, y2, x2, y1);
	
    // lines
    x1 = x - m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    x2 = x + m_doc->GetDrawingUnit(staff->m_drawingStaffSize);

	DrawHorizontalLine ( dc, x1, x2, y2, 1);
	DrawHorizontalLine ( dc, x1, x2, y1, 1);
}

void View::DrawRestLong( DeviceContext *dc, int x, int y, Staff *staff)
{
    int x1, x2, y1, y2;
    
    y1 = y;
	x1 = x;
	x2 = x + (m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2 / 3);
    
    // look if on line or between line
	if ( (y - staff->GetDrawingY()) % m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize))
		y1 -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    
	y2 = y1 + m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize)*2;
	DrawFullRectangle( dc, x1, y2, x2, y1);
}

void View::DrawRestQuarter( DeviceContext *dc, int x, int y, int valeur, unsigned char dots, bool cueSize, Staff *staff)
{
    int y2 = y + m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
	DrawSmuflCode( dc, x, y2, SMUFL_E4E5_restQuarter + (valeur-DUR_4), staff->m_drawingStaffSize, cueSize );
    
	if (dots)
	{	if (valeur < DUR_16) y += m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
		DrawDots( dc, (x + 2 * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize)), y, dots, staff);
	}
}

void View::DrawRestWhole( DeviceContext *dc, int x, int y, int valeur, unsigned char dots, bool cueSize, Staff *staff)
{
    int x1, x2, y1, y2, vertic;
    y1 = y;
    vertic = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
	
    int off = m_doc->GetDrawingLedgerLineLength(staff->m_drawingStaffSize, cueSize) * 2 / 3; // i.e., la moitie de la ronde

	x1 = x - off;
	x2 = x + off;

	if (valeur == DUR_1)
		vertic = -vertic;

    // look if one line or between line
	if ( (y - staff->GetDrawingY()) % m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize))
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
	if (y > (int)staff->GetDrawingY()  || y < staff->GetDrawingY() - m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize))
		DrawHorizontalLine ( dc, x1, x2, y1, m_doc->GetDrawingStaffLineWidth(staff->m_drawingStaffSize));

    if (dots) {
		DrawDots( dc, (x2 + m_doc->GetDrawingUnit(staff->m_drawingStaffSize)), y2, dots, staff);
    }
}

void View::DrawStem( DeviceContext *dc, LayerElement *object, Staff *staff, data_STEMDIRECTION dir, int radius, int xn, int originY, int heightY)
{
    assert(dynamic_cast<DurationInterface*>(object));
    
    int staffSize = staff->m_drawingStaffSize;
    int staffY = staff->GetDrawingY();
    int baseStem, totalFlagStemHeight, flagStemHeight, nbFlags;
    int drawingDur = dynamic_cast<DurationInterface*>(object)->GetActualDur();
    bool drawingCueSize = object->IsCueSize();
    int verticalCenter = staffY - m_doc->GetDrawingDoubleUnit(staffSize)*2;
    
    // baseStem is one octave for now (7 unit) - however, it should be 6 with 2-voice notation
    baseStem = m_doc->GetDrawingOctaveSize(staffSize);
    flagStemHeight = m_doc->GetDrawingDoubleUnit(staffSize);
    if (drawingCueSize) {
        baseStem = m_doc->GetGraceSize(baseStem);
        flagStemHeight = m_doc->GetGraceSize(flagStemHeight);
    }

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
    int stemY1 = (dir == STEMDIRECTION_up) ? y1 + m_doc->GetDrawingUnit(staffSize) / 4 : y1 - m_doc->GetDrawingUnit(staffSize) / 4;
    int stemY2 = y2;
    if (drawingDur > DUR_4) {
        // if we have flags, shorten the stem to make sure we have a nice overlap with the flag glyph
        int shortener = (drawingCueSize) ?
            m_doc->GetGraceSize(m_doc->GetDrawingUnit(staffSize)) :
            m_doc->GetDrawingUnit(staffSize);
        stemY2 = (dir == STEMDIRECTION_up) ? y2 - shortener : y2 + shortener;
    }
    
    // draw the stems and the flags
    if (dir == STEMDIRECTION_up) {
        DrawFullRectangle( dc, x2 - m_doc->GetDrawingStemWidth(staffSize), stemY1, x2, stemY2);
        
        if (drawingDur > DUR_4) {
            DrawSmuflCode( dc, x2 - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize), y2, SMUFL_E240_flag8thUp, staff->m_drawingStaffSize, drawingCueSize );
            for (int i=0; i < nbFlags; i++)
                DrawSmuflCode( dc, x2 - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize), y2 - (i + 1) * flagStemHeight, SMUFL_E240_flag8thUp,  staff->m_drawingStaffSize, drawingCueSize );
        }
    }
    else {
        DrawFullRectangle( dc, x2, stemY1, x2 + m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize), stemY2);
        
        if (drawingDur > DUR_4) {
            DrawSmuflCode( dc, x2 , y2, SMUFL_E241_flag8thDown , staff->m_drawingStaffSize, drawingCueSize );
            for (int i=0; i < nbFlags; i++)
                DrawSmuflCode( dc, x2, y2 + (i + 1) * flagStemHeight, SMUFL_E241_flag8thDown,  staff->m_drawingStaffSize, drawingCueSize );
        }
    }
    
    // Store the start and end values
    StemmedDrawingInterface *interface = dynamic_cast<StemmedDrawingInterface*>(object);
    assert(interface);
    interface->SetDrawingStemStart(Point(x2 - (m_doc->GetDrawingStemWidth(staffSize) / 2), y1));
    interface->SetDrawingStemEnd(Point(x2 - (m_doc->GetDrawingStemWidth(staffSize) / 2), y2));
    interface->SetDrawingStemDir(dir);
    
    // cast to note is check when setting drawingCueSize value
    if ( drawingCueSize && ( dynamic_cast<Note*>(object)->GetGrace() == GRACE_acc ) ) {
        DrawAcciaccaturaSlash(dc, object);
    }
}

// Draw a trill above the notehead
// This function works as the up-fermata portion of DrawFermata
// if there are many symbols to draw we could make a generalized function
void View::DrawTrill(DeviceContext *dc, LayerElement *element, Staff *staff)
{
    int x, y;    

    // It shoud be moved according to half of the trill size
    x = element->GetDrawingX() - m_doc->GetGlyphWidth(SMUFL_E566_ornamentTrill, staff->m_drawingStaffSize, false);

    // HARDCODED
    if ((element->GetDrawingY()) < staff->GetDrawingY())
        y = staff->GetDrawingY() + m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
    else
        y = (element->GetDrawingY()) + m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
    
    DrawSmuflCode ( dc, x, y, SMUFL_E566_ornamentTrill, staff->m_drawingStaffSize, false );
}

//----------------------------------------------------------------------------
// Calculation or preparation methods
///----------------------------------------------------------------------------

bool View::CalculateAccidX(Staff *staff, Accid *accid, Chord *chord, bool adjustHorizontally)
{
    std::vector< std::vector<bool> > *accidSpace = &chord->m_accidSpace;
    
    //global drawing variables
    int fullUnit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
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
            if (accidSpace->at(accidTop + (ACCID_HEIGHT * FLAT_CORNER_HEIGHT_IGNORE)).at(currentX - accidWidthDiff)) currentX += 1;
            // just in case
            else if (currentX - accidWidthDiff + (ACCID_WIDTH * FLAT_CORNER_WIDTH_IGNORE) >= xLength ) break;
            else if (accidSpace->at(accidTop).at(currentX - accidWidthDiff + (ACCID_WIDTH * FLAT_CORNER_WIDTH_IGNORE))) currentX += 1;
            else if (accidSpace->at(accidBot).at(currentX - accidWidthDiff)) currentX += 1;
            else if (accidSpace->at(accidTop).at(currentX)) currentX += 1;
            else if (accidSpace->at(accidBot).at(currentX)) currentX += 1;
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
            if (accidSpace->at(accidTop + (ACCID_HEIGHT * NATURAL_CORNER_HEIGHT_IGNORE)).at(currentX - accidWidthDiff)) currentX += 1;
            // just in case
            else if (currentX - accidWidthDiff + (ACCID_WIDTH * NATURAL_CORNER_WIDTH_IGNORE) >= xLength ) break;
            else if (accidSpace->at(accidTop).at(currentX - accidWidthDiff + (ACCID_WIDTH * NATURAL_CORNER_WIDTH_IGNORE))) currentX += 1;
            else if (accidSpace->at(accidMid).at(currentX - accidWidthDiff)) currentX += 1;
            else if (accidSpace->at(accidBot).at(currentX - accidWidthDiff)) currentX += 1;
            else if (accidSpace->at(accidTop).at(currentX)) currentX += 1;
            else if (accidSpace->at(accidMid).at(currentX)) currentX += 1;
            else if (accidSpace->at(accidBot).at(currentX)) currentX += 1;
            else break;
        };
    }
    else if (type == ACCIDENTAL_EXPLICIT_s) {
        accidBot = accidTop + accidHeightDiff;
        alignmentThreshold = 1;
        //Midpoint needs to be checked for non-flats as there's a chance that a natural/sharp could completely overlap a flat
        int accidMid = accidTop + (accidBot - accidTop) / 2;
        while (currentX < xLength) {
            if (accidSpace->at(accidTop).at(currentX - accidWidthDiff)) currentX += 1;
            else if (accidSpace->at(accidMid).at(currentX - accidWidthDiff)) currentX += 1;
            else if (accidSpace->at(accidBot).at(currentX - accidWidthDiff)) currentX += 1;
            else if (accidSpace->at(accidTop).at(currentX)) currentX += 1;
            else if (accidSpace->at(accidMid).at(currentX)) currentX += 1;
            else if (accidSpace->at(accidBot).at(currentX)) currentX += 1;
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
            if (accidSpace->at(accidTop).at(currentX - accidWidthDiff)) currentX += 1;
            else if (accidSpace->at(accidMid).at(currentX - accidWidthDiff)) currentX += 1;
            else if (accidSpace->at(accidBot).at(currentX - accidWidthDiff)) currentX += 1;
            else if (accidSpace->at(accidTop).at(currentX)) currentX += 1;
            else if (accidSpace->at(accidMid).at(currentX)) currentX += 1;
            else if (accidSpace->at(accidBot).at(currentX)) currentX += 1;
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
        if((accidSpace->at(yComp).at(currentX + 1) == false) && (accidSpace->at(yComp).at(currentX) == true)) currentX += 1;
    }
    
    //If the accidental is lined up with the one below it, move it left by a halfunit to avoid visual confusion
    //This doesn't need to be done with accidentals that are as far left or down as possible
    if ((currentX < xLength - 1) && (accidBot < (yHeight - 1)) && accidBot > 1)
    {
        int yComp = accidBot;
        assert(yComp < accidSpaceSize);
        assert(yComp >= 0);
        if((accidSpace->at(yComp).at(currentX + 1) == false) && (accidSpace->at(yComp).at(currentX) == true)) currentX += 1;
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

int View::GetSylY( Syl *syl, Staff *staff )
{
    assert( syl && staff );
    
    int y = syl->GetStart()->GetDrawingY();
    if (staff->GetAlignment() ) {
        y = staff->GetDrawingY() + staff->GetAlignment()->GetMaxHeight() -
        syl->m_drawingVerse * TEMP_STYLE_LYIRC_LINE_SPACE * m_doc->GetDrawingUnit(staff->m_drawingStaffSize)  / PARAM_DENOMINATOR;
    }
    return y;
}

bool View::IsOnStaffLine ( int y, Staff *staff)
{
    return ((y - staff->GetDrawingY()) % m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) == 0 );
}

void View::PrepareChordDots ( DeviceContext *dc, Chord *chord, int x, int y, unsigned char dots, Staff *staff )
{
    std::list<int> *dotsList = &chord->m_dots;
    int fullUnit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
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
}


} // namespace vrv
