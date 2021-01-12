/////////////////////////////////////////////////////////////////////////////
// Name:        beam.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "beam.h"

//----------------------------------------------------------------------------

#include <array>
#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "btrem.h"
#include "doc.h"
#include "editorial.h"
#include "elementpart.h"
#include "functorparams.h"
#include "gracegrp.h"
#include "layer.h"
#include "measure.h"
#include "note.h"
#include "rest.h"
#include "smufl.h"
#include "space.h"
#include "staff.h"
#include "tuplet.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// BeamSegment
//----------------------------------------------------------------------------

BeamSegment::BeamSegment()
{
    Reset();
}

BeamSegment::~BeamSegment()
{
    this->ClearCoordRefs();
}

void BeamSegment::Reset()
{
    this->ClearCoordRefs();

    m_startingX = 0;
    m_startingY = 0;
    m_beamSlope = 0.0;
    m_verticalCenter = 0;
    m_avgY = 0;
    m_extendedToCenter = false;
    m_ledgerLinesAbove = 0;
    m_ledgerLinesBelow = 0;
    m_uniformStemLength = 0;

    m_firstNoteOrChord = NULL;
    m_lastNoteOrChord = NULL;
}

const ArrayOfBeamElementCoords *BeamSegment::GetElementCoordRefs()
{
    // this->GetList(this);
    return &m_beamElementCoordRefs;
}

void BeamSegment::ClearCoordRefs()
{
    m_beamElementCoordRefs.clear();
}

void BeamSegment::InitCoordRefs(const ArrayOfBeamElementCoords *beamElementCoords)
{
    m_beamElementCoordRefs = *beamElementCoords;
}

void BeamSegment::CalcBeam(
    Layer *layer, Staff *staff, Doc *doc, BeamDrawingInterface *beamInterface, data_BEAMPLACE place, bool init)
{
    assert(layer);
    assert(staff);
    assert(doc);

    int y1, y2;
    assert(m_beamElementCoordRefs.size() > 0);

    // For recursive calls, avoid to re-init values
    if (init) {
        this->CalcBeamInit(layer, staff, doc, beamInterface, place);
    }

    bool horizontal = beamInterface->IsHorizontal();

    // Beam@place has precedence - however, in some cases, CalcBeam is called recusively because we need to change the
    // place This occurs when mixed makes no sense and the beam is placed above or below instead.
    this->CalcBeamPlace(layer, beamInterface, place);

    CalcBeamStemLength(staff, beamInterface->m_drawingPlace == BEAMPLACE_below ? STEMDIRECTION_down : STEMDIRECTION_up);

    if (BEAMPLACE_mixed == beamInterface->m_drawingPlace) {
        CalcMixedBeamPlace(staff);
        CalcPartialFlagPlace();
    }

    // Set drawing stem positions
    CalcBeamPosition(doc, staff, layer, beamInterface, horizontal);
    if (BEAMPLACE_mixed == beamInterface->m_drawingPlace) {
        if (!beamInterface->m_hasCrossStaffContent && NeedToResetPosition(staff, doc, beamInterface)) {
            CalcBeamInit(layer, staff, doc, beamInterface, place);
            CalcBeamPosition(doc, staff, layer, beamInterface, horizontal);
        }
    }

    // ArrayOfBeamElementCoords stemUps;
    // ArrayOfBeamElementCoords stemDowns;

    /******************************************************************/
    // Set the stem lengths to stem objects

    for (auto coord : m_beamElementCoordRefs) {
        // All notes and chords get their stem value stored
        LayerElement *el = coord->m_element;
        if ((el->Is(NOTE)) || (el->Is(CHORD))) {
            StemmedDrawingInterface *stemmedInterface = el->GetStemmedDrawingInterface();
            assert(beamInterface);

            assert(coord->m_closestNote);
            y1 = coord->m_yBeam;
            y2 = coord->m_closestNote->GetDrawingY();
            if (beamInterface->m_drawingPlace == BEAMPLACE_above) {
                y1 -= doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
                y2 += stemmedInterface->GetStemUpSE(doc, staff->m_drawingStaffSize, beamInterface->m_cueSize).y;
            }
            else if (beamInterface->m_drawingPlace == BEAMPLACE_below) {
                y1 += doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
                y2 += stemmedInterface->GetStemDownNW(doc, staff->m_drawingStaffSize, beamInterface->m_cueSize).y;
            }
            else if (beamInterface->m_drawingPlace == BEAMPLACE_mixed) {
                int stemOffset = 0;
                if (coord->m_partialFlagPlace == coord->m_beamRelativePlace) {
                    stemOffset = (coord->m_dur - DUR_8) * beamInterface->m_beamWidth;
                }
                if (coord->m_beamRelativePlace == BEAMPLACE_below) {
                    y1 -= doc->GetDrawingStemWidth(staff->m_drawingStaffSize) + stemOffset;
                    y2 += stemmedInterface->GetStemDownNW(doc, staff->m_drawingStaffSize, beamInterface->m_cueSize).y;
                }
                else {
                    y1 += stemOffset;
                    y2 += stemmedInterface->GetStemUpSE(doc, staff->m_drawingStaffSize, beamInterface->m_cueSize).y;
                }
            }

            if (coord->m_element->Is(CHORD)) {
                Chord *chord = vrv_cast<Chord *>(coord->m_element);
                assert(chord);
                int yMax, yMin;
                chord->GetYExtremes(yMax, yMin);
                if (beamInterface->m_drawingPlace == BEAMPLACE_mixed) {
                    if (coord->m_beamRelativePlace == BEAMPLACE_above) y2 += (yMin - yMax);
                }
                else {
                    y2 += (beamInterface->m_drawingPlace == BEAMPLACE_above) ? (yMin - yMax) : (yMax - yMin);
                }
            }

            Stem *stem = stemmedInterface->GetDrawingStem();
            // This is the case with fTrem on whole notes
            if (!stem) continue;

            // stem->SetDrawingStemDir(beamInterface->m_stemDir);
            // Since the value were calculated relatively to the element position, adjust them
            stem->SetDrawingXRel(coord->m_x - el->GetDrawingX());
            stem->SetDrawingYRel(y2 - el->GetDrawingY());
            stem->SetDrawingStemLen(y2 - y1);
        }
    }
}

bool BeamSegment::DoesBeamOverlap(int staffTop, int topOffset, int staffBottom, int bottomOffset)
{
    // find if current beam fits within the staff
    auto withinBounds
        = std::find_if(m_beamElementCoordRefs.begin(), m_beamElementCoordRefs.end(), [&](BeamElementCoord *coord) {
              if ((coord->m_yBeam > staffTop - topOffset) || (coord->m_yBeam < staffBottom + bottomOffset)) {
                  return true;
              }
              return false;
          });
    if (withinBounds != m_beamElementCoordRefs.end()) return false;
    auto overlapping
        = std::find_if(m_beamElementCoordRefs.begin(), m_beamElementCoordRefs.end(), [&](BeamElementCoord *coord) {
              assert(coord->m_element);
              if (!coord->m_element->Is({ NOTE, CHORD })) return false;
              int elemY = coord->m_element->GetDrawingY();
              if (coord->m_stem->GetDrawingStemDir() == STEMDIRECTION_down) {
                  if (elemY <= coord->m_yBeam + topOffset) return true;
              }
              else if (coord->m_stem->GetDrawingStemDir() == STEMDIRECTION_up) {
                  if (elemY >= coord->m_yBeam - bottomOffset) return true;
              }
              return false;
          });
    if (overlapping != m_beamElementCoordRefs.end()) return false;
    return true;
}

