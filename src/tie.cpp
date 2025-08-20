/////////////////////////////////////////////////////////////////////////////
// Name:        tie.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tie.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "chord.h"
#include "comparison.h"
#include "doc.h"
#include "dot.h"
#include "elementpart.h"
#include "functor.h"
#include "layer.h"
#include "note.h"
#include "slur.h"
#include "staff.h"
#include "stem.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Tie
//----------------------------------------------------------------------------

static const ClassRegistrar<Tie> s_factory("tie", TIE);

Tie::Tie() : ControlElement(TIE), TimeSpanningInterface(), AttCurvature(), AttLineRendBase()
{
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_CURVATURE);
    this->RegisterAttClass(ATT_LINERENDBASE);

    this->Reset();
}

Tie::Tie(ClassId classId) : ControlElement(classId), TimeSpanningInterface(), AttCurvature(), AttLineRendBase()
{
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_CURVATURE);
    this->RegisterAttClass(ATT_LINERENDBASE);

    this->Reset();
}

Tie::~Tie() {}

void Tie::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    this->ResetCurvature();
    this->ResetLineRendBase();
}

bool Tie::AdjustEnharmonicTies(const Doc *doc, const FloatingCurvePositioner *curve, Point bezier[4],
    const Note *startNote, const Note *endNote, curvature_CURVEDIR drawingCurveDir) const
{
    ListOfConstObjects objects = endNote->FindAllDescendantsByType(ACCID);
    if (objects.empty()) return false;

    int overlap = 0;
    bool discard = false;
    for (const Object *object : objects) {
        overlap = curve->CalcAdjustment(object, discard);
    }
    if (!overlap) return false;

    // adjust overlap with regards to direction
    overlap *= (drawingCurveDir == curvature_CURVEDIR_below) ? -1 : 1;

    const int drawingRadius = startNote->GetDrawingRadius(doc);
    const int drawingUnit = doc->GetDrawingUnit(100);
    // adjust endpoints of curve
    if ((startNote->GetDrawingStemDir() == STEMDIRECTION_up) && (drawingCurveDir == curvature_CURVEDIR_above)) {
        bezier[0].x = startNote->GetDrawingX() + 2 * drawingRadius + drawingUnit / 2;
    }
    else {
        bezier[0].x = startNote->GetDrawingX() + drawingRadius;
    }
    if ((startNote->GetDrawingStemDir() == STEMDIRECTION_down) && (drawingCurveDir == curvature_CURVEDIR_below)) {
        bezier[3].x = endNote->GetDrawingX() - drawingUnit / 2;
    }
    else {
        bezier[3].x = endNote->GetDrawingX() + drawingRadius;
    }

    const int endpointShift = overlap * 0.6;
    if (drawingCurveDir == curvature_CURVEDIR_below) {
        if (startNote->GetDrawingLoc() < endNote->GetDrawingLoc()) {
            bezier[0].y += endpointShift;
            bezier[3].y = bezier[0].y;
        }
        else if (startNote->GetDrawingLoc() > endNote->GetDrawingLoc()) {
            bezier[3].y += endpointShift;
            bezier[0].y = bezier[3].y;
        }
    }
    else if (drawingCurveDir == curvature_CURVEDIR_above) {
        if (startNote->GetDrawingLoc() > endNote->GetDrawingLoc()) {
            bezier[0].y += endpointShift;
            bezier[3].y = bezier[0].y;
        }
        else if (startNote->GetDrawingLoc() < endNote->GetDrawingLoc()) {
            bezier[3].y += endpointShift;
            bezier[0].y = bezier[3].y;
        }
    }

    // adjust control points of the curve
    const int length = bezier[3].x - bezier[0].x;
    bezier[1].x = bezier[0].x + 0.25 * length;
    bezier[1].y += 1.2 * overlap;
    bezier[2].x = bezier[0].x + 0.75 * length;
    bezier[2].y += 1.2 * overlap;

    return true;
}

