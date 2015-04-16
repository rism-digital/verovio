/////////////////////////////////////////////////////////////////////////////
// Name:        view_beam.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Author and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "view.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <math.h>

//----------------------------------------------------------------------------

#include "beam.h"
#include "devicecontext.h"
#include "doc.h"
#include "layer.h"
#include "layerelement.h"
#include "note.h"
#include "staff.h"
#include "style.h"
#include "vrv.h"

namespace vrv {

void View::DrawBeam( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure )
{
    assert(layer); // Pointer to layer cannot be NULL"
    assert(staff); // Pointer to staff cannot be NULL"
    
    Beam *beam = dynamic_cast<Beam*>(element);
    
    LayerElement *current;

	bool changingDur = OFF;
    bool beamHasChord = OFF;
    bool hasMultipleStemDir = OFF;
    data_STEMDIRECTION stemDir = STEMDIRECTION_NONE;
    
    // position variables
    int high, low, yExtreme;
    int verticalCenter;
    
    double verticalShiftFactor = 3.0;
    
    // For slope calculation and linear regression
    double verticalBoost = 0.0; //extra height to ensure the beam clears all the noteheads
    double s_x=0.0; //sum of all x(n) for n in beamElementCoord
    double s_y=0.0; //sum of all y(n)
    double s_xy=0.0; //sum of (x(n) * y(n))
    double s_x2=0.0; //sum of all x(n)^2
    double s_y2=0.0; //sum of all y(n)^2
    double startingY, beamSlope; //startingY is the initial position of the beam, beamSlope is the slope
    
    // position in the beam element list
	int elementCount, last;
    
    // duration variables
	int shortestDur, lastDur, currentDur, testDur;

    // position x for the stem (normal and cue size)
	int dx[2];
    
    // temporary coordinates
    int fx1,fx2,fy1,fy2;
    
    // temporary variables
	int avgY, shiftY, barY, verticalShift, y1, fullBars, polygonHeight;
    double xr;

    // loops
	int i, j;
    
    // beam bar sizes
	int beamWidth, beamWidthBlack, beamWidthWhite;

    
    /******************************************************************/
    // initialization
    
    shortestDur = 0;
    lastDur = elementCount = 0;
    high = avgY = verticalBoost = 0.0;
    
    verticalCenter = staff->GetDrawingY() - (m_doc->m_drawingDoubleUnit[staff->staffSize] * 2); //center point of the staff
    yExtreme = verticalCenter; //value of farthest y point on the staff from verticalCenter minus verticalCenter; used if beamHasChord = ON

    ListOfObjects* beamChildren = beam->GetList(beam);
    
    // Should we assert this at the beginning?
    if (beamChildren->empty()) {
        return;
    }
    const ArrayOfBeamElementCoords *beamElementCoords = beam->GetElementCoords();
    
    // current point to the first Note in the layed out layer
    current = dynamic_cast<LayerElement*>(beamChildren->front());
    // Beam list should contain only DurationInterface objects
    assert( dynamic_cast<DurationInterface*>(current) );
    
	low = current->GetDrawingY();
    lastDur = dynamic_cast<DurationInterface*>(current)->GetActualDur();
    
    // x-offset values for stem bases, dx[y] where y = element->m_cueSize
    dx[0] =  m_doc->m_drawingNoteRadius[staff->staffSize][0] - (m_doc->m_style->m_stemWidth)/2;
    dx[1] =  m_doc->m_drawingNoteRadius[staff->staffSize][1] - (m_doc->m_style->m_stemWidth)/2;
    
    /******************************************************************/
    // Populate BeamElementCoord for each element in the beam
    // This could be moved to Beam::InitCoord for optimization because there should be no
    // need of redoing it everytime it is drawn.
    
    data_STEMDIRECTION currentStemDir;
    
    ListOfObjects::iterator iter = beamChildren->begin();
	do {
        // Beam list should contain only DurationInterface objects
        assert( dynamic_cast<DurationInterface*>(current) );
        
        currentDur = dynamic_cast<DurationInterface*>(current)->GetActualDur();
        
        if ( current->IsChord() ) {
            beamHasChord = true;
        }

        // Can it happen? With rests?
		if (currentDur > DUR_4) {
			(*beamElementCoords)[elementCount]->m_element = current;
            current->m_beamElementCoord = (*beamElementCoords)[elementCount];
            (*beamElementCoords)[elementCount]->m_x = current->GetDrawingX();
			(*beamElementCoords)[elementCount]->m_dur = currentDur;
            
            // Look at beam breaks
            (*beamElementCoords)[elementCount]->m_breaksec = 0;
            AttBeamsecondary *beamsecondary = dynamic_cast<AttBeamsecondary*>(current);
            if ( elementCount && beamsecondary && beamsecondary->HasBreaksec()) {
                if (!changingDur) changingDur = ON;
                (*beamElementCoords)[elementCount]->m_breaksec = beamsecondary->GetBreaksec();
            }
            
            // Skip rests
            if (current->IsNote() || current->IsChord()) {
                // look at the stemDir to see if we have multiple stem Dir
                if (!hasMultipleStemDir) {
                    currentStemDir = dynamic_cast<AttStemmed*>(current)->GetStemDir();
                    if (currentStemDir != STEMDIRECTION_NONE) {
                        if ((stemDir != STEMDIRECTION_NONE) && (stemDir != currentStemDir)) {
                            hasMultipleStemDir = ON;
                        }
                    }
                    stemDir = currentStemDir;
                }
                // keep the shortest dur in the beam
                shortestDur = std::max(currentDur,shortestDur);
                // check if we have more than duration in the beam
                if (!changingDur && currentDur != lastDur) changingDur = ON;
                lastDur = currentDur;
            }
			elementCount++;
		}
        
        iter++;
        if (iter == beamChildren->end()) {
            break;
        }
        current = dynamic_cast<LayerElement*>(*iter);
		if (current == NULL) {
            LogDebug("Error accessing element in Beam list");
            return;
        }
        
	}	while (1);

    //elementCount must be greater than 0 here
    if (elementCount == 0){
        LogDebug("Beam with no notes of duration > 8 detected. Exiting DrawBeamPostponed gracefully.");
        return;
    }

	last = elementCount - 1;
    
    /******************************************************************/
    // Calculate the extreme values
    
    int yMax = 0, yMin = 0;
    int curY;
    // elementCount holds the last one
	for (i = 0; i < elementCount; i++) {

        if (dynamic_cast<Chord*>((*beamElementCoords)[i]->m_element)) {
            dynamic_cast<Chord*>((*beamElementCoords)[i]->m_element)->GetYExtremes(&yMax, &yMin);
            (*beamElementCoords)[i]->m_yTop = yMax;
            (*beamElementCoords)[i]->m_yBottom = yMin;
            
            avgY += (*beamElementCoords)[i]->m_y + ((yMax - yMin) / 2);
            
            // highest and lowest value;
            high= std::max(yMax, high);
            low = std::min(yMin, low);
        }
        else {
            (*beamElementCoords)[i]->m_y = (*beamElementCoords)[i]->m_element->GetDrawingY();
            
            // highest and lowest value;
            high= std::max((*beamElementCoords)[i]->m_y, high);
            low = std::min((*beamElementCoords)[i]->m_y, low);
            
            curY = (*beamElementCoords)[i]->m_element->GetDrawingY();
            (*beamElementCoords)[i]->m_y = curY;
            (*beamElementCoords)[i]->m_yTop = curY;
            (*beamElementCoords)[i]->m_yBottom = curY;
            avgY += (*beamElementCoords)[i]->m_y;
        }
	}

    /******************************************************************/
    // Set the stem direction
    
    yExtreme = (abs(high - verticalCenter) > abs(low - verticalCenter) ? high : low);
    avgY /= elementCount;
    
    // If we have one stem direction in the beam, then don't look at the layer
    if (stemDir != STEMDIRECTION_NONE) stemDir = layer->GetDrawingStemDir(); // force layer direction if it exists
    
    // Automatic stem direction if nothing in the notes or in the layer
    if (stemDir == STEMDIRECTION_NONE) {
        if (beamHasChord) stemDir = (yExtreme < verticalCenter) ?  STEMDIRECTION_up : STEMDIRECTION_down; //if it has a chord, go by the most extreme position
        else stemDir = (avgY <  verticalCenter) ? STEMDIRECTION_up : STEMDIRECTION_down; //otherwise go by average
    }
    
    beam->SetDrawingStemDir(stemDir);
    
    if (stemDir == STEMDIRECTION_up) { //set stem direction for all the notes
        for (i = 0; i < elementCount; i++) {
            (*beamElementCoords)[i]->m_y = (*beamElementCoords)[i]->m_yTop;
        }
    }
    else {
        for (i = 0; i < elementCount; i++) {
            (*beamElementCoords)[i]->m_y = (*beamElementCoords)[i]->m_yBottom;
        }
    }

    // We look only at the last note for checking if cuesized. Somehow arbitrarily
    if ((*beamElementCoords)[last]->m_element->m_cueSize == false)  {
        beamWidthBlack = m_doc->m_drawingBeamWidth[staff->staffSize];
        beamWidthWhite = m_doc->m_drawingBeamWhiteWidth[staff->staffSize];
    }
    else {
        beamWidthBlack = std::max(2, (m_doc->m_drawingBeamWidth[staff->staffSize] * m_doc->m_style->m_graceNum / m_doc->m_style->m_graceDen));
        beamWidthWhite = std::max(2, (m_doc->m_drawingBeamWhiteWidth[staff->staffSize] * m_doc->m_style->m_graceNum / m_doc->m_style->m_graceDen));
    }
    
	beamWidth = beamWidthBlack + beamWidthWhite;

    /******************************************************************/
    // Calculate the slope doing a linear regression
    
    // The vertical shift depends on the shortestDur value we have in the beam
    verticalShift = ((shortestDur-DUR_8)*(beamWidth));

    //if the beam has smaller-size notes
    if ((*beamElementCoords)[last]->m_element->m_cueSize) {
        verticalShift += m_doc->m_drawingUnit[staff->staffSize]*5;
    }
    else {
        verticalShift += (shortestDur > DUR_8) ?
            m_doc->m_drawingDoubleUnit[staff->staffSize] * verticalShiftFactor :
            m_doc->m_drawingDoubleUnit[staff->staffSize] * (verticalShiftFactor + 0.5);
    }
    
    // swap x position and verticalShift direction with stem down
    if (stemDir == STEMDIRECTION_down) {
        dx[0] = -dx[0];
        dx[1] = -dx[1];
        verticalShift = -verticalShift;
    }
    
    for (i=0; i<elementCount; i++)
    {
        //change the stem dir for all objects
        if ( (*beamElementCoords)[i]->m_element->IsNote() ) {
            ((Note*)(*beamElementCoords)[i]->m_element)->m_drawingStemDir = stemDir;
        }
        else if ( (*beamElementCoords)[i]->m_element->IsChord() ) {
            ((Chord*)(*beamElementCoords)[i]->m_element)->SetDrawingStemDir(stemDir);
        }

        (*beamElementCoords)[i]->m_yBeam = (*beamElementCoords)[i]->m_y + verticalShift;
        (*beamElementCoords)[i]->m_x +=  dx[(*beamElementCoords)[i]->m_element->m_cueSize];
        
        s_y += (*beamElementCoords)[i]->m_yBeam;
        s_y2 += (*beamElementCoords)[i]->m_yBeam * (*beamElementCoords)[i]->m_yBeam;
        s_x += (*beamElementCoords)[i]->m_x;
        s_x2 += (*beamElementCoords)[i]->m_x * (*beamElementCoords)[i]->m_x;
        s_xy += (*beamElementCoords)[i]->m_x * (*beamElementCoords)[i]->m_yBeam;
    }

	y1 = elementCount * s_xy - s_x * s_y;
	xr = elementCount * s_x2 - s_x * s_x;

    // Prevent division by 0
    if (y1 && xr) {
		beamSlope = y1 / xr;
    }
    else {
		beamSlope = 0.0;
    }
    
	/* Correction esthetique : */
	if (fabs(beamSlope) < m_doc->m_drawingBeamMinSlope ) beamSlope = 0.0;
	if (fabs(beamSlope) > m_doc->m_drawingBeamMaxSlope ) beamSlope = (beamSlope > 0) ? m_doc->m_drawingBeamMaxSlope : - m_doc->m_drawingBeamMaxSlope;
	/* pente correcte: entre 0 et env 0.4 (0.2 a 0.4) */
    
	startingY = (s_y - beamSlope * s_x) / elementCount;

    /******************************************************************/
    // Start the Beam graphic and draw the children
    
    dc->StartGraphic( element, "", element->GetUuid() );
    
    DrawLayerChildren(dc, beam, layer, staff, measure);

    /******************************************************************/
    // Calculate the stem lengths and draw them

    double oldYPos; //holds y position before calculation to determine if beam needs extra height
    double expectedY;
	for ( i=0; i<elementCount; i++ ) {
        oldYPos = (*beamElementCoords)[i]->m_yBeam;
		expectedY = startingY + verticalBoost + beamSlope * (*beamElementCoords)[i]->m_x;
        
        //if the stem is not long enough, add extra stem length needed to all members of the beam
        if ((stemDir == STEMDIRECTION_up && (oldYPos > expectedY)) || (stemDir == STEMDIRECTION_down && (oldYPos < expectedY))) {
            verticalBoost += oldYPos - expectedY;
        }
	}
    
    for (i=0; i<elementCount; i++)
    {
		(*beamElementCoords)[i]->m_yBeam = startingY + verticalBoost + beamSlope * (*beamElementCoords)[i]->m_x;
    }
        
	for (i=0; i<elementCount; i++)
	{
        if (stemDir == STEMDIRECTION_up) {
            fy1 = (*beamElementCoords)[i]->m_yBeam - m_doc->m_style->m_stemWidth;
            fy2 = (*beamElementCoords)[i]->m_yBottom + m_doc->m_drawingUnit[staff->staffSize]/4;
        }
        else {
            fy1 = (*beamElementCoords)[i]->m_yBeam + m_doc->m_style->m_stemWidth;
            fy2 = (*beamElementCoords)[i]->m_yTop - m_doc->m_drawingUnit[staff->staffSize]/4;
        }
        
        LayerElement *el = (*beamElementCoords)[i]->m_element;
        el->m_drawingStemStart.x = el->m_drawingStemEnd.x = (*beamElementCoords)[i]->m_x;
        el->m_drawingStemStart.y = fy2;
        el->m_drawingStemEnd.y = fy1;
        el->m_drawingStemDir = stemDir;
        
        if((el->IsNote() && ! dynamic_cast<Note*>(el)->IsChordTone()) || el->IsChord()){
            DrawVerticalLine (dc,fy2, fy1, (*beamElementCoords)[i]->m_x, m_doc->m_style->m_stemWidth);
        }
	}

    /******************************************************************/
    // Draw the beam full bars
    
    // Number of bars to draw - if we do not have changing values, draw
    // the number of bars according to the shortestDur value. Otherwise draw
    // only one bar and the others will be drawn separately.
    fullBars =  !changingDur ? (shortestDur - DUR_4) : 1;
    
    // Adjust the x position of the first and last element for taking into account the stem width
	(*beamElementCoords)[0]->m_x -= (m_doc->m_style->m_stemWidth) / 2;
	(*beamElementCoords)[last]->m_x += (m_doc->m_style->m_stemWidth) / 2;

    // Shift direction
	shiftY = (stemDir == STEMDIRECTION_down) ? 1.0 : -1.0;

	fy1 = (*beamElementCoords)[0]->m_yBeam;
    fy2 = (*beamElementCoords)[last]->m_yBeam;

    fx1 = (*beamElementCoords)[0]->m_x;
    fx2 = (*beamElementCoords)[last]->m_x;

	s_y = shiftY;
	s_y2 = shiftY;
    
    // For acc and rit beam (see AttBeamingVis set
    // s_y = 0 and s_y2 = 0 respectively

	for (j=0; j<fullBars ; j++)
	{
		polygonHeight = beamWidthBlack*shiftY;
        DrawObliquePolygon (dc,fx1,fy1,fx2,fy2, polygonHeight);
		fy1 += polygonHeight; fy2 += polygonHeight;

        // s_y must == 0 for accelerando beams
		if (!s_y) fy1 += (beamWidthBlack * shiftY) * -1;
		else fy1 += s_y*beamWidthWhite;
        
        // reverse for retardendo beam
		if (!s_y2) fy2 += (beamWidthBlack * shiftY) * -1;
		else fy2 += s_y2*beamWidthWhite;
	}
    
    /******************************************************************/
    // Draw the beam partial bars (if any)

	/* calcul des x en cas de beaming multiple */
	/* parcours horizontal ajoutant barres en fonction de m_dur la plus 
	breve (shortestDur), controle par boucle while; la premiere boucle for
	controle le nombre d'etapes horizontales du parcours (par le nombre
	de commandes '+' enregistrees); la deuxieme boucle for teste pour
	chaque paquet entre deux '+' les valeurs; elle construit une array de 
	marqueurs partitionnant les sous-groupes; la troisieme boucle for est
	pilotee par l'indice de l'array; elle dessine horizontalement les barres 
	de chaque sous-groupe en suivant les marqueurs */

    if (changingDur) {
        testDur = DUR_8 + fullBars;
        barY = beamWidth;

        if (stemDir == STEMDIRECTION_up) {
            barY = -barY;
        }

        // loop
        while (testDur <= shortestDur) {
            // true at the beginning of a beam or after a breakSec
            bool start = true;
            
            // all but the last one
            for (i = 0; i < elementCount - 1; i++) {
                bool breakSec = (((*beamElementCoords)[i]->m_breaksec) && (testDur - DUR_8 >= (*beamElementCoords)[i]->m_breaksec));
                (*beamElementCoords)[i]->m_partialFlags[testDur-DUR_8] = PARTIAL_NONE;
                // partial is needed
                if ((*beamElementCoords)[i]->m_dur >= (char)testDur) {
                    // and for the next one too, but no break - through
                    if (((*beamElementCoords)[i+1]->m_dur >= (char)testDur) && !breakSec) {
                        (*beamElementCoords)[i]->m_partialFlags[testDur-DUR_8] = PARTIAL_THROUGH;
                    }
                    // not needed for the next one or break
                    else {
                        // we are starting a beam or after a beam break - put it right
                        if (start) {
                            (*beamElementCoords)[i]->m_partialFlags[testDur-DUR_8] = PARTIAL_RIGHT;
                        }
                        // or the previous one had no partial - put it left
                        else if ((*beamElementCoords)[i-1]->m_dur < (char)testDur) {
                            (*beamElementCoords)[i]->m_partialFlags[testDur-DUR_8] = PARTIAL_LEFT;
                        }
                    }
                }
                // not we are in a group
                if (breakSec) {
                    start = true;
                }
                else {
                    start = false;
                }
            }
            // last one
            (*beamElementCoords)[i]->m_partialFlags[testDur-DUR_8] = PARTIAL_NONE;
            // partial is needed
            if ((*beamElementCoords)[i]->m_dur >= (char)testDur) {
                // and the previous one had no partial - put it left
                if ((*beamElementCoords)[i-1]->m_dur < (char)testDur) {
                    (*beamElementCoords)[i]->m_partialFlags[testDur-DUR_8] = PARTIAL_LEFT;
                }
            }
            
            // draw them
            for (i=0; i<elementCount; i++) {
                if ((*beamElementCoords)[i]->m_partialFlags[testDur-DUR_8] == PARTIAL_THROUGH) {
                    // through should never be set on the last one
                    assert( i < elementCount - 1);
                    if (i >= elementCount - 1) continue; // assert for debug and skip otherwise
                    fy1 = (*beamElementCoords)[i]->m_yBeam + barY;
                    fy2 = (*beamElementCoords)[i+1]->m_yBeam + barY;
                    polygonHeight = beamWidthBlack * shiftY;
                    DrawObliquePolygon (dc, (*beamElementCoords)[i]->m_x, fy1,(*beamElementCoords)[i+1]->m_x, fy2, polygonHeight);
                }
                else if ((*beamElementCoords)[i]->m_partialFlags[testDur-DUR_8] == PARTIAL_RIGHT) {
                    fy1 = (*beamElementCoords)[i]->m_yBeam + barY;
                    int x2 = (*beamElementCoords)[i]->m_x + m_doc->m_drawingLedgerLine[staff->staffSize][0];
                    fy2 = startingY + verticalBoost + barY + beamSlope * x2;
                    polygonHeight= beamWidthBlack*shiftY;
                    DrawObliquePolygon (dc, (*beamElementCoords)[i]->m_x, fy1, x2, fy2, polygonHeight);
                }
                else if ((*beamElementCoords)[i]->m_partialFlags[testDur-DUR_8] == PARTIAL_LEFT) {
                    fy2 = (*beamElementCoords)[i]->m_yBeam + barY;
                    int x1 = (*beamElementCoords)[i]->m_x - m_doc->m_drawingLedgerLine[staff->staffSize][0];
                    fy1 = startingY + verticalBoost + barY + beamSlope * x1;
                    polygonHeight = beamWidthBlack*shiftY;
                    DrawObliquePolygon (dc, x1, fy1, (*beamElementCoords)[i]->m_x, fy2, polygonHeight);
                }
            }

            testDur += 1;
            barY += shiftY * beamWidth;
        } // end of while
    } // end of drawing partial bars
    
    dc->EndGraphic(element, this );
}
    
} // namespace vrv
