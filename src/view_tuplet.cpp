/////////////////////////////////////////////////////////////////////////////
// Name:        view_tuplet.cpp
// Author:      Rodolfo Zitellini
// Created:     21/08/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "beam.h"
#include "devicecontext.h"
#include "doc.h"
#include "note.h"
#include "smufl.h"
#include "staff.h"
#include "style.h"
#include "tuplet.h"

namespace vrv {

#define TUPLET_OFFSET (25 * DEFINITION_FACTOR)

/**
 * Analyze a tuplet object and figure out if all the notes are in the same beam
 * or not
 */
bool View::OneBeamInTuplet(Tuplet *tuplet)
{
    assert(tuplet);

    Beam *currentBeam = NULL;
    ArrayOfObjects elems;

    // Are we contained in a beam?
    if (tuplet->GetFirstParent(BEAM, MAX_BEAM_DEPTH) && (tuplet->GetNoteCount() != 0)) return true;

    // No we contain a beam? Go on and search for it in the children
    for (int i = 0; i < tuplet->GetChildCount(); i++) {
        currentBeam = dynamic_cast<Beam *>(tuplet->GetChild(i));

        // first child is not a beam, or it is a beam but we have more than one child
        if (!currentBeam || tuplet->GetChildCount() > 1) {
            return false;
        }
    }

    return true;
}

/**
 * This function gets the tuplet coords for drawing the bracket and number
 * @param tuplet - the tuplet object
 * @param layer - layer obj
 * @param start, end, center - these are the coordinates returned
 * @return the direction of the beam
 *
 * We can divide the tuplets in three types:
 * 1) All notes beamed
 * 2) All notes unbeamed
 * 3) a mixture of the above
 *
 * The first type is the simplest to calculate, as we just need the
 * start and end of the beam
 * types 2 and 3 are treated in the same manner to calculate the points:
 * - if all the stems are in the same direction, the bracket goes from the
 *   first to the last stem and the number is centered. If a stem in the
 *   middle is longer than the first or last, the y positions are offsetted
 *   accordingly to evitate collisions
 * - if stems go in two different directions, the bracket and number are
 *   placed in the side that has more stems in that direction. If the
 *   stems are equal, if goes up. In this case the bracket is horizontal
 *   so we just need the tallnes of the most tall stem. If a notehead
 *   is lower (or upper) than this stem, we compensate that too with an offset

 */

data_STEMDIRECTION View::GetTupletCoordinates(Tuplet *tuplet, Layer *layer, Point *start, Point *end, Point *center)
{
    assert(tuplet);
    assert(layer);
    assert(start);
    assert(end);
    assert(center);

    Point first, last;
    int x, y;
    data_STEMDIRECTION direction = STEMDIRECTION_up;

    ListOfObjects *tupletChildren = tuplet->GetList(tuplet);
    LayerElement *firstElement = dynamic_cast<LayerElement *>(tupletChildren->front());
    LayerElement *lastElement = dynamic_cast<LayerElement *>(tupletChildren->back());

    // There are unbeamed notes of two different beams
    // treat all the notes as unbeamed
    int ups = 0, downs = 0; // quantity of up- and down-stems

    // In this case use the center of the notehead to calculate the exact center
    // as it looks better
    x = firstElement->GetDrawingX()
        + (lastElement->GetDrawingX() - firstElement->GetDrawingX() + lastElement->GetSelfX2()) / 2;

    // Return the start and end position for the brackes
    // starting from the first edge and last of the BBoxes
    start->x = firstElement->GetSelfX1() + firstElement->GetDrawingX();
    end->x = lastElement->GetSelfX2() + lastElement->GetDrawingX();

    // The first step is to calculate all the stem directions
    // cycle into the elements and count the up and down dirs
    ListOfObjects::iterator iter = tupletChildren->begin();
    while (iter != tupletChildren->end()) {
        if ((*iter)->Is(NOTE)) {
            Note *currentNote = dynamic_cast<Note *>(*iter);
            assert(currentNote);
            if (currentNote->GetDrawingStemDir() == STEMDIRECTION_up)
                ups++;
            else
                downs++;
        }
        ++iter;
    }
    // true means up
    direction = ups > downs ? STEMDIRECTION_up : STEMDIRECTION_down;

    // if ups or downs are 0, it means all the stems go in the same direction
    if (ups == 0 || downs == 0) {

        Note *firstNote = dynamic_cast<Note *>(tuplet->FindChildByType(NOTE));
        Note *lastNote = dynamic_cast<Note *>(tuplet->FindChildByType(NOTE, UNLIMITED_DEPTH, BACKWARD));

        // Calculate the average between the first and last stem
        // set center, start and end too.
        y = firstElement->GetDrawingY();
        if (firstNote && lastNote) {
            if (direction == STEMDIRECTION_up) { // up
                y = lastNote->GetDrawingStemEnd(lastNote).y
                    + (firstNote->GetDrawingStemEnd(firstNote).y - lastNote->GetDrawingStemEnd(lastNote).y) / 2
                    + TUPLET_OFFSET;
                start->y = firstNote->GetDrawingStemEnd(firstNote).y + TUPLET_OFFSET;
                end->y = lastNote->GetDrawingStemEnd(lastNote).y + TUPLET_OFFSET;
            }
            else {
                y = lastNote->GetDrawingStemEnd(lastNote).y
                    + (firstNote->GetDrawingStemEnd(firstNote).y - lastNote->GetDrawingStemEnd(lastNote).y) / 2
                    - TUPLET_OFFSET;
                start->y = firstNote->GetDrawingStemEnd(firstNote).y - TUPLET_OFFSET;
                end->y = lastNote->GetDrawingStemEnd(lastNote).y - TUPLET_OFFSET;
            }
        }

        // Now we cycle again in all the intermediate notes (i.e. we start from the second note
        // and stop at last -1)
        // We will see if the position of the note is more (or less for down stems) of the calculated
        // average. In this case we offset down or up all the points
        iter = tupletChildren->begin();
        while (iter != tupletChildren->end()) {
            if ((*iter)->Is(NOTE)) {
                Note *currentNote = dynamic_cast<Note *>(*iter);
                assert(currentNote);

                if (direction == STEMDIRECTION_up) {
                    // The note is more than the avg, adjust to y the difference
                    // from this note to the avg
                    if (currentNote->GetDrawingStemEnd(currentNote).y + TUPLET_OFFSET > y) {
                        int offset = y - (currentNote->GetDrawingStemEnd(currentNote).y + TUPLET_OFFSET);
                        y -= offset;
                        end->y -= offset;
                        start->y -= offset;
                    }
                }
                else {
                    if (currentNote->GetDrawingStemEnd(currentNote).y - TUPLET_OFFSET < y) {
                        int offset = y - (currentNote->GetDrawingStemEnd(currentNote).y - TUPLET_OFFSET);
                        y -= offset;
                        end->y -= offset;
                        start->y -= offset;
                    }
                }
            }
            ++iter;
        }
    }
    else {
        // two-directional beams
        // this case is similar to the above, but the bracket is only horizontal
        // y is 0 because the final y pos is above the tallest stem
        y = 0;

        // Find the tallest stem and set y to it (with the offset distance)
        iter = tupletChildren->begin();
        while (iter != tupletChildren->end()) {
            if ((*iter)->Is(NOTE)) {
                Note *currentNote = dynamic_cast<Note *>(*iter);
                assert(currentNote);

                if (currentNote->GetDrawingStemDir() == direction) {
                    if (direction == STEMDIRECTION_up) {
                        if (y == 0 || currentNote->GetDrawingStemEnd(currentNote).y + TUPLET_OFFSET >= y)
                            y = currentNote->GetDrawingStemEnd(currentNote).y + TUPLET_OFFSET;
                    }
                    else {
                        if (y == 0 || currentNote->GetDrawingStemEnd(currentNote).y - TUPLET_OFFSET <= y)
                            y = currentNote->GetDrawingStemEnd(currentNote).y - TUPLET_OFFSET;
                    }
                }
                else {
                    // do none for now
                    // but if a notehead with a reversed stem is taller that the last
                    // calculated y, we need to offset
                }
            }
            ++iter;
        }

        // end and start are on the same line (and so il center when set later)
        end->y = start->y = y;
    }

    center->x = x;
    center->y = y;
    return direction;
}

void View::DrawTupletPostponed(DeviceContext *dc, Tuplet *tuplet, Layer *layer, Staff *staff)
{
    assert(dc);
    assert(tuplet);
    assert(layer);
    assert(staff);

    if ((tuplet->GetBracketVisible() == BOOLEAN_false) && (tuplet->GetNumVisible() == BOOLEAN_false)) {
        tuplet->SetEmptyBB();
        return;
    }

    tuplet->ResetList(tuplet);

    TextExtend extend;
    std::wstring notes;

    bool drawingCueSize = tuplet->IsCueSize();

    dc->SetFont(m_doc->GetDrawingSmuflFont(staff->m_drawingStaffSize, drawingCueSize));

    if (tuplet->GetNum() > 0) {
        notes = IntToTupletFigures((short int)tuplet->GetNum());
        dc->GetSmuflTextExtent(notes, &extend);
    }

    Point start, end, center;
    data_STEMDIRECTION direction = GetTupletCoordinates(tuplet, layer, &start, &end, &center);

    // Calculate position for number 0x82
    // since the number is slanted, move the center left
    // by 4 pixels so it seems more centered to the eye
    int txt_x = center.x - (extend.m_width / 2);
    // we need to move down the figure of half of it height, which is about an accid width;
    // also, cue size is not supported. Does it has to?
    int txt_y = center.y - m_doc->GetGlyphWidth(SMUFL_E262_accidentalSharp, staff->m_drawingStaffSize, drawingCueSize);

    if (tuplet->GetNum() && (tuplet->GetNumVisible() != BOOLEAN_false)) {
        DrawSmuflString(dc, txt_x, txt_y, notes, false, staff->m_drawingStaffSize);
    }

    dc->ResetFont();

    // Nothing to do if the bracket is not visible
    if (tuplet->GetBracketVisible() == BOOLEAN_false) {
        return;
    }
    // If all tuplets beamed draw no bracket by default
    if (OneBeamInTuplet(tuplet) && !(tuplet->GetBracketVisible() == BOOLEAN_true)) {
        return;
    }

    int verticalLine = m_doc->GetDrawingUnit(100);

    dc->SetPen(m_currentColour, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize), AxSOLID);

