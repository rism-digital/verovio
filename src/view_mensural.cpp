/////////////////////////////////////////////////////////////////////////////
// Name:        view_mensural.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "devicecontext.h"
#include "doc.h"
#include "dot.h"
#include "elementpart.h"
#include "layer.h"
#include "ligature.h"
#include "mensur.h"
#include "note.h"
#include "options.h"
#include "plica.h"
#include "proport.h"
#include "smufl.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

thread_local int View::s_drawingLigX[2], View::s_drawingLigY[2]; // to keep coords. of ligatures
thread_local bool View::s_drawingLigObliqua = false; // mark the first pass for an oblique

//----------------------------------------------------------------------------
// View - Mensural
//----------------------------------------------------------------------------

void View::DrawMensuralNote(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Note *note = vrv_cast<Note *>(element);
    assert(note);

    const int yNote = element->GetDrawingY();
    const int xNote = element->GetDrawingX();
    const data_DURATION drawingDur = note->GetDrawingDur();

    /************** Noteheads: **************/

    // Ligature, maxima,longa, and brevis
    if (note->IsInLigature() && !m_options->m_ligatureAsBracket.GetValue()) {
        this->DrawLigatureNote(dc, element, layer, staff);
    }
    else if (drawingDur < DURATION_1) {
        this->DrawMaximaToBrevis(dc, yNote, element, layer, staff);
    }
    // Semibrevis and shorter
    else {
        char32_t code = note->GetMensuralNoteheadGlyph();
        dc->StartCustomGraphic("notehead");
        this->DrawSmuflCode(dc, xNote, yNote, code, staff->m_drawingStaffSize, false);
        dc->EndCustomGraphic();
    }

    /************ Draw children (verse / syl) ************/

    this->DrawLayerChildren(dc, note, layer, staff, measure);
}

void View::DrawMensur(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Mensur *mensur = vrv_cast<Mensur *>(element);
    assert(mensur);

    if (!mensur->HasSign() && !mensur->HasNum()) {
        // only react to visual attributes
        return;
    }

    int y = staff->GetDrawingY() - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * (staff->m_drawingLines - 1);
    int x = element->GetDrawingX();
    int perfectRadius = m_doc->GetGlyphWidth(SMUFL_E910_mensuralProlation1, staff->m_drawingStaffSize, false) / 2;
    int code = 0;

    if (mensur->HasLoc()) {
        y = staff->GetDrawingY()
            - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * (2 * staff->m_drawingLines - 2 - mensur->GetLoc());
    }
    else if (mensur->HasNumbase() && !mensur->HasNum()) {
        y += 2 * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    }

    if (mensur->GetSign() == MENSURATIONSIGN_O) {
        code = SMUFL_E911_mensuralProlation2;
    }
    else if (mensur->GetSign() == MENSURATIONSIGN_C) {
        if (mensur->GetOrient() == ORIENTATION_reversed) {
            code = SMUFL_E916_mensuralProlation7;
            // additional offset
            // perfectRadius -= 2 * perfectRadius - m_doc->GetGlyphWidth(SMUFL_E916_mensuralProlation7,
            // staff->m_drawingStaffSize, false);
        }
        else {
            code = SMUFL_E915_mensuralProlation6;
        }
    }

    dc->StartGraphic(element, "", element->GetID());

    this->DrawSmuflCode(dc, x, y, code, staff->m_drawingStaffSize, false);

    x += perfectRadius;
    // only one slash supported
    if (mensur->HasSlash()) {
        this->DrawSmuflCode(dc,
            x - m_doc->GetGlyphWidth(SMUFL_E925_mensuralProlationCombiningStroke, staff->m_drawingStaffSize, false) / 2,
            y, SMUFL_E925_mensuralProlationCombiningStroke, staff->m_drawingStaffSize, false);
    }
    if (mensur->GetDot() == BOOLEAN_true) {
        this->DrawSmuflCode(dc,
            x - m_doc->GetGlyphWidth(SMUFL_E920_mensuralProlationCombiningDot, staff->m_drawingStaffSize, false) / 2, y,
            SMUFL_E920_mensuralProlationCombiningDot, staff->m_drawingStaffSize, false);
    }

    if (mensur->HasNum()) {
        x = element->GetDrawingX();
        if (mensur->HasSign() || mensur->HasTempus()) {
            x += m_doc->GetDrawingUnit(staff->m_drawingStaffSize)
                * 6; // step forward because we have a sign or a meter symbol
        }
        int numbase = mensur->HasNumbase() ? mensur->GetNumbase() : 0;
        this->DrawProportFigures(dc, x, y, mensur->GetNum(), numbase, staff);
    }
    // It is sure we have a sign - draw the numbase underneath the sign
    else if (mensur->HasNumbase()) {
        // Draw a single figure but passing numbase - adjust the y accordingly
        y -= 4 * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        this->DrawProportFigures(dc, x, y, mensur->GetNumbase(), 0, staff);
    }

    dc->EndGraphic(element, this);
} // namespace vrv