bool Tie::CalculatePosition(const Doc *doc, const Staff *staff, int x1, int x2, int spanningType, Point bezier[4])
{
    if (!doc || !staff) return false;

    const int drawingUnit = doc->GetDrawingUnit(staff->m_drawingStaffSize);
    /************** parent layers **************/

    if ((spanningType != SPANNING_START_END) && (spanningType != SPANNING_START) && (spanningType != SPANNING_END)) {
        //  this make no sense
        LogDebug("Tie across an entire system is not supported");
        return false;
    }

    Note *note1 = dynamic_cast<Note *>(this->GetStart());
    Note *note2 = dynamic_cast<Note *>(this->GetEnd());

    if (!note1 && !note2) {
        // no note, obviously nothing to do...
        // this also means that notes with tstamp events are not supported
        return false;
    }

    bool isOuterChordNote = false;
    LayerElement *durElement = NULL;
    Chord *startParentChord = NULL;
    Chord *endParentChord = NULL;
    Layer *layer1 = NULL;
    if (note1) {
        durElement = note1;
        layer1 = note1->m_crossStaff ? note1->m_crossLayer : vrv_cast<Layer *>(note1->GetFirstAncestor(LAYER));
        startParentChord = note1->IsChordTone();
    }
    if (startParentChord) {
        durElement = startParentChord;
        if (startParentChord->m_crossStaff) layer1 = startParentChord->m_crossLayer;
    }
    if (note2) {
        endParentChord = note2->IsChordTone();
    }

    Point startPoint(x1, staff->GetDrawingY());
    Point endPoint(x2, staff->GetDrawingY());

    /************** note stem direction **************/

    data_STEMDIRECTION noteStemDir = STEMDIRECTION_NONE;
    switch (spanningType) {
        case SPANNING_START_END:
        case SPANNING_START:
        case SPANNING_END: {
            if (note1)
                noteStemDir = note1->GetDrawingStemDir();
            else if (note2)
                noteStemDir = note2->GetDrawingStemDir();
            break;
        }
        default: break;
    }

    /************** direction **************/

    bool isAboveStaffCenter = startPoint.y > (staff->GetDrawingY() - 4 * drawingUnit);
    curvature_CURVEDIR drawingCurveDir
        = this->GetPreferredCurveDirection(layer1, note1, startParentChord, noteStemDir, isAboveStaffCenter);
    if (startParentChord) {
        if (((curvature_CURVEDIR_above == drawingCurveDir) && (note1 == startParentChord->GetTopNote()))
            || ((curvature_CURVEDIR_below == drawingCurveDir) && (note1 == startParentChord->GetBottomNote()))) {
            isOuterChordNote = true;
        }
    }

    /************** x positions **************/

    const bool adjustVertically = this->CalculateXPosition(doc, staff, startParentChord, endParentChord, spanningType,
        isOuterChordNote, startPoint, endPoint, drawingCurveDir);

    /************** y position **************/

    const bool isGraceToNoteTie = (note1 && note2) && note1->IsGraceNote() && !note2->IsGraceNote();

    const int ySign = (drawingCurveDir == curvature_CURVEDIR_above) ? 1 : -1;

    startPoint.y += ySign * drawingUnit / 2;
    endPoint.y += ySign * drawingUnit / 2;
    if (adjustVertically && !isGraceToNoteTie) {
        startPoint.y += ySign * drawingUnit;
        endPoint.y += ySign * drawingUnit;
    }

    /************** bezier points **************/

    // adjust the 'height' of the bezier based on the width of staff lines to make sure that the tie does not overlap
    // with them
    const int height = (1.6 - doc->GetOptions()->m_staffLineWidth.GetValue()) * drawingUnit;
    const int distance = endPoint.x - startPoint.x;

    // control points
    Point c1, c2;

    c1.x = startPoint.x + distance / 4; // point at 1/4
    c2.x = startPoint.x + distance / 4 * 3; // point at 3/4

    c1.y = startPoint.y + ySign * height;
    c2.y = endPoint.y + ySign * height;

    // Point bezier[4];
    bezier[0] = startPoint;
    bezier[1] = c1;
    bezier[2] = c2;
    bezier[3] = endPoint;

    assert(this->GetCurrentFloatingPositioner());
    FloatingPositioner *positioner = this->GetCurrentFloatingPositioner();
    assert(positioner && positioner->Is(FLOATING_CURVE_POSITIONER));
    FloatingCurvePositioner *curve = vrv_cast<FloatingCurvePositioner *>(positioner);
    assert(curve);

    const int thickness = drawingUnit * doc->GetOptions()->m_tieMidpointThickness.GetValue();
    curve->UpdateCurveParams(bezier, thickness, drawingCurveDir);

    if ((!startParentChord || isOuterChordNote) && durElement && (spanningType != SPANNING_END)) {
        this->UpdateTiePositioning(curve, bezier, durElement, note1, drawingUnit, drawingCurveDir);
        curve->UpdateCurveParams(bezier, thickness, drawingCurveDir);
    }
    if (!startParentChord && !endParentChord && note1 && note2 && (spanningType == SPANNING_START_END)) {
        if (this->AdjustEnharmonicTies(doc, curve, bezier, note1, note2, drawingCurveDir)) {
            curve->UpdateCurveParams(bezier, thickness, drawingCurveDir);
        }
    }

    return true;
}

