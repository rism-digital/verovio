/////////////////////////////////////////////////////////////////////////////
// Name:        beam.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "beam.h"

//----------------------------------------------------------------------------

#include <array>
#include <cassert>
#include <math.h>

//----------------------------------------------------------------------------

#include "artic.h"
#include "btrem.h"
#include "doc.h"
#include "editorial.h"
#include "ftrem.h"
#include "functor.h"
#include "gracegrp.h"
#include "layer.h"
#include "measure.h"
#include "note.h"
#include "rest.h"
#include "smufl.h"
#include "space.h"
#include "staff.h"
#include "stem.h"
#include "tabdursym.h"
#include "tabgrp.h"
#include "tuplet.h"
#include "verticalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// BeamSegment
//----------------------------------------------------------------------------

BeamSegment::BeamSegment()
{
    this->Reset();
}

BeamSegment::~BeamSegment()
{
    this->ClearCoordRefs();
}

void BeamSegment::Reset()
{
    this->ClearCoordRefs();

    m_beamSlope = 0.0;
    m_verticalCenter = 0;
    m_ledgerLinesAbove = 0;
    m_ledgerLinesBelow = 0;
    m_uniformStemLength = 0;
    m_weightedPlace = BEAMPLACE_NONE;

    m_firstNoteOrChord = NULL;
    m_lastNoteOrChord = NULL;

    m_stemSameasRole = SAMEAS_NONE;
    m_stemSameasReverseRole = NULL;
}

const ArrayOfBeamElementCoords *BeamSegment::GetElementCoordRefs()
{
    // this->GetList();
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

void BeamSegment::CalcBeam(const Layer *layer, Staff *staff, const Doc *doc, BeamDrawingInterface *beamInterface,
    data_BEAMPLACE place, bool init)
{
    assert(layer);
    assert(staff);
    assert(doc);

    assert(m_beamElementCoordRefs.size() > 0);

    // For recursive calls, avoid to re-init values
    if (init) {
        this->CalcBeamInit(staff, doc, beamInterface, place);
    }

    bool horizontal = true;
    if (staff->IsTablature()) {
        int glyphSize = staff->GetDrawingStaffNotationSize();
        beamInterface->m_fractionSize = glyphSize * 2 / 3;

        // Always horizontal when outside the staff and not when inside the staff
        // Eventually we will need to look at the content to pre-determine if horizontal for inside beams
        horizontal = staff->IsTabWithStemsOutside();
        this->CalcBeamPlaceTab(layer, staff, doc, beamInterface, place);
    }
    else {
        beamInterface->m_fractionSize = staff->m_drawingStaffSize;

        if (doc->GetOptions()->m_beamMaxSlope.GetValue()) horizontal = beamInterface->IsHorizontal();
        // Beam@place has precedence - however, in some cases, CalcBeam is called recursively because we need to change
        // the place This occurs when mixed makes no sense and the beam is placed above or below instead.
        this->CalcBeamPlace(layer, beamInterface, place);
    }

    if (BEAMPLACE_mixed == beamInterface->m_drawingPlace) {
        this->CalcMixedBeamPlace(staff);
        this->CalcPartialFlagPlace();
    }
    this->CalcBeamStemLength(staff, beamInterface->m_drawingPlace, horizontal);

    // Set drawing stem positions
    this->CalcBeamPosition(doc, staff, beamInterface, horizontal);
    if (BEAMPLACE_mixed == beamInterface->m_drawingPlace) {
        if (NeedToResetPosition(staff, doc, beamInterface)) {
            this->CalcBeamInit(staff, doc, beamInterface, place);
            this->CalcBeamStemLength(staff, beamInterface->m_drawingPlace, horizontal);
            this->CalcBeamPosition(doc, staff, beamInterface, horizontal);
        }
    }

    /******************************************************************/
    // Set the stem lengths to stem objects

    if (staff->IsTablature()) {
        this->CalcSetStemValuesTab(staff, doc, beamInterface);
    }
    else {
        this->CalcSetStemValues(staff, doc, beamInterface);
    }
}

void BeamSegment::CalcSetStemValues(const Staff *staff, const Doc *doc, const BeamDrawingInterface *beamInterface)
{
    assert(staff);
    assert(doc);
    assert(beamInterface);

    const int stemWidth = doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
    for (BeamElementCoord *coord : m_beamElementCoordRefs) {
        // All notes and chords get their stem value stored
        LayerElement *el = coord->m_element;
        if (!el->Is({ CHORD, NOTE })) continue;

        // Get the interface for the chord or note
        StemmedDrawingInterface *stemmedInterface = coord->GetStemHolderInterface();
        if (!stemmedInterface) continue;

        assert(coord->m_closestNote);

        int y1 = coord->m_yBeam;
        int y2 = coord->m_closestNote->GetDrawingY();
        bool isStemSameas = false;

        // With stem.sameas the y is not the beam one but the one of the other note
        // We also need to adjust the length differently (below)
        if (this->StemSameasIsSecondary() && el->Is(NOTE)) {
            Note *note = vrv_cast<Note *>(el);
            assert(note);
            if (note->HasStemSameasNote()) {
                y1 = note->GetStemSameasNote()->GetDrawingY();
                isStemSameas = true;
            }
        }

        int stemAdjust = 0;
        if (beamInterface->m_drawingPlace == BEAMPLACE_above) {
            if (isStemSameas) {
                // Move up according to the cut-outs
                y1 += stemmedInterface->GetStemUpSE(doc, staff->m_drawingStaffSize, beamInterface->m_cueSize).y;
            }
            else {
                // Set adjust to ensure that drawn stem is slightly shorter than the top-beam
                stemAdjust = -stemWidth;
            }
            y2 += stemmedInterface->GetStemUpSE(doc, staff->m_drawingStaffSize, beamInterface->m_cueSize).y;
        }
        else if (beamInterface->m_drawingPlace == BEAMPLACE_below) {
            if (isStemSameas) {
                y1 += stemmedInterface->GetStemDownNW(doc, staff->m_drawingStaffSize, beamInterface->m_cueSize).y;
            }
            else {
                stemAdjust = stemWidth;
            }
            y2 += stemmedInterface->GetStemDownNW(doc, staff->m_drawingStaffSize, beamInterface->m_cueSize).y;
        }
        else if (beamInterface->m_drawingPlace == BEAMPLACE_mixed) {
            int stemOffset = 0;
            const int unit = doc->GetDrawingUnit(staff->m_drawingStaffSize);
            if (coord->m_partialFlagPlace == coord->m_beamRelativePlace) {
                stemOffset = (coord->m_dur - DURATION_8) * beamInterface->m_beamWidth;
            }
            else if (el->GetIsInBeamSpan() && (coord->m_partialFlagPlace != BEAMPLACE_above)
                && (coord->m_stem->GetDrawingStemDir() == STEMDIRECTION_up)) {
                stemOffset = -unit / 2;
            }
            // handle cross-staff fTrem cases
            const auto [beams, beamsFloat] = beamInterface->GetFloatingBeamCount();
            if ((coord->m_stem && coord->m_stem->GetDrawingStemDir() == STEMDIRECTION_down)
                && ((beams > 0) || (beamsFloat > 0))) {
                int beamsCount = std::max(beams, beamsFloat);
                if (beamsFloat <= 0) beamsCount--;
                stemOffset = beamsCount * beamInterface->m_beamWidth;
            }

            if (coord->m_beamRelativePlace == BEAMPLACE_below) {
                y2 += stemmedInterface->GetStemDownNW(doc, staff->m_drawingStaffSize, beamInterface->m_cueSize).y;
                stemAdjust = -(beamInterface->m_beamWidthBlack + stemOffset);
            }
            else {
                y2 += stemmedInterface->GetStemUpSE(doc, staff->m_drawingStaffSize, beamInterface->m_cueSize).y;
                stemAdjust = stemOffset;
            }
        }

        if (coord->m_element->Is(CHORD)) {
            Chord *chord = vrv_cast<Chord *>(coord->m_element);
            assert(chord);
            int yMax, yMin;
            chord->GetYExtremes(yMax, yMin);
            if (beamInterface->m_drawingPlace == BEAMPLACE_mixed) {
                y2 += (coord->m_beamRelativePlace == BEAMPLACE_above) ? (yMin - yMax) : (yMax - yMin);
            }
            else {
                y2 += (beamInterface->m_drawingPlace == BEAMPLACE_above) ? (yMin - yMax) : (yMax - yMin);
            }
        }

        coord->UpdateStemLength(
            stemmedInterface, y1, y2, stemAdjust, (beamInterface->m_drawingPlace == BEAMPLACE_mixed));
    }

    if (doc->GetOptions()->m_beamFrenchStyle.GetValue() && (m_beamElementCoordRefs.size() > 2)) {
        this->AdjustBeamToFrenchStyle(beamInterface);
    }

    this->AdjustBeamToTremolos(doc, staff, beamInterface);
}

void BeamSegment::CalcSetStemValuesTab(const Staff *staff, const Doc *doc, const BeamDrawingInterface *beamInterface)
{
    assert(staff);
    assert(doc);
    assert(beamInterface);

    int y1, y2;

    for (BeamElementCoord *coord : m_beamElementCoordRefs) {
        // All notes and chords get their stem value stored
        LayerElement *el = coord->m_element;
        if (el->Is(TABGRP)) {
            // Just in case
            if (!coord->m_closestNote && !coord->m_tabDurSym) continue;

            // Get the interface from child tabDurSym
            StemmedDrawingInterface *stemmedInterface = coord->GetStemHolderInterface();
            if (!stemmedInterface) continue;

            y1 = coord->m_yBeam;

            y2 = (coord->m_closestNote) ? coord->m_closestNote->GetDrawingY() : coord->m_tabDurSym->GetDrawingY();

            if (beamInterface->m_drawingPlace == BEAMPLACE_above) {
                // Move down to ensure the stem is slightly shorter than the top-beam
                y1 -= doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
                if (coord->m_closestNote) y2 += doc->GetDrawingUnit(staff->m_drawingStaffSize);
            }
            else {
                y1 += doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
                if (coord->m_closestNote) y2 -= doc->GetDrawingUnit(staff->m_drawingStaffSize);
            }

            Stem *stem = stemmedInterface->GetDrawingStem();
            // This is the case with fTrem on whole notes
            if (!stem) continue;

            // stem->SetDrawingStemDir(stemDir);
            stem->SetDrawingXRel(coord->m_x - el->GetDrawingX());
            if (coord->m_closestNote) {
                stem->SetDrawingYRel(y2 - el->GetDrawingY());
            }
            stem->SetDrawingStemLen(y2 - y1);
        }
    }
}

bool BeamSegment::DoesBeamOverlap(
    const BeamDrawingInterface *beamInterface, int topBorder, int bottomBorder, int minStemLength) const
{
    // Check if beam fits within the bounds
    const bool outsideBounds = std::any_of(m_beamElementCoordRefs.begin(), m_beamElementCoordRefs.end(),
        [&](BeamElementCoord *coord) { return ((coord->m_yBeam > topBorder) || (coord->m_yBeam < bottomBorder)); });

    if (outsideBounds) return true;

    // Check for minimal stem length
    const auto [minLengthAbove, minLengthBelow] = this->GetMinimalStemLength(beamInterface);
    return (std::min(minLengthAbove, minLengthBelow) < minStemLength);
}

std::pair<int, int> BeamSegment::GetVerticalOffset(const BeamDrawingInterface *beamInterface) const
{
    const auto [topBeams, bottomBeams] = beamInterface->GetAdditionalBeamCount();
    const int topOffset = topBeams * beamInterface->m_beamWidth;
    const int bottomOffset = bottomBeams * beamInterface->m_beamWidth;
    return { topOffset, bottomOffset };
}

std::pair<int, int> BeamSegment::GetMinimalStemLength(const BeamDrawingInterface *beamInterface) const
{
    assert(beamInterface);

    int minLengthAbove = VRV_UNSET;
    int minLengthBelow = VRV_UNSET;
    int currentLength = VRV_UNSET;

    const auto [topOffset, bottomOffset] = this->GetVerticalOffset(beamInterface);

    // lambda check whether coord has element set and whether that element is CHORD or NOTE
    const auto isNoteOrChord
        = [](BeamElementCoord *coord) { return (coord->m_element && coord->m_element->Is({ CHORD, NOTE })); };

    for (BeamElementCoord *coord : m_beamElementCoordRefs) {
        if (!isNoteOrChord(coord)) continue;

        // Get the stem direction
        const StemmedDrawingInterface *stemmedInterface = coord->GetStemHolderInterface();
        if (!stemmedInterface) continue;
        const Stem *stem = stemmedInterface->GetDrawingStem();
        const bool isStemUp = (stem->GetDrawingStemDir() == STEMDIRECTION_up);

        if (isStemUp) {
            currentLength = coord->m_yBeam - bottomOffset - coord->m_closestNote->GetDrawingY();
        }
        else {
            currentLength = coord->m_closestNote->GetDrawingY() - coord->m_yBeam - topOffset;
        }

        // Update the min length
        int &minLength = isStemUp ? minLengthBelow : minLengthAbove;
        if (minLength == VRV_UNSET) {
            minLength = currentLength;
        }
        else {
            minLength = std::min(minLength, currentLength);
        }
    }
    return { minLengthAbove, minLengthBelow };
}

bool BeamSegment::NeedToResetPosition(Staff *staff, const Doc *doc, BeamDrawingInterface *beamInterface)
{
    // CASE 1: CROSS STAFF BEAMS
    // Those are easy, since they request staff space (See BeamSegment::RequestStaffSpace(...))
    if (beamInterface->m_crossStaffContent) {
        data_BEAMPLACE place = m_beamElementCoordRefs.at(0)->m_beamRelativePlace;
        auto iter = std::find_if(m_beamElementCoordRefs.begin(), m_beamElementCoordRefs.end(),
            [&place](BeamElementCoord *coord) { return coord->m_beamRelativePlace != place; });
        if (iter == m_beamElementCoordRefs.end()) {
            beamInterface->m_drawingPlace = place;
            return true;
        }
        return false;
    }

    // CASE 2: SINGLE STAFF BEAMS
    if (doc->GetOptions()->m_beamMixedPreserve.GetValue()) {
        return false;
    }

    const int unit = doc->GetDrawingUnit(staff->m_drawingStaffSize);
    const int minStemLength = doc->GetOptions()->m_beamMixedStemMin.GetValue() * unit;
    auto [topOffset, bottomOffset] = this->GetVerticalOffset(beamInterface);

    // find top and bottom of the staff
    const int staffTop = staff->GetDrawingY();
    const int staffBottom
        = staffTop - doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * (staff->m_drawingLines - 1);
    const int topBorder = staffTop - topOffset - unit;
    const int bottomBorder = staffBottom + bottomOffset + unit;

    // Check if the beam is admissible
    if (!this->DoesBeamOverlap(beamInterface, topBorder, bottomBorder, minStemLength)) {
        return false;
    }

    // Calculate midpoint for the beam with mixed placement
    int min = m_beamElementCoordRefs.at(0)->m_element->GetDrawingY();
    int max = m_beamElementCoordRefs.at(0)->m_element->GetDrawingY();
    for (BeamElementCoord *coord : m_beamElementCoordRefs) {
        max = std::max(max, coord->m_element->GetDrawingY());
        min = std::min(min, coord->m_element->GetDrawingY());
    }
    const int midpoint = (max + min) / 2;
    bool isMidpointWithinBounds = (midpoint < topBorder) && (midpoint > bottomBorder);

    // If midpoint fits within bounds of the staff, try to place beam there
    if (isMidpointWithinBounds) {
        const int midpointOffset
            = (m_beamElementCoordRefs.front()->m_yBeam + m_beamElementCoordRefs.back()->m_yBeam - 2 * midpoint) / 2;
        std::for_each(m_beamElementCoordRefs.begin(), m_beamElementCoordRefs.end(),
            [midpointOffset](BeamElementCoord *coord) { coord->m_yBeam -= midpointOffset; });
        if (!this->DoesBeamOverlap(beamInterface, topBorder, bottomBorder, minStemLength)) return false;
    }
    // If midpoint is above the staff, try to place beam at the top edge of the staff
    if (!isMidpointWithinBounds && (midpoint > staffBottom)) {
        const int offset
            = (m_beamElementCoordRefs.front()->m_yBeam + m_beamElementCoordRefs.back()->m_yBeam - 2 * topBorder) / 2;
        std::for_each(m_beamElementCoordRefs.begin(), m_beamElementCoordRefs.end(),
            [offset](BeamElementCoord *coord) { coord->m_yBeam -= offset; });
    }
    // otherwise try placing it on the bottom edge
    else if (!isMidpointWithinBounds && (midpoint < staffTop)) {
        const int offset
            = (m_beamElementCoordRefs.front()->m_yBeam + m_beamElementCoordRefs.back()->m_yBeam - 2 * bottomBorder) / 2;
        std::for_each(m_beamElementCoordRefs.begin(), m_beamElementCoordRefs.end(),
            [offset](BeamElementCoord *coord) { coord->m_yBeam -= offset; });
    }
    if (!this->DoesBeamOverlap(beamInterface, topBorder, bottomBorder, minStemLength)) return false;

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
        m_beamElementCoordRefs.at(0)->m_element->GetID().c_str(),
        (beamInterface->m_drawingPlace == BEAMPLACE_above) ? "above" : "below");

    return true;
}

