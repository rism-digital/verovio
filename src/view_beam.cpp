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
#include "rpt.h"
#include "smufl.h"
#include "staff.h"
#include "style.h"
#include "vrv.h"

namespace vrv {


void View::DrawBeam(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(layer);
    assert(staff);
    assert(measure);
    
    Beam *beam = dynamic_cast<Beam*>(element);
    assert(beam);
    
    LayerElement *current;

    BeamParams params;
	params.m_changingDur = OFF;
    params.m_beamHasChord = OFF;
    params.m_hasMultipleStemDir = OFF;
    params.m_cueSize = OFF;
    params.m_shortestDur = 0;
    params.m_stemDir = STEMDIRECTION_NONE;
    
    // position in the beam element list
	int elementCount, last;
    
    // duration variables
	int lastDur, currentDur, testDur;
    
    // temporary coordinates
    int x1, x2, y1, y2;
    
    // temporary variables
    int shiftY;
    int barY, fullBars, polygonHeight;
    double dy1, dy2;

    // loops
	int i, j;
    
    /******************************************************************/
    // initialization
    
    lastDur = elementCount = 0;
    
    ListOfObjects* beamChildren = beam->GetList(beam);
    
    // Should we assert this at the beginning?
    if (beamChildren->empty()) {
        return;
    }
    const ArrayOfBeamElementCoords *beamElementCoords = beam->GetElementCoords();
    
    // current point to the first Note in the layed out layer
    current = dynamic_cast<LayerElement*>(beamChildren->front());
    // Beam list should contain only DurationInterface objects
    assert(dynamic_cast<DurationInterface*>(current));
    
    lastDur = dynamic_cast<DurationInterface*>(current)->GetActualDur();

    
    /******************************************************************/
    // Populate BeamElementCoord for each element in the beam
    // This could be moved to Beam::InitCoord for optimization because there should be no
    // need of redoing it everytime it is drawn.
    
    data_STEMDIRECTION currentStemDir;
    
    ListOfObjects::iterator iter = beamChildren->begin();
	do {
        // Beam list should contain only DurationInterface objects
        assert(dynamic_cast<DurationInterface*>(current));
        currentDur = dynamic_cast<DurationInterface*>(current)->GetActualDur();
        
        if (current->Is() == CHORD) {
            params.m_beamHasChord = true;
        }

        // Can it happen? With rests?
		if (currentDur > DUR_4) {
			(*beamElementCoords).at(elementCount)->m_element = current;
            current->m_beamElementCoord = (*beamElementCoords).at(elementCount);
            (*beamElementCoords).at(elementCount)->m_x = current->GetDrawingX();
			(*beamElementCoords).at(elementCount)->m_dur = currentDur;
            
            // Look at beam breaks
            (*beamElementCoords).at(elementCount)->m_breaksec = 0;
            AttBeamsecondary *beamsecondary = dynamic_cast<AttBeamsecondary*>(current);
            if (elementCount && beamsecondary && beamsecondary->HasBreaksec()) {
                if (!params.m_changingDur) params.m_changingDur = ON;
                (*beamElementCoords).at(elementCount)->m_breaksec = beamsecondary->GetBreaksec();
            }
            
            // Skip rests
            if ((current->Is() == NOTE) || (current->Is() == CHORD)) {
                // look at the stemDir to see if we have multiple stem Dir
                if (!params.m_hasMultipleStemDir) {
                    assert(dynamic_cast<AttStems*>(current));
                    currentStemDir = dynamic_cast<AttStems*>(current)->GetStemDir();
                    if (currentStemDir != STEMDIRECTION_NONE) {
                        if ((params.m_stemDir != STEMDIRECTION_NONE) && (params.m_stemDir != currentStemDir)) {
                            params.m_hasMultipleStemDir = ON;
                        }
                    }
                    params.m_stemDir = currentStemDir;
                }
                // keep the shortest dur in the beam
                params.m_shortestDur = std::max(currentDur,params.m_shortestDur);
                // check if we have more than duration in the beam
                if (!params.m_changingDur && currentDur != lastDur) params.m_changingDur = ON;
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
        LogDebug("Beam with no notes of duration > 8 detected. Exiting DrawBeam.");
        return;
    }

	last = elementCount - 1;
    
    // We look only at the last note for checking if cuesized. Somehow arbitrarily
    params.m_cueSize = (*beamElementCoords).at(last)->m_element->IsCueSize();

    /******************************************************************/
    // Calculate the beam slope and position
    
    CalcBeam(layer, staff, beamElementCoords, elementCount, &params);
    
    /******************************************************************/
    // Start the Beam graphic and draw the children
    
    dc->StartGraphic(element, "", element->GetUuid());
    
    
    /******************************************************************/
    // Draw the children
    
    DrawLayerChildren(dc, beam, layer, staff, measure);

    /******************************************************************/
    // Draw the stems and the beam full bars
    
    for (i=0; i<elementCount; i++)
    {
        LayerElement *el = (*beamElementCoords).at(i)->m_element;
        if(((el->Is() == NOTE) && !dynamic_cast<Note*>(el)->IsChordTone()) || (el->Is() == CHORD)) {
            StemmedDrawingInterface *interface = dynamic_cast<StemmedDrawingInterface*>(el);
            assert(interface);
            
            DrawVerticalLine (dc, interface->GetDrawingStemStart().y, interface->GetDrawingStemEnd().y,
                              interface->GetDrawingStemStart().x, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize));
        }
    }
    
    // Number of bars to draw - if we do not have changing values, draw
    // the number of bars according to the shortestDur value. Otherwise draw
    // only one bar and the others will be drawn separately.
    fullBars =  !params.m_changingDur ? (params.m_shortestDur - DUR_4) : 1;
    
    // Adjust the x position of the first and last element for taking into account the stem width
	(*beamElementCoords).at(0)->m_x -= (m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;
	(*beamElementCoords).at(last)->m_x += (m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;

    // Shift direction
	shiftY = (params.m_stemDir == STEMDIRECTION_down) ? 1.0 : -1.0;

	y1 = (*beamElementCoords).at(0)->m_yBeam;
    y2 = (*beamElementCoords).at(last)->m_yBeam;

    x1 = (*beamElementCoords).at(0)->m_x;
    x2 = (*beamElementCoords).at(last)->m_x;

	dy1 = shiftY;
	dy2 = shiftY;
    
    // For acc and rit beam (see AttBeamingVis set
    // s_y = 0 and s_y2 = 0 respectively

	for (j = 0; j < fullBars ; j++) {
		polygonHeight = params.m_beamWidthBlack * shiftY;
        DrawObliquePolygon (dc, x1, y1, x2, y2, polygonHeight);
		y1 += polygonHeight; y2 += polygonHeight;

        // dy1 must == 0 for accelerando beams
		if (!dy1) y1 += (params.m_beamWidthBlack * shiftY) * -1;
		else y1 += dy1 * params.m_beamWidthWhite;
        
        // reverse for retardendo beam
		if (!dy2) y2 += (params.m_beamWidthBlack * shiftY) * -1;
		else y2 += dy2 * params.m_beamWidthWhite;
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

    if (params.m_changingDur) {
        testDur = DUR_8 + fullBars;
        barY = params.m_beamWidth;

        if (params.m_stemDir == STEMDIRECTION_up) {
            barY = -barY;
        }
        
        int fractBeamWidth = m_doc->GetGlyphWidth(SMUFL_E0A3_noteheadHalf, staff->m_drawingStaffSize, params.m_cueSize) * 7 / 10;

        // loop
        while (testDur <= params.m_shortestDur) {
            // true at the beginning of a beam or after a breakSec
            bool start = true;
            
            // all but the last one
            for (i = 0; i < elementCount - 1; i++) {
                bool breakSec = (((*beamElementCoords).at(i)->m_breaksec) && (testDur - DUR_8 >= (*beamElementCoords).at(i)->m_breaksec));
                (*beamElementCoords).at(i)->m_partialFlags[testDur-DUR_8] = PARTIAL_NONE;
                // partial is needed
                if ((*beamElementCoords).at(i)->m_dur >= (char)testDur) {
                    // and for the next one too, but no break - through
                    if (((*beamElementCoords).at(i+1)->m_dur >= (char)testDur) && !breakSec) {
                        (*beamElementCoords).at(i)->m_partialFlags[testDur-DUR_8] = PARTIAL_THROUGH;
                    }
                    // not needed for the next one or break
                    else {
                        // we are starting a beam or after a beam break - put it right
                        if (start) {
                            (*beamElementCoords).at(i)->m_partialFlags[testDur-DUR_8] = PARTIAL_RIGHT;
                        }
                        // or the previous one had no partial - put it left
                        else if ((*beamElementCoords).at(i-1)->m_dur < (char)testDur) {
                            (*beamElementCoords).at(i)->m_partialFlags[testDur-DUR_8] = PARTIAL_LEFT;
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
            (*beamElementCoords).at(i)->m_partialFlags[testDur-DUR_8] = PARTIAL_NONE;
            // partial is needed
            if ((*beamElementCoords).at(i)->m_dur >= (char)testDur) {
                // and the previous one had no partial - put it left
                if ((*beamElementCoords).at(i-1)->m_dur < (char)testDur) {
                    (*beamElementCoords).at(i)->m_partialFlags[testDur-DUR_8] = PARTIAL_LEFT;
                }
            }
            
            // draw them
            for (i = 0; i < elementCount; i++) {
                if ((*beamElementCoords).at(i)->m_partialFlags[testDur-DUR_8] == PARTIAL_THROUGH) {
                    // through should never be set on the last one
                    assert(i < elementCount - 1);
                    if (i >= elementCount - 1) continue; // assert for debug and skip otherwise
                    y1 = (*beamElementCoords).at(i)->m_yBeam + barY;
                    y2 = (*beamElementCoords).at(i+1)->m_yBeam + barY;
                    polygonHeight = params.m_beamWidthBlack * shiftY;
                    DrawObliquePolygon (dc, (*beamElementCoords).at(i)->m_x, y1,(*beamElementCoords).at(i+1)->m_x, y2, polygonHeight);
                }
                else if ((*beamElementCoords).at(i)->m_partialFlags[testDur-DUR_8] == PARTIAL_RIGHT) {
                    y1 = (*beamElementCoords).at(i)->m_yBeam + barY;
                    int x2 = (*beamElementCoords).at(i)->m_x + fractBeamWidth;
                    y2 = params.m_startingY + params.m_verticalBoost + barY + params.m_beamSlope * x2;
                    polygonHeight= params.m_beamWidthBlack * shiftY;
                    DrawObliquePolygon (dc, (*beamElementCoords).at(i)->m_x, y1, x2, y2, polygonHeight);
                }
                else if ((*beamElementCoords).at(i)->m_partialFlags[testDur-DUR_8] == PARTIAL_LEFT) {
                    y2 = (*beamElementCoords).at(i)->m_yBeam + barY;
                    int x1 = (*beamElementCoords).at(i)->m_x - fractBeamWidth;
                    y1 = params.m_startingY + params.m_verticalBoost + barY + params.m_beamSlope * x1;
                    polygonHeight = params.m_beamWidthBlack * shiftY;
                    DrawObliquePolygon (dc, x1, y1, (*beamElementCoords).at(i)->m_x, y2, polygonHeight);
                }
            }

            testDur += 1;
            barY += shiftY * params.m_beamWidth;
        } // end of while
    } // end of drawing partial bars
    
    dc->EndGraphic(element, this);
}

void View::DrawFTrem(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);
    
    
    // temporary coordinates
    int x1, x2, y1, y2;
    
    // temporary variables
    int shiftY;
    int fullBars, polygonHeight;
    double dy1, dy2;
    
    // loop
    int i, j;
    
    FTrem *fTrem = dynamic_cast<FTrem*>(element);
    assert(fTrem);
    
    ArrayOfBeamElementCoords beamElementCoords;
    BeamElementCoord firstElement;
    BeamElementCoord secondElement;
    beamElementCoords.push_back(&firstElement);
    beamElementCoords.push_back(&secondElement);
    
    int elementCount = 2;
    
    ListOfObjects* fTremChildren = fTrem->GetList(fTrem);
    
    // Should we assert this at the beginning?
    if (fTremChildren->empty()) {
        return;
    }
    // current point to the first Note in the layed out layer
    firstElement.m_element = dynamic_cast<LayerElement*>(fTremChildren->front());
    // fTrem list should contain only DurationInterface objects
    assert(dynamic_cast<DurationInterface*>(firstElement.m_element));
    // current point to the first Note in the layed out layer
    secondElement.m_element = dynamic_cast<LayerElement*>(fTremChildren->back());
    // fTrem list should contain only DurationInterface objects
    assert(dynamic_cast<DurationInterface*>(secondElement.m_element));
    // Should we assert this at the beginning?
    if (firstElement.m_element == secondElement.m_element) {
        return;
    }
    
    BeamParams params;
    params.m_changingDur = OFF;
    params.m_beamHasChord = OFF;
    params.m_hasMultipleStemDir = OFF;
    params.m_cueSize = OFF;
    // adjust params.m_shortestDur depending on the number of slashes
    params.m_shortestDur = std::max(DUR_8, DUR_1 + fTrem->GetSlash());
    params.m_stemDir = STEMDIRECTION_NONE;
    
    // We look only at the first one for the duration since both are expected to be the same
    assert(dynamic_cast<AttDurationMusical*>(firstElement.m_element));
    int dur =  dynamic_cast<AttDurationMusical*>(firstElement.m_element)->GetDur();
    
    Chord *childChord1 = NULL;
    Chord *childChord2 = NULL;
    
    if (firstElement.m_element->Is() == CHORD) {
        childChord1 = dynamic_cast<Chord*>(firstElement.m_element);
        params.m_beamHasChord = ON;
    }
    if (secondElement.m_element->Is() == CHORD) {
        childChord2 = dynamic_cast<Chord*>(secondElement.m_element);
        params.m_beamHasChord = ON;
    }

    // For now look at the stemDir only on the first note
    assert(dynamic_cast<AttStems*>(firstElement.m_element));
    params.m_stemDir =  dynamic_cast<AttStems*>(firstElement.m_element)->GetStemDir();
    
    // We look only at the first note for checking if cuesized. Somehow arbitrarily
    params.m_cueSize = firstElement.m_element->IsCueSize();
    
    // x positions
    firstElement.m_x = firstElement.m_element->GetDrawingX();
    secondElement.m_x = secondElement.m_element->GetDrawingX();
    
    /******************************************************************/
    // Calculate the beam slope and position
    
    CalcBeam(layer, staff, &beamElementCoords, elementCount, &params);
    
    
    /******************************************************************/
    // Start the grahic
    
    dc->StartGraphic(element, "", element->GetUuid());
    
    /******************************************************************/
    // Draw the children
    
    DrawLayerChildren(dc, fTrem, layer, staff, measure);
    
    /******************************************************************/
    // Draw the stems and the bars
    
    if (dur > DUR_1) {
        for (i=0; i<elementCount; i++)
        {
            LayerElement *el = beamElementCoords.at(i)->m_element;
            if(((el->Is() == NOTE) && !dynamic_cast<Note*>(el)->IsChordTone()) || (el->Is() == CHORD)) {
                StemmedDrawingInterface *interface = dynamic_cast<StemmedDrawingInterface*>(el);
                assert(interface);
                DrawVerticalLine (dc, interface->GetDrawingStemStart().y, interface->GetDrawingStemEnd().y,
                                  interface->GetDrawingStemStart().x, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize));
            }
        }
    }
    
    // Number of bars to draw - if we do not have changing values, draw
    // the number of bars according to the shortestDur value. Otherwise draw
    // only one bar and the others will be drawn separately.
    fullBars = fTrem->GetSlash();
    
    // Adjust the x position of the first and last element for taking into account the stem width
    firstElement.m_x -= (m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;
    secondElement.m_x += (m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;
    
    // Shift direction
    shiftY = (params.m_stemDir == STEMDIRECTION_down) ? 1.0 : -1.0;
    polygonHeight = params.m_beamWidthBlack * shiftY;
    
    y1 = firstElement.m_yBeam;
    y2 = secondElement.m_yBeam;
    
    x1 = firstElement.m_x;
    x2 = secondElement.m_x;
    
    dy1 = shiftY;
    dy2 = shiftY;
    
    int space = m_doc->GetDrawingBeamWidth(staff->m_drawingStaffSize, params.m_cueSize);
    // for non stem notes the bar should be shortenend
    if (dur < DUR_2) {
        x1 += 2 * space;
        y1 += 2 * space * params.m_beamSlope;
        x2 -= 2 * space;
        y2 -= 2 * space * params.m_beamSlope;
    }
    else if (dur == DUR_4) {
        x1 += space;
        y1 += space * params.m_beamSlope;
        x2 -= space;
        y2 -= space * params.m_beamSlope;
    }

    
    for (j = 0; j < fullBars ; j++) {
        DrawObliquePolygon (dc, x1, y1, x2, y2, polygonHeight);
        y1 += polygonHeight;
        y2 += polygonHeight;
        y1 += dy1 * params.m_beamWidthWhite;
        y2 += dy2 * params.m_beamWidthWhite;
        
        // shorten the bar after having drawn the first one (but the first one)
        if ((j == 0) && (dur > DUR_1) && (dur != DUR_4)) {
            x1 += space;
            y1 += space * params.m_beamSlope;
            x2 -= space;
            y2 -= space * params.m_beamSlope;
        }
    }
    
    dc->EndGraphic(element, this);
}
    
void View::CalcBeam(Layer *layer, Staff *staff, const ArrayOfBeamElementCoords *beamElementCoords, int elementCount, BeamParams *params)
{
    assert(layer);
    assert(staff);
    assert(beamElementCoords);
    
    int y1, y2, avgY, yExtreme, high, low, verticalCenter, verticalShift;
    double xr, verticalShiftFactor;
    
    // loop
    int i;
    
    // position x for the stem (normal and cue size)
    int stemX[2];
    
    // For slope calculation and linear regression
    double s_x = 0.0; // sum of all x(n) for n in beamElementCoord
    double s_y = 0.0; // sum of all y(n)
    double s_xy = 0.0; // sum of (x(n) * y(n))
    double s_x2 = 0.0; // sum of all x(n)^2
    double s_y2 = 0.0; // sum of all y(n)^2
    
    /******************************************************************/
    // initialization
    
    high = avgY = 0.0;
    low = -VRV_UNSET;
    params->m_verticalBoost = 0.0;
    
    verticalShiftFactor = 3.0;
    verticalCenter = staff->GetDrawingY() - (m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * 2); //center point of the staff
    yExtreme = verticalCenter; //value of farthest y point on the staff from verticalCenter minus verticalCenter; used if beamHasChord = ON
    
    int last = elementCount - 1;
    
    // x-offset values for stem bases, dx[y] where y = element->m_cueSize
    stemX[0] =  m_doc->GetGlyphWidth(SMUFL_E0A3_noteheadHalf, staff->m_drawingStaffSize, false) / 2 - (m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;
    stemX[1] =  m_doc->GetGlyphWidth(SMUFL_E0A3_noteheadHalf, staff->m_drawingStaffSize, true) / 2 - (m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;
    
    /******************************************************************/
    // Calculate the extreme values
    
    int yMax = 0, yMin = 0;
    int curY;
    // elementCount holds the last one
    for (i = 0; i < elementCount; i++) {
        
        
        if ((*beamElementCoords).at(i)->m_element->Is() == CHORD) {
            Chord *chord = dynamic_cast<Chord*>((*beamElementCoords).at(i)->m_element);
            assert(chord);
            chord->GetYExtremes(&yMax, &yMin);
            (*beamElementCoords).at(i)->m_yTop = yMax;
            (*beamElementCoords).at(i)->m_yBottom = yMin;
            
            avgY += (*beamElementCoords).at(i)->m_y + ((yMax - yMin) / 2);
            
            // highest and lowest value;
            high= std::max(yMax, high);
            low = std::min(yMin, low);
        }
        else {
            (*beamElementCoords).at(i)->m_y = (*beamElementCoords).at(i)->m_element->GetDrawingY();
            
            // highest and lowest value;
            high= std::max((*beamElementCoords).at(i)->m_y, high);
            low = std::min((*beamElementCoords).at(i)->m_y, low);
            
            curY = (*beamElementCoords).at(i)->m_element->GetDrawingY();
            (*beamElementCoords).at(i)->m_y = curY;
            (*beamElementCoords).at(i)->m_yTop = curY;
            (*beamElementCoords).at(i)->m_yBottom = curY;
            avgY += (*beamElementCoords).at(i)->m_y;
        }
    }
    
    /******************************************************************/
    // Set the stem direction
    
    yExtreme = (abs(high - verticalCenter) > abs(low - verticalCenter) ? high : low);
    avgY /= elementCount;
    
    // If we have one stem direction in the beam, then don't look at the layer
    if (params->m_stemDir == STEMDIRECTION_NONE) params->m_stemDir = layer->GetDrawingStemDir(); // force layer direction if it exists
    
    // Automatic stem direction if nothing in the notes or in the layer
    if (params->m_stemDir == STEMDIRECTION_NONE) {
        if (params->m_beamHasChord) params->m_stemDir = (yExtreme < verticalCenter) ?  STEMDIRECTION_up : STEMDIRECTION_down; //if it has a chord, go by the most extreme position
        else params->m_stemDir = (avgY <  verticalCenter) ? STEMDIRECTION_up : STEMDIRECTION_down; //otherwise go by average
    }
    
    if (params->m_stemDir == STEMDIRECTION_up) { //set stem direction for all the notes
        for (i = 0; i < elementCount; i++) {
            (*beamElementCoords).at(i)->m_y = (*beamElementCoords).at(i)->m_yTop;
        }
    }
    else {
        for (i = 0; i < elementCount; i++) {
            (*beamElementCoords).at(i)->m_y = (*beamElementCoords).at(i)->m_yBottom;
        }
    }
    
    params->m_beamWidthBlack = m_doc->GetDrawingBeamWidth(staff->m_drawingStaffSize, params->m_cueSize);
    params->m_beamWidthWhite = m_doc->GetDrawingBeamWhiteWidth(staff->m_drawingStaffSize, params->m_cueSize);
    params->m_beamWidth = params->m_beamWidthBlack + params->m_beamWidthWhite;
    
    /******************************************************************/
    // Calculate the slope doing a linear regression
    
    // The vertical shift depends on the shortestDur value we have in the beam
    verticalShift = ((params->m_shortestDur - DUR_8) * (params->m_beamWidth));
    
    //if the beam has smaller-size notes
    if ((*beamElementCoords).at(last)->m_element->IsCueSize()) {
        verticalShift += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 5;
    }
    else {
        verticalShift += (params->m_shortestDur > DUR_8) ?
        m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * verticalShiftFactor :
        m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * (verticalShiftFactor + 0.5);
    }
    
    // swap x position and verticalShift direction with stem down
    if (params->m_stemDir == STEMDIRECTION_down) {
        stemX[0] = -stemX[0];
        stemX[1] = -stemX[1];
        verticalShift = -verticalShift;
    }
    
    for (i = 0; i < elementCount; i++)
    {
        //change the stem dir for all objects
        if ((*beamElementCoords).at(i)->m_element->Is() == NOTE) {
            ((Note*)(*beamElementCoords).at(i)->m_element)->SetDrawingStemDir(params->m_stemDir);
        }
        else if ((*beamElementCoords).at(i)->m_element->Is() == CHORD) {
            ((Chord*)(*beamElementCoords).at(i)->m_element)->SetDrawingStemDir(params->m_stemDir);
        }
        
        (*beamElementCoords).at(i)->m_yBeam = (*beamElementCoords).at(i)->m_y + verticalShift;
        (*beamElementCoords).at(i)->m_x +=  stemX[params->m_cueSize];
        
        s_y += (*beamElementCoords).at(i)->m_yBeam;
        s_y2 += (*beamElementCoords).at(i)->m_yBeam * (*beamElementCoords).at(i)->m_yBeam;
        s_x += (*beamElementCoords).at(i)->m_x;
        s_x2 += (*beamElementCoords).at(i)->m_x * (*beamElementCoords).at(i)->m_x;
        s_xy += (*beamElementCoords).at(i)->m_x * (*beamElementCoords).at(i)->m_yBeam;
    }
    
    y1 = elementCount * s_xy - s_x * s_y;
    xr = elementCount * s_x2 - s_x * s_x;
    
    // Prevent division by 0
    if (y1 && xr) {
        params->m_beamSlope = y1 / xr;
    }
    else {
        params->m_beamSlope = 0.0;
    }
    
    /* Correction esthetique : */
    if (fabs(params->m_beamSlope) < m_doc->m_drawingBeamMinSlope) params->m_beamSlope = 0.0;
    if (fabs(params->m_beamSlope) > m_doc->m_drawingBeamMaxSlope) params->m_beamSlope = (params->m_beamSlope > 0) ? m_doc->m_drawingBeamMaxSlope : - m_doc->m_drawingBeamMaxSlope;
    /* pente correcte: entre 0 et env 0.4 (0.2 a 0.4) */
    
    params->m_startingY = (s_y - params->m_beamSlope * s_x) / elementCount;
    
    /******************************************************************/
    // Calculate the stem lengths
    
    // first check that the stem length is long enough (to be improved?)
    double oldYPos; //holds y position before calculation to determine if beam needs extra height
    double expectedY;
    for (i = 0; i < elementCount; i++) {
        oldYPos = (*beamElementCoords).at(i)->m_yBeam;
        expectedY = params->m_startingY + params->m_verticalBoost + params->m_beamSlope * (*beamElementCoords).at(i)->m_x;
        
        //if the stem is not long enough, add extra stem length needed to all members of the beam
        if ((params->m_stemDir == STEMDIRECTION_up && (oldYPos > expectedY)) || (params->m_stemDir == STEMDIRECTION_down && (oldYPos < expectedY))) {
            params->m_verticalBoost += oldYPos - expectedY;
        }
    }
    for (i = 0; i < elementCount; i++) {
        (*beamElementCoords).at(i)->m_yBeam =  params->m_startingY + params->m_verticalBoost + params->m_beamSlope * (*beamElementCoords).at(i)->m_x;
    }

    // then check that the stem length reaches the center for the staff
    double minDistToCenter = -VRV_UNSET;
    for (i = 0; i < elementCount; i++) {
        if ((params->m_stemDir == STEMDIRECTION_up) && ((*beamElementCoords).at(i)->m_yBeam - verticalCenter < minDistToCenter)) {
            minDistToCenter = (*beamElementCoords).at(i)->m_yBeam - verticalCenter;
        }
        else if ((params->m_stemDir == STEMDIRECTION_down) && (verticalCenter - (*beamElementCoords).at(i)->m_yBeam < minDistToCenter)) {
            minDistToCenter = verticalCenter - (*beamElementCoords).at(i)->m_yBeam;
        }
    }
    minDistToCenter += (params->m_beamWidthBlack / 2) + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 4;
    if (minDistToCenter < 0) {
        for (i = 0; i < elementCount; i++) {
            (*beamElementCoords).at(i)->m_yBeam +=  (params->m_stemDir == STEMDIRECTION_down) ? minDistToCenter : -minDistToCenter;
        }
    }
    
    for (i=0; i<elementCount; i++) {
        if (params->m_stemDir == STEMDIRECTION_up) {
            y1 = (*beamElementCoords).at(i)->m_yBeam - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            y2 = (*beamElementCoords).at(i)->m_yBottom + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 4;
        }
        else {
            y1 = (*beamElementCoords).at(i)->m_yBeam + m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            y2 = (*beamElementCoords).at(i)->m_yTop - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 4;
        }
        
        // All notes and chords get their stem value stored
        LayerElement *el = (*beamElementCoords).at(i)->m_element;
        if ((el->Is() == NOTE) || (el->Is() == CHORD)) {
            StemmedDrawingInterface *interface = dynamic_cast<StemmedDrawingInterface*>(el);
            assert(interface);
            
            interface->SetDrawingStemDir(params->m_stemDir);
            interface->SetDrawingStemStart(Point((*beamElementCoords).at(i)->m_x, y2));
            interface->SetDrawingStemEnd(Point((*beamElementCoords).at(i)->m_x, y1));
        }
    }
}
    
} // namespace vrv