int Tie::CalculateAdjacentChordXOffset(const Doc *doc, const Staff *staff, const Chord *parentChord, const Note *note,
    curvature_CURVEDIR drawingCurveDir, int initialX, bool isStartPoint) const
{
    assert(parentChord);
    const int drawingUnit = doc->GetDrawingUnit(staff->m_drawingStaffSize);
    const int radius = note ? note->GetDrawingRadius(doc) : drawingUnit;

    // adjust starting point for the ties in chords with adjacent notes
    if (isStartPoint) {
        const int defaultX = initialX + (radius + drawingUnit / 2);
        // if stem is down adjacent notes are going to be on the left side of the stem, hence always take right side of
        // the chord
        if (parentChord->GetDrawingStemDir() == STEMDIRECTION_down) {
            if ((curvature_CURVEDIR_below == drawingCurveDir) && (note == parentChord->GetBottomNote())) {
                return defaultX;
            }
            const Stem *stem = parentChord->GetDrawingStem();
            if (stem && !stem->IsVirtual()) {
                return stem->GetContentRight() + 2 * radius + drawingUnit / 2;
            }
            else {
                return parentChord->GetContentRight() + drawingUnit / 2;
            }
        }
        else {
            if (!note) return defaultX;
            const std::list<const Note *> adjacentNotes
                = parentChord->GetAdjacentNotesList(staff, note->GetDrawingLoc());
            for (const auto adjacentNote : adjacentNotes) {
                if (adjacentNote->GetDrawingX() > note->GetDrawingX()) {
                    if ((drawingCurveDir == curvature_CURVEDIR_above)
                        && (note->GetDrawingLoc() < adjacentNote->GetDrawingLoc())) {
                        return parentChord->GetContentRight() + drawingUnit / 2;
                    }
                    else if ((drawingCurveDir == curvature_CURVEDIR_below)
                        && (note->GetDrawingLoc() > adjacentNote->GetDrawingLoc())) {
                        return parentChord->GetContentRight() + drawingUnit / 2;
                    }
                }
            }
            return defaultX;
        }
    }
    // adjust ending point for the ties in chords with adjacent notes
    else {
        const int defaultX = initialX - (radius + drawingUnit / 2);
        // similar to the starting point - when stem direction is up, all adjacent notes are on the right, so take left
        // side of the chord
        if (parentChord->GetDrawingStemDir() == STEMDIRECTION_up) {
            if ((curvature_CURVEDIR_above == drawingCurveDir) && (note == parentChord->GetTopNote())) {
                return defaultX;
            }
            const Stem *stem = parentChord->GetDrawingStem();
            if (stem && !stem->IsVirtual()) {
                return stem->GetContentLeft() - 2 * radius - drawingUnit / 2;
            }
            else {
                return parentChord->GetContentLeft() - drawingUnit / 2;
            }
        }
        else {
            if (!note) return defaultX;
            const std::list<const Note *> adjacentNotes
                = parentChord->GetAdjacentNotesList(staff, note->GetDrawingLoc());
            for (const auto adjacentNote : adjacentNotes) {
                if (adjacentNote->GetDrawingX() < note->GetDrawingX()) {
                    if ((drawingCurveDir == curvature_CURVEDIR_above)
                        && (note->GetDrawingLoc() < adjacentNote->GetDrawingLoc())) {
                        return parentChord->GetContentLeft() - drawingUnit / 2;
                    }
                    else if ((drawingCurveDir == curvature_CURVEDIR_below)
                        && (note->GetDrawingLoc() > adjacentNote->GetDrawingLoc())) {
                        return parentChord->GetContentLeft() - drawingUnit / 2;
                    }
                }
            }
            return defaultX;
        }
    }
}

