/////////////////////////////////////////////////////////////////////////////
// Name:        view_beam.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Author and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <cassert>
#include <iostream>
#include <math.h>

//----------------------------------------------------------------------------

#include "beam.h"
#include "beamspan.h"
#include "devicecontext.h"
#include "doc.h"
#include "ftrem.h"
#include "layer.h"
#include "layerelement.h"
#include "options.h"
#include "smufl.h"
#include "staff.h"
#include "system.h"
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

    /******************************************************************/
    // initialization

    // Should we assert this at the beginning?
    if (beam->HasEmptyList(beam)) {
        return;
    }

    beam->m_beamSegment.InitCoordRefs(beam->GetElementCoords());

    data_BEAMPLACE initialPlace = beam->GetPlace();
    if (beam->HasStemSameasBeam()) beam->m_beamSegment.InitSameasRoles(beam->GetStemSameasBeam(), initialPlace);

    /******************************************************************/
    // Calculate the beam slope and position

    if (!beam->m_beamSegment.StemSameasIsSecondary()) {
        beam->m_beamSegment.CalcBeam(layer, beam->m_beamStaff, m_doc, beam, initialPlace);
    }

    /******************************************************************/
    // Start the Beam graphic and draw the children

    dc->StartGraphic(element, "", element->GetID());

    /******************************************************************/
    // Draw the children

    this->DrawLayerChildren(dc, beam, layer, staff, measure);

    /******************************************************************/
    // Draw the beamSegment - but not if it is a secondary beam in a stem.sameas

    if (!beam->m_beamSegment.StemSameasIsSecondary())
        this->DrawBeamSegment(dc, &beam->m_beamSegment, beam, layer, staff);

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

    /******************************************************************/
    // initialization

    // Should we assert this at the beginning?
    if (fTrem->HasEmptyList(fTrem)) {
        return;
    }
    const ArrayOfBeamElementCoords *beamElementCoords = fTrem->GetElementCoords();

    if (beamElementCoords->size() != 2) {
        LogError("View draw: <fTrem> element has invalid number of descendants.");
        return;
    }

    /******************************************************************/
    // Calculate the beam slope and position

    fTrem->m_beamSegment.CalcBeam(layer, fTrem->m_beamStaff, m_doc, fTrem);

    /******************************************************************/
    // Start the graphic

    dc->StartGraphic(element, "", element->GetID());

    /******************************************************************/
    // Draw the children

    this->DrawLayerChildren(dc, fTrem, layer, staff, measure);

    /******************************************************************/
    // Draw the stems and the bars

    this->DrawFTremSegment(dc, staff, fTrem);

    dc->EndGraphic(element, this);
}

