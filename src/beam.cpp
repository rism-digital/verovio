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

    int i, y1, y2;
    int elementCount = (int)m_beamElementCoordRefs.size();
    assert(elementCount > 0);

    // For recursive calls, avoid to re-init values
    if (init) {
        this->CalcBeamInit(layer, staff, doc, beamInterface, place);
    }

    bool horizontal = beamInterface->IsHorizontal();

    // Beam@place has precedence - however, in some cases, CalcBeam is called recusively because we need to change the
    // place This occurs when mixed makes no sense and the beam is placed above or below instead.
    this->CalcBeamPlace(layer, beamInterface, place);

    CalcBeamStemLength(staff, beamInterface->m_drawingPlace == BEAMPLACE_below ? STEMDIRECTION_down : STEMDIRECTION_up);

    // Set drawing stem positions
    for (i = 0; i < elementCount; ++i) {
        BeamElementCoord *coord = m_beamElementCoordRefs.at(i);

        if (beamInterface->m_drawingPlace == BEAMPLACE_above) {
            coord->SetDrawingStemDir(STEMDIRECTION_up, staff, doc, this, beamInterface);
        }
        else if (beamInterface->m_drawingPlace == BEAMPLACE_below) {
            coord->SetDrawingStemDir(STEMDIRECTION_down, staff, doc, this, beamInterface);
        }
        // cross-staff or beam@place=mixed
        else {
            if (beamInterface->m_isCrossStaff) {
                // TODO - look at staff@n and set the stem direction
                Staff *currentCrossStaff = coord->m_element->m_crossStaff;
                if (currentCrossStaff) {
                    // if (currentCrossStaff->GetN() < staff->GetN()
                }
            }
            else {
                data_STEMDIRECTION stemDir = coord->GetStemDir();
                // TODO - Handle cases where there is no given stem direction (here we can still have NONE)
                coord->SetDrawingStemDir(stemDir, staff, doc, this, beamInterface);
            }
        }
    }

    // ArrayOfBeamElementCoords stemUps;
    // ArrayOfBeamElementCoords stemDowns;

    /******************************************************************/
    // Calculate the slope is necessary

    this->m_beamSlope = 0.0;
    if (!horizontal) {
        bool shorten;
        int step;
        if (this->CalcBeamSlope(layer, staff, doc, beamInterface, shorten, step)) {
            this->CalcAdjustSlope(staff, doc, beamInterface, shorten, step, elementCount);
        }
        else {
            this->CalcSetValues(elementCount);
        }
    }
    else {

        // Find the longest stem length
        int maxLength = (beamInterface->m_drawingPlace == BEAMPLACE_above) ? VRV_UNSET : -VRV_UNSET;

        for (i = 0; i < elementCount; ++i) {
            BeamElementCoord *coord = m_beamElementCoordRefs.at(i);
            if (!coord->m_stem) continue;

            if (beamInterface->m_drawingPlace == BEAMPLACE_above) {
                if (maxLength < coord->m_yBeam) maxLength = coord->m_yBeam;
            }
            else if (beamInterface->m_drawingPlace == BEAMPLACE_below) {
                if (maxLength > coord->m_yBeam) maxLength = coord->m_yBeam;
            }
        }

        m_beamElementCoordRefs.at(0)->m_yBeam = maxLength;

        this->CalcSetValues(elementCount);
    }

    /******************************************************************/
    // Set the stem lengths to stem objects

    for (i = 0; i < elementCount; ++i) {
        BeamElementCoord *coord = m_beamElementCoordRefs.at(i);
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
            else {
                y1 += doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
                y2 += stemmedInterface->GetStemDownNW(doc, staff->m_drawingStaffSize, beamInterface->m_cueSize).y;
            }

            if (coord->m_element->Is(CHORD)) {
                Chord *chord = vrv_cast<Chord *>(coord->m_element);
                assert(chord);
                int yMax, yMin;
                chord->GetYExtremes(yMax, yMin);
                y2 += (beamInterface->m_drawingPlace == BEAMPLACE_above) ? (yMin - yMax) : (yMax - yMin);
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
    if ((curStep != 0) && (curStep < step)) {
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

void BeamSegment::CalcAdjustSlope(
    Staff *staff, Doc *doc, BeamDrawingInterface *beamInterface, bool shorten, int &step, const int &elementCount)
{
    assert(staff);
    assert(doc);
    assert(beamInterface);

    this->CalcSetValues(elementCount);

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
    for (int i = 0; i < elementCount; i++) {
        BeamElementCoord *coord = m_beamElementCoordRefs.at(i);
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

            this->CalcSetValues(elementCount);
            // Try again - shortening will obviously be false at this stage
            return this->CalcAdjustSlope(staff, doc, beamInterface, false, step, elementCount);
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

            this->CalcSetValues(elementCount);
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
    else if (beamInterface->m_isCrossStaff) {
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
    if (beamInterface->m_drawingPlace == BEAMPLACE_mixed) beamInterface->m_drawingPlace = BEAMPLACE_above;
}

void BeamSegment::CalcBeamStemLength(Staff *staff, data_STEMDIRECTION stemDir)
{
    const int stemDirBias = (stemDir == STEMDIRECTION_up) ? 1 : -1;
    for (auto coord : m_beamElementCoordRefs) {
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

void BeamSegment::CalcSetValues(const int &elementCount)
{
    this->m_startingX = m_beamElementCoordRefs.at(0)->m_x;
    this->m_startingY = m_beamElementCoordRefs.at(0)->m_yBeam;

    for (int i = 0; i < elementCount; i++) {
        BeamElementCoord *coord = m_beamElementCoordRefs.at(i);
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
    this->m_closestNote = NULL;

    this->m_yBeam = this->m_element->GetDrawingY();

    if (this->m_element->Is(NOTE)) {
        m_closestNote = dynamic_cast<Note *>(this->m_element);
    }

    if (stemDir == STEMDIRECTION_up) {
        this->m_x += interface->m_stemXAbove[interface->m_cueSize];
        if (this->m_element->Is(CHORD)) {
            Chord *chord = vrv_cast<Chord *>(this->m_element);
            assert(chord);
            m_closestNote = chord->GetTopNote();
        }
        if (m_closestNote) {
            this->m_yBeam = m_closestNote->GetDrawingY();
            m_closestNote->HasLedgerLines(ledgerLinesOpposite, ledgerLines);
        }
    }
    else {
        this->m_x += interface->m_stemXBelow[interface->m_cueSize];
        if (this->m_element->Is(CHORD)) {
            Chord *chord = vrv_cast<Chord *>(this->m_element);
            assert(chord);
            m_closestNote = chord->GetBottomNote();
        }
        if (m_closestNote) {
            this->m_yBeam = m_closestNote->GetDrawingY();
            m_closestNote->HasLedgerLines(ledgerLines, ledgerLinesOpposite);
        }
    }

    if (!m_closestNote) return;

    this->m_centered = segment->m_uniformStemLength % 2;
    this->m_yBeam += (segment->m_uniformStemLength * doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2);

    const bool isInGraceGroup = m_element->GetFirstAncestor(GRACEGRP);
    if (m_element->IsGraceNote() || isInGraceGroup) return;

    // Make sure the stem reaches the center of the staff
    // Mark the segment as extendedToCenter since we then want a reduced slope
    if (((stemDir == STEMDIRECTION_up) && (this->m_yBeam <= segment->m_verticalCenter))
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

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Beam::AdjustBeams(FunctorParams *functorParams)
{
    AdjustBeamParams *params = vrv_params_cast<AdjustBeamParams *>(functorParams);
    assert(params);

    // process highest-level beam
    if (!params->m_beam) {
        params->m_beam = this;
        params->m_y1 = (*m_beamSegment.m_beamElementCoordRefs.begin())->m_yBeam;
        params->m_y2 = m_beamSegment.m_beamElementCoordRefs.back()->m_yBeam;
        params->m_directionBias = (m_drawingPlace == BEAMPLACE_above) ? 1 : -1;
        return FUNCTOR_CONTINUE;
    }

    //const int directionBias = (vrv_cast<Beam *>(params->m_beam)->m_drawingPlace == BEAMPLACE_above) ? 1 : -1;

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