void BeamSegment::AdjustBeamToFrenchStyle(const BeamDrawingInterface *beamInterface)
{
    assert(beamInterface);

    // set to store durations of relevant notes (it's ordered, so min duration is going to be first)
    std::set<data_DURATION> noteDurations;
    // lambda check whether coord has element set and whether that element is CHORD or NOTE
    const auto isNoteOrChord
        = [](BeamElementCoord *coord) { return (coord->m_element && coord->m_element->Is({ CHORD, NOTE })); };
    // iterators
    using CoordIt = ArrayOfBeamElementCoords::iterator;
    using CoordReverseIt = ArrayOfBeamElementCoords::reverse_iterator;
    for (CoordIt it = std::next(m_beamElementCoordRefs.begin()); it != std::prev(m_beamElementCoordRefs.end()); ++it) {
        // clear values
        noteDurations.clear();
        if (!isNoteOrChord(*it)) continue;

        // get current element duration
        const data_DURATION val = (*it)->m_breaksec
            ? vrv::DurationMin(static_cast<data_DURATION>((*it)->m_breaksec + DURATION_4), (*it)->m_dur)
            : (*it)->m_dur;
        noteDurations.insert(val);
        // get next element duration
        CoordIt nextElement = std::find_if(it + 1, m_beamElementCoordRefs.end(), isNoteOrChord);
        if (nextElement != m_beamElementCoordRefs.end()) {
            noteDurations.insert((*nextElement)->m_dur);
        }
        // get previous element duration
        CoordReverseIt reverse = std::make_reverse_iterator(it);
        CoordReverseIt prevElement = std::find_if(reverse, m_beamElementCoordRefs.rend(), isNoteOrChord);
        if (prevElement != m_beamElementCoordRefs.rend()) {
            const data_DURATION prevVal = (*prevElement)->m_breaksec
                ? vrv::DurationMin(
                      static_cast<data_DURATION>((*prevElement)->m_breaksec + DURATION_4), (*prevElement)->m_dur)
                : (*prevElement)->m_dur;
            noteDurations.insert(prevVal);
        }

        // Get min duration of elements around
        const int minDur = *noteDurations.begin();
        if (minDur == DURATION_8) continue;

        // Get stem and adjust it's length
        StemmedDrawingInterface *stemmedInterface = (*it)->GetStemHolderInterface();
        if (!stemmedInterface) continue;
        Stem *stem = stemmedInterface->GetDrawingStem();

        const int sign = beamInterface->m_drawingPlace == BEAMPLACE_mixed
            ? ((*it)->m_beamRelativePlace == BEAMPLACE_below ? -1 : 1)
            : (beamInterface->m_drawingPlace == BEAMPLACE_below ? -1 : 1);
        const int lengthAdjust = sign * (minDur - DURATION_8) * beamInterface->m_beamWidth;
        stem->SetDrawingStemAdjust(stem->GetDrawingStemAdjust() + lengthAdjust);
    }
}