bool Tie::CalculateXPosition(const Doc *doc, const Staff *staff, const Chord *startParentChord,
    const Chord *endParentChord, int spanningType, bool isOuterChordNote, Point &startPoint, Point &endPoint,
    curvature_CURVEDIR drawingCurveDir) const
{
    const Note *startNote = dynamic_cast<const Note *>(this->GetStart());
    const Note *endNote = dynamic_cast<const Note *>(this->GetEnd());
    const int r1 = startNote ? startNote->GetDrawingRadius(doc) : 0;
    const int r2 = endNote ? endNote->GetDrawingRadius(doc) : 0;

    // Vertical correction cannot be applied for chords
    const int drawingUnit = doc->GetDrawingUnit(staff->m_drawingStaffSize);
    const double minTieLength = doc->GetOptions()->m_tieMinLength.GetValue();
    const bool isShortTie = (endPoint.x - startPoint.x < (1 + minTieLength) * drawingUnit + r1 + r2);
    const bool adjustVertically = !startParentChord && !endParentChord && isShortTie;

    // the normal case
    if (spanningType == SPANNING_START_END) {
        if (startNote) {
            startPoint.y = startNote->GetDrawingY();
            endPoint.y = startPoint.y;
        }
        else if (endNote) {
            endPoint.y = endNote->GetDrawingY();
            startPoint.y = endPoint.y;
        }
        // isShort is never true with tstamp1
        if (!adjustVertically) {
            // startPoint
            if (startParentChord && startParentChord->HasAdjacentNotesInStaff(staff)) {
                startPoint.x = this->CalculateAdjacentChordXOffset(
                    doc, staff, startParentChord, startNote, drawingCurveDir, startPoint.x, true);
            }
            else {
                startPoint.x += r1 + drawingUnit / 2;
            }
            // endPoint
            const Staff *endStaff = staff;
            if (endParentChord) endStaff = endParentChord->GetAncestorStaff();
            if (endParentChord && endParentChord->HasAdjacentNotesInStaff(endStaff)) {
                endPoint.x = this->CalculateAdjacentChordXOffset(
                    doc, endStaff, endParentChord, endNote, drawingCurveDir, endPoint.x, false);
            }
            else {
                endPoint.x -= r2 + drawingUnit / 2;
            }
        }
        else {
            // Prevent collisions with articulations
            if (startNote && startNote->FindDescendantByType(ARTIC)) {
                startPoint.x += r1;
            }
            if (endNote && endNote->FindDescendantByType(ARTIC)) {
                endPoint.x -= r2;
            }
        }
        // Prevent collisions with dots
        if (startParentChord && !isOuterChordNote && (startParentChord->GetDots() > 0)) {
            if (isShortTie) {
                startPoint.x += drawingUnit;
            }
            else {
                const Dots *dots = vrv_cast<const Dots *>(startParentChord->FindDescendantByType(DOTS));
                assert(dots);
                startPoint.x = dots->GetDrawingX() + (1 + startParentChord->GetDots()) * drawingUnit;
            }
        }
    }
    // This is the case when the tie is split over two system of two pages.
    // In this case, we are now drawing its beginning to the end of the measure (i.e., the last aligner)
    else if (spanningType == SPANNING_START) {
        if (startNote) {
            startPoint.y = startNote->GetDrawingY();
            endPoint.y = startPoint.y;
        }
        if (!adjustVertically) {
            if (startParentChord && startParentChord->HasAdjacentNotesInStaff(staff)) {
                startPoint.x = this->CalculateAdjacentChordXOffset(
                    doc, staff, startParentChord, startNote, drawingCurveDir, startPoint.x, true);
            }
            else {
                startPoint.x += r1 + drawingUnit / 2;
            }
            if (startNote && startNote->GetDots() > 0) {
                startPoint.x += drawingUnit * startNote->GetDots() * 3 / 2;
            }
            else if (startParentChord && (startParentChord->GetDots() > 0)) {
                startPoint.x += 2 * drawingUnit * startParentChord->GetDots();
            }
        }
        else {
            // Prevent collisions with articulations
            if (startNote && startNote->FindDescendantByType(ARTIC)) {
                startPoint.x += r1;
            }
        }
        // Prevent collisions with dots
        if (startParentChord && !isOuterChordNote && (startParentChord->GetDots() > 0)) {
            const Dots *dots = vrv_cast<const Dots *>(startParentChord->FindDescendantByType(DOTS));
            assert(dots);
            startPoint.x = dots->GetDrawingX() + (1 + startParentChord->GetDots()) * drawingUnit;
        }
        endPoint.x -= (drawingUnit + doc->GetDrawingBarLineWidth(staff->m_drawingStaffSize)) / 2;
    }
    // Now this is the case when the tie is split but we are drawing the end of it
    else if (spanningType == SPANNING_END) {
        if (endNote) {
            endPoint.y = endNote->GetDrawingY();
            startPoint.y = endPoint.y;
        }
        if (!adjustVertically) {
            // endPoint
            const Staff *endStaff = staff;
            if (endParentChord) endStaff = endParentChord->GetAncestorStaff();
            if (endParentChord && endParentChord->HasAdjacentNotesInStaff(endStaff)) {
                endPoint.x = this->CalculateAdjacentChordXOffset(
                    doc, endStaff, endParentChord, endNote, drawingCurveDir, endPoint.x, false);
            }
            else {
                endPoint.x -= r2 + drawingUnit / 2;
            }
        }
        else {
            // Prevent collisions with articulations
            if (endNote && endNote->FindDescendantByType(ARTIC)) {
                endPoint.x -= r2;
            }
        }
    }

    return adjustVertically;
}

