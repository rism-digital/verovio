/////////////////////////////////////////////////////////////////////////////
// Name:        beam.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "beam.h"

//----------------------------------------------------------------------------

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

void BeamSegment::CalcBeam(Layer *layer, Staff *staff, Doc *doc, BeamDrawingInterface *beamInterface, data_BEAMPLACE place)
{
    assert(layer);
    assert(staff);
    assert(doc);

    int elementCount = (int)m_beamElementCoordRefs.size();
    assert(elementCount > 0);

    int y1, y2, avgY, high, low, verticalCenter, verticalShift;
    double xr, verticalShiftFactor;

    // loop
    int i;

    // position x for the stem (normal and cue-sized)
    int stemX[2];

    // For slope calculation and linear regression
    double s_x = 0.0; // sum of all x(n) for n in beamElementCoord
    double s_y = 0.0; // sum of all y(n)
    double s_xy = 0.0; // sum of (x(n) * y(n))
    double s_x2 = 0.0; // sum of all x(n)^2
    double s_y2 = 0.0; // sum of all y(n)^2

    /******************************************************************/
    // initialization

    for (i = 0; i < elementCount; ++i) {
        m_beamElementCoordRefs.at(i)->m_x = m_beamElementCoordRefs.at(i)->m_element->GetDrawingX();
    }

    // make it relative to the first x / staff y
    int xRel = m_beamElementCoordRefs.at(0)->m_x;
    int yRel = staff->GetDrawingY();

    avgY = 0;
    high = VRV_UNSET;
    low = -VRV_UNSET;

    verticalShiftFactor = 3.0;
    verticalCenter = staff->GetDrawingY()
        - (doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * 2); // center point of the staff

    int last = elementCount - 1;

    /******************************************************************/
    // Calculate the extreme values

    int yMax = 0, yMin = 0;
    int curY;
    int nbRests = 0;
    // elementCount holds the last one
    for (i = 0; i < elementCount; ++i) {
        if (m_beamElementCoordRefs.at(i)->m_element->Is(CHORD)) {
            Chord *chord = dynamic_cast<Chord *>(m_beamElementCoordRefs.at(i)->m_element);
            assert(chord);
            chord->GetYExtremes(yMax, yMin);
            m_beamElementCoordRefs.at(i)->m_yTop = yMax;
            m_beamElementCoordRefs.at(i)->m_yBottom = yMin;

            avgY += ((yMax + yMin) / 2);

            // highest and lowest value;
            high = std::max(yMax, high);
            low = std::min(yMin, low);
        }
        else if (m_beamElementCoordRefs.at(i)->m_element->Is(NOTE)) {
            m_beamElementCoordRefs.at(i)->m_y = m_beamElementCoordRefs.at(i)->m_element->GetDrawingY();

            // highest and lowest value;
            high = std::max(m_beamElementCoordRefs.at(i)->m_y, high);
            low = std::min(m_beamElementCoordRefs.at(i)->m_y, low);

            curY = m_beamElementCoordRefs.at(i)->m_element->GetDrawingY();
            m_beamElementCoordRefs.at(i)->m_yTop = curY;
            m_beamElementCoordRefs.at(i)->m_yBottom = curY;
            avgY += m_beamElementCoordRefs.at(i)->m_y;
        }
        else {
            m_beamElementCoordRefs.at(i)->m_y = m_beamElementCoordRefs.at(i)->m_element->GetDrawingY();

            curY = m_beamElementCoordRefs.at(i)->m_element->GetDrawingY();
            m_beamElementCoordRefs.at(i)->m_yTop = curY;
            m_beamElementCoordRefs.at(i)->m_yBottom = curY;
            nbRests++;
        }
    }

    /******************************************************************/
    // Set the stem direction

    // Only if not only rests. (Will produce non-sense output anyway)
    if (elementCount != nbRests) {
        avgY /= (elementCount - nbRests);
    }

    // Beam@place has precedence in all cases
    if (place != BEAMPLACE_NONE) {
        if (beamInterface->m_hasMultipleStemDir && (place != BEAMPLACE_mixed)) {
            LogDebug("Stem directions (mixed) contradict beam placement (below or above)");
        }
        else if ((beamInterface->m_stemDir == STEMDIRECTION_up) && (place == BEAMPLACE_below)) {
            LogDebug("Stem directions (up) contradict beam placement (below)");
        }
        else if ((beamInterface->m_stemDir == STEMDIRECTION_down) && (place == BEAMPLACE_above)) {
            LogDebug("Stem directions (down) contradict beam placement (above)");
        }
        beamInterface->m_beamPlace = place;
    }
    // Default with cross-staff
    else if (beamInterface->m_crossStaff) {
        beamInterface->m_beamPlace = BEAMPLACE_mixed;
    }
    else if (beamInterface->m_hasMultipleStemDir) {
        beamInterface->m_beamPlace = BEAMPLACE_mixed;
    }
    else {
        // force layer direction if it exists and nothing in the notes
        if (beamInterface->m_stemDir == STEMDIRECTION_NONE) {
            beamInterface->m_stemDir = layer->GetDrawingStemDir(&m_beamElementCoordRefs);
        }
        // Now look at the stem direction
        if (beamInterface->m_stemDir == STEMDIRECTION_up) {
            beamInterface->m_beamPlace = BEAMPLACE_above;
        }
        else if (beamInterface->m_stemDir == STEMDIRECTION_down) {
            beamInterface->m_beamPlace = BEAMPLACE_below;
        }
        else {
            beamInterface->m_beamPlace = (avgY < verticalCenter) ? BEAMPLACE_below : BEAMPLACE_below;
        }
    }
    assert(beamInterface->m_beamPlace != BEAMPLACE_NONE);
    
    // Temporary fix to set the stem dir
    beamInterface->m_stemDir = (beamInterface->m_beamPlace == BEAMPLACE_below) ? STEMDIRECTION_down : STEMDIRECTION_up;

    for (i = 0; i < elementCount; ++i) {
        if (!m_beamElementCoordRefs.at(i)->m_stem) continue;

        if (beamInterface->m_beamPlace == BEAMPLACE_above) {
            m_beamElementCoordRefs.at(i)->m_stem->SetDrawingStemDir(STEMDIRECTION_up);
        }
        else if (beamInterface->m_beamPlace == BEAMPLACE_below) {
            m_beamElementCoordRefs.at(i)->m_stem->SetDrawingStemDir(STEMDIRECTION_down);
        }
        // mixed
        else {
            if (beamInterface->m_crossStaff) {
                Staff *currentCrossStaff = m_beamElementCoordRefs.at(i)->m_element->m_crossStaff;
                if (currentCrossStaff) {
                    // if (currentCrossStaff->GetN() < staff->GetN()
                }
            }
        }
    }

    if (beamInterface->m_beamPlace == BEAMPLACE_above) { // set stem direction for all the notes
        for (i = 0; i < elementCount; ++i) {
            m_beamElementCoordRefs.at(i)->m_y = m_beamElementCoordRefs.at(i)->m_yTop;
        }
    }
    else {
        for (i = 0; i < elementCount; ++i) {
            m_beamElementCoordRefs.at(i)->m_y = m_beamElementCoordRefs.at(i)->m_yBottom;
        }
    }

    beamInterface->m_beamWidthBlack = doc->GetDrawingBeamWidth(staff->m_drawingStaffSize, beamInterface->m_cueSize);
    beamInterface->m_beamWidthWhite = doc->GetDrawingBeamWhiteWidth(staff->m_drawingStaffSize, beamInterface->m_cueSize);
    beamInterface->m_beamWidth = beamInterface->m_beamWidthBlack + beamInterface->m_beamWidthWhite;

    /******************************************************************/
    // Calculate the slope doing a linear regression

    // The vertical shift depends on the shortestDur value we have in the beam
    verticalShift = ((beamInterface->m_shortestDur - DUR_8) * (beamInterface->m_beamWidth));

    // if the beam has smaller-size notes
    if (m_beamElementCoordRefs.at(last)->m_element->GetDrawingCueSize()) {
        verticalShift += doc->GetDrawingUnit(staff->m_drawingStaffSize) * 5;
    }
    else {
        verticalShift += (beamInterface->m_shortestDur > DUR_8)
            ? doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * verticalShiftFactor
            : doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * (verticalShiftFactor + 0.5);
    }

    // swap x position and verticalShift direction with stem down
    if (beamInterface->m_beamPlace == BEAMPLACE_above) {
        // x-offset values for stem bases, dx[y] where y = element->m_cueSize
        stemX[0] = doc->GetGlyphWidth(SMUFL_E0A3_noteheadHalf, staff->m_drawingStaffSize, false)
            - (doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;
        stemX[1] = doc->GetGlyphWidth(SMUFL_E0A3_noteheadHalf, staff->m_drawingStaffSize, true)
            - (doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;
    }
    else {
        stemX[0] = (doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;
        stemX[1] = (doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;
        verticalShift = -verticalShift;
    }

    for (i = 0; i < elementCount; ++i) {
        // change the stem dir for all objects
        if (m_beamElementCoordRefs.at(i)->m_element->Is(NOTE)) {
            ((Note *)m_beamElementCoordRefs.at(i)->m_element)->SetDrawingStemDir(beamInterface->m_stemDir);
        }
        else if (m_beamElementCoordRefs.at(i)->m_element->Is(CHORD)) {
            ((Chord *)m_beamElementCoordRefs.at(i)->m_element)->SetDrawingStemDir(beamInterface->m_stemDir);
        }

        m_beamElementCoordRefs.at(i)->m_yBeam = m_beamElementCoordRefs.at(i)->m_y + verticalShift;
        m_beamElementCoordRefs.at(i)->m_x += stemX[beamInterface->m_cueSize];
    }

    for (i = 0; i < elementCount; i++) {
        s_y += m_beamElementCoordRefs.at(i)->m_yBeam - yRel;
        s_y2 += pow(m_beamElementCoordRefs.at(i)->m_yBeam - yRel, 2);
        s_x += m_beamElementCoordRefs.at(i)->m_x - xRel;
        s_x2 += pow(m_beamElementCoordRefs.at(i)->m_x - xRel, 2);
        s_xy += (m_beamElementCoordRefs.at(i)->m_x - xRel) * (m_beamElementCoordRefs.at(i)->m_yBeam - yRel);
    }

    y1 = elementCount * s_xy - s_x * s_y;
    xr = elementCount * s_x2 - s_x * s_x;

    // Prevent division by 0
    if (y1 && xr) {
        this->m_beamSlope = y1 / xr;
    }
    else {
        this->m_beamSlope = 0.0;
    }

    /* Correction esthetique : */
    if (fabs(this->m_beamSlope) < doc->m_drawingBeamMinSlope) this->m_beamSlope = 0.0;
    if (fabs(this->m_beamSlope) > doc->m_drawingBeamMaxSlope)
        this->m_beamSlope = (this->m_beamSlope > 0) ? doc->m_drawingBeamMaxSlope : -doc->m_drawingBeamMaxSlope;
    /* pente correcte: entre 0 et env 0.4 (0.2 a 0.4) */

    this->m_startingX = xRel;
    this->m_startingY = m_beamElementCoordRefs.at(0)->m_yBeam;

    /******************************************************************/
    // Calculate the stem lengths

    // first check that the stem length is long enough (to be improved?)
    double oldYPos; // holds y position before calculation to determine if beam needs extra height
    double expectedY;
    int verticalAdjustment = 0;
    for (i = 0; i < elementCount; i++) {
        if (m_beamElementCoordRefs.at(i)->m_element->Is(REST)) {
            // Here we need to take into account the bounding box of the rest
            continue;
        }

        oldYPos = m_beamElementCoordRefs.at(i)->m_yBeam;
        expectedY = this->m_startingY + verticalAdjustment
            + this->m_beamSlope * (m_beamElementCoordRefs.at(i)->m_x - this->m_startingX);

        // if the stem is not long enough, add extra stem length needed to all members of the beam
        if ((beamInterface->m_stemDir == STEMDIRECTION_up && (oldYPos > expectedY))
            || (beamInterface->m_stemDir == STEMDIRECTION_down && (oldYPos < expectedY))) {
            verticalAdjustment += oldYPos - expectedY;
        }
    }

    // Now adjust the startingY position and all the elements
    this->m_startingY += verticalAdjustment;
    for (i = 0; i < elementCount; i++) {
        m_beamElementCoordRefs.at(i)->m_yBeam
            = this->m_startingY + this->m_beamSlope * (m_beamElementCoordRefs.at(i)->m_x - this->m_startingX);
    }

    // then check that the stem length reaches the center for the staff
    double minDistToCenter = -VRV_UNSET;

    for (i = 0; i < elementCount; ++i) {
        if ((beamInterface->m_beamPlace == BEAMPLACE_above)
            && (m_beamElementCoordRefs.at(i)->m_yBeam - verticalCenter < minDistToCenter)) {
            minDistToCenter = m_beamElementCoordRefs.at(i)->m_yBeam - verticalCenter;
        }
        else if ((beamInterface->m_beamPlace == BEAMPLACE_below)
            && (verticalCenter - m_beamElementCoordRefs.at(i)->m_yBeam < minDistToCenter)) {
            minDistToCenter = verticalCenter - m_beamElementCoordRefs.at(i)->m_yBeam;
        }
    }

    if (minDistToCenter < 0) {
        this->m_startingY += (beamInterface->m_beamPlace == BEAMPLACE_below) ? minDistToCenter : -minDistToCenter;
        for (i = 0; i < elementCount; ++i) {
            m_beamElementCoordRefs.at(i)->m_yBeam
                += (beamInterface->m_beamPlace == BEAMPLACE_below) ? minDistToCenter : -minDistToCenter;
        }
    }

    for (i = 0; i < elementCount; ++i) {
        // All notes and chords get their stem value stored
        LayerElement *el = m_beamElementCoordRefs.at(i)->m_element;
        if ((el->Is(NOTE)) || (el->Is(CHORD))) {
            StemmedDrawingInterface *stemmedInterface = el->GetStemmedDrawingInterface();
            assert(beamInterface);

            if (beamInterface->m_beamPlace == BEAMPLACE_above) {
                y1 = m_beamElementCoordRefs.at(i)->m_yBeam - doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
                y2 = m_beamElementCoordRefs.at(i)->m_yBottom
                    + stemmedInterface->GetStemUpSE(doc, staff->m_drawingStaffSize, beamInterface->m_cueSize).y;
            }
            else {
                y1 = m_beamElementCoordRefs.at(i)->m_yBeam + doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
                y2 = m_beamElementCoordRefs.at(i)->m_yTop
                    + stemmedInterface->GetStemDownNW(doc, staff->m_drawingStaffSize, beamInterface->m_cueSize).y;
            }

            Stem *stem = stemmedInterface->GetDrawingStem();
            // This is the case with fTrem on whole notes
            if (!stem) continue;

            stem->SetDrawingStemDir(beamInterface->m_stemDir);
            // Since the value were calculated relatively to the element position, adjust them
            stem->SetDrawingXRel(m_beamElementCoordRefs.at(i)->m_x - el->GetDrawingX());
            stem->SetDrawingYRel(y2 - el->GetDrawingY());
            stem->SetDrawingStemLen(y2 - y1);
        }
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

void Beam::AddChild(Object *child)
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
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
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
            LayerElement *element = dynamic_cast<LayerElement *>(*iter);
            assert(element);
            // if we are at the beginning of the beam
            // and the note is cueSize
            // assume all the beam is of grace notes
            if (childList->begin() == iter) {
                if (element->IsGraceNote()) firstNoteGrace = true;
            }
            // if the first note in beam was NOT a grace
            // we have grace notes embedded in a beam
            // drop them
            if (!firstNoteGrace && element->IsGraceNote()) {
                iter = childList->erase(iter);
                continue;
            }
            // also remove notes within chords
            if (element->Is(NOTE)) {
                Note *note = dynamic_cast<Note *>(element);
                assert(note);
                if (note->IsChordTone()) {
                    iter = childList->erase(iter);
                    continue;
                }
            }
            ++iter;
        }
    }
    
    Staff *staff = dynamic_cast<Staff *>(this->GetFirstParent(STAFF));
    assert(staff);
    Staff *beamStaff = staff;
    if (this->HasBeamWith()) {
        Measure *measure = dynamic_cast<Measure *>(this->GetFirstParent(MEASURE));
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
        Note *note = dynamic_cast<Note *>(element);
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

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Beam::CalcStem(FunctorParams *functorParams)
{
    CalcStemParams *params = dynamic_cast<CalcStemParams *>(functorParams);
    assert(params);

    const ArrayOfObjects *beamChildren = this->GetList(this);

    // Should we assert this at the beginning?
    if (beamChildren->empty()) {
        return FUNCTOR_CONTINUE;
    }

    this->m_beamSegment.InitCoordRefs(this->GetElementCoords());

    Layer *layer = dynamic_cast<Layer *>(this->GetFirstParent(LAYER));
    assert(layer);
    Staff *staff = dynamic_cast<Staff *>(layer->GetFirstParent(STAFF));
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