/* This function draws any flags as well as the stem. */

void View::DrawMensuralStem(DeviceContext *dc, Note *note, Staff *staff, data_STEMDIRECTION dir, int xn, int originY)
{
    assert(note);

    const int staffSize = staff->m_drawingStaffSize;
    const data_DURATION drawingDur = note->GetDrawingDur();
    const int radius = note->GetDrawingRadius(m_doc);
    // Cue size is currently disabled
    const bool drawingCueSize = false;
    const bool mensural_black = (staff->m_drawingNotationType == NOTATIONTYPE_mensural_black);

    /* In black notation, the semiminima gets one flag; in white notation, it gets none.
        In both cases, as in CWMN, each shorter duration gets one additional flag. */
    const int nbFlags = (mensural_black ? drawingDur - DURATION_2 : drawingDur - DURATION_4);

    // SMuFL's mensural stems are not centered
    const int halfStemWidth
        = m_doc->GetGlyphWidth(SMUFL_E93E_mensuralCombStemUp, staff->m_drawingStaffSize, drawingCueSize) / 2;
    const int yOffset = m_doc->GetDrawingUnit(staffSize) - halfStemWidth;
    originY = (dir == STEMDIRECTION_up) ? originY + yOffset : originY - yOffset;

    // draw the stems and the flags
    char32_t code;
    if (dir == STEMDIRECTION_up) {
        switch (nbFlags) {
            case 1: code = SMUFL_E949_mensuralCombStemUpFlagSemiminima; break;
            case 2: code = SMUFL_E94B_mensuralCombStemUpFlagFusa; break;
            default: code = SMUFL_E93E_mensuralCombStemUp;
        }
    }
    else {
        switch (nbFlags) {
            case 1: code = SMUFL_E94A_mensuralCombStemDownFlagSemiminima; break;
            case 2: code = SMUFL_E94C_mensuralCombStemDownFlagFusa; break;
            default: code = SMUFL_E93F_mensuralCombStemDown;
        }
    }

    this->DrawSmuflCode(dc, xn + radius - halfStemWidth, originY, code, staff->m_drawingStaffSize, drawingCueSize);

    // Store the stem direction ?
    note->SetDrawingStemDir(dir);
}

