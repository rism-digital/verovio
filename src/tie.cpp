/////////////////////////////////////////////////////////////////////////////
// Name:        tie.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tie.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "chord.h"
#include "comparison.h"
#include "doc.h"
#include "dot.h"
#include "elementpart.h"
#include "functorparams.h"
#include "layer.h"
#include "note.h"
#include "slur.h"
#include "staff.h"

namespace vrv {

//----------------------------------------------------------------------------
// Tie
//----------------------------------------------------------------------------

static const ClassRegistrar<Tie> s_factory("tie", TIE);

Tie::Tie() : ControlElement("tie-"), TimeSpanningInterface(), AttColor(), AttCurvature(), AttCurveRend()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_CURVATURE);
    RegisterAttClass(ATT_CURVEREND);

    Reset();
}

Tie::Tie(const std::string &classid)
    : ControlElement(classid), TimeSpanningInterface(), AttColor(), AttCurvature(), AttCurveRend()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_CURVATURE);
    RegisterAttClass(ATT_CURVEREND);

    Reset();
}

Tie::~Tie() {}

void Tie::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    ResetColor();
    ResetCurvature();
    ResetCurveRend();
}

bool Tie::CalculatePosition(Doc *doc, Staff *staff, int x1, int x2, int spanningType, Point bezier[4])
{
    if (!doc || !staff) return false;

    const int drawingUnit = doc->GetDrawingUnit(staff->m_drawingStaffSize);
    /************** parent layers **************/

    if ((spanningType != SPANNING_START_END) && (spanningType != SPANNING_START) && (spanningType != SPANNING_END)) {
        //  this make no sense
        LogDebug("Tie across an entire system is not supported");
        return false;
    }

    Note *note1 = dynamic_cast<Note *>(GetStart());
    Note *note2 = dynamic_cast<Note *>(GetEnd());

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
        layer1 = note1->m_crossStaff ? note1->m_crossLayer : dynamic_cast<Layer *>(note1->GetFirstAncestor(LAYER));
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
        = GetPreferredCurveDirection(layer1, note1, startParentChord, noteStemDir, isAboveStaffCenter);
    if (startParentChord) {
        if (((curvature_CURVEDIR_above == drawingCurveDir) && (note1 == startParentChord->GetTopNote()))
            || ((curvature_CURVEDIR_below == drawingCurveDir) && (note1 == startParentChord->GetBottomNote()))) {
            isOuterChordNote = true;
        }
    }

    /************** x positions **************/

    CalculateXPosition(
        doc, staff, startParentChord, endParentChord, spanningType, isOuterChordNote, startPoint, endPoint);

    /************** y position **************/

    // shortTie correction cannot be applied for chords
    const bool isShortTie = !startParentChord && !endParentChord && (endPoint.x - startPoint.x < 4 * drawingUnit);

    const int ySign = (drawingCurveDir == curvature_CURVEDIR_above) ? 1 : -1;

    startPoint.y += ySign * drawingUnit / 2;
    endPoint.y += ySign * drawingUnit / 2;
    if (isShortTie) {
        startPoint.y += ySign * drawingUnit;
        endPoint.y += ySign * drawingUnit;
    }

    /************** bezier points **************/

    // the 'height' of the bezier
    int height = drawingUnit;
    // if the space between the to points is more than two staff height, increase the height
    if (endPoint.x - startPoint.x > 2 * doc->GetDrawingStaffSize(staff->m_drawingStaffSize)) {
        height += drawingUnit;
    }

    // control points
    Point c1, c2;
    // the height of the control points
    height *= 4 / 3;

    c1.x = startPoint.x + (endPoint.x - startPoint.x) / 4; // point at 1/4
    c2.x = startPoint.x + (endPoint.x - startPoint.x) / 4 * 3; // point at 3/4

    c1.y = startPoint.y + ySign * height;
    c2.y = endPoint.y + ySign * height;

    // Point bezier[4];
    bezier[0] = startPoint;
    bezier[1] = c1;
    bezier[2] = c2;
    bezier[3] = endPoint;

    assert(GetCurrentFloatingPositioner());
    FloatingPositioner *positioner = GetCurrentFloatingPositioner();
    assert(positioner && positioner->Is(FLOATING_CURVE_POSITIONER));
    FloatingCurvePositioner *curve = vrv_cast<FloatingCurvePositioner *>(positioner);
    assert(curve);

    const int thickness = drawingUnit * doc->GetOptions()->m_tieMidpointThickness.GetValue();
    curve->UpdateCurveParams(bezier, 0.0, thickness, drawingCurveDir);

    if ((!startParentChord || isOuterChordNote) && durElement && (spanningType != SPANNING_END)) {
        UpdateTiePositioning(curve, bezier, durElement, note1, drawingUnit, drawingCurveDir);
        curve->UpdateCurveParams(bezier, 0.0, thickness, drawingCurveDir);
    }

    return true;
}