    // Draw the bracket, interrupt where the number is

    // get the slope
    double m = (double)(start.y - end.y) / (double)(start.x - end.x);

    // x = 10 pixels before the number
    int x = txt_x - 40;
    // xa = just after, the number is abundant so I do not add anything
    int xa = txt_x + extend.m_width + 20;

    // calculate the y coords in the slope
    double y1 = (double)start.y + m * (x - (double)start.x);
    double y2 = (double)start.y + m * (xa - (double)start.x);

    if (tuplet->GetNumVisible() == BOOLEAN_false) {
        // one single line
        dc->DrawLine(start.x, ToDeviceContextY(start.y), end.x, ToDeviceContextY(end.y));
    }
    else {
        // first line
        dc->DrawLine(start.x, ToDeviceContextY(start.y), (int)x, ToDeviceContextY((int)y1));
        // second line after gap
        dc->DrawLine((int)xa, ToDeviceContextY((int)y2), end.x, ToDeviceContextY(end.y));
    }

    // vertical bracket lines
    if (direction == STEMDIRECTION_up) {
        dc->DrawLine(start.x, ToDeviceContextY(start.y), start.x, ToDeviceContextY(start.y - verticalLine));
        dc->DrawLine(end.x, ToDeviceContextY(end.y), end.x, ToDeviceContextY(end.y - verticalLine));
    }
    else {
        dc->DrawLine(start.x, ToDeviceContextY(start.y), start.x, ToDeviceContextY(start.y + verticalLine));
        dc->DrawLine(end.x, ToDeviceContextY(end.y), end.x, ToDeviceContextY(end.y + verticalLine));
    }

    dc->ResetPen();
}

} // namespace vrv
