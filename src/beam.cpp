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
#include "note.h"
#include "rest.h"
#include "smufl.h"
#include "space.h"
#include "staff.h"
#include "tuplet.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// BeamDrawingParams
//----------------------------------------------------------------------------

BeamDrawingParams::BeamDrawingParams()
{
    Reset();
}

void BeamDrawingParams::Reset()
{
    m_changingDur = false;
    m_beamHasChord = false;
    m_hasMultipleStemDir = false;
    m_cueSize = false;
    m_crossStaff = false;
    m_shortestDur = 0;
    m_stemDir = STEMDIRECTION_NONE;

    m_beamWidth = 0;
    m_beamWidthBlack = 0;
    m_beamWidthWhite = 0;
    m_startingX = 0;
    m_startingY = 0;
    m_beamSlope = 0.0;
}

void BeamDrawingParams::CalcBeam(
    Layer *layer, Staff *staff, Doc *doc, const ArrayOfBeamElementCoords *beamElementCoords, int elementCount)
{
    assert(layer);
    assert(staff);
    assert(doc);
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
        (*beamElementCoords).at(i)->m_x = (*beamElementCoords).at(i)->m_element->GetDrawingX();
    }

    // make it relative to the first x / staff y
    int xRel = (*beamElementCoords).at(0)->m_x;
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

        if ((*beamElementCoords).at(i)->m_element->Is(CHORD)) {
            Chord *chord = dynamic_cast<Chord *>((*beamElementCoords).at(i)->m_element);
            assert(chord);
            chord->GetYExtremes(yMax, yMin);
            (*beamElementCoords).at(i)->m_yTop = yMax;
            (*beamElementCoords).at(i)->m_yBottom = yMin;

            avgY += ((yMax + yMin) / 2);

            // highest and lowest value;
            high = std::max(yMax, high);
            low = std::min(yMin, low);
        }
        else if ((*beamElementCoords).at(i)->m_element->Is(NOTE)) {
            (*beamElementCoords).at(i)->m_y = (*beamElementCoords).at(i)->m_element->GetDrawingY();

            // highest and lowest value;
            high = std::max((*beamElementCoords).at(i)->m_y, high);
            low = std::min((*beamElementCoords).at(i)->m_y, low);

            curY = (*beamElementCoords).at(i)->m_element->GetDrawingY();
            (*beamElementCoords).at(i)->m_yTop = curY;
            (*beamElementCoords).at(i)->m_yBottom = curY;
            avgY += (*beamElementCoords).at(i)->m_y;
        }
        else {
            (*beamElementCoords).at(i)->m_y = (*beamElementCoords).at(i)->m_element->GetDrawingY();

            curY = (*beamElementCoords).at(i)->m_element->GetDrawingY();
            (*beamElementCoords).at(i)->m_yTop = curY;
            (*beamElementCoords).at(i)->m_yBottom = curY;
            nbRests++;
        }
    }

    /******************************************************************/
    // Set the stem direction

    // Only if not only rests. (Will produce non-sense output anyway)
    if (elementCount != nbRests) {
        avgY /= (elementCount - nbRests);
    }

    // If we have one stem direction in the beam, then don't look at the layer
    // We probably do not want to call this if we have a cross-staff situation
    if (!this->m_crossStaff && (this->m_stemDir == STEMDIRECTION_NONE)) {
        this->m_stemDir = layer->GetDrawingStemDir(beamElementCoords); // force layer direction if it exists
    }

    // Automatic stem direction if nothing in the notes or in the layer
    if (this->m_stemDir == STEMDIRECTION_NONE) {
        this->m_stemDir = (avgY < verticalCenter) ? STEMDIRECTION_up : STEMDIRECTION_down;
    }

    if (this->m_stemDir == STEMDIRECTION_up) { // set stem direction for all the notes
        for (i = 0; i < elementCount; ++i) {
            (*beamElementCoords).at(i)->m_y = (*beamElementCoords).at(i)->m_yTop;
        }
    }
    else {
        for (i = 0; i < elementCount; ++i) {
            (*beamElementCoords).at(i)->m_y = (*beamElementCoords).at(i)->m_yBottom;
        }
    }

    this->m_beamWidthBlack = doc->GetDrawingBeamWidth(staff->m_drawingStaffSize, this->m_cueSize);
    this->m_beamWidthWhite = doc->GetDrawingBeamWhiteWidth(staff->m_drawingStaffSize, this->m_cueSize);
    this->m_beamWidth = this->m_beamWidthBlack + this->m_beamWidthWhite;

    /******************************************************************/
    // Calculate the slope doing a linear regression

    // The vertical shift depends on the shortestDur value we have in the beam
    verticalShift = ((this->m_shortestDur - DUR_8) * (this->m_beamWidth));

    // if the beam has smaller-size notes
    if ((*beamElementCoords).at(last)->m_element->GetDrawingCueSize()) {
        verticalShift += doc->GetDrawingUnit(staff->m_drawingStaffSize) * 5;
    }
    else {
        verticalShift += (this->m_shortestDur > DUR_8)
            ? doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * verticalShiftFactor
            : doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * (verticalShiftFactor + 0.5);
    }

    // swap x position and verticalShift direction with stem down
    if (this->m_stemDir == STEMDIRECTION_up) {
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
        if ((*beamElementCoords).at(i)->m_element->Is(NOTE)) {
            ((Note *)(*beamElementCoords).at(i)->m_element)->SetDrawingStemDir(this->m_stemDir);
        }
        else if ((*beamElementCoords).at(i)->m_element->Is(CHORD)) {
            ((Chord *)(*beamElementCoords).at(i)->m_element)->SetDrawingStemDir(this->m_stemDir);
        }

        (*beamElementCoords).at(i)->m_yBeam = (*beamElementCoords).at(i)->m_y + verticalShift;
        (*beamElementCoords).at(i)->m_x += stemX[this->m_cueSize];
    }

    for (i = 0; i < elementCount; i++) {
        s_y += (*beamElementCoords).at(i)->m_yBeam - yRel;
        s_y2 += pow((*beamElementCoords).at(i)->m_yBeam - yRel, 2);
        s_x += (*beamElementCoords).at(i)->m_x - xRel;
        s_x2 += pow((*beamElementCoords).at(i)->m_x - xRel, 2);
        s_xy += ((*beamElementCoords).at(i)->m_x - xRel) * ((*beamElementCoords).at(i)->m_yBeam - yRel);
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
    this->m_startingY = (*beamElementCoords).at(0)->m_yBeam;

    /******************************************************************/
    // Calculate the stem lengths

    // first check that the stem length is long enough (to be improved?)
    double oldYPos; // holds y position before calculation to determine if beam needs extra height
    double expectedY;
    int verticalAdjustment = 0;
    for (i = 0; i < elementCount; i++) {
        if ((*beamElementCoords).at(i)->m_element->Is(REST)) {
            // Here we need to take into account the bounding box of the rest
            continue;
        }

        oldYPos = (*beamElementCoords).at(i)->m_yBeam;
        expectedY = this->m_startingY + verticalAdjustment
            + this->m_beamSlope * ((*beamElementCoords).at(i)->m_x - this->m_startingX);

        // if the stem is not long enough, add extra stem length needed to all members of the beam
        if ((this->m_stemDir == STEMDIRECTION_up && (oldYPos > expectedY))
            || (this->m_stemDir == STEMDIRECTION_down && (oldYPos < expectedY))) {
            verticalAdjustment += oldYPos - expectedY;
        }
    }

    // Now adjust the startingY position and all the elements
    this->m_startingY += verticalAdjustment;
    for (i = 0; i < elementCount; i++) {
        (*beamElementCoords).at(i)->m_yBeam
            = this->m_startingY + this->m_beamSlope * ((*beamElementCoords).at(i)->m_x - this->m_startingX);
    }

    // then check that the stem length reaches the center for the staff
    double minDistToCenter = -VRV_UNSET;

    for (i = 0; i < elementCount; ++i) {
        if ((this->m_stemDir == STEMDIRECTION_up)
            && ((*beamElementCoords).at(i)->m_yBeam - verticalCenter < minDistToCenter)) {
            minDistToCenter = (*beamElementCoords).at(i)->m_yBeam - verticalCenter;
        }
        else if ((this->m_stemDir == STEMDIRECTION_down)
            && (verticalCenter - (*beamElementCoords).at(i)->m_yBeam < minDistToCenter)) {
            minDistToCenter = verticalCenter - (*beamElementCoords).at(i)->m_yBeam;
        }
    }

    if (minDistToCenter < 0) {
        this->m_startingY += (this->m_stemDir == STEMDIRECTION_down) ? minDistToCenter : -minDistToCenter;
        for (i = 0; i < elementCount; ++i) {
            (*beamElementCoords).at(i)->m_yBeam
                += (this->m_stemDir == STEMDIRECTION_down) ? minDistToCenter : -minDistToCenter;
        }
    }

    for (i = 0; i < elementCount; ++i) {
        // All notes and chords get their stem value stored
        LayerElement *el = (*beamElementCoords).at(i)->m_element;
        if ((el->Is(NOTE)) || (el->Is(CHORD))) {
            StemmedDrawingInterface *interface = el->GetStemmedDrawingInterface();
            assert(interface);

            if (this->m_stemDir == STEMDIRECTION_up) {
                y1 = (*beamElementCoords).at(i)->m_yBeam - doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
                y2 = (*beamElementCoords).at(i)->m_yBottom
                    + interface->GetStemUpSE(doc, staff->m_drawingStaffSize, this->m_cueSize).y;
            }
            else {
                y1 = (*beamElementCoords).at(i)->m_yBeam + doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
                y2 = (*beamElementCoords).at(i)->m_yTop
                    + interface->GetStemDownNW(doc, staff->m_drawingStaffSize, this->m_cueSize).y;
            }

            Stem *stem = interface->GetDrawingStem();
            // This is the case with fTrem on whole notes
            if (!stem) continue;

            stem->SetDrawingStemDir(this->m_stemDir);
            // Since the value were calculated relatively to the element position, adjust them
            stem->SetDrawingXRel((*beamElementCoords).at(i)->m_x - el->GetDrawingX());
            stem->SetDrawingYRel(y2 - el->GetDrawingY());
            stem->SetDrawingStemLen(y2 - y1);
        }
    }
}

