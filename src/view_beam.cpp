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
#include "ftrem.h"
#include "layer.h"
#include "layerelement.h"
#include "measure.h"
#include "note.h"
#include "options.h"
#include "smufl.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

void View::DrawBeam(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(layer);
    assert(staff);
    assert(measure);

    Beam *beam = dynamic_cast<Beam *>(element);
    assert(beam);
    
    Staff *beamStaff = staff;
    if (beam->GetBeamWith() == OTHERSTAFF_below) {
        beamStaff = dynamic_cast<Staff*>(measure->GetNext(staff, STAFF));
        if (beamStaff == NULL) {
            LogError("Cannot access staff below for beam '%s'", beam->GetUuid().c_str());
            beamStaff = staff;
        }
    }
    else if (beam->GetBeamWith() == OTHERSTAFF_above) {
        beamStaff = dynamic_cast<Staff*>(measure->GetPrevious(staff, STAFF));
        if (beamStaff == NULL) {
            LogError("Cannot access staff above for beam '%s'", beam->GetUuid().c_str());
            beamStaff = staff;
        }
    }

    // temporary coordinates
    int x1, x2, y1, y2;

    // temporary variables
    int shiftY;
    int fullBars, polygonHeight;
    double dy1, dy2;

    // loops
    int i, j;

    /******************************************************************/
    // initialization

    const ListOfObjects *beamChildren = beam->GetList(beam);

    // Should we assert this at the beginning?
    if (beamChildren->empty()) {
        return;
    }
    const ArrayOfBeamElementCoords *beamElementCoords = beam->GetElementCoords();

    int elementCount = (int)beamChildren->size();
    int last = elementCount - 1;

    /******************************************************************/
    // Calculate the beam slope and position

    beam->m_drawingParams.CalcBeam(layer, beamStaff, m_doc, beamElementCoords, elementCount);

    /******************************************************************/
    // Start the Beam graphic and draw the children

    dc->StartGraphic(element, "", element->GetUuid());

    /******************************************************************/
    // Draw the children

    DrawLayerChildren(dc, beam, layer, staff, measure);

    /******************************************************************/
    // Draw the beam full bars

    // Number of bars to draw - if we do not have changing values, draw
    // the number of bars according to the shortestDur value. Otherwise draw
    // only one bar and the others will be drawn separately.
    fullBars = !beam->m_drawingParams.m_changingDur ? (beam->m_drawingParams.m_shortestDur - DUR_4) : 1;

    // Adjust the x position of the first and last element for taking into account the stem width
    beamElementCoords->at(0)->m_x -= (m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;
    beamElementCoords->at(last)->m_x += (m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;

    // Shift direction
    shiftY = (beam->m_drawingParams.m_stemDir == STEMDIRECTION_down) ? 1.0 : -1.0;

    y1 = beamElementCoords->at(0)->m_yBeam;
    y2 = beamElementCoords->at(last)->m_yBeam;

    x1 = beamElementCoords->at(0)->m_x;
    x2 = beamElementCoords->at(last)->m_x;

    dy1 = shiftY;
    dy2 = shiftY;

    // For acc and rit beam (see AttBeamingVis set
    // s_y = 0 and s_y2 = 0 respectively

    for (j = 0; j < fullBars; ++j) {
        polygonHeight = beam->m_drawingParams.m_beamWidthBlack * shiftY;
        DrawObliquePolygon(dc, x1, y1, x2, y2, polygonHeight);
        y1 += polygonHeight;
        y2 += polygonHeight;

        // dy1 must == 0 for accelerando beams
        if (!dy1)
            y1 += (beam->m_drawingParams.m_beamWidthBlack * shiftY) * -1;
        else
            y1 += dy1 * beam->m_drawingParams.m_beamWidthWhite;

        // reverse for retardendo beam
        if (!dy2)
            y2 += (beam->m_drawingParams.m_beamWidthBlack * shiftY) * -1;
        else
            y2 += dy2 * beam->m_drawingParams.m_beamWidthWhite;
    }

    /******************************************************************/
    // Draw the beam for partial bars (if any)

    /* calcul des x en cas de beaming multiple */
    /* parcours horizontal ajoutant barres en fonction de m_dur la plus
    breve (shortestDur), controle par boucle while; la premiere boucle for
    controle le nombre d'etapes horizontales du parcours (par le nombre
    de commandes '+' enregistrees); la deuxieme boucle for teste pour
    chaque paquet entre deux '+' les valeurs; elle construit une array de
    marqueurs partitionnant les sous-groupes; la troisieme boucle for est
    pilotee par l'indice de l'array; elle dessine horizontalement les barres
    de chaque sous-groupe en suivant les marqueurs */

    // Map the indexes of the notes/chords since we need to ignore rests when drawing partials
    // However, exception for the first and last element of a beam
    std::vector<int> noteIndexes;
    for (i = 0; i < elementCount; ++i) {
        if (beamElementCoords->at(i)->m_element->Is(REST))
            if (i > 0 && i < elementCount - 1) continue;
        noteIndexes.push_back(i);
    }
    int noteCount = (int)noteIndexes.size();

    if (beam->m_drawingParams.m_changingDur && noteCount > 0) {
        int testDur = DUR_8 + fullBars;
        int barY = beam->m_drawingParams.m_beamWidth;

        if (beam->m_drawingParams.m_stemDir == STEMDIRECTION_up) {
            barY = -barY;
        }

        int fractBeamWidth
            = m_doc->GetGlyphWidth(SMUFL_E0A3_noteheadHalf, staff->m_drawingStaffSize, beam->m_drawingParams.m_cueSize)
            * 7 / 10;

        // loop
        while (testDur <= beam->m_drawingParams.m_shortestDur) {
            // true at the beginning of a beam or after a breakSec
            bool start = true;

            int idx = 0;

            // all but the last one
            for (i = 0; i < noteCount - 1; ++i) {
                idx = noteIndexes.at(i);
                int nextIdx = noteIndexes.at(i + 1);

                bool breakSec = ((beamElementCoords->at(idx)->m_breaksec)
                    && (testDur - DUR_8 >= beamElementCoords->at(idx)->m_breaksec));
                beamElementCoords->at(idx)->m_partialFlags[testDur - DUR_8] = PARTIAL_NONE;
                // partial is needed
                if (beamElementCoords->at(idx)->m_dur >= (char)testDur) {
                    // and for the next one too, but no break - through
                    if ((beamElementCoords->at(nextIdx)->m_dur >= (char)testDur) && !breakSec) {
                        beamElementCoords->at(idx)->m_partialFlags[testDur - DUR_8] = PARTIAL_THROUGH;
                    }
                    // not needed for the next one or break
                    else if (!beamElementCoords->at(idx)->m_element->Is(REST)) {
                        // we are starting a beam or after a beam break - put it right
                        if (start) {
                            beamElementCoords->at(idx)->m_partialFlags[testDur - DUR_8] = PARTIAL_RIGHT;
                        }
                        // or the previous one had no partial
                        else if (beamElementCoords->at(noteIndexes.at(i - 1))->m_dur < (char)testDur) {
                            // if we are at the full bar level, put it left
                            if (testDur == DUR_8 + fullBars) {
                                beamElementCoords->at(idx)->m_partialFlags[testDur - DUR_8] = PARTIAL_LEFT;
                            }
                            // if the previous level underneath was a partial through, put it left
                            else if (beamElementCoords->at(noteIndexes.at(i - 1))->m_partialFlags[testDur - 1 - DUR_8]
                                == PARTIAL_THROUGH) {
                                beamElementCoords->at(idx)->m_partialFlags[testDur - DUR_8] = PARTIAL_LEFT;
                            }
                            // if the level underneath was not left (right or through), put it right
                            else if (beamElementCoords->at(idx)->m_partialFlags[testDur - 1 - DUR_8] != PARTIAL_LEFT) {
                                beamElementCoords->at(idx)->m_partialFlags[testDur - DUR_8] = PARTIAL_RIGHT;
                            }
                            // it was put left before, put it left
                            else {
                                beamElementCoords->at(idx)->m_partialFlags[testDur - DUR_8] = PARTIAL_LEFT;
                            }
                        }
                    }
                }
                // we are not in a group
                if (breakSec) {
                    start = true;
                }
                else {
                    start = false;
                }
            }
            // last one
            idx = (int)noteIndexes.back();
            beamElementCoords->at(idx)->m_partialFlags[testDur - DUR_8] = PARTIAL_NONE;
            // partial is needed
            if ((beamElementCoords->at(idx)->m_dur >= (char)testDur)) {
                // and the previous one had no partial - put it left
                if ((noteCount == 1) || (beamElementCoords->at(noteIndexes.at(i - 1))->m_dur < (char)testDur)
                    || start) {
                    beamElementCoords->at(idx)->m_partialFlags[testDur - DUR_8] = PARTIAL_LEFT;
                }
            }

            // draw them
            for (i = 0; i < noteCount; ++i) {
                idx = noteIndexes.at(i);
                if (beamElementCoords->at(idx)->m_partialFlags[testDur - DUR_8] == PARTIAL_THROUGH) {
                    // through should never be set on the last one
                    assert(i < noteCount - 1);
                    if (i >= noteCount - 1) continue; // assert for debug and skip otherwise
                    y1 = beamElementCoords->at(idx)->m_yBeam + barY;
                    y2 = beamElementCoords->at(noteIndexes.at(i + 1))->m_yBeam + barY;
                    polygonHeight = beam->m_drawingParams.m_beamWidthBlack * shiftY;
                    DrawObliquePolygon(dc, beamElementCoords->at(idx)->m_x, y1,
                        beamElementCoords->at(noteIndexes.at(i + 1))->m_x, y2, polygonHeight);
                }
                else if (beamElementCoords->at(idx)->m_partialFlags[testDur - DUR_8] == PARTIAL_RIGHT) {
                    y1 = beamElementCoords->at(idx)->m_yBeam + barY;
                    int x2 = beamElementCoords->at(idx)->m_x + fractBeamWidth;
                    y2 = beam->m_drawingParams.m_startingY + barY
                        + beam->m_drawingParams.m_beamSlope * (x2 - beam->m_drawingParams.m_startingX);
                    polygonHeight = beam->m_drawingParams.m_beamWidthBlack * shiftY;
                    DrawObliquePolygon(dc, beamElementCoords->at(idx)->m_x, y1, x2, y2, polygonHeight);
                }
                else if (beamElementCoords->at(idx)->m_partialFlags[testDur - DUR_8] == PARTIAL_LEFT) {
                    y2 = beamElementCoords->at(idx)->m_yBeam + barY;
                    int x1 = beamElementCoords->at(idx)->m_x - fractBeamWidth;
                    y1 = beam->m_drawingParams.m_startingY + barY
                        + beam->m_drawingParams.m_beamSlope * (x1 - beam->m_drawingParams.m_startingX);
                    polygonHeight = beam->m_drawingParams.m_beamWidthBlack * shiftY;
                    DrawObliquePolygon(dc, x1, y1, beamElementCoords->at(idx)->m_x, y2, polygonHeight);
                }
            }

            testDur += 1;
            barY += shiftY * beam->m_drawingParams.m_beamWidth;
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

    FTrem *fTrem = dynamic_cast<FTrem *>(element);
    assert(fTrem);

    // temporary coordinates
    int x1, x2, y1, y2;

    // temporary variables
    int shiftY;
    int fullBars, polygonHeight;
    double dy1, dy2;

    /******************************************************************/
    // initialization

    const ListOfObjects *fTremChildren = fTrem->GetList(fTrem);

    // Should we assert this at the beginning?
    if (fTremChildren->empty()) {
        return;
    }
    const ArrayOfBeamElementCoords *beamElementCoords = fTrem->GetElementCoords();

    assert(beamElementCoords->size() == 2);

    int elementCount = 2;

    BeamElementCoord *firstElement = beamElementCoords->at(0);
    BeamElementCoord *secondElement = beamElementCoords->at(1);

    /******************************************************************/
    // Calculate the beam slope and position

    fTrem->m_drawingParams.CalcBeam(layer, staff, m_doc, beamElementCoords, elementCount);

    /******************************************************************/
    // Start the grahic

    dc->StartGraphic(element, "", element->GetUuid());

    /******************************************************************/
    // Draw the children

    DrawLayerChildren(dc, fTrem, layer, staff, measure);

    /******************************************************************/
    // Draw the stems and the bars

    // We look only at the first one for the duration since both are expected to be the same
    assert(dynamic_cast<AttDurationLogical *>(firstElement->m_element));
    int dur = (dynamic_cast<AttDurationLogical *>(firstElement->m_element))->GetDur();

    if (dur > DUR_1) {
        for (int i = 0; i < elementCount; ++i) {
            LayerElement *el = beamElementCoords->at(i)->m_element;
            if (((el->Is(NOTE)) && !(dynamic_cast<Note *>(el))->IsChordTone()) || (el->Is(CHORD))) {
                StemmedDrawingInterface *interface = el->GetStemmedDrawingInterface();
                assert(interface);
                DrawVerticalLine(dc, interface->GetDrawingStemStart(el).y, interface->GetDrawingStemEnd(el).y,
                    interface->GetDrawingStemStart(el).x, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize));
            }
        }
    }

    // Number of bars to draw - if we do not have changing values, draw
    // the number of bars according to the shortestDur value. Otherwise draw
    // only one bar and the others will be drawn separately.
    fullBars = fTrem->GetSlash();

    // Adjust the x position of the first and last element for taking into account the stem width
    firstElement->m_x -= (m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;
    secondElement->m_x += (m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;

    // Shift direction
    shiftY = (fTrem->m_drawingParams.m_stemDir == STEMDIRECTION_down) ? 1.0 : -1.0;
    polygonHeight = fTrem->m_drawingParams.m_beamWidthBlack * shiftY;

    y1 = firstElement->m_yBeam;
    y2 = secondElement->m_yBeam;

    x1 = firstElement->m_x;
    x2 = secondElement->m_x;

    dy1 = shiftY;
    dy2 = shiftY;

    int space = m_doc->GetDrawingBeamWidth(staff->m_drawingStaffSize, fTrem->m_drawingParams.m_cueSize);
    // for non-stem notes the bar should be shortenend
    if (dur < DUR_2) {
        x1 += 2 * space;
        y1 += 2 * space * fTrem->m_drawingParams.m_beamSlope;
        x2 -= 2 * space;
        y2 -= 2 * space * fTrem->m_drawingParams.m_beamSlope;
    }
    else if (dur == DUR_4) {
        x1 += space;
        y1 += space * fTrem->m_drawingParams.m_beamSlope;
        x2 -= space;
        y2 -= space * fTrem->m_drawingParams.m_beamSlope;
    }

    for (int j = 0; j < fullBars; ++j) {
        DrawObliquePolygon(dc, x1, y1, x2, y2, polygonHeight);
        y1 += polygonHeight;
        y2 += polygonHeight;
        y1 += dy1 * fTrem->m_drawingParams.m_beamWidthWhite;
        y2 += dy2 * fTrem->m_drawingParams.m_beamWidthWhite;

        // shorten the bar after having drawn the first one (but the first one)
        if ((j == 0) && (dur > DUR_1) && (dur != DUR_4)) {
            x1 += space;
            y1 += space * fTrem->m_drawingParams.m_beamSlope;
            x2 -= space;
            y2 -= space * fTrem->m_drawingParams.m_beamSlope;
        }
    }

    dc->EndGraphic(element, this);
}

} // namespace vrv