curvature_CURVEDIR Tie::GetPreferredCurveDirection(const Layer *layer, const Note *note, const Chord *startParentChord,
    data_STEMDIRECTION noteStemDir, bool isAboveStaffCenter) const
{
    data_STEMDIRECTION layerStemDir;
    curvature_CURVEDIR drawingCurveDir = curvature_CURVEDIR_above;
    // first should be the tie @curvedir
    if (this->HasCurvedir()) {
        drawingCurveDir
            = (this->GetCurvedir() == curvature_CURVEDIR_above) ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
    }
    // then layer direction trumps note direction
    else if (layer && ((layerStemDir = layer->GetDrawingStemDir(note)) != STEMDIRECTION_NONE)) {
        drawingCurveDir = (layerStemDir == STEMDIRECTION_up) ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
    }
    // look if in a chord
    else if (startParentChord) {
        if (startParentChord->PositionInChord(note) < 0) {
            drawingCurveDir = curvature_CURVEDIR_below;
        }
        else if (startParentChord->PositionInChord(note) > 0) {
            drawingCurveDir = curvature_CURVEDIR_above;
        }
        // away from the stem if odd number (center note)
        else {
            drawingCurveDir = (noteStemDir != STEMDIRECTION_up) ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
        }
    }
    else if (noteStemDir == STEMDIRECTION_up) {
        drawingCurveDir = curvature_CURVEDIR_below;
    }
    else if (noteStemDir == STEMDIRECTION_NONE) {
        // no information from the note stem directions, look at the position in the notes
        drawingCurveDir = isAboveStaffCenter ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
    }

    return drawingCurveDir;
}