void View::DrawMaximaToBrevis(DeviceContext *dc, int y, LayerElement *element, Layer *layer, Staff *staff)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);

    Note *note = vrv_cast<Note *>(element);
    assert(note);

    const bool isMensuralBlack = (staff->m_drawingNotationType == NOTATIONTYPE_mensural_black);
    const bool fillNotehead = (isMensuralBlack || note->GetColored()) && !(isMensuralBlack && note->GetColored());

    const int stemWidth = m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
    const int strokeWidth = 2.8 * stemWidth;
    const int staffSize = staff->m_drawingStaffSize;

    int shape = LIGATURE_DEFAULT;
    if (note->GetActualDur() != DURATION_breve) {
        bool up = false;
        // Mensural notes have no Stem child - rely on the MEI @stem.dir
        if (note->GetStemDir() != STEMDIRECTION_NONE) {
            up = (note->GetStemDir() == STEMDIRECTION_up);
        }
        // For CMN we rely on the drawing stem dir interface pre-calculated in functors
        else if (staff->m_drawingNotationType == NOTATIONTYPE_NONE
            || staff->m_drawingNotationType == NOTATIONTYPE_cmn) {
            up = (note->GetDrawingStemDir() == STEMDIRECTION_up);
        }
        // For mensural white, just calculate it here - keep it down for mensural black
        else if (staff->m_drawingNotationType != NOTATIONTYPE_mensural_black) {
            int verticalCenter = staff->GetDrawingY() - m_doc->GetDrawingUnit(staffSize) * (staff->m_drawingLines - 1);
            up = (note->GetDrawingY() < verticalCenter);
        }
        shape = (up) ? LIGATURE_STEM_RIGHT_UP : LIGATURE_STEM_RIGHT_DOWN;
    }

    Point topLeft, bottomRight;
    int sides[4];
    this->CalcBrevisPoints(note, staff, &topLeft, &bottomRight, sides, shape, isMensuralBlack);

    dc->StartCustomGraphic("notehead");

    if (!fillNotehead) {
        // double the bases of rectangles
        this->DrawObliquePolygon(
            dc, topLeft.x + stemWidth, topLeft.y, bottomRight.x - stemWidth, topLeft.y, -strokeWidth);
        this->DrawObliquePolygon(
            dc, topLeft.x + stemWidth, bottomRight.y, bottomRight.x - stemWidth, bottomRight.y, strokeWidth);
    }
    else {
        this->DrawFilledRectangle(dc, topLeft.x + stemWidth, topLeft.y, bottomRight.x - stemWidth, bottomRight.y);
    }

    if (note->FindDescendantByType(PLICA)) {
        // Right side is a stem - end the notehead first
        dc->EndCustomGraphic();
        return;
    }

    // serifs and / or stem
    this->DrawFilledRectangle(dc, topLeft.x, sides[0], topLeft.x + stemWidth, sides[1]);

    if (note->GetActualDur() != DURATION_breve) {
        // Right side is a stem - end the notehead first
        dc->EndCustomGraphic();
        dc->StartCustomGraphic("stem");
        this->DrawFilledRectangle(dc, bottomRight.x - stemWidth, sides[2], bottomRight.x, sides[3]);
        dc->EndCustomGraphic();
    }
    else {
        // Right side is a serif
        this->DrawFilledRectangle(dc, bottomRight.x - stemWidth, sides[2], bottomRight.x, sides[3]);
        dc->EndCustomGraphic();
    }

    return;
}

void View::DrawLigature(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);

    Ligature *ligature = vrv_cast<Ligature *>(element);
    assert(ligature);

    dc->StartGraphic(ligature, "", ligature->GetID());

    // Draw children (notes)
    this->DrawLayerChildren(dc, ligature, layer, staff, measure);

    // Render a bracket for the ligature
    if (m_options->m_ligatureAsBracket.GetValue()) {
        const ListOfObjects &notes = ligature->GetList();

        if (notes.size() > 0) {
            int y = staff->GetDrawingY();
            Note *firstNote = ligature->GetFirstNote();
            int x1 = firstNote->GetContentLeft();
            Note *lastNote = ligature->GetLastNote();
            int x2 = lastNote->GetContentRight();
            // Look for the highest note position in the ligature
            for (Object *object : notes) {
                Note *note = vrv_cast<Note *>(object);
                assert(note);
                y = std::max(y, note->GetContentTop());
            }
            int bracketSize = 2 * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
            // move the bracket up
            y += bracketSize + m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            int lineWidth = m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            this->DrawFilledRectangle(dc, x1, y, x1 + lineWidth, y - bracketSize);
            this->DrawFilledRectangle(dc, x1, y, x2, y - lineWidth);
            this->DrawFilledRectangle(dc, x2 - lineWidth, y, x2, y - bracketSize);
        }
    }

    dc->EndGraphic(ligature, this);
}