void BeamSegment::AdjustBeamToLedgerLines(
    const Doc *doc, const Staff *staff, const BeamDrawingInterface *beamInterface, bool isHorizontal)
{
    int adjust = 0;
    const int staffTop = staff->GetDrawingY();
    const int staffHeight = doc->GetDrawingStaffSize(staff->m_drawingStaffSize);
    const int doubleUnit = doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
    const int staffMargin = isHorizontal ? doubleUnit / 2 : 0;
    for (BeamElementCoord *coord : m_beamElementCoordRefs) {
        if (beamInterface->m_drawingPlace == BEAMPLACE_below) {
            const int topPosition = coord->m_yBeam + beamInterface->GetTotalBeamWidth();
            if (topPosition > staffTop - staffMargin) {
                adjust = ((topPosition - staffTop) / doubleUnit + 1) * doubleUnit;
                break;
            }
        }
        else if (beamInterface->m_drawingPlace == BEAMPLACE_above) {
            const int bottomPosition = coord->m_yBeam - beamInterface->GetTotalBeamWidth();
            const int bottomMargin = staffTop - staffHeight;
            if (bottomPosition < bottomMargin + staffMargin) {
                adjust = ((bottomPosition - bottomMargin) / doubleUnit - 1) * doubleUnit;
                break;
            }
        }
    }
    // make sure there is at least one staff space between beams and staff ends (i.e. ledger lines)
    if (adjust) {
        std::for_each(m_beamElementCoordRefs.begin(), m_beamElementCoordRefs.end(),
            [adjust](BeamElementCoord *coord) { coord->m_yBeam -= adjust; });
    }
}

void BeamSegment::AdjustBeamToTremolos(const Doc *doc, const Staff *staff, const BeamDrawingInterface *beamInterface)
{
    // iterate over all coords and find maximum required adjustment for stems and beam; additional beams should be taken
    // into account to make sure that correct value is calculated
    int maxAdjustment = 0;
    for (BeamElementCoord *coord : m_beamElementCoordRefs) {
        // Get the interface for the chord or note
        StemmedDrawingInterface *stemmedInterface = coord->GetStemHolderInterface();
        if (!stemmedInterface) continue;

        Stem *stem = stemmedInterface->GetDrawingStem();
        const int offset = (coord->m_dur - DURATION_8) * beamInterface->m_beamWidth + beamInterface->m_beamWidthBlack;
        const int currentAdjustment = stem->CalculateStemModAdjustment(doc, staff, offset);
        if (std::abs(currentAdjustment) > std::abs(maxAdjustment)) maxAdjustment = currentAdjustment;
    }
    // exit in case no adjustment is required
    if (maxAdjustment == 0) return;
    // otherwise apply adjustment to all stems within beam and all beam positions
    for (BeamElementCoord *coord : m_beamElementCoordRefs) {
        coord->m_yBeam -= maxAdjustment;

        // Get the interface for the chord or note
        StemmedDrawingInterface *stemmedInterface = coord->GetStemHolderInterface();
        if (!stemmedInterface) continue;

        Stem *stem = stemmedInterface->GetDrawingStem();
        stem->SetDrawingStemLen(stem->GetDrawingStemLen() + maxAdjustment);
    }
}

void BeamSegment::CalcBeamInit(
    const Staff *staff, const Doc *doc, BeamDrawingInterface *beamInterface, data_BEAMPLACE place)
{
    assert(staff);
    assert(doc);
    assert(beamInterface);

    int elementCount = (int)m_beamElementCoordRefs.size();
    assert(elementCount > 0);

    /******************************************************************/
    // initialization

    for (int i = 0; i < elementCount; ++i) {
        BeamElementCoord *coord = m_beamElementCoordRefs.at(i);
        coord->m_x = coord->m_element->GetDrawingX();
    }

    m_verticalCenter = staff->GetDrawingY()
        - (doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * 2); // center point of the staff

    beamInterface->m_beamWidthBlack = doc->GetDrawingBeamWidth(staff->m_drawingStaffSize, beamInterface->m_cueSize);
    beamInterface->m_beamWidthWhite
        = doc->GetDrawingBeamWhiteWidth(staff->m_drawingStaffSize, beamInterface->m_cueSize);
    if (beamInterface->m_shortestDur == DURATION_64) {
        beamInterface->m_beamWidthWhite *= 4;
        beamInterface->m_beamWidthWhite /= 3;
    }

    if (staff->IsTablature()) {
        // Adjust the height and spacing of the beams
        beamInterface->m_beamWidthBlack /= 2;
        beamInterface->m_beamWidthWhite /= 2;

        // Adjust it further for tab.lute.french, tab.lute.german and tab.lute.italian
        if (staff->IsTabLuteFrench() || staff->IsTabLuteGerman() || staff->IsTabLuteItalian()) {
            beamInterface->m_beamWidthBlack = beamInterface->m_beamWidthBlack * 2 / 5;
            beamInterface->m_beamWidthWhite = beamInterface->m_beamWidthWhite * 3 / 5;
        }
    }

    beamInterface->m_beamWidth = beamInterface->m_beamWidthBlack + beamInterface->m_beamWidthWhite;

    /******************************************************************/
    // Calculate the extreme values

    m_nbNotesOrChords = 0;
    m_ledgerLinesAbove = 0;
    m_ledgerLinesBelow = 0;

    // somebody might want to have a beam with only rest of space elements...
    m_firstNoteOrChord = NULL;
    m_lastNoteOrChord = NULL;

    int yMax = m_verticalCenter;
    int yMin = m_verticalCenter;
    auto SetExtrema = [&yMax, &yMin](int currentY) {
        yMax = std::max(currentY, yMax);
        yMin = std::min(currentY, yMin);
    };

    // elementCount holds the last one
    for (int i = 0; i < elementCount; ++i) {
        BeamElementCoord *coord = m_beamElementCoordRefs.at(i);
        coord->m_yBeam = 0;

        if (coord->m_element->Is({ CHORD, NOTE, TABGRP })) {
            if (!m_firstNoteOrChord) m_firstNoteOrChord = coord;
            m_lastNoteOrChord = coord;
            m_nbNotesOrChords++;
        }

        int chordYMax = 0;
        int chordYMin = 0;

        if (coord->m_element->Is(CHORD)) {
            Chord *chord = vrv_cast<Chord *>(coord->m_element);
            assert(chord);
            Note *bottomNote = chord->GetBottomNote();
            assert(bottomNote);
            Note *topNote = chord->GetTopNote();
            assert(topNote);
            this->CalcBeamInitForNotePair(bottomNote, topNote, staff, chordYMax, chordYMin);
            SetExtrema(chordYMax);
            SetExtrema(chordYMin);
        }
        else if (coord->m_element->Is(NOTE)) {
            Note *note = vrv_cast<Note *>(coord->m_element);
            assert(note);
            // In a stem.sameas context, use both notes to determine the beam place (e,g, same as a chord)
            if (note->HasStemSameasNote()) {
                this->CalcBeamInitForNotePair(note, note->GetStemSameasNote(), staff, chordYMax, chordYMin);
                SetExtrema(chordYMax);
                SetExtrema(chordYMin);
            }
            else {
                SetExtrema(note->GetDrawingY());
                int linesAbove = 0;
                int linesBelow = 0;
                if (note->HasLedgerLines(linesAbove, linesBelow, staff)) {
                    m_ledgerLinesBelow += linesBelow;
                    m_ledgerLinesAbove += linesAbove;
                }
            }
        }
    }

    m_weightedPlace = ((m_verticalCenter - yMin) > (yMax - m_verticalCenter)) ? BEAMPLACE_above : BEAMPLACE_below;
}

void BeamSegment::CalcBeamInitForNotePair(
    const Note *note1, const Note *note2, const Staff *staff, int &yMax, int &yMin)
{
    assert(note1);
    assert(note2);

    const Note *bottomNote = (note1->GetDrawingY() > note2->GetDrawingY()) ? note2 : note1;
    const Note *topNote = (note1->GetDrawingY() > note2->GetDrawingY()) ? note1 : note2;

    yMax = bottomNote->GetDrawingY();
    yMin = topNote->GetDrawingY();

    int linesAbove = 0;
    int linesBelow = 0;
    if (bottomNote->HasLedgerLines(linesAbove, linesBelow, staff)) {
        m_ledgerLinesBelow += linesBelow;
    }
    if (topNote->HasLedgerLines(linesAbove, linesBelow, staff)) {
        m_ledgerLinesAbove += linesAbove;
    }
}