void View::DrawFTremSegment(DeviceContext *dc, Staff *staff, FTrem *fTrem)
{
    assert(dc);
    assert(staff);
    assert(fTrem);

    const ArrayOfBeamElementCoords *beamElementCoords = fTrem->GetElementCoords();

    BeamElementCoord *firstElement = beamElementCoords->at(0);
    BeamElementCoord *secondElement = beamElementCoords->at(1);

    // We look only at the first one for the duration since both are expected to be the same
    AttDurationLog *durationElement = dynamic_cast<AttDurationLog *>(firstElement->m_element);
    if (!durationElement) return;
    const int dur = durationElement->GetDur();

    if (dur > DUR_1) {
        // Adjust the x position of the first and last element for taking into account the stem width
        firstElement->m_x -= (m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;
        secondElement->m_x += (m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;
    }

    // Number of bars to draw
    const int allBars = fTrem->GetBeams();
    int floatingBars = fTrem->HasBeamsFloat() ? fTrem->GetBeamsFloat() : 0;
    int fullBars = allBars - floatingBars;

    // Set initial coordinates for the beam
    int y1 = firstElement->m_yBeam;
    int y2 = secondElement->m_yBeam;

    int x1 = firstElement->m_x;
    int x2 = secondElement->m_x;

    // Shift direction
    const double shiftY = (fTrem->m_drawingPlace == BEAMPLACE_below) ? 1.0 : -1.0;
    const double dy1 = shiftY;
    const double dy2 = shiftY;

    int space = m_doc->GetDrawingBeamWidth(staff->m_drawingStaffSize, fTrem->m_cueSize);
    // for non-stem notes the bar should be shortened
    if (dur < DUR_2) {
        if (fTrem->m_drawingPlace == BEAMPLACE_below) x1 += 2 * space;
        y1 += 2 * space * fTrem->m_beamSegment.m_beamSlope;
        if (fTrem->m_drawingPlace == BEAMPLACE_above) x2 -= 2 * space;
        y2 -= 2 * space * fTrem->m_beamSegment.m_beamSlope;
        // floating bars make no sense here
        fullBars = allBars;
        floatingBars = 0;
    }
    else if ((dur > DUR_2) && !floatingBars) {
        fullBars = dur - 4;
        floatingBars = allBars - fullBars;
    }

    const int polygonHeight = fTrem->m_beamWidthBlack * shiftY;
    for (int j = 0; j < fullBars; ++j) {
        this->DrawObliquePolygon(dc, x1, y1, x2, y2, polygonHeight);
        y1 += polygonHeight;
        y2 += polygonHeight;
        y1 += dy1 * fTrem->m_beamWidthWhite;
        y2 += dy2 * fTrem->m_beamWidthWhite;
    }

    // If we have no full bar but only floating bars, then move it inside
    if (fullBars == 0) {
        y1 += dy1 * fTrem->m_beamWidthWhite / 2;
        y2 += dy2 * fTrem->m_beamWidthWhite / 2;
    }

    // shorten the bar after having drawn the first one (but the first one)
    x1 += space;
    y1 += space * fTrem->m_beamSegment.m_beamSlope;
    x2 -= space;
    y2 -= space * fTrem->m_beamSegment.m_beamSlope;

    for (int j = 0; j < floatingBars; ++j) {
        this->DrawObliquePolygon(dc, x1, y1, x2, y2, polygonHeight);
        y1 += polygonHeight;
        y2 += polygonHeight;
        y1 += dy1 * fTrem->m_beamWidthWhite;
        y2 += dy2 * fTrem->m_beamWidthWhite;
    }
}

void View::DrawBeamSegment(
    DeviceContext *dc, BeamSegment *beamSegment, BeamDrawingInterface *beamInterface, Layer *layer, Staff *staff)
{
    assert(dc);
    assert(beamSegment);
    assert(beamInterface);
    assert(layer);
    assert(staff);

    // temporary coordinates
    int x1, x2, y1, y2;

    // temporary variables
    int shiftY;

    // loops
    int i;

    const ArrayOfBeamElementCoords *beamElementCoords = beamSegment->GetElementCoordRefs();

    int elementCount = (int)beamElementCoords->size();
    int last = elementCount - 1;

    /******************************************************************/
    // Draw the beam full bar

    // Adjust the x position of the first and last element for taking into account the stem width
    beamElementCoords->at(0)->m_x -= (m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;
    beamElementCoords->at(last)->m_x += (m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;

    // Shift direction
    shiftY = (beamInterface->m_drawingPlace == BEAMPLACE_below) ? 1.0 : -1.0;

    y1 = beamElementCoords->at(0)->m_yBeam;
    y2 = beamElementCoords->at(last)->m_yBeam;

    x1 = beamElementCoords->at(0)->m_x;
    x2 = beamElementCoords->at(last)->m_x;

    // For acc and rit beam (see AttBeamingVis set
    // s_y = 0 and s_y2 = 0 respectively

    const int polygonHeight = beamInterface->m_beamWidthBlack * shiftY;
    this->DrawObliquePolygon(dc, x1, y1, x2, y2, polygonHeight);

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

    int durRef = DUR_8;
    int durRef2 = DUR_16;

    if (staff->IsTabLuteFrench() || staff->IsTabLuteItalian()) {
        durRef = DUR_4;
        durRef2 = DUR_8;
    }

    int barY = 0;

    if (noteCount > 0) {
        int testDur = durRef2;

        int fractBeamWidth
            = m_doc->GetGlyphWidth(SMUFL_E0A4_noteheadBlack, beamInterface->m_fractionSize, beamInterface->m_cueSize);

        // loop
        while (testDur <= beamInterface->m_shortestDur) {
            // true at the beginning of a beam or after a breakSec
            bool start = true;

            int idx = 0;
            barY += beamInterface->m_beamWidth;

            // all but the last one
            for (i = 0; i < noteCount - 1; ++i) {
                idx = noteIndexes.at(i);
                int nextIdx = noteIndexes.at(i + 1);

                bool breakSec = ((beamElementCoords->at(idx)->m_breaksec)
                    && (testDur - durRef >= beamElementCoords->at(idx)->m_breaksec));
                beamElementCoords->at(idx)->m_partialFlags[testDur - durRef] = PARTIAL_NONE;
                // partial is needed
                if (beamElementCoords->at(idx)->m_dur >= (char)testDur) {
                    // and for the next one too, but no break - through
                    if ((beamElementCoords->at(nextIdx)->m_dur >= (char)testDur) && !breakSec) {
                        beamElementCoords->at(idx)->m_partialFlags[testDur - durRef] = PARTIAL_THROUGH;
                    }
                    // not needed for the next one or break
                    else {
                        // we are starting a beam or after a beam break - put it right
                        if (start) {
                            if ((idx != 0) && (beamElementCoords->at(idx - 1)->m_element->Is(REST))) {
                                beamElementCoords->at(idx)->m_partialFlags[testDur - durRef] = PARTIAL_LEFT;
                            }
                            else {
                                beamElementCoords->at(idx)->m_partialFlags[testDur - durRef] = PARTIAL_RIGHT;
                            }
                        }
                        // or the previous one had no partial
                        else if (beamElementCoords->at(noteIndexes.at(i - 1))->m_dur < (char)testDur) {
                            // if we are at the full bar level, put it left
                            if (testDur == durRef2) {
                                beamElementCoords->at(idx)->m_partialFlags[testDur - durRef] = PARTIAL_LEFT;
                            }
                            // if the previous level underneath was a partial through, put it left
                            else if (beamElementCoords->at(noteIndexes.at(i - 1))->m_partialFlags[testDur - 1 - durRef]
                                == PARTIAL_THROUGH) {
                                beamElementCoords->at(idx)->m_partialFlags[testDur - durRef] = PARTIAL_LEFT;
                            }
                            // if the level underneath was not left (right or through), put it right
                            else if (beamElementCoords->at(idx)->m_partialFlags[testDur - 1 - durRef] != PARTIAL_LEFT) {
                                beamElementCoords->at(idx)->m_partialFlags[testDur - durRef] = PARTIAL_RIGHT;
                            }
                            // it was put left before, put it left
                            else {
                                beamElementCoords->at(idx)->m_partialFlags[testDur - durRef] = PARTIAL_LEFT;
                            }
                        }
                    }
                }
                // we are not in a group
                start = breakSec;
            }
            // last one
            idx = (int)noteIndexes.back();
            beamElementCoords->at(idx)->m_partialFlags[testDur - durRef] = PARTIAL_NONE;
            // partial is needed
            if ((beamElementCoords->at(idx)->m_dur >= (char)testDur)) {
                // and the previous one had no partial - put it left
                if ((noteCount == 1) || (beamElementCoords->at(noteIndexes.at(i - 1))->m_dur < (char)testDur)
                    || start) {
                    beamElementCoords->at(idx)->m_partialFlags[testDur - durRef] = PARTIAL_LEFT;
                }
            }

            // draw them
            for (i = 0; i < noteCount; ++i) {
                int barYPos = 0;
                idx = noteIndexes.at(i);
                if (BEAMPLACE_mixed == beamInterface->m_drawingPlace) {
                    int elemIndex = idx;
                    if (BEAMPLACE_NONE == beamElementCoords->at(idx)->m_partialFlagPlace) {
                        if ((0 == i) || ((noteCount - 1) == i)
                            || (beamElementCoords->at(noteIndexes.at(i - 1))->m_partialFlagPlace
                                != beamElementCoords->at(noteIndexes.at(i + 1))->m_partialFlagPlace)) {
                            continue;
                        }
                        elemIndex = i - 1;
                    }
                    barYPos
                        = barY * ((BEAMPLACE_above == beamElementCoords->at(elemIndex)->m_partialFlagPlace) ? 1 : -1);
                }
                else {
                    barYPos = shiftY * barY;
                }
                if (beamElementCoords->at(idx)->m_partialFlags[testDur - durRef] == PARTIAL_THROUGH) {
                    // through should never be set on the last one
                    assert(i < noteCount - 1);
                    if (i >= noteCount - 1) continue; // assert for debug and skip otherwise
                    y1 = beamElementCoords->at(idx)->m_yBeam + barYPos;
                    y2 = beamElementCoords->at(noteIndexes.at(i + 1))->m_yBeam + barYPos;
                    this->DrawObliquePolygon(dc, beamElementCoords->at(idx)->m_x, y1,
                        beamElementCoords->at(noteIndexes.at(i + 1))->m_x, y2, polygonHeight);
                }
                else if (beamElementCoords->at(idx)->m_partialFlags[testDur - durRef] == PARTIAL_RIGHT) {
                    y1 = beamElementCoords->at(idx)->m_yBeam + barYPos;
                    int x2 = beamElementCoords->at(idx)->m_x + fractBeamWidth;
                    y2 = beamSegment->m_firstNoteOrChord->m_yBeam + barYPos
                        + beamSegment->m_beamSlope * (x2 - beamSegment->m_firstNoteOrChord->m_x);
                    this->DrawObliquePolygon(dc, beamElementCoords->at(idx)->m_x, y1, x2, y2, polygonHeight);
                }
                else if (beamElementCoords->at(idx)->m_partialFlags[testDur - durRef] == PARTIAL_LEFT) {
                    y2 = beamElementCoords->at(idx)->m_yBeam + barYPos;
                    int x1 = beamElementCoords->at(idx)->m_x - fractBeamWidth;
                    y1 = beamSegment->m_firstNoteOrChord->m_yBeam + barYPos
                        + beamSegment->m_beamSlope * (x1 - beamSegment->m_firstNoteOrChord->m_x);
                    this->DrawObliquePolygon(dc, x1, y1, beamElementCoords->at(idx)->m_x, y2, polygonHeight);
                }
            }

            testDur += 1;

        } // end of while
    } // end of drawing partial bars
}

void View::DrawBeamSpan(DeviceContext *dc, BeamSpan *beamSpan, System *system, Object *graphic)
{
    assert(dc);
    assert(beamSpan);
    assert(system);

    // Draw segments for the beamSpan
    if (graphic) {
        dc->ResumeGraphic(graphic, graphic->GetID());
    }
    else {
        dc->StartGraphic(beamSpan, "", beamSpan->GetID(), SPANNING);
    }

    BeamSpanSegment *segment = beamSpan->GetSegmentForSystem(system);
    if (segment) {
        // Reset current segment and set coordinates based on stored begin/end iterators for the ElementCoords
        segment->Reset();

        const auto coordsFirst = std::find(
            beamSpan->m_beamElementCoords.begin(), beamSpan->m_beamElementCoords.end(), segment->GetBeginCoord());
        const auto coordsLast = std::find(
            beamSpan->m_beamElementCoords.begin(), beamSpan->m_beamElementCoords.end(), segment->GetEndCoord());

        if ((coordsFirst != beamSpan->m_beamElementCoords.end())
            && (coordsLast != beamSpan->m_beamElementCoords.end())) {
            ArrayOfBeamElementCoords coord(coordsFirst, coordsLast + 1);
            segment->InitCoordRefs(&coord);
            segment->CalcBeam(segment->GetLayer(), segment->GetStaff(), m_doc, beamSpan, beamSpan->m_drawingPlace);
            segment->AppendSpanningCoordinates(segment->GetMeasure());

            // Draw corresponding beam segment
            this->DrawBeamSegment(dc, segment, beamSpan, segment->GetLayer(), segment->GetStaff());
        }
    }

    if (graphic) {
        dc->EndResumedGraphic(graphic, this);
    }
    else {
        dc->EndGraphic(beamSpan, this);
    }
}

} // namespace vrv