void View::DrawLigatureNote(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);

    Note *note = vrv_cast<Note *>(element);
    assert(note);

    Ligature *ligature = vrv_cast<Ligature *>(note->GetFirstAncestor(LIGATURE));
    assert(ligature);

    Note *prevNote = dynamic_cast<Note *>(ligature->GetListPrevious(note));
    Note *nextNote = dynamic_cast<Note *>(ligature->GetListNext(note));

    int position = ligature->GetListIndex(note);
    assert(position != -1);
    int shape = ligature->m_drawingShapes.at(position);
    int prevShape = (position > 0) ? ligature->m_drawingShapes.at(position - 1) : 0;

    /** code duplicated from View::DrawMaximaToBrevis */
    bool isMensuralBlack = (staff->m_drawingNotationType == NOTATIONTYPE_mensural_black);
    bool fillNotehead = (isMensuralBlack || note->GetColored()) && !(isMensuralBlack && note->GetColored());
    bool oblique = ((shape & LIGATURE_OBLIQUE) || (prevShape & LIGATURE_OBLIQUE));
    bool obliqueEnd = (prevShape & LIGATURE_OBLIQUE);
    bool stackedEnd = (shape & LIGATURE_STACKED);
    int stemWidth = m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
    int strokeWidth = 2.8 * stemWidth;
    /** end code duplicated */

    bool straight = true;
    switch (m_doc->GetOptions()->m_ligatureOblique.GetValue()) {
        case LIGATURE_OBL_auto: straight = !isMensuralBlack; break;
        case LIGATURE_OBL_straight: straight = true; break;
        case LIGATURE_OBL_curved: straight = false; break;
    }

    Point points[4];
    Point *topLeft = &points[0];
    Point *bottomLeft = &points[1];
    Point *topRight = &points[2];
    Point *bottomRight = &points[3];
    int sides[4];
    if (!oblique) {
        this->CalcBrevisPoints(note, staff, topLeft, bottomRight, sides, shape, isMensuralBlack);
        bottomLeft->x = topLeft->x;
        bottomLeft->y = bottomRight->y;
        topRight->x = bottomRight->x;
        topRight->y = topLeft->y;
    }
    else {
        // First half of the oblique - checking the nextNote is there just in case, but is should
        if ((shape & LIGATURE_OBLIQUE) && nextNote) {
            // return;
            this->CalcObliquePoints(note, nextNote, staff, points, sides, shape, isMensuralBlack, true, straight);
        }
        // Second half of the oblique - checking the prevNote is there just in case, but is should
        else if ((prevShape & LIGATURE_OBLIQUE) && prevNote) {
            this->CalcObliquePoints(prevNote, note, staff, points, sides, prevShape, isMensuralBlack, false, straight);
        }
        else {
            assert(false);
        }
    }

    // Oblique polygons
    if (straight) {
        if (!fillNotehead) {
            this->DrawObliquePolygon(dc, topLeft->x, topLeft->y, topRight->x, topRight->y, -strokeWidth);
            this->DrawObliquePolygon(dc, bottomLeft->x, bottomLeft->y, bottomRight->x, bottomRight->y, strokeWidth);
        }
        else {
            this->DrawObliquePolygon(dc, topLeft->x, topLeft->y, topRight->x, topRight->y, bottomLeft->y - topLeft->y);
        }
    }
    // Bent parallelograms
    else {
        const int thickness = topLeft->y - bottomLeft->y;
        // The curved side points (two ends and two control points)
        Point curvedSide[4];
        curvedSide[0] = this->ToDeviceContext(*topLeft);
        curvedSide[3] = this->ToDeviceContext(*topRight);
        //
        const int width = (curvedSide[3].x - curvedSide[0].x);
        const int height = (curvedSide[3].y - curvedSide[0].y);
        curvedSide[1] = curvedSide[3];
        curvedSide[1].x -= (width * 0.7);
        curvedSide[1].y -= (height * 0.7) + (height * 0.07);
        curvedSide[2] = curvedSide[3];
        curvedSide[2].x -= (width * 0.3);
        curvedSide[2].y -= (height * 0.3) + (height * 0.07);

        if (!fillNotehead) {
            dc->DrawBentParallelogramFilled(curvedSide, strokeWidth);
            for (Point &point : curvedSide) point.y += thickness - strokeWidth;
            dc->DrawBentParallelogramFilled(curvedSide, strokeWidth);
        }
        else {
            dc->DrawBentParallelogramFilled(curvedSide, thickness);
        }
    }

    // Do not draw a left connector with obliques
    if (!obliqueEnd) {
        int sideTop = sides[0];
        int sideBottom = sides[1];
        if (prevNote) {
            Point prevTopLeft = *topLeft;
            Point prevBottomRight = *bottomRight;
            int prevSides[4];
            memcpy(prevSides, sides, 4 * sizeof(int));
            this->CalcBrevisPoints(
                prevNote, staff, &prevTopLeft, &prevBottomRight, prevSides, prevShape, isMensuralBlack);
            if (!stackedEnd) {
                sideTop = std::max(sides[0], prevSides[2]);
                sideBottom = std::min(sides[1], prevSides[3]);
            }
            else {
                // Stacked end - simply use the bottom right [3] note since the interval is going up anyway
                sides[3] = prevSides[3];
            }
        }
        this->DrawFilledRoundedRectangle(dc, topLeft->x, sideTop, topLeft->x + stemWidth, sideBottom, stemWidth / 3);
    }

    if (!nextNote) {
        this->DrawFilledRoundedRectangle(
            dc, bottomRight->x - stemWidth, sides[2], bottomRight->x, sides[3], stemWidth / 3);
    }

    return;
}