bool BeamSegment::CalcBeamSlope(const Staff *staff, const Doc *doc, BeamDrawingInterface *beamInterface, int &step)
{
    assert(staff);
    assert(doc);
    assert(beamInterface);

    m_beamSlope = 0.0;

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

    // This can happen with two notes with 32sd or 64th notes and a diatonic step
    // Force the noteSlope to be considered instead
    if (m_beamSlope == 0.0) m_beamSlope = noteSlope;

    if (m_beamSlope == 0.0) return false;

    const int unit = doc->GetDrawingUnit(staff->m_drawingStaffSize);
    // Indicates if we have a short step of half a unit
    // This occurs with 8th and 16th only and with a reduced distance of 3 stave-spaces (6 units)
    bool shortStep = false;
    step = this->CalcBeamSlopeStep(doc, staff, beamInterface, noteStep, shortStep);

    // Short accessors
    const data_BEAMPLACE place = beamInterface->m_drawingPlace;
    // The current step according to stems - this can be flat because of the stem extended
    const int curStep = abs(m_firstNoteOrChord->m_yBeam - m_lastNoteOrChord->m_yBeam);
    // We can keep the current slope but only if curStep is not 0 and smaller than the step
    if ((curStep != 0) && (curStep < step) && (BEAMPLACE_mixed != place)) {
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
        if ((step <= unit) || (step > unit * 2)) {
            step = unit * 2;
        }
        this->CalcMixedBeamPosition(beamInterface, step, unit);
    }

    m_beamSlope = BoundingBox::CalcSlope(Point(m_firstNoteOrChord->m_x, m_firstNoteOrChord->m_yBeam),
        Point(m_lastNoteOrChord->m_x, m_lastNoteOrChord->m_yBeam));

    if (m_nbNotesOrChords == 2) {
        // Also, we never have to adjust the slope with two notes
        return false;
    }

    return true;
}

int BeamSegment::CalcBeamSlopeStep(
    const Doc *doc, const Staff *staff, BeamDrawingInterface *beamInterface, int noteStep, bool &shortStep)
{
    const int unit = doc->GetDrawingUnit(staff->m_drawingStaffSize);
    // Default (maximum) step is two stave-spaces (4 units)
    int step = 4 * unit;
    // The distance between the two extremes
    const int dist = m_lastNoteOrChord->m_x - m_firstNoteOrChord->m_x;

    if (m_nbNotesOrChords == 2) {
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
        if (noteStep < unit * 3) {
            step = unit / 2;
            shortStep = true;
        }
        // A fifth or smaller - reduce the step
        else if (noteStep <= unit * 4) {
            step = unit * 2;
        }
        else if (m_nbNotesOrChords == 4) {
            if ((m_beamElementCoordRefs.at(1)->m_yBeam == m_beamElementCoordRefs.at(2)->m_yBeam)
                && ((m_firstNoteOrChord->m_yBeam == m_beamElementCoordRefs.at(1)->m_yBeam)
                    || (m_lastNoteOrChord->m_yBeam == m_beamElementCoordRefs.at(2)->m_yBeam))) {
                step = unit * 2;
            }
        }
    }

    // duration
    const data_DURATION dur = beamInterface->m_shortestDur;
    // Prevent short step with values not shorter than a 16th
    if (shortStep && (dur >= DURATION_32)) {
        step = unit * 2;
        shortStep = false;
    }

    return step;
}

void BeamSegment::CalcMixedBeamPosition(const BeamDrawingInterface *beamInterface, int step, int unit)
{
    const auto [topOffset, bottomOffset] = this->GetVerticalOffset(beamInterface);

    // Calculate midpoint for the beam, taking into account highest and lowest points, as well as number of additional
    // beams above and below main beam. Start position of the beam is then further adjusted based on the step size to
    // make sure that beam is truly centered
    int centerY = this->CalcMixedBeamCenterY(step, unit);
    centerY += (beamInterface->m_beamWidthBlack + bottomOffset - topOffset) / 2;
    const bool isSlopeUp = (m_firstNoteOrChord->m_beamRelativePlace == m_lastNoteOrChord->m_beamRelativePlace)
        ? (m_beamSlope > 0)
        : (m_lastNoteOrChord->m_beamRelativePlace == BEAMPLACE_below);
    m_firstNoteOrChord->m_yBeam = isSlopeUp ? centerY - step / 2 : centerY + step / 2;
    m_lastNoteOrChord->m_yBeam = isSlopeUp ? m_firstNoteOrChord->m_yBeam + step : m_firstNoteOrChord->m_yBeam - step;
}

