/////////////////////////////////////////////////////////////////////////////
// Name:        view_tuplet.cpp
// Author:      Rodolfo Zitellini and Klaus Rettinghaus
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
#include "elementpart.h"
#include "note.h"
#include "options.h"
#include "smufl.h"
#include "staff.h"
#include "tuplet.h"

namespace vrv {

int View::NestedTuplets(Object *object)
{
    assert(object);

    int tupletDepth = 1;

    for (int i = 0; i < object->GetChildCount(); ++i) {
        int tupletCount = 1;

        // check how many nested tuplets there are
        if ((object->GetChild(i))->Is(TUPLET)) {
            tupletCount += NestedTuplets(object->GetChild(i));
        }
        // and don't forget beams
        if ((object->GetChild(i))->Is(BEAM)) {
            tupletCount = NestedTuplets(object->GetChild(i));
        }

        tupletDepth = tupletCount > tupletDepth ? tupletCount : tupletDepth;
    }

    return tupletDepth;
}

void View::DrawTuplet(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Tuplet *tuplet = vrv_cast<Tuplet *>(element);
    assert(tuplet);

    // We do it here because we have no dedicated functor to do it (which would be an overkill)
    if (tuplet->GetDrawingBracketPos() == STAFFREL_basic_NONE) {
        tuplet->CalcDrawingBracketAndNumPos();
    }

    dc->StartGraphic(element, "", element->GetUuid());

    // Draw the inner elements
    DrawLayerChildren(dc, tuplet, layer, staff, measure);

    dc->EndGraphic(element, this);
}

void View::DrawTupletBracket(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    TupletBracket *tupletBracket = vrv_cast<TupletBracket *>(element);
    assert(tupletBracket);

    if (tupletBracket->GetBracketVisible() == BOOLEAN_false) {
        tupletBracket->SetEmptyBB();
        return;
    }

    Tuplet *tuplet = vrv_cast<Tuplet *>(tupletBracket->GetFirstAncestor(TUPLET));
    assert(tuplet);

    if (!tuplet->GetDrawingLeft() || !tuplet->GetDrawingRight()) {
        tupletBracket->SetEmptyBB();
        return;
    }

    // do not draw brackets on tremolos
    if (tuplet->GetChildCount(BTREM)) return;

    data_STAFFREL_basic position = tuplet->GetDrawingBracketPos();
    const int lineWidth
        = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * m_options->m_tupletBracketThickness.GetValue();

    dc->ResumeGraphic(tupletBracket, tupletBracket->GetUuid());

    const int xLeft = tuplet->GetDrawingLeft()->GetDrawingX() + tupletBracket->GetDrawingXRelLeft();
    const int xRight = tuplet->GetDrawingRight()->GetDrawingX() + tupletBracket->GetDrawingXRelRight();
    const int yLeft = tupletBracket->GetDrawingYLeft() - lineWidth / 2;
    const int yRight = tupletBracket->GetDrawingYRight() - lineWidth / 2;
    int bracketHeight;

    // Draw a bracked with a gap
    if (tupletBracket->GetAlignedNum() && tupletBracket->GetAlignedNum()->HasSelfBB()) {
        const int xNumLeft
            = tupletBracket->GetAlignedNum()->GetSelfLeft() - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
        const int xNumRight
            = tupletBracket->GetAlignedNum()->GetSelfRight() + m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
        const double slope = (double)(yRight - yLeft) / (double)(xRight - xLeft);
        const int yNumLeft = yLeft + slope * (xNumLeft - xLeft);
        DrawObliquePolygon(dc, xLeft, yLeft, xNumLeft, yNumLeft, lineWidth);
        const int yNumRight = yRight - slope * (xRight - xNumRight);
        DrawObliquePolygon(dc, xNumRight, yNumRight, xRight, yRight, lineWidth);
        bracketHeight
            = abs(tupletBracket->GetAlignedNum()->GetSelfTop() - tupletBracket->GetAlignedNum()->GetSelfBottom()) / 2;
    }
    else {
        DrawObliquePolygon(dc, xLeft, yLeft, xRight, yRight, lineWidth);
        // HARDCODED
        bracketHeight = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 6 / 5;
    }

    if (position == STAFFREL_basic_above) {
        bracketHeight *= -1;
    }

    DrawFilledRectangle(dc, xLeft, yLeft, xLeft + lineWidth, yLeft + bracketHeight);
    DrawFilledRectangle(dc, xRight, yRight, xRight - lineWidth, yRight + bracketHeight);

    dc->EndResumedGraphic(tupletBracket, this);

    return;
}

void View::DrawTupletNum(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    TupletNum *tupletNum = vrv_cast<TupletNum *>(element);
    assert(tupletNum);

    Tuplet *tuplet = vrv_cast<Tuplet *>(tupletNum->GetFirstAncestor(TUPLET));
    assert(tuplet);

    if (!tuplet->HasNum() || (tuplet->GetNumVisible() == BOOLEAN_false)) {
        tupletNum->SetEmptyBB();
        return;
    }

    if (!tuplet->GetDrawingLeft() || !tuplet->GetDrawingRight()) {
        tupletNum->SetEmptyBB();
        return;
    }

    TextExtend extend;
    std::wstring notes;

    const bool drawingCueSize = tuplet->GetDrawingCueSize();
    dc->SetFont(m_doc->GetDrawingSmuflFont(staff->m_drawingStaffSize, drawingCueSize));
    notes = IntToTupletFigures((short int)tuplet->GetNum());
    if (tuplet->GetNumFormat() == tupletVis_NUMFORMAT_ratio) {
        notes.push_back(SMUFL_E88A_tupletColon);
        notes += IntToTupletFigures((short int)tuplet->GetNumbase());
    }
    dc->GetSmuflTextExtent(notes, &extend);

    int x = tupletNum->GetDrawingXMid(m_doc);
    // since the number is slanted, move the center left
    x -= extend.m_width / 2;

    int y = tupletNum->GetDrawingYMid();
    // adjust the baseline (to be improved with slanted brackets
    y -= m_doc->GetGlyphHeight(notes.back(), staff->m_drawingStaffSize, drawingCueSize) / 2;

    dc->ResumeGraphic(tupletNum, tupletNum->GetUuid());

    DrawSmuflString(dc, x, y, notes, HORIZONTALALIGNMENT_left, staff->m_drawingStaffSize, drawingCueSize);

    dc->EndResumedGraphic(tupletNum, this);

    dc->ResetFont();

    return;
}

} // namespace vrv