void Tie::UpdateTiePositioning(const FloatingCurvePositioner *curve, Point bezier[4], const LayerElement *durElement,
    const Note *startNote, int drawingUnit, curvature_CURVEDIR drawingCurveDir) const
{
    ListOfConstObjects objects;
    ClassIdsComparison cmp({ DOT, DOTS, FLAG });
    durElement->FindAllDescendantsByComparison(&objects, &cmp);

    int adjust = 0;
    int dotsPosition = 0;
    for (const Object *object : objects) {
        if (!object->HasSelfBB()) continue;
        // if we have possible overlap with dots, we need to move tie up/down to avoid it. This happens only for the
        // outer ties, so there should be no issue of inner tie moving up and colliding with other elements
        if (object->Is(DOTS)) {
            bool discard = false;
            // initial margin is non-zero to make sure that we adjust ties that are very close to the dots, but don't
            // overlap with them
            int margin = 25;
            // overlap with elements opposite to direction of the tie; this is needed to make sure there is no overlap
            // with dots BB that is on the curving side of the tie. Ideally this should be changed when each dot in the
            // chord gets separate BB
            int oppositeOverlap = 0;
            // calculate position for the tie in case there is overlap with FLAG in the future
            dotsPosition = object->GetDrawingX()
                + (1 + dynamic_cast<const AttAugmentDots *>(durElement)->GetDots()) * drawingUnit;
            if (durElement->Is(CHORD)) {
                // If this is chord, we need to make sure that ties are compared against relative dot. Since all dots
                // have one BB and this action is done for outer ties only, we can safely take height of the BB to
                // determine margin for adjustment calculation
                const Chord *parentChord = vrv_cast<const Chord *>(durElement);
                int offset = (object->GetSelfRight() - object->GetSelfLeft()) / parentChord->GetDots();
                if ((drawingCurveDir == curvature_CURVEDIR_above) && (startNote != parentChord->GetTopNote())) {
                    margin = object->GetSelfBottom() - object->GetSelfTop() + offset;
                }
                else if ((drawingCurveDir == curvature_CURVEDIR_below) && (startNote != parentChord->GetBottomNote())) {
                    margin = object->GetSelfBottom() - object->GetSelfTop() - offset;
                }
                const int overlap = curve->CalcAdjustment(object, discard);
                if ((overlap > 0) && (overlap < 1.5 * offset)) {
                    oppositeOverlap = overlap;
                }
            }
            // calculate intersection with curve based on margin
            const int step = drawingUnit / 2;
            int intersection = curve->CalcAdjustment(object, discard, margin, false);
            if (intersection) {
                intersection = (intersection / step + 1) * step + 0.5 * step;
                intersection *= (drawingCurveDir == curvature_CURVEDIR_below) ? -1 : 1;
            }
            else if (oppositeOverlap) {
                intersection = (oppositeOverlap / step) * step * 0.5;
            }
            else {
                continue;
            }

            if (std::abs(intersection) > std::abs(adjust)) adjust = intersection;
        }
        // In case there is overlap with flag, we need to move starting point to the side, to avoid it
        else if (object->Is(FLAG)) {
            bool discard = false;
            int intersection = curve->CalcAdjustment(object, discard);
            if (intersection != 0) {
                // in case there is an overlap with flag and dots are present - shift tie starting point to the right of
                // the dot, otherwise shift it just enough to avoid overlapping with flag
                if (!dotsPosition) {
                    bezier[0].x += drawingUnit;
                }
                else {
                    bezier[0].x = dotsPosition;
                }
                const int controlPointDist = (bezier[3].x - bezier[0].x) / 4;
                bezier[1].x = bezier[0].x + controlPointDist;
                bezier[2].x = bezier[3].x - controlPointDist;
            }
        }
    }
    if (adjust) {
        for (int i = 0; i < 4; ++i) {
            bezier[i].y += adjust;
        }
    }
}
//----------------------------------------------------------------------------
// Tie functor methods
//----------------------------------------------------------------------------

FunctorCode Tie::Accept(Functor &functor)
{
    return functor.VisitTie(this);
}

FunctorCode Tie::Accept(ConstFunctor &functor) const
{
    return functor.VisitTie(this);
}

FunctorCode Tie::AcceptEnd(Functor &functor)
{
    return functor.VisitTieEnd(this);
}

FunctorCode Tie::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitTieEnd(this);
}

} // namespace vrv