void BeamSegment::CalcBeamPosition(
    const Doc *doc, const Staff *staff, BeamDrawingInterface *beamInterface, bool isHorizontal)
{
    // Set drawing stem positions
    for (BeamElementCoord *coord : m_beamElementCoordRefs) {
        if (beamInterface->m_drawingPlace == BEAMPLACE_above) {
            coord->SetDrawingStemDir(STEMDIRECTION_up, staff, doc, this, beamInterface);
        }
        else if (beamInterface->m_drawingPlace == BEAMPLACE_below) {
            coord->SetDrawingStemDir(STEMDIRECTION_down, staff, doc, this, beamInterface);
        }
        // cross-staff or beam@place=mixed
        else {
            if (beamInterface->m_crossStaffContent) {
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

    // Nothing else to do with tab beams outside the staff
    if (staff->IsTablature() && staff->IsTabWithStemsOutside()) return;

    /******************************************************************/
    // Calculate the slope is necessary

    m_beamSlope = 0.0;
    if (!isHorizontal) {
        int step;
        if (this->CalcBeamSlope(staff, doc, beamInterface, step)) {
            this->CalcAdjustSlope(staff, doc, beamInterface, step);
        }
        else {
            this->CalcAdjustPosition(staff, doc, beamInterface);
        }
    }
    else {
        this->CalcHorizontalBeam(doc, staff, beamInterface);
    }

    if (!beamInterface->m_crossStaffContent) this->AdjustBeamToLedgerLines(doc, staff, beamInterface, isHorizontal);
}

void BeamSegment::CalcAdjustSlope(const Staff *staff, const Doc *doc, BeamDrawingInterface *beamInterface, int &step)
{
    assert(staff);
    assert(doc);
    assert(beamInterface);

    this->CalcAdjustPosition(staff, doc, beamInterface);

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

    bool lengthen = false;
    for (BeamElementCoord *coord : m_beamElementCoordRefs) {
        if (coord->m_stem && coord->m_closestNote) {
            const int len = abs(coord->m_yBeam - coord->m_closestNote->GetDrawingY());
            if (len < refLen) {
                lengthen = true;
                break;
            }
            // Here we should look at duration too because longer values in the middle could actually be OK as they are
            else if (((coord != m_lastNoteOrChord) || (coord != m_firstNoteOrChord)) && (coord->m_dur > DURATION_8)) {
                const int durLen = len - 0.9 * unit;
                if (durLen < refLen) {
                    lengthen = true;
                    break;
                }
            }
        }
    }
    // We need to lengthen the stems
    if (lengthen) {
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
            m_beamSlope = BoundingBox::CalcSlope(Point(m_firstNoteOrChord->m_x, m_firstNoteOrChord->m_yBeam),
                Point(m_lastNoteOrChord->m_x, m_lastNoteOrChord->m_yBeam));

            this->CalcAdjustPosition(staff, doc, beamInterface);
            // Try again - shortening will obviously be false at this stage
            return this->CalcAdjustSlope(staff, doc, beamInterface, step);
        }
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
            m_beamSlope = BoundingBox::CalcSlope(Point(m_firstNoteOrChord->m_x, m_firstNoteOrChord->m_yBeam),
                Point(m_lastNoteOrChord->m_x, m_lastNoteOrChord->m_yBeam));

            this->CalcAdjustPosition(staff, doc, beamInterface);
            // Simply ignore shortening
            return;
        }
    }
}

void BeamSegment::CalcAdjustPosition(const Staff *staff, const Doc *doc, const BeamDrawingInterface *beamInterface)
{
    const int staffTop = staff->GetDrawingY();
    const int staffHeight = doc->GetDrawingStaffSize(staff->m_drawingStaffSize);
    const int unit = doc->GetDrawingUnit(staff->m_drawingStaffSize);

    if (!m_firstNoteOrChord || !m_lastNoteOrChord) return;

    int adjust = 0;
    const int start = m_firstNoteOrChord->m_yBeam;
    const int end = m_lastNoteOrChord->m_yBeam;
    const int height = std::abs(end - start);
    if ((start <= staffTop) && (start >= staffTop - staffHeight)) {
        const int positionWithinStaffLines = std::abs((staffTop - start) % (unit * 2));
        if (beamInterface->m_drawingPlace == BEAMPLACE_above) {
            if (((positionWithinStaffLines == unit) && (m_beamSlope > 0) && (height != unit))
                || ((positionWithinStaffLines == 0.5 * unit) && (m_beamSlope < 0))) {
                adjust = -0.5 * unit;
            }
        }
        else if (beamInterface->m_drawingPlace == BEAMPLACE_below) {
            if (((positionWithinStaffLines == unit) && (m_beamSlope < 0) && (height != unit))
                || ((positionWithinStaffLines == 1.5 * unit) && (m_beamSlope > 0))) {
                adjust = 0.5 * unit;
            }
        }
    }

    m_firstNoteOrChord->m_yBeam += adjust;

    this->CalcSetValues();
}

void BeamSegment::CalcBeamPlace(const Layer *layer, BeamDrawingInterface *beamInterface, data_BEAMPLACE place)
{
    assert(layer);
    assert(beamInterface);

    if (place != BEAMPLACE_NONE) {
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
    else if (beamInterface->m_crossStaffContent) {
        beamInterface->m_drawingPlace = BEAMPLACE_mixed;
    }
    // Look at the layer direction or, finally, at the note position
    else {
        data_STEMDIRECTION layerStemDir = STEMDIRECTION_NONE;
        // Do not look at the layer context when notes from different layers are stemmed together
        if (!this->StemSameas()) layerStemDir = layer->GetDrawingStemDir(&m_beamElementCoordRefs);
        // Layer direction ?
        if (layerStemDir == STEMDIRECTION_NONE) {
            if (m_ledgerLinesBelow != m_ledgerLinesAbove) {
                beamInterface->m_drawingPlace
                    = (m_ledgerLinesBelow > m_ledgerLinesAbove) ? BEAMPLACE_above : BEAMPLACE_below;
            }
            else {
                beamInterface->m_drawingPlace = m_weightedPlace;
            }
        }
        // Look at the note position
        else {
            beamInterface->m_drawingPlace = (layerStemDir == STEMDIRECTION_up) ? BEAMPLACE_above : BEAMPLACE_below;
        }
    }

    // If we have a stem.sameas context and it is unset, update the roles.
    // This will update the roles for both beams
    if (this->StemSameasIsUnset()) {
        this->UpdateSameasRoles(beamInterface->m_drawingPlace);
    }

    // For now force it above
    // if (beamInterface->m_drawingPlace == BEAMPLACE_mixed) beamInterface->m_drawingPlace = BEAMPLACE_above;
}

void BeamSegment::CalcBeamPlaceTab(
    const Layer *layer, const Staff *staff, const Doc *doc, BeamDrawingInterface *beamInterface, data_BEAMPLACE place)
{
    assert(layer);
    assert(staff);
    assert(doc);
    assert(beamInterface);

    if (place != BEAMPLACE_NONE) {
        beamInterface->m_drawingPlace = (place == BEAMPLACE_below) ? BEAMPLACE_below : BEAMPLACE_above;
    }
    // Do we have more that one layer?
    else {
        data_STEMDIRECTION layerStemDir = layer->GetDrawingStemDir();
        // The layerStemDir can be none (single layer), up (1st layer), or down (2n layer)
        // Is is put above by default with tablature with a single layer
        beamInterface->m_drawingPlace = (layerStemDir == STEMDIRECTION_down) ? BEAMPLACE_below : BEAMPLACE_above;
    }

    if (beamInterface->m_drawingPlace == BEAMPLACE_below && staff->IsTabWithStemsOutside()) {
        for (BeamElementCoord *coord : m_beamElementCoordRefs) {
            if (!coord->m_element || !coord->m_element->Is(TABGRP)) continue;
            TabGrp *tabGrp = vrv_cast<TabGrp *>(coord->m_element);
            assert(tabGrp);
            TabDurSym *tabDurSym = vrv_cast<TabDurSym *>(tabGrp->FindDescendantByType(TABDURSYM));
            if (tabDurSym) tabDurSym->AdjustDrawingYRel(staff, doc);
        }
    }
}

void BeamSegment::CalcBeamStemLength(const Staff *staff, data_BEAMPLACE place, bool isHorizontal)
{
    const auto [noteLoc, noteDur, preferredDur] = this->CalcStemDefiningNote(staff, place);
    const data_STEMDIRECTION globalStemDir = (place == BEAMPLACE_below) ? STEMDIRECTION_down : STEMDIRECTION_up;
    for (BeamElementCoord *coord : m_beamElementCoordRefs) {
        const data_STEMDIRECTION stemDir = (place != BEAMPLACE_mixed) ? globalStemDir
            : (coord->m_beamRelativePlace == BEAMPLACE_below)         ? STEMDIRECTION_down
                                                                      : STEMDIRECTION_up;
        // Get the tabDurSym stem length (if any)
        if (coord->m_tabDurSym) {
            m_uniformStemLength = coord->CalculateStemLengthTab(staff, stemDir);
            continue;
        }
        if (!coord->m_closestNote) continue;
        // skip current element if it's longer that minDuration and is not a part of fTrem
        if ((coord->m_dur < noteDur) && !(coord->m_element && coord->m_element->GetFirstAncestor(FTREM))) continue;
        // adjust stem length if location matches
        const data_DURATION dur = (preferredDur != DURATION_NONE) ? preferredDur : coord->m_dur;
        const int coordStemLength = coord->CalculateStemLength(staff, stemDir, isHorizontal, dur);
        if (coord->m_closestNote->GetDrawingLoc() == noteLoc) {
            m_uniformStemLength = coordStemLength;
        }
    }
    // make adjustments for the grace notes length
    for (BeamElementCoord *coord : m_beamElementCoordRefs) {
        if (coord->m_element) {
            if (coord->m_element->IsGraceNote()) {
                m_uniformStemLength *= 0.75;
                break;
            }
        }
    }
}

int BeamSegment::CalcMixedBeamCenterY(int step, int unit) const
{
    const int dist = m_lastNoteOrChord->m_x - m_firstNoteOrChord->m_x;
    const bool isSlopeUp = (m_firstNoteOrChord->m_beamRelativePlace == m_lastNoteOrChord->m_beamRelativePlace)
        ? (m_beamSlope > 0)
        : (m_lastNoteOrChord->m_beamRelativePlace == BEAMPLACE_below);
    const int sign = isSlopeUp ? 1 : -1;
    const double targetSlope = double(sign * step) / dist;

    int highestBelowBeam = VRV_UNSET;
    int lowestAboveBeam = VRV_UNSET;
    for (BeamElementCoord *coord : m_beamElementCoordRefs) {
        const int normalizedY = coord->m_yBeam - targetSlope * (coord->m_x - m_firstNoteOrChord->m_x);
        // Note that for elements below the beam the beamRelativePlace is above and vice versa
        if (coord->m_beamRelativePlace == BEAMPLACE_above) {
            if ((highestBelowBeam == VRV_UNSET) || (normalizedY > highestBelowBeam)) {
                highestBelowBeam = normalizedY;
            }
        }
        if (coord->m_beamRelativePlace == BEAMPLACE_below) {
            if ((lowestAboveBeam == VRV_UNSET) || (normalizedY < lowestAboveBeam)) {
                lowestAboveBeam = normalizedY;
            }
        }
    }

    int centerY = (m_firstNoteOrChord->m_yBeam + m_lastNoteOrChord->m_yBeam) / 2;
    if ((highestBelowBeam != VRV_UNSET) && (lowestAboveBeam != VRV_UNSET)) {
        centerY = (highestBelowBeam + lowestAboveBeam) / 2 + targetSlope * dist / 2;
    }

    // Resulting shift must be an integral multiple of half a unit
    centerY += (m_firstNoteOrChord->m_yBeam - centerY) % (unit / 2);

    return centerY;
}

std::tuple<int, int, data_DURATION> BeamSegment::CalcStemDefiningNote(const Staff *staff, data_BEAMPLACE place) const
{
    data_DURATION shortestDuration = DURATION_4;
    int shortestLoc = VRV_UNSET;
    data_DURATION relevantDuration = DURATION_4;
    int relevantLoc = VRV_UNSET;
    const data_STEMDIRECTION globalStemDir = (place == BEAMPLACE_below) ? STEMDIRECTION_down : STEMDIRECTION_up;
    for (BeamElementCoord *coord : m_beamElementCoordRefs) {
        const data_STEMDIRECTION stemDir = (place != BEAMPLACE_mixed) ? globalStemDir
            : (coord->m_beamRelativePlace == BEAMPLACE_below)         ? STEMDIRECTION_down
                                                                      : STEMDIRECTION_up;
        coord->SetClosestNoteOrTabDurSym(stemDir, staff->IsTabWithStemsOutside());
        // Nothing else to do if we have no closest note (that includes tab beams outside the staff)
        if (!coord->m_closestNote) continue;
        const int currentLoc = coord->m_closestNote->GetDrawingLoc();
        // set initial values for both locations and durations to that of first note
        if (relevantLoc == VRV_UNSET) {
            relevantLoc = currentLoc;
            shortestLoc = relevantLoc;
            relevantDuration = coord->m_dur;
            shortestDuration = relevantDuration;
            continue;
        }
        // save location and duration of the note, if it is closer to the beam (i.e. placed higher on the staff for
        // above beams and vice versa for below beams)
        if ((place == BEAMPLACE_above) && (currentLoc > relevantLoc)) {
            relevantLoc = currentLoc;
            relevantDuration = coord->m_dur;
        }
        else if ((place == BEAMPLACE_below) && (currentLoc < relevantLoc)) {
            relevantLoc = currentLoc;
            relevantDuration = coord->m_dur;
        }
        // save location and duration of the note that have shortest duration
        if (coord->m_dur > shortestDuration) {
            shortestDuration = coord->m_dur;
            shortestLoc = currentLoc;
        }
        else if (coord->m_dur == shortestDuration) {
            if (((stemDir == STEMDIRECTION_up) && (currentLoc > shortestLoc))
                || ((stemDir == STEMDIRECTION_down) && (currentLoc < shortestLoc))) {
                shortestDuration = coord->m_dur;
                shortestLoc = currentLoc;
            }
        }
    }

    data_DURATION adjusted_duration = DURATION_NONE;
    // if shortest note location does not offset its duration (shorter notes need more space for additional beams) then
    // give preference to the its location
    if ((shortestDuration - relevantDuration) > (std::abs(relevantLoc - shortestLoc) + 1)) {
        relevantLoc = shortestLoc;
        relevantDuration = shortestDuration;
    }
    else if ((shortestDuration - relevantDuration) == (std::abs(relevantLoc - shortestLoc) + 1)) {
        if (((globalStemDir == STEMDIRECTION_up) && (relevantLoc > 4))
            || ((globalStemDir == STEMDIRECTION_down) && (relevantLoc < 4))) {
            relevantLoc = shortestLoc;
            relevantDuration = shortestDuration;
        }
    }
    else if ((shortestDuration - relevantDuration) == std::abs(relevantLoc - shortestLoc)) {
        adjusted_duration = static_cast<data_DURATION>((relevantDuration + shortestDuration) / 2);
    }

    return { relevantLoc, relevantDuration, adjusted_duration };
}

void BeamSegment::CalcHorizontalBeam(const Doc *doc, const Staff *staff, const BeamDrawingInterface *beamInterface)
{

    if (beamInterface->m_drawingPlace == BEAMPLACE_mixed) {
        const int unit = doc->GetDrawingUnit(staff->m_drawingStaffSize);
        this->CalcMixedBeamPosition(beamInterface, 0, unit);
    }
    else {
        int maxLength = (beamInterface->m_drawingPlace == BEAMPLACE_above) ? VRV_UNSET : -VRV_UNSET;

        // Find the longest stem length
        for (BeamElementCoord *coord : m_beamElementCoordRefs) {
            if (!coord->m_stem) continue;

            if (beamInterface->m_drawingPlace == BEAMPLACE_above) {
                if (maxLength < coord->m_yBeam) maxLength = coord->m_yBeam;
            }
            else if (beamInterface->m_drawingPlace == BEAMPLACE_below) {
                if (maxLength > coord->m_yBeam) maxLength = coord->m_yBeam;
            }
        }

        if (-VRV_UNSET != abs(maxLength)) {
            m_beamElementCoordRefs.at(0)->m_yBeam = maxLength;
        }
    }

    this->CalcAdjustPosition(staff, doc, beamInterface);
}

void BeamSegment::CalcMixedBeamPlace(const Staff *staff)
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
    else {
        const auto iter = std::find_if(m_beamElementCoordRefs.begin(), m_beamElementCoordRefs.end(),
            [](auto coord) { return coord->m_element->HasCrossStaff(); });
        if ((iter != m_beamElementCoordRefs.end()) && (*iter)->m_element->Is(CHORD)) {
            Chord *chord = vrv_cast<Chord *>((*iter)->m_element);
            Staff *staffAbove = NULL;
            Staff *staffBelow = NULL;
            chord->GetCrossStaffExtremes(staffAbove, staffBelow);
            // change beam direction in case if cross-staff note is located in staff above
            if (staffAbove) beamPlaceBelow = true;
            // otherwise just leave default value
        }
    }

    for (BeamElementCoord *coord : m_beamElementCoordRefs) {
        if (it != m_beamElementCoordRefs.end()) {
            if (!coord->m_element->m_crossStaff) {
                coord->m_beamRelativePlace = beamPlaceBelow ? BEAMPLACE_below : BEAMPLACE_above;
            }
            else {
                coord->m_beamRelativePlace = beamPlaceBelow ? BEAMPLACE_above : BEAMPLACE_below;
            }
        }
        else if (coord->GetStemDir() != STEMDIRECTION_NONE) {
            coord->m_beamRelativePlace = (STEMDIRECTION_up == coord->GetStemDir()) ? BEAMPLACE_above : BEAMPLACE_below;
        }
        else {
            coord->m_beamRelativePlace = beamPlaceBelow ? BEAMPLACE_below : BEAMPLACE_above;
        }
    }
}

void BeamSegment::CalcPartialFlagPlace()
{
    // Start from note that is shorter than DURATION_8 - we do not care otherwise, since those do not have additional
    // beams
    auto start = std::find_if(m_beamElementCoordRefs.begin(), m_beamElementCoordRefs.end(),
        [](BeamElementCoord *coord) { return coord->m_dur >= DURATION_16; });
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
                return ((coord->m_beamRelativePlace != place) || (coord->m_dur <= DURATION_8) || (coord->m_breaksec));
            });
            subdivision = found;

            // Handle different cases, where we either don't want to proceed (e.g. end of the beam reached) or we want
            // to process them separately (e.g. on direction change from shorter to longer notes, or vice versa, we do
            // not want last note of the subdivision to have additional beam, so that it's clearly distinguishable).
            if ((m_beamElementCoordRefs.end() == found) || ((*found)->m_dur <= DURATION_8)) break;
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
    const int startingX = m_firstNoteOrChord->m_x;
    const int startingY = m_firstNoteOrChord->m_yBeam;

    for (BeamElementCoord *coord : m_beamElementCoordRefs) {
        coord->m_yBeam = startingY + m_beamSlope * (coord->m_x - startingX);
    }
}