void View::DrawDotInLigature(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);

    Dot *dot = vrv_cast<Dot *>(element);
    assert(dot);

    assert(dot->m_drawingPreviousElement && dot->m_drawingPreviousElement->Is(NOTE));
    Note *note = vrv_cast<Note *>(dot->m_drawingPreviousElement);
    assert(note);

    Ligature *ligature = vrv_cast<Ligature *>(note->GetFirstAncestor(LIGATURE));
    assert(ligature);

    double shiftMultiplier = 3.0;
    bool isVerticalDot = false;
    if (!m_doc->GetOptions()->m_ligatureAsBracket.GetValue()) {
        const int position = ligature->GetListIndex(note);
        assert(position != -1);
        const int shape = ligature->m_drawingShapes.at(position);
        const bool isLast = (position == (int)ligature->m_drawingShapes.size() - 1);
        isVerticalDot = !isLast && (shape & LIGATURE_OBLIQUE);
    }
    else {
        if (note->GetActualDur() == DURATION_1) shiftMultiplier = 3.5;
    }

    int y = note->GetDrawingY();
    int x = note->GetDrawingX();
    if (isVerticalDot) {
        x += note->GetDrawingRadius(m_doc, true);
        y += m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    }
    else {
        x += shiftMultiplier * note->GetDrawingRadius(m_doc, true);
        y -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    }

    this->DrawDotsPart(dc, x, y, 1, staff);

    return;
}