bool BeamSegment::NeedToResetPosition(Staff *staff, Doc *doc, BeamDrawingInterface *beamInterface)
{
    const int unit = doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
    // find shortest duration for above/below beams for the sake of calculating overlap with additional beams
    int topShortestDur = DUR_8;
    int bottomShortestDur = DUR_8;
    std::for_each(m_beamElementCoordRefs.begin(), m_beamElementCoordRefs.end(), [&](BeamElementCoord *coord) {
        if (coord->m_partialFlagPlace == BEAMPLACE_above) {
            topShortestDur = std::max(topShortestDur, coord->m_dur);
        }
        else if (coord->m_partialFlagPlace == BEAMPLACE_below) {
            bottomShortestDur = std::max(bottomShortestDur, coord->m_dur);
        }
    });
    const int topOffset = (topShortestDur - DUR_8) * beamInterface->m_beamWidth + unit / 2;
    const int bottomOffset = (bottomShortestDur - DUR_8) * beamInterface->m_beamWidth + unit / 2;

    // find top and bottom of the staff
    const int staffTop = staff->GetDrawingY();
    const int staffBottom
        = staffTop - doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * (staff->m_drawingLines - 1);

    if (DoesBeamOverlap(staffTop, topOffset, staffBottom, bottomOffset)) return false;

    // Calculate midpoint for the beam with mixed placement
    int min = m_beamElementCoordRefs.at(0)->m_element->GetDrawingY();
    int max = m_beamElementCoordRefs.at(0)->m_element->GetDrawingY();
    for (auto coord : m_beamElementCoordRefs) {
        max = std::max(max, coord->m_element->GetDrawingY());
        min = std::min(min, coord->m_element->GetDrawingY());
    }
    const int midpoint = (max + min) / 2;
    bool isMidpointWithinBounds = (midpoint < staffTop - topOffset) && (midpoint > staffBottom + bottomOffset);

    // If midpoint fits within bounds of the staff, try to place beam there
    if (isMidpointWithinBounds) {
        const int midpointOffset
            = (m_beamElementCoordRefs.front()->m_yBeam + m_beamElementCoordRefs.back()->m_yBeam - 2 * midpoint) / 2;
        std::for_each(m_beamElementCoordRefs.begin(), m_beamElementCoordRefs.end(),
            [midpointOffset](BeamElementCoord *coord) { coord->m_yBeam += midpointOffset; });
        if (DoesBeamOverlap(staffTop, topOffset, staffBottom, bottomOffset)) return false;
    }
    // If midpoint if above the staff, try to place beam at the top edge of the staff
    if (!isMidpointWithinBounds && (midpoint > staffBottom)) {
        const int offset = (m_beamElementCoordRefs.front()->m_yBeam + m_beamElementCoordRefs.back()->m_yBeam
                               - 2 * (staffTop - topOffset))
            / 2;
        std::for_each(m_beamElementCoordRefs.begin(), m_beamElementCoordRefs.end(),
            [offset](BeamElementCoord *coord) { coord->m_yBeam -= offset; });
    }
    // otherwise try placing it on the bottom edge
    else if (!isMidpointWithinBounds && (midpoint < staffTop)) {
        const int offset = (m_beamElementCoordRefs.front()->m_yBeam + m_beamElementCoordRefs.back()->m_yBeam
                               - 2 * (staffBottom + bottomOffset))
            / 2;
        std::for_each(m_beamElementCoordRefs.begin(), m_beamElementCoordRefs.end(),
            [offset](BeamElementCoord *coord) { coord->m_yBeam += offset; });
    }
    if (DoesBeamOverlap(staffTop, topOffset, staffBottom, bottomOffset)) return false;

    // If none of the positions work - there's no space for us to draw a mixed beam (or there is space but it would
    // overlap with ledger line). Adjust beam placement based on the most frequent stem direction
    const int stemUpCount = (int)std::count_if(m_beamElementCoordRefs.begin(), m_beamElementCoordRefs.end(),
        [](BeamElementCoord *coord) { return coord->GetStemDir() == STEMDIRECTION_up; });
    const int stemDownCount = (int)std::count_if(m_beamElementCoordRefs.begin(), m_beamElementCoordRefs.end(),
        [](BeamElementCoord *coord) { return coord->GetStemDir() == STEMDIRECTION_down; });
    data_STEMDIRECTION newDirection = (stemUpCount >= stemDownCount) ? STEMDIRECTION_up : STEMDIRECTION_down;
    beamInterface->m_drawingPlace = (newDirection == STEMDIRECTION_up) ? BEAMPLACE_above : BEAMPLACE_below;
    if ((newDirection == STEMDIRECTION_down) && (m_uniformStemLength > 0)) m_uniformStemLength *= -1;

    LogWarning("Insufficient space to draw mixed beam, starting at '%s'. Drawing '%s' instead.",
        m_beamElementCoordRefs.at(0)->m_element->GetUuid().c_str(),
        (beamInterface->m_drawingPlace == BEAMPLACE_above) ? "above" : "below");

    return true;
}