int BeamSegment::GetAdjacentElementsDuration(int elementX) const
{
    if ((elementX < m_beamElementCoordRefs.front()->m_x) || (elementX > m_beamElementCoordRefs.back()->m_x)) {
        return DURATION_8;
    }
    for (int i = 0; i < int(m_beamElementCoordRefs.size()) - 1; ++i) {
        if ((m_beamElementCoordRefs.at(i)->m_x < elementX) && (m_beamElementCoordRefs.at(i + 1)->m_x > elementX)) {
            return std::min(m_beamElementCoordRefs.at(i)->m_dur, m_beamElementCoordRefs.at(i + 1)->m_dur);
        }
    }
    return DURATION_8;
}

int BeamSegment::GetStartingX() const
{
    return (m_beamElementCoordRefs.empty() ? 0 : m_beamElementCoordRefs.at(0)->m_x);
}

int BeamSegment::GetStartingY() const
{
    return (m_beamElementCoordRefs.empty() ? 0 : m_beamElementCoordRefs.at(0)->m_yBeam);
}

void BeamSegment::InitSameasRoles(Beam *sameasBeam, data_BEAMPLACE &initialPlace)
{
    if (!sameasBeam) return;

    // This is the first time and the first beam for which we are calling it.
    // All we need to do is setting the pointer to the role of the other beam
    // and mark both of them as unset
    if (m_stemSameasRole == SAMEAS_NONE) {
        m_stemSameasReverseRole = &sameasBeam->m_beamSegment.m_stemSameasRole;
        m_stemSameasRole = SAMEAS_UNSET;
        (*m_stemSameasReverseRole) = SAMEAS_UNSET;
    }
    // The reverse role is not set, which means we are calling it from the second beam.
    // We need to set the initial place of the beam as previously calculated for the first one.
    // If the role of the second beam (this) has been mark as is primary, it means the beam
    // has been place below, above if secondary.
    else if (!m_stemSameasReverseRole) {
        initialPlace = (this->StemSameasIsPrimary()) ? BEAMPLACE_below : BEAMPLACE_above;
    }
    // Otherwise, calling it (again) from the first beam, nothing to do.
}

void BeamSegment::UpdateSameasRoles(data_BEAMPLACE place)
{
    if (!m_stemSameasReverseRole || !this->StemSameasIsUnset()) return;

    // Because m_stemSameasReverseRole is instantiated, then it means we are in
    // the first of the beams sharing the stems.
    // The beam is placed above, this one is primary, the other secondary.
    if (place == BEAMPLACE_above) {
        m_stemSameasRole = SAMEAS_PRIMARY;
        (*m_stemSameasReverseRole) = SAMEAS_SECONDARY;
    }
    // The beam is placed below, this one is secondary, the other primary.
    else {
        m_stemSameasRole = SAMEAS_SECONDARY;
        (*m_stemSameasReverseRole) = SAMEAS_PRIMARY;
    }
}

void BeamSegment::CalcNoteHeadShiftForStemSameas(Beam *sameasBeam, data_BEAMPLACE place)
{
    if (!sameasBeam) return;

    // We want to do this only from the second beams sharing the stems and if the role is set
    if (m_stemSameasReverseRole || this->StemSameasIsUnset()) return;

    const ArrayOfBeamElementCoords *sameasCoords = &sameasBeam->m_beamSegment.m_beamElementCoordRefs;
    data_STEMDIRECTION stemDir = (place == BEAMPLACE_above) ? STEMDIRECTION_up : STEMDIRECTION_down;

    // Loop through both list of elements and calculate the note head shift
    const int sameasSize = (int)sameasCoords->size();
    for (int i = 0; i < int(m_beamElementCoordRefs.size()) && i < sameasSize; ++i) {
        if (!m_beamElementCoordRefs.at(i)->m_element || !sameasCoords->at(i)->m_element) continue;

        Note *note1 = (m_beamElementCoordRefs.at(i)->m_element->Is(NOTE))
            ? vrv_cast<Note *>(m_beamElementCoordRefs.at(i)->m_element)
            : NULL;
        Note *note2
            = (sameasCoords->at(i)->m_element->Is(NOTE)) ? vrv_cast<Note *>(sameasCoords->at(i)->m_element) : NULL;

        if (!note1 || !note2) continue;

        note1->CalcNoteHeadShiftForSameasNote(note2, stemDir);
    }
}