void View::DrawPlica(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);

    Plica *plica = vrv_cast<Plica *>(element);
    assert(plica);

    Note *note = vrv_cast<Note *>(plica->GetFirstAncestor(NOTE));
    assert(note);

    const bool isMensuralBlack = (staff->m_drawingNotationType == NOTATIONTYPE_mensural_black);
    const int stemWidth = m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);

    const bool isLonga = (note->GetActualDur() == DURATION_long);
    const bool up = (plica->GetDir() == STEMDIRECTION_basic_up);

    int shape = LIGATURE_DEFAULT;
    Point topLeft, bottomRight;
    int sides[4];
    this->CalcBrevisPoints(note, staff, &topLeft, &bottomRight, sides, shape, isMensuralBlack);

    int stem = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    stem *= (!isMensuralBlack) ? 7 : 5;
    int shortStem = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    shortStem *= (!isMensuralBlack) ? 3.5 : 2.5;

    dc->StartGraphic(plica, "", plica->GetID());

    if (isLonga) {
        if (up) {
            this->DrawFilledRectangle(dc, topLeft.x, sides[1], topLeft.x + stemWidth, sides[1] + shortStem);
            this->DrawFilledRectangle(dc, bottomRight.x, sides[1], bottomRight.x - stemWidth, sides[1] + stem);
        }
        else {
            this->DrawFilledRectangle(dc, topLeft.x, sides[0], topLeft.x + stemWidth, sides[0] - shortStem);
            this->DrawFilledRectangle(dc, bottomRight.x, sides[0], bottomRight.x - stemWidth, sides[0] - stem);
        }
    }
    // brevis
    else {
        if (up) {
            this->DrawFilledRectangle(dc, topLeft.x, sides[1], topLeft.x + stemWidth, sides[1] + stem);
            this->DrawFilledRectangle(dc, bottomRight.x, sides[1], bottomRight.x - stemWidth, sides[1] + shortStem);
        }
        else {
            this->DrawFilledRectangle(dc, topLeft.x, sides[0], topLeft.x + stemWidth, sides[0] - stem);
            this->DrawFilledRectangle(dc, bottomRight.x, sides[0], bottomRight.x - stemWidth, sides[0] - shortStem);
        }
    }

    dc->EndGraphic(plica, this);
}

void View::DrawProportFigures(DeviceContext *dc, int x, int y, int num, int numBase, Staff *staff)
{
    assert(dc);
    assert(staff);

    int ynum = 0, yden = 0;
    int textSize = staff->m_drawingStaffSize;
    std::u32string wtext;

    if (numBase) {
        ynum = y + m_doc->GetDrawingDoubleUnit(textSize);
        yden = y - m_doc->GetDrawingDoubleUnit(textSize);
    }
    else {
        ynum = y;
    }

    if (numBase > 9 || num > 9) {
        x += m_doc->GetDrawingUnit(textSize) * 2;
    }

    dc->SetFont(m_doc->GetDrawingSmuflFont(textSize, false));

    wtext = IntToTimeSigFigures(num);
    this->DrawSmuflString(dc, x, ynum, wtext, HORIZONTALALIGNMENT_center, textSize); // true = center

    if (numBase) {
        wtext = IntToTimeSigFigures(numBase);
        this->DrawSmuflString(dc, x, yden, wtext, HORIZONTALALIGNMENT_center, textSize); // true = center
    }

    dc->ResetFont();

    return;
}

void View::DrawProport(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(element);
    assert(layer);
    assert(staff);

    dc->StartGraphic(element, "", element->GetID());

    dc->EndGraphic(element, this);
}

void View::CalcBrevisPoints(
    Note *note, Staff *staff, Point *topLeft, Point *bottomRight, int sides[4], int shape, bool isMensuralBlack)
{
    assert(note);
    assert(staff);
    assert(topLeft);
    assert(bottomRight);

    int y = note->GetDrawingY();

    // Calculate size of the rectangle
    topLeft->x = note->GetDrawingX();
    const int width = 2 * note->GetDrawingRadius(m_doc, true);
    bottomRight->x = topLeft->x + width;

    double heightFactor = (isMensuralBlack) ? 0.8 : 1.0;
    topLeft->y = y + m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * heightFactor;
    bottomRight->y = y - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * heightFactor;

    sides[0] = topLeft->y;
    sides[1] = bottomRight->y;

    if (!isMensuralBlack) {
        // add sherif
        sides[0] += (int)m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 3;
        sides[1] -= (int)m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 3;
    }
    else if (shape & LIGATURE_OBLIQUE) {
        // shorten the sides to make sure they are note visible with oblique ligatures
        sides[0] -= (int)m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2;
        sides[1] += (int)m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2;
    }

    sides[2] = sides[0];
    sides[3] = sides[1];

    int stem = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    stem *= (!isMensuralBlack) ? 7 : 5;

    if (shape & LIGATURE_STEM_LEFT_UP) sides[0] = y + stem;
    if (shape & LIGATURE_STEM_LEFT_DOWN) sides[1] = y - stem;
    if (shape & LIGATURE_STEM_RIGHT_UP) sides[2] = y + stem;
    if (shape & LIGATURE_STEM_RIGHT_DOWN) sides[3] = y - stem;
}

