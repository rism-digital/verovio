/////////////////////////////////////////////////////////////////////////////
// Name:        view_mensural.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <assert.h>

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
#include "rest.h"
#include "smufl.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

int View::s_drawingLigX[2], View::s_drawingLigY[2]; // to keep coords. of ligatures
bool View::s_drawingLigObliqua = false; // mark the first pass for an oblique

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

    int yNote = element->GetDrawingY();
    int xNote = element->GetDrawingX();
    int drawingDur = note->GetDrawingDur();
    int radius = note->GetDrawingRadius(m_doc);
    int staffY = staff->GetDrawingY();
    int verticalCenter = 0;
    bool mensural_black = (staff->m_drawingNotationType == NOTATIONTYPE_mensural_black);

    /************** Stem/notehead direction: **************/

    data_STEMDIRECTION layerStemDir;
    data_STEMDIRECTION stemDir = STEMDIRECTION_NONE;

    verticalCenter = staffY - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * 2;
    if (note->HasStemDir()) {
        stemDir = note->GetStemDir();
    }
    else if ((layerStemDir = layer->GetDrawingStemDir(note)) != STEMDIRECTION_NONE) {
        stemDir = layerStemDir;
    }
    else {
        if (drawingDur < DUR_1) {
            stemDir = STEMDIRECTION_down;
        }
        else {
            stemDir = (yNote > verticalCenter) ? STEMDIRECTION_down : STEMDIRECTION_up;
        }
    }

    /************** Noteheads: **************/

    // Ligature, maxima,longa, and brevis
    if (note->IsInLigature()) {
        DrawLigatureNote(dc, element, layer, staff);
    }
    else if (drawingDur < DUR_1) {
        DrawMaximaToBrevis(dc, yNote, element, layer, staff);
    }
    // Semibrevis and shorter
    else {
        wchar_t code = note->GetMensuralSmuflNoteHead();
        dc->StartCustomGraphic("notehead");
        DrawSmuflCode(dc, xNote, yNote, code, staff->m_drawingStaffSize, false);
        dc->EndCustomGraphic();
        // For semibrevis with stem in black notation, encoded with an explicit stem direction
        if (((drawingDur > DUR_1) || ((note->GetStemDir() != STEMDIRECTION_NONE) && mensural_black))
            && note->GetStemVisible() != BOOLEAN_false) {
            DrawMensuralStem(dc, note, staff, stemDir, radius, xNote, yNote);
        }
    }

    /************ Draw children (verse / syl) ************/

    DrawLayerChildren(dc, note, layer, staff, measure);
}