void BeamSegment::RequestStaffSpace(const Doc *doc, const BeamDrawingInterface *beamInterface)
{
    assert(doc);
    assert(beamInterface);

    if (beamInterface->m_drawingPlace != BEAMPLACE_mixed) return;
    if (!beamInterface->m_beamStaff || !beamInterface->m_crossStaffContent) return;

    // Min length is taken from the option
    // We add an extra unit for cross staff (more tolerance is possible here compared to single staff mixed beams)
    const int unit = doc->GetDrawingUnit(beamInterface->m_beamStaff->m_drawingStaffSize);
    const int minLength = (1 + doc->GetOptions()->m_beamMixedStemMin.GetValue()) * unit;

    // Determine the alignments above and below
    StaffAlignment *above = NULL;
    StaffAlignment *below = NULL;
    if (beamInterface->m_beamStaff->GetN() < beamInterface->m_crossStaffContent->GetN()) {
        above = beamInterface->m_beamStaff->GetAlignment();
        below = beamInterface->m_crossStaffContent->GetAlignment();
    }
    else {
        above = beamInterface->m_crossStaffContent->GetAlignment();
        below = beamInterface->m_beamStaff->GetAlignment();
    }

    // Update the requested staff space
    const auto [minLengthAbove, minLengthBelow] = this->GetMinimalStemLength(beamInterface);
    if ((minLengthAbove < minLength) && above) {
        above->SetRequestedSpaceBelow(minLength - minLengthAbove);
    }
    if ((minLengthBelow < minLength) && below) {
        below->SetRequestedSpaceAbove(minLength - minLengthBelow);
    }
}

//----------------------------------------------------------------------------
// Beam
//----------------------------------------------------------------------------

static const ClassRegistrar<Beam> s_factory("beam", BEAM);

Beam::Beam() : LayerElement(BEAM), BeamDrawingInterface(), AttBeamedWith(), AttBeamRend(), AttColor(), AttCue()
{
    this->RegisterAttClass(ATT_BEAMEDWITH);
    this->RegisterAttClass(ATT_BEAMREND);
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_CUE);

    this->Reset();
}

Beam::~Beam() {}

void Beam::Reset()
{
    LayerElement::Reset();
    BeamDrawingInterface::Reset();
    this->ResetBeamedWith();
    this->ResetBeamRend();
    this->ResetColor();
    this->ResetCue();

    m_stemSameas = NULL;
}

void Beam::CloneReset()
{
    // Since these are owned by the beam we cloned from, empty the list
    // Do it before Object::CloneReset since that one will reset the coord list
    m_beamElementCoords.clear();

    LayerElement::CloneReset();
}

bool Beam::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ BEAM, BTREM, CHORD, CLEF, FTREM, GRACEGRP, NOTE, REST, SPACE, TABGRP,
        TUPLET };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else if (Object::IsEditorialElement(classId)) {
        return true;
    }
    else {
        return false;
    }
}

void Beam::FilterList(ListOfConstObjects &childList) const
{
    bool firstNoteGrace = false;
    // We want to keep only notes and rests
    // Eventually, we also need to filter out grace notes properly (e.g., with sub-beams)
    ListOfConstObjects::iterator iter = childList.begin();

    const bool isTabBeam = this->IsTabBeam();

    while (iter != childList.end()) {
        if (!(*iter)->IsLayerElement()) {
            // remove anything that is not an LayerElement (e.g. Verse, Syl, etc)
            iter = childList.erase(iter);
            continue;
        }
        if (!(*iter)->HasInterface(INTERFACE_DURATION)) {
            // remove anything that has not a DurationInterface
            iter = childList.erase(iter);
            continue;
        }
        else if (isTabBeam) {
            if (!(*iter)->Is(TABGRP)) {
                iter = childList.erase(iter);
            }
            else {
                ++iter;
            }
            continue;
        }
        else {
            const LayerElement *element = vrv_cast<const LayerElement *>(*iter);
            assert(element);
            // if we are at the beginning of the beam
            // and the note is cueSize
            // assume all the beam is of grace notes
            if (childList.begin() == iter) {
                if (element->IsGraceNote()) firstNoteGrace = true;
            }
            // if the first note in beam was NOT a grace
            // we have grace notes embedded in a beam
            // drop them
            if (!firstNoteGrace && (element->IsGraceNote())) {
                iter = childList.erase(iter);
                continue;
            }
            // also remove notes within chords
            if (element->Is(NOTE)) {
                const Note *note = vrv_cast<const Note *>(element);
                assert(note);
                if (note->IsChordTone()) {
                    iter = childList.erase(iter);
                    continue;
                }
            }
            ++iter;
        }
    }
}

const ArrayOfBeamElementCoords *Beam::GetElementCoords()
{
    this->GetList();
    return &m_beamElementCoords;
}

bool Beam::IsTabBeam() const
{
    return (this->FindDescendantByType(TABGRP));
}

FunctorCode Beam::Accept(Functor &functor)
{
    return functor.VisitBeam(this);
}

FunctorCode Beam::Accept(ConstFunctor &functor) const
{
    return functor.VisitBeam(this);
}

FunctorCode Beam::AcceptEnd(Functor &functor)
{
    return functor.VisitBeamEnd(this);
}

FunctorCode Beam::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitBeamEnd(this);
}

//----------------------------------------------------------------------------
// BeamSpanSegment
//----------------------------------------------------------------------------

BeamSpanSegment::BeamSpanSegment()
{
    m_measure = NULL;
    m_staff = NULL;
    m_layer = NULL;
    m_begin = NULL;
    m_end = NULL;
    m_spanningType = SPANNING_START_END;
}

void BeamSpanSegment::SetSpanningType(int systemIndex, int systemCount)
{
    if (0 == systemIndex) {
        m_spanningType = SPANNING_START;
    }
    else if ((systemCount - 1) == systemIndex) {
        m_spanningType = SPANNING_END;
    }
    else {
        m_spanningType = SPANNING_MIDDLE;
    }
}

void BeamSpanSegment::AppendSpanningCoordinates(const Measure *measure)
{
    const int spanningType = m_spanningType;
    if (SPANNING_START_END == spanningType) return;

    const BarLine *bar = measure->GetRightBarLine();
    const int rightSide = bar->GetDrawingX();
    BeamElementCoord *front = m_beamElementCoordRefs.front();
    BeamElementCoord *back = m_beamElementCoordRefs.back();
    double slope = 0.0;
    if (m_beamElementCoordRefs.size() > 1) {
        slope = (double)(back->m_yBeam - front->m_yBeam) / (double)(back->m_x - front->m_x);
    }

    // in case if beamSpan starts in current system - stretch beam to the right barline
    if ((SPANNING_START == spanningType) || (SPANNING_MIDDLE == spanningType)) {
        BeamElementCoord *right = new BeamElementCoord(*back);
        const int distance = rightSide - back->m_x;
        right->m_x = rightSide;
        right->m_yBeam += distance * slope;
        m_beamElementCoordRefs.push_back(right);
    }
    // otherwise start beam closer to the start of the Measure, to indicate spanning
    if ((SPANNING_END == spanningType) || (SPANNING_MIDDLE == spanningType)) {
        BeamElementCoord *left = new BeamElementCoord(*front);
        int offset = 0;
        if (m_beamElementCoordRefs.size() > 1) {
            const int divideBy = 2 * ((int)m_beamElementCoordRefs.size() - 1);
            offset = (back->m_x - front->m_x) / divideBy;
        }
        else {
            // 1.5 * unit offset in this case (hardcoded for the time being)
            offset = 270;
        }
        left->m_x -= offset;
        left->m_yBeam -= offset * slope;
        m_beamElementCoordRefs.insert(m_beamElementCoordRefs.begin(), left);
    }
}

//----------------------------------------------------------------------------
// BeamElementCoord
//----------------------------------------------------------------------------

BeamElementCoord::~BeamElementCoord() {}