void BeamSegment::CalcBeamInit(
    Layer *layer, Staff *staff, Doc *doc, BeamDrawingInterface *beamInterface, data_BEAMPLACE place)
{
    assert(layer);
    assert(staff);
    assert(doc);
    assert(beamInterface);

    int i;

    int elementCount = (int)m_beamElementCoordRefs.size();
    assert(elementCount > 0);

    /******************************************************************/
    // initialization

    for (i = 0; i < elementCount; ++i) {
        BeamElementCoord *coord = m_beamElementCoordRefs.at(i);
        coord->m_x = coord->m_element->GetDrawingX();
    }

    this->m_verticalCenter = staff->GetDrawingY()
        - (doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * 2); // center point of the staff

    beamInterface->m_beamWidthBlack = doc->GetDrawingBeamWidth(staff->m_drawingStaffSize, beamInterface->m_cueSize);
    beamInterface->m_beamWidthWhite
        = doc->GetDrawingBeamWhiteWidth(staff->m_drawingStaffSize, beamInterface->m_cueSize);
    if (beamInterface->m_shortestDur == DUR_64) {
        beamInterface->m_beamWidthWhite *= 4;
        beamInterface->m_beamWidthWhite /= 3;
    }
    beamInterface->m_beamWidth = beamInterface->m_beamWidthBlack + beamInterface->m_beamWidthWhite;

    // x-offset values for stem bases, dx[y] where y = element->m_cueSize
    beamInterface->m_stemXAbove[0] = doc->GetGlyphWidth(SMUFL_E0A4_noteheadBlack, staff->m_drawingStaffSize, false)
        - (doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;
    beamInterface->m_stemXAbove[1] = doc->GetGlyphWidth(SMUFL_E0A4_noteheadBlack, staff->m_drawingStaffSize, true)
        - (doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;
    beamInterface->m_stemXBelow[0] = (doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;
    beamInterface->m_stemXBelow[1] = (doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;

    /******************************************************************/
    // Calculate the extreme values

    int yMax = 0, yMin = 0;
    int nbRests = 0;

    m_avgY = 0;
    m_nbNotesOrChords = 0;
    m_extendedToCenter = false;
    m_ledgerLinesAbove = 0;
    m_ledgerLinesBelow = 0;

    // elementCount holds the last one
    for (i = 0; i < elementCount; ++i) {
        BeamElementCoord *coord = m_beamElementCoordRefs.at(i);
        coord->m_yBeam = 0;

        if (coord->m_element->Is({ CHORD, NOTE })) {
            if (!m_firstNoteOrChord) m_firstNoteOrChord = coord;
            m_lastNoteOrChord = coord;
            m_nbNotesOrChords++;
        }

        if (coord->m_element->Is(CHORD)) {
            Chord *chord = vrv_cast<Chord *>(coord->m_element);
            assert(chord);
            chord->GetYExtremes(yMax, yMin);

            this->m_avgY += ((yMax + yMin) / 2);

            int linesAbove = 0;
            int linesBelow = 0;
            Note *bottomNote = chord->GetBottomNote();
            assert(bottomNote);
            if (bottomNote->HasLedgerLines(linesAbove, linesBelow, staff)) {
                m_ledgerLinesBelow += linesBelow;
            }
            Note *topNote = chord->GetTopNote();
            assert(topNote);
            if (topNote->HasLedgerLines(linesAbove, linesBelow, staff)) {
                m_ledgerLinesAbove += linesAbove;
            }
        }
        else if (coord->m_element->Is(NOTE)) {
            Note *note = vrv_cast<Note *>(coord->m_element);
            assert(note);
            this->m_avgY += note->GetDrawingY();

            int linesAbove = 0;
            int linesBelow = 0;
            if (note->HasLedgerLines(linesAbove, linesBelow, staff)) {
                m_ledgerLinesBelow += linesBelow;
                m_ledgerLinesAbove += linesAbove;
            }
        }
        else {
            nbRests++;
        }
    }

    // Only if not only rests. (Will produce non-sense output anyway)
    if (elementCount != nbRests) {
        this->m_avgY /= (elementCount - nbRests);
    }
}

bool BeamSegment::CalcBeamSlope(
    Layer *layer, Staff *staff, Doc *doc, BeamDrawingInterface *beamInterface, bool &shorten, int &step)
{
    assert(layer);
    assert(staff);
    assert(doc);
    assert(beamInterface);

    m_beamSlope = 0.0;
    shorten = false;

    if (m_nbNotesOrChords < 2) {
        return false;
    }
    assert(m_firstNoteOrChord && m_lastNoteOrChord);

    m_beamSlope = BoundingBox::CalcSlope(Point(m_firstNoteOrChord->m_x, m_firstNoteOrChord->m_yBeam),
        Point(m_lastNoteOrChord->m_x, m_lastNoteOrChord->m_yBeam));

    int noteStep = 0;
    double noteSlope = 0.0;
    if (m_firstNoteOrChord->m_closestNote && m_lastNoteOrChord->m_closestNote) {
        noteSlope
            = BoundingBox::CalcSlope(Point(m_firstNoteOrChord->m_x, m_firstNoteOrChord->m_closestNote->GetDrawingY()),
                Point(m_lastNoteOrChord->m_x, m_lastNoteOrChord->m_closestNote->GetDrawingY()));
        noteStep
            = abs(m_firstNoteOrChord->m_closestNote->GetDrawingY() - m_lastNoteOrChord->m_closestNote->GetDrawingY());
    }

    // LogDebug("Slope (original) %f %f", m_beamSlope, noteSlope);

    // This can happen with two notes with 32sd or 64th notes and a diatonic step
    // Force the noteSlope to be considered instead
    if (m_beamSlope == 0.0) m_beamSlope = noteSlope;

    if (m_beamSlope == 0.0) return false;

    const int unit = doc->GetDrawingUnit(staff->m_drawingStaffSize);
    // Default (maximum) step is two stave-spaces (4 units)
    step = unit * 4;
    // The current step according to stems - this can be flat because of the stem extended
    int curStep = abs(m_firstNoteOrChord->m_yBeam - m_lastNoteOrChord->m_yBeam);
    // The distance between the two extremes
    int dist = m_lastNoteOrChord->m_x - m_firstNoteOrChord->m_x;
    // Short accessors
    const data_BEAMPLACE place = beamInterface->m_drawingPlace;
    const int dur = beamInterface->m_shortestDur;

    // Indicates if we have a short step of half a unit
    // This occurs with 8th and 16th only and with a reduced distance of 3 stave-spaces (6 units)
    bool shortStep = false;
    if (m_extendedToCenter) {
        step = unit / 2;
        shortStep = true;
    }
    else if (m_nbNotesOrChords == 2) {
        step = unit * 2;
        // Short distance
        if (dist <= unit * 6) {
            step = unit / 2;
            shortStep = true;
        }
    }
    // With three notes and a short distance (12 units) also reduce the step to a stave-space (2 units)
    else if (m_nbNotesOrChords == 3) {
        // Short distance
        if (dist <= unit * 12) {
            step = unit * 2;
        }
        // A fifth or smaller - reduce the step
        else if (noteStep <= unit * 4) {
            step = unit * 2;
        }
    }
    else {
        // A fourth or smaller - reduce to a short step
        if (noteStep <= unit * 3) {
            step = unit / 2;
            shortStep = true;
        }
        // A fifth or smaller - reduce the step
        else if (noteStep <= unit * 4) {
            step = unit * 2;
        }
    }

    // Prevent short step with values not shorter than a 16th
    if (shortStep && (dur >= DUR_32)) {
        step = unit * 2;
        shortStep = false;
    }

    // We can keep the current slope but only if curStep is not 0 and smaller than the step
    if ((curStep != 0) && (curStep < step) && (BEAMPLACE_mixed != place)) {
        LogDebug("Current %d step is lower than max step %d", curStep, step);
        return false;
    }
    // This occurs when the current stem would yield an horizontal beam
    // We need to extend one side first in order to make sure it will be oblique
    else if (curStep == 0) {
        if (place == BEAMPLACE_above) {
            // upwards
            if (m_beamSlope > 0.0) {
                m_lastNoteOrChord->m_yBeam += step;
            }
            // downwards
            else {
                m_firstNoteOrChord->m_yBeam += step;
            }
        }
        else if (place == BEAMPLACE_below) {
            // downwards
            if (m_beamSlope < 0.0) {
                m_lastNoteOrChord->m_yBeam -= step;
            }
            // upwards
            else {
                m_firstNoteOrChord->m_yBeam -= step;
            }
        }
    }

    // Now adjust the stem - for short steps, we need to adjust both side when the shorter one is not centered
    if (place == BEAMPLACE_above) {
        // upwards
        if (m_beamSlope > 0.0) {
            // For 8th note groups, reduce the length of the last note if possible - disabled because not working well
            /*
            if ((beamInterface->m_shortestDur == DUR_8) && !m_lastNoteOrChord->m_shortened
                && !this->m_extendedToCenter) {
                shorten = true;
            }
            */
            m_firstNoteOrChord->m_centered = m_lastNoteOrChord->m_centered;
            if (shortStep) {
                if (!m_lastNoteOrChord->m_centered) {
                    m_lastNoteOrChord->m_yBeam = m_lastNoteOrChord->m_yBeam + step;
                    m_lastNoteOrChord->m_centered = true;
                }
            }
            m_firstNoteOrChord->m_yBeam = m_lastNoteOrChord->m_yBeam - step;
        }
        // downwards
        else {
            // For 8th note groups, reduce the length of the first note if possible - disabled because not working well
            /*
            if ((beamInterface->m_shortestDur == DUR_8) && !m_firstNoteOrChord->m_shortened
                && !this->m_extendedToCenter) {
                //shorten = true;
            }
            */
            m_lastNoteOrChord->m_centered = m_firstNoteOrChord->m_centered;
            if (shortStep) {
                if (!m_firstNoteOrChord->m_centered) {
                    m_firstNoteOrChord->m_yBeam = m_firstNoteOrChord->m_yBeam + step;
                    m_firstNoteOrChord->m_centered = true;
                }
            }
            m_lastNoteOrChord->m_yBeam = m_firstNoteOrChord->m_yBeam - step;
        }
    }
    else if (place == BEAMPLACE_below) {
        // downwards
        if (m_beamSlope < 0.0) {
            // For 8th note groups, reduce the length of the last note if possible - disabled because not working well
            /*
            if ((beamInterface->m_shortestDur == DUR_8) && !m_lastNoteOrChord->m_shortened
                && !this->m_extendedToCenter) {
                //shorten = true;
            }
            */
            m_firstNoteOrChord->m_centered = m_lastNoteOrChord->m_centered;
            if (shortStep) {
                if (!m_lastNoteOrChord->m_centered) {
                    m_lastNoteOrChord->m_yBeam = m_lastNoteOrChord->m_yBeam - step;
                    m_lastNoteOrChord->m_centered = true;
                }
            }
            m_firstNoteOrChord->m_yBeam = m_lastNoteOrChord->m_yBeam + step;
        }
        // upwards
        else {
            // For 8th note groups, reduce the length of the first note if possible - disabled because not working well
            /*
            if ((beamInterface->m_shortestDur == DUR_8) && !m_firstNoteOrChord->m_shortened
                && !this->m_extendedToCenter) {
                //shorten = true;
            }
            */
            m_lastNoteOrChord->m_centered = m_firstNoteOrChord->m_centered;
            if (shortStep) {
                if (!m_firstNoteOrChord->m_centered) {
                    m_firstNoteOrChord->m_yBeam = m_firstNoteOrChord->m_yBeam - step;
                    m_firstNoteOrChord->m_centered = true;
                }
            }
            m_lastNoteOrChord->m_yBeam = m_firstNoteOrChord->m_yBeam + step;
        }
    }
    else if (place == BEAMPLACE_mixed) {
        int heightDiff = m_lastNoteOrChord->m_yBeam - m_firstNoteOrChord->m_yBeam;
        if (curStep < step) {
            std::swap(m_firstNoteOrChord->m_yBeam, m_lastNoteOrChord->m_yBeam);
        }
        if (m_beamSlope > 0.0) {
            m_firstNoteOrChord->m_yBeam += (heightDiff - step) / 2;
            m_lastNoteOrChord->m_yBeam -= (heightDiff - step) / 2;
        }
        else {
            m_firstNoteOrChord->m_yBeam += (heightDiff + step) / 2;
            m_lastNoteOrChord->m_yBeam -= (heightDiff + step) / 2;
        }
    }

    this->m_beamSlope = BoundingBox::CalcSlope(Point(m_firstNoteOrChord->m_x, m_firstNoteOrChord->m_yBeam),
        Point(m_lastNoteOrChord->m_x, m_lastNoteOrChord->m_yBeam));
    // LogDebug("Slope (adjusted) %f", m_beamSlope);

    if (m_nbNotesOrChords == 2) {
        // For now do not shorten beams with two note
        shorten = false;
        // Also, we never have to adjust the slope with two notes
        return false;
    }

    return true;
}

void BeamSegment::CalcBeamPosition(
    Doc *doc, Staff *staff, Layer *layer, BeamDrawingInterface *beamInterface, bool isHorizontal)
{
    // Set drawing stem positions
    for (auto coord : m_beamElementCoordRefs) {
        if (beamInterface->m_drawingPlace == BEAMPLACE_above) {
            coord->SetDrawingStemDir(STEMDIRECTION_up, staff, doc, this, beamInterface);
        }
        else if (beamInterface->m_drawingPlace == BEAMPLACE_below) {
            coord->SetDrawingStemDir(STEMDIRECTION_down, staff, doc, this, beamInterface);
        }
        // cross-staff or beam@place=mixed
        else {
            if (beamInterface->m_hasCrossStaffContent) {
                data_STEMDIRECTION dir
                    = (coord->m_beamRelativePlace == BEAMPLACE_above) ? STEMDIRECTION_up : STEMDIRECTION_down;
                coord->SetDrawingStemDir(dir, staff, doc, this, beamInterface);
            }
            else {
                data_STEMDIRECTION stemDir = coord->GetStemDir();
                // TODO - Handle cases where there is no given stem direction (here we can still have NONE)
                coord->SetDrawingStemDir(stemDir, staff, doc, this, beamInterface);
            }
        }
    }

    /******************************************************************/
    // Calculate the slope is necessary

    m_beamSlope = 0.0;
    if (!isHorizontal) {
        bool shorten;
        int step;
        if (CalcBeamSlope(layer, staff, doc, beamInterface, shorten, step)) {
            CalcAdjustSlope(staff, doc, beamInterface, shorten, step);
        }
        else {
            CalcSetValues();
        }
    }
    else {

        // Find the longest stem length
        int maxLength = (beamInterface->m_drawingPlace == BEAMPLACE_above) ? VRV_UNSET : -VRV_UNSET;

        for (auto coord : m_beamElementCoordRefs) {
            if (!coord->m_stem) continue;

            if (beamInterface->m_drawingPlace == BEAMPLACE_above) {
                if (maxLength < coord->m_yBeam) maxLength = coord->m_yBeam;
            }
            else if (beamInterface->m_drawingPlace == BEAMPLACE_below) {
                if (maxLength > coord->m_yBeam) maxLength = coord->m_yBeam;
            }
            else if (beamInterface->m_drawingPlace == BEAMPLACE_mixed) {
                if (coord->m_beamRelativePlace == BEAMPLACE_above) {
                    if (maxLength < coord->m_yBeam) maxLength = coord->m_yBeam;
                }
                else if (coord->m_beamRelativePlace == BEAMPLACE_below) {
                    if (maxLength > coord->m_yBeam) maxLength = coord->m_yBeam;
                }
            }
        }

        if (-VRV_UNSET != abs(maxLength)) {
            m_beamElementCoordRefs.at(0)->m_yBeam = maxLength;
        }

        CalcSetValues();
    }
}

void BeamSegment::CalcAdjustSlope(Staff *staff, Doc *doc, BeamDrawingInterface *beamInterface, bool shorten, int &step)
{
    assert(staff);
    assert(doc);
    assert(beamInterface);

    this->CalcSetValues();

    const int unit = doc->GetDrawingUnit(staff->m_drawingStaffSize);

    int refLen = 0;
    if (beamInterface->m_drawingPlace == BEAMPLACE_above) {
        // upwards
        if (m_beamSlope > 0.0) {
            refLen = m_lastNoteOrChord->m_yBeam - m_lastNoteOrChord->m_closestNote->GetDrawingY();
        }
        // downwards
        else {
            refLen = m_firstNoteOrChord->m_yBeam - m_firstNoteOrChord->m_closestNote->GetDrawingY();
        }
    }
    else if (beamInterface->m_drawingPlace == BEAMPLACE_below) {
        // downwards
        if (m_beamSlope < 0.0) {
            refLen = m_lastNoteOrChord->m_closestNote->GetDrawingY() - m_lastNoteOrChord->m_yBeam;
        }
        // upwards
        else {
            refLen = m_firstNoteOrChord->m_closestNote->GetDrawingY() - m_firstNoteOrChord->m_yBeam;
        }
    }
    // We can actually tolerate a stem slightly shorter within the beam
    refLen -= unit;

    int lengthen = 0;
    for (auto coord : m_beamElementCoordRefs) {
        if (coord->m_stem && coord->m_closestNote) {
            // Here we should look at duration to because longer values in the middle could actually be OK as they are
            int len = abs(coord->m_yBeam - coord->m_closestNote->GetDrawingY());
            if (len < refLen) lengthen = std::max(lengthen, refLen - len);
        }
    }
    // We need to legthen the stems
    if (lengthen > 0) {
        // First if the slope step is 4 units (or more?) reduce it to 2 units and try again (recursive call)
        if (step >= 4 * unit) {
            step = 2 * unit;
            if (beamInterface->m_drawingPlace == BEAMPLACE_above) {
                // upwards
                if (m_beamSlope > 0.0) {
                    m_firstNoteOrChord->m_yBeam += 2 * unit;
                }
                // downwards
                else {
                    m_lastNoteOrChord->m_yBeam += 2 * unit;
                }
            }
            else if (beamInterface->m_drawingPlace == BEAMPLACE_below) {
                // downwards
                if (m_beamSlope < 0.0) {
                    m_firstNoteOrChord->m_yBeam -= 2 * unit;
                }
                // upwards
                else {
                    m_lastNoteOrChord->m_yBeam -= 2 * unit;
                }
            }

            // Reset the slop and the value
            this->m_beamSlope = BoundingBox::CalcSlope(Point(m_firstNoteOrChord->m_x, m_firstNoteOrChord->m_yBeam),
                Point(m_lastNoteOrChord->m_x, m_lastNoteOrChord->m_yBeam));

            this->CalcSetValues();
            // Try again - shortening will obviously be false at this stage
            return this->CalcAdjustSlope(staff, doc, beamInterface, false, step);
        }
        // Do lengthen the stems
        /*
        else {
            // We blindly extend it by unit. This can break the rule of the beam touching staff lines. To be improved
            int count = ceil( (double)lengthen/double(unit) );
            int lengthening = (beamInterface->m_drawingPlace == BEAMPLACE_below) ? -count * unit : count * unit;
            for (int i = 0; i < elementCount; i++) {
                BeamElementCoord *coord = m_beamElementCoordRefs.at(i);
                coord->m_yBeam += lengthening;
            }
            this->CalcSetValues(elementCount);
            return;
        }
        */
        // Other handling possibility by simply making the beam horizontal - not sure which one is best
        else {
            if (beamInterface->m_drawingPlace == BEAMPLACE_above) {
                // upwards
                if (m_beamSlope > 0.0) {
                    m_firstNoteOrChord->m_yBeam = m_lastNoteOrChord->m_yBeam;
                }
                // downwards
                else {
                    m_lastNoteOrChord->m_yBeam = m_firstNoteOrChord->m_yBeam;
                }
            }
            else if (beamInterface->m_drawingPlace == BEAMPLACE_below) {
                // downwards
                if (m_beamSlope < 0.0) {
                    m_firstNoteOrChord->m_yBeam = m_lastNoteOrChord->m_yBeam;
                }
                // upwards
                else {
                    m_lastNoteOrChord->m_yBeam = m_firstNoteOrChord->m_yBeam;
                }
            }

            // Reset the slop and the value
            this->m_beamSlope = BoundingBox::CalcSlope(Point(m_firstNoteOrChord->m_x, m_firstNoteOrChord->m_yBeam),
                Point(m_lastNoteOrChord->m_x, m_lastNoteOrChord->m_yBeam));

            this->CalcSetValues();
            // Simply ignore shortening
            return;
        }
    }

    /******************************************************************/
    // Shorten the stem length when possible

    // Diabled anyway - see CalcBeamSlope that never sets shorten to true
    /*
    if (shorten) {
        // LogDebug("Shorten true if %d", unit * 7);
        // First check that we actually can short, which means that all stem are at least 7 units
        for (int i = 0; i < elementCount; i++) {
            BeamElementCoord *coord = m_beamElementCoordRefs.at(i);
            if (coord->m_stem && coord->m_closestNote) {
                int len = abs(coord->m_yBeam - coord->m_closestNote->GetDrawingY());
                if ((len / unit) < 7) {
                    // LogDebug("Shorten false: %d", len);
                    shorten = false;
                    break;
                }
            }
        }

        // If we can, shorten by two units
        if (shorten) {
            int shortening = (beamInterface->m_drawingPlace == BEAMPLACE_below) ? 2 * unit : -2 * unit;
            for (int i = 0; i < elementCount; i++) {
                BeamElementCoord *coord = m_beamElementCoordRefs.at(i);
                coord->m_yBeam += shortening;
            }
        }
    }
    */
}

void BeamSegment::CalcBeamPlace(Layer *layer, BeamDrawingInterface *beamInterface, data_BEAMPLACE place)
{
    assert(layer);
    assert(beamInterface);

    if (place != BEAMPLACE_NONE) {
        /*
        if (beamInterface->m_hasMultipleStemDir && (place != BEAMPLACE_mixed)) {
            LogDebug("Stem directions (mixed) contradict beam placement (below or above)");
        }
        else if ((beamInterface->m_notesStemDir == STEMDIRECTION_up) && (place == BEAMPLACE_below)) {
            LogDebug("Stem directions (up) contradict beam placement (below)");
        }
        else if ((beamInterface->m_notesStemDir == STEMDIRECTION_down) && (place == BEAMPLACE_above)) {
            LogDebug("Stem directions (down) contradict beam placement (above)");
        }
        */
        beamInterface->m_drawingPlace = place;
    }
    // Default with cross-staff
    else if (beamInterface->m_hasMultipleStemDir) {
        beamInterface->m_drawingPlace = BEAMPLACE_mixed;
    }
    // Now look at the stem direction of the notes within the beam
    else if (beamInterface->m_notesStemDir == STEMDIRECTION_up) {
        beamInterface->m_drawingPlace = BEAMPLACE_above;
    }
    else if (beamInterface->m_notesStemDir == STEMDIRECTION_down) {
        beamInterface->m_drawingPlace = BEAMPLACE_below;
    }
    else if (beamInterface->m_hasCrossStaffContent) {
        beamInterface->m_drawingPlace = BEAMPLACE_mixed;
    }
    // Look at the layer direction or, finally, at the note position
    else {
        data_STEMDIRECTION layerStemDir = layer->GetDrawingStemDir(&m_beamElementCoordRefs);
        // Layer direction ?
        if (layerStemDir == STEMDIRECTION_NONE) {
            if (this->m_ledgerLinesBelow != this->m_ledgerLinesAbove) {
                beamInterface->m_drawingPlace
                    = (this->m_ledgerLinesBelow > this->m_ledgerLinesAbove) ? BEAMPLACE_above : BEAMPLACE_below;
            }
            else {
                beamInterface->m_drawingPlace
                    = (this->m_avgY < this->m_verticalCenter) ? BEAMPLACE_above : BEAMPLACE_below;
            }
        }
        // Look at the note position
        else {
            beamInterface->m_drawingPlace = (layerStemDir == STEMDIRECTION_up) ? BEAMPLACE_above : BEAMPLACE_below;
        }
    }

    // For now force it above
    // if (beamInterface->m_drawingPlace == BEAMPLACE_mixed) beamInterface->m_drawingPlace = BEAMPLACE_above;
}

void BeamSegment::CalcBeamStemLength(Staff *staff, data_STEMDIRECTION stemDir)
{
    const int stemDirBias = (stemDir == STEMDIRECTION_up) ? 1 : -1;
    for (auto coord : m_beamElementCoordRefs) {
        coord->SetClosestNote(stemDir);
        const int coordStemDir = coord->CalculateStemLength(staff, stemDir);
        if (stemDirBias * coordStemDir > stemDirBias * m_uniformStemLength) {
            m_uniformStemLength = coordStemDir;
        }
    }
    // make adjustments for the grace notes length
    for (auto coord : m_beamElementCoordRefs) {
        if (coord->m_element) {
            const bool isInGraceGroup = coord->m_element->GetFirstAncestor(GRACEGRP);
            if (coord->m_element->IsGraceNote() || isInGraceGroup) {
                m_uniformStemLength *= 0.75;
                break;
            }
        }
    }
}

void BeamSegment::CalcMixedBeamPlace(Staff *staff)
{
    const int currentStaffN = staff->GetN();
    const auto it = std::find_if(m_beamElementCoordRefs.begin(), m_beamElementCoordRefs.end(),
        [](auto coord) { return NULL != coord->m_element->m_crossStaff; });

    bool beamPlaceBelow = false;
    if (it != m_beamElementCoordRefs.end()) {
        Staff *currentCrossStaff = (*it)->m_element->m_crossStaff;
        const int crossStaffN = currentCrossStaff->GetN();
        beamPlaceBelow = currentStaffN <= crossStaffN;
    }

    for (auto coord : m_beamElementCoordRefs) {
        if (it != m_beamElementCoordRefs.end()) {
            if (!coord->m_element->m_crossStaff) {
                coord->m_beamRelativePlace = beamPlaceBelow ? BEAMPLACE_below : BEAMPLACE_above;
            }
            else {
                coord->m_beamRelativePlace = !beamPlaceBelow ? BEAMPLACE_below : BEAMPLACE_above;
            }
        }
        else {
            coord->m_beamRelativePlace = (STEMDIRECTION_up == coord->GetStemDir()) ? BEAMPLACE_above : BEAMPLACE_below;
        }
    }
}

void BeamSegment::CalcPartialFlagPlace()
{
    // Start from note that is shorter than DUR_8 - we do not care otherwise, since those do not have addiitonal beams
    auto start = std::find_if(m_beamElementCoordRefs.begin(), m_beamElementCoordRefs.end(),
        [](BeamElementCoord *coord) { return coord->m_dur >= DUR_16; });
    if (m_beamElementCoordRefs.end() == start) return;
    while (start != m_beamElementCoordRefs.end()) {
        auto subdivision = start;
        data_BEAMPLACE place = (*start)->m_beamRelativePlace;
        bool isProcessed = false;
        bool breakSec = false;
        // Process beam as a collection of subdivision. Subdivision will extend as long as we don't encounter 8th note
        // or direction changes
        while (true) {
            if (breakSec) break;
            // Find first note longer than 8th or first note that is cross-staff
            auto found = std::find_if(subdivision, m_beamElementCoordRefs.end(), [&](BeamElementCoord *coord) {
                if (coord->m_element->Is(REST)) return false;
                return ((coord->m_beamRelativePlace != place) || (coord->m_dur <= DUR_8) || (coord->m_breaksec));
            });
            subdivision = found;

            // Handle different cases, where we either don't want to proceed (e.g. end of the beam reached) or we want
            // to process them separately (e.g. on direction change from shorter to longer notes, or vice versa, we do
            // not want last note of the subdivision to have additional beam, so that it's clearly distinguishable).
            if ((m_beamElementCoordRefs.end() == found) || ((*found)->m_dur <= DUR_8)) break;
            if (((*found)->m_breaksec)) breakSec = true;
            if ((m_beamElementCoordRefs.end() - 1) == found) {
                subdivision = m_beamElementCoordRefs.end();
                isProcessed = true;
                break;
            }

            // If no other conditions are hit - this is proper cross-staff case, so change drawing place to that of the
            // new direction
            place = (*found)->m_beamRelativePlace;
        }
        std::for_each(start, subdivision,
            [place](BeamElementCoord *coord) { coord->m_partialFlagPlace = (data_BEAMPLACE)((place % 2) + 1); });
        if (isProcessed) break;
        if (m_beamElementCoordRefs.end() != subdivision) ++subdivision;

        start = subdivision;
    }
}

void BeamSegment::CalcSetValues()
{
    this->m_startingX = m_beamElementCoordRefs.at(0)->m_x;
    this->m_startingY = m_beamElementCoordRefs.at(0)->m_yBeam;

    for (auto coord : m_beamElementCoordRefs) {
        coord->m_yBeam = this->m_startingY + this->m_beamSlope * (coord->m_x - this->m_startingX);
    }
}

//----------------------------------------------------------------------------
// Beam
//----------------------------------------------------------------------------

Beam::Beam()
    : LayerElement("beam-"), ObjectListInterface(), BeamDrawingInterface(), AttColor(), AttBeamedWith(), AttBeamRend()
{
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_BEAMEDWITH);
    RegisterAttClass(ATT_BEAMREND);

    Reset();
}

Beam::~Beam() {}

void Beam::Reset()
{
    LayerElement::Reset();
    BeamDrawingInterface::Reset();
    ResetColor();
    ResetBeamedWith();
    ResetBeamRend();
}

bool Beam::IsSupportedChild(Object *child)
{
    if (child->Is(BEAM)) {
        assert(dynamic_cast<Beam *>(child));
    }
    else if (child->Is(BTREM)) {
        assert(dynamic_cast<BTrem *>(child));
    }
    else if (child->Is(CHORD)) {
        assert(dynamic_cast<Chord *>(child));
    }
    else if (child->Is(CLEF)) {
        assert(dynamic_cast<Clef *>(child));
    }
    else if (child->Is(GRACEGRP)) {
        assert(dynamic_cast<GraceGrp *>(child));
    }
    else if (child->Is(NOTE)) {
        assert(dynamic_cast<Note *>(child));
    }
    else if (child->Is(REST)) {
        assert(dynamic_cast<Rest *>(child));
    }
    else if (child->Is(SPACE)) {
        assert(dynamic_cast<Space *>(child));
    }
    else if (child->Is(TUPLET)) {
        assert(dynamic_cast<Tuplet *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

void Beam::FilterList(ArrayOfObjects *childList)
{
    bool firstNoteGrace = false;
    // We want to keep only notes and rests
    // Eventually, we also need to filter out grace notes properly (e.g., with sub-beams)
    ArrayOfObjects::iterator iter = childList->begin();

    while (iter != childList->end()) {
        if (!(*iter)->IsLayerElement()) {
            // remove anything that is not an LayerElement (e.g. Verse, Syl, etc)
            iter = childList->erase(iter);
            continue;
        }
        if (!(*iter)->HasInterface(INTERFACE_DURATION)) {
            // remove anything that has not a DurationInterface
            iter = childList->erase(iter);
            continue;
        }
        else {
            LayerElement *element = vrv_cast<LayerElement *>(*iter);
            assert(element);
            // if we are at the beginning of the beam
            // and the note is cueSize
            // assume all the beam is of grace notes
            const bool isInGraceGroup = element->GetFirstAncestor(GRACEGRP);
            if (childList->begin() == iter) {
                if (element->IsGraceNote() || isInGraceGroup) firstNoteGrace = true;
            }
            // if the first note in beam was NOT a grace
            // we have grace notes embedded in a beam
            // drop them
            if (!firstNoteGrace && (element->IsGraceNote() || isInGraceGroup)) {
                iter = childList->erase(iter);
                continue;
            }
            // also remove notes within chords
            if (element->Is(NOTE)) {
                Note *note = vrv_cast<Note *>(element);
                assert(note);
                if (note->IsChordTone()) {
                    iter = childList->erase(iter);
                    continue;
                }
            }
            ++iter;
        }
    }

    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);
    Staff *beamStaff = staff;
    /*
    if (this->HasBeamWith()) {
        Measure *measure = vrv_cast<Measure *>(this->GetFirstAncestor(MEASURE));
        assert(measure);
        if (this->GetBeamWith() == OTHERSTAFF_below) {
            beamStaff = dynamic_cast<Staff *>(measure->GetNext(staff, STAFF));
            if (beamStaff == NULL) {
                LogError("Cannot access staff below for beam '%s'", this->GetUuid().c_str());
                beamStaff = staff;
            }
        }
        else if (this->GetBeamWith() == OTHERSTAFF_above) {
            beamStaff = dynamic_cast<Staff *>(measure->GetPrevious(staff, STAFF));
            if (beamStaff == NULL) {
                LogError("Cannot access staff above for beam '%s'", this->GetUuid().c_str());
                beamStaff = staff;
            }
        }
    }
    */

    InitCoords(childList, beamStaff, this->GetPlace());
}

int Beam::GetPosition(LayerElement *element)
{
    this->GetList(this);
    int position = this->GetListIndex(element);
    // Check if this is a note in the chord
    if ((position == -1) && (element->Is(NOTE))) {
        Note *note = vrv_cast<Note *>(element);
        assert(note);
        Chord *chord = note->IsChordTone();
        if (chord) position = this->GetListIndex(chord);
    }
    return position;
}

bool Beam::IsFirstInBeam(LayerElement *element)
{
    this->GetList(this);
    int position = this->GetPosition(element);
    // This method should be called only if the note is part of a beam
    assert(position != -1);
    // this is the first one
    if (position == 0) return true;
    return false;
}

bool Beam::IsLastInBeam(LayerElement *element)
{
    int size = (int)this->GetList(this)->size();
    int position = this->GetPosition(element);
    // This method should be called only if the note is part of a beam
    assert(position != -1);
    // this is the last one
    if (position == (size - 1)) return true;
    return false;
}

const ArrayOfBeamElementCoords *Beam::GetElementCoords()
{
    this->GetList(this);
    return &m_beamElementCoords;
}

//----------------------------------------------------------------------------
// BeamElementCoord
//----------------------------------------------------------------------------

BeamElementCoord::~BeamElementCoord() {}

data_STEMDIRECTION BeamElementCoord::GetStemDir()
{
    // m_stem is not necssary set, so we need to look at the Note / Chord original value
    // Example: IsInBeam called in Note::PrepareLayerElementParts when reaching the first note of the beam
    if (m_stem) {
        return m_stem->GetStemDir();
    }

    if (!m_element) {
        LogDebug("Element not set in BeamElementCoord");
        return STEMDIRECTION_NONE;
    }

    AttStems *stemInterface = dynamic_cast<AttStems *>(m_element);

    if (!stemInterface) {
        LogDebug("Element is not an AttStems");
        return STEMDIRECTION_NONE;
    }

    return stemInterface->GetStemDir();
}

void BeamElementCoord::SetDrawingStemDir(
    data_STEMDIRECTION stemDir, Staff *staff, Doc *doc, BeamSegment *segment, BeamDrawingInterface *interface)
{
    assert(staff);
    assert(doc);
    assert(interface);

    if (m_element->Is(REST)) {
        this->m_x += m_element->GetDrawingRadius(doc);
        return;
    }

    if (!this->m_element->Is({ CHORD, NOTE })) return;

    StemmedDrawingInterface *stemInterface = this->m_element->GetStemmedDrawingInterface();
    assert(stemInterface);
    m_stem = stemInterface->GetDrawingStem();
    assert(m_stem);

    const int unit = doc->GetDrawingUnit(staff->m_drawingStaffSize);

    this->m_stem->SetDrawingStemDir(stemDir);
    int ledgerLines = 0;
    int ledgerLinesOpposite = 0;
    this->m_shortened = false;

    this->m_yBeam = this->m_element->GetDrawingY();
    this->m_x += (STEMDIRECTION_up == stemDir) ? interface->m_stemXAbove[interface->m_cueSize]
                                               : interface->m_stemXBelow[interface->m_cueSize];
    if (!m_closestNote) return;

    this->m_yBeam = m_closestNote->GetDrawingY();
    if (stemDir == STEMDIRECTION_up) {
        m_closestNote->HasLedgerLines(ledgerLinesOpposite, ledgerLines);
    }
    else {
        m_closestNote->HasLedgerLines(ledgerLines, ledgerLinesOpposite);
    }

    int stemLen = segment->m_uniformStemLength;
    if (interface->m_hasCrossStaffContent || (BEAMPLACE_mixed == interface->m_drawingPlace)) {
        if (((STEMDIRECTION_up == stemDir) && (stemLen < 0)) || ((STEMDIRECTION_down == stemDir) && (stemLen > 0))) {
            stemLen *= -1;
        }
    }
    this->m_centered = segment->m_uniformStemLength % 2;
    this->m_yBeam += (stemLen * doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2);

    const bool isInGraceGroup = m_element->GetFirstAncestor(GRACEGRP);
    if (m_element->IsGraceNote() || isInGraceGroup) return;

    // Make sure the stem reaches the center of the staff
    // Mark the segment as extendedToCenter since we then want a reduced slope
    if (interface->m_hasCrossStaffContent || (BEAMPLACE_mixed == interface->m_drawingPlace)) {
        segment->m_extendedToCenter = false;
    }
    else if (((stemDir == STEMDIRECTION_up) && (this->m_yBeam <= segment->m_verticalCenter))
        || ((stemDir == STEMDIRECTION_down) && (segment->m_verticalCenter <= this->m_yBeam))) {
        this->m_yBeam = segment->m_verticalCenter;
        segment->m_extendedToCenter = true;
        this->m_centered = false;
    }
    else {
        segment->m_extendedToCenter = false;
    }

    // Make sure there is a at least one staff space before the ledger lines
    if ((ledgerLines > 2) && (interface->m_shortestDur > DUR_32)) {
        this->m_yBeam += (stemDir == STEMDIRECTION_up) ? 4 * unit : -4 * unit;
    }
    else if ((ledgerLines > 1) && (interface->m_shortestDur > DUR_16)) {
        this->m_yBeam += (stemDir == STEMDIRECTION_up) ? 2 * unit : -2 * unit;
    }

    this->m_yBeam += m_overlapMargin;
}

int BeamElementCoord::CalculateStemLength(Staff *staff, data_STEMDIRECTION stemDir)
{
    if (!m_closestNote) return 0;

    const bool onStaffLine = m_closestNote->GetDrawingLoc() % 2;
    bool extend = onStaffLine;
    const int standardStemLen = STANDARD_STEMLENGTH * 2;
    // Check if the stem has to be shortened because outside the staff
    // In this case, Note::CalcStemLenInThirdUnits will return a value shorter than 2 * STANDARD_STEMLENGTH
    const int stemLenInHalfUnits = m_closestNote->CalcStemLenInThirdUnits(staff) * 2 / 3;
    // Do not extend when not on the staff line
    if (stemLenInHalfUnits != standardStemLen) {
        this->m_shortened = true;
        extend = false;
    }

    int stemLen = (stemDir == STEMDIRECTION_up) ? 1 : -1;
    // For 8th notes, use the shortened stem (if shortened)
    if (this->m_dur == DUR_8) {
        if (stemLenInHalfUnits != standardStemLen) {
            stemLen *= stemLenInHalfUnits;
        }
        else {
            stemLen *= (onStaffLine) ? 14 : 13;
        }
    }
    else {
        switch (this->m_dur) {
            case (DUR_16): stemLen *= (extend) ? 14 : 13; break;
            case (DUR_32): stemLen *= (extend) ? 18 : 16; break;
            case (DUR_64): stemLen *= (extend) ? 22 : 20; break;
            case (DUR_128): stemLen *= (extend) ? 26 : 24; break;
            case (DUR_256): stemLen *= (extend) ? 30 : 28; break;
            case (DUR_512): stemLen *= (extend) ? 34 : 32; break;
            case (DUR_1024): stemLen *= (extend) ? 38 : 36; break;
            default: stemLen *= 14;
        }
    }

    return stemLen;
}

void BeamElementCoord::SetClosestNote(data_STEMDIRECTION stemDir)
{
    m_closestNote = NULL;
    if (m_element->Is(NOTE)) {
        m_closestNote = vrv_cast<Note *>(m_element);
    }
    else if (m_element->Is(CHORD)) {
        Chord *chord = vrv_cast<Chord *>(m_element);
        assert(chord);
        m_closestNote = (STEMDIRECTION_up == stemDir) ? chord->GetTopNote() : chord->GetBottomNote();
    }
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Beam::AdjustBeams(FunctorParams *functorParams)
{
    AdjustBeamParams *params = vrv_params_cast<AdjustBeamParams *>(functorParams);
    assert(params);

    if (this->HasSameas() || !this->GetChildCount()) {
        return FUNCTOR_CONTINUE;
    }

    // process highest-level beam
    if (!params->m_beam) {
        params->m_beam = this;
        params->m_y1 = (*m_beamSegment.m_beamElementCoordRefs.begin())->m_yBeam;
        params->m_y2 = m_beamSegment.m_beamElementCoordRefs.back()->m_yBeam;
        params->m_directionBias = (m_drawingPlace == BEAMPLACE_above) ? 1 : -1;
        return FUNCTOR_CONTINUE;
    }

    // const int directionBias = (vrv_cast<Beam *>(params->m_beam)->m_drawingPlace == BEAMPLACE_above) ? 1 : -1;

    const int leftMargin = (*m_beamSegment.m_beamElementCoordRefs.begin())->m_yBeam - params->m_y1;
    const int rightMargin = m_beamSegment.m_beamElementCoordRefs.back()->m_yBeam - params->m_y2;

    const int overlapMargin = std::max(leftMargin * params->m_directionBias, rightMargin * params->m_directionBias);
    if (overlapMargin >= params->m_overlapMargin) {
        Staff *staff = vrv_cast<Staff *>(GetFirstAncestor(STAFF));
        assert(staff);
        const int staffOffset = params->m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        params->m_overlapMargin = (overlapMargin + staffOffset) * params->m_directionBias;
    }
    return FUNCTOR_SIBLINGS;
}

int Beam::AdjustBeamsEnd(FunctorParams *functorParams)
{
    AdjustBeamParams *params = vrv_params_cast<AdjustBeamParams *>(functorParams);
    assert(params);

    if (params->m_beam != this) return FUNCTOR_CONTINUE;

    // set overlap margin for each coord in the beam
    if (params->m_overlapMargin) {
        std::for_each(m_beamSegment.m_beamElementCoordRefs.begin(), m_beamSegment.m_beamElementCoordRefs.end(),
            [overlap = params->m_overlapMargin](BeamElementCoord *coord) { coord->m_overlapMargin = overlap; });
    }
    params->m_beam = NULL;
    params->m_overlapMargin = 0;

    return FUNCTOR_CONTINUE;
}

int Beam::CalcStem(FunctorParams *functorParams)
{
    CalcStemParams *params = vrv_params_cast<CalcStemParams *>(functorParams);
    assert(params);

    const ArrayOfObjects *beamChildren = this->GetList(this);

    // Should we assert this at the beginning?
    if (beamChildren->empty()) {
        return FUNCTOR_CONTINUE;
    }

    this->m_beamSegment.InitCoordRefs(this->GetElementCoords());

    Layer *layer = vrv_cast<Layer *>(this->GetFirstAncestor(LAYER));
    assert(layer);
    Staff *staff = vrv_cast<Staff *>(layer->GetFirstAncestor(STAFF));
    assert(staff);

    this->m_beamSegment.CalcBeam(layer, staff, params->m_doc, this, this->GetPlace());

    return FUNCTOR_CONTINUE;
}

int Beam::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);

    this->m_beamSegment.Reset();

    // We want the list of the ObjectListInterface to be re-generated
    this->Modify();

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