void View::CalcObliquePoints(Note *note1, Note *note2, Staff *staff, Point points[4], int sides[4], int shape,
    bool isMensuralBlack, bool firstHalf, bool straight)
{
    assert(note1);
    assert(note2);
    assert(staff);

    const int stemWidth = m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
    const int noteDiff = note1->PitchDifferenceTo(note2);

    // Adjustment for end points according to the note diff
    const int yAdjust = noteDiff * stemWidth / 5;

    Point *topLeft = &points[0];
    Point *bottomLeft = &points[1];
    Point *topRight = &points[2];
    Point *bottomRight = &points[3];

    int sides1[4];
    this->CalcBrevisPoints(note1, staff, topLeft, bottomLeft, sides1, shape, isMensuralBlack);
    // Correct the x of bottomLeft
    bottomLeft->x = topLeft->x;
    // Copy the left sides
    sides[0] = sides1[0];
    sides[1] = sides1[1];

    int sides2[4];
    // add OBLIQUE shape to make sure sides are shortened in mensural black
    this->CalcBrevisPoints(note2, staff, topRight, bottomRight, sides2, LIGATURE_OBLIQUE, isMensuralBlack);
    // Correct the x of topRight;
    topRight->x = bottomRight->x;
    // Copy the right sides
    sides[2] = sides2[2];
    sides[3] = sides2[3];

    // With oblique it is best visually to move them up / down - more with (white) ligatures with serif
    // double adjustmentFactor = (isMensuralBlack) ? 2.5 : 1.8;
    double slope = 0.0;
    if (bottomRight->x != bottomLeft->x)
        slope = (double)(bottomRight->y - bottomLeft->y) / (double)(bottomRight->x - bottomLeft->x);

    int length = (bottomRight->x - bottomLeft->x) / 2;
    if (!straight) slope *= 0.85;

    if (firstHalf) {
        // make sure there is one pixel of overlap
        length += 1;
        bottomRight->x = bottomLeft->x + length;
        topRight->x = bottomRight->x;
        bottomRight->y = bottomLeft->y + (int)(length * slope);
        topRight->y = topLeft->y + (int)(length * slope);
        //
        topLeft->y += yAdjust;
        bottomLeft->y += yAdjust;
    }
    else {
        bottomLeft->x = bottomLeft->x + length;
        topLeft->x = bottomLeft->x;
        bottomLeft->y = bottomLeft->y + (int)(length * slope);
        topLeft->y = topLeft->y + (int)(length * slope);
        //
        topRight->y -= yAdjust;
        bottomRight->y -= yAdjust;
    }
}

data_STEMDIRECTION View::GetMensuralStemDir(Layer *layer, Note *note, int verticalCenter)
{
    // constants
    const data_DURATION drawingDur = note->GetDrawingDur();
    const int yNote = note->GetDrawingY();

    data_STEMDIRECTION layerStemDir;
    data_STEMDIRECTION stemDir = STEMDIRECTION_NONE;
    if (note->HasStemDir()) {
        stemDir = note->GetStemDir();
    }
    else if ((layerStemDir = layer->GetDrawingStemDir(note)) != STEMDIRECTION_NONE) {
        stemDir = layerStemDir;
    }
    else {
        if (drawingDur < DURATION_1) {
            stemDir = STEMDIRECTION_down;
        }
        else {
            stemDir = (yNote > verticalCenter) ? STEMDIRECTION_down : STEMDIRECTION_up;
        }
    }

    return stemDir;
}

} // namespace vrv
