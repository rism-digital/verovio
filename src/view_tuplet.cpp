/////////////////////////////////////////////////////////////////////////////
// Name:        view_tuplet.cpp
// Author:      Rodolfo Zitellini and Klaus Rettinghaus
// Created:     21/08/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "beam.h"
#include "devicecontext.h"
#include "doc.h"
#include "elementpart.h"
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
        tuplet->CalcDrawingBracketAndNumPos(m_doc->GetOptions()->m_tupletNumHead.GetValue());
    }

    dc->StartGraphic(element, "", element->GetID());

    // Draw the inner elements
    this->DrawLayerChildren(dc, tuplet, layer, staff, measure);

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

    dc->ResumeGraphic(tupletBracket, tupletBracket->GetID());

    const int unit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    const int lineWidth
        = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * m_options->m_tupletBracketThickness.GetValue();
    const int xLeft = tupletBracket->GetDrawingXLeft() + lineWidth / 2;
    const int xRight = tupletBracket->GetDrawingXRight() - lineWidth / 2;
    const int yLeft = tupletBracket->GetDrawingYLeft();
    const int yRight = tupletBracket->GetDrawingYRight();
    int bracketHeight = (tuplet->GetDrawingBracketPos() == STAFFREL_basic_above) ? -1 : 1;

    dc->SetPen(lineWidth, PEN_SOLID, 0, 0, LINECAP_BUTT, LINEJOIN_MITER);

    // Draw a bracket with a gap
    if (tupletBracket->GetAlignedNum() && tupletBracket->GetAlignedNum()->HasSelfBB()) {
        const int xNumLeft = tupletBracket->GetAlignedNum()->GetSelfLeft() - unit / 2;
        const int xNumRight = tupletBracket->GetAlignedNum()->GetSelfRight() + unit / 2;
        const double slope = (double)(yRight - yLeft) / (double)(xRight - xLeft);
        const int yNumLeft = yLeft + slope * (xNumLeft - xLeft);
        const int yNumRight = yRight - slope * (xRight - xNumRight);
        bracketHeight
            *= abs(tupletBracket->GetAlignedNum()->GetSelfTop() - tupletBracket->GetAlignedNum()->GetSelfBottom()) / 2;

        Point bracketLeft[3];
        bracketLeft[0] = { this->ToDeviceContextX(xLeft), this->ToDeviceContextY(yLeft + bracketHeight) };
        bracketLeft[1] = { this->ToDeviceContextX(xLeft), this->ToDeviceContextY(yLeft) };
        bracketLeft[2] = { this->ToDeviceContextX(xNumLeft), this->ToDeviceContextY(yNumLeft) };
        Point bracketRight[3];
        bracketRight[0] = { this->ToDeviceContextX(xRight), this->ToDeviceContextY(yRight + bracketHeight) };
        bracketRight[1] = { this->ToDeviceContextX(xRight), this->ToDeviceContextY(yRight) };
        bracketRight[2] = { this->ToDeviceContextX(xNumRight), this->ToDeviceContextY(yNumRight) };

        dc->DrawPolyline(3, bracketLeft);
        dc->DrawPolyline(3, bracketRight);
    }
    else {
        bracketHeight *= unit + lineWidth;

        Point bracket[4];
        bracket[0] = { this->ToDeviceContextX(xLeft), this->ToDeviceContextY(yLeft + bracketHeight) };
        bracket[1] = { this->ToDeviceContextX(xLeft), this->ToDeviceContextY(yLeft) };
        bracket[2] = { this->ToDeviceContextX(xRight), this->ToDeviceContextY(yRight) };
        bracket[3] = { this->ToDeviceContextX(xRight), this->ToDeviceContextY(yRight + bracketHeight) };

        dc->DrawPolyline(4, bracket);
    }

    dc->ResetPen();

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
    std::u32string notes;

    const bool drawingCueSize = tuplet->GetDrawingCueSize();
    const int glyphSize = staff->GetDrawingStaffNotationSize();
    dc->SetFont(m_doc->GetDrawingSmuflFont(glyphSize, drawingCueSize));
    notes = IntToTupletFigures((short int)tuplet->GetNum());
    if (tuplet->GetNumFormat() == tupletVis_NUMFORMAT_ratio) {
        if (tuplet->HasNumbase()) {
            notes.push_back(SMUFL_E88A_tupletColon);
            notes += IntToTupletFigures((short int)tuplet->GetNumbase());
        }
    }
    dc->GetSmuflTextExtent(notes, &extend);

    int x = tupletNum->GetDrawingXMid(m_doc);
    // since the number is slanted, move the center left
    x -= extend.m_width / 2;

    int y = tupletNum->GetDrawingYMid();
    // adjust the baseline (to be improved with slanted brackets
    y -= m_doc->GetGlyphHeight(notes.back(), glyphSize, drawingCueSize) / 2;

    dc->ResumeGraphic(tupletNum, tupletNum->GetID());

    this->DrawSmuflString(dc, x, y, notes, HORIZONTALALIGNMENT_left, glyphSize, drawingCueSize);

    dc->EndResumedGraphic(tupletNum, this);

    dc->ResetFont();

    return;
}

} // namespace vrv