void View::DrawMensuralRest(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    wchar_t charCode;

    Rest *rest = vrv_cast<Rest *>(element);
    assert(rest);

    bool drawingCueSize = rest->GetDrawingCueSize();
    int drawingDur = rest->GetActualDur();
    int x = element->GetDrawingX();
    int y = element->GetDrawingY();

    switch (drawingDur) {
        case DUR_MX: charCode = SMUFL_E9F0_mensuralRestMaxima; break;
        case DUR_LG: charCode = SMUFL_E9F2_mensuralRestLongaImperfecta; break;
        case DUR_BR: charCode = SMUFL_E9F3_mensuralRestBrevis; break;
        case DUR_1: charCode = SMUFL_E9F4_mensuralRestSemibrevis; break;
        case DUR_2: charCode = SMUFL_E9F5_mensuralRestMinima; break;
        case DUR_4: charCode = SMUFL_E9F6_mensuralRestSemiminima; break;
        case DUR_8: charCode = SMUFL_E9F7_mensuralRestFusa; break;
        case DUR_16: charCode = SMUFL_E9F8_mensuralRestSemifusa; break;
        default: charCode = 0; // This should never happen
    }
    DrawSmuflCode(dc, x, y, charCode, staff->m_drawingStaffSize, drawingCueSize);
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

    if (!mensur->HasSign()) {
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

    dc->StartGraphic(element, "", element->GetUuid());

    DrawSmuflCode(dc, x, y, code, staff->m_drawingStaffSize, false);

    x += perfectRadius;
    // only one slash supported
    if (mensur->HasSlash()) {
        DrawSmuflCode(dc,
            x - m_doc->GetGlyphWidth(SMUFL_E925_mensuralProlationCombiningStroke, staff->m_drawingStaffSize, false) / 2,
            y, SMUFL_E925_mensuralProlationCombiningStroke, staff->m_drawingStaffSize, false);
    }
    if (mensur->GetDot() == BOOLEAN_true) {
        DrawSmuflCode(dc,
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
        DrawProportFigures(dc, x, y, mensur->GetNum(), numbase, staff);
    }

    dc->EndGraphic(element, this);
} // namespace vrv

/* This function draws any flags as well as the stem. */

void View::DrawMensuralStem(
    DeviceContext *dc, Note *note, Staff *staff, data_STEMDIRECTION dir, int radius, int xn, int originY, int heightY)
{
    assert(note);

    int staffSize = staff->m_drawingStaffSize;
    int staffY = staff->GetDrawingY();
    int baseStem, totalFlagStemHeight, flagStemHeight, nbFlags;
    int drawingDur = note->GetDrawingDur();
    // Cue size is currently disabled
    bool drawingCueSize = false;
    int verticalCenter = staffY - m_doc->GetDrawingDoubleUnit(staffSize) * 2;
    bool mensural_black = (staff->m_drawingNotationType == NOTATIONTYPE_mensural_black);

    baseStem = m_doc->GetDrawingUnit(staffSize) * STANDARD_STEMLENGTH;
    flagStemHeight = m_doc->GetDrawingDoubleUnit(staffSize);
    if (drawingCueSize) {
        baseStem = m_doc->GetCueSize(baseStem);
        flagStemHeight = m_doc->GetCueSize(flagStemHeight);
    }

    nbFlags = (mensural_black ? drawingDur - DUR_2 : drawingDur - DUR_4);
    totalFlagStemHeight = flagStemHeight * (nbFlags * 2 - 1) / 2;

    /* SMuFL provides combining stem-and-flag characters with one and two flags, but
        at the moment, I'm using only the one flag ones, partly out of concern for
        possible three-flag notes. */

    /* In black notation, the semiminima gets one flag; in white notation, it gets none.
        In both cases, as in CWMN, each shorter duration gets one additional flag. */

    if (dir == STEMDIRECTION_down) {
        // flip all lengths. Exception: in mensural notation, the stem will never be at
        //   left, so leave radius as is.
        baseStem = -baseStem;
        totalFlagStemHeight = -totalFlagStemHeight;
        heightY = -heightY;
    }

    // If we have flags, add them to the height.
    int y1 = originY;
    int y2 = ((nbFlags > 0) ? (y1 + baseStem + totalFlagStemHeight) : (y1 + baseStem)) + heightY;
    int x2 = xn + radius;

    if ((dir == STEMDIRECTION_up) && (y2 < verticalCenter)) {
        y2 = verticalCenter;
    }
    else if ((dir == STEMDIRECTION_down) && (y2 > verticalCenter)) {
        y2 = verticalCenter;
    }

    // shorten the stem at its connection with the note head
    // this will not work if the pseudo size is changed
    int shortening = 0.9 * m_doc->GetDrawingUnit(staffSize);

    // LogDebug("DrawMensuralStem: drawingDur=%d mensural_black=%d nbFlags=%d", drawingDur, mensural_black, nbFlags);
    int stemY1 = (dir == STEMDIRECTION_up) ? y1 + shortening : y1 - shortening;
    int stemY2 = y2;
    if (nbFlags > 0) {
        // if we have flags, shorten the stem to make sure we have a nice overlap with the flag glyph
        int shortener
            = (drawingCueSize) ? m_doc->GetCueSize(m_doc->GetDrawingUnit(staffSize)) : m_doc->GetDrawingUnit(staffSize);
        stemY2 = (dir == STEMDIRECTION_up) ? y2 - shortener : y2 + shortener;
    }

    int halfStemWidth = m_doc->GetDrawingStemWidth(staffSize) / 2;
    // draw the stems and the flags

    dc->StartCustomGraphic("stem");
    if (dir == STEMDIRECTION_up) {

        if (nbFlags > 0) {
            for (int i = 0; i < nbFlags; ++i) {
                DrawSmuflCode(dc, x2 - halfStemWidth, stemY1 - i * flagStemHeight,
                    SMUFL_E949_mensuralCombStemUpFlagSemiminima, staff->m_drawingStaffSize, drawingCueSize);
            }
        }
        else {
            DrawFilledRectangle(dc, x2 - halfStemWidth, stemY1, x2 + halfStemWidth, stemY2);
        }
    }
    else {
        if (nbFlags > 0) {
            for (int i = 0; i < nbFlags; ++i) {
                DrawSmuflCode(dc, x2 - halfStemWidth, stemY1 + i * flagStemHeight,
                    SMUFL_E94A_mensuralCombStemDownFlagSemiminima, staff->m_drawingStaffSize, drawingCueSize);
            }
        }
        else {
            DrawFilledRectangle(dc, x2 - halfStemWidth, stemY1, x2 + halfStemWidth, stemY2);
        }
    }
    dc->EndCustomGraphic();

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

    bool isMensuralBlack = (staff->m_drawingNotationType == NOTATIONTYPE_mensural_black);
    bool fillNotehead = (isMensuralBlack || note->GetColored()) && !(isMensuralBlack && note->GetColored());

    int stemWidth = m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
    int strokeWidth = 2.8 * stemWidth;

    int shape = LIGATURE_DEFAULT;
    if (note->GetActualDur() != DUR_BR) {
        bool up = false;
        // Mensural notes have no Stem child - rely on the MEI @stem.dir
        if (note->GetStemDir() != STEMDIRECTION_NONE) {
            up = (note->GetStemDir() == STEMDIRECTION_up);
        }
        else if (staff->m_drawingNotationType == NOTATIONTYPE_NONE
            || staff->m_drawingNotationType == NOTATIONTYPE_cmn) {
            up = (note->GetDrawingStemDir() == STEMDIRECTION_up);
        }
        shape = (up) ? LIGATURE_STEM_RIGHT_UP : LIGATURE_STEM_RIGHT_DOWN;
    }

    Point topLeft, bottomRight;
    int sides[4];
    this->CalcBrevisPoints(note, staff, &topLeft, &bottomRight, sides, shape, isMensuralBlack);

    dc->StartCustomGraphic("notehead");

    if (!fillNotehead) {
        // double the bases of rectangles
        DrawObliquePolygon(dc, topLeft.x + stemWidth, topLeft.y, bottomRight.x - stemWidth, topLeft.y, -strokeWidth);
        DrawObliquePolygon(
            dc, topLeft.x + stemWidth, bottomRight.y, bottomRight.x - stemWidth, bottomRight.y, strokeWidth);
    }
    else {
        DrawFilledRectangle(dc, topLeft.x + stemWidth, topLeft.y, bottomRight.x - stemWidth, bottomRight.y);
    }

    if (note->FindDescendantByType(PLICA)) {
        // Right side is a stem - end the notehead first
        dc->EndCustomGraphic();
        return;
    }

    // serifs and / or stem
    DrawFilledRectangle(dc, topLeft.x, sides[0], topLeft.x + stemWidth, sides[1]);

    if (note->GetActualDur() != DUR_BR) {
        // Right side is a stem - end the notehead first
        dc->EndCustomGraphic();
        dc->StartCustomGraphic("stem");
        DrawFilledRectangle(dc, bottomRight.x - stemWidth, sides[2], bottomRight.x, sides[3]);
        dc->EndCustomGraphic();
    }
    else {
        // Right side is a serif
        DrawFilledRectangle(dc, bottomRight.x - stemWidth, sides[2], bottomRight.x, sides[3]);
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

    dc->StartGraphic(ligature, "", ligature->GetUuid());

    // Draw children (notes)
    DrawLayerChildren(dc, ligature, layer, staff, measure);

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
            CalcObliquePoints(note, nextNote, staff, points, sides, shape, isMensuralBlack, true);
        }
        // Second half of the oblique - checking the prevNote is there just in case, but is should
        else if ((prevShape & LIGATURE_OBLIQUE) && prevNote) {
            CalcObliquePoints(prevNote, note, staff, points, sides, prevShape, isMensuralBlack, false);
        }
        else {
            assert(false);
        }
    }

    if (!fillNotehead) {
        // double the bases of rectangles
        DrawObliquePolygon(dc, topLeft->x, topLeft->y, topRight->x, topRight->y, -strokeWidth);
        DrawObliquePolygon(dc, bottomLeft->x, bottomLeft->y, bottomRight->x, bottomRight->y, strokeWidth);
    }
    else {
        DrawObliquePolygon(dc, topLeft->x, topLeft->y, topRight->x, topRight->y, bottomLeft->y - topLeft->y);
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
            CalcBrevisPoints(prevNote, staff, &prevTopLeft, &prevBottomRight, prevSides, prevShape, isMensuralBlack);
            if (!stackedEnd) {
                sideTop = std::max(sides[0], prevSides[2]);
                sideBottom = std::min(sides[1], prevSides[3]);
            }
            else {
                // Stacked end - simply use the bottom right [3] note since the interval is going up anyway
                sides[3] = prevSides[3];
            }
        }
        DrawFilledRoundedRectangle(dc, topLeft->x, sideTop, topLeft->x + stemWidth, sideBottom, stemWidth / 3);
    }

    if (!nextNote) {
        DrawFilledRoundedRectangle(dc, bottomRight->x - stemWidth, sides[2], bottomRight->x, sides[3], stemWidth / 3);
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

    Note *note = dot->m_drawingNote;
    assert(dot->m_drawingNote);

    Ligature *ligature = vrv_cast<Ligature *>(note->GetFirstAncestor(LIGATURE));
    assert(ligature);

    int position = ligature->GetListIndex(note);
    assert(position != -1);
    int shape = ligature->m_drawingShapes.at(position);
    bool isLast = (position == (int)ligature->m_drawingShapes.size() - 1);

    int y = note->GetDrawingY();
    int x = note->GetDrawingX();
    if (!isLast && (shape & LIGATURE_OBLIQUE)) {
        x += note->GetDrawingRadius(m_doc, true);
        y += m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    }
    else {
        x += 3 * note->GetDrawingRadius(m_doc, true);
        y -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    }

    DrawDotsPart(dc, x, y, 1, staff);

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

    bool isMensuralBlack = (staff->m_drawingNotationType == NOTATIONTYPE_mensural_black);
    int stemWidth = m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);

    bool isLonga = (note->GetActualDur() == DUR_LG);
    bool up = (plica->GetDir() == STEMDIRECTION_basic_up);

    int shape = LIGATURE_DEFAULT;
    Point topLeft, bottomRight;
    int sides[4];
    this->CalcBrevisPoints(note, staff, &topLeft, &bottomRight, sides, shape, isMensuralBlack);

    int stem = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    stem *= (!isMensuralBlack) ? 7 : 5;
    int shortStem = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    shortStem *= (!isMensuralBlack) ? 3.5 : 2.5;

    dc->StartGraphic(plica, "", plica->GetUuid());

    if (isLonga) {
        if (up) {
            DrawFilledRectangle(dc, topLeft.x, sides[1], topLeft.x + stemWidth, sides[1] + shortStem);
            DrawFilledRectangle(dc, bottomRight.x, sides[1], bottomRight.x - stemWidth, sides[1] + stem);
        }
        else {
            DrawFilledRectangle(dc, topLeft.x, sides[0], topLeft.x + stemWidth, sides[0] - shortStem);
            DrawFilledRectangle(dc, bottomRight.x, sides[0], bottomRight.x - stemWidth, sides[0] - stem);
        }
    }
    // brevis
    else {
        if (up) {
            DrawFilledRectangle(dc, topLeft.x, sides[1], topLeft.x + stemWidth, sides[1] + stem);
            DrawFilledRectangle(dc, bottomRight.x, sides[1], bottomRight.x - stemWidth, sides[1] + shortStem);
        }
        else {
            DrawFilledRectangle(dc, topLeft.x, sides[0], topLeft.x + stemWidth, sides[0] - stem);
            DrawFilledRectangle(dc, bottomRight.x, sides[0], bottomRight.x - stemWidth, sides[0] - shortStem);
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
    std::wstring wtext;

    if (numBase) {
        ynum = y + m_doc->GetDrawingDoubleUnit(textSize);
        yden = y - m_doc->GetDrawingDoubleUnit(textSize);
    }
    else
        ynum = y;

    if (numBase > 9 || num > 9) {
        x += m_doc->GetDrawingUnit(textSize) * 2;
    }

    dc->SetFont(m_doc->GetDrawingSmuflFont(textSize, false));

    wtext = IntToTimeSigFigures(num);
    DrawSmuflString(dc, x, ynum, wtext, HORIZONTALALIGNMENT_center, textSize); // true = center

    if (numBase) {
        wtext = IntToTimeSigFigures(numBase);
        DrawSmuflString(dc, x, yden, wtext, HORIZONTALALIGNMENT_center, textSize); // true = center
    }

    dc->ResetFont();

    return;
}

void View::DrawProport(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(layer);
    assert(staff);
    assert(dynamic_cast<Proport *>(element)); // Element must be a Proport"

    int x1, x2, y1, y2;

    Proport *proport = dynamic_cast<Proport *>(element);

    dc->StartGraphic(element, "", element->GetUuid());

    int y = staff->GetDrawingY() - (m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 4);
    int x = element->GetDrawingX();

    x1 = x + 120;
    x2 = x1 + 150; // ??TEST: JUST DRAW AN ARBITRARY RECTANGLE
    y1 = y;
    y2 = y + 50 + (50 * proport->GetNum());
    // DrawFilledRectangle(dc,x1,y1,x2,y2);
    DrawPartFilledRectangle(dc, x1, y1, x2, y2, 0);

    if (proport->HasNum()) {
        x = element->GetDrawingX();
        // if (proport->GetSign() || proport->HasTempus())           // ??WHAT SHOULD THIS BE?
        {
            x += m_doc->GetDrawingUnit(staff->m_drawingStaffSize)
                * 5; // step forward because we have a sign or a meter symbol
        }
        int numbase = proport->HasNumbase() ? proport->GetNumbase() : 0;
        DrawProportFigures(dc, x,
            staff->GetDrawingY() - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * (staff->m_drawingLines - 1),
            proport->GetNum(), numbase, staff);
    }

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
    int width = 2 * note->GetDrawingRadius(m_doc, true);
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
    bool isMensuralBlack, bool firstHalf)
{
    assert(note1);
    assert(note2);
    assert(staff);

    int stemWidth = m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);

    Point *topLeft = &points[0];
    Point *bottomLeft = &points[1];
    Point *topRight = &points[2];
    Point *bottomRight = &points[3];

    int sides1[4];
    CalcBrevisPoints(note1, staff, topLeft, bottomLeft, sides1, shape, isMensuralBlack);
    // Correct the x of bottomLeft
    bottomLeft->x = topLeft->x;
    // Copy the left sides
    sides[0] = sides1[0];
    sides[1] = sides1[1];

    int sides2[4];
    // add OBLIQUE shape to make sure sides are shortened in mensural black
    CalcBrevisPoints(note2, staff, topRight, bottomRight, sides2, LIGATURE_OBLIQUE, isMensuralBlack);
    // Correct the x of topRight;
    topRight->x = bottomRight->x;
    // Copy the right sides
    sides[2] = sides2[2];
    sides[3] = sides2[3];

    // With oblique it is best visually to move them up / down - more with (white) ligatures with serif
    double adjustmentFactor = (isMensuralBlack) ? 0.5 : 1.8;
    double slope = 0.0;
    if (bottomRight->x != bottomLeft->x)
        slope = (double)(bottomRight->y - bottomLeft->y) / (double)(bottomRight->x - bottomLeft->x);
    int adjustment = (int)(slope * stemWidth) * adjustmentFactor;
    topLeft->y -= adjustment;
    bottomLeft->y -= adjustment;
    topRight->y += adjustment;
    bottomRight->y += adjustment;

    slope = 0.0;
    // recalculate slope after adjustment
    if (bottomRight->x != bottomLeft->x)
        slope = (double)(bottomRight->y - bottomLeft->y) / (double)(bottomRight->x - bottomLeft->x);
    int length = (bottomRight->x - bottomLeft->x) / 2;

    if (firstHalf) {
        // make sure there are some pixels of overlap
        length += 10;
        bottomRight->x = bottomLeft->x + length;
        topRight->x = bottomRight->x;
        bottomRight->y = bottomLeft->y + (int)(length * slope);
        topRight->y = topLeft->y + (int)(length * slope);
    }
    else {
        bottomLeft->x = bottomLeft->x + length;
        topLeft->x = bottomLeft->x;
        bottomLeft->y = bottomLeft->y + (int)(length * slope);
        topLeft->y = topLeft->y + (int)(length * slope);
    }
}

} // namespace vrv