//----------------------------------------------------------------------------
// Beam
//----------------------------------------------------------------------------

Beam::Beam() : LayerElement("beam-"), ObjectListInterface(), AttColor(), AttBeamedWith(), AttBeamRend()
{
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_BEAMEDWITH);
    RegisterAttClass(ATT_BEAMREND);

    Reset();
}

Beam::~Beam()
{
    ClearCoords();
}

void Beam::Reset()
{
    LayerElement::Reset();
    ResetColor();
    ResetBeamedWith();
    ResetBeamRend();

    ClearCoords();
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

    InitCoords(childList);
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

void Beam::InitCoords(ArrayOfObjects *childList)
{
    ClearCoords();

    if (childList->empty()) {
        return;
    }

    m_beamElementCoords.reserve(childList->size());
    int i;
    for (i = 0; i < (int)childList->size(); ++i) {
        m_beamElementCoords.push_back(new BeamElementCoord());
    }

    // current point to the first Note in the layed out layer
    LayerElement *current = dynamic_cast<LayerElement *>(childList->front());
    // Beam list should contain only DurationInterface objects
    assert(current->GetDurationInterface());

    int lastDur = (current->GetDurationInterface())->GetActualDur();

    /******************************************************************/
    // Populate BeamElementCoord for each element in the beam
    // This could be moved to Beam::InitCoord for optimization because there should be no
    // need for redoing it everytime it is drawn.

    data_STEMDIRECTION currentStemDir;
    Layer *layer = NULL;
    Staff *currentStaff = NULL;

    int elementCount = 0;

    ArrayOfObjects::iterator iter = childList->begin();
    do {
        // Beam list should contain only DurationInterface objects
        assert(current->GetDurationInterface());
        int currentDur = (current->GetDurationInterface())->GetActualDur();

        if (current->Is(CHORD)) {
            m_drawingParams.m_beamHasChord = true;
        }

        m_beamElementCoords.at(elementCount)->m_element = current;
        current->m_beamElementCoord = m_beamElementCoords.at(elementCount);
        m_beamElementCoords.at(elementCount)->m_dur = currentDur;

        // Look at beam breaks
        m_beamElementCoords.at(elementCount)->m_breaksec = 0;
        AttBeamSecondary *beamsecondary = dynamic_cast<AttBeamSecondary *>(current);
        if (beamsecondary && beamsecondary->HasBreaksec()) {
            if (!m_drawingParams.m_changingDur) m_drawingParams.m_changingDur = true;
            m_beamElementCoords.at(elementCount)->m_breaksec = beamsecondary->GetBreaksec();
        }

        Staff *staff = current->GetCrossStaff(layer);
        if (staff != currentStaff) {
            m_drawingParams.m_crossStaff = true;
        }
        currentStaff = staff;

        // Skip rests
        if (current->Is({ NOTE, CHORD })) {
            // look at the stemDir to see if we have multiple stem Dir
            if (!m_drawingParams.m_hasMultipleStemDir) {
                StemmedDrawingInterface *interface = current->GetStemmedDrawingInterface();
                assert(interface);
                Stem *stem = interface->GetDrawingStem();
                currentStemDir = STEMDIRECTION_NONE;
                if (stem) {
                    assert(dynamic_cast<AttStems *>(stem));
                    currentStemDir = (dynamic_cast<AttStems *>(stem))->GetStemDir();
                }
                if (currentStemDir != STEMDIRECTION_NONE) {
                    if ((m_drawingParams.m_stemDir != STEMDIRECTION_NONE)
                        && (m_drawingParams.m_stemDir != currentStemDir)) {
                        m_drawingParams.m_hasMultipleStemDir = true;
                    }
                    m_drawingParams.m_stemDir = currentStemDir;
                }
            }
            // keep the shortest dur in the beam
            m_drawingParams.m_shortestDur = std::max(currentDur, m_drawingParams.m_shortestDur);
        }
        // check if we have more than duration in the beam
        if (!m_drawingParams.m_changingDur && currentDur != lastDur) m_drawingParams.m_changingDur = true;
        lastDur = currentDur;

        elementCount++;

        ++iter;
        if (iter == childList->end()) {
            break;
        }
        current = dynamic_cast<LayerElement *>(*iter);
        if (current == NULL) {
            LogDebug("Error accessing element in Beam list");
            return;
        }

    } while (1);

    // elementCount must be greater than 0 here
    if (elementCount == 0) {
        LogDebug("Beam with no notes of duration > 8 detected. Exiting DrawBeam.");
        return;
    }

    int last = elementCount - 1;

    // We look only at the last note for checking if cue-sized. Somehow arbitrarily
    m_drawingParams.m_cueSize = m_beamElementCoords.at(last)->m_element->GetDrawingCueSize();

    // Always set stem diretion to up for grace note beam unless stem direction is provided
    if (this->m_drawingParams.m_cueSize && (this->m_drawingParams.m_stemDir == STEMDIRECTION_NONE)) {
        this->m_drawingParams.m_stemDir = STEMDIRECTION_up;
    }
}

void Beam::ClearCoords()
{
    ArrayOfBeamElementCoords::iterator iter;
    for (iter = m_beamElementCoords.begin(); iter != m_beamElementCoords.end(); ++iter) {
        delete *iter;
    }
    m_beamElementCoords.clear();
}

//----------------------------------------------------------------------------
// BeamElementCoord
//----------------------------------------------------------------------------

BeamElementCoord::~BeamElementCoord()
{
    if (m_element) m_element->m_beamElementCoord = NULL;
}

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
    const ArrayOfBeamElementCoords *beamElementCoords = this->GetElementCoords();

    int elementCount = (int)beamChildren->size();

    Layer *layer = dynamic_cast<Layer *>(this->GetFirstAncestor(LAYER));
    assert(layer);
    Staff *staff = dynamic_cast<Staff *>(layer->GetFirstAncestor(STAFF));
    assert(staff);

    this->m_drawingParams.CalcBeam(layer, staff, params->m_doc, beamElementCoords, elementCount);

    return FUNCTOR_CONTINUE;
}

int Beam::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);

    this->m_drawingParams.Reset();

    // We want the list of the ObjectListInterface to be re-generated
    this->Modify();

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