data_STEMDIRECTION BeamElementCoord::GetStemDir() const
{
    // m_stem is not necessary set, so we need to look at the Note / Chord original value
    // Example: IsInBeam called in PrepareLayerElementPartsFunctor::VisitNote when reaching
    // the first note of the beam
    if (m_stem) {
        return m_stem->GetDir();
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

void BeamElementCoord::SetDrawingStemDir(data_STEMDIRECTION stemDir, const Staff *staff, const Doc *doc,
    const BeamSegment *segment, const BeamDrawingInterface *interface)
{
    assert(staff);
    assert(doc);
    assert(interface);

    int stemLen = segment->m_uniformStemLength;
    if (interface->m_crossStaffContent || (BEAMPLACE_mixed == interface->m_drawingPlace)) {
        if (((STEMDIRECTION_up == stemDir) && (stemLen < 0)) || ((STEMDIRECTION_down == stemDir) && (stemLen > 0))) {
            stemLen *= -1;
        }
    }
    m_centered = (segment->m_uniformStemLength % 2) || (m_element->IsGraceNote());

    if (m_element->Is({ REST, SPACE })) {
        m_x += m_element->GetDrawingRadius(doc);
        m_yBeam = m_element->GetDrawingY();
        m_yBeam += (stemLen * doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2);
        return;
    }

    // Get the interface from the chord (if any)
    StemmedDrawingInterface *stemInterface = this->GetStemHolderInterface();
    // Not a Chord or a Note or no TabDurSym in TabGrp
    if (!stemInterface) return;

    m_stem = stemInterface->GetDrawingStem();
    assert(m_stem);

    m_stem->SetDrawingStemDir(stemDir);
    m_yBeam = m_element->GetDrawingY();

    // Move and take into account the glyph cut-outs
    if (STEMDIRECTION_up == stemDir) {
        m_x += stemInterface->GetStemUpSE(doc, staff->m_drawingStaffSize, interface->m_cueSize).x;
        m_x -= doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2;
    }
    else {
        m_x += stemInterface->GetStemDownNW(doc, staff->m_drawingStaffSize, interface->m_cueSize).x;
        m_x += doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2;
    }

    if (m_tabDurSym && !m_closestNote) {
        m_yBeam = m_tabDurSym->GetDrawingY();
        m_yBeam += (stemLen * doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2);
        return;
    }

    if (!m_closestNote) return;

    if (!interface->m_cueSize && (m_element->IsGraceNote() || m_element->GetDrawingCueSize())
        && !this->m_element->GetFirstAncestor(CHORD) && (STEMDIRECTION_up == stemDir)) {
        const double cueScaling = doc->GetCueScaling();
        const int diameter = 2 * m_element->GetDrawingRadius(doc);
        const int cueShift = (1.0 / cueScaling - 1.0) * diameter;
        m_x -= cueShift;
    }

    m_yBeam = m_closestNote->GetDrawingY();
    m_yBeam += (stemLen * doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2);

    if (m_element->IsGraceNote()) return;

    // Make sure the stem reaches the center of the staff
    // Mark the segment as extendedToCenter since we then want a reduced slope
    if (!interface->m_isSpanningElement && !interface->m_crossStaffContent
        && (BEAMPLACE_mixed != interface->m_drawingPlace)) {
        if (((stemDir == STEMDIRECTION_up) && (m_yBeam <= segment->m_verticalCenter))
            || ((stemDir == STEMDIRECTION_down) && (segment->m_verticalCenter <= m_yBeam))) {
            m_yBeam = segment->m_verticalCenter;
            m_centered = false;
        }
    }

    m_yBeam += m_overlapMargin;
}

int BeamElementCoord::CalculateStemLength(
    const Staff *staff, data_STEMDIRECTION stemDir, bool isHorizontal, data_DURATION preferredDur) const
{
    if (!m_closestNote) return 0;

    const bool onStaffSpace = m_closestNote->GetDrawingLoc() % 2;
    bool extend = onStaffSpace;
    const int standardStemLen = STANDARD_STEMLENGTH * 2;
    // Check if the stem has to be shortened because outside the staff
    // In this case, Note::CalcStemLenInThirdUnits will return a value shorter than 2 * STANDARD_STEMLENGTH
    const int stemLenInHalfUnits = m_closestNote->CalcStemLenInThirdUnits(staff, stemDir) * 2 / 3;
    // Do not extend when not on the staff line
    if (stemLenInHalfUnits != standardStemLen) {
        extend = false;
    }

    const int directionBias = (stemDir == STEMDIRECTION_up) ? 1 : -1;
    int stemLen = directionBias;
    // For 8th notes, use the shortened stem (if shortened)
    if (preferredDur == DURATION_8) {
        if (stemLenInHalfUnits != standardStemLen) {
            stemLen *= stemLenInHalfUnits;
        }
        else {
            stemLen *= (onStaffSpace || !isHorizontal) ? 14 : 13;
        }
    }
    else {
        const bool isOddLength = (extend || !isHorizontal);
        switch (m_dur) {
            case (DURATION_16): stemLen *= isOddLength ? 14 : 13; break;
            case (DURATION_32): stemLen *= isOddLength ? 18 : 16; break;
            case (DURATION_64): stemLen *= isOddLength ? 22 : 20; break;
            case (DURATION_128): stemLen *= isOddLength ? 26 : 24; break;
            case (DURATION_256): stemLen *= isOddLength ? 30 : 28; break;
            case (DURATION_512): stemLen *= isOddLength ? 34 : 32; break;
            case (DURATION_1024): stemLen *= isOddLength ? 38 : 36; break;
            default: stemLen *= 14;
        }
    }

    return stemLen + CalculateStemModAdjustment(stemLen, directionBias);
}

int BeamElementCoord::CalculateStemLengthTab(const Staff *staff, data_STEMDIRECTION stemDir) const
{
    if (!m_tabDurSym) return 0;

    const int directionBias = (stemDir == STEMDIRECTION_up) ? 1 : -1;
    return m_tabDurSym->CalcStemLenInThirdUnits(staff, stemDir) * 2 / 3 * directionBias;
}

int BeamElementCoord::CalculateStemModAdjustment(int stemLength, int directionBias) const
{
    // handle @stem.mod attribute to properly draw beams with tremolos
    int slashFactor = 0;
    if (m_element->Is(NOTE)) {
        if (m_closestNote->GetStemMod() < STEMMODIFIER_sprech) slashFactor = m_closestNote->GetStemMod() - 1;
    }
    else if (m_element->Is(CHORD)) {
        const Chord *chord = vrv_cast<const Chord *>(m_element);
        assert(chord);
        if (chord->GetStemMod() < STEMMODIFIER_sprech) slashFactor = chord->GetStemMod() - 1;
    }
    const int stemLengthInUnits = abs(stemLength / 2);
    // if stem length is very short and is not enough to fit slashes, we need to adjust it
    if (stemLengthInUnits - 3 < slashFactor) {
        return (directionBias * (3 + slashFactor - stemLengthInUnits) * 4);
    }

    return 0;
}

StemmedDrawingInterface *BeamElementCoord::GetStemHolderInterface()
{
    if (!m_element || !m_element->Is({ CHORD, NOTE, TABGRP })) return NULL;

    if (m_element->Is({ CHORD, NOTE })) return m_element->GetStemmedDrawingInterface();

    TabGrp *tabGrp = vrv_cast<TabGrp *>(m_element);
    assert(tabGrp);
    TabDurSym *tabDurSym = vrv_cast<TabDurSym *>(tabGrp->FindDescendantByType(TABDURSYM));
    if (tabDurSym) return tabDurSym->GetStemmedDrawingInterface();

    return NULL;
}

void BeamElementCoord::SetClosestNoteOrTabDurSym(data_STEMDIRECTION stemDir, bool outsideStaff)
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
    else if (m_element->Is(TABGRP)) {
        TabGrp *tabGrp = vrv_cast<TabGrp *>(m_element);
        assert(tabGrp);
        m_tabDurSym = vrv_cast<TabDurSym *>(tabGrp->FindDescendantByType(TABDURSYM));
        if (!outsideStaff) {
            m_closestNote = (STEMDIRECTION_up == stemDir) ? tabGrp->GetTopNote() : tabGrp->GetBottomNote();
        }
    }
}

void BeamElementCoord::UpdateStemLength(
    StemmedDrawingInterface *stemmedInterface, int y1, int y2, int stemAdjust, bool inMixedBeam)
{
    Stem *stem = stemmedInterface->GetDrawingStem();
    // This is the case with fTrem on whole notes
    if (!stem) return;

    // Since the values were calculated relatively to the element position, adjust them
    stem->SetDrawingXRel(m_x - m_element->GetDrawingX());
    stem->SetDrawingYRel(y2 - m_element->GetDrawingY());
    const int prevStemLen = stem->GetDrawingStemLen();
    const int newStemLen = y2 - y1;
    stem->SetDrawingStemLen(newStemLen);
    stem->SetDrawingStemAdjust(-stemAdjust);
    const int lenChange = newStemLen - prevStemLen;
    // If length didn't change or no mixed beam - just exit
    if (!lenChange || !inMixedBeam) return;

    // Adjust existing artic
    ListOfObjects artics = m_element->FindAllDescendantsByType(ARTIC);
    for (Object *object : artics) {
        Artic *artic = vrv_cast<Artic *>(object);
        if (((artic->GetDrawingPlace() == STAFFREL_above) && (stem->GetDrawingStemDir() == STEMDIRECTION_up))
            || ((artic->GetDrawingPlace() == STAFFREL_below) && (stem->GetDrawingStemDir() == STEMDIRECTION_down))) {
            artic->SetDrawingYRel(artic->GetDrawingYRel() - lenChange);
        }
    }
}

std::pair<int, int> Beam::GetAdditionalBeamCount() const
{
    data_DURATION topShortestDur = DURATION_8;
    data_DURATION bottomShortestDur = DURATION_8;
    std::for_each(m_beamElementCoords.begin(), m_beamElementCoords.end(), [&](BeamElementCoord *coord) {
        if (coord->m_partialFlagPlace == BEAMPLACE_above) {
            topShortestDur = vrv::DurationMax(topShortestDur, coord->m_dur);
        }
        else if (coord->m_partialFlagPlace == BEAMPLACE_below) {
            bottomShortestDur = vrv::DurationMax(bottomShortestDur, coord->m_dur);
        }
    });

    return { topShortestDur - DURATION_8, bottomShortestDur - DURATION_8 };
}

int Beam::GetBeamPartDuration(int x, bool includeRests) const
{
    // find element with position closest to the specified coordinate
    const auto it = std::find_if(m_beamSegment.m_beamElementCoordRefs.begin(),
        m_beamSegment.m_beamElementCoordRefs.end(), [x, includeRests](BeamElementCoord *coord) {
            return (x < coord->m_x) && (!coord->m_element->Is(REST) || includeRests);
        });
    // handle cases when coordinate is outside of the beam
    if (it == m_beamSegment.m_beamElementCoordRefs.end()) {
        return DURATION_8;
    }
    else if (it == m_beamSegment.m_beamElementCoordRefs.begin()) {
        return (*it)->m_dur;
    }
    // Get previous relevant element (skipping over rests if needed)
    auto reverseIt = std::make_reverse_iterator(it);
    reverseIt = std::find_if(reverseIt, m_beamSegment.m_beamElementCoordRefs.rend(),
        [includeRests](BeamElementCoord *coord) { return (!coord->m_element->Is(REST) || includeRests); });
    if (reverseIt != m_beamSegment.m_beamElementCoordRefs.rend()) return std::min((*it)->m_dur, (*reverseIt)->m_dur);
    return (*it)->m_dur;
}

int Beam::GetBeamPartDuration(const Object *object, bool includeRests) const
{
    return this->GetBeamPartDuration(object->GetDrawingX(), includeRests);
}

} // namespace vrv