void Tie::CalculateXPosition(Doc *doc, Staff *staff, Chord *startParentChord, Chord *endParentChord, int spanningType,
    bool isOuterChordNote, Point &startPoint, Point &endPoint)
{
    Note *startNote = dynamic_cast<Note *>(GetStart());
    Note *endNote = dynamic_cast<Note *>(GetEnd());

    const int drawingUnit = doc->GetDrawingUnit(staff->m_drawingStaffSize);
    bool isShortTie = false;
    // shortTie correction cannot be applied for chords
    if (!startParentChord && !endParentChord && (endPoint.x - startPoint.x < 4 * drawingUnit)) {
        isShortTie = true;
    }

    int r1 = drawingUnit;
    int r2 = r1;
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
        if (!isShortTie) {
            if (startNote) r1 = startNote->GetDrawingRadius(doc);
            if (endNote) r2 = endNote->GetDrawingRadius(doc);
            startPoint.x += r1 + drawingUnit / 2;
            endPoint.x -= r2 + drawingUnit / 2;
        }
        if (startParentChord && !isOuterChordNote && (startParentChord->GetDots() > 0)) {
            if ((endPoint.x - startPoint.x) <= 4 * drawingUnit) {
                startPoint.x += drawingUnit;
            }
            else {
                Dots *dots = vrv_cast<Dots *>(startParentChord->FindDescendantByType(DOTS));
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
            r1 = startNote->GetDrawingRadius(doc);
        }
        if (!isShortTie) {
            startPoint.x += r1 + drawingUnit / 2;
            if (startNote && startNote->GetDots() > 0) {
                startPoint.x += drawingUnit * startNote->GetDots() * 3 / 2;
            }
            else if (startParentChord && (startParentChord->GetDots() > 0)) {
                startPoint.x += 2 * drawingUnit * startParentChord->GetDots();
            }
        }
        if (startParentChord && !isOuterChordNote && (startParentChord->GetDots() > 0)) {
            Dots *dots = vrv_cast<Dots *>(startParentChord->FindDescendantByType(DOTS));
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
            r2 = endNote->GetDrawingRadius(doc);
        }
        if (!isShortTie) {
            endPoint.x -= r2 + drawingUnit / 2;
        }
    }
}

curvature_CURVEDIR Tie::GetPreferredCurveDirection(
    Layer *layer, Note *note, Chord *startParentChord, data_STEMDIRECTION noteStemDir, bool isAboveStaffCenter)
{
    data_STEMDIRECTION layerStemDir;
    curvature_CURVEDIR drawingCurveDir = curvature_CURVEDIR_above;
    // first should be the tie @curvedir
    if (HasCurvedir()) {
        drawingCurveDir
            = (GetCurvedir() == curvature_CURVEDIR_above) ? curvature_CURVEDIR_above : curvature_CURVEDIR_below;
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

void Tie::UpdateTiePositioning(FloatingCurvePositioner *curve, Point bezier[4], LayerElement *durElement,
    Note *startNote, int drawingUnit, curvature_CURVEDIR drawingCurveDir)
{
    ListOfObjects objects;
    ClassIdsComparison cmp({ DOT, DOTS, FLAG });
    durElement->FindAllDescendantByComparison(&objects, &cmp);

    int adjust = 0;
    int dotsPosition = 0;
    for (auto object : objects) {
        if (!object->HasSelfBB()) continue;
        // if we have possible overlap with dots, we need to move tie up/down to avoid it. This happens only for the
        // outter ties, so there should be no issue of inner tie moving up and colliding with other elements
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
            dotsPosition
                = object->GetDrawingX() + (1 + dynamic_cast<AttAugmentDots *>(durElement)->GetDots()) * drawingUnit;
            if (durElement->Is(CHORD)) {
                // If this is chord, we need to make sure that ties is compared agains relative dot. Since all dots have
                // one BB and this action is done for outer ties only, we can safely take height of the BB to determine
                // margin for adjustment calculation
                Chord *parentChord = vrv_cast<Chord *>(durElement);
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
            else
                continue;

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

int Tie::ResolveMIDITies(FunctorParams *)
{
    Note *note1 = dynamic_cast<Note *>(this->GetStart());
    Note *note2 = dynamic_cast<Note *>(this->GetEnd());

    if (!note1 || !note2) {
        return FUNCTOR_CONTINUE;
    }

    double sttd2 = note2->GetScoreTimeTiedDuration();
    double std2 = note2->GetScoreTimeDuration();

    if (sttd2 > 0.0) {
        note1->SetScoreTimeTiedDuration(sttd2 + std2);
    }
    else {
        note1->SetScoreTimeTiedDuration(std2);
    }
    note2->SetScoreTimeTiedDuration(-1.0);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
